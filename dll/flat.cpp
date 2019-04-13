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

S_API HSteamPipe SteamAPI_ISteamClient_CreateSteamPipe(intptr_t instancePtr)
{
    return get_steam_client()->CreateSteamPipe();
}

S_API bool SteamAPI_ISteamClient_BReleaseSteamPipe(intptr_t instancePtr, HSteamPipe hSteamPipe)
{
    return get_steam_client()->BReleaseSteamPipe(hSteamPipe);
}

S_API HSteamUser SteamAPI_ISteamClient_ConnectToGlobalUser(intptr_t instancePtr, HSteamPipe hSteamPipe)
{
    return get_steam_client()->ConnectToGlobalUser(hSteamPipe);
}

S_API HSteamUser SteamAPI_ISteamClient_CreateLocalUser(intptr_t instancePtr, HSteamPipe * phSteamPipe, EAccountType eAccountType)
{
    return get_steam_client()->CreateLocalUser(phSteamPipe, eAccountType);
}

S_API void SteamAPI_ISteamClient_ReleaseUser(intptr_t instancePtr, HSteamPipe hSteamPipe, HSteamUser hUser)
{
    return get_steam_client()->ReleaseUser(hSteamPipe, hUser);
}

S_API class ISteamUser * SteamAPI_ISteamClient_GetISteamUser(intptr_t instancePtr, HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamUser(hSteamUser, hSteamPipe, pchVersion);
}

S_API class ISteamGameServer * SteamAPI_ISteamClient_GetISteamGameServer(intptr_t instancePtr, HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamGameServer(hSteamUser, hSteamPipe, pchVersion);
}

S_API void SteamAPI_ISteamClient_SetLocalIPBinding(intptr_t instancePtr, uint32 unIP, uint16 usPort)
{
    return get_steam_client()->SetLocalIPBinding(unIP, usPort);
}

S_API class ISteamFriends * SteamAPI_ISteamClient_GetISteamFriends(intptr_t instancePtr, HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamFriends(hSteamUser, hSteamPipe, pchVersion);
}

S_API class ISteamUtils * SteamAPI_ISteamClient_GetISteamUtils(intptr_t instancePtr, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamUtils(hSteamPipe, pchVersion);
}

S_API class ISteamMatchmaking * SteamAPI_ISteamClient_GetISteamMatchmaking(intptr_t instancePtr, HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamMatchmaking(hSteamUser, hSteamPipe, pchVersion);
}

S_API class ISteamMatchmakingServers * SteamAPI_ISteamClient_GetISteamMatchmakingServers(intptr_t instancePtr, HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamMatchmakingServers(hSteamUser, hSteamPipe, pchVersion);
}

S_API void * SteamAPI_ISteamClient_GetISteamGenericInterface(intptr_t instancePtr, HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamGenericInterface(hSteamUser, hSteamPipe, pchVersion);
}

S_API class ISteamUserStats * SteamAPI_ISteamClient_GetISteamUserStats(intptr_t instancePtr, HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamUserStats(hSteamUser, hSteamPipe, pchVersion);
}

S_API class ISteamGameServerStats * SteamAPI_ISteamClient_GetISteamGameServerStats(intptr_t instancePtr, HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamGameServerStats(hSteamuser, hSteamPipe, pchVersion);
}

S_API class ISteamApps * SteamAPI_ISteamClient_GetISteamApps(intptr_t instancePtr, HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamApps(hSteamUser, hSteamPipe, pchVersion);
}

S_API class ISteamNetworking * SteamAPI_ISteamClient_GetISteamNetworking(intptr_t instancePtr, HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamNetworking(hSteamUser, hSteamPipe, pchVersion);
}

S_API class ISteamRemoteStorage * SteamAPI_ISteamClient_GetISteamRemoteStorage(intptr_t instancePtr, HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamRemoteStorage(hSteamuser, hSteamPipe, pchVersion);
}

S_API class ISteamScreenshots * SteamAPI_ISteamClient_GetISteamScreenshots(intptr_t instancePtr, HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamScreenshots(hSteamuser, hSteamPipe, pchVersion);
}

S_API class ISteamGameSearch * SteamAPI_ISteamClient_GetISteamGameSearch(intptr_t instancePtr, HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamGameSearch(hSteamuser, hSteamPipe, pchVersion);
}

S_API uint32 SteamAPI_ISteamClient_GetIPCCallCount(intptr_t instancePtr)
{
    return get_steam_client()->GetIPCCallCount();
}

S_API void SteamAPI_ISteamClient_SetWarningMessageHook(intptr_t instancePtr, SteamAPIWarningMessageHook_t pFunction)
{
    return get_steam_client()->SetWarningMessageHook(pFunction);
}

S_API bool SteamAPI_ISteamClient_BShutdownIfAllPipesClosed(intptr_t instancePtr)
{
    return get_steam_client()->BShutdownIfAllPipesClosed();
}

S_API class ISteamHTTP * SteamAPI_ISteamClient_GetISteamHTTP(intptr_t instancePtr, HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamHTTP(hSteamuser,hSteamPipe,pchVersion);
}

S_API void *SteamAPI_ISteamClient_GetISteamUnifiedMessages(intptr_t instancePtr, HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->DEPRECATED_GetISteamUnifiedMessages(hSteamuser,hSteamPipe,pchVersion);
}

S_API class ISteamController * SteamAPI_ISteamClient_GetISteamController(intptr_t instancePtr, HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamController(hSteamUser, hSteamPipe, pchVersion);
}

S_API class ISteamUGC * SteamAPI_ISteamClient_GetISteamUGC(intptr_t instancePtr, HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamUGC(hSteamUser, hSteamPipe, pchVersion);
}

S_API class ISteamAppList * SteamAPI_ISteamClient_GetISteamAppList(intptr_t instancePtr, HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamAppList(hSteamUser, hSteamPipe, pchVersion);
}

S_API class ISteamMusic * SteamAPI_ISteamClient_GetISteamMusic(intptr_t instancePtr, HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamMusic(hSteamuser, hSteamPipe, pchVersion);
}

S_API class ISteamMusicRemote * SteamAPI_ISteamClient_GetISteamMusicRemote(intptr_t instancePtr, HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamMusicRemote(hSteamuser, hSteamPipe, pchVersion);
}

S_API class ISteamHTMLSurface * SteamAPI_ISteamClient_GetISteamHTMLSurface(intptr_t instancePtr, HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamHTMLSurface(hSteamuser, hSteamPipe, pchVersion);
}

S_API class ISteamInventory * SteamAPI_ISteamClient_GetISteamInventory(intptr_t instancePtr, HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamInventory(hSteamuser, hSteamPipe, pchVersion);
}

S_API class ISteamVideo * SteamAPI_ISteamClient_GetISteamVideo(intptr_t instancePtr, HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamVideo(hSteamuser, hSteamPipe, pchVersion);
}

S_API class ISteamParentalSettings * SteamAPI_ISteamClient_GetISteamParentalSettings(intptr_t instancePtr, HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamParentalSettings(hSteamuser, hSteamPipe, pchVersion);
}

S_API class ISteamInput * SteamAPI_ISteamClient_GetISteamInput(intptr_t instancePtr, HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamInput(hSteamUser, hSteamPipe, pchVersion);
}

S_API class ISteamParties * SteamAPI_ISteamClient_GetISteamParties(intptr_t instancePtr, HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char * pchVersion)
{
    return get_steam_client()->GetISteamParties(hSteamUser, hSteamPipe, pchVersion);
}

S_API HSteamUser SteamAPI_ISteamUser_GetHSteamUser(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user)->GetHSteamUser();
}

S_API bool SteamAPI_ISteamUser_BLoggedOn(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user)->BLoggedOn();
}

S_API uint64 SteamAPI_ISteamUser_GetSteamID(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user)->GetSteamID().ConvertToUint64();
}

S_API int SteamAPI_ISteamUser_InitiateGameConnection(intptr_t instancePtr, void * pAuthBlob, int cbMaxAuthBlob, class CSteamID steamIDGameServer, uint32 unIPServer, uint16 usPortServer, bool bSecure)
{
    return (get_steam_client()->steam_user)->InitiateGameConnection(pAuthBlob, cbMaxAuthBlob, steamIDGameServer, unIPServer, usPortServer, bSecure);
}

S_API void SteamAPI_ISteamUser_TerminateGameConnection(intptr_t instancePtr, uint32 unIPServer, uint16 usPortServer)
{
    return (get_steam_client()->steam_user)->TerminateGameConnection(unIPServer, usPortServer);
}

S_API void SteamAPI_ISteamUser_TrackAppUsageEvent(intptr_t instancePtr, class CGameID gameID, int eAppUsageEvent, const char * pchExtraInfo)
{
    return (get_steam_client()->steam_user)->TrackAppUsageEvent(gameID, eAppUsageEvent, pchExtraInfo);
}

S_API bool SteamAPI_ISteamUser_GetUserDataFolder(intptr_t instancePtr, char * pchBuffer, int cubBuffer)
{
    return (get_steam_client()->steam_user)->GetUserDataFolder(pchBuffer, cubBuffer);
}

S_API void SteamAPI_ISteamUser_StartVoiceRecording(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user)->StartVoiceRecording();
}

S_API void SteamAPI_ISteamUser_StopVoiceRecording(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user)->StopVoiceRecording();
}

S_API EVoiceResult SteamAPI_ISteamUser_GetAvailableVoice(intptr_t instancePtr, uint32 * pcbCompressed, uint32 * pcbUncompressed_Deprecated, uint32 nUncompressedVoiceDesiredSampleRate_Deprecated)
{
    return (get_steam_client()->steam_user)->GetAvailableVoice(pcbCompressed, pcbUncompressed_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
}

S_API EVoiceResult SteamAPI_ISteamUser_GetVoice(intptr_t instancePtr, bool bWantCompressed, void * pDestBuffer, uint32 cbDestBufferSize, uint32 * nBytesWritten, bool bWantUncompressed_Deprecated, void * pUncompressedDestBuffer_Deprecated, uint32 cbUncompressedDestBufferSize_Deprecated, uint32 * nUncompressBytesWritten_Deprecated, uint32 nUncompressedVoiceDesiredSampleRate_Deprecated)
{
    return (get_steam_client()->steam_user)->GetVoice(bWantCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, bWantUncompressed_Deprecated, pUncompressedDestBuffer_Deprecated, cbUncompressedDestBufferSize_Deprecated, nUncompressBytesWritten_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
}

S_API EVoiceResult SteamAPI_ISteamUser_DecompressVoice(intptr_t instancePtr, const void * pCompressed, uint32 cbCompressed, void * pDestBuffer, uint32 cbDestBufferSize, uint32 * nBytesWritten, uint32 nDesiredSampleRate)
{
    return (get_steam_client()->steam_user)->DecompressVoice(pCompressed, cbCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, nDesiredSampleRate);
}

S_API uint32 SteamAPI_ISteamUser_GetVoiceOptimalSampleRate(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user)->GetVoiceOptimalSampleRate();
}

S_API HAuthTicket SteamAPI_ISteamUser_GetAuthSessionTicket(intptr_t instancePtr, void * pTicket, int cbMaxTicket, uint32 * pcbTicket)
{
    return (get_steam_client()->steam_user)->GetAuthSessionTicket(pTicket, cbMaxTicket, pcbTicket);
}

S_API EBeginAuthSessionResult SteamAPI_ISteamUser_BeginAuthSession(intptr_t instancePtr, const void * pAuthTicket, int cbAuthTicket, class CSteamID steamID)
{
    return (get_steam_client()->steam_user)->BeginAuthSession(pAuthTicket, cbAuthTicket, steamID);
}

S_API void SteamAPI_ISteamUser_EndAuthSession(intptr_t instancePtr, class CSteamID steamID)
{
    return (get_steam_client()->steam_user)->EndAuthSession(steamID);
}

S_API void SteamAPI_ISteamUser_CancelAuthTicket(intptr_t instancePtr, HAuthTicket hAuthTicket)
{
    return (get_steam_client()->steam_user)->CancelAuthTicket(hAuthTicket);
}

S_API EUserHasLicenseForAppResult SteamAPI_ISteamUser_UserHasLicenseForApp(intptr_t instancePtr, class CSteamID steamID, AppId_t appID)
{
    return (get_steam_client()->steam_user)->UserHasLicenseForApp(steamID, appID);
}

S_API bool SteamAPI_ISteamUser_BIsBehindNAT(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user)->BIsBehindNAT();
}

S_API void SteamAPI_ISteamUser_AdvertiseGame(intptr_t instancePtr, class CSteamID steamIDGameServer, uint32 unIPServer, uint16 usPortServer)
{
    return (get_steam_client()->steam_user)->AdvertiseGame(steamIDGameServer, unIPServer, usPortServer);
}

S_API SteamAPICall_t SteamAPI_ISteamUser_RequestEncryptedAppTicket(intptr_t instancePtr, void * pDataToInclude, int cbDataToInclude)
{
    return (get_steam_client()->steam_user)->RequestEncryptedAppTicket(pDataToInclude, cbDataToInclude);
}

S_API bool SteamAPI_ISteamUser_GetEncryptedAppTicket(intptr_t instancePtr, void * pTicket, int cbMaxTicket, uint32 * pcbTicket)
{
    return (get_steam_client()->steam_user)->GetEncryptedAppTicket(pTicket, cbMaxTicket, pcbTicket);
}

S_API int SteamAPI_ISteamUser_GetGameBadgeLevel(intptr_t instancePtr, int nSeries, bool bFoil)
{
    return (get_steam_client()->steam_user)->GetGameBadgeLevel(nSeries, bFoil);
}

S_API int SteamAPI_ISteamUser_GetPlayerSteamLevel(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user)->GetPlayerSteamLevel();
}

S_API SteamAPICall_t SteamAPI_ISteamUser_RequestStoreAuthURL(intptr_t instancePtr, const char * pchRedirectURL)
{
    return (get_steam_client()->steam_user)->RequestStoreAuthURL(pchRedirectURL);
}

S_API bool SteamAPI_ISteamUser_BIsPhoneVerified(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user)->BIsPhoneVerified();
}

S_API bool SteamAPI_ISteamUser_BIsTwoFactorEnabled(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user)->BIsTwoFactorEnabled();
}

S_API bool SteamAPI_ISteamUser_BIsPhoneIdentifying(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user)->BIsPhoneIdentifying();
}

S_API bool SteamAPI_ISteamUser_BIsPhoneRequiringVerification(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user)->BIsPhoneRequiringVerification();
}

S_API SteamAPICall_t SteamAPI_ISteamUser_GetMarketEligibility(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user)->GetMarketEligibility();
}

S_API const char * SteamAPI_ISteamFriends_GetPersonaName(intptr_t instancePtr)
{
    return (get_steam_client()->steam_friends)->GetPersonaName();
}

S_API SteamAPICall_t SteamAPI_ISteamFriends_SetPersonaName(intptr_t instancePtr, const char * pchPersonaName)
{
    return (get_steam_client()->steam_friends)->SetPersonaName(pchPersonaName);
}

S_API EPersonaState SteamAPI_ISteamFriends_GetPersonaState(intptr_t instancePtr)
{
    return (get_steam_client()->steam_friends)->GetPersonaState();
}

S_API int SteamAPI_ISteamFriends_GetFriendCount(intptr_t instancePtr, int iFriendFlags)
{
    return (get_steam_client()->steam_friends)->GetFriendCount(iFriendFlags);
}

S_API uint64 SteamAPI_ISteamFriends_GetFriendByIndex(intptr_t instancePtr, int iFriend, int iFriendFlags)
{
    return (get_steam_client()->steam_friends)->GetFriendByIndex(iFriend, iFriendFlags).ConvertToUint64();
}

S_API EFriendRelationship SteamAPI_ISteamFriends_GetFriendRelationship(intptr_t instancePtr, class CSteamID steamIDFriend)
{
    return (get_steam_client()->steam_friends)->GetFriendRelationship(steamIDFriend);
}

S_API EPersonaState SteamAPI_ISteamFriends_GetFriendPersonaState(intptr_t instancePtr, class CSteamID steamIDFriend)
{
    return (get_steam_client()->steam_friends)->GetFriendPersonaState(steamIDFriend);
}

S_API const char * SteamAPI_ISteamFriends_GetFriendPersonaName(intptr_t instancePtr, class CSteamID steamIDFriend)
{
    return (get_steam_client()->steam_friends)->GetFriendPersonaName(steamIDFriend);
}

S_API bool SteamAPI_ISteamFriends_GetFriendGamePlayed(intptr_t instancePtr, class CSteamID steamIDFriend, struct FriendGameInfo_t * pFriendGameInfo)
{
    return (get_steam_client()->steam_friends)->GetFriendGamePlayed(steamIDFriend, pFriendGameInfo);
}

S_API const char * SteamAPI_ISteamFriends_GetFriendPersonaNameHistory(intptr_t instancePtr, class CSteamID steamIDFriend, int iPersonaName)
{
    return (get_steam_client()->steam_friends)->GetFriendPersonaNameHistory(steamIDFriend, iPersonaName);
}

S_API int SteamAPI_ISteamFriends_GetFriendSteamLevel(intptr_t instancePtr, class CSteamID steamIDFriend)
{
    return (get_steam_client()->steam_friends)->GetFriendSteamLevel(steamIDFriend);
}

S_API const char * SteamAPI_ISteamFriends_GetPlayerNickname(intptr_t instancePtr, class CSteamID steamIDPlayer)
{
    return (get_steam_client()->steam_friends)->GetPlayerNickname(steamIDPlayer);
}

S_API int SteamAPI_ISteamFriends_GetFriendsGroupCount(intptr_t instancePtr)
{
    return (get_steam_client()->steam_friends)->GetFriendsGroupCount();
}

S_API FriendsGroupID_t SteamAPI_ISteamFriends_GetFriendsGroupIDByIndex(intptr_t instancePtr, int iFG)
{
    return (get_steam_client()->steam_friends)->GetFriendsGroupIDByIndex(iFG);
}

S_API const char * SteamAPI_ISteamFriends_GetFriendsGroupName(intptr_t instancePtr, FriendsGroupID_t friendsGroupID)
{
    return (get_steam_client()->steam_friends)->GetFriendsGroupName(friendsGroupID);
}

S_API int SteamAPI_ISteamFriends_GetFriendsGroupMembersCount(intptr_t instancePtr, FriendsGroupID_t friendsGroupID)
{
    return (get_steam_client()->steam_friends)->GetFriendsGroupMembersCount(friendsGroupID);
}

S_API void SteamAPI_ISteamFriends_GetFriendsGroupMembersList(intptr_t instancePtr, FriendsGroupID_t friendsGroupID, class CSteamID * pOutSteamIDMembers, int nMembersCount)
{
    return (get_steam_client()->steam_friends)->GetFriendsGroupMembersList(friendsGroupID, pOutSteamIDMembers, nMembersCount);
}

S_API bool SteamAPI_ISteamFriends_HasFriend(intptr_t instancePtr, class CSteamID steamIDFriend, int iFriendFlags)
{
    return (get_steam_client()->steam_friends)->HasFriend(steamIDFriend, iFriendFlags);
}

S_API int SteamAPI_ISteamFriends_GetClanCount(intptr_t instancePtr)
{
    return (get_steam_client()->steam_friends)->GetClanCount();
}

S_API uint64 SteamAPI_ISteamFriends_GetClanByIndex(intptr_t instancePtr, int iClan)
{
    return (get_steam_client()->steam_friends)->GetClanByIndex(iClan).ConvertToUint64();
}

S_API const char * SteamAPI_ISteamFriends_GetClanName(intptr_t instancePtr, class CSteamID steamIDClan)
{
    return (get_steam_client()->steam_friends)->GetClanName(steamIDClan);
}

S_API const char * SteamAPI_ISteamFriends_GetClanTag(intptr_t instancePtr, class CSteamID steamIDClan)
{
    return (get_steam_client()->steam_friends)->GetClanTag(steamIDClan);
}

S_API bool SteamAPI_ISteamFriends_GetClanActivityCounts(intptr_t instancePtr, class CSteamID steamIDClan, int * pnOnline, int * pnInGame, int * pnChatting)
{
    return (get_steam_client()->steam_friends)->GetClanActivityCounts(steamIDClan, pnOnline, pnInGame, pnChatting);
}

S_API SteamAPICall_t SteamAPI_ISteamFriends_DownloadClanActivityCounts(intptr_t instancePtr, class CSteamID * psteamIDClans, int cClansToRequest)
{
    return (get_steam_client()->steam_friends)->DownloadClanActivityCounts(psteamIDClans, cClansToRequest);
}

S_API int SteamAPI_ISteamFriends_GetFriendCountFromSource(intptr_t instancePtr, class CSteamID steamIDSource)
{
    return (get_steam_client()->steam_friends)->GetFriendCountFromSource(steamIDSource);
}

S_API uint64 SteamAPI_ISteamFriends_GetFriendFromSourceByIndex(intptr_t instancePtr, class CSteamID steamIDSource, int iFriend)
{
    return (get_steam_client()->steam_friends)->GetFriendFromSourceByIndex(steamIDSource, iFriend).ConvertToUint64();
}

S_API bool SteamAPI_ISteamFriends_IsUserInSource(intptr_t instancePtr, class CSteamID steamIDUser, class CSteamID steamIDSource)
{
    return (get_steam_client()->steam_friends)->IsUserInSource(steamIDUser, steamIDSource);
}

S_API void SteamAPI_ISteamFriends_SetInGameVoiceSpeaking(intptr_t instancePtr, class CSteamID steamIDUser, bool bSpeaking)
{
    return (get_steam_client()->steam_friends)->SetInGameVoiceSpeaking(steamIDUser, bSpeaking);
}

S_API void SteamAPI_ISteamFriends_ActivateGameOverlay(intptr_t instancePtr, const char * pchDialog)
{
    return (get_steam_client()->steam_friends)->ActivateGameOverlay(pchDialog);
}

S_API void SteamAPI_ISteamFriends_ActivateGameOverlayToUser(intptr_t instancePtr, const char * pchDialog, class CSteamID steamID)
{
    return (get_steam_client()->steam_friends)->ActivateGameOverlayToUser(pchDialog, steamID);
}

S_API void SteamAPI_ISteamFriends_ActivateGameOverlayToWebPage(intptr_t instancePtr, const char * pchURL, EActivateGameOverlayToWebPageMode eMode)
{
    return (get_steam_client()->steam_friends)->ActivateGameOverlayToWebPage(pchURL, eMode);
}

S_API void SteamAPI_ISteamFriends_ActivateGameOverlayToStore(intptr_t instancePtr, AppId_t nAppID, EOverlayToStoreFlag eFlag)
{
    return (get_steam_client()->steam_friends)->ActivateGameOverlayToStore(nAppID, eFlag);
}

S_API void SteamAPI_ISteamFriends_SetPlayedWith(intptr_t instancePtr, class CSteamID steamIDUserPlayedWith)
{
    return (get_steam_client()->steam_friends)->SetPlayedWith(steamIDUserPlayedWith);
}

S_API void SteamAPI_ISteamFriends_ActivateGameOverlayInviteDialog(intptr_t instancePtr, class CSteamID steamIDLobby)
{
    return (get_steam_client()->steam_friends)->ActivateGameOverlayInviteDialog(steamIDLobby);
}

S_API int SteamAPI_ISteamFriends_GetSmallFriendAvatar(intptr_t instancePtr, class CSteamID steamIDFriend)
{
    return (get_steam_client()->steam_friends)->GetSmallFriendAvatar(steamIDFriend);
}

S_API int SteamAPI_ISteamFriends_GetMediumFriendAvatar(intptr_t instancePtr, class CSteamID steamIDFriend)
{
    return (get_steam_client()->steam_friends)->GetMediumFriendAvatar(steamIDFriend);
}

S_API int SteamAPI_ISteamFriends_GetLargeFriendAvatar(intptr_t instancePtr, class CSteamID steamIDFriend)
{
    return (get_steam_client()->steam_friends)->GetLargeFriendAvatar(steamIDFriend);
}

S_API bool SteamAPI_ISteamFriends_RequestUserInformation(intptr_t instancePtr, class CSteamID steamIDUser, bool bRequireNameOnly)
{
    return (get_steam_client()->steam_friends)->RequestUserInformation(steamIDUser, bRequireNameOnly);
}

S_API SteamAPICall_t SteamAPI_ISteamFriends_RequestClanOfficerList(intptr_t instancePtr, class CSteamID steamIDClan)
{
    return (get_steam_client()->steam_friends)->RequestClanOfficerList(steamIDClan);
}

S_API uint64 SteamAPI_ISteamFriends_GetClanOwner(intptr_t instancePtr, class CSteamID steamIDClan)
{
    return (get_steam_client()->steam_friends)->GetClanOwner(steamIDClan).ConvertToUint64();
}

S_API int SteamAPI_ISteamFriends_GetClanOfficerCount(intptr_t instancePtr, class CSteamID steamIDClan)
{
    return (get_steam_client()->steam_friends)->GetClanOfficerCount(steamIDClan);
}

S_API uint64 SteamAPI_ISteamFriends_GetClanOfficerByIndex(intptr_t instancePtr, class CSteamID steamIDClan, int iOfficer)
{
    return (get_steam_client()->steam_friends)->GetClanOfficerByIndex(steamIDClan, iOfficer).ConvertToUint64();
}

S_API uint32 SteamAPI_ISteamFriends_GetUserRestrictions(intptr_t instancePtr)
{
    return (get_steam_client()->steam_friends)->GetUserRestrictions();
}

S_API bool SteamAPI_ISteamFriends_SetRichPresence(intptr_t instancePtr, const char * pchKey, const char * pchValue)
{
    return (get_steam_client()->steam_friends)->SetRichPresence(pchKey, pchValue);
}

S_API void SteamAPI_ISteamFriends_ClearRichPresence(intptr_t instancePtr)
{
    return (get_steam_client()->steam_friends)->ClearRichPresence();
}

S_API const char * SteamAPI_ISteamFriends_GetFriendRichPresence(intptr_t instancePtr, class CSteamID steamIDFriend, const char * pchKey)
{
    return (get_steam_client()->steam_friends)->GetFriendRichPresence(steamIDFriend, pchKey);
}

S_API int SteamAPI_ISteamFriends_GetFriendRichPresenceKeyCount(intptr_t instancePtr, class CSteamID steamIDFriend)
{
    return (get_steam_client()->steam_friends)->GetFriendRichPresenceKeyCount(steamIDFriend);
}

S_API const char * SteamAPI_ISteamFriends_GetFriendRichPresenceKeyByIndex(intptr_t instancePtr, class CSteamID steamIDFriend, int iKey)
{
    return (get_steam_client()->steam_friends)->GetFriendRichPresenceKeyByIndex(steamIDFriend, iKey);
}

S_API void SteamAPI_ISteamFriends_RequestFriendRichPresence(intptr_t instancePtr, class CSteamID steamIDFriend)
{
    return (get_steam_client()->steam_friends)->RequestFriendRichPresence(steamIDFriend);
}

S_API bool SteamAPI_ISteamFriends_InviteUserToGame(intptr_t instancePtr, class CSteamID steamIDFriend, const char * pchConnectString)
{
    return (get_steam_client()->steam_friends)->InviteUserToGame(steamIDFriend, pchConnectString);
}

S_API int SteamAPI_ISteamFriends_GetCoplayFriendCount(intptr_t instancePtr)
{
    return (get_steam_client()->steam_friends)->GetCoplayFriendCount();
}

S_API uint64 SteamAPI_ISteamFriends_GetCoplayFriend(intptr_t instancePtr, int iCoplayFriend)
{
    return (get_steam_client()->steam_friends)->GetCoplayFriend(iCoplayFriend).ConvertToUint64();
}

S_API int SteamAPI_ISteamFriends_GetFriendCoplayTime(intptr_t instancePtr, class CSteamID steamIDFriend)
{
    return (get_steam_client()->steam_friends)->GetFriendCoplayTime(steamIDFriend);
}

S_API AppId_t SteamAPI_ISteamFriends_GetFriendCoplayGame(intptr_t instancePtr, class CSteamID steamIDFriend)
{
    return (get_steam_client()->steam_friends)->GetFriendCoplayGame(steamIDFriend);
}

S_API SteamAPICall_t SteamAPI_ISteamFriends_JoinClanChatRoom(intptr_t instancePtr, class CSteamID steamIDClan)
{
    return (get_steam_client()->steam_friends)->JoinClanChatRoom(steamIDClan);
}

S_API bool SteamAPI_ISteamFriends_LeaveClanChatRoom(intptr_t instancePtr, class CSteamID steamIDClan)
{
    return (get_steam_client()->steam_friends)->LeaveClanChatRoom(steamIDClan);
}

S_API int SteamAPI_ISteamFriends_GetClanChatMemberCount(intptr_t instancePtr, class CSteamID steamIDClan)
{
    return (get_steam_client()->steam_friends)->GetClanChatMemberCount(steamIDClan);
}

S_API uint64 SteamAPI_ISteamFriends_GetChatMemberByIndex(intptr_t instancePtr, class CSteamID steamIDClan, int iUser)
{
    return (get_steam_client()->steam_friends)->GetChatMemberByIndex(steamIDClan, iUser).ConvertToUint64();
}

S_API bool SteamAPI_ISteamFriends_SendClanChatMessage(intptr_t instancePtr, class CSteamID steamIDClanChat, const char * pchText)
{
    return (get_steam_client()->steam_friends)->SendClanChatMessage(steamIDClanChat, pchText);
}

S_API int SteamAPI_ISteamFriends_GetClanChatMessage(intptr_t instancePtr, class CSteamID steamIDClanChat, int iMessage, void * prgchText, int cchTextMax, EChatEntryType * peChatEntryType, class CSteamID * psteamidChatter)
{
    return (get_steam_client()->steam_friends)->GetClanChatMessage(steamIDClanChat, iMessage, prgchText, cchTextMax, peChatEntryType, psteamidChatter);
}

S_API bool SteamAPI_ISteamFriends_IsClanChatAdmin(intptr_t instancePtr, class CSteamID steamIDClanChat, class CSteamID steamIDUser)
{
    return (get_steam_client()->steam_friends)->IsClanChatAdmin(steamIDClanChat, steamIDUser);
}

S_API bool SteamAPI_ISteamFriends_IsClanChatWindowOpenInSteam(intptr_t instancePtr, class CSteamID steamIDClanChat)
{
    return (get_steam_client()->steam_friends)->IsClanChatWindowOpenInSteam(steamIDClanChat);
}

S_API bool SteamAPI_ISteamFriends_OpenClanChatWindowInSteam(intptr_t instancePtr, class CSteamID steamIDClanChat)
{
    return (get_steam_client()->steam_friends)->OpenClanChatWindowInSteam(steamIDClanChat);
}

S_API bool SteamAPI_ISteamFriends_CloseClanChatWindowInSteam(intptr_t instancePtr, class CSteamID steamIDClanChat)
{
    return (get_steam_client()->steam_friends)->CloseClanChatWindowInSteam(steamIDClanChat);
}

S_API bool SteamAPI_ISteamFriends_SetListenForFriendsMessages(intptr_t instancePtr, bool bInterceptEnabled)
{
    return (get_steam_client()->steam_friends)->SetListenForFriendsMessages(bInterceptEnabled);
}

S_API bool SteamAPI_ISteamFriends_ReplyToFriendMessage(intptr_t instancePtr, class CSteamID steamIDFriend, const char * pchMsgToSend)
{
    return (get_steam_client()->steam_friends)->ReplyToFriendMessage(steamIDFriend, pchMsgToSend);
}

S_API int SteamAPI_ISteamFriends_GetFriendMessage(intptr_t instancePtr, class CSteamID steamIDFriend, int iMessageID, void * pvData, int cubData, EChatEntryType * peChatEntryType)
{
    return (get_steam_client()->steam_friends)->GetFriendMessage(steamIDFriend, iMessageID, pvData, cubData, peChatEntryType);
}

S_API SteamAPICall_t SteamAPI_ISteamFriends_GetFollowerCount(intptr_t instancePtr, class CSteamID steamID)
{
    return (get_steam_client()->steam_friends)->GetFollowerCount(steamID);
}

S_API SteamAPICall_t SteamAPI_ISteamFriends_IsFollowing(intptr_t instancePtr, class CSteamID steamID)
{
    return (get_steam_client()->steam_friends)->IsFollowing(steamID);
}

S_API SteamAPICall_t SteamAPI_ISteamFriends_EnumerateFollowingList(intptr_t instancePtr, uint32 unStartIndex)
{
    return (get_steam_client()->steam_friends)->EnumerateFollowingList(unStartIndex);
}

S_API bool SteamAPI_ISteamFriends_IsClanPublic(intptr_t instancePtr, class CSteamID steamIDClan)
{
    return (get_steam_client()->steam_friends)->IsClanPublic(steamIDClan);
}

S_API bool SteamAPI_ISteamFriends_IsClanOfficialGameGroup(intptr_t instancePtr, class CSteamID steamIDClan)
{
    return (get_steam_client()->steam_friends)->IsClanOfficialGameGroup(steamIDClan);
}

S_API int SteamAPI_ISteamFriends_GetNumChatsWithUnreadPriorityMessages(intptr_t instancePtr)
{
    return (get_steam_client()->steam_friends)->GetNumChatsWithUnreadPriorityMessages();
}

S_API uint32 SteamAPI_ISteamUtils_GetSecondsSinceAppActive(intptr_t instancePtr)
{
    return ((ISteamUtils *)instancePtr)->GetSecondsSinceAppActive();
}

S_API uint32 SteamAPI_ISteamUtils_GetSecondsSinceComputerActive(intptr_t instancePtr)
{
    return ((ISteamUtils *)instancePtr)->GetSecondsSinceComputerActive();
}

S_API EUniverse SteamAPI_ISteamUtils_GetConnectedUniverse(intptr_t instancePtr)
{
    return ((ISteamUtils *)instancePtr)->GetConnectedUniverse();
}

S_API uint32 SteamAPI_ISteamUtils_GetServerRealTime(intptr_t instancePtr)
{
    return ((ISteamUtils *)instancePtr)->GetServerRealTime();
}

S_API const char * SteamAPI_ISteamUtils_GetIPCountry(intptr_t instancePtr)
{
    return ((ISteamUtils *)instancePtr)->GetIPCountry();
}

S_API bool SteamAPI_ISteamUtils_GetImageSize(intptr_t instancePtr, int iImage, uint32 * pnWidth, uint32 * pnHeight)
{
    return ((ISteamUtils *)instancePtr)->GetImageSize(iImage, pnWidth, pnHeight);
}

S_API bool SteamAPI_ISteamUtils_GetImageRGBA(intptr_t instancePtr, int iImage, uint8 * pubDest, int nDestBufferSize)
{
    return ((ISteamUtils *)instancePtr)->GetImageRGBA(iImage, pubDest, nDestBufferSize);
}

S_API bool SteamAPI_ISteamUtils_GetCSERIPPort(intptr_t instancePtr, uint32 * unIP, uint16 * usPort)
{
    return ((ISteamUtils *)instancePtr)->GetCSERIPPort(unIP, usPort);
}

S_API uint8 SteamAPI_ISteamUtils_GetCurrentBatteryPower(intptr_t instancePtr)
{
    return ((ISteamUtils *)instancePtr)->GetCurrentBatteryPower();
}

S_API uint32 SteamAPI_ISteamUtils_GetAppID(intptr_t instancePtr)
{
    return ((ISteamUtils *)instancePtr)->GetAppID();
}

S_API void SteamAPI_ISteamUtils_SetOverlayNotificationPosition(intptr_t instancePtr, ENotificationPosition eNotificationPosition)
{
    return ((ISteamUtils *)instancePtr)->SetOverlayNotificationPosition(eNotificationPosition);
}

S_API bool SteamAPI_ISteamUtils_IsAPICallCompleted(intptr_t instancePtr, SteamAPICall_t hSteamAPICall, bool * pbFailed)
{
    return ((ISteamUtils *)instancePtr)->IsAPICallCompleted(hSteamAPICall, pbFailed);
}

S_API ESteamAPICallFailure SteamAPI_ISteamUtils_GetAPICallFailureReason(intptr_t instancePtr, SteamAPICall_t hSteamAPICall)
{
    return ((ISteamUtils *)instancePtr)->GetAPICallFailureReason(hSteamAPICall);
}

S_API bool SteamAPI_ISteamUtils_GetAPICallResult(intptr_t instancePtr, SteamAPICall_t hSteamAPICall, void * pCallback, int cubCallback, int iCallbackExpected, bool * pbFailed)
{
    return ((ISteamUtils *)instancePtr)->GetAPICallResult(hSteamAPICall, pCallback, cubCallback, iCallbackExpected, pbFailed);
}

S_API uint32 SteamAPI_ISteamUtils_GetIPCCallCount(intptr_t instancePtr)
{
    return ((ISteamUtils *)instancePtr)->GetIPCCallCount();
}

S_API void SteamAPI_ISteamUtils_SetWarningMessageHook(intptr_t instancePtr, SteamAPIWarningMessageHook_t pFunction)
{
    return ((ISteamUtils *)instancePtr)->SetWarningMessageHook(pFunction);
}

S_API bool SteamAPI_ISteamUtils_IsOverlayEnabled(intptr_t instancePtr)
{
    return ((ISteamUtils *)instancePtr)->IsOverlayEnabled();
}

S_API bool SteamAPI_ISteamUtils_BOverlayNeedsPresent(intptr_t instancePtr)
{
    return ((ISteamUtils *)instancePtr)->BOverlayNeedsPresent();
}

S_API SteamAPICall_t SteamAPI_ISteamUtils_CheckFileSignature(intptr_t instancePtr, const char * szFileName)
{
    return ((ISteamUtils *)instancePtr)->CheckFileSignature(szFileName);
}

S_API bool SteamAPI_ISteamUtils_ShowGamepadTextInput(intptr_t instancePtr, EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, const char * pchDescription, uint32 unCharMax, const char * pchExistingText)
{
    return ((ISteamUtils *)instancePtr)->ShowGamepadTextInput(eInputMode, eLineInputMode, pchDescription, unCharMax, pchExistingText);
}

S_API uint32 SteamAPI_ISteamUtils_GetEnteredGamepadTextLength(intptr_t instancePtr)
{
    return ((ISteamUtils *)instancePtr)->GetEnteredGamepadTextLength();
}

S_API bool SteamAPI_ISteamUtils_GetEnteredGamepadTextInput(intptr_t instancePtr, char * pchText, uint32 cchText)
{
    return ((ISteamUtils *)instancePtr)->GetEnteredGamepadTextInput(pchText, cchText);
}

S_API const char * SteamAPI_ISteamUtils_GetSteamUILanguage(intptr_t instancePtr)
{
    return ((ISteamUtils *)instancePtr)->GetSteamUILanguage();
}

S_API bool SteamAPI_ISteamUtils_IsSteamRunningInVR(intptr_t instancePtr)
{
    return ((ISteamUtils *)instancePtr)->IsSteamRunningInVR();
}

S_API void SteamAPI_ISteamUtils_SetOverlayNotificationInset(intptr_t instancePtr, int nHorizontalInset, int nVerticalInset)
{
    return ((ISteamUtils *)instancePtr)->SetOverlayNotificationInset(nHorizontalInset, nVerticalInset);
}

S_API bool SteamAPI_ISteamUtils_IsSteamInBigPictureMode(intptr_t instancePtr)
{
    return ((ISteamUtils *)instancePtr)->IsSteamInBigPictureMode();
}

S_API void SteamAPI_ISteamUtils_StartVRDashboard(intptr_t instancePtr)
{
    return ((ISteamUtils *)instancePtr)->StartVRDashboard();
}

S_API bool SteamAPI_ISteamUtils_IsVRHeadsetStreamingEnabled(intptr_t instancePtr)
{
    return ((ISteamUtils *)instancePtr)->IsVRHeadsetStreamingEnabled();
}

S_API void SteamAPI_ISteamUtils_SetVRHeadsetStreamingEnabled(intptr_t instancePtr, bool bEnabled)
{
    return ((ISteamUtils *)instancePtr)->SetVRHeadsetStreamingEnabled(bEnabled);
}

S_API int SteamAPI_ISteamMatchmaking_GetFavoriteGameCount(intptr_t instancePtr)
{
    return (get_steam_client()->steam_matchmaking)->GetFavoriteGameCount();
}

S_API bool SteamAPI_ISteamMatchmaking_GetFavoriteGame(intptr_t instancePtr, int iGame, AppId_t * pnAppID, uint32 * pnIP, uint16 * pnConnPort, uint16 * pnQueryPort, uint32 * punFlags, uint32 * pRTime32LastPlayedOnServer)
{
    return (get_steam_client()->steam_matchmaking)->GetFavoriteGame(iGame, pnAppID, pnIP, pnConnPort, pnQueryPort, punFlags, pRTime32LastPlayedOnServer);
}

S_API int SteamAPI_ISteamMatchmaking_AddFavoriteGame(intptr_t instancePtr, AppId_t nAppID, uint32 nIP, uint16 nConnPort, uint16 nQueryPort, uint32 unFlags, uint32 rTime32LastPlayedOnServer)
{
    return (get_steam_client()->steam_matchmaking)->AddFavoriteGame(nAppID, nIP, nConnPort, nQueryPort, unFlags, rTime32LastPlayedOnServer);
}

S_API bool SteamAPI_ISteamMatchmaking_RemoveFavoriteGame(intptr_t instancePtr, AppId_t nAppID, uint32 nIP, uint16 nConnPort, uint16 nQueryPort, uint32 unFlags)
{
    return (get_steam_client()->steam_matchmaking)->RemoveFavoriteGame(nAppID, nIP, nConnPort, nQueryPort, unFlags);
}

S_API SteamAPICall_t SteamAPI_ISteamMatchmaking_RequestLobbyList(intptr_t instancePtr)
{
    return (get_steam_client()->steam_matchmaking)->RequestLobbyList();
}

S_API void SteamAPI_ISteamMatchmaking_AddRequestLobbyListStringFilter(intptr_t instancePtr, const char * pchKeyToMatch, const char * pchValueToMatch, ELobbyComparison eComparisonType)
{
    return (get_steam_client()->steam_matchmaking)->AddRequestLobbyListStringFilter(pchKeyToMatch, pchValueToMatch, eComparisonType);
}

S_API void SteamAPI_ISteamMatchmaking_AddRequestLobbyListNumericalFilter(intptr_t instancePtr, const char * pchKeyToMatch, int nValueToMatch, ELobbyComparison eComparisonType)
{
    return (get_steam_client()->steam_matchmaking)->AddRequestLobbyListNumericalFilter(pchKeyToMatch, nValueToMatch, eComparisonType);
}

S_API void SteamAPI_ISteamMatchmaking_AddRequestLobbyListNearValueFilter(intptr_t instancePtr, const char * pchKeyToMatch, int nValueToBeCloseTo)
{
    return (get_steam_client()->steam_matchmaking)->AddRequestLobbyListNearValueFilter(pchKeyToMatch, nValueToBeCloseTo);
}

S_API void SteamAPI_ISteamMatchmaking_AddRequestLobbyListFilterSlotsAvailable(intptr_t instancePtr, int nSlotsAvailable)
{
    return (get_steam_client()->steam_matchmaking)->AddRequestLobbyListFilterSlotsAvailable(nSlotsAvailable);
}

S_API void SteamAPI_ISteamMatchmaking_AddRequestLobbyListDistanceFilter(intptr_t instancePtr, ELobbyDistanceFilter eLobbyDistanceFilter)
{
    return (get_steam_client()->steam_matchmaking)->AddRequestLobbyListDistanceFilter(eLobbyDistanceFilter);
}

S_API void SteamAPI_ISteamMatchmaking_AddRequestLobbyListResultCountFilter(intptr_t instancePtr, int cMaxResults)
{
    return (get_steam_client()->steam_matchmaking)->AddRequestLobbyListResultCountFilter(cMaxResults);
}

S_API void SteamAPI_ISteamMatchmaking_AddRequestLobbyListCompatibleMembersFilter(intptr_t instancePtr, class CSteamID steamIDLobby)
{
    return (get_steam_client()->steam_matchmaking)->AddRequestLobbyListCompatibleMembersFilter(steamIDLobby);
}

S_API uint64 SteamAPI_ISteamMatchmaking_GetLobbyByIndex(intptr_t instancePtr, int iLobby)
{
    return (get_steam_client()->steam_matchmaking)->GetLobbyByIndex(iLobby).ConvertToUint64();
}

S_API SteamAPICall_t SteamAPI_ISteamMatchmaking_CreateLobby(intptr_t instancePtr, ELobbyType eLobbyType, int cMaxMembers)
{
    return (get_steam_client()->steam_matchmaking)->CreateLobby(eLobbyType, cMaxMembers);
}

S_API SteamAPICall_t SteamAPI_ISteamMatchmaking_JoinLobby(intptr_t instancePtr, class CSteamID steamIDLobby)
{
    return (get_steam_client()->steam_matchmaking)->JoinLobby(steamIDLobby);
}

S_API void SteamAPI_ISteamMatchmaking_LeaveLobby(intptr_t instancePtr, class CSteamID steamIDLobby)
{
    return (get_steam_client()->steam_matchmaking)->LeaveLobby(steamIDLobby);
}

S_API bool SteamAPI_ISteamMatchmaking_InviteUserToLobby(intptr_t instancePtr, class CSteamID steamIDLobby, class CSteamID steamIDInvitee)
{
    return (get_steam_client()->steam_matchmaking)->InviteUserToLobby(steamIDLobby, steamIDInvitee);
}

S_API int SteamAPI_ISteamMatchmaking_GetNumLobbyMembers(intptr_t instancePtr, class CSteamID steamIDLobby)
{
    return (get_steam_client()->steam_matchmaking)->GetNumLobbyMembers(steamIDLobby);
}

S_API uint64 SteamAPI_ISteamMatchmaking_GetLobbyMemberByIndex(intptr_t instancePtr, class CSteamID steamIDLobby, int iMember)
{
    return (get_steam_client()->steam_matchmaking)->GetLobbyMemberByIndex(steamIDLobby, iMember).ConvertToUint64();
}

S_API const char * SteamAPI_ISteamMatchmaking_GetLobbyData(intptr_t instancePtr, class CSteamID steamIDLobby, const char * pchKey)
{
    return (get_steam_client()->steam_matchmaking)->GetLobbyData(steamIDLobby, pchKey);
}

S_API bool SteamAPI_ISteamMatchmaking_SetLobbyData(intptr_t instancePtr, class CSteamID steamIDLobby, const char * pchKey, const char * pchValue)
{
    return (get_steam_client()->steam_matchmaking)->SetLobbyData(steamIDLobby, pchKey, pchValue);
}

S_API int SteamAPI_ISteamMatchmaking_GetLobbyDataCount(intptr_t instancePtr, class CSteamID steamIDLobby)
{
    return (get_steam_client()->steam_matchmaking)->GetLobbyDataCount(steamIDLobby);
}

S_API bool SteamAPI_ISteamMatchmaking_GetLobbyDataByIndex(intptr_t instancePtr, class CSteamID steamIDLobby, int iLobbyData, char * pchKey, int cchKeyBufferSize, char * pchValue, int cchValueBufferSize)
{
    return (get_steam_client()->steam_matchmaking)->GetLobbyDataByIndex(steamIDLobby, iLobbyData, pchKey, cchKeyBufferSize, pchValue, cchValueBufferSize);
}

S_API bool SteamAPI_ISteamMatchmaking_DeleteLobbyData(intptr_t instancePtr, class CSteamID steamIDLobby, const char * pchKey)
{
    return (get_steam_client()->steam_matchmaking)->DeleteLobbyData(steamIDLobby, pchKey);
}

S_API const char * SteamAPI_ISteamMatchmaking_GetLobbyMemberData(intptr_t instancePtr, class CSteamID steamIDLobby, class CSteamID steamIDUser, const char * pchKey)
{
    return (get_steam_client()->steam_matchmaking)->GetLobbyMemberData(steamIDLobby, steamIDUser, pchKey);
}

S_API void SteamAPI_ISteamMatchmaking_SetLobbyMemberData(intptr_t instancePtr, class CSteamID steamIDLobby, const char * pchKey, const char * pchValue)
{
    return (get_steam_client()->steam_matchmaking)->SetLobbyMemberData(steamIDLobby, pchKey, pchValue);
}

S_API bool SteamAPI_ISteamMatchmaking_SendLobbyChatMsg(intptr_t instancePtr, class CSteamID steamIDLobby, const void * pvMsgBody, int cubMsgBody)
{
    return (get_steam_client()->steam_matchmaking)->SendLobbyChatMsg(steamIDLobby, pvMsgBody, cubMsgBody);
}

S_API int SteamAPI_ISteamMatchmaking_GetLobbyChatEntry(intptr_t instancePtr, class CSteamID steamIDLobby, int iChatID, class CSteamID * pSteamIDUser, void * pvData, int cubData, EChatEntryType * peChatEntryType)
{
    return (get_steam_client()->steam_matchmaking)->GetLobbyChatEntry(steamIDLobby, iChatID, pSteamIDUser, pvData, cubData, peChatEntryType);
}

S_API bool SteamAPI_ISteamMatchmaking_RequestLobbyData(intptr_t instancePtr, class CSteamID steamIDLobby)
{
    return (get_steam_client()->steam_matchmaking)->RequestLobbyData(steamIDLobby);
}

S_API void SteamAPI_ISteamMatchmaking_SetLobbyGameServer(intptr_t instancePtr, class CSteamID steamIDLobby, uint32 unGameServerIP, uint16 unGameServerPort, class CSteamID steamIDGameServer)
{
    return (get_steam_client()->steam_matchmaking)->SetLobbyGameServer(steamIDLobby, unGameServerIP, unGameServerPort, steamIDGameServer);
}

S_API bool SteamAPI_ISteamMatchmaking_GetLobbyGameServer(intptr_t instancePtr, class CSteamID steamIDLobby, uint32 * punGameServerIP, uint16 * punGameServerPort, class CSteamID * psteamIDGameServer)
{
    return (get_steam_client()->steam_matchmaking)->GetLobbyGameServer(steamIDLobby, punGameServerIP, punGameServerPort, psteamIDGameServer);
}

S_API bool SteamAPI_ISteamMatchmaking_SetLobbyMemberLimit(intptr_t instancePtr, class CSteamID steamIDLobby, int cMaxMembers)
{
    return (get_steam_client()->steam_matchmaking)->SetLobbyMemberLimit(steamIDLobby, cMaxMembers);
}

S_API int SteamAPI_ISteamMatchmaking_GetLobbyMemberLimit(intptr_t instancePtr, class CSteamID steamIDLobby)
{
    return (get_steam_client()->steam_matchmaking)->GetLobbyMemberLimit(steamIDLobby);
}

S_API bool SteamAPI_ISteamMatchmaking_SetLobbyType(intptr_t instancePtr, class CSteamID steamIDLobby, ELobbyType eLobbyType)
{
    return (get_steam_client()->steam_matchmaking)->SetLobbyType(steamIDLobby, eLobbyType);
}

S_API bool SteamAPI_ISteamMatchmaking_SetLobbyJoinable(intptr_t instancePtr, class CSteamID steamIDLobby, bool bLobbyJoinable)
{
    return (get_steam_client()->steam_matchmaking)->SetLobbyJoinable(steamIDLobby, bLobbyJoinable);
}

S_API uint64 SteamAPI_ISteamMatchmaking_GetLobbyOwner(intptr_t instancePtr, class CSteamID steamIDLobby)
{
    return (get_steam_client()->steam_matchmaking)->GetLobbyOwner(steamIDLobby).ConvertToUint64();
}

S_API bool SteamAPI_ISteamMatchmaking_SetLobbyOwner(intptr_t instancePtr, class CSteamID steamIDLobby, class CSteamID steamIDNewOwner)
{
    return (get_steam_client()->steam_matchmaking)->SetLobbyOwner(steamIDLobby, steamIDNewOwner);
}

S_API bool SteamAPI_ISteamMatchmaking_SetLinkedLobby(intptr_t instancePtr, class CSteamID steamIDLobby, class CSteamID steamIDLobbyDependent)
{
    return (get_steam_client()->steam_matchmaking)->SetLinkedLobby(steamIDLobby, steamIDLobbyDependent);
}

S_API void SteamAPI_ISteamMatchmakingServerListResponse_ServerResponded(intptr_t instancePtr, HServerListRequest hRequest, int iServer)
{
    return ((ISteamMatchmakingServerListResponse *)instancePtr)->ServerResponded(hRequest, iServer);
}

S_API void SteamAPI_ISteamMatchmakingServerListResponse_ServerFailedToRespond(intptr_t instancePtr, HServerListRequest hRequest, int iServer)
{
    return ((ISteamMatchmakingServerListResponse *)instancePtr)->ServerFailedToRespond(hRequest, iServer);
}

S_API void SteamAPI_ISteamMatchmakingServerListResponse_RefreshComplete(intptr_t instancePtr, HServerListRequest hRequest, EMatchMakingServerResponse response)
{
    return ((ISteamMatchmakingServerListResponse *)instancePtr)->RefreshComplete(hRequest, response);
}

S_API void SteamAPI_ISteamMatchmakingPingResponse_ServerResponded(intptr_t instancePtr, class gameserveritem_t & server)
{
    return ((ISteamMatchmakingPingResponse *)instancePtr)->ServerResponded(server);
}

S_API void SteamAPI_ISteamMatchmakingPingResponse_ServerFailedToRespond(intptr_t instancePtr)
{
    return ((ISteamMatchmakingPingResponse *)instancePtr)->ServerFailedToRespond();
}

S_API void SteamAPI_ISteamMatchmakingPlayersResponse_AddPlayerToList(intptr_t instancePtr, const char * pchName, int nScore, float flTimePlayed)
{
    return ((ISteamMatchmakingPlayersResponse *)instancePtr)->AddPlayerToList(pchName, nScore, flTimePlayed);
}

S_API void SteamAPI_ISteamMatchmakingPlayersResponse_PlayersFailedToRespond(intptr_t instancePtr)
{
    return ((ISteamMatchmakingPlayersResponse *)instancePtr)->PlayersFailedToRespond();
}

S_API void SteamAPI_ISteamMatchmakingPlayersResponse_PlayersRefreshComplete(intptr_t instancePtr)
{
    return ((ISteamMatchmakingPlayersResponse *)instancePtr)->PlayersRefreshComplete();
}

S_API void SteamAPI_ISteamMatchmakingRulesResponse_RulesResponded(intptr_t instancePtr, const char * pchRule, const char * pchValue)
{
    return ((ISteamMatchmakingRulesResponse *)instancePtr)->RulesResponded(pchRule, pchValue);
}

S_API void SteamAPI_ISteamMatchmakingRulesResponse_RulesFailedToRespond(intptr_t instancePtr)
{
    return ((ISteamMatchmakingRulesResponse *)instancePtr)->RulesFailedToRespond();
}

S_API void SteamAPI_ISteamMatchmakingRulesResponse_RulesRefreshComplete(intptr_t instancePtr)
{
    return ((ISteamMatchmakingRulesResponse *)instancePtr)->RulesRefreshComplete();
}

S_API HServerListRequest SteamAPI_ISteamMatchmakingServers_RequestInternetServerList(intptr_t instancePtr, AppId_t iApp, struct MatchMakingKeyValuePair_t ** ppchFilters, uint32 nFilters, class ISteamMatchmakingServerListResponse * pRequestServersResponse)
{
    return (get_steam_client()->steam_matchmaking_servers)->RequestInternetServerList(iApp, ppchFilters, nFilters, pRequestServersResponse);
}

S_API HServerListRequest SteamAPI_ISteamMatchmakingServers_RequestLANServerList(intptr_t instancePtr, AppId_t iApp, class ISteamMatchmakingServerListResponse * pRequestServersResponse)
{
    return (get_steam_client()->steam_matchmaking_servers)->RequestLANServerList(iApp, pRequestServersResponse);
}

S_API HServerListRequest SteamAPI_ISteamMatchmakingServers_RequestFriendsServerList(intptr_t instancePtr, AppId_t iApp, struct MatchMakingKeyValuePair_t ** ppchFilters, uint32 nFilters, class ISteamMatchmakingServerListResponse * pRequestServersResponse)
{
    return (get_steam_client()->steam_matchmaking_servers)->RequestFriendsServerList(iApp, ppchFilters, nFilters, pRequestServersResponse);
}

S_API HServerListRequest SteamAPI_ISteamMatchmakingServers_RequestFavoritesServerList(intptr_t instancePtr, AppId_t iApp, struct MatchMakingKeyValuePair_t ** ppchFilters, uint32 nFilters, class ISteamMatchmakingServerListResponse * pRequestServersResponse)
{
    return (get_steam_client()->steam_matchmaking_servers)->RequestFavoritesServerList(iApp, ppchFilters, nFilters, pRequestServersResponse);
}

S_API HServerListRequest SteamAPI_ISteamMatchmakingServers_RequestHistoryServerList(intptr_t instancePtr, AppId_t iApp, struct MatchMakingKeyValuePair_t ** ppchFilters, uint32 nFilters, class ISteamMatchmakingServerListResponse * pRequestServersResponse)
{
    return (get_steam_client()->steam_matchmaking_servers)->RequestHistoryServerList(iApp, ppchFilters, nFilters, pRequestServersResponse);
}

S_API HServerListRequest SteamAPI_ISteamMatchmakingServers_RequestSpectatorServerList(intptr_t instancePtr, AppId_t iApp, struct MatchMakingKeyValuePair_t ** ppchFilters, uint32 nFilters, class ISteamMatchmakingServerListResponse * pRequestServersResponse)
{
    return (get_steam_client()->steam_matchmaking_servers)->RequestSpectatorServerList(iApp, ppchFilters, nFilters, pRequestServersResponse);
}

S_API void SteamAPI_ISteamMatchmakingServers_ReleaseRequest(intptr_t instancePtr, HServerListRequest hServerListRequest)
{
    return (get_steam_client()->steam_matchmaking_servers)->ReleaseRequest(hServerListRequest);
}

S_API class gameserveritem_t * SteamAPI_ISteamMatchmakingServers_GetServerDetails(intptr_t instancePtr, HServerListRequest hRequest, int iServer)
{
    return (get_steam_client()->steam_matchmaking_servers)->GetServerDetails(hRequest, iServer);
}

S_API void SteamAPI_ISteamMatchmakingServers_CancelQuery(intptr_t instancePtr, HServerListRequest hRequest)
{
    return (get_steam_client()->steam_matchmaking_servers)->CancelQuery(hRequest);
}

S_API void SteamAPI_ISteamMatchmakingServers_RefreshQuery(intptr_t instancePtr, HServerListRequest hRequest)
{
    return (get_steam_client()->steam_matchmaking_servers)->RefreshQuery(hRequest);
}

S_API bool SteamAPI_ISteamMatchmakingServers_IsRefreshing(intptr_t instancePtr, HServerListRequest hRequest)
{
    return (get_steam_client()->steam_matchmaking_servers)->IsRefreshing(hRequest);
}

S_API int SteamAPI_ISteamMatchmakingServers_GetServerCount(intptr_t instancePtr, HServerListRequest hRequest)
{
    return (get_steam_client()->steam_matchmaking_servers)->GetServerCount(hRequest);
}

S_API void SteamAPI_ISteamMatchmakingServers_RefreshServer(intptr_t instancePtr, HServerListRequest hRequest, int iServer)
{
    return (get_steam_client()->steam_matchmaking_servers)->RefreshServer(hRequest, iServer);
}

S_API HServerQuery SteamAPI_ISteamMatchmakingServers_PingServer(intptr_t instancePtr, uint32 unIP, uint16 usPort, class ISteamMatchmakingPingResponse * pRequestServersResponse)
{
    return (get_steam_client()->steam_matchmaking_servers)->PingServer(unIP, usPort, pRequestServersResponse);
}

S_API HServerQuery SteamAPI_ISteamMatchmakingServers_PlayerDetails(intptr_t instancePtr, uint32 unIP, uint16 usPort, class ISteamMatchmakingPlayersResponse * pRequestServersResponse)
{
    return (get_steam_client()->steam_matchmaking_servers)->PlayerDetails(unIP, usPort, pRequestServersResponse);
}

S_API HServerQuery SteamAPI_ISteamMatchmakingServers_ServerRules(intptr_t instancePtr, uint32 unIP, uint16 usPort, class ISteamMatchmakingRulesResponse * pRequestServersResponse)
{
    return (get_steam_client()->steam_matchmaking_servers)->ServerRules(unIP, usPort, pRequestServersResponse);
}

S_API void SteamAPI_ISteamMatchmakingServers_CancelServerQuery(intptr_t instancePtr, HServerQuery hServerQuery)
{
    return (get_steam_client()->steam_matchmaking_servers)->CancelServerQuery(hServerQuery);
}

S_API EGameSearchErrorCode_t SteamAPI_ISteamGameSearch_AddGameSearchParams(intptr_t instancePtr, const char * pchKeyToFind, const char * pchValuesToFind)
{
    return ((ISteamGameSearch *)instancePtr)->AddGameSearchParams(pchKeyToFind, pchValuesToFind);
}

S_API EGameSearchErrorCode_t SteamAPI_ISteamGameSearch_SearchForGameWithLobby(intptr_t instancePtr, class CSteamID steamIDLobby, int nPlayerMin, int nPlayerMax)
{
    return ((ISteamGameSearch *)instancePtr)->SearchForGameWithLobby(steamIDLobby, nPlayerMin, nPlayerMax);
}

S_API EGameSearchErrorCode_t SteamAPI_ISteamGameSearch_SearchForGameSolo(intptr_t instancePtr, int nPlayerMin, int nPlayerMax)
{
    return ((ISteamGameSearch *)instancePtr)->SearchForGameSolo(nPlayerMin, nPlayerMax);
}

S_API EGameSearchErrorCode_t SteamAPI_ISteamGameSearch_AcceptGame(intptr_t instancePtr)
{
    return ((ISteamGameSearch *)instancePtr)->AcceptGame();
}

S_API EGameSearchErrorCode_t SteamAPI_ISteamGameSearch_DeclineGame(intptr_t instancePtr)
{
    return ((ISteamGameSearch *)instancePtr)->DeclineGame();
}

S_API EGameSearchErrorCode_t SteamAPI_ISteamGameSearch_RetrieveConnectionDetails(intptr_t instancePtr, class CSteamID steamIDHost, char * pchConnectionDetails, int cubConnectionDetails)
{
    return ((ISteamGameSearch *)instancePtr)->RetrieveConnectionDetails(steamIDHost, pchConnectionDetails, cubConnectionDetails);
}

S_API EGameSearchErrorCode_t SteamAPI_ISteamGameSearch_EndGameSearch(intptr_t instancePtr)
{
    return ((ISteamGameSearch *)instancePtr)->EndGameSearch();
}

S_API EGameSearchErrorCode_t SteamAPI_ISteamGameSearch_SetGameHostParams(intptr_t instancePtr, const char * pchKey, const char * pchValue)
{
    return ((ISteamGameSearch *)instancePtr)->SetGameHostParams(pchKey, pchValue);
}

S_API EGameSearchErrorCode_t SteamAPI_ISteamGameSearch_SetConnectionDetails(intptr_t instancePtr, const char * pchConnectionDetails, int cubConnectionDetails)
{
    return ((ISteamGameSearch *)instancePtr)->SetConnectionDetails(pchConnectionDetails, cubConnectionDetails);
}

S_API EGameSearchErrorCode_t SteamAPI_ISteamGameSearch_RequestPlayersForGame(intptr_t instancePtr, int nPlayerMin, int nPlayerMax, int nMaxTeamSize)
{
    return ((ISteamGameSearch *)instancePtr)->RequestPlayersForGame(nPlayerMin, nPlayerMax, nMaxTeamSize);
}

S_API EGameSearchErrorCode_t SteamAPI_ISteamGameSearch_HostConfirmGameStart(intptr_t instancePtr, uint64 ullUniqueGameID)
{
    return ((ISteamGameSearch *)instancePtr)->HostConfirmGameStart(ullUniqueGameID);
}

S_API EGameSearchErrorCode_t SteamAPI_ISteamGameSearch_CancelRequestPlayersForGame(intptr_t instancePtr)
{
    return ((ISteamGameSearch *)instancePtr)->CancelRequestPlayersForGame();
}

S_API EGameSearchErrorCode_t SteamAPI_ISteamGameSearch_SubmitPlayerResult(intptr_t instancePtr, uint64 ullUniqueGameID, class CSteamID steamIDPlayer, EPlayerResult_t EPlayerResult)
{
    return ((ISteamGameSearch *)instancePtr)->SubmitPlayerResult(ullUniqueGameID, steamIDPlayer, EPlayerResult);
}

S_API EGameSearchErrorCode_t SteamAPI_ISteamGameSearch_EndGame(intptr_t instancePtr, uint64 ullUniqueGameID)
{
    return ((ISteamGameSearch *)instancePtr)->EndGame(ullUniqueGameID);
}

S_API uint32 SteamAPI_ISteamParties_GetNumActiveBeacons(intptr_t instancePtr)
{
    return ((ISteamParties *)instancePtr)->GetNumActiveBeacons();
}

S_API PartyBeaconID_t SteamAPI_ISteamParties_GetBeaconByIndex(intptr_t instancePtr, uint32 unIndex)
{
    return ((ISteamParties *)instancePtr)->GetBeaconByIndex(unIndex);
}

S_API bool SteamAPI_ISteamParties_GetBeaconDetails(intptr_t instancePtr, PartyBeaconID_t ulBeaconID, class CSteamID * pSteamIDBeaconOwner, struct SteamPartyBeaconLocation_t * pLocation, char * pchMetadata, int cchMetadata)
{
    return ((ISteamParties *)instancePtr)->GetBeaconDetails(ulBeaconID, pSteamIDBeaconOwner, pLocation, pchMetadata, cchMetadata);
}

S_API SteamAPICall_t SteamAPI_ISteamParties_JoinParty(intptr_t instancePtr, PartyBeaconID_t ulBeaconID)
{
    return ((ISteamParties *)instancePtr)->JoinParty(ulBeaconID);
}

S_API bool SteamAPI_ISteamParties_GetNumAvailableBeaconLocations(intptr_t instancePtr, uint32 * puNumLocations)
{
    return ((ISteamParties *)instancePtr)->GetNumAvailableBeaconLocations(puNumLocations);
}

S_API bool SteamAPI_ISteamParties_GetAvailableBeaconLocations(intptr_t instancePtr, struct SteamPartyBeaconLocation_t * pLocationList, uint32 uMaxNumLocations)
{
    return ((ISteamParties *)instancePtr)->GetAvailableBeaconLocations(pLocationList, uMaxNumLocations);
}

S_API SteamAPICall_t SteamAPI_ISteamParties_CreateBeacon(intptr_t instancePtr, uint32 unOpenSlots, struct SteamPartyBeaconLocation_t * pBeaconLocation, const char * pchConnectString, const char * pchMetadata)
{
    return ((ISteamParties *)instancePtr)->CreateBeacon(unOpenSlots, pBeaconLocation, pchConnectString, pchMetadata);
}

S_API void SteamAPI_ISteamParties_OnReservationCompleted(intptr_t instancePtr, PartyBeaconID_t ulBeacon, class CSteamID steamIDUser)
{
    return ((ISteamParties *)instancePtr)->OnReservationCompleted(ulBeacon, steamIDUser);
}

S_API void SteamAPI_ISteamParties_CancelReservation(intptr_t instancePtr, PartyBeaconID_t ulBeacon, class CSteamID steamIDUser)
{
    return ((ISteamParties *)instancePtr)->CancelReservation(ulBeacon, steamIDUser);
}

S_API SteamAPICall_t SteamAPI_ISteamParties_ChangeNumOpenSlots(intptr_t instancePtr, PartyBeaconID_t ulBeacon, uint32 unOpenSlots)
{
    return ((ISteamParties *)instancePtr)->ChangeNumOpenSlots(ulBeacon, unOpenSlots);
}

S_API bool SteamAPI_ISteamParties_DestroyBeacon(intptr_t instancePtr, PartyBeaconID_t ulBeacon)
{
    return ((ISteamParties *)instancePtr)->DestroyBeacon(ulBeacon);
}

S_API bool SteamAPI_ISteamParties_GetBeaconLocationData(intptr_t instancePtr, struct SteamPartyBeaconLocation_t BeaconLocation, ESteamPartyBeaconLocationData eData, char * pchDataStringOut, int cchDataStringOut)
{
    return ((ISteamParties *)instancePtr)->GetBeaconLocationData(BeaconLocation, eData, pchDataStringOut, cchDataStringOut);
}

S_API bool SteamAPI_ISteamRemoteStorage_FileWrite(intptr_t instancePtr, const char * pchFile, const void * pvData, int32 cubData)
{
    return (get_steam_client()->steam_remote_storage)->FileWrite(pchFile, pvData, cubData);
}

S_API int32 SteamAPI_ISteamRemoteStorage_FileRead(intptr_t instancePtr, const char * pchFile, void * pvData, int32 cubDataToRead)
{
    return (get_steam_client()->steam_remote_storage)->FileRead(pchFile, pvData, cubDataToRead);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_FileWriteAsync(intptr_t instancePtr, const char * pchFile, const void * pvData, uint32 cubData)
{
    return (get_steam_client()->steam_remote_storage)->FileWriteAsync(pchFile, pvData, cubData);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_FileReadAsync(intptr_t instancePtr, const char * pchFile, uint32 nOffset, uint32 cubToRead)
{
    return (get_steam_client()->steam_remote_storage)->FileReadAsync(pchFile, nOffset, cubToRead);
}

S_API bool SteamAPI_ISteamRemoteStorage_FileReadAsyncComplete(intptr_t instancePtr, SteamAPICall_t hReadCall, void * pvBuffer, uint32 cubToRead)
{
    return (get_steam_client()->steam_remote_storage)->FileReadAsyncComplete(hReadCall, pvBuffer, cubToRead);
}

S_API bool SteamAPI_ISteamRemoteStorage_FileForget(intptr_t instancePtr, const char * pchFile)
{
    return (get_steam_client()->steam_remote_storage)->FileForget(pchFile);
}

S_API bool SteamAPI_ISteamRemoteStorage_FileDelete(intptr_t instancePtr, const char * pchFile)
{
    return (get_steam_client()->steam_remote_storage)->FileDelete(pchFile);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_FileShare(intptr_t instancePtr, const char * pchFile)
{
    return (get_steam_client()->steam_remote_storage)->FileShare(pchFile);
}

S_API bool SteamAPI_ISteamRemoteStorage_SetSyncPlatforms(intptr_t instancePtr, const char * pchFile, ERemoteStoragePlatform eRemoteStoragePlatform)
{
    return (get_steam_client()->steam_remote_storage)->SetSyncPlatforms(pchFile, eRemoteStoragePlatform);
}

S_API UGCFileWriteStreamHandle_t SteamAPI_ISteamRemoteStorage_FileWriteStreamOpen(intptr_t instancePtr, const char * pchFile)
{
    return (get_steam_client()->steam_remote_storage)->FileWriteStreamOpen(pchFile);
}

S_API bool SteamAPI_ISteamRemoteStorage_FileWriteStreamWriteChunk(intptr_t instancePtr, UGCFileWriteStreamHandle_t writeHandle, const void * pvData, int32 cubData)
{
    return (get_steam_client()->steam_remote_storage)->FileWriteStreamWriteChunk(writeHandle, pvData, cubData);
}

S_API bool SteamAPI_ISteamRemoteStorage_FileWriteStreamClose(intptr_t instancePtr, UGCFileWriteStreamHandle_t writeHandle)
{
    return (get_steam_client()->steam_remote_storage)->FileWriteStreamClose(writeHandle);
}

S_API bool SteamAPI_ISteamRemoteStorage_FileWriteStreamCancel(intptr_t instancePtr, UGCFileWriteStreamHandle_t writeHandle)
{
    return (get_steam_client()->steam_remote_storage)->FileWriteStreamCancel(writeHandle);
}

S_API bool SteamAPI_ISteamRemoteStorage_FileExists(intptr_t instancePtr, const char * pchFile)
{
    return (get_steam_client()->steam_remote_storage)->FileExists(pchFile);
}

S_API bool SteamAPI_ISteamRemoteStorage_FilePersisted(intptr_t instancePtr, const char * pchFile)
{
    return (get_steam_client()->steam_remote_storage)->FilePersisted(pchFile);
}

S_API int32 SteamAPI_ISteamRemoteStorage_GetFileSize(intptr_t instancePtr, const char * pchFile)
{
    return (get_steam_client()->steam_remote_storage)->GetFileSize(pchFile);
}

S_API int64 SteamAPI_ISteamRemoteStorage_GetFileTimestamp(intptr_t instancePtr, const char * pchFile)
{
    return (get_steam_client()->steam_remote_storage)->GetFileTimestamp(pchFile);
}

S_API ERemoteStoragePlatform SteamAPI_ISteamRemoteStorage_GetSyncPlatforms(intptr_t instancePtr, const char * pchFile)
{
    return (get_steam_client()->steam_remote_storage)->GetSyncPlatforms(pchFile);
}

S_API int32 SteamAPI_ISteamRemoteStorage_GetFileCount(intptr_t instancePtr)
{
    return (get_steam_client()->steam_remote_storage)->GetFileCount();
}

S_API const char * SteamAPI_ISteamRemoteStorage_GetFileNameAndSize(intptr_t instancePtr, int iFile, int32 * pnFileSizeInBytes)
{
    return (get_steam_client()->steam_remote_storage)->GetFileNameAndSize(iFile, pnFileSizeInBytes);
}

S_API bool SteamAPI_ISteamRemoteStorage_GetQuota(intptr_t instancePtr, uint64 * pnTotalBytes, uint64 * puAvailableBytes)
{
    return (get_steam_client()->steam_remote_storage)->GetQuota(pnTotalBytes, puAvailableBytes);
}

S_API bool SteamAPI_ISteamRemoteStorage_IsCloudEnabledForAccount(intptr_t instancePtr)
{
    return (get_steam_client()->steam_remote_storage)->IsCloudEnabledForAccount();
}

S_API bool SteamAPI_ISteamRemoteStorage_IsCloudEnabledForApp(intptr_t instancePtr)
{
    return (get_steam_client()->steam_remote_storage)->IsCloudEnabledForApp();
}

S_API void SteamAPI_ISteamRemoteStorage_SetCloudEnabledForApp(intptr_t instancePtr, bool bEnabled)
{
    return (get_steam_client()->steam_remote_storage)->SetCloudEnabledForApp(bEnabled);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_UGCDownload(intptr_t instancePtr, UGCHandle_t hContent, uint32 unPriority)
{
    return (get_steam_client()->steam_remote_storage)->UGCDownload(hContent, unPriority);
}

S_API bool SteamAPI_ISteamRemoteStorage_GetUGCDownloadProgress(intptr_t instancePtr, UGCHandle_t hContent, int32 * pnBytesDownloaded, int32 * pnBytesExpected)
{
    return (get_steam_client()->steam_remote_storage)->GetUGCDownloadProgress(hContent, pnBytesDownloaded, pnBytesExpected);
}

S_API bool SteamAPI_ISteamRemoteStorage_GetUGCDetails(intptr_t instancePtr, UGCHandle_t hContent, AppId_t * pnAppID, char ** ppchName, int32 * pnFileSizeInBytes, class CSteamID * pSteamIDOwner)
{
    return (get_steam_client()->steam_remote_storage)->GetUGCDetails(hContent, pnAppID, ppchName, pnFileSizeInBytes, pSteamIDOwner);
}

S_API int32 SteamAPI_ISteamRemoteStorage_UGCRead(intptr_t instancePtr, UGCHandle_t hContent, void * pvData, int32 cubDataToRead, uint32 cOffset, EUGCReadAction eAction)
{
    return (get_steam_client()->steam_remote_storage)->UGCRead(hContent, pvData, cubDataToRead, cOffset, eAction);
}

S_API int32 SteamAPI_ISteamRemoteStorage_GetCachedUGCCount(intptr_t instancePtr)
{
    return (get_steam_client()->steam_remote_storage)->GetCachedUGCCount();
}

S_API UGCHandle_t SteamAPI_ISteamRemoteStorage_GetCachedUGCHandle(intptr_t instancePtr, int32 iCachedContent)
{
    return (get_steam_client()->steam_remote_storage)->GetCachedUGCHandle(iCachedContent);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_PublishWorkshopFile(intptr_t instancePtr, const char * pchFile, const char * pchPreviewFile, AppId_t nConsumerAppId, const char * pchTitle, const char * pchDescription, ERemoteStoragePublishedFileVisibility eVisibility, struct SteamParamStringArray_t * pTags, EWorkshopFileType eWorkshopFileType)
{
    return (get_steam_client()->steam_remote_storage)->PublishWorkshopFile(pchFile, pchPreviewFile, nConsumerAppId, pchTitle, pchDescription, eVisibility, pTags, eWorkshopFileType);
}

S_API PublishedFileUpdateHandle_t SteamAPI_ISteamRemoteStorage_CreatePublishedFileUpdateRequest(intptr_t instancePtr, PublishedFileId_t unPublishedFileId)
{
    return (get_steam_client()->steam_remote_storage)->CreatePublishedFileUpdateRequest(unPublishedFileId);
}

S_API bool SteamAPI_ISteamRemoteStorage_UpdatePublishedFileFile(intptr_t instancePtr, PublishedFileUpdateHandle_t updateHandle, const char * pchFile)
{
    return (get_steam_client()->steam_remote_storage)->UpdatePublishedFileFile(updateHandle, pchFile);
}

S_API bool SteamAPI_ISteamRemoteStorage_UpdatePublishedFilePreviewFile(intptr_t instancePtr, PublishedFileUpdateHandle_t updateHandle, const char * pchPreviewFile)
{
    return (get_steam_client()->steam_remote_storage)->UpdatePublishedFilePreviewFile(updateHandle, pchPreviewFile);
}

S_API bool SteamAPI_ISteamRemoteStorage_UpdatePublishedFileTitle(intptr_t instancePtr, PublishedFileUpdateHandle_t updateHandle, const char * pchTitle)
{
    return (get_steam_client()->steam_remote_storage)->UpdatePublishedFileTitle(updateHandle, pchTitle);
}

S_API bool SteamAPI_ISteamRemoteStorage_UpdatePublishedFileDescription(intptr_t instancePtr, PublishedFileUpdateHandle_t updateHandle, const char * pchDescription)
{
    return (get_steam_client()->steam_remote_storage)->UpdatePublishedFileDescription(updateHandle, pchDescription);
}

S_API bool SteamAPI_ISteamRemoteStorage_UpdatePublishedFileVisibility(intptr_t instancePtr, PublishedFileUpdateHandle_t updateHandle, ERemoteStoragePublishedFileVisibility eVisibility)
{
    return (get_steam_client()->steam_remote_storage)->UpdatePublishedFileVisibility(updateHandle, eVisibility);
}

S_API bool SteamAPI_ISteamRemoteStorage_UpdatePublishedFileTags(intptr_t instancePtr, PublishedFileUpdateHandle_t updateHandle, struct SteamParamStringArray_t * pTags)
{
    return (get_steam_client()->steam_remote_storage)->UpdatePublishedFileTags(updateHandle, pTags);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_CommitPublishedFileUpdate(intptr_t instancePtr, PublishedFileUpdateHandle_t updateHandle)
{
    return (get_steam_client()->steam_remote_storage)->CommitPublishedFileUpdate(updateHandle);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_GetPublishedFileDetails(intptr_t instancePtr, PublishedFileId_t unPublishedFileId, uint32 unMaxSecondsOld)
{
    return (get_steam_client()->steam_remote_storage)->GetPublishedFileDetails(unPublishedFileId, unMaxSecondsOld);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_DeletePublishedFile(intptr_t instancePtr, PublishedFileId_t unPublishedFileId)
{
    return (get_steam_client()->steam_remote_storage)->DeletePublishedFile(unPublishedFileId);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_EnumerateUserPublishedFiles(intptr_t instancePtr, uint32 unStartIndex)
{
    return (get_steam_client()->steam_remote_storage)->EnumerateUserPublishedFiles(unStartIndex);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_SubscribePublishedFile(intptr_t instancePtr, PublishedFileId_t unPublishedFileId)
{
    return (get_steam_client()->steam_remote_storage)->SubscribePublishedFile(unPublishedFileId);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_EnumerateUserSubscribedFiles(intptr_t instancePtr, uint32 unStartIndex)
{
    return (get_steam_client()->steam_remote_storage)->EnumerateUserSubscribedFiles(unStartIndex);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_UnsubscribePublishedFile(intptr_t instancePtr, PublishedFileId_t unPublishedFileId)
{
    return (get_steam_client()->steam_remote_storage)->UnsubscribePublishedFile(unPublishedFileId);
}

S_API bool SteamAPI_ISteamRemoteStorage_UpdatePublishedFileSetChangeDescription(intptr_t instancePtr, PublishedFileUpdateHandle_t updateHandle, const char * pchChangeDescription)
{
    return (get_steam_client()->steam_remote_storage)->UpdatePublishedFileSetChangeDescription(updateHandle, pchChangeDescription);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_GetPublishedItemVoteDetails(intptr_t instancePtr, PublishedFileId_t unPublishedFileId)
{
    return (get_steam_client()->steam_remote_storage)->GetPublishedItemVoteDetails(unPublishedFileId);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_UpdateUserPublishedItemVote(intptr_t instancePtr, PublishedFileId_t unPublishedFileId, bool bVoteUp)
{
    return (get_steam_client()->steam_remote_storage)->UpdateUserPublishedItemVote(unPublishedFileId, bVoteUp);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_GetUserPublishedItemVoteDetails(intptr_t instancePtr, PublishedFileId_t unPublishedFileId)
{
    return (get_steam_client()->steam_remote_storage)->GetUserPublishedItemVoteDetails(unPublishedFileId);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_EnumerateUserSharedWorkshopFiles(intptr_t instancePtr, class CSteamID steamId, uint32 unStartIndex, struct SteamParamStringArray_t * pRequiredTags, struct SteamParamStringArray_t * pExcludedTags)
{
    return (get_steam_client()->steam_remote_storage)->EnumerateUserSharedWorkshopFiles(steamId, unStartIndex, pRequiredTags, pExcludedTags);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_PublishVideo(intptr_t instancePtr, EWorkshopVideoProvider eVideoProvider, const char * pchVideoAccount, const char * pchVideoIdentifier, const char * pchPreviewFile, AppId_t nConsumerAppId, const char * pchTitle, const char * pchDescription, ERemoteStoragePublishedFileVisibility eVisibility, struct SteamParamStringArray_t * pTags)
{
    return (get_steam_client()->steam_remote_storage)->PublishVideo(eVideoProvider, pchVideoAccount, pchVideoIdentifier, pchPreviewFile, nConsumerAppId, pchTitle, pchDescription, eVisibility, pTags);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_SetUserPublishedFileAction(intptr_t instancePtr, PublishedFileId_t unPublishedFileId, EWorkshopFileAction eAction)
{
    return (get_steam_client()->steam_remote_storage)->SetUserPublishedFileAction(unPublishedFileId, eAction);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_EnumeratePublishedFilesByUserAction(intptr_t instancePtr, EWorkshopFileAction eAction, uint32 unStartIndex)
{
    return (get_steam_client()->steam_remote_storage)->EnumeratePublishedFilesByUserAction(eAction, unStartIndex);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_EnumeratePublishedWorkshopFiles(intptr_t instancePtr, EWorkshopEnumerationType eEnumerationType, uint32 unStartIndex, uint32 unCount, uint32 unDays, struct SteamParamStringArray_t * pTags, struct SteamParamStringArray_t * pUserTags)
{
    return (get_steam_client()->steam_remote_storage)->EnumeratePublishedWorkshopFiles(eEnumerationType, unStartIndex, unCount, unDays, pTags, pUserTags);
}

S_API SteamAPICall_t SteamAPI_ISteamRemoteStorage_UGCDownloadToLocation(intptr_t instancePtr, UGCHandle_t hContent, const char * pchLocation, uint32 unPriority)
{
    return (get_steam_client()->steam_remote_storage)->UGCDownloadToLocation(hContent, pchLocation, unPriority);
}

S_API bool SteamAPI_ISteamUserStats_RequestCurrentStats(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user_stats)->RequestCurrentStats();
}

S_API bool SteamAPI_ISteamUserStats_GetStat(intptr_t instancePtr, const char * pchName, int32 * pData)
{
    return (get_steam_client()->steam_user_stats)->GetStat(pchName, pData);
}

S_API bool SteamAPI_ISteamUserStats_GetStat0(intptr_t instancePtr, const char * pchName, float * pData)
{
    return (get_steam_client()->steam_user_stats)->GetStat(pchName, pData);
}

S_API bool SteamAPI_ISteamUserStats_SetStat(intptr_t instancePtr, const char * pchName, int32 nData)
{
    return (get_steam_client()->steam_user_stats)->SetStat(pchName, nData);
}

S_API bool SteamAPI_ISteamUserStats_SetStat0(intptr_t instancePtr, const char * pchName, float fData)
{
    return (get_steam_client()->steam_user_stats)->SetStat(pchName, fData);
}

S_API bool SteamAPI_ISteamUserStats_UpdateAvgRateStat(intptr_t instancePtr, const char * pchName, float flCountThisSession, double dSessionLength)
{
    return (get_steam_client()->steam_user_stats)->UpdateAvgRateStat(pchName, flCountThisSession, dSessionLength);
}

S_API bool SteamAPI_ISteamUserStats_GetAchievement(intptr_t instancePtr, const char * pchName, bool * pbAchieved)
{
    return (get_steam_client()->steam_user_stats)->GetAchievement(pchName, pbAchieved);
}

S_API bool SteamAPI_ISteamUserStats_SetAchievement(intptr_t instancePtr, const char * pchName)
{
    return (get_steam_client()->steam_user_stats)->SetAchievement(pchName);
}

S_API bool SteamAPI_ISteamUserStats_ClearAchievement(intptr_t instancePtr, const char * pchName)
{
    return (get_steam_client()->steam_user_stats)->ClearAchievement(pchName);
}

S_API bool SteamAPI_ISteamUserStats_GetAchievementAndUnlockTime(intptr_t instancePtr, const char * pchName, bool * pbAchieved, uint32 * punUnlockTime)
{
    return (get_steam_client()->steam_user_stats)->GetAchievementAndUnlockTime(pchName, pbAchieved, punUnlockTime);
}

S_API bool SteamAPI_ISteamUserStats_StoreStats(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user_stats)->StoreStats();
}

S_API int SteamAPI_ISteamUserStats_GetAchievementIcon(intptr_t instancePtr, const char * pchName)
{
    return (get_steam_client()->steam_user_stats)->GetAchievementIcon(pchName);
}

S_API const char * SteamAPI_ISteamUserStats_GetAchievementDisplayAttribute(intptr_t instancePtr, const char * pchName, const char * pchKey)
{
    return (get_steam_client()->steam_user_stats)->GetAchievementDisplayAttribute(pchName, pchKey);
}

S_API bool SteamAPI_ISteamUserStats_IndicateAchievementProgress(intptr_t instancePtr, const char * pchName, uint32 nCurProgress, uint32 nMaxProgress)
{
    return (get_steam_client()->steam_user_stats)->IndicateAchievementProgress(pchName, nCurProgress, nMaxProgress);
}

S_API uint32 SteamAPI_ISteamUserStats_GetNumAchievements(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user_stats)->GetNumAchievements();
}

S_API const char * SteamAPI_ISteamUserStats_GetAchievementName(intptr_t instancePtr, uint32 iAchievement)
{
    return (get_steam_client()->steam_user_stats)->GetAchievementName(iAchievement);
}

S_API SteamAPICall_t SteamAPI_ISteamUserStats_RequestUserStats(intptr_t instancePtr, class CSteamID steamIDUser)
{
    return (get_steam_client()->steam_user_stats)->RequestUserStats(steamIDUser);
}

S_API bool SteamAPI_ISteamUserStats_GetUserStat(intptr_t instancePtr, class CSteamID steamIDUser, const char * pchName, int32 * pData)
{
    return (get_steam_client()->steam_user_stats)->GetUserStat(steamIDUser, pchName, pData);
}

S_API bool SteamAPI_ISteamUserStats_GetUserStat0(intptr_t instancePtr, class CSteamID steamIDUser, const char * pchName, float * pData)
{
    return (get_steam_client()->steam_user_stats)->GetUserStat(steamIDUser, pchName, pData);
}

S_API bool SteamAPI_ISteamUserStats_GetUserAchievement(intptr_t instancePtr, class CSteamID steamIDUser, const char * pchName, bool * pbAchieved)
{
    return (get_steam_client()->steam_user_stats)->GetUserAchievement(steamIDUser, pchName, pbAchieved);
}

S_API bool SteamAPI_ISteamUserStats_GetUserAchievementAndUnlockTime(intptr_t instancePtr, class CSteamID steamIDUser, const char * pchName, bool * pbAchieved, uint32 * punUnlockTime)
{
    return (get_steam_client()->steam_user_stats)->GetUserAchievementAndUnlockTime(steamIDUser, pchName, pbAchieved, punUnlockTime);
}

S_API bool SteamAPI_ISteamUserStats_ResetAllStats(intptr_t instancePtr, bool bAchievementsToo)
{
    return (get_steam_client()->steam_user_stats)->ResetAllStats(bAchievementsToo);
}

S_API SteamAPICall_t SteamAPI_ISteamUserStats_FindOrCreateLeaderboard(intptr_t instancePtr, const char * pchLeaderboardName, ELeaderboardSortMethod eLeaderboardSortMethod, ELeaderboardDisplayType eLeaderboardDisplayType)
{
    return (get_steam_client()->steam_user_stats)->FindOrCreateLeaderboard(pchLeaderboardName, eLeaderboardSortMethod, eLeaderboardDisplayType);
}

S_API SteamAPICall_t SteamAPI_ISteamUserStats_FindLeaderboard(intptr_t instancePtr, const char * pchLeaderboardName)
{
    return (get_steam_client()->steam_user_stats)->FindLeaderboard(pchLeaderboardName);
}

S_API const char * SteamAPI_ISteamUserStats_GetLeaderboardName(intptr_t instancePtr, SteamLeaderboard_t hSteamLeaderboard)
{
    return (get_steam_client()->steam_user_stats)->GetLeaderboardName(hSteamLeaderboard);
}

S_API int SteamAPI_ISteamUserStats_GetLeaderboardEntryCount(intptr_t instancePtr, SteamLeaderboard_t hSteamLeaderboard)
{
    return (get_steam_client()->steam_user_stats)->GetLeaderboardEntryCount(hSteamLeaderboard);
}

S_API ELeaderboardSortMethod SteamAPI_ISteamUserStats_GetLeaderboardSortMethod(intptr_t instancePtr, SteamLeaderboard_t hSteamLeaderboard)
{
    return (get_steam_client()->steam_user_stats)->GetLeaderboardSortMethod(hSteamLeaderboard);
}

S_API ELeaderboardDisplayType SteamAPI_ISteamUserStats_GetLeaderboardDisplayType(intptr_t instancePtr, SteamLeaderboard_t hSteamLeaderboard)
{
    return (get_steam_client()->steam_user_stats)->GetLeaderboardDisplayType(hSteamLeaderboard);
}

S_API SteamAPICall_t SteamAPI_ISteamUserStats_DownloadLeaderboardEntries(intptr_t instancePtr, SteamLeaderboard_t hSteamLeaderboard, ELeaderboardDataRequest eLeaderboardDataRequest, int nRangeStart, int nRangeEnd)
{
    return (get_steam_client()->steam_user_stats)->DownloadLeaderboardEntries(hSteamLeaderboard, eLeaderboardDataRequest, nRangeStart, nRangeEnd);
}

S_API SteamAPICall_t SteamAPI_ISteamUserStats_DownloadLeaderboardEntriesForUsers(intptr_t instancePtr, SteamLeaderboard_t hSteamLeaderboard, class CSteamID * prgUsers, int cUsers)
{
    return (get_steam_client()->steam_user_stats)->DownloadLeaderboardEntriesForUsers(hSteamLeaderboard, prgUsers, cUsers);
}

S_API bool SteamAPI_ISteamUserStats_GetDownloadedLeaderboardEntry(intptr_t instancePtr, SteamLeaderboardEntries_t hSteamLeaderboardEntries, int index, struct LeaderboardEntry_t * pLeaderboardEntry, int32 * pDetails, int cDetailsMax)
{
    return (get_steam_client()->steam_user_stats)->GetDownloadedLeaderboardEntry(hSteamLeaderboardEntries, index, pLeaderboardEntry, pDetails, cDetailsMax);
}

S_API SteamAPICall_t SteamAPI_ISteamUserStats_UploadLeaderboardScore(intptr_t instancePtr, SteamLeaderboard_t hSteamLeaderboard, ELeaderboardUploadScoreMethod eLeaderboardUploadScoreMethod, int32 nScore, const int32 * pScoreDetails, int cScoreDetailsCount)
{
    return (get_steam_client()->steam_user_stats)->UploadLeaderboardScore(hSteamLeaderboard, eLeaderboardUploadScoreMethod, nScore, pScoreDetails, cScoreDetailsCount);
}

S_API SteamAPICall_t SteamAPI_ISteamUserStats_AttachLeaderboardUGC(intptr_t instancePtr, SteamLeaderboard_t hSteamLeaderboard, UGCHandle_t hUGC)
{
    return (get_steam_client()->steam_user_stats)->AttachLeaderboardUGC(hSteamLeaderboard, hUGC);
}

S_API SteamAPICall_t SteamAPI_ISteamUserStats_GetNumberOfCurrentPlayers(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user_stats)->GetNumberOfCurrentPlayers();
}

S_API SteamAPICall_t SteamAPI_ISteamUserStats_RequestGlobalAchievementPercentages(intptr_t instancePtr)
{
    return (get_steam_client()->steam_user_stats)->RequestGlobalAchievementPercentages();
}

S_API int SteamAPI_ISteamUserStats_GetMostAchievedAchievementInfo(intptr_t instancePtr, char * pchName, uint32 unNameBufLen, float * pflPercent, bool * pbAchieved)
{
    return (get_steam_client()->steam_user_stats)->GetMostAchievedAchievementInfo(pchName, unNameBufLen, pflPercent, pbAchieved);
}

S_API int SteamAPI_ISteamUserStats_GetNextMostAchievedAchievementInfo(intptr_t instancePtr, int iIteratorPrevious, char * pchName, uint32 unNameBufLen, float * pflPercent, bool * pbAchieved)
{
    return (get_steam_client()->steam_user_stats)->GetNextMostAchievedAchievementInfo(iIteratorPrevious, pchName, unNameBufLen, pflPercent, pbAchieved);
}

S_API bool SteamAPI_ISteamUserStats_GetAchievementAchievedPercent(intptr_t instancePtr, const char * pchName, float * pflPercent)
{
    return (get_steam_client()->steam_user_stats)->GetAchievementAchievedPercent(pchName, pflPercent);
}

S_API SteamAPICall_t SteamAPI_ISteamUserStats_RequestGlobalStats(intptr_t instancePtr, int nHistoryDays)
{
    return (get_steam_client()->steam_user_stats)->RequestGlobalStats(nHistoryDays);
}

S_API bool SteamAPI_ISteamUserStats_GetGlobalStat(intptr_t instancePtr, const char * pchStatName, int64 * pData)
{
    return (get_steam_client()->steam_user_stats)->GetGlobalStat(pchStatName, pData);
}

S_API bool SteamAPI_ISteamUserStats_GetGlobalStat0(intptr_t instancePtr, const char * pchStatName, double * pData)
{
    return (get_steam_client()->steam_user_stats)->GetGlobalStat(pchStatName, pData);
}

S_API int32 SteamAPI_ISteamUserStats_GetGlobalStatHistory(intptr_t instancePtr, const char * pchStatName, int64 * pData, uint32 cubData)
{
    return (get_steam_client()->steam_user_stats)->GetGlobalStatHistory(pchStatName, pData, cubData);
}

S_API int32 SteamAPI_ISteamUserStats_GetGlobalStatHistory0(intptr_t instancePtr, const char * pchStatName, double * pData, uint32 cubData)
{
    return (get_steam_client()->steam_user_stats)->GetGlobalStatHistory(pchStatName, pData, cubData);
}

S_API bool SteamAPI_ISteamApps_BIsSubscribed(intptr_t instancePtr)
{
    return ((ISteamApps *)instancePtr)->BIsSubscribed();
}

S_API bool SteamAPI_ISteamApps_BIsLowViolence(intptr_t instancePtr)
{
    return ((ISteamApps *)instancePtr)->BIsLowViolence();
}

S_API bool SteamAPI_ISteamApps_BIsCybercafe(intptr_t instancePtr)
{
    return ((ISteamApps *)instancePtr)->BIsCybercafe();
}

S_API bool SteamAPI_ISteamApps_BIsVACBanned(intptr_t instancePtr)
{
    return ((ISteamApps *)instancePtr)->BIsVACBanned();
}

S_API const char * SteamAPI_ISteamApps_GetCurrentGameLanguage(intptr_t instancePtr)
{
    return ((ISteamApps *)instancePtr)->GetCurrentGameLanguage();
}

S_API const char * SteamAPI_ISteamApps_GetAvailableGameLanguages(intptr_t instancePtr)
{
    return ((ISteamApps *)instancePtr)->GetAvailableGameLanguages();
}

S_API bool SteamAPI_ISteamApps_BIsSubscribedApp(intptr_t instancePtr, AppId_t appID)
{
    return ((ISteamApps *)instancePtr)->BIsSubscribedApp(appID);
}

S_API bool SteamAPI_ISteamApps_BIsDlcInstalled(intptr_t instancePtr, AppId_t appID)
{
    return ((ISteamApps *)instancePtr)->BIsDlcInstalled(appID);
}

S_API uint32 SteamAPI_ISteamApps_GetEarliestPurchaseUnixTime(intptr_t instancePtr, AppId_t nAppID)
{
    return ((ISteamApps *)instancePtr)->GetEarliestPurchaseUnixTime(nAppID);
}

S_API bool SteamAPI_ISteamApps_BIsSubscribedFromFreeWeekend(intptr_t instancePtr)
{
    return ((ISteamApps *)instancePtr)->BIsSubscribedFromFreeWeekend();
}

S_API int SteamAPI_ISteamApps_GetDLCCount(intptr_t instancePtr)
{
    return ((ISteamApps *)instancePtr)->GetDLCCount();
}

S_API bool SteamAPI_ISteamApps_BGetDLCDataByIndex(intptr_t instancePtr, int iDLC, AppId_t * pAppID, bool * pbAvailable, char * pchName, int cchNameBufferSize)
{
    return ((ISteamApps *)instancePtr)->BGetDLCDataByIndex(iDLC, pAppID, pbAvailable, pchName, cchNameBufferSize);
}

S_API void SteamAPI_ISteamApps_InstallDLC(intptr_t instancePtr, AppId_t nAppID)
{
    return ((ISteamApps *)instancePtr)->InstallDLC(nAppID);
}

S_API void SteamAPI_ISteamApps_UninstallDLC(intptr_t instancePtr, AppId_t nAppID)
{
    return ((ISteamApps *)instancePtr)->UninstallDLC(nAppID);
}

S_API void SteamAPI_ISteamApps_RequestAppProofOfPurchaseKey(intptr_t instancePtr, AppId_t nAppID)
{
    return ((ISteamApps *)instancePtr)->RequestAppProofOfPurchaseKey(nAppID);
}

S_API bool SteamAPI_ISteamApps_GetCurrentBetaName(intptr_t instancePtr, char * pchName, int cchNameBufferSize)
{
    return ((ISteamApps *)instancePtr)->GetCurrentBetaName(pchName, cchNameBufferSize);
}

S_API bool SteamAPI_ISteamApps_MarkContentCorrupt(intptr_t instancePtr, bool bMissingFilesOnly)
{
    return ((ISteamApps *)instancePtr)->MarkContentCorrupt(bMissingFilesOnly);
}

S_API uint32 SteamAPI_ISteamApps_GetInstalledDepots(intptr_t instancePtr, AppId_t appID, DepotId_t * pvecDepots, uint32 cMaxDepots)
{
    return ((ISteamApps *)instancePtr)->GetInstalledDepots(appID, pvecDepots, cMaxDepots);
}

S_API uint32 SteamAPI_ISteamApps_GetAppInstallDir(intptr_t instancePtr, AppId_t appID, char * pchFolder, uint32 cchFolderBufferSize)
{
    return ((ISteamApps *)instancePtr)->GetAppInstallDir(appID, pchFolder, cchFolderBufferSize);
}

S_API bool SteamAPI_ISteamApps_BIsAppInstalled(intptr_t instancePtr, AppId_t appID)
{
    return ((ISteamApps *)instancePtr)->BIsAppInstalled(appID);
}

S_API uint64 SteamAPI_ISteamApps_GetAppOwner(intptr_t instancePtr)
{
    return ((ISteamApps *)instancePtr)->GetAppOwner().ConvertToUint64();
}

S_API const char * SteamAPI_ISteamApps_GetLaunchQueryParam(intptr_t instancePtr, const char * pchKey)
{
    return ((ISteamApps *)instancePtr)->GetLaunchQueryParam(pchKey);
}

S_API bool SteamAPI_ISteamApps_GetDlcDownloadProgress(intptr_t instancePtr, AppId_t nAppID, uint64 * punBytesDownloaded, uint64 * punBytesTotal)
{
    return ((ISteamApps *)instancePtr)->GetDlcDownloadProgress(nAppID, punBytesDownloaded, punBytesTotal);
}

S_API int SteamAPI_ISteamApps_GetAppBuildId(intptr_t instancePtr)
{
    return ((ISteamApps *)instancePtr)->GetAppBuildId();
}

S_API void SteamAPI_ISteamApps_RequestAllProofOfPurchaseKeys(intptr_t instancePtr)
{
    return ((ISteamApps *)instancePtr)->RequestAllProofOfPurchaseKeys();
}

S_API SteamAPICall_t SteamAPI_ISteamApps_GetFileDetails(intptr_t instancePtr, const char * pszFileName)
{
    return ((ISteamApps *)instancePtr)->GetFileDetails(pszFileName);
}

S_API int SteamAPI_ISteamApps_GetLaunchCommandLine(intptr_t instancePtr, char * pszCommandLine, int cubCommandLine)
{
    return ((ISteamApps *)instancePtr)->GetLaunchCommandLine(pszCommandLine, cubCommandLine);
}

S_API bool SteamAPI_ISteamApps_BIsSubscribedFromFamilySharing(intptr_t instancePtr)
{
    return ((ISteamApps *)instancePtr)->BIsSubscribedFromFamilySharing();
}

S_API bool SteamAPI_ISteamNetworking_SendP2PPacket(intptr_t instancePtr, class CSteamID steamIDRemote, const void * pubData, uint32 cubData, EP2PSend eP2PSendType, int nChannel)
{
    return ((ISteamNetworking *)instancePtr)->SendP2PPacket(steamIDRemote, pubData, cubData, eP2PSendType, nChannel);
}

S_API bool SteamAPI_ISteamNetworking_IsP2PPacketAvailable(intptr_t instancePtr, uint32 * pcubMsgSize, int nChannel)
{
    return ((ISteamNetworking *)instancePtr)->IsP2PPacketAvailable(pcubMsgSize, nChannel);
}

S_API bool SteamAPI_ISteamNetworking_ReadP2PPacket(intptr_t instancePtr, void * pubDest, uint32 cubDest, uint32 * pcubMsgSize, class CSteamID * psteamIDRemote, int nChannel)
{
    return ((ISteamNetworking *)instancePtr)->ReadP2PPacket(pubDest, cubDest, pcubMsgSize, psteamIDRemote, nChannel);
}

S_API bool SteamAPI_ISteamNetworking_AcceptP2PSessionWithUser(intptr_t instancePtr, class CSteamID steamIDRemote)
{
    return ((ISteamNetworking *)instancePtr)->AcceptP2PSessionWithUser(steamIDRemote);
}

S_API bool SteamAPI_ISteamNetworking_CloseP2PSessionWithUser(intptr_t instancePtr, class CSteamID steamIDRemote)
{
    return ((ISteamNetworking *)instancePtr)->CloseP2PSessionWithUser(steamIDRemote);
}

S_API bool SteamAPI_ISteamNetworking_CloseP2PChannelWithUser(intptr_t instancePtr, class CSteamID steamIDRemote, int nChannel)
{
    return ((ISteamNetworking *)instancePtr)->CloseP2PChannelWithUser(steamIDRemote, nChannel);
}

S_API bool SteamAPI_ISteamNetworking_GetP2PSessionState(intptr_t instancePtr, class CSteamID steamIDRemote, struct P2PSessionState_t * pConnectionState)
{
    return ((ISteamNetworking *)instancePtr)->GetP2PSessionState(steamIDRemote, pConnectionState);
}

S_API bool SteamAPI_ISteamNetworking_AllowP2PPacketRelay(intptr_t instancePtr, bool bAllow)
{
    return ((ISteamNetworking *)instancePtr)->AllowP2PPacketRelay(bAllow);
}

S_API SNetListenSocket_t SteamAPI_ISteamNetworking_CreateListenSocket(intptr_t instancePtr, int nVirtualP2PPort, uint32 nIP, uint16 nPort, bool bAllowUseOfPacketRelay)
{
    return ((ISteamNetworking *)instancePtr)->CreateListenSocket(nVirtualP2PPort, nIP, nPort, bAllowUseOfPacketRelay);
}

S_API SNetSocket_t SteamAPI_ISteamNetworking_CreateP2PConnectionSocket(intptr_t instancePtr, class CSteamID steamIDTarget, int nVirtualPort, int nTimeoutSec, bool bAllowUseOfPacketRelay)
{
    return ((ISteamNetworking *)instancePtr)->CreateP2PConnectionSocket(steamIDTarget, nVirtualPort, nTimeoutSec, bAllowUseOfPacketRelay);
}

S_API SNetSocket_t SteamAPI_ISteamNetworking_CreateConnectionSocket(intptr_t instancePtr, uint32 nIP, uint16 nPort, int nTimeoutSec)
{
    return ((ISteamNetworking *)instancePtr)->CreateConnectionSocket(nIP, nPort, nTimeoutSec);
}

S_API bool SteamAPI_ISteamNetworking_DestroySocket(intptr_t instancePtr, SNetSocket_t hSocket, bool bNotifyRemoteEnd)
{
    return ((ISteamNetworking *)instancePtr)->DestroySocket(hSocket, bNotifyRemoteEnd);
}

S_API bool SteamAPI_ISteamNetworking_DestroyListenSocket(intptr_t instancePtr, SNetListenSocket_t hSocket, bool bNotifyRemoteEnd)
{
    return ((ISteamNetworking *)instancePtr)->DestroyListenSocket(hSocket, bNotifyRemoteEnd);
}

S_API bool SteamAPI_ISteamNetworking_SendDataOnSocket(intptr_t instancePtr, SNetSocket_t hSocket, void * pubData, uint32 cubData, bool bReliable)
{
    return ((ISteamNetworking *)instancePtr)->SendDataOnSocket(hSocket, pubData, cubData, bReliable);
}

S_API bool SteamAPI_ISteamNetworking_IsDataAvailableOnSocket(intptr_t instancePtr, SNetSocket_t hSocket, uint32 * pcubMsgSize)
{
    return ((ISteamNetworking *)instancePtr)->IsDataAvailableOnSocket(hSocket, pcubMsgSize);
}

S_API bool SteamAPI_ISteamNetworking_RetrieveDataFromSocket(intptr_t instancePtr, SNetSocket_t hSocket, void * pubDest, uint32 cubDest, uint32 * pcubMsgSize)
{
    return ((ISteamNetworking *)instancePtr)->RetrieveDataFromSocket(hSocket, pubDest, cubDest, pcubMsgSize);
}

S_API bool SteamAPI_ISteamNetworking_IsDataAvailable(intptr_t instancePtr, SNetListenSocket_t hListenSocket, uint32 * pcubMsgSize, SNetSocket_t * phSocket)
{
    return ((ISteamNetworking *)instancePtr)->IsDataAvailable(hListenSocket, pcubMsgSize, phSocket);
}

S_API bool SteamAPI_ISteamNetworking_RetrieveData(intptr_t instancePtr, SNetListenSocket_t hListenSocket, void * pubDest, uint32 cubDest, uint32 * pcubMsgSize, SNetSocket_t * phSocket)
{
    return ((ISteamNetworking *)instancePtr)->RetrieveData(hListenSocket, pubDest, cubDest, pcubMsgSize, phSocket);
}

S_API bool SteamAPI_ISteamNetworking_GetSocketInfo(intptr_t instancePtr, SNetSocket_t hSocket, class CSteamID * pSteamIDRemote, int * peSocketStatus, uint32 * punIPRemote, uint16 * punPortRemote)
{
    return ((ISteamNetworking *)instancePtr)->GetSocketInfo(hSocket, pSteamIDRemote, peSocketStatus, punIPRemote, punPortRemote);
}

S_API bool SteamAPI_ISteamNetworking_GetListenSocketInfo(intptr_t instancePtr, SNetListenSocket_t hListenSocket, uint32 * pnIP, uint16 * pnPort)
{
    return ((ISteamNetworking *)instancePtr)->GetListenSocketInfo(hListenSocket, pnIP, pnPort);
}

S_API ESNetSocketConnectionType SteamAPI_ISteamNetworking_GetSocketConnectionType(intptr_t instancePtr, SNetSocket_t hSocket)
{
    return ((ISteamNetworking *)instancePtr)->GetSocketConnectionType(hSocket);
}

S_API int SteamAPI_ISteamNetworking_GetMaxPacketSize(intptr_t instancePtr, SNetSocket_t hSocket)
{
    return ((ISteamNetworking *)instancePtr)->GetMaxPacketSize(hSocket);
}

S_API ScreenshotHandle SteamAPI_ISteamScreenshots_WriteScreenshot(intptr_t instancePtr, void * pubRGB, uint32 cubRGB, int nWidth, int nHeight)
{
    return (get_steam_client()->steam_screenshots)->WriteScreenshot(pubRGB, cubRGB, nWidth, nHeight);
}

S_API ScreenshotHandle SteamAPI_ISteamScreenshots_AddScreenshotToLibrary(intptr_t instancePtr, const char * pchFilename, const char * pchThumbnailFilename, int nWidth, int nHeight)
{
    return (get_steam_client()->steam_screenshots)->AddScreenshotToLibrary(pchFilename, pchThumbnailFilename, nWidth, nHeight);
}

S_API void SteamAPI_ISteamScreenshots_TriggerScreenshot(intptr_t instancePtr)
{
    return (get_steam_client()->steam_screenshots)->TriggerScreenshot();
}

S_API void SteamAPI_ISteamScreenshots_HookScreenshots(intptr_t instancePtr, bool bHook)
{
    return (get_steam_client()->steam_screenshots)->HookScreenshots(bHook);
}

S_API bool SteamAPI_ISteamScreenshots_SetLocation(intptr_t instancePtr, ScreenshotHandle hScreenshot, const char * pchLocation)
{
    return (get_steam_client()->steam_screenshots)->SetLocation(hScreenshot, pchLocation);
}

S_API bool SteamAPI_ISteamScreenshots_TagUser(intptr_t instancePtr, ScreenshotHandle hScreenshot, class CSteamID steamID)
{
    return (get_steam_client()->steam_screenshots)->TagUser(hScreenshot, steamID);
}

S_API bool SteamAPI_ISteamScreenshots_TagPublishedFile(intptr_t instancePtr, ScreenshotHandle hScreenshot, PublishedFileId_t unPublishedFileID)
{
    return (get_steam_client()->steam_screenshots)->TagPublishedFile(hScreenshot, unPublishedFileID);
}

S_API bool SteamAPI_ISteamScreenshots_IsScreenshotsHooked(intptr_t instancePtr)
{
    return (get_steam_client()->steam_screenshots)->IsScreenshotsHooked();
}

S_API ScreenshotHandle SteamAPI_ISteamScreenshots_AddVRScreenshotToLibrary(intptr_t instancePtr, EVRScreenshotType eType, const char * pchFilename, const char * pchVRFilename)
{
    return (get_steam_client()->steam_screenshots)->AddVRScreenshotToLibrary(eType, pchFilename, pchVRFilename);
}

S_API bool SteamAPI_ISteamMusic_BIsEnabled(intptr_t instancePtr)
{
    return ((ISteamMusic *)instancePtr)->BIsEnabled();
}

S_API bool SteamAPI_ISteamMusic_BIsPlaying(intptr_t instancePtr)
{
    return ((ISteamMusic *)instancePtr)->BIsPlaying();
}

S_API AudioPlayback_Status SteamAPI_ISteamMusic_GetPlaybackStatus(intptr_t instancePtr)
{
    return ((ISteamMusic *)instancePtr)->GetPlaybackStatus();
}

S_API void SteamAPI_ISteamMusic_Play(intptr_t instancePtr)
{
    return ((ISteamMusic *)instancePtr)->Play();
}

S_API void SteamAPI_ISteamMusic_Pause(intptr_t instancePtr)
{
    return ((ISteamMusic *)instancePtr)->Pause();
}

S_API void SteamAPI_ISteamMusic_PlayPrevious(intptr_t instancePtr)
{
    return ((ISteamMusic *)instancePtr)->PlayPrevious();
}

S_API void SteamAPI_ISteamMusic_PlayNext(intptr_t instancePtr)
{
    return ((ISteamMusic *)instancePtr)->PlayNext();
}

S_API void SteamAPI_ISteamMusic_SetVolume(intptr_t instancePtr, float flVolume)
{
    return ((ISteamMusic *)instancePtr)->SetVolume(flVolume);
}

S_API float SteamAPI_ISteamMusic_GetVolume(intptr_t instancePtr)
{
    return ((ISteamMusic *)instancePtr)->GetVolume();
}

S_API bool SteamAPI_ISteamMusicRemote_RegisterSteamMusicRemote(intptr_t instancePtr, const char * pchName)
{
    return ((ISteamMusicRemote *)instancePtr)->RegisterSteamMusicRemote(pchName);
}

S_API bool SteamAPI_ISteamMusicRemote_DeregisterSteamMusicRemote(intptr_t instancePtr)
{
    return ((ISteamMusicRemote *)instancePtr)->DeregisterSteamMusicRemote();
}

S_API bool SteamAPI_ISteamMusicRemote_BIsCurrentMusicRemote(intptr_t instancePtr)
{
    return ((ISteamMusicRemote *)instancePtr)->BIsCurrentMusicRemote();
}

S_API bool SteamAPI_ISteamMusicRemote_BActivationSuccess(intptr_t instancePtr, bool bValue)
{
    return ((ISteamMusicRemote *)instancePtr)->BActivationSuccess(bValue);
}

S_API bool SteamAPI_ISteamMusicRemote_SetDisplayName(intptr_t instancePtr, const char * pchDisplayName)
{
    return ((ISteamMusicRemote *)instancePtr)->SetDisplayName(pchDisplayName);
}

S_API bool SteamAPI_ISteamMusicRemote_SetPNGIcon_64x64(intptr_t instancePtr, void * pvBuffer, uint32 cbBufferLength)
{
    return ((ISteamMusicRemote *)instancePtr)->SetPNGIcon_64x64(pvBuffer, cbBufferLength);
}

S_API bool SteamAPI_ISteamMusicRemote_EnablePlayPrevious(intptr_t instancePtr, bool bValue)
{
    return ((ISteamMusicRemote *)instancePtr)->EnablePlayPrevious(bValue);
}

S_API bool SteamAPI_ISteamMusicRemote_EnablePlayNext(intptr_t instancePtr, bool bValue)
{
    return ((ISteamMusicRemote *)instancePtr)->EnablePlayNext(bValue);
}

S_API bool SteamAPI_ISteamMusicRemote_EnableShuffled(intptr_t instancePtr, bool bValue)
{
    return ((ISteamMusicRemote *)instancePtr)->EnableShuffled(bValue);
}

S_API bool SteamAPI_ISteamMusicRemote_EnableLooped(intptr_t instancePtr, bool bValue)
{
    return ((ISteamMusicRemote *)instancePtr)->EnableLooped(bValue);
}

S_API bool SteamAPI_ISteamMusicRemote_EnableQueue(intptr_t instancePtr, bool bValue)
{
    return ((ISteamMusicRemote *)instancePtr)->EnableQueue(bValue);
}

S_API bool SteamAPI_ISteamMusicRemote_EnablePlaylists(intptr_t instancePtr, bool bValue)
{
    return ((ISteamMusicRemote *)instancePtr)->EnablePlaylists(bValue);
}

S_API bool SteamAPI_ISteamMusicRemote_UpdatePlaybackStatus(intptr_t instancePtr, AudioPlayback_Status nStatus)
{
    return ((ISteamMusicRemote *)instancePtr)->UpdatePlaybackStatus(nStatus);
}

S_API bool SteamAPI_ISteamMusicRemote_UpdateShuffled(intptr_t instancePtr, bool bValue)
{
    return ((ISteamMusicRemote *)instancePtr)->UpdateShuffled(bValue);
}

S_API bool SteamAPI_ISteamMusicRemote_UpdateLooped(intptr_t instancePtr, bool bValue)
{
    return ((ISteamMusicRemote *)instancePtr)->UpdateLooped(bValue);
}

S_API bool SteamAPI_ISteamMusicRemote_UpdateVolume(intptr_t instancePtr, float flValue)
{
    return ((ISteamMusicRemote *)instancePtr)->UpdateVolume(flValue);
}

S_API bool SteamAPI_ISteamMusicRemote_CurrentEntryWillChange(intptr_t instancePtr)
{
    return ((ISteamMusicRemote *)instancePtr)->CurrentEntryWillChange();
}

S_API bool SteamAPI_ISteamMusicRemote_CurrentEntryIsAvailable(intptr_t instancePtr, bool bAvailable)
{
    return ((ISteamMusicRemote *)instancePtr)->CurrentEntryIsAvailable(bAvailable);
}

S_API bool SteamAPI_ISteamMusicRemote_UpdateCurrentEntryText(intptr_t instancePtr, const char * pchText)
{
    return ((ISteamMusicRemote *)instancePtr)->UpdateCurrentEntryText(pchText);
}

S_API bool SteamAPI_ISteamMusicRemote_UpdateCurrentEntryElapsedSeconds(intptr_t instancePtr, int nValue)
{
    return ((ISteamMusicRemote *)instancePtr)->UpdateCurrentEntryElapsedSeconds(nValue);
}

S_API bool SteamAPI_ISteamMusicRemote_UpdateCurrentEntryCoverArt(intptr_t instancePtr, void * pvBuffer, uint32 cbBufferLength)
{
    return ((ISteamMusicRemote *)instancePtr)->UpdateCurrentEntryCoverArt(pvBuffer, cbBufferLength);
}

S_API bool SteamAPI_ISteamMusicRemote_CurrentEntryDidChange(intptr_t instancePtr)
{
    return ((ISteamMusicRemote *)instancePtr)->CurrentEntryDidChange();
}

S_API bool SteamAPI_ISteamMusicRemote_QueueWillChange(intptr_t instancePtr)
{
    return ((ISteamMusicRemote *)instancePtr)->QueueWillChange();
}

S_API bool SteamAPI_ISteamMusicRemote_ResetQueueEntries(intptr_t instancePtr)
{
    return ((ISteamMusicRemote *)instancePtr)->ResetQueueEntries();
}

S_API bool SteamAPI_ISteamMusicRemote_SetQueueEntry(intptr_t instancePtr, int nID, int nPosition, const char * pchEntryText)
{
    return ((ISteamMusicRemote *)instancePtr)->SetQueueEntry(nID, nPosition, pchEntryText);
}

S_API bool SteamAPI_ISteamMusicRemote_SetCurrentQueueEntry(intptr_t instancePtr, int nID)
{
    return ((ISteamMusicRemote *)instancePtr)->SetCurrentQueueEntry(nID);
}

S_API bool SteamAPI_ISteamMusicRemote_QueueDidChange(intptr_t instancePtr)
{
    return ((ISteamMusicRemote *)instancePtr)->QueueDidChange();
}

S_API bool SteamAPI_ISteamMusicRemote_PlaylistWillChange(intptr_t instancePtr)
{
    return ((ISteamMusicRemote *)instancePtr)->PlaylistWillChange();
}

S_API bool SteamAPI_ISteamMusicRemote_ResetPlaylistEntries(intptr_t instancePtr)
{
    return ((ISteamMusicRemote *)instancePtr)->ResetPlaylistEntries();
}

S_API bool SteamAPI_ISteamMusicRemote_SetPlaylistEntry(intptr_t instancePtr, int nID, int nPosition, const char * pchEntryText)
{
    return ((ISteamMusicRemote *)instancePtr)->SetPlaylistEntry(nID, nPosition, pchEntryText);
}

S_API bool SteamAPI_ISteamMusicRemote_SetCurrentPlaylistEntry(intptr_t instancePtr, int nID)
{
    return ((ISteamMusicRemote *)instancePtr)->SetCurrentPlaylistEntry(nID);
}

S_API bool SteamAPI_ISteamMusicRemote_PlaylistDidChange(intptr_t instancePtr)
{
    return ((ISteamMusicRemote *)instancePtr)->PlaylistDidChange();
}

S_API HTTPRequestHandle SteamAPI_ISteamHTTP_CreateHTTPRequest(intptr_t instancePtr, EHTTPMethod eHTTPRequestMethod, const char * pchAbsoluteURL)
{
    return ((ISteamHTTP *)instancePtr)->CreateHTTPRequest(eHTTPRequestMethod, pchAbsoluteURL);
}

S_API bool SteamAPI_ISteamHTTP_SetHTTPRequestContextValue(intptr_t instancePtr, HTTPRequestHandle hRequest, uint64 ulContextValue)
{
    return ((ISteamHTTP *)instancePtr)->SetHTTPRequestContextValue(hRequest, ulContextValue);
}

S_API bool SteamAPI_ISteamHTTP_SetHTTPRequestNetworkActivityTimeout(intptr_t instancePtr, HTTPRequestHandle hRequest, uint32 unTimeoutSeconds)
{
    return ((ISteamHTTP *)instancePtr)->SetHTTPRequestNetworkActivityTimeout(hRequest, unTimeoutSeconds);
}

S_API bool SteamAPI_ISteamHTTP_SetHTTPRequestHeaderValue(intptr_t instancePtr, HTTPRequestHandle hRequest, const char * pchHeaderName, const char * pchHeaderValue)
{
    return ((ISteamHTTP *)instancePtr)->SetHTTPRequestHeaderValue(hRequest, pchHeaderName, pchHeaderValue);
}

S_API bool SteamAPI_ISteamHTTP_SetHTTPRequestGetOrPostParameter(intptr_t instancePtr, HTTPRequestHandle hRequest, const char * pchParamName, const char * pchParamValue)
{
    return ((ISteamHTTP *)instancePtr)->SetHTTPRequestGetOrPostParameter(hRequest, pchParamName, pchParamValue);
}

S_API bool SteamAPI_ISteamHTTP_SendHTTPRequest(intptr_t instancePtr, HTTPRequestHandle hRequest, SteamAPICall_t * pCallHandle)
{
    return ((ISteamHTTP *)instancePtr)->SendHTTPRequest(hRequest, pCallHandle);
}

S_API bool SteamAPI_ISteamHTTP_SendHTTPRequestAndStreamResponse(intptr_t instancePtr, HTTPRequestHandle hRequest, SteamAPICall_t * pCallHandle)
{
    return ((ISteamHTTP *)instancePtr)->SendHTTPRequestAndStreamResponse(hRequest, pCallHandle);
}

S_API bool SteamAPI_ISteamHTTP_DeferHTTPRequest(intptr_t instancePtr, HTTPRequestHandle hRequest)
{
    return ((ISteamHTTP *)instancePtr)->DeferHTTPRequest(hRequest);
}

S_API bool SteamAPI_ISteamHTTP_PrioritizeHTTPRequest(intptr_t instancePtr, HTTPRequestHandle hRequest)
{
    return ((ISteamHTTP *)instancePtr)->PrioritizeHTTPRequest(hRequest);
}

S_API bool SteamAPI_ISteamHTTP_GetHTTPResponseHeaderSize(intptr_t instancePtr, HTTPRequestHandle hRequest, const char * pchHeaderName, uint32 * unResponseHeaderSize)
{
    return ((ISteamHTTP *)instancePtr)->GetHTTPResponseHeaderSize(hRequest, pchHeaderName, unResponseHeaderSize);
}

S_API bool SteamAPI_ISteamHTTP_GetHTTPResponseHeaderValue(intptr_t instancePtr, HTTPRequestHandle hRequest, const char * pchHeaderName, uint8 * pHeaderValueBuffer, uint32 unBufferSize)
{
    return ((ISteamHTTP *)instancePtr)->GetHTTPResponseHeaderValue(hRequest, pchHeaderName, pHeaderValueBuffer, unBufferSize);
}

S_API bool SteamAPI_ISteamHTTP_GetHTTPResponseBodySize(intptr_t instancePtr, HTTPRequestHandle hRequest, uint32 * unBodySize)
{
    return ((ISteamHTTP *)instancePtr)->GetHTTPResponseBodySize(hRequest, unBodySize);
}

S_API bool SteamAPI_ISteamHTTP_GetHTTPResponseBodyData(intptr_t instancePtr, HTTPRequestHandle hRequest, uint8 * pBodyDataBuffer, uint32 unBufferSize)
{
    return ((ISteamHTTP *)instancePtr)->GetHTTPResponseBodyData(hRequest, pBodyDataBuffer, unBufferSize);
}

S_API bool SteamAPI_ISteamHTTP_GetHTTPStreamingResponseBodyData(intptr_t instancePtr, HTTPRequestHandle hRequest, uint32 cOffset, uint8 * pBodyDataBuffer, uint32 unBufferSize)
{
    return ((ISteamHTTP *)instancePtr)->GetHTTPStreamingResponseBodyData(hRequest, cOffset, pBodyDataBuffer, unBufferSize);
}

S_API bool SteamAPI_ISteamHTTP_ReleaseHTTPRequest(intptr_t instancePtr, HTTPRequestHandle hRequest)
{
    return ((ISteamHTTP *)instancePtr)->ReleaseHTTPRequest(hRequest);
}

S_API bool SteamAPI_ISteamHTTP_GetHTTPDownloadProgressPct(intptr_t instancePtr, HTTPRequestHandle hRequest, float * pflPercentOut)
{
    return ((ISteamHTTP *)instancePtr)->GetHTTPDownloadProgressPct(hRequest, pflPercentOut);
}

S_API bool SteamAPI_ISteamHTTP_SetHTTPRequestRawPostBody(intptr_t instancePtr, HTTPRequestHandle hRequest, const char * pchContentType, uint8 * pubBody, uint32 unBodyLen)
{
    return ((ISteamHTTP *)instancePtr)->SetHTTPRequestRawPostBody(hRequest, pchContentType, pubBody, unBodyLen);
}

S_API HTTPCookieContainerHandle SteamAPI_ISteamHTTP_CreateCookieContainer(intptr_t instancePtr, bool bAllowResponsesToModify)
{
    return ((ISteamHTTP *)instancePtr)->CreateCookieContainer(bAllowResponsesToModify);
}

S_API bool SteamAPI_ISteamHTTP_ReleaseCookieContainer(intptr_t instancePtr, HTTPCookieContainerHandle hCookieContainer)
{
    return ((ISteamHTTP *)instancePtr)->ReleaseCookieContainer(hCookieContainer);
}

S_API bool SteamAPI_ISteamHTTP_SetCookie(intptr_t instancePtr, HTTPCookieContainerHandle hCookieContainer, const char * pchHost, const char * pchUrl, const char * pchCookie)
{
    return ((ISteamHTTP *)instancePtr)->SetCookie(hCookieContainer, pchHost, pchUrl, pchCookie);
}

S_API bool SteamAPI_ISteamHTTP_SetHTTPRequestCookieContainer(intptr_t instancePtr, HTTPRequestHandle hRequest, HTTPCookieContainerHandle hCookieContainer)
{
    return ((ISteamHTTP *)instancePtr)->SetHTTPRequestCookieContainer(hRequest, hCookieContainer);
}

S_API bool SteamAPI_ISteamHTTP_SetHTTPRequestUserAgentInfo(intptr_t instancePtr, HTTPRequestHandle hRequest, const char * pchUserAgentInfo)
{
    return ((ISteamHTTP *)instancePtr)->SetHTTPRequestUserAgentInfo(hRequest, pchUserAgentInfo);
}

S_API bool SteamAPI_ISteamHTTP_SetHTTPRequestRequiresVerifiedCertificate(intptr_t instancePtr, HTTPRequestHandle hRequest, bool bRequireVerifiedCertificate)
{
    return ((ISteamHTTP *)instancePtr)->SetHTTPRequestRequiresVerifiedCertificate(hRequest, bRequireVerifiedCertificate);
}

S_API bool SteamAPI_ISteamHTTP_SetHTTPRequestAbsoluteTimeoutMS(intptr_t instancePtr, HTTPRequestHandle hRequest, uint32 unMilliseconds)
{
    return ((ISteamHTTP *)instancePtr)->SetHTTPRequestAbsoluteTimeoutMS(hRequest, unMilliseconds);
}

S_API bool SteamAPI_ISteamHTTP_GetHTTPRequestWasTimedOut(intptr_t instancePtr, HTTPRequestHandle hRequest, bool * pbWasTimedOut)
{
    return ((ISteamHTTP *)instancePtr)->GetHTTPRequestWasTimedOut(hRequest, pbWasTimedOut);
}

S_API bool SteamAPI_ISteamInput_Init(intptr_t instancePtr)
{
    return (get_steam_client()->steam_controller)->Init();
}

S_API bool SteamAPI_ISteamInput_Shutdown(intptr_t instancePtr)
{
    return (get_steam_client()->steam_controller)->Shutdown();
}

S_API void SteamAPI_ISteamInput_RunFrame(intptr_t instancePtr)
{
    return (get_steam_client()->steam_controller)->RunFrame();
}

S_API int SteamAPI_ISteamInput_GetConnectedControllers(intptr_t instancePtr, InputHandle_t * handlesOut)
{
    return (get_steam_client()->steam_controller)->GetConnectedControllers(handlesOut);
}

S_API InputActionSetHandle_t SteamAPI_ISteamInput_GetActionSetHandle(intptr_t instancePtr, const char * pszActionSetName)
{
    return (get_steam_client()->steam_controller)->GetActionSetHandle(pszActionSetName);
}

S_API void SteamAPI_ISteamInput_ActivateActionSet(intptr_t instancePtr, InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle)
{
    return (get_steam_client()->steam_controller)->ActivateActionSet(inputHandle, actionSetHandle);
}

S_API InputActionSetHandle_t SteamAPI_ISteamInput_GetCurrentActionSet(intptr_t instancePtr, InputHandle_t inputHandle)
{
    return (get_steam_client()->steam_controller)->GetCurrentActionSet(inputHandle);
}

S_API void SteamAPI_ISteamInput_ActivateActionSetLayer(intptr_t instancePtr, InputHandle_t inputHandle, InputActionSetHandle_t actionSetLayerHandle)
{
    return (get_steam_client()->steam_controller)->ActivateActionSetLayer(inputHandle, actionSetLayerHandle);
}

S_API void SteamAPI_ISteamInput_DeactivateActionSetLayer(intptr_t instancePtr, InputHandle_t inputHandle, InputActionSetHandle_t actionSetLayerHandle)
{
    return (get_steam_client()->steam_controller)->DeactivateActionSetLayer(inputHandle, actionSetLayerHandle);
}

S_API void SteamAPI_ISteamInput_DeactivateAllActionSetLayers(intptr_t instancePtr, InputHandle_t inputHandle)
{
    return (get_steam_client()->steam_controller)->DeactivateAllActionSetLayers(inputHandle);
}

S_API int SteamAPI_ISteamInput_GetActiveActionSetLayers(intptr_t instancePtr, InputHandle_t inputHandle, InputActionSetHandle_t * handlesOut)
{
    return (get_steam_client()->steam_controller)->GetActiveActionSetLayers(inputHandle, handlesOut);
}

S_API InputDigitalActionHandle_t SteamAPI_ISteamInput_GetDigitalActionHandle(intptr_t instancePtr, const char * pszActionName)
{
    return (get_steam_client()->steam_controller)->GetDigitalActionHandle(pszActionName);
}

S_API struct InputDigitalActionData_t SteamAPI_ISteamInput_GetDigitalActionData(intptr_t instancePtr, InputHandle_t inputHandle, InputDigitalActionHandle_t digitalActionHandle)
{
    return (get_steam_client()->steam_controller)->GetDigitalActionData(inputHandle, digitalActionHandle);
}

S_API int SteamAPI_ISteamInput_GetDigitalActionOrigins(intptr_t instancePtr, InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle, InputDigitalActionHandle_t digitalActionHandle, EInputActionOrigin * originsOut)
{
    return (get_steam_client()->steam_controller)->GetDigitalActionOrigins(inputHandle, actionSetHandle, digitalActionHandle, originsOut);
}

S_API InputAnalogActionHandle_t SteamAPI_ISteamInput_GetAnalogActionHandle(intptr_t instancePtr, const char * pszActionName)
{
    return (get_steam_client()->steam_controller)->GetAnalogActionHandle(pszActionName);
}

S_API struct InputAnalogActionData_t SteamAPI_ISteamInput_GetAnalogActionData(intptr_t instancePtr, InputHandle_t inputHandle, InputAnalogActionHandle_t analogActionHandle)
{
    return (get_steam_client()->steam_controller)->GetAnalogActionData(inputHandle, analogActionHandle);
}

S_API int SteamAPI_ISteamInput_GetAnalogActionOrigins(intptr_t instancePtr, InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle, InputAnalogActionHandle_t analogActionHandle, EInputActionOrigin * originsOut)
{
    return (get_steam_client()->steam_controller)->GetAnalogActionOrigins(inputHandle, actionSetHandle, analogActionHandle, originsOut);
}

S_API const char * SteamAPI_ISteamInput_GetGlyphForActionOrigin(intptr_t instancePtr, EInputActionOrigin eOrigin)
{
    return (get_steam_client()->steam_controller)->GetGlyphForActionOrigin(eOrigin);
}

S_API const char * SteamAPI_ISteamInput_GetStringForActionOrigin(intptr_t instancePtr, EInputActionOrigin eOrigin)
{
    return (get_steam_client()->steam_controller)->GetStringForActionOrigin(eOrigin);
}

S_API void SteamAPI_ISteamInput_StopAnalogActionMomentum(intptr_t instancePtr, InputHandle_t inputHandle, InputAnalogActionHandle_t eAction)
{
    return (get_steam_client()->steam_controller)->StopAnalogActionMomentum(inputHandle, eAction);
}

S_API struct InputMotionData_t SteamAPI_ISteamInput_GetMotionData(intptr_t instancePtr, InputHandle_t inputHandle)
{
    return (get_steam_client()->steam_controller)->GetMotionData(inputHandle);
}

S_API void SteamAPI_ISteamInput_TriggerVibration(intptr_t instancePtr, InputHandle_t inputHandle, unsigned short usLeftSpeed, unsigned short usRightSpeed)
{
    return (get_steam_client()->steam_controller)->TriggerVibration(inputHandle, usLeftSpeed, usRightSpeed);
}

S_API void SteamAPI_ISteamInput_SetLEDColor(intptr_t instancePtr, InputHandle_t inputHandle, uint8 nColorR, uint8 nColorG, uint8 nColorB, unsigned int nFlags)
{
    return (get_steam_client()->steam_controller)->SetLEDColor(inputHandle, nColorR, nColorG, nColorB, nFlags);
}

S_API void SteamAPI_ISteamInput_TriggerHapticPulse(intptr_t instancePtr, InputHandle_t inputHandle, ESteamControllerPad eTargetPad, unsigned short usDurationMicroSec)
{
    return (get_steam_client()->steam_controller)->TriggerHapticPulse(inputHandle, eTargetPad, usDurationMicroSec);
}

S_API void SteamAPI_ISteamInput_TriggerRepeatedHapticPulse(intptr_t instancePtr, InputHandle_t inputHandle, ESteamControllerPad eTargetPad, unsigned short usDurationMicroSec, unsigned short usOffMicroSec, unsigned short unRepeat, unsigned int nFlags)
{
    return (get_steam_client()->steam_controller)->TriggerRepeatedHapticPulse(inputHandle, eTargetPad, usDurationMicroSec, usOffMicroSec, unRepeat, nFlags);
}

S_API bool SteamAPI_ISteamInput_ShowBindingPanel(intptr_t instancePtr, InputHandle_t inputHandle)
{
    return (get_steam_client()->steam_controller)->ShowBindingPanel(inputHandle);
}

S_API ESteamInputType SteamAPI_ISteamInput_GetInputTypeForHandle(intptr_t instancePtr, InputHandle_t inputHandle)
{
    return (get_steam_client()->steam_controller)->GetInputTypeForHandle(inputHandle);
}

S_API InputHandle_t SteamAPI_ISteamInput_GetControllerForGamepadIndex(intptr_t instancePtr, int nIndex)
{
    return (get_steam_client()->steam_controller)->GetControllerForGamepadIndex(nIndex);
}

S_API int SteamAPI_ISteamInput_GetGamepadIndexForController(intptr_t instancePtr, InputHandle_t ulinputHandle)
{
    return (get_steam_client()->steam_controller)->GetGamepadIndexForController(ulinputHandle);
}

S_API const char * SteamAPI_ISteamInput_GetStringForXboxOrigin(intptr_t instancePtr, EXboxOrigin eOrigin)
{
    return (get_steam_client()->steam_controller)->GetStringForXboxOrigin(eOrigin);
}

S_API const char * SteamAPI_ISteamInput_GetGlyphForXboxOrigin(intptr_t instancePtr, EXboxOrigin eOrigin)
{
    return (get_steam_client()->steam_controller)->GetGlyphForXboxOrigin(eOrigin);
}

S_API EInputActionOrigin SteamAPI_ISteamInput_GetActionOriginFromXboxOrigin(intptr_t instancePtr, InputHandle_t inputHandle, EXboxOrigin eOrigin)
{
    return (get_steam_client()->steam_controller)->GetActionOriginFromXboxOrigin(inputHandle, eOrigin);
}

S_API EInputActionOrigin SteamAPI_ISteamInput_TranslateActionOrigin(intptr_t instancePtr, ESteamInputType eDestinationInputType, EInputActionOrigin eSourceOrigin)
{
    return (get_steam_client()->steam_controller)->TranslateActionOrigin(eDestinationInputType, eSourceOrigin);
}

S_API bool SteamAPI_ISteamController_Init(intptr_t instancePtr)
{
    return (get_steam_client()->steam_controller)->Init();
}

S_API bool SteamAPI_ISteamController_Shutdown(intptr_t instancePtr)
{
    return (get_steam_client()->steam_controller)->Shutdown();
}

S_API void SteamAPI_ISteamController_RunFrame(intptr_t instancePtr)
{
    return (get_steam_client()->steam_controller)->RunFrame();
}

S_API int SteamAPI_ISteamController_GetConnectedControllers(intptr_t instancePtr, ControllerHandle_t * handlesOut)
{
    return (get_steam_client()->steam_controller)->GetConnectedControllers(handlesOut);
}

S_API ControllerActionSetHandle_t SteamAPI_ISteamController_GetActionSetHandle(intptr_t instancePtr, const char * pszActionSetName)
{
    return (get_steam_client()->steam_controller)->GetActionSetHandle(pszActionSetName);
}

S_API void SteamAPI_ISteamController_ActivateActionSet(intptr_t instancePtr, ControllerHandle_t controllerHandle, ControllerActionSetHandle_t actionSetHandle)
{
    return (get_steam_client()->steam_controller)->ActivateActionSet(controllerHandle, actionSetHandle);
}

S_API ControllerActionSetHandle_t SteamAPI_ISteamController_GetCurrentActionSet(intptr_t instancePtr, ControllerHandle_t controllerHandle)
{
    return (get_steam_client()->steam_controller)->GetCurrentActionSet(controllerHandle);
}

S_API void SteamAPI_ISteamController_ActivateActionSetLayer(intptr_t instancePtr, ControllerHandle_t controllerHandle, ControllerActionSetHandle_t actionSetLayerHandle)
{
    return (get_steam_client()->steam_controller)->ActivateActionSetLayer(controllerHandle, actionSetLayerHandle);
}

S_API void SteamAPI_ISteamController_DeactivateActionSetLayer(intptr_t instancePtr, ControllerHandle_t controllerHandle, ControllerActionSetHandle_t actionSetLayerHandle)
{
    return (get_steam_client()->steam_controller)->DeactivateActionSetLayer(controllerHandle, actionSetLayerHandle);
}

S_API void SteamAPI_ISteamController_DeactivateAllActionSetLayers(intptr_t instancePtr, ControllerHandle_t controllerHandle)
{
    return (get_steam_client()->steam_controller)->DeactivateAllActionSetLayers(controllerHandle);
}

S_API int SteamAPI_ISteamController_GetActiveActionSetLayers(intptr_t instancePtr, ControllerHandle_t controllerHandle, ControllerActionSetHandle_t * handlesOut)
{
    return (get_steam_client()->steam_controller)->GetActiveActionSetLayers(controllerHandle, handlesOut);
}

S_API ControllerDigitalActionHandle_t SteamAPI_ISteamController_GetDigitalActionHandle(intptr_t instancePtr, const char * pszActionName)
{
    return (get_steam_client()->steam_controller)->GetDigitalActionHandle(pszActionName);
}

S_API struct InputDigitalActionData_t SteamAPI_ISteamController_GetDigitalActionData(intptr_t instancePtr, ControllerHandle_t controllerHandle, ControllerDigitalActionHandle_t digitalActionHandle)
{
    return (get_steam_client()->steam_controller)->GetDigitalActionData(controllerHandle, digitalActionHandle);
}

S_API int SteamAPI_ISteamController_GetDigitalActionOrigins(intptr_t instancePtr, ControllerHandle_t controllerHandle, ControllerActionSetHandle_t actionSetHandle, ControllerDigitalActionHandle_t digitalActionHandle, EControllerActionOrigin * originsOut)
{
    return (get_steam_client()->steam_controller)->GetDigitalActionOrigins(controllerHandle, actionSetHandle, digitalActionHandle, originsOut);
}

S_API ControllerAnalogActionHandle_t SteamAPI_ISteamController_GetAnalogActionHandle(intptr_t instancePtr, const char * pszActionName)
{
    return (get_steam_client()->steam_controller)->GetAnalogActionHandle(pszActionName);
}

S_API struct InputAnalogActionData_t SteamAPI_ISteamController_GetAnalogActionData(intptr_t instancePtr, ControllerHandle_t controllerHandle, ControllerAnalogActionHandle_t analogActionHandle)
{
    return (get_steam_client()->steam_controller)->GetAnalogActionData(controllerHandle, analogActionHandle);
}

S_API int SteamAPI_ISteamController_GetAnalogActionOrigins(intptr_t instancePtr, ControllerHandle_t controllerHandle, ControllerActionSetHandle_t actionSetHandle, ControllerAnalogActionHandle_t analogActionHandle, EControllerActionOrigin * originsOut)
{
    return (get_steam_client()->steam_controller)->GetAnalogActionOrigins(controllerHandle, actionSetHandle, analogActionHandle, originsOut);
}

S_API const char * SteamAPI_ISteamController_GetGlyphForActionOrigin(intptr_t instancePtr, EControllerActionOrigin eOrigin)
{
    return (get_steam_client()->steam_controller)->GetGlyphForActionOrigin(eOrigin);
}

S_API const char * SteamAPI_ISteamController_GetStringForActionOrigin(intptr_t instancePtr, EControllerActionOrigin eOrigin)
{
    return (get_steam_client()->steam_controller)->GetStringForActionOrigin(eOrigin);
}

S_API void SteamAPI_ISteamController_StopAnalogActionMomentum(intptr_t instancePtr, ControllerHandle_t controllerHandle, ControllerAnalogActionHandle_t eAction)
{
    return (get_steam_client()->steam_controller)->StopAnalogActionMomentum(controllerHandle, eAction);
}

S_API struct InputMotionData_t SteamAPI_ISteamController_GetMotionData(intptr_t instancePtr, ControllerHandle_t controllerHandle)
{
    return (get_steam_client()->steam_controller)->GetMotionData(controllerHandle);
}

S_API void SteamAPI_ISteamController_TriggerHapticPulse(intptr_t instancePtr, ControllerHandle_t controllerHandle, ESteamControllerPad eTargetPad, unsigned short usDurationMicroSec)
{
    return (get_steam_client()->steam_controller)->TriggerHapticPulse(controllerHandle, eTargetPad, usDurationMicroSec);
}

S_API void SteamAPI_ISteamController_TriggerRepeatedHapticPulse(intptr_t instancePtr, ControllerHandle_t controllerHandle, ESteamControllerPad eTargetPad, unsigned short usDurationMicroSec, unsigned short usOffMicroSec, unsigned short unRepeat, unsigned int nFlags)
{
    return (get_steam_client()->steam_controller)->TriggerRepeatedHapticPulse(controllerHandle, eTargetPad, usDurationMicroSec, usOffMicroSec, unRepeat, nFlags);
}

S_API void SteamAPI_ISteamController_TriggerVibration(intptr_t instancePtr, ControllerHandle_t controllerHandle, unsigned short usLeftSpeed, unsigned short usRightSpeed)
{
    return (get_steam_client()->steam_controller)->TriggerVibration(controllerHandle, usLeftSpeed, usRightSpeed);
}

S_API void SteamAPI_ISteamController_SetLEDColor(intptr_t instancePtr, ControllerHandle_t controllerHandle, uint8 nColorR, uint8 nColorG, uint8 nColorB, unsigned int nFlags)
{
    return (get_steam_client()->steam_controller)->SetLEDColor(controllerHandle, nColorR, nColorG, nColorB, nFlags);
}

S_API bool SteamAPI_ISteamController_ShowBindingPanel(intptr_t instancePtr, ControllerHandle_t controllerHandle)
{
    return (get_steam_client()->steam_controller)->ShowBindingPanel(controllerHandle);
}

S_API ESteamInputType SteamAPI_ISteamController_GetInputTypeForHandle(intptr_t instancePtr, ControllerHandle_t controllerHandle)
{
    return (get_steam_client()->steam_controller)->GetInputTypeForHandle(controllerHandle);
}

S_API ControllerHandle_t SteamAPI_ISteamController_GetControllerForGamepadIndex(intptr_t instancePtr, int nIndex)
{
    return (get_steam_client()->steam_controller)->GetControllerForGamepadIndex(nIndex);
}

S_API int SteamAPI_ISteamController_GetGamepadIndexForController(intptr_t instancePtr, ControllerHandle_t ulControllerHandle)
{
    return (get_steam_client()->steam_controller)->GetGamepadIndexForController(ulControllerHandle);
}

S_API const char * SteamAPI_ISteamController_GetStringForXboxOrigin(intptr_t instancePtr, EXboxOrigin eOrigin)
{
    return (get_steam_client()->steam_controller)->GetStringForXboxOrigin(eOrigin);
}

S_API const char * SteamAPI_ISteamController_GetGlyphForXboxOrigin(intptr_t instancePtr, EXboxOrigin eOrigin)
{
    return (get_steam_client()->steam_controller)->GetGlyphForXboxOrigin(eOrigin);
}

S_API EControllerActionOrigin SteamAPI_ISteamController_GetActionOriginFromXboxOrigin(intptr_t instancePtr, ControllerHandle_t controllerHandle, EXboxOrigin eOrigin)
{
    return (get_steam_client()->steam_controller)->GetActionOriginFromXboxOrigin_(controllerHandle, eOrigin);
}

S_API EControllerActionOrigin SteamAPI_ISteamController_TranslateActionOrigin(intptr_t instancePtr, ESteamInputType eDestinationInputType, EControllerActionOrigin eSourceOrigin)
{
    return (get_steam_client()->steam_controller)->TranslateActionOrigin(eDestinationInputType, eSourceOrigin);
}

S_API UGCQueryHandle_t SteamAPI_ISteamUGC_CreateQueryUserUGCRequest(intptr_t instancePtr, AccountID_t unAccountID, EUserUGCList eListType, EUGCMatchingUGCType eMatchingUGCType, EUserUGCListSortOrder eSortOrder, AppId_t nCreatorAppID, AppId_t nConsumerAppID, uint32 unPage)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->CreateQueryUserUGCRequest(unAccountID, eListType, eMatchingUGCType, eSortOrder, nCreatorAppID, nConsumerAppID, unPage);
}

S_API UGCQueryHandle_t SteamAPI_ISteamUGC_CreateQueryAllUGCRequest(intptr_t instancePtr, EUGCQuery eQueryType, EUGCMatchingUGCType eMatchingeMatchingUGCTypeFileType, AppId_t nCreatorAppID, AppId_t nConsumerAppID, uint32 unPage)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->CreateQueryAllUGCRequest(eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, unPage);
}

S_API UGCQueryHandle_t SteamAPI_ISteamUGC_CreateQueryAllUGCRequest0(intptr_t instancePtr, EUGCQuery eQueryType, EUGCMatchingUGCType eMatchingeMatchingUGCTypeFileType, AppId_t nCreatorAppID, AppId_t nConsumerAppID, const char * pchCursor)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->CreateQueryAllUGCRequest(eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, pchCursor);
}

S_API UGCQueryHandle_t SteamAPI_ISteamUGC_CreateQueryUGCDetailsRequest(intptr_t instancePtr, PublishedFileId_t * pvecPublishedFileID, uint32 unNumPublishedFileIDs)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->CreateQueryUGCDetailsRequest(pvecPublishedFileID, unNumPublishedFileIDs);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_SendQueryUGCRequest(intptr_t instancePtr, UGCQueryHandle_t handle)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SendQueryUGCRequest(handle);
}

S_API bool SteamAPI_ISteamUGC_GetQueryUGCResult(intptr_t instancePtr, UGCQueryHandle_t handle, uint32 index, struct SteamUGCDetails_t * pDetails)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->GetQueryUGCResult(handle, index, pDetails);
}

S_API bool SteamAPI_ISteamUGC_GetQueryUGCPreviewURL(intptr_t instancePtr, UGCQueryHandle_t handle, uint32 index, char * pchURL, uint32 cchURLSize)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->GetQueryUGCPreviewURL(handle, index, pchURL, cchURLSize);
}

S_API bool SteamAPI_ISteamUGC_GetQueryUGCMetadata(intptr_t instancePtr, UGCQueryHandle_t handle, uint32 index, char * pchMetadata, uint32 cchMetadatasize)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->GetQueryUGCMetadata(handle, index, pchMetadata, cchMetadatasize);
}

S_API bool SteamAPI_ISteamUGC_GetQueryUGCChildren(intptr_t instancePtr, UGCQueryHandle_t handle, uint32 index, PublishedFileId_t * pvecPublishedFileID, uint32 cMaxEntries)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->GetQueryUGCChildren(handle, index, pvecPublishedFileID, cMaxEntries);
}

S_API bool SteamAPI_ISteamUGC_GetQueryUGCStatistic(intptr_t instancePtr, UGCQueryHandle_t handle, uint32 index, EItemStatistic eStatType, uint64 * pStatValue)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->GetQueryUGCStatistic(handle, index, eStatType, pStatValue);
}

S_API uint32 SteamAPI_ISteamUGC_GetQueryUGCNumAdditionalPreviews(intptr_t instancePtr, UGCQueryHandle_t handle, uint32 index)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->GetQueryUGCNumAdditionalPreviews(handle, index);
}

S_API bool SteamAPI_ISteamUGC_GetQueryUGCAdditionalPreview(intptr_t instancePtr, UGCQueryHandle_t handle, uint32 index, uint32 previewIndex, char * pchURLOrVideoID, uint32 cchURLSize, char * pchOriginalFileName, uint32 cchOriginalFileNameSize, EItemPreviewType * pPreviewType)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->GetQueryUGCAdditionalPreview(handle, index, previewIndex, pchURLOrVideoID, cchURLSize, pchOriginalFileName, cchOriginalFileNameSize, pPreviewType);
}

S_API uint32 SteamAPI_ISteamUGC_GetQueryUGCNumKeyValueTags(intptr_t instancePtr, UGCQueryHandle_t handle, uint32 index)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->GetQueryUGCNumKeyValueTags(handle, index);
}

S_API bool SteamAPI_ISteamUGC_GetQueryUGCKeyValueTag(intptr_t instancePtr, UGCQueryHandle_t handle, uint32 index, uint32 keyValueTagIndex, char * pchKey, uint32 cchKeySize, char * pchValue, uint32 cchValueSize)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->GetQueryUGCKeyValueTag(handle, index, keyValueTagIndex, pchKey, cchKeySize, pchValue, cchValueSize);
}

S_API bool SteamAPI_ISteamUGC_ReleaseQueryUGCRequest(intptr_t instancePtr, UGCQueryHandle_t handle)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->ReleaseQueryUGCRequest(handle);
}

S_API bool SteamAPI_ISteamUGC_AddRequiredTag(intptr_t instancePtr, UGCQueryHandle_t handle, const char * pTagName)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->AddRequiredTag(handle, pTagName);
}

S_API bool SteamAPI_ISteamUGC_AddExcludedTag(intptr_t instancePtr, UGCQueryHandle_t handle, const char * pTagName)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->AddExcludedTag(handle, pTagName);
}

S_API bool SteamAPI_ISteamUGC_SetReturnOnlyIDs(intptr_t instancePtr, UGCQueryHandle_t handle, bool bReturnOnlyIDs)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetReturnOnlyIDs(handle, bReturnOnlyIDs);
}

S_API bool SteamAPI_ISteamUGC_SetReturnKeyValueTags(intptr_t instancePtr, UGCQueryHandle_t handle, bool bReturnKeyValueTags)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetReturnKeyValueTags(handle, bReturnKeyValueTags);
}

S_API bool SteamAPI_ISteamUGC_SetReturnLongDescription(intptr_t instancePtr, UGCQueryHandle_t handle, bool bReturnLongDescription)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetReturnLongDescription(handle, bReturnLongDescription);
}

S_API bool SteamAPI_ISteamUGC_SetReturnMetadata(intptr_t instancePtr, UGCQueryHandle_t handle, bool bReturnMetadata)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetReturnMetadata(handle, bReturnMetadata);
}

S_API bool SteamAPI_ISteamUGC_SetReturnChildren(intptr_t instancePtr, UGCQueryHandle_t handle, bool bReturnChildren)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetReturnChildren(handle, bReturnChildren);
}

S_API bool SteamAPI_ISteamUGC_SetReturnAdditionalPreviews(intptr_t instancePtr, UGCQueryHandle_t handle, bool bReturnAdditionalPreviews)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetReturnAdditionalPreviews(handle, bReturnAdditionalPreviews);
}

S_API bool SteamAPI_ISteamUGC_SetReturnTotalOnly(intptr_t instancePtr, UGCQueryHandle_t handle, bool bReturnTotalOnly)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetReturnTotalOnly(handle, bReturnTotalOnly);
}

S_API bool SteamAPI_ISteamUGC_SetReturnPlaytimeStats(intptr_t instancePtr, UGCQueryHandle_t handle, uint32 unDays)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetReturnPlaytimeStats(handle, unDays);
}

S_API bool SteamAPI_ISteamUGC_SetLanguage(intptr_t instancePtr, UGCQueryHandle_t handle, const char * pchLanguage)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetLanguage(handle, pchLanguage);
}

S_API bool SteamAPI_ISteamUGC_SetAllowCachedResponse(intptr_t instancePtr, UGCQueryHandle_t handle, uint32 unMaxAgeSeconds)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetAllowCachedResponse(handle, unMaxAgeSeconds);
}

S_API bool SteamAPI_ISteamUGC_SetCloudFileNameFilter(intptr_t instancePtr, UGCQueryHandle_t handle, const char * pMatchCloudFileName)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetCloudFileNameFilter(handle, pMatchCloudFileName);
}

S_API bool SteamAPI_ISteamUGC_SetMatchAnyTag(intptr_t instancePtr, UGCQueryHandle_t handle, bool bMatchAnyTag)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetMatchAnyTag(handle, bMatchAnyTag);
}

S_API bool SteamAPI_ISteamUGC_SetSearchText(intptr_t instancePtr, UGCQueryHandle_t handle, const char * pSearchText)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetSearchText(handle, pSearchText);
}

S_API bool SteamAPI_ISteamUGC_SetRankedByTrendDays(intptr_t instancePtr, UGCQueryHandle_t handle, uint32 unDays)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetRankedByTrendDays(handle, unDays);
}

S_API bool SteamAPI_ISteamUGC_AddRequiredKeyValueTag(intptr_t instancePtr, UGCQueryHandle_t handle, const char * pKey, const char * pValue)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->AddRequiredKeyValueTag(handle, pKey, pValue);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_RequestUGCDetails(intptr_t instancePtr, PublishedFileId_t nPublishedFileID, uint32 unMaxAgeSeconds)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->RequestUGCDetails(nPublishedFileID, unMaxAgeSeconds);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_CreateItem(intptr_t instancePtr, AppId_t nConsumerAppId, EWorkshopFileType eFileType)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->CreateItem(nConsumerAppId, eFileType);
}

S_API UGCUpdateHandle_t SteamAPI_ISteamUGC_StartItemUpdate(intptr_t instancePtr, AppId_t nConsumerAppId, PublishedFileId_t nPublishedFileID)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->StartItemUpdate(nConsumerAppId, nPublishedFileID);
}

S_API bool SteamAPI_ISteamUGC_SetItemTitle(intptr_t instancePtr, UGCUpdateHandle_t handle, const char * pchTitle)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetItemTitle(handle, pchTitle);
}

S_API bool SteamAPI_ISteamUGC_SetItemDescription(intptr_t instancePtr, UGCUpdateHandle_t handle, const char * pchDescription)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetItemDescription(handle, pchDescription);
}

S_API bool SteamAPI_ISteamUGC_SetItemUpdateLanguage(intptr_t instancePtr, UGCUpdateHandle_t handle, const char * pchLanguage)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetItemUpdateLanguage(handle, pchLanguage);
}

S_API bool SteamAPI_ISteamUGC_SetItemMetadata(intptr_t instancePtr, UGCUpdateHandle_t handle, const char * pchMetaData)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetItemMetadata(handle, pchMetaData);
}

S_API bool SteamAPI_ISteamUGC_SetItemVisibility(intptr_t instancePtr, UGCUpdateHandle_t handle, ERemoteStoragePublishedFileVisibility eVisibility)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetItemVisibility(handle, eVisibility);
}

S_API bool SteamAPI_ISteamUGC_SetItemTags(intptr_t instancePtr, UGCUpdateHandle_t updateHandle, const struct SteamParamStringArray_t * pTags)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetItemTags(updateHandle, pTags);
}

S_API bool SteamAPI_ISteamUGC_SetItemContent(intptr_t instancePtr, UGCUpdateHandle_t handle, const char * pszContentFolder)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetItemContent(handle, pszContentFolder);
}

S_API bool SteamAPI_ISteamUGC_SetItemPreview(intptr_t instancePtr, UGCUpdateHandle_t handle, const char * pszPreviewFile)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetItemPreview(handle, pszPreviewFile);
}

S_API bool SteamAPI_ISteamUGC_SetAllowLegacyUpload(intptr_t instancePtr, UGCUpdateHandle_t handle, bool bAllowLegacyUpload)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetAllowLegacyUpload(handle, bAllowLegacyUpload);
}

S_API bool SteamAPI_ISteamUGC_RemoveItemKeyValueTags(intptr_t instancePtr, UGCUpdateHandle_t handle, const char * pchKey)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->RemoveItemKeyValueTags(handle, pchKey);
}

S_API bool SteamAPI_ISteamUGC_AddItemKeyValueTag(intptr_t instancePtr, UGCUpdateHandle_t handle, const char * pchKey, const char * pchValue)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->AddItemKeyValueTag(handle, pchKey, pchValue);
}

S_API bool SteamAPI_ISteamUGC_AddItemPreviewFile(intptr_t instancePtr, UGCUpdateHandle_t handle, const char * pszPreviewFile, EItemPreviewType type)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->AddItemPreviewFile(handle, pszPreviewFile, type);
}

S_API bool SteamAPI_ISteamUGC_AddItemPreviewVideo(intptr_t instancePtr, UGCUpdateHandle_t handle, const char * pszVideoID)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->AddItemPreviewVideo(handle, pszVideoID);
}

S_API bool SteamAPI_ISteamUGC_UpdateItemPreviewFile(intptr_t instancePtr, UGCUpdateHandle_t handle, uint32 index, const char * pszPreviewFile)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->UpdateItemPreviewFile(handle, index, pszPreviewFile);
}

S_API bool SteamAPI_ISteamUGC_UpdateItemPreviewVideo(intptr_t instancePtr, UGCUpdateHandle_t handle, uint32 index, const char * pszVideoID)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->UpdateItemPreviewVideo(handle, index, pszVideoID);
}

S_API bool SteamAPI_ISteamUGC_RemoveItemPreview(intptr_t instancePtr, UGCUpdateHandle_t handle, uint32 index)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->RemoveItemPreview(handle, index);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_SubmitItemUpdate(intptr_t instancePtr, UGCUpdateHandle_t handle, const char * pchChangeNote)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SubmitItemUpdate(handle, pchChangeNote);
}

S_API EItemUpdateStatus SteamAPI_ISteamUGC_GetItemUpdateProgress(intptr_t instancePtr, UGCUpdateHandle_t handle, uint64 * punBytesProcessed, uint64 * punBytesTotal)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->GetItemUpdateProgress(handle, punBytesProcessed, punBytesTotal);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_SetUserItemVote(intptr_t instancePtr, PublishedFileId_t nPublishedFileID, bool bVoteUp)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SetUserItemVote(nPublishedFileID, bVoteUp);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_GetUserItemVote(intptr_t instancePtr, PublishedFileId_t nPublishedFileID)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->GetUserItemVote(nPublishedFileID);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_AddItemToFavorites(intptr_t instancePtr, AppId_t nAppId, PublishedFileId_t nPublishedFileID)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->AddItemToFavorites(nAppId, nPublishedFileID);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_RemoveItemFromFavorites(intptr_t instancePtr, AppId_t nAppId, PublishedFileId_t nPublishedFileID)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->RemoveItemFromFavorites(nAppId, nPublishedFileID);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_SubscribeItem(intptr_t instancePtr, PublishedFileId_t nPublishedFileID)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SubscribeItem(nPublishedFileID);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_UnsubscribeItem(intptr_t instancePtr, PublishedFileId_t nPublishedFileID)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->UnsubscribeItem(nPublishedFileID);
}

S_API uint32 SteamAPI_ISteamUGC_GetNumSubscribedItems(intptr_t instancePtr)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->GetNumSubscribedItems();
}

S_API uint32 SteamAPI_ISteamUGC_GetSubscribedItems(intptr_t instancePtr, PublishedFileId_t * pvecPublishedFileID, uint32 cMaxEntries)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->GetSubscribedItems(pvecPublishedFileID, cMaxEntries);
}

S_API uint32 SteamAPI_ISteamUGC_GetItemState(intptr_t instancePtr, PublishedFileId_t nPublishedFileID)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->GetItemState(nPublishedFileID);
}

S_API bool SteamAPI_ISteamUGC_GetItemInstallInfo(intptr_t instancePtr, PublishedFileId_t nPublishedFileID, uint64 * punSizeOnDisk, char * pchFolder, uint32 cchFolderSize, uint32 * punTimeStamp)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->GetItemInstallInfo(nPublishedFileID, punSizeOnDisk, pchFolder, cchFolderSize, punTimeStamp);
}

S_API bool SteamAPI_ISteamUGC_GetItemDownloadInfo(intptr_t instancePtr, PublishedFileId_t nPublishedFileID, uint64 * punBytesDownloaded, uint64 * punBytesTotal)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->GetItemDownloadInfo(nPublishedFileID, punBytesDownloaded, punBytesTotal);
}

S_API bool SteamAPI_ISteamUGC_DownloadItem(intptr_t instancePtr, PublishedFileId_t nPublishedFileID, bool bHighPriority)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->DownloadItem(nPublishedFileID, bHighPriority);
}

S_API bool SteamAPI_ISteamUGC_BInitWorkshopForGameServer(intptr_t instancePtr, DepotId_t unWorkshopDepotID, const char * pszFolder)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->BInitWorkshopForGameServer(unWorkshopDepotID, pszFolder);
}

S_API void SteamAPI_ISteamUGC_SuspendDownloads(intptr_t instancePtr, bool bSuspend)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->SuspendDownloads(bSuspend);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_StartPlaytimeTracking(intptr_t instancePtr, PublishedFileId_t * pvecPublishedFileID, uint32 unNumPublishedFileIDs)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->StartPlaytimeTracking(pvecPublishedFileID, unNumPublishedFileIDs);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_StopPlaytimeTracking(intptr_t instancePtr, PublishedFileId_t * pvecPublishedFileID, uint32 unNumPublishedFileIDs)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->StopPlaytimeTracking(pvecPublishedFileID, unNumPublishedFileIDs);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_StopPlaytimeTrackingForAllItems(intptr_t instancePtr)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->StopPlaytimeTrackingForAllItems();
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_AddDependency(intptr_t instancePtr, PublishedFileId_t nParentPublishedFileID, PublishedFileId_t nChildPublishedFileID)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->AddDependency(nParentPublishedFileID, nChildPublishedFileID);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_RemoveDependency(intptr_t instancePtr, PublishedFileId_t nParentPublishedFileID, PublishedFileId_t nChildPublishedFileID)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->RemoveDependency(nParentPublishedFileID, nChildPublishedFileID);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_AddAppDependency(intptr_t instancePtr, PublishedFileId_t nPublishedFileID, AppId_t nAppID)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->AddAppDependency(nPublishedFileID, nAppID);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_RemoveAppDependency(intptr_t instancePtr, PublishedFileId_t nPublishedFileID, AppId_t nAppID)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->RemoveAppDependency(nPublishedFileID, nAppID);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_GetAppDependencies(intptr_t instancePtr, PublishedFileId_t nPublishedFileID)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->GetAppDependencies(nPublishedFileID);
}

S_API SteamAPICall_t SteamAPI_ISteamUGC_DeleteItem(intptr_t instancePtr, PublishedFileId_t nPublishedFileID)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_ugc);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_ugc);
    auto ptr = get_steam_client()->steam_gameserver_ugc;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_ugc;
    }

    return (ptr)->DeleteItem(nPublishedFileID);
}

S_API uint32 SteamAPI_ISteamAppList_GetNumInstalledApps(intptr_t instancePtr)
{
    return ((ISteamAppList *)instancePtr)->GetNumInstalledApps();
}

S_API uint32 SteamAPI_ISteamAppList_GetInstalledApps(intptr_t instancePtr, AppId_t * pvecAppID, uint32 unMaxAppIDs)
{
    return ((ISteamAppList *)instancePtr)->GetInstalledApps(pvecAppID, unMaxAppIDs);
}

S_API int SteamAPI_ISteamAppList_GetAppName(intptr_t instancePtr, AppId_t nAppID, char * pchName, int cchNameMax)
{
    return ((ISteamAppList *)instancePtr)->GetAppName(nAppID, pchName, cchNameMax);
}

S_API int SteamAPI_ISteamAppList_GetAppInstallDir(intptr_t instancePtr, AppId_t nAppID, char * pchDirectory, int cchNameMax)
{
    return ((ISteamAppList *)instancePtr)->GetAppInstallDir(nAppID, pchDirectory, cchNameMax);
}

S_API int SteamAPI_ISteamAppList_GetAppBuildId(intptr_t instancePtr, AppId_t nAppID)
{
    return ((ISteamAppList *)instancePtr)->GetAppBuildId(nAppID);
}

S_API void SteamAPI_ISteamHTMLSurface_DestructISteamHTMLSurface(intptr_t instancePtr)
{
    //return (get_steam_client()->steam_HTMLsurface)->DestructISteamHTMLSurface();
}

S_API bool SteamAPI_ISteamHTMLSurface_Init(intptr_t instancePtr)
{
    return (get_steam_client()->steam_HTMLsurface)->Init();
}

S_API bool SteamAPI_ISteamHTMLSurface_Shutdown(intptr_t instancePtr)
{
    return (get_steam_client()->steam_HTMLsurface)->Shutdown();
}

S_API SteamAPICall_t SteamAPI_ISteamHTMLSurface_CreateBrowser(intptr_t instancePtr, const char * pchUserAgent, const char * pchUserCSS)
{
    return (get_steam_client()->steam_HTMLsurface)->CreateBrowser(pchUserAgent, pchUserCSS);
}

S_API void SteamAPI_ISteamHTMLSurface_RemoveBrowser(intptr_t instancePtr, HHTMLBrowser unBrowserHandle)
{
    return (get_steam_client()->steam_HTMLsurface)->RemoveBrowser(unBrowserHandle);
}

S_API void SteamAPI_ISteamHTMLSurface_LoadURL(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, const char * pchURL, const char * pchPostData)
{
    return (get_steam_client()->steam_HTMLsurface)->LoadURL(unBrowserHandle, pchURL, pchPostData);
}

S_API void SteamAPI_ISteamHTMLSurface_SetSize(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, uint32 unWidth, uint32 unHeight)
{
    return (get_steam_client()->steam_HTMLsurface)->SetSize(unBrowserHandle, unWidth, unHeight);
}

S_API void SteamAPI_ISteamHTMLSurface_StopLoad(intptr_t instancePtr, HHTMLBrowser unBrowserHandle)
{
    return (get_steam_client()->steam_HTMLsurface)->StopLoad(unBrowserHandle);
}

S_API void SteamAPI_ISteamHTMLSurface_Reload(intptr_t instancePtr, HHTMLBrowser unBrowserHandle)
{
    return (get_steam_client()->steam_HTMLsurface)->Reload(unBrowserHandle);
}

S_API void SteamAPI_ISteamHTMLSurface_GoBack(intptr_t instancePtr, HHTMLBrowser unBrowserHandle)
{
    return (get_steam_client()->steam_HTMLsurface)->GoBack(unBrowserHandle);
}

S_API void SteamAPI_ISteamHTMLSurface_GoForward(intptr_t instancePtr, HHTMLBrowser unBrowserHandle)
{
    return (get_steam_client()->steam_HTMLsurface)->GoForward(unBrowserHandle);
}

S_API void SteamAPI_ISteamHTMLSurface_AddHeader(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, const char * pchKey, const char * pchValue)
{
    return (get_steam_client()->steam_HTMLsurface)->AddHeader(unBrowserHandle, pchKey, pchValue);
}

S_API void SteamAPI_ISteamHTMLSurface_ExecuteJavascript(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, const char * pchScript)
{
    return (get_steam_client()->steam_HTMLsurface)->ExecuteJavascript(unBrowserHandle, pchScript);
}

S_API void SteamAPI_ISteamHTMLSurface_MouseUp(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, EHTMLMouseButton eMouseButton)
{
    return (get_steam_client()->steam_HTMLsurface)->MouseUp(unBrowserHandle, eMouseButton);
}

S_API void SteamAPI_ISteamHTMLSurface_MouseDown(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, EHTMLMouseButton eMouseButton)
{
    return (get_steam_client()->steam_HTMLsurface)->MouseDown(unBrowserHandle, eMouseButton);
}

S_API void SteamAPI_ISteamHTMLSurface_MouseDoubleClick(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, EHTMLMouseButton eMouseButton)
{
    return (get_steam_client()->steam_HTMLsurface)->MouseDoubleClick(unBrowserHandle, eMouseButton);
}

S_API void SteamAPI_ISteamHTMLSurface_MouseMove(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, int x, int y)
{
    return (get_steam_client()->steam_HTMLsurface)->MouseMove(unBrowserHandle, x, y);
}

S_API void SteamAPI_ISteamHTMLSurface_MouseWheel(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, int32 nDelta)
{
    return (get_steam_client()->steam_HTMLsurface)->MouseWheel(unBrowserHandle, nDelta);
}

S_API void SteamAPI_ISteamHTMLSurface_KeyDown(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, uint32 nNativeKeyCode, EHTMLKeyModifiers eHTMLKeyModifiers, bool bIsSystemKey)
{
    return (get_steam_client()->steam_HTMLsurface)->KeyDown(unBrowserHandle, nNativeKeyCode, eHTMLKeyModifiers, bIsSystemKey);
}

S_API void SteamAPI_ISteamHTMLSurface_KeyUp(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, uint32 nNativeKeyCode, EHTMLKeyModifiers eHTMLKeyModifiers)
{
    return (get_steam_client()->steam_HTMLsurface)->KeyUp(unBrowserHandle, nNativeKeyCode, eHTMLKeyModifiers);
}

S_API void SteamAPI_ISteamHTMLSurface_KeyChar(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, uint32 cUnicodeChar, EHTMLKeyModifiers eHTMLKeyModifiers)
{
    return (get_steam_client()->steam_HTMLsurface)->KeyChar(unBrowserHandle, cUnicodeChar, eHTMLKeyModifiers);
}

S_API void SteamAPI_ISteamHTMLSurface_SetHorizontalScroll(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, uint32 nAbsolutePixelScroll)
{
    return (get_steam_client()->steam_HTMLsurface)->SetHorizontalScroll(unBrowserHandle, nAbsolutePixelScroll);
}

S_API void SteamAPI_ISteamHTMLSurface_SetVerticalScroll(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, uint32 nAbsolutePixelScroll)
{
    return (get_steam_client()->steam_HTMLsurface)->SetVerticalScroll(unBrowserHandle, nAbsolutePixelScroll);
}

S_API void SteamAPI_ISteamHTMLSurface_SetKeyFocus(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, bool bHasKeyFocus)
{
    return (get_steam_client()->steam_HTMLsurface)->SetKeyFocus(unBrowserHandle, bHasKeyFocus);
}

S_API void SteamAPI_ISteamHTMLSurface_ViewSource(intptr_t instancePtr, HHTMLBrowser unBrowserHandle)
{
    return (get_steam_client()->steam_HTMLsurface)->ViewSource(unBrowserHandle);
}

S_API void SteamAPI_ISteamHTMLSurface_CopyToClipboard(intptr_t instancePtr, HHTMLBrowser unBrowserHandle)
{
    return (get_steam_client()->steam_HTMLsurface)->CopyToClipboard(unBrowserHandle);
}

S_API void SteamAPI_ISteamHTMLSurface_PasteFromClipboard(intptr_t instancePtr, HHTMLBrowser unBrowserHandle)
{
    return (get_steam_client()->steam_HTMLsurface)->PasteFromClipboard(unBrowserHandle);
}

S_API void SteamAPI_ISteamHTMLSurface_Find(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, const char * pchSearchStr, bool bCurrentlyInFind, bool bReverse)
{
    return (get_steam_client()->steam_HTMLsurface)->Find(unBrowserHandle, pchSearchStr, bCurrentlyInFind, bReverse);
}

S_API void SteamAPI_ISteamHTMLSurface_StopFind(intptr_t instancePtr, HHTMLBrowser unBrowserHandle)
{
    return (get_steam_client()->steam_HTMLsurface)->StopFind(unBrowserHandle);
}

S_API void SteamAPI_ISteamHTMLSurface_GetLinkAtPosition(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, int x, int y)
{
    return (get_steam_client()->steam_HTMLsurface)->GetLinkAtPosition(unBrowserHandle, x, y);
}

S_API void SteamAPI_ISteamHTMLSurface_SetCookie(intptr_t instancePtr, const char * pchHostname, const char * pchKey, const char * pchValue, const char * pchPath, RTime32 nExpires, bool bSecure, bool bHTTPOnly)
{
    return (get_steam_client()->steam_HTMLsurface)->SetCookie(pchHostname, pchKey, pchValue, pchPath, nExpires, bSecure, bHTTPOnly);
}

S_API void SteamAPI_ISteamHTMLSurface_SetPageScaleFactor(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, float flZoom, int nPointX, int nPointY)
{
    return (get_steam_client()->steam_HTMLsurface)->SetPageScaleFactor(unBrowserHandle, flZoom, nPointX, nPointY);
}

S_API void SteamAPI_ISteamHTMLSurface_SetBackgroundMode(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, bool bBackgroundMode)
{
    return (get_steam_client()->steam_HTMLsurface)->SetBackgroundMode(unBrowserHandle, bBackgroundMode);
}

S_API void SteamAPI_ISteamHTMLSurface_SetDPIScalingFactor(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, float flDPIScaling)
{
    return (get_steam_client()->steam_HTMLsurface)->SetDPIScalingFactor(unBrowserHandle, flDPIScaling);
}

S_API void SteamAPI_ISteamHTMLSurface_OpenDeveloperTools(intptr_t instancePtr, HHTMLBrowser unBrowserHandle)
{
    return (get_steam_client()->steam_HTMLsurface)->OpenDeveloperTools(unBrowserHandle);
}

S_API void SteamAPI_ISteamHTMLSurface_AllowStartRequest(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, bool bAllowed)
{
    return (get_steam_client()->steam_HTMLsurface)->AllowStartRequest(unBrowserHandle, bAllowed);
}

S_API void SteamAPI_ISteamHTMLSurface_JSDialogResponse(intptr_t instancePtr, HHTMLBrowser unBrowserHandle, bool bResult)
{
    return (get_steam_client()->steam_HTMLsurface)->JSDialogResponse(unBrowserHandle, bResult);
}

S_API EResult SteamAPI_ISteamInventory_GetResultStatus(intptr_t instancePtr, SteamInventoryResult_t resultHandle)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->GetResultStatus(resultHandle);
}

S_API bool SteamAPI_ISteamInventory_GetResultItems(intptr_t instancePtr, SteamInventoryResult_t resultHandle, struct SteamItemDetails_t * pOutItemsArray, uint32 * punOutItemsArraySize)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->GetResultItems(resultHandle, pOutItemsArray, punOutItemsArraySize);
}

S_API bool SteamAPI_ISteamInventory_GetResultItemProperty(intptr_t instancePtr, SteamInventoryResult_t resultHandle, uint32 unItemIndex, const char * pchPropertyName, char * pchValueBuffer, uint32 * punValueBufferSizeOut)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->GetResultItemProperty(resultHandle, unItemIndex, pchPropertyName, pchValueBuffer, punValueBufferSizeOut);
}

S_API uint32 SteamAPI_ISteamInventory_GetResultTimestamp(intptr_t instancePtr, SteamInventoryResult_t resultHandle)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->GetResultTimestamp(resultHandle);
}

S_API bool SteamAPI_ISteamInventory_CheckResultSteamID(intptr_t instancePtr, SteamInventoryResult_t resultHandle, class CSteamID steamIDExpected)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->CheckResultSteamID(resultHandle, steamIDExpected);
}

S_API void SteamAPI_ISteamInventory_DestroyResult(intptr_t instancePtr, SteamInventoryResult_t resultHandle)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->DestroyResult(resultHandle);
}

S_API bool SteamAPI_ISteamInventory_GetAllItems(intptr_t instancePtr, SteamInventoryResult_t * pResultHandle)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->GetAllItems(pResultHandle);
}

S_API bool SteamAPI_ISteamInventory_GetItemsByID(intptr_t instancePtr, SteamInventoryResult_t * pResultHandle, const SteamItemInstanceID_t * pInstanceIDs, uint32 unCountInstanceIDs)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->GetItemsByID(pResultHandle, pInstanceIDs, unCountInstanceIDs);
}

S_API bool SteamAPI_ISteamInventory_SerializeResult(intptr_t instancePtr, SteamInventoryResult_t resultHandle, void * pOutBuffer, uint32 * punOutBufferSize)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->SerializeResult(resultHandle, pOutBuffer, punOutBufferSize);
}

S_API bool SteamAPI_ISteamInventory_DeserializeResult(intptr_t instancePtr, SteamInventoryResult_t * pOutResultHandle, const void * pBuffer, uint32 unBufferSize, bool bRESERVED_MUST_BE_FALSE)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->DeserializeResult(pOutResultHandle, pBuffer, unBufferSize, bRESERVED_MUST_BE_FALSE);
}

S_API bool SteamAPI_ISteamInventory_GenerateItems(intptr_t instancePtr, SteamInventoryResult_t * pResultHandle, const SteamItemDef_t * pArrayItemDefs, const uint32 * punArrayQuantity, uint32 unArrayLength)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->GenerateItems(pResultHandle, pArrayItemDefs, punArrayQuantity, unArrayLength);
}

S_API bool SteamAPI_ISteamInventory_GrantPromoItems(intptr_t instancePtr, SteamInventoryResult_t * pResultHandle)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->GrantPromoItems(pResultHandle);
}

S_API bool SteamAPI_ISteamInventory_AddPromoItem(intptr_t instancePtr, SteamInventoryResult_t * pResultHandle, SteamItemDef_t itemDef)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->AddPromoItem(pResultHandle, itemDef);
}

S_API bool SteamAPI_ISteamInventory_AddPromoItems(intptr_t instancePtr, SteamInventoryResult_t * pResultHandle, const SteamItemDef_t * pArrayItemDefs, uint32 unArrayLength)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->AddPromoItems(pResultHandle, pArrayItemDefs, unArrayLength);
}

S_API bool SteamAPI_ISteamInventory_ConsumeItem(intptr_t instancePtr, SteamInventoryResult_t * pResultHandle, SteamItemInstanceID_t itemConsume, uint32 unQuantity)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->ConsumeItem(pResultHandle, itemConsume, unQuantity);
}

S_API bool SteamAPI_ISteamInventory_ExchangeItems(intptr_t instancePtr, SteamInventoryResult_t * pResultHandle, const SteamItemDef_t * pArrayGenerate, const uint32 * punArrayGenerateQuantity, uint32 unArrayGenerateLength, const SteamItemInstanceID_t * pArrayDestroy, const uint32 * punArrayDestroyQuantity, uint32 unArrayDestroyLength)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->ExchangeItems(pResultHandle, pArrayGenerate, punArrayGenerateQuantity, unArrayGenerateLength, pArrayDestroy, punArrayDestroyQuantity, unArrayDestroyLength);
}

S_API bool SteamAPI_ISteamInventory_TransferItemQuantity(intptr_t instancePtr, SteamInventoryResult_t * pResultHandle, SteamItemInstanceID_t itemIdSource, uint32 unQuantity, SteamItemInstanceID_t itemIdDest)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->TransferItemQuantity(pResultHandle, itemIdSource, unQuantity, itemIdDest);
}

S_API void SteamAPI_ISteamInventory_SendItemDropHeartbeat(intptr_t instancePtr)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->SendItemDropHeartbeat();
}

S_API bool SteamAPI_ISteamInventory_TriggerItemDrop(intptr_t instancePtr, SteamInventoryResult_t * pResultHandle, SteamItemDef_t dropListDefinition)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->TriggerItemDrop(pResultHandle, dropListDefinition);
}

S_API bool SteamAPI_ISteamInventory_TradeItems(intptr_t instancePtr, SteamInventoryResult_t * pResultHandle, class CSteamID steamIDTradePartner, const SteamItemInstanceID_t * pArrayGive, const uint32 * pArrayGiveQuantity, uint32 nArrayGiveLength, const SteamItemInstanceID_t * pArrayGet, const uint32 * pArrayGetQuantity, uint32 nArrayGetLength)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->TradeItems(pResultHandle, steamIDTradePartner, pArrayGive, pArrayGiveQuantity, nArrayGiveLength, pArrayGet, pArrayGetQuantity, nArrayGetLength);
}

S_API bool SteamAPI_ISteamInventory_LoadItemDefinitions(intptr_t instancePtr)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->LoadItemDefinitions();
}

S_API bool SteamAPI_ISteamInventory_GetItemDefinitionIDs(intptr_t instancePtr, SteamItemDef_t * pItemDefIDs, uint32 * punItemDefIDsArraySize)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->GetItemDefinitionIDs(pItemDefIDs, punItemDefIDsArraySize);
}

S_API bool SteamAPI_ISteamInventory_GetItemDefinitionProperty(intptr_t instancePtr, SteamItemDef_t iDefinition, const char * pchPropertyName, char * pchValueBuffer, uint32 * punValueBufferSizeOut)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->GetItemDefinitionProperty(iDefinition, pchPropertyName, pchValueBuffer, punValueBufferSizeOut);
}

S_API SteamAPICall_t SteamAPI_ISteamInventory_RequestEligiblePromoItemDefinitionsIDs(intptr_t instancePtr, class CSteamID steamID)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->RequestEligiblePromoItemDefinitionsIDs(steamID);
}

S_API bool SteamAPI_ISteamInventory_GetEligiblePromoItemDefinitionIDs(intptr_t instancePtr, class CSteamID steamID, SteamItemDef_t * pItemDefIDs, uint32 * punItemDefIDsArraySize)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->GetEligiblePromoItemDefinitionIDs(steamID, pItemDefIDs, punItemDefIDsArraySize);
}

S_API SteamAPICall_t SteamAPI_ISteamInventory_StartPurchase(intptr_t instancePtr, const SteamItemDef_t * pArrayItemDefs, const uint32 * punArrayQuantity, uint32 unArrayLength)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->StartPurchase(pArrayItemDefs, punArrayQuantity, unArrayLength);
}

S_API SteamAPICall_t SteamAPI_ISteamInventory_RequestPrices(intptr_t instancePtr)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->RequestPrices();
}

S_API uint32 SteamAPI_ISteamInventory_GetNumItemsWithPrices(intptr_t instancePtr)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->GetNumItemsWithPrices();
}

S_API bool SteamAPI_ISteamInventory_GetItemsWithPrices(intptr_t instancePtr, SteamItemDef_t * pArrayItemDefs, uint64 * pCurrentPrices, uint64 * pBasePrices, uint32 unArrayLength)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->GetItemsWithPrices(pArrayItemDefs, pCurrentPrices, pBasePrices, unArrayLength);
}

S_API bool SteamAPI_ISteamInventory_GetItemPrice(intptr_t instancePtr, SteamItemDef_t iDefinition, uint64 * pCurrentPrice, uint64 * pBasePrice)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->GetItemPrice(iDefinition, pCurrentPrice, pBasePrice);
}

S_API SteamInventoryUpdateHandle_t SteamAPI_ISteamInventory_StartUpdateProperties(intptr_t instancePtr)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->StartUpdateProperties();
}

S_API bool SteamAPI_ISteamInventory_RemoveProperty(intptr_t instancePtr, SteamInventoryUpdateHandle_t handle, SteamItemInstanceID_t nItemID, const char * pchPropertyName)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->RemoveProperty(handle, nItemID, pchPropertyName);
}

S_API bool SteamAPI_ISteamInventory_SetProperty(intptr_t instancePtr, SteamInventoryUpdateHandle_t handle, SteamItemInstanceID_t nItemID, const char * pchPropertyName, const char * pchPropertyValue)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->SetProperty(handle, nItemID, pchPropertyName, pchPropertyValue);
}

S_API bool SteamAPI_ISteamInventory_SetProperty0(intptr_t instancePtr, SteamInventoryUpdateHandle_t handle, SteamItemInstanceID_t nItemID, const char * pchPropertyName, bool bValue)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->SetProperty(handle, nItemID, pchPropertyName, bValue);
}

S_API bool SteamAPI_ISteamInventory_SetProperty1(intptr_t instancePtr, SteamInventoryUpdateHandle_t handle, SteamItemInstanceID_t nItemID, const char * pchPropertyName, int64 nValue)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->SetProperty(handle, nItemID, pchPropertyName, nValue);
}

S_API bool SteamAPI_ISteamInventory_SetProperty2(intptr_t instancePtr, SteamInventoryUpdateHandle_t handle, SteamItemInstanceID_t nItemID, const char * pchPropertyName, float flValue)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->SetProperty(handle, nItemID, pchPropertyName, flValue);
}

S_API bool SteamAPI_ISteamInventory_SubmitUpdateProperties(intptr_t instancePtr, SteamInventoryUpdateHandle_t handle, SteamInventoryResult_t * pResultHandle)
{
    int test1 = ((char *)instancePtr - (char*)get_steam_client()->steam_inventory);
    int test2 = ((char *)instancePtr - (char*)get_steam_client()->steam_gameserver_inventory);
    auto ptr = get_steam_client()->steam_gameserver_inventory;
    if (test1 >= 0 && (test2 < 0 || test1 < test2)) {
        ptr = get_steam_client()->steam_inventory;
    }

    return (ptr)->SubmitUpdateProperties(handle, pResultHandle);
}

S_API void SteamAPI_ISteamVideo_GetVideoURL(intptr_t instancePtr, AppId_t unVideoAppID)
{
    return ((ISteamVideo *)instancePtr)->GetVideoURL(unVideoAppID);
}

S_API bool SteamAPI_ISteamVideo_IsBroadcasting(intptr_t instancePtr, int * pnNumViewers)
{
    return ((ISteamVideo *)instancePtr)->IsBroadcasting(pnNumViewers);
}

S_API void SteamAPI_ISteamVideo_GetOPFSettings(intptr_t instancePtr, AppId_t unVideoAppID)
{
    return ((ISteamVideo *)instancePtr)->GetOPFSettings(unVideoAppID);
}

S_API bool SteamAPI_ISteamVideo_GetOPFStringForApp(intptr_t instancePtr, AppId_t unVideoAppID, char * pchBuffer, int32 * pnBufferSize)
{
    return ((ISteamVideo *)instancePtr)->GetOPFStringForApp(unVideoAppID, pchBuffer, pnBufferSize);
}

S_API bool SteamAPI_ISteamParentalSettings_BIsParentalLockEnabled(intptr_t instancePtr)
{
    return ((ISteamParentalSettings *)instancePtr)->BIsParentalLockEnabled();
}

S_API bool SteamAPI_ISteamParentalSettings_BIsParentalLockLocked(intptr_t instancePtr)
{
    return ((ISteamParentalSettings *)instancePtr)->BIsParentalLockLocked();
}

S_API bool SteamAPI_ISteamParentalSettings_BIsAppBlocked(intptr_t instancePtr, AppId_t nAppID)
{
    return ((ISteamParentalSettings *)instancePtr)->BIsAppBlocked(nAppID);
}

S_API bool SteamAPI_ISteamParentalSettings_BIsAppInBlockList(intptr_t instancePtr, AppId_t nAppID)
{
    return ((ISteamParentalSettings *)instancePtr)->BIsAppInBlockList(nAppID);
}

S_API bool SteamAPI_ISteamParentalSettings_BIsFeatureBlocked(intptr_t instancePtr, EParentalFeature eFeature)
{
    return ((ISteamParentalSettings *)instancePtr)->BIsFeatureBlocked(eFeature);
}

S_API bool SteamAPI_ISteamParentalSettings_BIsFeatureInBlockList(intptr_t instancePtr, EParentalFeature eFeature)
{
    return ((ISteamParentalSettings *)instancePtr)->BIsFeatureInBlockList(eFeature);
}

S_API bool SteamAPI_ISteamGameServer_InitGameServer(intptr_t instancePtr, uint32 unIP, uint16 usGamePort, uint16 usQueryPort, uint32 unFlags, AppId_t nGameAppId, const char * pchVersionString)
{
    return ((ISteamGameServer *)instancePtr)->InitGameServer(unIP, usGamePort, usQueryPort, unFlags, nGameAppId, pchVersionString);
}

S_API void SteamAPI_ISteamGameServer_SetProduct(intptr_t instancePtr, const char * pszProduct)
{
    return ((ISteamGameServer *)instancePtr)->SetProduct(pszProduct);
}

S_API void SteamAPI_ISteamGameServer_SetGameDescription(intptr_t instancePtr, const char * pszGameDescription)
{
    return ((ISteamGameServer *)instancePtr)->SetGameDescription(pszGameDescription);
}

S_API void SteamAPI_ISteamGameServer_SetModDir(intptr_t instancePtr, const char * pszModDir)
{
    return ((ISteamGameServer *)instancePtr)->SetModDir(pszModDir);
}

S_API void SteamAPI_ISteamGameServer_SetDedicatedServer(intptr_t instancePtr, bool bDedicated)
{
    return ((ISteamGameServer *)instancePtr)->SetDedicatedServer(bDedicated);
}

S_API void SteamAPI_ISteamGameServer_LogOn(intptr_t instancePtr, const char * pszToken)
{
    return ((ISteamGameServer *)instancePtr)->LogOn(pszToken);
}

S_API void SteamAPI_ISteamGameServer_LogOnAnonymous(intptr_t instancePtr)
{
    return ((ISteamGameServer *)instancePtr)->LogOnAnonymous();
}

S_API void SteamAPI_ISteamGameServer_LogOff(intptr_t instancePtr)
{
    return ((ISteamGameServer *)instancePtr)->LogOff();
}

S_API bool SteamAPI_ISteamGameServer_BLoggedOn(intptr_t instancePtr)
{
    return ((ISteamGameServer *)instancePtr)->BLoggedOn();
}

S_API bool SteamAPI_ISteamGameServer_BSecure(intptr_t instancePtr)
{
    return ((ISteamGameServer *)instancePtr)->BSecure();
}

S_API uint64 SteamAPI_ISteamGameServer_GetSteamID(intptr_t instancePtr)
{
    return ((ISteamGameServer *)instancePtr)->GetSteamID().ConvertToUint64();
}

S_API bool SteamAPI_ISteamGameServer_WasRestartRequested(intptr_t instancePtr)
{
    return ((ISteamGameServer *)instancePtr)->WasRestartRequested();
}

S_API void SteamAPI_ISteamGameServer_SetMaxPlayerCount(intptr_t instancePtr, int cPlayersMax)
{
    return ((ISteamGameServer *)instancePtr)->SetMaxPlayerCount(cPlayersMax);
}

S_API void SteamAPI_ISteamGameServer_SetBotPlayerCount(intptr_t instancePtr, int cBotplayers)
{
    return ((ISteamGameServer *)instancePtr)->SetBotPlayerCount(cBotplayers);
}

S_API void SteamAPI_ISteamGameServer_SetServerName(intptr_t instancePtr, const char * pszServerName)
{
    return ((ISteamGameServer *)instancePtr)->SetServerName(pszServerName);
}

S_API void SteamAPI_ISteamGameServer_SetMapName(intptr_t instancePtr, const char * pszMapName)
{
    return ((ISteamGameServer *)instancePtr)->SetMapName(pszMapName);
}

S_API void SteamAPI_ISteamGameServer_SetPasswordProtected(intptr_t instancePtr, bool bPasswordProtected)
{
    return ((ISteamGameServer *)instancePtr)->SetPasswordProtected(bPasswordProtected);
}

S_API void SteamAPI_ISteamGameServer_SetSpectatorPort(intptr_t instancePtr, uint16 unSpectatorPort)
{
    return ((ISteamGameServer *)instancePtr)->SetSpectatorPort(unSpectatorPort);
}

S_API void SteamAPI_ISteamGameServer_SetSpectatorServerName(intptr_t instancePtr, const char * pszSpectatorServerName)
{
    return ((ISteamGameServer *)instancePtr)->SetSpectatorServerName(pszSpectatorServerName);
}

S_API void SteamAPI_ISteamGameServer_ClearAllKeyValues(intptr_t instancePtr)
{
    return ((ISteamGameServer *)instancePtr)->ClearAllKeyValues();
}

S_API void SteamAPI_ISteamGameServer_SetKeyValue(intptr_t instancePtr, const char * pKey, const char * pValue)
{
    return ((ISteamGameServer *)instancePtr)->SetKeyValue(pKey, pValue);
}

S_API void SteamAPI_ISteamGameServer_SetGameTags(intptr_t instancePtr, const char * pchGameTags)
{
    return ((ISteamGameServer *)instancePtr)->SetGameTags(pchGameTags);
}

S_API void SteamAPI_ISteamGameServer_SetGameData(intptr_t instancePtr, const char * pchGameData)
{
    return ((ISteamGameServer *)instancePtr)->SetGameData(pchGameData);
}

S_API void SteamAPI_ISteamGameServer_SetRegion(intptr_t instancePtr, const char * pszRegion)
{
    return ((ISteamGameServer *)instancePtr)->SetRegion(pszRegion);
}

S_API bool SteamAPI_ISteamGameServer_SendUserConnectAndAuthenticate(intptr_t instancePtr, uint32 unIPClient, const void * pvAuthBlob, uint32 cubAuthBlobSize, class CSteamID * pSteamIDUser)
{
    return ((ISteamGameServer *)instancePtr)->SendUserConnectAndAuthenticate(unIPClient, pvAuthBlob, cubAuthBlobSize, pSteamIDUser);
}

S_API uint64 SteamAPI_ISteamGameServer_CreateUnauthenticatedUserConnection(intptr_t instancePtr)
{
    return ((ISteamGameServer *)instancePtr)->CreateUnauthenticatedUserConnection().ConvertToUint64();
}

S_API void SteamAPI_ISteamGameServer_SendUserDisconnect(intptr_t instancePtr, class CSteamID steamIDUser)
{
    return ((ISteamGameServer *)instancePtr)->SendUserDisconnect(steamIDUser);
}

S_API bool SteamAPI_ISteamGameServer_BUpdateUserData(intptr_t instancePtr, class CSteamID steamIDUser, const char * pchPlayerName, uint32 uScore)
{
    return ((ISteamGameServer *)instancePtr)->BUpdateUserData(steamIDUser, pchPlayerName, uScore);
}

S_API HAuthTicket SteamAPI_ISteamGameServer_GetAuthSessionTicket(intptr_t instancePtr, void * pTicket, int cbMaxTicket, uint32 * pcbTicket)
{
    return ((ISteamGameServer *)instancePtr)->GetAuthSessionTicket(pTicket, cbMaxTicket, pcbTicket);
}

S_API EBeginAuthSessionResult SteamAPI_ISteamGameServer_BeginAuthSession(intptr_t instancePtr, const void * pAuthTicket, int cbAuthTicket, class CSteamID steamID)
{
    return ((ISteamGameServer *)instancePtr)->BeginAuthSession(pAuthTicket, cbAuthTicket, steamID);
}

S_API void SteamAPI_ISteamGameServer_EndAuthSession(intptr_t instancePtr, class CSteamID steamID)
{
    return ((ISteamGameServer *)instancePtr)->EndAuthSession(steamID);
}

S_API void SteamAPI_ISteamGameServer_CancelAuthTicket(intptr_t instancePtr, HAuthTicket hAuthTicket)
{
    return ((ISteamGameServer *)instancePtr)->CancelAuthTicket(hAuthTicket);
}

S_API EUserHasLicenseForAppResult SteamAPI_ISteamGameServer_UserHasLicenseForApp(intptr_t instancePtr, class CSteamID steamID, AppId_t appID)
{
    return ((ISteamGameServer *)instancePtr)->UserHasLicenseForApp(steamID, appID);
}

S_API bool SteamAPI_ISteamGameServer_RequestUserGroupStatus(intptr_t instancePtr, class CSteamID steamIDUser, class CSteamID steamIDGroup)
{
    return ((ISteamGameServer *)instancePtr)->RequestUserGroupStatus(steamIDUser, steamIDGroup);
}

S_API void SteamAPI_ISteamGameServer_GetGameplayStats(intptr_t instancePtr)
{
    return ((ISteamGameServer *)instancePtr)->GetGameplayStats();
}

S_API SteamAPICall_t SteamAPI_ISteamGameServer_GetServerReputation(intptr_t instancePtr)
{
    return ((ISteamGameServer *)instancePtr)->GetServerReputation();
}

S_API uint32 SteamAPI_ISteamGameServer_GetPublicIP(intptr_t instancePtr)
{
    return ((ISteamGameServer *)instancePtr)->GetPublicIP();
}

S_API bool SteamAPI_ISteamGameServer_HandleIncomingPacket(intptr_t instancePtr, const void * pData, int cbData, uint32 srcIP, uint16 srcPort)
{
    return ((ISteamGameServer *)instancePtr)->HandleIncomingPacket(pData, cbData, srcIP, srcPort);
}

S_API int SteamAPI_ISteamGameServer_GetNextOutgoingPacket(intptr_t instancePtr, void * pOut, int cbMaxOut, uint32 * pNetAdr, uint16 * pPort)
{
    return ((ISteamGameServer *)instancePtr)->GetNextOutgoingPacket(pOut, cbMaxOut, pNetAdr, pPort);
}

S_API void SteamAPI_ISteamGameServer_EnableHeartbeats(intptr_t instancePtr, bool bActive)
{
    return ((ISteamGameServer *)instancePtr)->EnableHeartbeats(bActive);
}

S_API void SteamAPI_ISteamGameServer_SetHeartbeatInterval(intptr_t instancePtr, int iHeartbeatInterval)
{
    return ((ISteamGameServer *)instancePtr)->SetHeartbeatInterval(iHeartbeatInterval);
}

S_API void SteamAPI_ISteamGameServer_ForceHeartbeat(intptr_t instancePtr)
{
    return ((ISteamGameServer *)instancePtr)->ForceHeartbeat();
}

S_API SteamAPICall_t SteamAPI_ISteamGameServer_AssociateWithClan(intptr_t instancePtr, class CSteamID steamIDClan)
{
    return ((ISteamGameServer *)instancePtr)->AssociateWithClan(steamIDClan);
}

S_API SteamAPICall_t SteamAPI_ISteamGameServer_ComputeNewPlayerCompatibility(intptr_t instancePtr, class CSteamID steamIDNewPlayer)
{
    return ((ISteamGameServer *)instancePtr)->ComputeNewPlayerCompatibility(steamIDNewPlayer);
}

S_API SteamAPICall_t SteamAPI_ISteamGameServerStats_RequestUserStats(intptr_t instancePtr, class CSteamID steamIDUser)
{
    return ((ISteamGameServerStats *)instancePtr)->RequestUserStats(steamIDUser);
}

S_API bool SteamAPI_ISteamGameServerStats_GetUserStat(intptr_t instancePtr, class CSteamID steamIDUser, const char * pchName, int32 * pData)
{
    return ((ISteamGameServerStats *)instancePtr)->GetUserStat(steamIDUser, pchName, pData);
}

S_API bool SteamAPI_ISteamGameServerStats_GetUserStat0(intptr_t instancePtr, class CSteamID steamIDUser, const char * pchName, float * pData)
{
    return ((ISteamGameServerStats *)instancePtr)->GetUserStat(steamIDUser, pchName, pData);
}

S_API bool SteamAPI_ISteamGameServerStats_GetUserAchievement(intptr_t instancePtr, class CSteamID steamIDUser, const char * pchName, bool * pbAchieved)
{
    return ((ISteamGameServerStats *)instancePtr)->GetUserAchievement(steamIDUser, pchName, pbAchieved);
}

S_API bool SteamAPI_ISteamGameServerStats_SetUserStat(intptr_t instancePtr, class CSteamID steamIDUser, const char * pchName, int32 nData)
{
    return ((ISteamGameServerStats *)instancePtr)->SetUserStat(steamIDUser, pchName, nData);
}

S_API bool SteamAPI_ISteamGameServerStats_SetUserStat0(intptr_t instancePtr, class CSteamID steamIDUser, const char * pchName, float fData)
{
    return ((ISteamGameServerStats *)instancePtr)->SetUserStat(steamIDUser, pchName, fData);
}

S_API bool SteamAPI_ISteamGameServerStats_UpdateUserAvgRateStat(intptr_t instancePtr, class CSteamID steamIDUser, const char * pchName, float flCountThisSession, double dSessionLength)
{
    return ((ISteamGameServerStats *)instancePtr)->UpdateUserAvgRateStat(steamIDUser, pchName, flCountThisSession, dSessionLength);
}

S_API bool SteamAPI_ISteamGameServerStats_SetUserAchievement(intptr_t instancePtr, class CSteamID steamIDUser, const char * pchName)
{
    return ((ISteamGameServerStats *)instancePtr)->SetUserAchievement(steamIDUser, pchName);
}

S_API bool SteamAPI_ISteamGameServerStats_ClearUserAchievement(intptr_t instancePtr, class CSteamID steamIDUser, const char * pchName)
{
    return ((ISteamGameServerStats *)instancePtr)->ClearUserAchievement(steamIDUser, pchName);
}

S_API SteamAPICall_t SteamAPI_ISteamGameServerStats_StoreUserStats(intptr_t instancePtr, class CSteamID steamIDUser)
{
    return ((ISteamGameServerStats *)instancePtr)->StoreUserStats(steamIDUser);
}
