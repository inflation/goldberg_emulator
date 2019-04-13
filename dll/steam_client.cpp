/* Copyright (C) 2019 Mr Goldberg
   This file is part of the Goldberg Emulator

   The Goldberg Emulator is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   The Goldberg Emulator is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the Goldberg Emulator; if not, see
   <http://www.gnu.org/licenses/>.  */

#include "steam_client.h"

#include <fstream>

static void network_thread(Networking *network)
{
    PRINT_DEBUG("network thread starting\n");
    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        global_mutex.lock();
        if (!network->isAlive()) {
            global_mutex.unlock();
            //delete network;
            PRINT_DEBUG("network thread exit\n");
            return;
        }

        PRINT_DEBUG("network thread run\n");
        network->Run();
        global_mutex.unlock();
    }
}

Steam_Client::Steam_Client()
{
    std::string program_path = Local_Storage::get_program_path(), save_path = Local_Storage::get_user_appdata_path();;

    PRINT_DEBUG("Current Path %s save_path: %s\n", program_path.c_str(), save_path.c_str());

    char array[10] = {};
    array[0] = '0';
    Local_Storage::get_file_data(program_path + "steam_appid.txt", array, sizeof(array) - 1);
    uint32 appid = 0;
    try {
        appid = std::stoi(array);
    } catch (...) {}
    if (!appid) {
        memset(array, 0, sizeof(array));
        array[0] = '0';
        Local_Storage::get_file_data("steam_appid.txt", array, sizeof(array) - 1);
        try {
            appid = std::stoi(array);
        } catch (...) {}
        if (!appid) {
            memset(array, 0, sizeof(array));
            array[0] = '0';
            Local_Storage::get_file_data(Local_Storage::get_game_settings_path() + "steam_appid.txt", array, sizeof(array) - 1);
            try {
                appid = std::stoi(array);
            } catch (...) {}
        }
    }

    if (!appid) {
        std::string str_appid = get_env_variable("SteamAppId");
        std::string str_gameid = get_env_variable("SteamGameId");
        PRINT_DEBUG("str_appid %s str_gameid: %s\n", str_appid.c_str(), str_gameid.c_str());
        uint32 appid_env = 0;
        uint32 gameid_env = 0;

        if (str_appid.size() > 0) {
            try {
                appid_env = std::stoul(str_appid);
            } catch (...) {
                appid_env = 0;
            }
        }

        if (str_gameid.size() > 0) {
            try {
                gameid_env = std::stoul(str_gameid);
            } catch (...) {
                gameid_env = 0;
            }
        }

        PRINT_DEBUG("appid_env %u gameid_env: %u\n", appid_env, gameid_env);
        if (appid_env) {
            appid = appid_env;
        }

        if (gameid_env) {
            appid = gameid_env;
        }
    }

    {
        char array[33] = {};
        if (Local_Storage::get_file_data(program_path + "local_save.txt", array, sizeof(array) - 1) != -1) {
            save_path = program_path + Settings::sanitize(array);
        }
    }

    PRINT_DEBUG("Set save_path: %s\n", save_path.c_str());
    local_storage = new Local_Storage(save_path);
    local_storage->setAppId(appid);

    char array_port[10] = {};
    array_port[0] = '0';
    local_storage->get_data_settings("listen_port.txt", array_port, sizeof(array_port) - 1);
    uint16 port = std::stoi(array_port);
    if (port == 0) {
        port = DEFAULT_PORT;
        snprintf(array_port, sizeof(array_port), "%hu", port);
        local_storage->store_data_settings("listen_port.txt", array_port, strlen(array_port));
    }

    char name[32] = {};

    if (local_storage->get_data_settings("account_name.txt", name, sizeof(name) - 1) <= 0) {
        strcpy(name, DEFAULT_NAME);
        local_storage->store_data_settings("account_name.txt", name, strlen(name));
    }

    char language[32] = {};
    if (local_storage->get_data_settings("language.txt", language, sizeof(language) - 1) <= 0) {
        strcpy(language, DEFAULT_LANGUAGE);
        local_storage->store_data_settings("language.txt", language, strlen(language));
    }

    char array_steam_id[32] = {};
    CSteamID user_id;
    uint64 steam_id = 0;
    bool generate_new = false;
    //try to load steam id from game specific settings folder first
    if (local_storage->get_data(SETTINGS_STORAGE_FOLDER, "user_steam_id.txt", array_steam_id, sizeof(array_steam_id) - 1) > 0) {
        user_id = CSteamID((uint64)std::atoll(array_steam_id));
        if (!user_id.IsValid()) {
            generate_new = true;
        }
    } else {
        generate_new = true;
    }

    if (generate_new) {
        generate_new = false;
        if (local_storage->get_data_settings("user_steam_id.txt", array_steam_id, sizeof(array_steam_id) - 1) > 0) {
            user_id = CSteamID((uint64)std::atoll(array_steam_id));
            if (!user_id.IsValid()) {
                generate_new = true;
            }
        } else {
            generate_new = true;
        }
    }

    if (generate_new) {
        user_id = generate_steam_id_user();
        uint64 steam_id = user_id.ConvertToUint64();
        char temp_text[32] = {};
        snprintf(temp_text, sizeof(temp_text), "%llu", steam_id);
        local_storage->store_data_settings("user_steam_id.txt", temp_text, strlen(temp_text));
    }

    settings_client = new Settings(user_id, CGameID(appid), name, language);
    settings_server = new Settings(generate_steam_id_server(), CGameID(appid), name, language);

    {
        std::string dlc_config_path = Local_Storage::get_game_settings_path() + "DLC.txt";
        std::ifstream input( dlc_config_path );
        if (input.is_open()) {
            settings_client->unlockAllDLC(false);
            PRINT_DEBUG("Locking all DLC\n");

            for( std::string line; getline( input, line ); ) {
                if (!line.empty() && line[line.length()-1] == '\n') {
                    line.erase(line.length()-1);
                }

                if (!line.empty() && line[line.length()-1] == '\r') {
                    line.erase(line.length()-1);
                }

                std::size_t deliminator = line.find("=");
                if (deliminator != 0 && deliminator != std::string::npos && deliminator != line.size()) {
                    AppId_t appid = stol(line.substr(0, deliminator));
                    std::string name = line.substr(deliminator + 1);
                    bool available = true;

                    if (appid) {
                        PRINT_DEBUG("Adding DLC: %u|%s| %u\n", appid, name.c_str(), available);
                        settings_client->addDLC(appid, name, available);
                        settings_server->addDLC(appid, name, available);
                    }
                }
            }
        } else {
            //unlock all DLC
            PRINT_DEBUG("Unlocking all DLC\n");
            settings_client->unlockAllDLC(true);
            settings_server->unlockAllDLC(true);
        }
    }

    {
        std::string mod_path = Local_Storage::get_game_settings_path() + "mods";
        std::vector<std::string> paths = Local_Storage::get_filenames_path(mod_path);
        for (auto & p: paths) {
            PRINT_DEBUG("mod directory %s\n", p.c_str());
            try {
                PublishedFileId_t id = std::stoull(p);
                settings_client->addMod(id, p, mod_path + PATH_SEPARATOR + p);
                settings_server->addMod(id, p, mod_path + PATH_SEPARATOR + p);
            } catch (...) {}
        }
    }

    network = new Networking(settings_server->get_local_steam_id(), appid, port);

    callback_results_client = new SteamCallResults();
    callback_results_server = new SteamCallResults();
    callbacks_client = new SteamCallBacks(callback_results_client);
    callbacks_server = new SteamCallBacks(callback_results_server);
    run_every_runcb = new RunEveryRunCB();

    PRINT_DEBUG("steam client init: id: %llu server id: %llu appid: %u port: %u \n", user_id.ConvertToUint64(), settings_server->get_local_steam_id().ConvertToUint64(), appid, port);

    steam_user = new Steam_User(settings_client, local_storage, network, callback_results_client, callbacks_client);
    steam_friends = new Steam_Friends(settings_client, network, callback_results_client, callbacks_client, run_every_runcb);
    steam_utils = new Steam_Utils(settings_client, callback_results_client);
    steam_matchmaking = new Steam_Matchmaking(settings_client, network, callback_results_client, callbacks_client, run_every_runcb);
    steam_matchmaking_servers = new Steam_Matchmaking_Servers(settings_client, network);
    steam_user_stats = new Steam_User_Stats(settings_client, local_storage, callback_results_client, callbacks_client);
    steam_apps = new Steam_Apps(settings_client, callback_results_client);
    steam_networking = new Steam_Networking(settings_client, network, callbacks_client, run_every_runcb);
    steam_remote_storage = new Steam_Remote_Storage(local_storage, callback_results_client);
    steam_screenshots = new Steam_Screenshots();
    steam_http = new Steam_HTTP(settings_client, network, callback_results_client, callbacks_client);
    steam_controller = new Steam_Controller();
    steam_ugc = new Steam_UGC(settings_client, callback_results_client, callbacks_client);
    steam_applist = new Steam_Applist();
    steam_music = new Steam_Music(callbacks_client);
    steam_musicremote = new Steam_MusicRemote();
    steam_HTMLsurface = new Steam_HTMLsurface(settings_client, network, callback_results_client, callbacks_client);
    steam_inventory = new Steam_Inventory(settings_client, callback_results_client, callbacks_client);
    steam_video = new Steam_Video();
    steam_parental = new Steam_Parental();
    steam_networking_sockets = new Steam_Networking_Sockets(settings_client, network, callback_results_client, callbacks_client, run_every_runcb);
    steam_networking_sockets_serialized = new Steam_Networking_Sockets_Serialized(settings_client, network, callback_results_client, callbacks_client, run_every_runcb);
    steam_game_coordinator = new Steam_Game_Coordinator(settings_client, network, callback_results_client, callbacks_client, run_every_runcb);
    steam_networking_utils = new Steam_Networking_Utils(settings_client, network, callback_results_client, callbacks_client, run_every_runcb);
    steam_unified_messages = new Steam_Unified_Messages(settings_client, network, callback_results_client, callbacks_client, run_every_runcb);
    steam_game_search = new Steam_Game_Search(settings_client, network, callback_results_client, callbacks_client, run_every_runcb);
    steam_parties = new Steam_Parties(settings_client, network, callback_results_client, callbacks_client, run_every_runcb);

    PRINT_DEBUG("client init gameserver\n");
    steam_gameserver = new Steam_GameServer(settings_server, network, callbacks_server);
    steam_gameserver_utils = new Steam_Utils(settings_server, callback_results_server);
    steam_gameserverstats = new Steam_GameServerStats(settings_server, network, callback_results_server, callbacks_server);
    steam_gameserver_networking = new Steam_Networking(settings_server, network, callbacks_server, run_every_runcb);
    steam_gameserver_http = new Steam_HTTP(settings_server, network, callback_results_server, callbacks_server);
    steam_gameserver_inventory = new Steam_Inventory(settings_server, callback_results_server, callbacks_server);
    steam_gameserver_ugc = new Steam_UGC(settings_server, callback_results_server, callbacks_server);
    steam_gameserver_apps = new Steam_Apps(settings_server, callback_results_server);
    steam_gameserver_networking_sockets = new Steam_Networking_Sockets(settings_server, network, callback_results_server, callbacks_server, run_every_runcb);
    steam_gameserver_networking_sockets_serialized = new Steam_Networking_Sockets_Serialized(settings_server, network, callback_results_server, callbacks_server, run_every_runcb);
    steam_gameserver_game_coordinator = new Steam_Game_Coordinator(settings_server, network, callback_results_server, callbacks_server, run_every_runcb);
    steam_masterserver_updater = new Steam_Masterserver_Updater(settings_server, network, callback_results_server, callbacks_server, run_every_runcb);

    PRINT_DEBUG("client init end\n");
}

Steam_Client::~Steam_Client()
{
    network->shutDown();
}

void Steam_Client::userLogIn()
{
    network->addListenId(settings_client->get_local_steam_id());
    user_logged_in = true;
}

void Steam_Client::serverInit()
{
    server_init = true;
}

bool Steam_Client::IsServerInit()
{
    return server_init;
}

bool Steam_Client::IsUserLogIn()
{
    return user_logged_in;
}

void Steam_Client::serverShutdown()
{
    server_init = false;
}

void Steam_Client::setAppID(uint32 appid)
{
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    if (appid && !settings_client->get_local_game_id().AppID()) {
        settings_client->set_game_id(CGameID(appid));
        settings_server->set_game_id(CGameID(appid));
        local_storage->setAppId(appid);
        network->setAppID(appid);
    }

    
}

    // Creates a communication pipe to the Steam client.
// NOT THREADSAFE - ensure that no other threads are accessing Steamworks API when calling
HSteamPipe Steam_Client::CreateSteamPipe()
{
    PRINT_DEBUG("CreateSteamPipe\n");
    return CLIENT_STEAM_PIPE;
}

// Releases a previously created communications pipe
// NOT THREADSAFE - ensure that no other threads are accessing Steamworks API when calling
bool Steam_Client::BReleaseSteamPipe( HSteamPipe hSteamPipe )
{
    PRINT_DEBUG("BReleaseSteamPipe\n");
}

// connects to an existing global user, failing if none exists
// used by the game to coordinate with the steamUI
// NOT THREADSAFE - ensure that no other threads are accessing Steamworks API when calling
HSteamUser Steam_Client::ConnectToGlobalUser( HSteamPipe hSteamPipe )
{
    PRINT_DEBUG("ConnectToGlobalUser\n");
    return CLIENT_HSTEAMUSER;
}

// used by game servers, create a steam user that won't be shared with anyone else
// NOT THREADSAFE - ensure that no other threads are accessing Steamworks API when calling
HSteamUser Steam_Client::CreateLocalUser( HSteamPipe *phSteamPipe, EAccountType eAccountType )
{
    PRINT_DEBUG("CreateLocalUser\n");
    return SERVER_HSTEAMUSER;
}

HSteamUser Steam_Client::CreateLocalUser( HSteamPipe *phSteamPipe )
{
    return CreateLocalUser(phSteamPipe, k_EAccountTypeGameServer);
}

// removes an allocated user
// NOT THREADSAFE - ensure that no other threads are accessing Steamworks API when calling
void Steam_Client::ReleaseUser( HSteamPipe hSteamPipe, HSteamUser hUser )
{
    PRINT_DEBUG("ReleaseUser\n");
}

// retrieves the ISteamUser interface associated with the handle
ISteamUser *Steam_Client::GetISteamUser( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamUser %s\n", pchVersion);
    if (!user_logged_in) return NULL;

    if (strcmp(pchVersion, "SteamUser009") == 0) {
        return (ISteamUser *)(void *)(ISteamUser009 *)steam_user;
    } else if (strcmp(pchVersion, "SteamUser010") == 0) {
        return (ISteamUser *)(void *)(ISteamUser010 *)steam_user;
    } else if (strcmp(pchVersion, "SteamUser011") == 0) {
        return (ISteamUser *)(void *)(ISteamUser011 *)steam_user;
    } else if (strcmp(pchVersion, "SteamUser012") == 0) {
        return (ISteamUser *)(void *)(ISteamUser012 *)steam_user;
    } else if (strcmp(pchVersion, "SteamUser013") == 0) {
        return (ISteamUser *)(void *)(ISteamUser013 *)steam_user;
    } else if (strcmp(pchVersion, "SteamUser014") == 0) {
        return (ISteamUser *)(void *)(ISteamUser014 *)steam_user;
    } else if (strcmp(pchVersion, "SteamUser015") == 0) {
        return (ISteamUser *)(void *)(ISteamUser015 *)steam_user;
    } else if (strcmp(pchVersion, "SteamUser016") == 0) {
        return (ISteamUser *)(void *)(ISteamUser016 *)steam_user;
    } else if (strcmp(pchVersion, "SteamUser017") == 0) {
        return (ISteamUser *)(void *)(ISteamUser017 *)steam_user;
    } else if (strcmp(pchVersion, "SteamUser018") == 0) {
        return (ISteamUser *)(void *)(ISteamUser018 *)steam_user;
    } else if (strcmp(pchVersion, "SteamUser019") == 0) {
        return (ISteamUser *)(void *)(ISteamUser019 *)steam_user;
    } else if (strcmp(pchVersion, STEAMUSER_INTERFACE_VERSION) == 0) {
        return (ISteamUser *)(void *)(ISteamUser *)steam_user;
    } else {
        return (ISteamUser *)(void *)(ISteamUser *)steam_user;
    }

    return steam_user;
}

// retrieves the ISteamGameServer interface associated with the handle
ISteamGameServer *Steam_Client::GetISteamGameServer( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamGameServer %s\n", pchVersion);
    if (!server_init) return NULL;
    return steam_gameserver;
}

// set the local IP and Port to bind to
// this must be set before CreateLocalUser()
void Steam_Client::SetLocalIPBinding( uint32 unIP, uint16 usPort )
{
    PRINT_DEBUG("SetLocalIPBinding %u %hu\n", unIP, usPort);
}

// returns the ISteamFriends interface
ISteamFriends *Steam_Client::GetISteamFriends( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamFriends %s\n", pchVersion);
    if (!user_logged_in) return NULL;

    if (strcmp(pchVersion, "SteamFriends004") == 0) {
        return (ISteamFriends *)(void *)(ISteamFriends004 *)steam_friends;
    } else if (strcmp(pchVersion, "SteamFriends005") == 0) {
        return (ISteamFriends *)(void *)(ISteamFriends005 *)steam_friends;
    } else if (strcmp(pchVersion, "SteamFriends006") == 0) {
        return (ISteamFriends *)(void *)(ISteamFriends006 *)steam_friends;
    } else if (strcmp(pchVersion, "SteamFriends007") == 0) {
        return (ISteamFriends *)(void *)(ISteamFriends007 *)steam_friends;
    } else if (strcmp(pchVersion, "SteamFriends008") == 0) {
        return (ISteamFriends *)(void *)(ISteamFriends008 *)steam_friends;
    } else if (strcmp(pchVersion, "SteamFriends009") == 0) {
        return (ISteamFriends *)(void *)(ISteamFriends009 *)steam_friends;
    } else if (strcmp(pchVersion, "SteamFriends010") == 0) {
        return (ISteamFriends *)(void *)(ISteamFriends010 *)steam_friends;
    } else if (strcmp(pchVersion, "SteamFriends011") == 0) {
        return (ISteamFriends *)(void *)(ISteamFriends011 *)steam_friends;
    } else if (strcmp(pchVersion, "SteamFriends012") == 0) {
        return (ISteamFriends *)(void *)(ISteamFriends012 *)steam_friends;
    } else if (strcmp(pchVersion, "SteamFriends013") == 0) {
        return (ISteamFriends *)(void *)(ISteamFriends013 *)steam_friends;
    } else if (strcmp(pchVersion, "SteamFriends014") == 0) {
        return (ISteamFriends *)(void *)(ISteamFriends014 *)steam_friends;
    } else if (strcmp(pchVersion, "SteamFriends015") == 0) {
        return (ISteamFriends *)(void *)(ISteamFriends015 *)steam_friends;
    } else if (strcmp(pchVersion, "SteamFriends016") == 0) {
        return (ISteamFriends *)(void *)(ISteamFriends016 *)steam_friends;
    } else if (strcmp(pchVersion, STEAMFRIENDS_INTERFACE_VERSION) == 0) {
        return (ISteamFriends *)(void *)(ISteamFriends *)steam_friends;
    } else {
        return (ISteamFriends *)(void *)(ISteamFriends *)steam_friends;
    }

    return steam_friends;
}

// returns the ISteamUtils interface
ISteamUtils *Steam_Client::GetISteamUtils( HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamUtils %s\n", pchVersion);

    Steam_Utils *steam_utils_temp;

    if (hSteamPipe == SERVER_STEAM_PIPE) {
        steam_utils_temp = steam_gameserver_utils;
    } else {
        if (!user_logged_in) return NULL;
        steam_utils_temp = steam_utils;
    }

    if (strcmp(pchVersion, "SteamUtils002") == 0) {
        return (ISteamUtils *)(void *)(ISteamUtils002 *)steam_utils_temp;
    } else if (strcmp(pchVersion, "SteamUtils003") == 0) {
        return (ISteamUtils *)(void *)(ISteamUtils003 *)steam_utils_temp;
    } else if (strcmp(pchVersion, "SteamUtils004") == 0) {
        return (ISteamUtils *)(void *)(ISteamUtils004 *)steam_utils_temp;
    } else if (strcmp(pchVersion, "SteamUtils005") == 0) {
        return (ISteamUtils *)(void *)(ISteamUtils005 *)steam_utils_temp;
    } else if (strcmp(pchVersion, "SteamUtils006") == 0) {
        return (ISteamUtils *)(void *)(ISteamUtils006 *)steam_utils_temp;
    } else if (strcmp(pchVersion, "SteamUtils007") == 0) {
        return (ISteamUtils *)(void *)(ISteamUtils007 *)steam_utils_temp;
    } else if (strcmp(pchVersion, "SteamUtils008") == 0) {
        return (ISteamUtils *)(void *)(ISteamUtils008 *)steam_utils_temp;
    } else if (strcmp(pchVersion, STEAMUTILS_INTERFACE_VERSION) == 0) {
        return (ISteamUtils *)(void *)(ISteamUtils *)steam_utils_temp;
    } else {
        return (ISteamUtils *)(void *)(ISteamUtils *)steam_utils_temp;
    }

    return steam_utils_temp;
}

// returns the ISteamMatchmaking interface
ISteamMatchmaking *Steam_Client::GetISteamMatchmaking( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamMatchmaking %s\n", pchVersion);
    if (!user_logged_in) return NULL;

    if (strcmp(pchVersion, "SteamMatchMaking001") == 0) {
        //TODO
        return (ISteamMatchmaking *)(void *)(ISteamMatchmaking006 *)steam_matchmaking;
    } else if (strcmp(pchVersion, "SteamMatchMaking002") == 0) {
        //TODO
        return (ISteamMatchmaking *)(void *)(ISteamMatchmaking006 *)steam_matchmaking;
    } else if (strcmp(pchVersion, "SteamMatchMaking003") == 0) {
        //TODO
        return (ISteamMatchmaking *)(void *)(ISteamMatchmaking006 *)steam_matchmaking;
    } else if (strcmp(pchVersion, "SteamMatchMaking004") == 0) {
        //TODO
        return (ISteamMatchmaking *)(void *)(ISteamMatchmaking006 *)steam_matchmaking;
    } else if (strcmp(pchVersion, "SteamMatchMaking005") == 0) {
        //TODO
        return (ISteamMatchmaking *)(void *)(ISteamMatchmaking006 *)steam_matchmaking;
    } else if (strcmp(pchVersion, "SteamMatchMaking006") == 0) {
        return (ISteamMatchmaking *)(void *)(ISteamMatchmaking006 *)steam_matchmaking;
    } else if (strcmp(pchVersion, "SteamMatchMaking007") == 0) {
        return (ISteamMatchmaking *)(void *)(ISteamMatchmaking007 *)steam_matchmaking;
    } else if (strcmp(pchVersion, "SteamMatchMaking008") == 0) {
        return (ISteamMatchmaking *)(void *)(ISteamMatchmaking008 *)steam_matchmaking;
    } else if (strcmp(pchVersion, STEAMMATCHMAKING_INTERFACE_VERSION) == 0) {
        return (ISteamMatchmaking *)(void *)(ISteamMatchmaking *)steam_matchmaking;
    } else {
        return (ISteamMatchmaking *)(void *)(ISteamMatchmaking *)steam_matchmaking;
    }


    return steam_matchmaking;
}

// returns the ISteamMatchmakingServers interface
ISteamMatchmakingServers *Steam_Client::GetISteamMatchmakingServers( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamMatchmakingServers %s\n", pchVersion);
    if (!user_logged_in) return NULL;
    return steam_matchmaking_servers;
}

// returns the a generic interface
void *Steam_Client::GetISteamGenericInterface( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamGenericInterface %s\n", pchVersion);

    bool server = false;
    if (hSteamUser == SERVER_HSTEAMUSER) {
        server = true;
    } else {
        if ((strstr(pchVersion, "SteamNetworkingUtils") != pchVersion) && (strstr(pchVersion, "SteamUtils") != pchVersion)) {
            if (!user_logged_in) return NULL;
        }
    }

    if (strstr(pchVersion, "SteamNetworkingSocketsSerialized") == pchVersion) {
        Steam_Networking_Sockets_Serialized *steam_networking_sockets_serialized_temp;
        if (server) {
            steam_networking_sockets_serialized_temp = steam_gameserver_networking_sockets_serialized;
        } else {
            steam_networking_sockets_serialized_temp = steam_networking_sockets_serialized;
        }

        if (strcmp(pchVersion, "SteamNetworkingSocketsSerialized002") == 0) {
            return (void *)(ISteamNetworkingSocketsSerialized002 *)steam_networking_sockets_serialized_temp;
        } else if (strcmp(pchVersion, "SteamNetworkingSocketsSerialized003") == 0) {
            return (void *)(ISteamNetworkingSocketsSerialized003 *)steam_networking_sockets_serialized_temp;
        } else {
            return (void *)(ISteamNetworkingSocketsSerialized003 *)steam_networking_sockets_serialized_temp;
        }
    } else if (strstr(pchVersion, "SteamNetworkingSockets") == pchVersion) {
        Steam_Networking_Sockets *steam_networking_sockets_temp;
        if (server) {
            steam_networking_sockets_temp = steam_gameserver_networking_sockets;
        } else {
            steam_networking_sockets_temp = steam_networking_sockets;
        }

        if (strcmp(pchVersion, "SteamNetworkingSockets001") == 0) {
            return (void *)(ISteamNetworkingSockets001 *) steam_networking_sockets_temp;
        } else {
            return (void *)(ISteamNetworkingSockets *) steam_networking_sockets_temp;
        }
    } else if (strstr(pchVersion, "SteamGameCoordinator") == pchVersion) {
        Steam_Game_Coordinator *steam_game_coordinator_temp;
        if (server) {
            steam_game_coordinator_temp = steam_gameserver_game_coordinator;
        } else {
            steam_game_coordinator_temp = steam_game_coordinator;
        }

        return (void *)(ISteamGameCoordinator *)steam_game_coordinator_temp;
    } else if (strstr(pchVersion, "SteamNetworkingUtils") == pchVersion) {
            if (strcmp(pchVersion, "SteamNetworkingUtils001") == 0) {
                return (void *)(ISteamNetworkingUtils *)steam_networking_utils;
            } else {
                return (void *)(ISteamNetworkingUtils *)steam_networking_utils;
            }
    } else if (strstr(pchVersion, "STEAMREMOTESTORAGE_INTERFACE_VERSION") == pchVersion) {
        return GetISteamRemoteStorage(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "SteamGameServerStats") == pchVersion) {
        return GetISteamGameServerStats(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "SteamMatchMakingServers") == pchVersion) {
        return GetISteamMatchmakingServers(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "STEAMREMOTESTORAGE_INTERFACE_VERSION") == pchVersion) {
        return GetISteamRemoteStorage(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "SteamFriends") == pchVersion) {
        return GetISteamFriends(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "SteamMatchMaking") == pchVersion) {
        return GetISteamMatchmaking(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "SteamController") == pchVersion) {
        return GetISteamController(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "STEAMUGC_INTERFACE_VERSION") == pchVersion) {
        return GetISteamUGC(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "STEAMINVENTORY_INTERFACE") == pchVersion) {
        return GetISteamInventory(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "STEAMUSERSTATS_INTERFACE_VERSION") == pchVersion) {
        return GetISteamUserStats(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "SteamUser") == pchVersion) {
        return GetISteamUser(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "SteamNetworking") == pchVersion) {
        return GetISteamNetworking(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "SteamGameServer") == pchVersion) {
        return GetISteamGameServer(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "SteamUtils") == pchVersion) {
        return GetISteamUtils(hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "STEAMAPPS_INTERFACE_VERSION") == pchVersion) {
        return GetISteamApps(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "STEAMSCREENSHOTS_INTERFACE_VERSION") == pchVersion) {
        return GetISteamScreenshots(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "STEAMHTTP_INTERFACE_VERSION") == pchVersion) {
        return GetISteamHTTP(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "STEAMUNIFIEDMESSAGES_INTERFACE_VERSION") == pchVersion) {
        return DEPRECATED_GetISteamUnifiedMessages(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "STEAMAPPLIST_INTERFACE_VERSION") == pchVersion) {
        return GetISteamAppList(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "STEAMMUSIC_INTERFACE_VERSION") == pchVersion) {
        return GetISteamMusic(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "STEAMMUSICREMOTE_INTERFACE_VERSION") == pchVersion) {
        return GetISteamMusicRemote(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "STEAMHTMLSURFACE_INTERFACE_VERSION") == pchVersion) {
        return GetISteamHTMLSurface(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "STEAMVIDEO_INTERFACE") == pchVersion) {
        return GetISteamVideo(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "SteamMasterServerUpdater") == pchVersion) {
        return GetISteamMasterServerUpdater(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "SteamMatchGameSearch") == pchVersion) {
        return GetISteamGameSearch(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "SteamParties") == pchVersion) {
        return GetISteamParties(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "SteamInput") == pchVersion) {
        return GetISteamInput(hSteamUser, hSteamPipe, pchVersion);
    } else if (strstr(pchVersion, "STEAMPARENTALSETTINGS_INTERFACE_VERSION") == pchVersion) {
        return GetISteamParentalSettings(hSteamUser, hSteamPipe, pchVersion);
    } else {
        PRINT_DEBUG("No interface: %s\n", pchVersion);
        //TODO: all the interfaces
        return NULL;
    }
}

// returns the ISteamUserStats interface
ISteamUserStats *Steam_Client::GetISteamUserStats( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamUserStats %s\n", pchVersion);
    if (!user_logged_in) return NULL;

    if (strcmp(pchVersion, "STEAMUSERSTATS_INTERFACE_VERSION001") == 0) {
        //TODO
        return (ISteamUserStats *)(void *)(ISteamUserStats003 *)steam_user_stats;
    } else if (strcmp(pchVersion, "STEAMUSERSTATS_INTERFACE_VERSION002") == 0) {
        //TODO
        return (ISteamUserStats *)(void *)(ISteamUserStats003 *)steam_user_stats;
    } else if (strcmp(pchVersion, "STEAMUSERSTATS_INTERFACE_VERSION003") == 0) {
        return (ISteamUserStats *)(void *)(ISteamUserStats003 *)steam_user_stats;
    } else if (strcmp(pchVersion, "STEAMUSERSTATS_INTERFACE_VERSION004") == 0) {
        return (ISteamUserStats *)(void *)(ISteamUserStats004 *)steam_user_stats;
    } else if (strcmp(pchVersion, "STEAMUSERSTATS_INTERFACE_VERSION005") == 0) {
        return (ISteamUserStats *)(void *)(ISteamUserStats005 *)steam_user_stats;
    } else if (strcmp(pchVersion, "STEAMUSERSTATS_INTERFACE_VERSION006") == 0) {
        return (ISteamUserStats *)(void *)(ISteamUserStats006 *)steam_user_stats;
    } else if (strcmp(pchVersion, "STEAMUSERSTATS_INTERFACE_VERSION007") == 0) {
        return (ISteamUserStats *)(void *)(ISteamUserStats007 *)steam_user_stats;
    } else if (strcmp(pchVersion, "STEAMUSERSTATS_INTERFACE_VERSION008") == 0) {
        return (ISteamUserStats *)(void *)(ISteamUserStats008 *)steam_user_stats;
    } else if (strcmp(pchVersion, "STEAMUSERSTATS_INTERFACE_VERSION009") == 0) {
        return (ISteamUserStats *)(void *)(ISteamUserStats009 *)steam_user_stats;
    } else if (strcmp(pchVersion, "STEAMUSERSTATS_INTERFACE_VERSION010") == 0) {
        return (ISteamUserStats *)(void *)(ISteamUserStats010 *)steam_user_stats;
    } else if (strcmp(pchVersion, STEAMUSERSTATS_INTERFACE_VERSION) == 0) {
        return (ISteamUserStats *)(void *)(ISteamUserStats *)steam_user_stats;
    } else {
        return (ISteamUserStats *)(void *)(ISteamUserStats *)steam_user_stats;
    }

    return steam_user_stats;
}

// returns the ISteamGameServerStats interface
ISteamGameServerStats *Steam_Client::GetISteamGameServerStats( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamGameServerStats %s\n", pchVersion);
    return steam_gameserverstats;
}

// returns apps interface
ISteamApps *Steam_Client::GetISteamApps( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamApps %s\n", pchVersion);
    if (hSteamUser == SERVER_HSTEAMUSER) {
        return steam_gameserver_apps;
    }

    if (!user_logged_in) return NULL;
    return steam_apps;
}

// networking
ISteamNetworking *Steam_Client::GetISteamNetworking( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamNetworking %s\n", pchVersion);

    Steam_Networking *steam_networking_temp;

    if (hSteamUser == SERVER_HSTEAMUSER) {
        steam_networking_temp = steam_gameserver_networking;
    } else {
        if (!user_logged_in) return NULL;
        steam_networking_temp = steam_networking;
    }

    if (strcmp(pchVersion, "SteamNetworking001") == 0) {
        return (ISteamNetworking *)(void *)(ISteamNetworking001 *)steam_networking_temp;
    } else if (strcmp(pchVersion, "SteamNetworking002") == 0) {
        return (ISteamNetworking *)(void *)(ISteamNetworking002 *)steam_networking_temp;
    } else if (strcmp(pchVersion, "SteamNetworking003") == 0) {
        return (ISteamNetworking *)(void *)(ISteamNetworking003 *)steam_networking_temp;
    } else if (strcmp(pchVersion, "SteamNetworking004") == 0) {
        return (ISteamNetworking *)(void *)(ISteamNetworking004 *)steam_networking_temp;
    } else if (strcmp(pchVersion, STEAMUGC_INTERFACE_VERSION) == 0) {
        return (ISteamNetworking *)(void *)(ISteamNetworking *)steam_networking_temp;
    } else {
        return (ISteamNetworking *)(void *)(ISteamNetworking *)steam_networking_temp;
    }

    return steam_networking_temp;
}

// remote storage
ISteamRemoteStorage *Steam_Client::GetISteamRemoteStorage( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamRemoteStorage %s\n", pchVersion);
    if (!user_logged_in) return NULL;

    if (strcmp(pchVersion, "STEAMREMOTESTORAGE_INTERFACE_VERSION001") == 0) {
        return (ISteamRemoteStorage *)(void *)(ISteamRemoteStorage001 *)steam_remote_storage;
    } else if (strcmp(pchVersion, "STEAMREMOTESTORAGE_INTERFACE_VERSION002") == 0) {
        return (ISteamRemoteStorage *)(void *)(ISteamRemoteStorage002 *)steam_remote_storage;
    } else if (strcmp(pchVersion, "STEAMREMOTESTORAGE_INTERFACE_VERSION003") == 0) {
        return (ISteamRemoteStorage *)(void *)(ISteamRemoteStorage003 *)steam_remote_storage;
    } else if (strcmp(pchVersion, "STEAMREMOTESTORAGE_INTERFACE_VERSION004") == 0) {
        return (ISteamRemoteStorage *)(void *)(ISteamRemoteStorage004 *)steam_remote_storage;
    } else if (strcmp(pchVersion, "STEAMREMOTESTORAGE_INTERFACE_VERSION005") == 0) {
        return (ISteamRemoteStorage *)(void *)(ISteamRemoteStorage005 *)steam_remote_storage;
    } else if (strcmp(pchVersion, "STEAMREMOTESTORAGE_INTERFACE_VERSION006") == 0) {
        return (ISteamRemoteStorage *)(void *)(ISteamRemoteStorage006 *)steam_remote_storage;
    } else if (strcmp(pchVersion, "STEAMREMOTESTORAGE_INTERFACE_VERSION007") == 0) {
        return (ISteamRemoteStorage *)(void *)(ISteamRemoteStorage007 *)steam_remote_storage;
    } else if (strcmp(pchVersion, "STEAMREMOTESTORAGE_INTERFACE_VERSION008") == 0) {
        return (ISteamRemoteStorage *)(void *)(ISteamRemoteStorage008 *)steam_remote_storage;
    } else if (strcmp(pchVersion, "STEAMREMOTESTORAGE_INTERFACE_VERSION009") == 0) {
        return (ISteamRemoteStorage *)(void *)(ISteamRemoteStorage009 *)steam_remote_storage;
    } else if (strcmp(pchVersion, "STEAMREMOTESTORAGE_INTERFACE_VERSION010") == 0) {
        return (ISteamRemoteStorage *)(void *)(ISteamRemoteStorage010 *)steam_remote_storage;
    } else if (strcmp(pchVersion, "STEAMREMOTESTORAGE_INTERFACE_VERSION011") == 0) {
        return (ISteamRemoteStorage *)(void *)(ISteamRemoteStorage011 *)steam_remote_storage;
    } else if (strcmp(pchVersion, "STEAMREMOTESTORAGE_INTERFACE_VERSION012") == 0) {
        return (ISteamRemoteStorage *)(void *)(ISteamRemoteStorage012 *)steam_remote_storage;
    } else if (strcmp(pchVersion, "STEAMREMOTESTORAGE_INTERFACE_VERSION013") == 0) {
        return (ISteamRemoteStorage *)(void *)(ISteamRemoteStorage013 *)steam_remote_storage;
    } else if (strcmp(pchVersion, STEAMREMOTESTORAGE_INTERFACE_VERSION) == 0) {
        return (ISteamRemoteStorage *)(void *)(ISteamRemoteStorage *)steam_remote_storage;
    } else {
        return (ISteamRemoteStorage *)(void *)(ISteamRemoteStorage *)steam_remote_storage;
    }

    return steam_remote_storage;
}

// user screenshots
ISteamScreenshots *Steam_Client::GetISteamScreenshots( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamScreenshots %s\n", pchVersion);
    if (!user_logged_in) return NULL;
    return steam_screenshots;
}


// Deprecated. Applications should use SteamAPI_RunCallbacks() or SteamGameServer_RunCallbacks() instead.
void Steam_Client::RunFrame()
{
    PRINT_DEBUG("RunFrame\n");
}

// returns the number of IPC calls made since the last time this function was called
// Used for perf debugging so you can understand how many IPC calls your game makes per frame
// Every IPC call is at minimum a thread context switch if not a process one so you want to rate
// control how often you do them.
uint32 Steam_Client::GetIPCCallCount()
{
    PRINT_DEBUG("Steam_Client::GetIPCCallCount\n");
    return steam_utils->GetIPCCallCount();
}

// API warning handling
// 'int' is the severity; 0 for msg, 1 for warning
// 'const char *' is the text of the message
// callbacks will occur directly after the API function is called that generated the warning or message.
void Steam_Client::SetWarningMessageHook( SteamAPIWarningMessageHook_t pFunction )
{
    PRINT_DEBUG("Steam_Client::SetWarningMessageHook\n");
}

// Trigger global shutdown for the DLL
bool Steam_Client::BShutdownIfAllPipesClosed()
{
    PRINT_DEBUG("BShutdownIfAllPipesClosed\n");
    return true;
}

// Expose HTTP interface
ISteamHTTP *Steam_Client::GetISteamHTTP( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamHTTP %s\n", pchVersion);
    if (hSteamuser == SERVER_HSTEAMUSER) {
        return steam_gameserver_http;
    }

    if (!user_logged_in) return NULL;
    return steam_http;
}

// Deprecated - the ISteamUnifiedMessages interface is no longer intended for public consumption.
void *Steam_Client::DEPRECATED_GetISteamUnifiedMessages( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) 
{
    PRINT_DEBUG("DEPRECATED_GetISteamUnifiedMessages %s\n", pchVersion);
    return (void *)(ISteamUnifiedMessages *)steam_unified_messages;
}

ISteamUnifiedMessages *Steam_Client::GetISteamUnifiedMessages( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamUnifiedMessages %s\n", pchVersion);
    return steam_unified_messages;
}

// Exposes the ISteamController interface
ISteamController *Steam_Client::GetISteamController( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamController %s\n", pchVersion);
    if (!user_logged_in) return NULL;

    if (strcmp(pchVersion, "STEAMCONTROLLER_INTERFACE_VERSION") == 0) {
        return (ISteamController *)(void *)(ISteamController001 *)steam_controller;
    } else if (strcmp(pchVersion, "STEAMCONTROLLER_INTERFACE_VERSION_002") == 0) {
        //I'm pretty sure this interface is never actually used
        return (ISteamController *)(void *)(ISteamController003 *)steam_controller;
    } else if (strcmp(pchVersion, "SteamController003") == 0) {
        return (ISteamController *)(void *)(ISteamController003 *)steam_controller;
    } else if (strcmp(pchVersion, "SteamController004") == 0) {
        return (ISteamController *)(void *)(ISteamController004 *)steam_controller;
    } else if (strcmp(pchVersion, "SteamController005") == 0) {
        return (ISteamController *)(void *)(ISteamController005 *)steam_controller;
    } else if (strcmp(pchVersion, "SteamController006") == 0) {
        return (ISteamController *)(void *)(ISteamController006 *)steam_controller;
    } else if (strcmp(pchVersion, STEAMCONTROLLER_INTERFACE_VERSION) == 0) {
        return (ISteamController *)(void *)(ISteamController *)steam_controller;
    } else {
        return (ISteamController *)(void *)(ISteamController *)steam_controller;
    }

    return steam_controller;
}

// Exposes the ISteamUGC interface
ISteamUGC *Steam_Client::GetISteamUGC( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamUGC %s\n", pchVersion);
    Steam_UGC *steam_ugc_temp;

    if (hSteamUser == SERVER_HSTEAMUSER) {
        steam_ugc_temp = steam_gameserver_ugc;
    } else {
        if (!user_logged_in) return NULL;
        steam_ugc_temp = steam_ugc;
    }

    if (strcmp(pchVersion, "STEAMUGC_INTERFACE_VERSION") == 0) {
        //Is this actually a valid interface version?
        return (ISteamUGC *)(void *)(ISteamUGC001 *)steam_ugc_temp;
    } else if (strcmp(pchVersion, "STEAMUGC_INTERFACE_VERSION001") == 0) {
        return (ISteamUGC *)(void *)(ISteamUGC001 *)steam_ugc_temp;
    } else if (strcmp(pchVersion, "STEAMUGC_INTERFACE_VERSION002") == 0) {
        return (ISteamUGC *)(void *)(ISteamUGC002 *)steam_ugc_temp;
    } else if (strcmp(pchVersion, "STEAMUGC_INTERFACE_VERSION003") == 0) {
        return (ISteamUGC *)(void *)(ISteamUGC003 *)steam_ugc_temp;
    } else if (strcmp(pchVersion, "STEAMUGC_INTERFACE_VERSION004") == 0) {
        return (ISteamUGC *)(void *)(ISteamUGC004 *)steam_ugc_temp;
    } else if (strcmp(pchVersion, "STEAMUGC_INTERFACE_VERSION005") == 0) {
        return (ISteamUGC *)(void *)(ISteamUGC005 *)steam_ugc_temp;
    } else if (strcmp(pchVersion, "STEAMUGC_INTERFACE_VERSION006") == 0) {
        return (ISteamUGC *)(void *)(ISteamUGC006 *)steam_ugc_temp;
    } else if (strcmp(pchVersion, "STEAMUGC_INTERFACE_VERSION007") == 0) {
        return (ISteamUGC *)(void *)(ISteamUGC007 *)steam_ugc_temp;
    } else if (strcmp(pchVersion, "STEAMUGC_INTERFACE_VERSION008") == 0) {
        return (ISteamUGC *)(void *)(ISteamUGC008 *)steam_ugc_temp;
    } else if (strcmp(pchVersion, "STEAMUGC_INTERFACE_VERSION009") == 0) {
        return (ISteamUGC *)(void *)(ISteamUGC009 *)steam_ugc_temp;
    } else if (strcmp(pchVersion, "STEAMUGC_INTERFACE_VERSION010") == 0) {
        return (ISteamUGC *)(void *)(ISteamUGC010 *)steam_ugc_temp;
    } else if (strcmp(pchVersion, "STEAMUGC_INTERFACE_VERSION011") == 0) {
        //TODO
        return (ISteamUGC *)(void *)(ISteamUGC *)steam_ugc_temp;
    } else if (strcmp(pchVersion, STEAMUGC_INTERFACE_VERSION) == 0) {
        return (ISteamUGC *)(void *)(ISteamUGC *)steam_ugc_temp;
    } else {
        return (ISteamUGC *)(void *)(ISteamUGC *)steam_ugc_temp;
    }

    return steam_ugc_temp;
}

// returns app list interface, only available on specially registered apps
ISteamAppList *Steam_Client::GetISteamAppList( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamAppList %s\n", pchVersion);
    if (!user_logged_in) return NULL;
    return steam_applist;
}

// Music Player
ISteamMusic *Steam_Client::GetISteamMusic( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamMusic %s\n", pchVersion);
    if (!user_logged_in) return NULL;
    return steam_music;
}

// Music Player Remote
ISteamMusicRemote *Steam_Client::GetISteamMusicRemote(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion)
{
    PRINT_DEBUG("GetISteamMusicRemote %s\n", pchVersion);
    if (!user_logged_in) return NULL;
    return steam_musicremote;
}

// html page display
ISteamHTMLSurface *Steam_Client::GetISteamHTMLSurface(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion)
{
    PRINT_DEBUG("GetISteamHTMLSurface %s\n", pchVersion);
    if (!user_logged_in) return NULL;

    if (strcmp(pchVersion, "STEAMHTMLSURFACE_INTERFACE_VERSION_001") == 0) {
        return (ISteamHTMLSurface *)(void *)(ISteamHTMLSurface001 *)steam_HTMLsurface;
    } else if (strcmp(pchVersion, "STEAMHTMLSURFACE_INTERFACE_VERSION_002") == 0) {
        return (ISteamHTMLSurface *)(void *)(ISteamHTMLSurface002 *)steam_HTMLsurface;
    } else if (strcmp(pchVersion, "STEAMHTMLSURFACE_INTERFACE_VERSION_003") == 0) {
        return (ISteamHTMLSurface *)(void *)(ISteamHTMLSurface003 *)steam_HTMLsurface;
    } else if (strcmp(pchVersion, "STEAMHTMLSURFACE_INTERFACE_VERSION_004") == 0) {
        return (ISteamHTMLSurface *)(void *)(ISteamHTMLSurface004 *)steam_HTMLsurface;
    } else if (strcmp(pchVersion, STEAMHTMLSURFACE_INTERFACE_VERSION) == 0) {
        return (ISteamHTMLSurface *)(void *)(ISteamHTMLSurface *)steam_HTMLsurface;
    } else {
        return (ISteamHTMLSurface *)(void *)(ISteamHTMLSurface *)steam_HTMLsurface;
    }

    return steam_HTMLsurface;
}

// Helper functions for internal Steam usage
void Steam_Client::DEPRECATED_Set_SteamAPI_CPostAPIResultInProcess( void (*)() )
{
    PRINT_DEBUG("DEPRECATED_Set_SteamAPI_CPostAPIResultInProcess\n");
}

void Steam_Client::DEPRECATED_Remove_SteamAPI_CPostAPIResultInProcess( void (*)() )
{
    PRINT_DEBUG("DEPRECATED_Remove_SteamAPI_CPostAPIResultInProcess\n");
}

void Steam_Client::Set_SteamAPI_CCheckCallbackRegisteredInProcess( SteamAPI_CheckCallbackRegistered_t func )
{
    PRINT_DEBUG("Set_SteamAPI_CCheckCallbackRegisteredInProcess\n");
}

void Steam_Client::Set_SteamAPI_CPostAPIResultInProcess( SteamAPI_PostAPIResultInProcess_t func )
{
    PRINT_DEBUG("Set_SteamAPI_CPostAPIResultInProcess\n");
}

void Steam_Client::Remove_SteamAPI_CPostAPIResultInProcess( SteamAPI_PostAPIResultInProcess_t func )
{
    PRINT_DEBUG("Remove_SteamAPI_CPostAPIResultInProcess\n");
}

// inventory
ISteamInventory *Steam_Client::GetISteamInventory( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamInventory %s\n", pchVersion);
    Steam_Inventory *steam_inventory_temp;
    Settings *settings_temp;
    SteamCallBacks *callbacks_temp;
    SteamCallResults *callback_results_temp;

    if (hSteamuser == SERVER_HSTEAMUSER) {
        steam_inventory_temp = steam_gameserver_inventory;
    } else {
        if (!user_logged_in) return NULL;
        steam_inventory_temp = steam_inventory;
    }

    if (strcmp(pchVersion, "STEAMINVENTORY_INTERFACE_V001") == 0) {
        return (ISteamInventory *)(void *)(ISteamInventory001 *)steam_inventory_temp;
    } else if (strcmp(pchVersion, "STEAMINVENTORY_INTERFACE_V002") == 0) {
        return (ISteamInventory *)(void *)(ISteamInventory002 *)steam_inventory_temp;
    } else if (strcmp(pchVersion, STEAMINVENTORY_INTERFACE_VERSION) == 0) {
        return (ISteamInventory *)(void *)(ISteamInventory *)steam_inventory_temp;
    } else {
        return (ISteamInventory *)(void *)(ISteamInventory *)steam_inventory_temp;
    }

    return steam_inventory_temp;
}

// Video
ISteamVideo *Steam_Client::GetISteamVideo( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamVideo %s\n", pchVersion);
    if (!user_logged_in) return NULL;
    return steam_video;
}

// Parental controls
ISteamParentalSettings *Steam_Client::GetISteamParentalSettings( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamParentalSettings %s\n", pchVersion);
    if (!user_logged_in) return NULL;
    return steam_parental;
}

ISteamMasterServerUpdater *Steam_Client::GetISteamMasterServerUpdater( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamMasterServerUpdater %s\n", pchVersion);
    return steam_masterserver_updater;
}

ISteamContentServer *Steam_Client::GetISteamContentServer( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamContentServer %s\n", pchVersion);
    return NULL;
}

// game search
ISteamGameSearch *Steam_Client::GetISteamGameSearch( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamGameSearch %s\n", pchVersion);
    if (!user_logged_in) return NULL;

    return steam_game_search;
}

// Exposes the Steam Input interface for controller support
ISteamInput *Steam_Client::GetISteamInput( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamInput %s\n", pchVersion);
    if (!user_logged_in) return NULL;

    return steam_controller;
}

// Steam Parties interface
ISteamParties *Steam_Client::GetISteamParties( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion )
{
    PRINT_DEBUG("GetISteamParties %s\n", pchVersion);
    if (!user_logged_in) return NULL;

    return steam_parties;
}

void Steam_Client::RegisterCallback( class CCallbackBase *pCallback, int iCallback)
{
    int base_callback = (iCallback / 100) * 100;
    int callback_id = iCallback % 100;
    bool isGameServer = CCallbackMgr::isServer(pCallback);
    PRINT_DEBUG("isGameServer %u\n", isGameServer);

    switch (base_callback) {
        case k_iSteamUserCallbacks:
            PRINT_DEBUG("k_iSteamUserCallbacks %i\n", callback_id);
            break;

        case k_iSteamGameServerCallbacks:
            PRINT_DEBUG("k_iSteamGameServerCallbacks %i\n", callback_id);
            break;

        case k_iSteamFriendsCallbacks:
            PRINT_DEBUG("k_iSteamFriendsCallbacks %i\n", callback_id);
            break;

        case k_iSteamBillingCallbacks:
            PRINT_DEBUG("k_iSteamBillingCallbacks %i\n", callback_id);
            break;

        case k_iSteamMatchmakingCallbacks:
            PRINT_DEBUG("k_iSteamMatchmakingCallbacks %i\n", callback_id);
            break;

        case k_iSteamContentServerCallbacks:
            PRINT_DEBUG("k_iSteamContentServerCallbacks %i\n", callback_id);
            break;

        case k_iSteamUtilsCallbacks:
            PRINT_DEBUG("k_iSteamUtilsCallbacks %i\n", callback_id);
            break;

        case k_iClientFriendsCallbacks:
            PRINT_DEBUG("k_iClientFriendsCallbacks %i\n", callback_id);
            break;

        case k_iClientUserCallbacks:
            PRINT_DEBUG("k_iClientUserCallbacks %i\n", callback_id);
            break;

        case k_iSteamAppsCallbacks:
            PRINT_DEBUG("k_iSteamAppsCallbacks %i\n", callback_id);
            break;

        case k_iSteamUserStatsCallbacks:
            PRINT_DEBUG("k_iSteamUserStatsCallbacks %i\n", callback_id);
            break;

        case k_iSteamNetworkingCallbacks:
            PRINT_DEBUG("k_iSteamNetworkingCallbacks %i\n", callback_id);
            break;

        case k_iClientRemoteStorageCallbacks:
            PRINT_DEBUG("k_iClientRemoteStorageCallbacks %i\n", callback_id);
            break;

        case k_iClientDepotBuilderCallbacks:
            PRINT_DEBUG("k_iClientDepotBuilderCallbacks %i\n", callback_id);
            break;

        case k_iSteamGameServerItemsCallbacks:
            PRINT_DEBUG("k_iSteamGameServerItemsCallbacks %i\n", callback_id);
            break;

        case k_iClientUtilsCallbacks:
            PRINT_DEBUG("k_iClientUtilsCallbacks %i\n", callback_id);
            break;

        case k_iSteamGameCoordinatorCallbacks:
            PRINT_DEBUG("k_iSteamGameCoordinatorCallbacks %i\n", callback_id);
            break;

        case k_iSteamGameServerStatsCallbacks:
            PRINT_DEBUG("k_iSteamGameServerStatsCallbacks %i\n", callback_id);
            break;

        case k_iSteam2AsyncCallbacks:
            PRINT_DEBUG("k_iSteam2AsyncCallbacks %i\n", callback_id);
            break;

        case k_iSteamGameStatsCallbacks:
            PRINT_DEBUG("k_iSteamGameStatsCallbacks %i\n", callback_id);
            break;

        case k_iClientHTTPCallbacks:
            PRINT_DEBUG("k_iClientHTTPCallbacks %i\n", callback_id);
            break;

        case k_iClientScreenshotsCallbacks:
            PRINT_DEBUG("k_iClientScreenshotsCallbacks %i\n", callback_id);
            break;

        case k_iSteamScreenshotsCallbacks:
            PRINT_DEBUG("k_iSteamScreenshotsCallbacks %i\n", callback_id);
            break;

        case k_iClientAudioCallbacks:
            PRINT_DEBUG("k_iClientAudioCallbacks %i\n", callback_id);
            break;

        case k_iClientUnifiedMessagesCallbacks:
            PRINT_DEBUG("k_iClientUnifiedMessagesCallbacks %i\n", callback_id);
            break;

        case k_iSteamStreamLauncherCallbacks:
            PRINT_DEBUG("k_iSteamStreamLauncherCallbacks %i\n", callback_id);
            break;

        case k_iClientControllerCallbacks:
            PRINT_DEBUG("k_iClientControllerCallbacks %i\n", callback_id);
            break;

        case k_iSteamControllerCallbacks:
            PRINT_DEBUG("k_iSteamControllerCallbacks %i\n", callback_id);
            break;

        case k_iClientParentalSettingsCallbacks:
            PRINT_DEBUG("k_iClientParentalSettingsCallbacks %i\n", callback_id);
            break;

        case k_iClientDeviceAuthCallbacks:
            PRINT_DEBUG("k_iClientDeviceAuthCallbacks %i\n", callback_id);
            break;

        case k_iClientNetworkDeviceManagerCallbacks:
            PRINT_DEBUG("k_iClientNetworkDeviceManagerCallbacks %i\n", callback_id);
            break;

        case k_iClientMusicCallbacks:
            PRINT_DEBUG("k_iClientMusicCallbacks %i\n", callback_id);
            break;

        case k_iClientRemoteClientManagerCallbacks:
            PRINT_DEBUG("k_iClientRemoteClientManagerCallbacks %i\n", callback_id);
            break;

        case k_iClientUGCCallbacks:
            PRINT_DEBUG("k_iClientUGCCallbacks %i\n", callback_id);
            break;

        case k_iSteamStreamClientCallbacks:
            PRINT_DEBUG("k_iSteamStreamClientCallbacks %i\n", callback_id);
            break;

        case k_IClientProductBuilderCallbacks:
            PRINT_DEBUG("k_IClientProductBuilderCallbacks %i\n", callback_id);
            break;

        case k_iClientShortcutsCallbacks:
            PRINT_DEBUG("k_iClientShortcutsCallbacks %i\n", callback_id);
            break;

        case k_iClientRemoteControlManagerCallbacks:
            PRINT_DEBUG("k_iClientRemoteControlManagerCallbacks %i\n", callback_id);
            break;

        case k_iSteamAppListCallbacks:
            PRINT_DEBUG("k_iSteamAppListCallbacks %i\n", callback_id);
            break;

        case k_iSteamMusicCallbacks:
            PRINT_DEBUG("k_iSteamMusicCallbacks %i\n", callback_id);
            break;

        case k_iSteamMusicRemoteCallbacks:
            PRINT_DEBUG("k_iSteamMusicRemoteCallbacks %i\n", callback_id);
            break;

        case k_iClientVRCallbacks:
            PRINT_DEBUG("k_iClientVRCallbacks %i\n", callback_id);
            break;

        case k_iClientGameNotificationCallbacks:
            PRINT_DEBUG("k_iClientGameNotificationCallbacks %i\n", callback_id);
            break;
 
        case k_iSteamGameNotificationCallbacks:
            PRINT_DEBUG("k_iSteamGameNotificationCallbacks %i\n", callback_id);
            break;
 
        case k_iSteamHTMLSurfaceCallbacks:
            PRINT_DEBUG("k_iSteamHTMLSurfaceCallbacks %i\n", callback_id);
            break;

        case k_iClientVideoCallbacks:
            PRINT_DEBUG("k_iClientVideoCallbacks %i\n", callback_id);
            break;

        case k_iClientInventoryCallbacks:
            PRINT_DEBUG("k_iClientInventoryCallbacks %i\n", callback_id);
            break;

        case k_iClientBluetoothManagerCallbacks:
            PRINT_DEBUG("k_iClientBluetoothManagerCallbacks %i\n", callback_id);
            break;

        case k_iClientSharedConnectionCallbacks:
            PRINT_DEBUG("k_iClientSharedConnectionCallbacks %i\n", callback_id);
            break;

        case k_ISteamParentalSettingsCallbacks:
            PRINT_DEBUG("k_ISteamParentalSettingsCallbacks %i\n", callback_id);
            break;

        case k_iClientShaderCallbacks:
            PRINT_DEBUG("k_iClientShaderCallbacks %i\n", callback_id);
            break;
        
        default:
            PRINT_DEBUG("Unknown callback base %i\n", base_callback);
    };

    if (isGameServer) {
        callbacks_server->addCallBack(iCallback, pCallback);
    } else {
        callbacks_client->addCallBack(iCallback, pCallback);
    }
}

void Steam_Client::UnregisterCallback( class CCallbackBase *pCallback)
{
    int iCallback = pCallback->GetICallback();
    int base_callback = (iCallback / 100) * 100;
    int callback_id = iCallback % 100;
    bool isGameServer = CCallbackMgr::isServer(pCallback);
    PRINT_DEBUG("isGameServer %u\n", isGameServer);

    switch (base_callback) {
        case k_iSteamUserCallbacks:
            PRINT_DEBUG("k_iSteamUserCallbacks %i\n", callback_id);
            break;

        case k_iSteamGameServerCallbacks:
            PRINT_DEBUG("k_iSteamGameServerCallbacks %i\n", callback_id);
            break;

        case k_iSteamFriendsCallbacks:
            PRINT_DEBUG("k_iSteamFriendsCallbacks %i\n", callback_id);
            break;

        case k_iSteamBillingCallbacks:
            PRINT_DEBUG("k_iSteamBillingCallbacks %i\n", callback_id);
            break;

        case k_iSteamMatchmakingCallbacks:
            PRINT_DEBUG("k_iSteamMatchmakingCallbacks %i\n", callback_id);
            break;

        case k_iSteamContentServerCallbacks:
            PRINT_DEBUG("k_iSteamContentServerCallbacks %i\n", callback_id);
            break;

        case k_iSteamUtilsCallbacks:
            PRINT_DEBUG("k_iSteamUtilsCallbacks %i\n", callback_id);
            break;

        case k_iClientFriendsCallbacks:
            PRINT_DEBUG("k_iClientFriendsCallbacks %i\n", callback_id);
            break;

        case k_iClientUserCallbacks:
            PRINT_DEBUG("k_iClientUserCallbacks %i\n", callback_id);
            break;

        case k_iSteamAppsCallbacks:
            PRINT_DEBUG("k_iSteamAppsCallbacks %i\n", callback_id);
            break;

        case k_iSteamUserStatsCallbacks:
            PRINT_DEBUG("k_iSteamUserStatsCallbacks %i\n", callback_id);
            break;

        case k_iSteamNetworkingCallbacks:
            PRINT_DEBUG("k_iSteamNetworkingCallbacks %i\n", callback_id);
            break;

        case k_iClientRemoteStorageCallbacks:
            PRINT_DEBUG("k_iClientRemoteStorageCallbacks %i\n", callback_id);
            break;

        case k_iClientDepotBuilderCallbacks:
            PRINT_DEBUG("k_iClientDepotBuilderCallbacks %i\n", callback_id);
            break;

        case k_iSteamGameServerItemsCallbacks:
            PRINT_DEBUG("k_iSteamGameServerItemsCallbacks %i\n", callback_id);
            break;

        case k_iClientUtilsCallbacks:
            PRINT_DEBUG("k_iClientUtilsCallbacks %i\n", callback_id);
            break;

        case k_iSteamGameCoordinatorCallbacks:
            PRINT_DEBUG("k_iSteamGameCoordinatorCallbacks %i\n", callback_id);
            break;

        case k_iSteamGameServerStatsCallbacks:
            PRINT_DEBUG("k_iSteamGameServerStatsCallbacks %i\n", callback_id);
            break;

        case k_iSteam2AsyncCallbacks:
            PRINT_DEBUG("k_iSteam2AsyncCallbacks %i\n", callback_id);
            break;

        case k_iSteamGameStatsCallbacks:
            PRINT_DEBUG("k_iSteamGameStatsCallbacks %i\n", callback_id);
            break;

        case k_iClientHTTPCallbacks:
            PRINT_DEBUG("k_iClientHTTPCallbacks %i\n", callback_id);
            break;

        case k_iClientScreenshotsCallbacks:
            PRINT_DEBUG("k_iClientScreenshotsCallbacks %i\n", callback_id);
            break;

        case k_iSteamScreenshotsCallbacks:
            PRINT_DEBUG("k_iSteamScreenshotsCallbacks %i\n", callback_id);
            break;

        case k_iClientAudioCallbacks:
            PRINT_DEBUG("k_iClientAudioCallbacks %i\n", callback_id);
            break;

        case k_iClientUnifiedMessagesCallbacks:
            PRINT_DEBUG("k_iClientUnifiedMessagesCallbacks %i\n", callback_id);
            break;

        case k_iSteamStreamLauncherCallbacks:
            PRINT_DEBUG("k_iSteamStreamLauncherCallbacks %i\n", callback_id);
            break;

        case k_iClientControllerCallbacks:
            PRINT_DEBUG("k_iClientControllerCallbacks %i\n", callback_id);
            break;

        case k_iSteamControllerCallbacks:
            PRINT_DEBUG("k_iSteamControllerCallbacks %i\n", callback_id);
            break;

        case k_iClientParentalSettingsCallbacks:
            PRINT_DEBUG("k_iClientParentalSettingsCallbacks %i\n", callback_id);
            break;

        case k_iClientDeviceAuthCallbacks:
            PRINT_DEBUG("k_iClientDeviceAuthCallbacks %i\n", callback_id);
            break;

        case k_iClientNetworkDeviceManagerCallbacks:
            PRINT_DEBUG("k_iClientNetworkDeviceManagerCallbacks %i\n", callback_id);
            break;

        case k_iClientMusicCallbacks:
            PRINT_DEBUG("k_iClientMusicCallbacks %i\n", callback_id);
            break;

        case k_iClientRemoteClientManagerCallbacks:
            PRINT_DEBUG("k_iClientRemoteClientManagerCallbacks %i\n", callback_id);
            break;

        case k_iClientUGCCallbacks:
            PRINT_DEBUG("k_iClientUGCCallbacks %i\n", callback_id);
            break;

        case k_iSteamStreamClientCallbacks:
            PRINT_DEBUG("k_iSteamStreamClientCallbacks %i\n", callback_id);
            break;

        case k_IClientProductBuilderCallbacks:
            PRINT_DEBUG("k_IClientProductBuilderCallbacks %i\n", callback_id);
            break;

        case k_iClientShortcutsCallbacks:
            PRINT_DEBUG("k_iClientShortcutsCallbacks %i\n", callback_id);
            break;

        case k_iClientRemoteControlManagerCallbacks:
            PRINT_DEBUG("k_iClientRemoteControlManagerCallbacks %i\n", callback_id);
            break;

        case k_iSteamAppListCallbacks:
            PRINT_DEBUG("k_iSteamAppListCallbacks %i\n", callback_id);
            break;

        case k_iSteamMusicCallbacks:
            PRINT_DEBUG("k_iSteamMusicCallbacks %i\n", callback_id);
            break;

        case k_iSteamMusicRemoteCallbacks:
            PRINT_DEBUG("k_iSteamMusicRemoteCallbacks %i\n", callback_id);
            break;

        case k_iClientVRCallbacks:
            PRINT_DEBUG("k_iClientVRCallbacks %i\n", callback_id);
            break;

        case k_iClientGameNotificationCallbacks:
            PRINT_DEBUG("k_iClientGameNotificationCallbacks %i\n", callback_id);
            break;
 
        case k_iSteamGameNotificationCallbacks:
            PRINT_DEBUG("k_iSteamGameNotificationCallbacks %i\n", callback_id);
            break;
 
        case k_iSteamHTMLSurfaceCallbacks:
            PRINT_DEBUG("k_iSteamHTMLSurfaceCallbacks %i\n", callback_id);
            break;

        case k_iClientVideoCallbacks:
            PRINT_DEBUG("k_iClientVideoCallbacks %i\n", callback_id);
            break;

        case k_iClientInventoryCallbacks:
            PRINT_DEBUG("k_iClientInventoryCallbacks %i\n", callback_id);
            break;

        case k_iClientBluetoothManagerCallbacks:
            PRINT_DEBUG("k_iClientBluetoothManagerCallbacks %i\n", callback_id);
            break;

        case k_iClientSharedConnectionCallbacks:
            PRINT_DEBUG("k_iClientSharedConnectionCallbacks %i\n", callback_id);
            break;

        case k_ISteamParentalSettingsCallbacks:
            PRINT_DEBUG("k_ISteamParentalSettingsCallbacks %i\n", callback_id);
            break;

        case k_iClientShaderCallbacks:
            PRINT_DEBUG("k_iClientShaderCallbacks %i\n", callback_id);
            break;
        
        default:
            PRINT_DEBUG("Unknown callback base %i\n", base_callback);
    };

    if (isGameServer) {
        callbacks_server->rmCallBack(iCallback, pCallback);
    } else {
        callbacks_client->rmCallBack(iCallback, pCallback);
    }
}

void Steam_Client::RegisterCallResult( class CCallbackBase *pCallback, SteamAPICall_t hAPICall)
{
    PRINT_DEBUG("Steam_Client::RegisterCallResult %llu %i\n", hAPICall, pCallback->GetICallback());
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    callback_results_client->addCallBack(hAPICall, pCallback);
    callback_results_server->addCallBack(hAPICall, pCallback);
    
}

void Steam_Client::UnregisterCallResult( class CCallbackBase *pCallback, SteamAPICall_t hAPICall)
{
    PRINT_DEBUG("Steam_Client::UnregisterCallResult %llu %i\n", hAPICall, pCallback->GetICallback());
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    callback_results_client->rmCallBack(hAPICall, pCallback);
    callback_results_server->rmCallBack(hAPICall, pCallback);
}

void Steam_Client::RunCallbacks(bool runClientCB, bool runGameserverCB)
{
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    if (!network_keepalive.joinable()) network_keepalive = std::thread(network_thread, network);

    network->Run();
    PRINT_DEBUG("Steam_Client::RunCallbacks steam_matchmaking_servers\n");
    steam_matchmaking_servers->RunCallbacks();
    run_every_runcb->run();

    steam_gameserver->RunCallbacks();

    if (runClientCB) {
        PRINT_DEBUG("Steam_Client::RunCallbacks callback_results_client\n");
        callback_results_client->runCallResults();
    }

    if (runGameserverCB) {
        PRINT_DEBUG("Steam_Client::RunCallbacks callback_results_server\n");
        callback_results_server->runCallResults();
    }

    PRINT_DEBUG("Steam_Client::RunCallbacks callbacks_server\n");
    callbacks_server->runCallBacks();
    PRINT_DEBUG("Steam_Client::RunCallbacks callbacks_client\n");
    callbacks_client->runCallBacks();
    PRINT_DEBUG("Steam_Client::RunCallbacks done\n");
    
}
