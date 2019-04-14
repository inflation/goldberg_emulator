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

#include "dll.h"


static char old_client[128] = "SteamClient017";
static char old_gameserver[128] = "SteamGameServer012";
static char old_gameserver_stats[128] = "SteamGameServerStats001";
static char old_user[128] = "SteamUser018";
static char old_friends[128] = "SteamFriends015";
static char old_utils[128] = "SteamUtils007";
static char old_matchmaking[128] = "SteamMatchMaking009";
static char old_matchmaking_servers[128] = "SteamMatchMakingServers002";
static char old_userstats[128] = "STEAMUSERSTATS_INTERFACE_VERSION011";
static char old_apps[128] = "STEAMAPPS_INTERFACE_VERSION007";
static char old_networking[128] = "SteamNetworking005";
static char old_remote_storage_interface[128] = "STEAMREMOTESTORAGE_INTERFACE_VERSION013";
static char old_screenshots[128] = "STEAMSCREENSHOTS_INTERFACE_VERSION002";
static char old_http[128] = "STEAMHTTP_INTERFACE_VERSION002";
static char old_unified_messages[128] = "STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001";
static char old_controller[128] = "SteamController003";
static char old_ugc_interface[128] = "STEAMUGC_INTERFACE_VERSION007";
static char old_applist[128] = "STEAMAPPLIST_INTERFACE_VERSION001";
static char old_music[128] = "STEAMMUSIC_INTERFACE_VERSION001";
static char old_music_remote[128] = "STEAMMUSICREMOTE_INTERFACE_VERSION001";
static char old_html_surface[128] = "STEAMHTMLSURFACE_INTERFACE_VERSION_003";
static char old_inventory[128] = "STEAMINVENTORY_INTERFACE_V001";
static char old_video[128] = "STEAMVIDEO_INTERFACE_V001";
static char old_masterserver_updater[128] = "SteamMasterServerUpdater001";

#include <fstream>
static void load_old_interface_versions()
{
    static bool loaded = false;
    if (loaded) return;
    std::string interfaces_path = Local_Storage::get_program_path() + "steam_interfaces.txt";
    std::ifstream input( interfaces_path );
    PRINT_DEBUG("load from: %s\n", interfaces_path.c_str());

    for( std::string line; getline( input, line ); )
    {
        PRINT_DEBUG("line: %s\n", line.c_str());
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
#define REPLACE_WITH_FILE(s, f) {if (line.find(s) != std::string::npos) {strncpy(f, line.c_str(), sizeof(f) - 1); continue;}}
        REPLACE_WITH_FILE("SteamClient", old_client);
        REPLACE_WITH_FILE("SteamGameServerStats", old_gameserver_stats);
        REPLACE_WITH_FILE("SteamFriends", old_friends);
        REPLACE_WITH_FILE("SteamMatchMakingServers", old_matchmaking_servers);
        REPLACE_WITH_FILE("SteamMatchMaking", old_matchmaking);
        REPLACE_WITH_FILE("STEAMREMOTESTORAGE_INTERFACE_VERSION", old_remote_storage_interface);
        REPLACE_WITH_FILE("STEAMSCREENSHOTS_INTERFACE_VERSION", old_screenshots);
        REPLACE_WITH_FILE("STEAMHTTP_INTERFACE_VERSION", old_http);
        REPLACE_WITH_FILE("STEAMCONTROLLER_INTERFACE_VERSION", old_controller);
        REPLACE_WITH_FILE("SteamController", old_controller);
        REPLACE_WITH_FILE("STEAMUGC_INTERFACE_VERSION", old_ugc_interface);
        REPLACE_WITH_FILE("STEAMINVENTORY_INTERFACE", old_inventory);
        REPLACE_WITH_FILE("STEAMUSERSTATS_INTERFACE_VERSION", old_userstats);
        REPLACE_WITH_FILE("STEAMAPPS_INTERFACE_VERSION", old_apps);
        REPLACE_WITH_FILE("STEAMHTMLSURFACE_INTERFACE_VERSION", old_html_surface);
        REPLACE_WITH_FILE("SteamNetworking", old_networking);
        REPLACE_WITH_FILE("SteamUser", old_user);
        REPLACE_WITH_FILE("SteamUtils", old_utils);
        REPLACE_WITH_FILE("SteamGameServer", old_gameserver);
        PRINT_DEBUG("NOT REPLACED %s\n", line.c_str());
#undef REPLACE_WITH_FILE
    }

    PRINT_DEBUG("client: %s\n", old_client);
    PRINT_DEBUG("gameserver: %s\n", old_gameserver);
    PRINT_DEBUG("gameserver stats: %s\n", old_gameserver_stats);
    PRINT_DEBUG("user: %s\n", old_user);
    PRINT_DEBUG("friends: %s\n", old_friends);
    PRINT_DEBUG("matchmaking: %s\n", old_matchmaking);
    PRINT_DEBUG("remote: %s\n", old_remote_storage_interface);
    PRINT_DEBUG("screenshots: %s\n", old_screenshots);
    PRINT_DEBUG("http: %s\n", old_http);
    PRINT_DEBUG("controller %s\n", old_controller);
    PRINT_DEBUG("ugc: %s\n", old_ugc_interface);
    PRINT_DEBUG("inventory: %s\n", old_inventory);
    PRINT_DEBUG("userstats: %s\n", old_userstats);
    PRINT_DEBUG("apps: %s\n", old_apps);
    PRINT_DEBUG("networking: %s\n", old_networking);
    PRINT_DEBUG("html surface: %s\n", old_html_surface);
    PRINT_DEBUG("utils: %s\n", old_utils);
    loaded = true;
}

//steam_api_internal.h
S_API HSteamUser SteamAPI_GetHSteamUser()
{
    PRINT_DEBUG("SteamAPI_GetHSteamUser\n");
    return CLIENT_HSTEAMUSER;
}

S_API ISteamClient *g_pSteamClientGameServer;
ISteamClient *g_pSteamClientGameServer;

Steam_Client *get_steam_client()
{
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    static Steam_Client *client = new Steam_Client();
    if (!g_pSteamClientGameServer) g_pSteamClientGameServer = client;
    return client;
}

Steam_Client *get_steam_client_old()
{
    return get_steam_client();
}

Steam_Client *get_steam_clientserver_old()
{
    return get_steam_client();
}

S_API void * S_CALLTYPE SteamInternal_CreateInterface( const char *ver )
{
    PRINT_DEBUG("SteamInternal_CreateInterface %s\n", ver);
    void *steam_client;

    if (strcmp(ver, "SteamClient007") == 0) {
        steam_client = (ISteamClient007 *)get_steam_client();
    } else if (strcmp(ver, "SteamClient008") == 0) {
        steam_client = (ISteamClient008 *)get_steam_client();
    } else if (strcmp(ver, "SteamClient009") == 0) {
        steam_client = (ISteamClient009 *)get_steam_client();
    } else if (strcmp(ver, "SteamClient010") == 0) {
        steam_client = (ISteamClient010 *)get_steam_client();
    } else if (strcmp(ver, "SteamClient011") == 0) {
        steam_client = (ISteamClient011 *)get_steam_client();
    } else if (strcmp(ver, "SteamClient012") == 0) {
        steam_client = (ISteamClient012 *)get_steam_client();
    } else if (strcmp(ver, "SteamClient013") == 0) {
        steam_client = (ISteamClient013 *)get_steam_client();
    } else if (strcmp(ver, "SteamClient014") == 0) {
        steam_client = (ISteamClient014 *)get_steam_client();
    } else if (strcmp(ver, "SteamClient015") == 0) {
        steam_client = (ISteamClient015 *)get_steam_client();
    } else if (strcmp(ver, "SteamClient016") == 0) {
        steam_client = (ISteamClient016 *)get_steam_client();
    } else if (strcmp(ver, "SteamClient017") == 0) {
        steam_client = (ISteamClient017 *)get_steam_client();
    } else if (strcmp(ver, STEAMCLIENT_INTERFACE_VERSION) == 0) {
        steam_client = (ISteamClient *)get_steam_client();
    } else {
        steam_client = (ISteamClient *)get_steam_client();
    }

    if (steam_client) g_pSteamClientGameServer = (ISteamClient *)steam_client;
    return steam_client;
}

struct ContextInitData { void (*pFn)(void* pCtx); uintp counter; CSteamAPIContext ctx; };

S_API void * S_CALLTYPE SteamInternal_ContextInit( void *pContextInitData )
{
    //PRINT_DEBUG("SteamInternal_ContextInit\n");
    struct ContextInitData *contextInitData = (struct ContextInitData *)pContextInitData;
    if (!contextInitData->counter) {
        PRINT_DEBUG("SteamInternal_ContextInit initializing\n");
        contextInitData->pFn(&contextInitData->ctx);
        //this is hackish but whatever.
        if (contextInitData->ctx.SteamUser()) contextInitData->counter = 1;
    }

    return &contextInitData->ctx;
}

//steam_api.h
// SteamAPI_Init must be called before using any other API functions. If it fails, an
// error message will be output to the debugger (or stderr) with further information.
S_API bool S_CALLTYPE SteamAPI_Init()
{
    PRINT_DEBUG("SteamAPI_Init called\n");
#ifdef EMU_EXPERIMENTAL_BUILD
    crack_SteamAPI_Init();
#endif
    load_old_interface_versions();
    get_steam_client()->userLogIn();
    return true;
}

//TODO: not sure if this is the right signature for this function.
S_API bool S_CALLTYPE SteamAPI_InitAnonymousUser()
{
    PRINT_DEBUG("SteamAPI_InitAnonymousUser called\n");
    return SteamAPI_Init();
}

// SteamAPI_Shutdown should be called during process shutdown if possible.
S_API void S_CALLTYPE SteamAPI_Shutdown()
{
    PRINT_DEBUG("SteamAPI_Shutdown\n");
}

// SteamAPI_RestartAppIfNecessary ensures that your executable was launched through Steam.
//
// Returns true if the current process should terminate. Steam is now re-launching your application.
//
// Returns false if no action needs to be taken. This means that your executable was started through
// the Steam client, or a steam_appid.txt file is present in your game's directory (for development).
// Your current process should continue if false is returned.
//
// NOTE: If you use the Steam DRM wrapper on your primary executable file, this check is unnecessary
// since the DRM wrapper will ensure that your application was launched properly through Steam.
S_API bool S_CALLTYPE SteamAPI_RestartAppIfNecessary( uint32 unOwnAppID )
{
    PRINT_DEBUG("SteamAPI_RestartAppIfNecessary %u\n", unOwnAppID);
#ifdef EMU_EXPERIMENTAL_BUILD
    crack_SteamAPI_RestartAppIfNecessary(unOwnAppID);
#endif
    get_steam_client()->setAppID(unOwnAppID);
    return false;
}

// Many Steam API functions allocate a small amount of thread-local memory for parameter storage.
// SteamAPI_ReleaseCurrentThreadMemory() will free API memory associated with the calling thread.
// This function is also called automatically by SteamAPI_RunCallbacks(), so a single-threaded
// program never needs to explicitly call this function.
S_API void S_CALLTYPE SteamAPI_ReleaseCurrentThreadMemory()
{
    PRINT_DEBUG("SteamAPI_ReleaseCurrentThreadMemory\n");
}

// crash dump recording functions
S_API void S_CALLTYPE SteamAPI_WriteMiniDump( uint32 uStructuredExceptionCode, void* pvExceptionInfo, uint32 uBuildID )
{
    PRINT_DEBUG("SteamAPI_WriteMiniDump\n");
}

S_API void S_CALLTYPE SteamAPI_SetMiniDumpComment( const char *pchMsg )
{
    PRINT_DEBUG("SteamAPI_SetMiniDumpComment: %s\n", pchMsg);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------//
//	steam callback and call-result helpers
//
//	The following macros and classes are used to register your application for
//	callbacks and call-results, which are delivered in a predictable manner.
//
//	STEAM_CALLBACK macros are meant for use inside of a C++ class definition.
//	They map a Steam notification callback directly to a class member function
//	which is automatically prototyped as "void func( callback_type *pParam )".
//
//	CCallResult is used with specific Steam APIs that return "result handles".
//	The handle can be passed to a CCallResult object's Set function, along with
//	an object pointer and member-function pointer. The member function will
//	be executed once the results of the Steam API call are available.
//
//	CCallback and CCallbackManual classes can be used instead of STEAM_CALLBACK
//	macros if you require finer control over registration and unregistration.
//
//	Callbacks and call-results are queued automatically and are only
//	delivered/executed when your application calls SteamAPI_RunCallbacks().
//----------------------------------------------------------------------------------------------------------------------------------------------------------//

// SteamAPI_RunCallbacks is safe to call from multiple threads simultaneously,
// but if you choose to do this, callback code could be executed on any thread.
// One alternative is to call SteamAPI_RunCallbacks from the main thread only,
// and call SteamAPI_ReleaseCurrentThreadMemory regularly on other threads.
S_API void S_CALLTYPE SteamAPI_RunCallbacks()
{
    PRINT_DEBUG("SteamAPI_RunCallbacks\n");
    get_steam_client()->RunCallbacks(true, false);
    std::this_thread::sleep_for(std::chrono::microseconds(1)); //fixes resident evil revelations lagging.
}


// Declares a callback member function plus a helper member variable which
// registers the callback on object creation and unregisters on destruction.
// The optional fourth 'var' param exists only for backwards-compatibility
// and can be ignored.
//#define STEAM_CALLBACK( thisclass, func, .../*callback_type, [deprecated] var*/ ) \
//	_STEAM_CALLBACK_SELECT( ( __VA_ARGS__, 4, 3 ), ( /**/, thisclass, func, __VA_ARGS__ ) )

// Declares a callback function and a named CCallbackManual variable which
// has Register and Unregister functions instead of automatic registration.
//#define STEAM_CALLBACK_MANUAL( thisclass, func, callback_type, var )	\
//	CCallbackManual< thisclass, callback_type > var; void func( callback_type *pParam )


// Internal functions used by the utility CCallback objects to receive callbacks
S_API void S_CALLTYPE SteamAPI_RegisterCallback( class CCallbackBase *pCallback, int iCallback )
{
    PRINT_DEBUG("SteamAPI_RegisterCallback %p %u funct:%u\n", pCallback, iCallback, pCallback->GetICallback());
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    get_steam_client()->RegisterCallback(pCallback, iCallback);
}

S_API void S_CALLTYPE SteamAPI_UnregisterCallback( class CCallbackBase *pCallback )
{
    PRINT_DEBUG("SteamAPI_UnregisterCallback %p\n", pCallback);
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    get_steam_client()->UnregisterCallback(pCallback);
}

// Internal functions used by the utility CCallResult objects to receive async call results
S_API void S_CALLTYPE SteamAPI_RegisterCallResult( class CCallbackBase *pCallback, SteamAPICall_t hAPICall )
{
    PRINT_DEBUG("SteamAPI_RegisterCallResult\n");
    if (!hAPICall)
        return;

    get_steam_client()->RegisterCallResult(pCallback, hAPICall);
}

S_API void S_CALLTYPE SteamAPI_UnregisterCallResult( class CCallbackBase *pCallback, SteamAPICall_t hAPICall )
{
    PRINT_DEBUG("SteamAPI_UnregisterCallResult\n");
    if (!hAPICall)
        return;

    get_steam_client()->UnregisterCallResult(pCallback, hAPICall);
}

S_API void *S_CALLTYPE SteamInternal_FindOrCreateUserInterface( HSteamUser hSteamUser, const char *pszVersion )
{
    PRINT_DEBUG("SteamInternal_FindOrCreateUserInterface %i %s\n", hSteamUser, pszVersion);
    return get_steam_client()->GetISteamGenericInterface(hSteamUser, SteamAPI_GetHSteamPipe(), pszVersion);
}

S_API void *S_CALLTYPE SteamInternal_FindOrCreateGameServerInterface( HSteamUser hSteamUser, const char *pszVersion )
{
    PRINT_DEBUG("SteamInternal_FindOrCreateGameServerInterface %i %s\n", hSteamUser, pszVersion);
    return get_steam_client()->GetISteamGenericInterface(hSteamUser, SteamGameServer_GetHSteamPipe(), pszVersion);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------//
//	steamclient.dll private wrapper functions
//
//	The following functions are part of abstracting API access to the steamclient.dll, but should only be used in very specific cases
//----------------------------------------------------------------------------------------------------------------------------------------------------------//

// SteamAPI_IsSteamRunning() returns true if Steam is currently running
S_API bool S_CALLTYPE SteamAPI_IsSteamRunning()
{
    PRINT_DEBUG("SteamAPI_IsSteamRunning\n");
    return true;
}

// Pumps out all the steam messages, calling registered callbacks.
// NOT THREADSAFE - do not call from multiple threads simultaneously.
S_API void Steam_RunCallbacks( HSteamPipe hSteamPipe, bool bGameServerCallbacks )
{
    PRINT_DEBUG("Steam_RunCallbacks\n");

    SteamAPI_RunCallbacks();

    if (bGameServerCallbacks)
        SteamGameServer_RunCallbacks();
}

// register the callback funcs to use to interact with the steam dll
S_API void Steam_RegisterInterfaceFuncs( void *hModule )
{
    PRINT_DEBUG("Steam_RegisterInterfaceFuncs\n");
}

// returns the HSteamUser of the last user to dispatch a callback
S_API HSteamUser Steam_GetHSteamUserCurrent()
{
    PRINT_DEBUG("Steam_GetHSteamUserCurrent\n");
    //TODO?
    return CLIENT_HSTEAMUSER;
}

// returns the filename path of the current running Steam process, used if you need to load an explicit steam dll by name.
// DEPRECATED - implementation is Windows only, and the path returned is a UTF-8 string which must be converted to UTF-16 for use with Win32 APIs
S_API const char *SteamAPI_GetSteamInstallPath()
{
    PRINT_DEBUG("SteamAPI_GetSteamInstallPath\n");
    static char steam_folder[1024];
    std::string path = Local_Storage::get_program_path();
    strcpy(steam_folder, path.c_str());
    steam_folder[path.length() - 1] = 0;
    return steam_folder;
}

// returns the pipe we are communicating to Steam with
S_API HSteamPipe SteamAPI_GetHSteamPipe()
{
    PRINT_DEBUG("SteamAPI_GetHSteamPipe\n");
    return CLIENT_STEAM_PIPE;
}

// sets whether or not Steam_RunCallbacks() should do a try {} catch (...) {} around calls to issuing callbacks
S_API void SteamAPI_SetTryCatchCallbacks( bool bTryCatchCallbacks )
{
    PRINT_DEBUG("SteamAPI_SetTryCatchCallbacks\n");
}

// backwards compat export, passes through to SteamAPI_ variants
S_API HSteamPipe GetHSteamPipe()
{
    PRINT_DEBUG("GetHSteamPipe\n");
    return CLIENT_STEAM_PIPE;
}

S_API HSteamUser GetHSteamUser()
{
    PRINT_DEBUG("GetHSteamUser\n");
    return CLIENT_HSTEAMUSER;
}


// exists only for backwards compat with code written against older SDKs
S_API bool S_CALLTYPE SteamAPI_InitSafe()
{
    PRINT_DEBUG("SteamAPI_InitSafe\n");
    SteamAPI_Init();
    return true;
}

S_API ISteamClient *SteamClient() {
    PRINT_DEBUG("SteamClient()\n");
    load_old_interface_versions();
    return (ISteamClient *)SteamInternal_CreateInterface(old_client);
}
S_API ISteamUser *SteamUser()								{ PRINT_DEBUG("SteamUser()\n");return get_steam_client_old()->GetISteamUser(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_user); }
S_API ISteamFriends *SteamFriends()						{ PRINT_DEBUG("SteamFriends()\n");return get_steam_client_old()->GetISteamFriends(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_friends ); }
S_API ISteamUtils *SteamUtils()							{ PRINT_DEBUG("SteamUtils()\n");return get_steam_client_old()->GetISteamUtils(SteamAPI_GetHSteamPipe(), old_utils); }
S_API ISteamMatchmaking *SteamMatchmaking()				{ PRINT_DEBUG("SteamMatchmaking()\n");return get_steam_client_old()->GetISteamMatchmaking(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_matchmaking); }
S_API ISteamUserStats *SteamUserStats()					{ PRINT_DEBUG("SteamUserStats()\n");return get_steam_client_old()->GetISteamUserStats(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_userstats); }
S_API ISteamApps *SteamApps()								{ PRINT_DEBUG("SteamApps()\n");return get_steam_client_old()->GetISteamApps(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_apps); }
S_API ISteamMatchmakingServers *SteamMatchmakingServers()	{ PRINT_DEBUG("SteamMatchmakingServers()\n");return get_steam_client_old()->GetISteamMatchmakingServers(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_matchmaking_servers); }
S_API ISteamNetworking *SteamNetworking()					{ PRINT_DEBUG("SteamNetworking()\n");return get_steam_client_old()->GetISteamNetworking(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_networking); }
S_API ISteamRemoteStorage *SteamRemoteStorage()			{ PRINT_DEBUG("SteamRemoteStorage()\n");return get_steam_client_old()->GetISteamRemoteStorage(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_remote_storage_interface); }
S_API ISteamScreenshots *SteamScreenshots()				{ PRINT_DEBUG("SteamScreenshots()\n");return get_steam_client_old()->GetISteamScreenshots(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_screenshots); }
S_API ISteamHTTP *SteamHTTP()								{ PRINT_DEBUG("SteamHTTP()\n");return get_steam_client_old()->GetISteamHTTP(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_http); }
S_API ISteamController *SteamController()					{ PRINT_DEBUG("SteamController()\n");return get_steam_client_old()->GetISteamController(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_controller); }
S_API ISteamUGC *SteamUGC()								{ PRINT_DEBUG("SteamUGC()\n");return get_steam_client_old()->GetISteamUGC(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_ugc_interface ); }
S_API ISteamAppList *SteamAppList()						{ PRINT_DEBUG("SteamAppList()\n");return get_steam_client_old()->GetISteamAppList(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_applist); }
S_API ISteamMusic *SteamMusic()							{ PRINT_DEBUG("SteamMusic()\n");return get_steam_client_old()->GetISteamMusic(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_music); }
S_API ISteamMusicRemote *SteamMusicRemote()				{ PRINT_DEBUG("SteamMusicRemote()\n");return get_steam_client_old()->GetISteamMusicRemote(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_music_remote); }
S_API ISteamHTMLSurface *SteamHTMLSurface()				{ PRINT_DEBUG("SteamHTMLSurface()\n");return get_steam_client_old()->GetISteamHTMLSurface(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_html_surface); }
S_API ISteamInventory *SteamInventory()					{ PRINT_DEBUG("SteamInventory()\n");return get_steam_client_old()->GetISteamInventory(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_inventory); }
S_API ISteamVideo *SteamVideo()							{ PRINT_DEBUG("SteamVideo()\n");return get_steam_client_old()->GetISteamVideo(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), old_video); }
S_API ISteamParentalSettings *SteamParentalSettings()		{ PRINT_DEBUG("SteamParentalSettings()\n");return get_steam_client_old()->GetISteamParentalSettings(SteamAPI_GetHSteamUser(), SteamAPI_GetHSteamPipe(), ""); }



//Gameserver stuff

S_API void * S_CALLTYPE SteamGameServerInternal_CreateInterface( const char *ver )
{
    PRINT_DEBUG("SteamGameServerInternal_CreateInterface %s\n", ver);
    return SteamInternal_CreateInterface(ver);
}

S_API HSteamPipe S_CALLTYPE SteamGameServer_GetHSteamPipe()
{
    PRINT_DEBUG("SteamGameServer_GetHSteamPipe\n");
    return SERVER_STEAM_PIPE;
}

S_API HSteamUser S_CALLTYPE SteamGameServer_GetHSteamUser()
{
    PRINT_DEBUG("SteamGameServer_GetHSteamUser\n");
    return SERVER_HSTEAMUSER;
}

S_API bool S_CALLTYPE SteamGameServer_InitSafe(uint32 unIP, uint16 usSteamPort, uint16 usGamePort, uint16 usQueryPort, EServerMode eServerMode, const char *pchVersionString )
{
    return SteamInternal_GameServer_Init( unIP, usSteamPort, usGamePort, usQueryPort, eServerMode, pchVersionString );
}

S_API bool S_CALLTYPE SteamInternal_GameServer_Init( uint32 unIP, uint16 usPort, uint16 usGamePort, uint16 usQueryPort, EServerMode eServerMode, const char *pchVersionString )
{
    PRINT_DEBUG("SteamInternal_GameServer_Init %u %hu %hu %hu %u %s\n", unIP, usPort, usGamePort, usQueryPort, eServerMode, pchVersionString);
    SteamGameServerClient();
    get_steam_client()->serverInit();
    return get_steam_client()->GetISteamGameServer(SERVER_HSTEAMUSER, SERVER_STEAM_PIPE, /* TODO: figure out right interface version */old_gameserver)->InitGameServer(unIP, usGamePort, usQueryPort, eServerMode, 0, pchVersionString);
}

S_API bool SteamGameServer_Init( uint32 unIP, uint16 usSteamPort, uint16 usGamePort, uint16 usQueryPort, EServerMode eServerMode, const char *pchVersionString )
{
    return SteamInternal_GameServer_Init( unIP, usSteamPort, usGamePort, usQueryPort, eServerMode, pchVersionString );
}

S_API void SteamGameServer_Shutdown()
{
    PRINT_DEBUG("SteamGameServer_Shutdown\n");
    get_steam_clientserver_old()->serverShutdown();
}

S_API void SteamGameServer_RunCallbacks()
{
    PRINT_DEBUG("SteamGameServer_RunCallbacks\n");
    get_steam_client()->RunCallbacks(false, true);
}

S_API bool SteamGameServer_BSecure()
{
    PRINT_DEBUG("SteamGameServer_BSecure\n");
    return true;
}

S_API uint64 SteamGameServer_GetSteamID()
{
    PRINT_DEBUG("SteamGameServer_GetSteamID\n");
    ISteamGameServer *gs = get_steam_client()->GetISteamGameServer(SERVER_HSTEAMUSER, SERVER_STEAM_PIPE, /* TODO: figure out right interface version */old_gameserver);
    if (!gs) return 0;
    return gs->GetSteamID().ConvertToUint64();
}

S_API ISteamClient *SteamGameServerClient() { 
    PRINT_DEBUG("SteamGameServerClient()\n");
    load_old_interface_versions();
    get_steam_clientserver_old();
    return (ISteamClient *)SteamInternal_CreateInterface(old_client); 
}

S_API ISteamGameServer *SteamGameServer() { PRINT_DEBUG("SteamGameServer()\n"); return get_steam_clientserver_old()->GetISteamGameServer(SteamGameServer_GetHSteamUser(), SteamGameServer_GetHSteamPipe(), old_gameserver ); }
S_API ISteamUtils *SteamGameServerUtils() { PRINT_DEBUG("SteamGameServerUtils()\n"); return get_steam_clientserver_old()->GetISteamUtils(SteamGameServer_GetHSteamPipe(), old_utils ); }
S_API ISteamNetworking *SteamGameServerNetworking() { PRINT_DEBUG("SteamGameServerNetworking()\n"); return get_steam_clientserver_old()->GetISteamNetworking(SteamGameServer_GetHSteamUser(), SteamGameServer_GetHSteamPipe(), old_networking ); }
S_API ISteamGameServerStats *SteamGameServerStats() { PRINT_DEBUG("SteamGameServerStats()\n"); return get_steam_clientserver_old()->GetISteamGameServerStats(SteamGameServer_GetHSteamUser(), SteamGameServer_GetHSteamPipe(), old_gameserver_stats ); }
S_API ISteamHTTP *SteamGameServerHTTP() { PRINT_DEBUG("SteamGameServerHTTP()\n"); return get_steam_clientserver_old()->GetISteamHTTP(SteamGameServer_GetHSteamUser(), SteamGameServer_GetHSteamPipe(), old_http ); }
S_API ISteamInventory *SteamGameServerInventory() { PRINT_DEBUG("SteamGameServerInventory()\n"); return get_steam_clientserver_old()->GetISteamInventory(SteamGameServer_GetHSteamUser(), SteamGameServer_GetHSteamPipe(), old_inventory ); }
S_API ISteamUGC *SteamGameServerUGC() { PRINT_DEBUG("SteamGameServerUGC()\n"); return get_steam_clientserver_old()->GetISteamUGC(SteamGameServer_GetHSteamUser(), SteamGameServer_GetHSteamPipe(), old_ugc_interface ); }
S_API ISteamApps *SteamGameServerApps() { PRINT_DEBUG("SteamGameServerApps()\n"); return get_steam_clientserver_old()->GetISteamApps(SteamGameServer_GetHSteamUser(), SteamGameServer_GetHSteamPipe(), old_apps ); }

S_API ISteamMasterServerUpdater *SteamMasterServerUpdater() {PRINT_DEBUG("SteamMasterServerUpdater()\n"); return get_steam_clientserver_old()->GetISteamMasterServerUpdater(SteamGameServer_GetHSteamUser(), SteamGameServer_GetHSteamPipe(), old_masterserver_updater); }

S_API uint32 SteamGameServer_GetIPCCallCount()
{
    return get_steam_client()->GetIPCCallCount();
}


S_API void S_CALLTYPE SteamAPI_UseBreakpadCrashHandler( char const *pchVersion, char const *pchDate, char const *pchTime, bool bFullMemoryDumps, void *pvContext, PFNPreMinidumpCallback m_pfnPreMinidumpCallback )
{
    
}

S_API void S_CALLTYPE SteamAPI_SetBreakpadAppID( uint32 unAppID )
{

}

//VR stuff
S_API void *VR_Init(int *error, int type)
{
    if (error) *error = 108; //HmdError_Init_HmdNotFound
    return NULL;
}

S_API void *VR_GetGenericInterface( const char *pchInterfaceVersion, int *peError )
{
    return NULL;
}

S_API const char *VR_GetStringForHmdError( int error )
{
    return "";
}

S_API bool VR_IsHmdPresent()
{
    return false;
}

S_API void VR_Shutdown()
{

}

S_API bool SteamAPI_RestartApp( uint32 appid )
{
    PRINT_DEBUG("SteamAPI_RestartApp %u\n", appid);
    return SteamAPI_RestartAppIfNecessary(appid);
}

//OLD steam_c stuff
/*

ISteamApps_BIsCybercafe
ISteamApps_BIsLowViolence
ISteamApps_BIsSubscribed
ISteamApps_BIsSubscribedApp
ISteamApps_GetAvailableGameLanguages
ISteamApps_GetCurrentGameLanguage
ISteamClient_SetLocalIPBinding
ISteamGameServer_BLoggedOn
ISteamGameServer_BSecure
ISteamGameServer_BUpdateUserData
ISteamGameServer_CreateUnauthenticatedUserConnection
ISteamGameServer_GetSteamID
ISteamGameServer_SendUserConnectAndAuthenticate
ISteamGameServer_SendUserDisconnect
ISteamGameServer_SetGameType
ISteamGameServer_SetServerType
ISteamGameServer_UpdateSpectatorPort
ISteamGameServer_UpdateStatus
S_API bool ISteamMasterServerUpdater_AddMasterServer
ISteamMasterServerUpdater_ClearAllKeyValues
ISteamMasterServerUpdater_ForceHeartbeat
ISteamMasterServerUpdater_GetMasterServerAddress
ISteamMasterServerUpdater_GetNextOutgoingPacket
ISteamMasterServerUpdater_GetNumMasterServers
ISteamMasterServerUpdater_HandleIncomingPacket
ISteamMasterServerUpdater_NotifyShutdown
ISteamMasterServerUpdater_RemoveMasterServer
ISteamMasterServerUpdater_SetActive
ISteamMasterServerUpdater_SetBasicServerData
ISteamMasterServerUpdater_SetHeartbeatInterval
ISteamMasterServerUpdater_SetKeyValue
ISteamMasterServerUpdater_WasRestartRequested
ISteamUser_BLoggedOn
ISteamUser_InitiateGameConnection
ISteamUser_TerminateGameConnection
ISteamUtils_GetAppID

SteamContentServer
SteamContentServerUtils
SteamContentServer_Init
SteamContentServer_RunCallbacks
SteamContentServer_Shutdown


SteamGameServer_BSecure
SteamGameServer_GetHSteamPipe
SteamGameServer_GetHSteamUser
SteamGameServer_GetIPCCallCount
SteamGameServer_GetSteamID
SteamGameServer_Init
SteamGameServer_InitSafe
SteamGameServer_RunCallbacks
SteamGameServer_Shutdown

SteamMasterServerUpdater


S_API bool Steam_BGetCallback( HSteamPipe hSteamPipe, CallbackMsg_t *pCallbackMsg )
{
    return false;
}

S_API void Steam_FreeLastCallback( HSteamPipe hSteamPipe )
{

}

S_API bool Steam_GetAPICallResult( HSteamPipe hSteamPipe, SteamAPICall_t hSteamAPICall, void* pCallback, int cubCallback, int iCallbackExpected, bool* pbFailed )
{
    return false;
}

*/
