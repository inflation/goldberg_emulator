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

#include "base.h"
#include "local_storage.h"

static std::chrono::time_point<std::chrono::steady_clock> app_initialized_time = std::chrono::steady_clock::now();


class Steam_Utils : 
public ISteamUtils002,
public ISteamUtils003,
public ISteamUtils004,
public ISteamUtils005,
public ISteamUtils006,
public ISteamUtils007,
public ISteamUtils008,
public ISteamUtils
{
private:
    Settings *settings;
    class SteamCallResults *callback_results;

public:
Steam_Utils(Settings *settings, class SteamCallResults *callback_results)
{
    this->settings = settings;
    this->callback_results = callback_results;
}

// return the number of seconds since the user 
uint32 GetSecondsSinceAppActive()
{
    PRINT_DEBUG("GetSecondsSinceAppActive\n");
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - app_initialized_time).count();
}

uint32 GetSecondsSinceComputerActive()
{
    PRINT_DEBUG("GetSecondsSinceComputerActive\n");
    return GetSecondsSinceAppActive() + 2000;
}


// the universe this client is connecting to
EUniverse GetConnectedUniverse()
{
    PRINT_DEBUG("GetConnectedUniverse\n");
    return k_EUniversePublic;
}


// Steam server time.  Number of seconds since January 1, 1970, GMT (i.e unix time)
uint32 GetServerRealTime()
{
    PRINT_DEBUG("GetServerRealTime\n");
    uint32 server_time = std::chrono::duration_cast<std::chrono::duration<uint32>>(std::chrono::system_clock::now().time_since_epoch()).count();
    PRINT_DEBUG("Time %lu\n", server_time);
    return server_time;
}


// returns the 2 digit ISO 3166-1-alpha-2 format country code this client is running in (as looked up via an IP-to-location database)
// e.g "US" or "UK".
const char *GetIPCountry()
{
    PRINT_DEBUG("GetIPCountry\n");
    return "US";
}

static uint32 width_image(int iImage)
{
    if ((iImage % 3) == 1) return 32;
    if ((iImage % 3) == 2) return 64;
    return 184;
}

// returns true if the image exists, and valid sizes were filled out
bool GetImageSize( int iImage, uint32 *pnWidth, uint32 *pnHeight )
{
    PRINT_DEBUG("GetImageSize\n");
    if (!iImage || !pnWidth || !pnHeight) return false;

    *pnWidth = width_image(iImage);
    *pnHeight = width_image(iImage);;
    return true;
}


// returns true if the image exists, and the buffer was successfully filled out
// results are returned in RGBA format
// the destination buffer size should be 4 * height * width * sizeof(char)
bool GetImageRGBA( int iImage, uint8 *pubDest, int nDestBufferSize )
{
    PRINT_DEBUG("GetImageRGBA\n");
    if (!iImage || !pubDest || !nDestBufferSize) return false;
    unsigned size = width_image(iImage) * width_image(iImage) * 4;
    if (nDestBufferSize < size) size = nDestBufferSize;
    memset(pubDest, 0xFF, size);
    return true;
}


// returns the IP of the reporting server for valve - currently only used in Source engine games
bool GetCSERIPPort( uint32 *unIP, uint16 *usPort )
{
    PRINT_DEBUG("GetCSERIPPort\n");
    return false;
}


// return the amount of battery power left in the current system in % [0..100], 255 for being on AC power
uint8 GetCurrentBatteryPower()
{
    PRINT_DEBUG("GetCurrentBatteryPower\n");
    return 255;
}


// returns the appID of the current process
uint32 GetAppID()
{
    PRINT_DEBUG("GetAppID\n");
    return settings->get_local_game_id().AppID();
}


// Sets the position where the overlay instance for the currently calling game should show notifications.
// This position is per-game and if this function is called from outside of a game context it will do nothing.
void SetOverlayNotificationPosition( ENotificationPosition eNotificationPosition )
{
    PRINT_DEBUG("SetOverlayNotificationPosition\n");
}


// API asynchronous call results
// can be used directly, but more commonly used via the callback dispatch API (see steam_api.h)
bool IsAPICallCompleted( SteamAPICall_t hSteamAPICall, bool *pbFailed )
{
    PRINT_DEBUG("IsAPICallCompleted: %llu\n", hSteamAPICall);
    if (hSteamAPICall == 1) { //bug ? soul calibur 6 calls this function with the return value 1 of Steam_User_Stats::RequestCurrentStats and expects this function to return true
        if (pbFailed) *pbFailed = true;
        return true;
    }

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    if (!callback_results->exists(hSteamAPICall)) return false;
    if (pbFailed) *pbFailed = false;
    return true; //all api calls "complete" right away
}

ESteamAPICallFailure GetAPICallFailureReason( SteamAPICall_t hSteamAPICall )
{
    PRINT_DEBUG("GetAPICallFailureReason\n");
    return k_ESteamAPICallFailureNone;
}

bool GetAPICallResult( SteamAPICall_t hSteamAPICall, void *pCallback, int cubCallback, int iCallbackExpected, bool *pbFailed )
{
    PRINT_DEBUG("GetAPICallResult %llu %i %i\n", hSteamAPICall, cubCallback, iCallbackExpected);
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    if (callback_results->callback_result(hSteamAPICall, pCallback, cubCallback)) {
        *pbFailed = false;
        PRINT_DEBUG("GetAPICallResult Succeeded\n");
        return true;
    } else {
        return false;
    }
}


// Deprecated. Applications should use SteamAPI_RunCallbacks() instead. Game servers do not need to call this function.
STEAM_PRIVATE_API( void RunFrame()
{
    PRINT_DEBUG("RunFrame\n");
}
 )

// returns the number of IPC calls made since the last time this function was called
// Used for perf debugging so you can understand how many IPC calls your game makes per frame
// Every IPC call is at minimum a thread context switch if not a process one so you want to rate
// control how often you do them.
uint32 GetIPCCallCount()
{
    PRINT_DEBUG("GetIPCCallCount\n");
    static int i = 0;
    i += 123;
    return i; //TODO
}


// API warning handling
// 'int' is the severity; 0 for msg, 1 for warning
// 'const char *' is the text of the message
// callbacks will occur directly after the API function is called that generated the warning or message
void SetWarningMessageHook( SteamAPIWarningMessageHook_t pFunction )
{
    PRINT_DEBUG("Steam_Utils::SetWarningMessageHook\n");
}


// Returns true if the overlay is running & the user can access it. The overlay process could take a few seconds to
// start & hook the game process, so this function will initially return false while the overlay is loading.
bool IsOverlayEnabled()
{
    PRINT_DEBUG("IsOverlayEnabled\n");
    //TODO
    return false;
}


// Normally this call is unneeded if your game has a constantly running frame loop that calls the 
// D3D Present API, or OGL SwapBuffers API every frame.
//
// However, if you have a game that only refreshes the screen on an event driven basis then that can break 
// the overlay, as it uses your Present/SwapBuffers calls to drive it's internal frame loop and it may also
// need to Present() to the screen any time an even needing a notification happens or when the overlay is
// brought up over the game by a user.  You can use this API to ask the overlay if it currently need a present
// in that case, and then you can check for this periodically (roughly 33hz is desirable) and make sure you
// refresh the screen with Present or SwapBuffers to allow the overlay to do it's work.
bool BOverlayNeedsPresent()
{
    PRINT_DEBUG("BOverlayNeedsPresent\n");
    return false;
}


// Asynchronous call to check if an executable file has been signed using the public key set on the signing tab
// of the partner site, for example to refuse to load modified executable files.  
// The result is returned in CheckFileSignature_t.
//   k_ECheckFileSignatureNoSignaturesFoundForThisApp - This app has not been configured on the signing tab of the partner site to enable this function.
//   k_ECheckFileSignatureNoSignaturesFoundForThisFile - This file is not listed on the signing tab for the partner site.
//   k_ECheckFileSignatureFileNotFound - The file does not exist on disk.
//   k_ECheckFileSignatureInvalidSignature - The file exists, and the signing tab has been set for this file, but the file is either not signed or the signature does not match.
//   k_ECheckFileSignatureValidSignature - The file is signed and the signature is valid.
STEAM_CALL_RESULT( CheckFileSignature_t )
SteamAPICall_t CheckFileSignature( const char *szFileName )
{
    PRINT_DEBUG("CheckFileSignature\n");
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    CheckFileSignature_t data;
    data.m_eCheckFileSignature = k_ECheckFileSignatureValidSignature;
    return callback_results->addCallResult(data.k_iCallback, &data, sizeof(data));
}


// Activates the Big Picture text input dialog which only supports gamepad input
bool ShowGamepadTextInput( EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, const char *pchDescription, uint32 unCharMax, const char *pchExistingText )
{
    PRINT_DEBUG("ShowGamepadTextInput\n");
    return false;
}

bool ShowGamepadTextInput( EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, const char *pchDescription, uint32 unCharMax )
{
	PRINT_DEBUG("ShowGamepadTextInput old\n");
	return ShowGamepadTextInput(eInputMode, eLineInputMode, pchDescription, unCharMax, NULL);
}

// Returns previously entered text & length
uint32 GetEnteredGamepadTextLength()
{
    PRINT_DEBUG("GetEnteredGamepadTextLength\n");
    return 0;
}

bool GetEnteredGamepadTextInput( char *pchText, uint32 cchText )
{
    PRINT_DEBUG("GetEnteredGamepadTextInput\n");
    return false;
}


// returns the language the steam client is running in, you probably want ISteamApps::GetCurrentGameLanguage instead, this is for very special usage cases
const char *GetSteamUILanguage()
{
    PRINT_DEBUG("GetSteamUILanguage\n");
    return settings->get_language();
}


// returns true if Steam itself is running in VR mode
bool IsSteamRunningInVR()
{
    PRINT_DEBUG("IsSteamRunningInVR\n");
    return false;
}


// Sets the inset of the overlay notification from the corner specified by SetOverlayNotificationPosition.
void SetOverlayNotificationInset( int nHorizontalInset, int nVerticalInset )
{
    PRINT_DEBUG("SetOverlayNotificationInset\n");
}


// returns true if Steam & the Steam Overlay are running in Big Picture mode
// Games much be launched through the Steam client to enable the Big Picture overlay. During development,
// a game can be added as a non-steam game to the developers library to test this feature
bool IsSteamInBigPictureMode()
{
    PRINT_DEBUG("IsSteamInBigPictureMode\n");
    return false;
}


// ask SteamUI to create and render its OpenVR dashboard
void StartVRDashboard()
{
    PRINT_DEBUG("StartVRDashboard\n");
}


// Returns true if the HMD content will be streamed via Steam In-Home Streaming
bool IsVRHeadsetStreamingEnabled()
{
    PRINT_DEBUG("IsVRHeadsetStreamingEnabled\n");
    return false;
}


// Set whether the HMD content will be streamed via Steam In-Home Streaming
// If this is set to true, then the scene in the HMD headset will be streamed, and remote input will not be allowed.
// If this is set to false, then the application window will be streamed instead, and remote input will be allowed.
// The default is true unless "VRHeadsetStreaming" "0" is in the extended appinfo for a game.
// (this is useful for games that have asymmetric multiplayer gameplay)
void SetVRHeadsetStreamingEnabled( bool bEnabled )
{
    PRINT_DEBUG("SetVRHeadsetStreamingEnabled\n");
}
};
