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
Steam_Client *get_steam_client();

S_API ISteamClient *SteamClient();
S_API ISteamUser *SteamUser();
S_API ISteamFriends *SteamFriends();
S_API ISteamUtils *SteamUtils();
S_API ISteamMatchmaking *SteamMatchmaking();
S_API ISteamUserStats *SteamUserStats();
S_API ISteamApps *SteamApps();
S_API ISteamNetworking *SteamNetworking();
S_API ISteamMatchmakingServers *SteamMatchmakingServers();
S_API ISteamRemoteStorage *SteamRemoteStorage();
S_API ISteamScreenshots *SteamScreenshots();
S_API ISteamHTTP *SteamHTTP();
S_API ISteamController *SteamController();
S_API ISteamUGC *SteamUGC();
S_API ISteamAppList *SteamAppList();
S_API ISteamMusic *SteamMusic();
S_API ISteamMusicRemote *SteamMusicRemote();
S_API ISteamHTMLSurface *SteamHTMLSurface();
S_API ISteamInventory *SteamInventory();
S_API ISteamVideo *SteamVideo();
S_API ISteamParentalSettings *SteamParentalSettings();


S_API ISteamClient *SteamGameServerClient();
S_API ISteamGameServer *SteamGameServer();
S_API ISteamUtils *SteamGameServerUtils();
S_API ISteamNetworking *SteamGameServerNetworking();
S_API ISteamGameServerStats *SteamGameServerStats();
S_API ISteamHTTP *SteamGameServerHTTP();
S_API ISteamInventory *SteamGameServerInventory();
S_API ISteamUGC *SteamGameServerUGC();
S_API ISteamApps *SteamGameServerApps();
