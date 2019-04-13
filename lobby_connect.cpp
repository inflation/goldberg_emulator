/*
This is the source code for the lobby_connect.exe program.
If you want to build it, just dl the steam sdk and link it to the steam api dll
Then to execute it just use my emu dll instead of the steam sdk dll.

If you make any improvements and want to share them post your code in the thread and if it improves it I'll use it.

You can also use this code to add this functionality to your launchers.

Note if you use my steam emu dll: When I build my lobby_connect.exe I static link it to a build of my emu that has reading from disk disabled.
This means that it's a good idea for you to use local_save.txt so that your launcher doesn't conflict with running
games by having the same steam user id. There must also be no steam_appid.txt or else you will only see games with that appid.
*/

#include "sdk_includes/steam_api.h"
#include "dll/defines.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <windows.h>

int main() {
    if (SteamAPI_Init()) {
        //Set appid to: LOBBY_CONNECT_APPID
        SteamAPI_RestartAppIfNecessary(LOBBY_CONNECT_APPID);
        std::cout << "This is a program to find lobbies and run the game with lobby connect parameters" << std::endl;
        std::cout << "Api initialized, ";
top:
        std::cout << "waiting a few seconds for connections:" << std::endl;
        for (int i = 0; i < 10; ++i) {
            SteamAPI_RunCallbacks();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        int friend_count = SteamFriends()->GetFriendCount(0);
        std::cout << "People on the network: " << friend_count << std::endl;
        for (int i = 0; i < friend_count; ++i) {
            CSteamID id = SteamFriends()->GetFriendByIndex(i, 0);
            const char *name = SteamFriends()->GetFriendPersonaName(id);

            FriendGameInfo_t friend_info = {};
            SteamFriends()->GetFriendGamePlayed(id, &friend_info);
            std::cout << name << " is playing: " << friend_info.m_gameID.AppID() << std::endl;
        }

        std::cout << std::endl << "--------------Menu-------------" << std::endl << "\tappid\tname\tcommand line" << std::endl;

        std::vector<std::string> arguments;
        for (int i = 0; i < friend_count; ++i) {
            CSteamID id = SteamFriends()->GetFriendByIndex(i, 0);
            const char *name = SteamFriends()->GetFriendPersonaName(id);
            const char *connect = SteamFriends()->GetFriendRichPresence( id, "connect");
            FriendGameInfo_t friend_info = {};
            SteamFriends()->GetFriendGamePlayed(id, &friend_info);

            if (strlen(connect) > 0) {
                std::cout << arguments.size() << "\t" << friend_info.m_gameID.AppID() << "\t" << name << "\t" << connect << std::endl;
                arguments.push_back(connect);
            } else {
                if (friend_info.m_steamIDLobby != k_steamIDNil) {
                    std::string connect = "+connect_lobby " + std::to_string(friend_info.m_steamIDLobby.ConvertToUint64());
                    std::cout << arguments.size() << "\t" << friend_info.m_gameID.AppID() << "\t" << name << "\t" << connect << std::endl;
                    arguments.push_back(connect);
                }
            }
        }

        std::cout << arguments.size() << ": Retry." << std::endl;
        std::cout << std::endl << "Enter the number corresponding to your choice then press Enter." << std::endl;
        unsigned int choice;
        std::cin >> choice;

        if (choice >= arguments.size()) goto top;

        std::cout << "starting the game with: " << arguments[choice] << std::endl << "Please select the game exe" << std::endl;

        OPENFILENAMEA ofn;
        char szFileName[MAX_PATH] = "";
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = 0;
        ofn.lpstrFilter = "Exe Files (*.exe)\0*.exe\0All Files (*.*)\0*.*\0";
        ofn.lpstrFile = szFileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        ofn.lpstrDefExt = "txt";
        if(GetOpenFileNameA(&ofn))
        {
            std::string filename = szFileName;
            filename = "\"" + filename + "\" " + arguments[choice];
            std::cout << filename << std::endl;
            STARTUPINFOA lpStartupInfo;
            PROCESS_INFORMATION lpProcessInfo;

            ZeroMemory( &lpStartupInfo, sizeof( lpStartupInfo ) );
            lpStartupInfo.cb = sizeof( lpStartupInfo );
            ZeroMemory( &lpProcessInfo, sizeof( lpProcessInfo ) );

            CreateProcessA( NULL,
                        const_cast<char *>(filename.c_str()), NULL, NULL,
                        NULL, NULL, NULL, NULL,
                        &lpStartupInfo,
                        &lpProcessInfo
                        );
        }
    }
}
