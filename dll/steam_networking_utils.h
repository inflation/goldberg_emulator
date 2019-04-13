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

class Steam_Networking_Utils :
public ISteamNetworkingUtils
{
    class Settings *settings;
    class Networking *network;
    class SteamCallResults *callback_results;
    class SteamCallBacks *callbacks;
    class RunEveryRunCB *run_every_runcb;
    std::chrono::time_point<std::chrono::steady_clock> initialized_time = std::chrono::steady_clock::now();
    FSteamNetworkingSocketsDebugOutput debug_function;

public:
static void steam_callback(void *object, Common_Message *msg)
{
    PRINT_DEBUG("steam_networkingutils_callback\n");

    Steam_Networking_Utils *steam_networkingutils = (Steam_Networking_Utils *)object;
    steam_networkingutils->Callback(msg);
}

static void steam_run_every_runcb(void *object)
{
    PRINT_DEBUG("steam_networkingutils_run_every_runcb\n");

    Steam_Networking_Utils *steam_networkingutils = (Steam_Networking_Utils *)object;
    steam_networkingutils->RunCallbacks();
}

Steam_Networking_Utils(class Settings *settings, class Networking *network, class SteamCallResults *callback_results, class SteamCallBacks *callbacks, class RunEveryRunCB *run_every_runcb)
{
    this->settings = settings;
    this->network = network;
    this->run_every_runcb = run_every_runcb;
    //this->network->setCallback(CALLBACK_ID_USER_STATUS, settings->get_local_steam_id(), &Steam_Networking_Utils::steam_callback, this);
    this->run_every_runcb->add(&Steam_Networking_Utils::steam_run_every_runcb, this);

    this->callback_results = callback_results;
    this->callbacks = callbacks;
}

~Steam_Networking_Utils()
{
    //TODO rm network callbacks
    this->run_every_runcb->remove(&Steam_Networking_Utils::steam_run_every_runcb, this);
}

bool InitializeRelayAccess()
{
    PRINT_DEBUG("Steam_Networking_Utils::InitializeRelayAccess\n");
    return true;
}

float GetLocalPingLocation( SteamNetworkPingLocation_t &result )
{
    PRINT_DEBUG("Steam_Networking_Utils::GetLocalPingLocation\n");
    return -1;
}

int EstimatePingTimeBetweenTwoLocations( const SteamNetworkPingLocation_t &location1, const SteamNetworkPingLocation_t &location2 )
{
    PRINT_DEBUG("Steam_Networking_Utils::EstimatePingTimeBetweenTwoLocations\n");
    return k_nSteamNetworkingPing_Unknown ;
}


int EstimatePingTimeFromLocalHost( const SteamNetworkPingLocation_t &remoteLocation )
{
    PRINT_DEBUG("Steam_Networking_Utils::EstimatePingTimeFromLocalHost\n");
}


void ConvertPingLocationToString( const SteamNetworkPingLocation_t &location, char *pszBuf, int cchBufSize )
{
    PRINT_DEBUG("Steam_Networking_Utils::ConvertPingLocationToString\n");
}


bool ParsePingLocationString( const char *pszString, SteamNetworkPingLocation_t &result )
{
    PRINT_DEBUG("Steam_Networking_Utils::ParsePingLocationString\n");
}


bool CheckPingDataUpToDate( float flMaxAgeSeconds )
{
    PRINT_DEBUG("Steam_Networking_Utils::CheckPingDataUpToDate %f\n", flMaxAgeSeconds);
    return true;
}


bool IsPingMeasurementInProgress()
{
    PRINT_DEBUG("Steam_Networking_Utils::IsPingMeasurementInProgress\n");
}


int GetPingToDataCenter( SteamNetworkingPOPID popID, SteamNetworkingPOPID *pViaRelayPoP )
{
    PRINT_DEBUG("Steam_Networking_Utils::GetPingToDataCenter\n");
}


int GetDirectPingToPOP( SteamNetworkingPOPID popID )
{
    PRINT_DEBUG("Steam_Networking_Utils::GetDirectPingToPOP\n");
}


int GetPOPCount()
{
    PRINT_DEBUG("Steam_Networking_Utils::GetPOPCount\n");
}


int GetPOPList( SteamNetworkingPOPID *list, int nListSz )
{
    PRINT_DEBUG("Steam_Networking_Utils::GetPOPList\n");
}


//
// Misc
//

/// Fetch current timestamp.  This timer has the following properties:
///
/// - Monotonicity is guaranteed.
/// - The initial value will be at least 24*3600*30*1e6, i.e. about
///   30 days worth of microseconds.  In this way, the timestamp value of
///   0 will always be at least "30 days ago".  Also, negative numbers
///   will never be returned.
/// - Wraparound / overflow is not a practical concern.
///
/// If you are running under the debugger and stop the process, the clock
/// might not advance the full wall clock time that has elapsed between
/// calls.  If the process is not blocked from normal operation, the
/// timestamp values will track wall clock time, even if you don't call
/// the function frequently.
///
/// The value is only meaningful for this run of the process.  Don't compare
/// it to values obtained on another computer, or other runs of the same process.
SteamNetworkingMicroseconds GetLocalTimestamp()
{
    PRINT_DEBUG("Steam_Networking_Utils::GetLocalTimestamp\n");
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - initialized_time).count() + (SteamNetworkingMicroseconds)24*3600*30*1e6;
}


/// Set a function to receive network-related information that is useful for debugging.
/// This can be very useful during development, but it can also be useful for troubleshooting
/// problems with tech savvy end users.  If you have a console or other log that customers
/// can examine, these log messages can often be helpful to troubleshoot network issues.
/// (Especially any warning/error messages.)
///
/// The detail level indicates what message to invoke your callback on.  Lower numeric
/// value means more important, and the value you pass is the lowest priority (highest
/// numeric value) you wish to receive callbacks for.
///
/// Except when debugging, you should only use k_ESteamNetworkingSocketsDebugOutputType_Msg
/// or k_ESteamNetworkingSocketsDebugOutputType_Warning.  For best performance, do NOT
/// request a high detail level and then filter out messages in your callback.  Instead,
/// call function function to adjust the desired level of detail.
///
/// IMPORTANT: This may be called from a service thread, while we own a mutex, etc.
/// Your output function must be threadsafe and fast!  Do not make any other
/// Steamworks calls from within the handler.
void SetDebugOutputFunction( ESteamNetworkingSocketsDebugOutputType eDetailLevel, FSteamNetworkingSocketsDebugOutput pfnFunc )
{
    PRINT_DEBUG("Steam_Networking_Utils::SetDebugOutputFunction %i\n", eDetailLevel);
    if (eDetailLevel != k_ESteamNetworkingSocketsDebugOutputType_None) {
        debug_function = pfnFunc;
    }
}


//
// Set and get configuration values, see ESteamNetworkingConfigValue for individual descriptions.
//

// Shortcuts for common cases.  (Implemented as inline functions below)
/*
bool SetGlobalConfigValueInt32( ESteamNetworkingConfigValue eValue, int32 val );
bool SetGlobalConfigValueFloat( ESteamNetworkingConfigValue eValue, float val );
bool SetGlobalConfigValueString( ESteamNetworkingConfigValue eValue, const char *val );
bool SetConnectionConfigValueInt32( HSteamNetConnection hConn, ESteamNetworkingConfigValue eValue, int32 val );
bool SetConnectionConfigValueFloat( HSteamNetConnection hConn, ESteamNetworkingConfigValue eValue, float val );
bool SetConnectionConfigValueString( HSteamNetConnection hConn, ESteamNetworkingConfigValue eValue, const char *val );
*/
/// Set a configuration value.
/// - eValue: which value is being set
/// - eScope: Onto what type of object are you applying the setting?
/// - scopeArg: Which object you want to change?  (Ignored for global scope).  E.g. connection handle, listen socket handle, interface pointer, etc.
/// - eDataType: What type of data is in the buffer at pValue?  This must match the type of the variable exactly!
/// - pArg: Value to set it to.  You can pass NULL to remove a non-global sett at this scope,
///   causing the value for that object to use global defaults.  Or at global scope, passing NULL
///   will reset any custom value and restore it to the system default.
///   NOTE: When setting callback functions, do not pass the function pointer directly.
///   Your argument should be a pointer to a function pointer.
bool SetConfigValue( ESteamNetworkingConfigValue eValue, ESteamNetworkingConfigScope eScopeType, intptr_t scopeObj,
    ESteamNetworkingConfigDataType eDataType, const void *pArg )
{
    PRINT_DEBUG("Steam_Networking_Utils::SetConfigValue\n");
}


/// Get a configuration value.
/// - eValue: which value to fetch
/// - eScopeType: query setting on what type of object
/// - eScopeArg: the object to query the setting for
/// - pOutDataType: If non-NULL, the data type of the value is returned.
/// - pResult: Where to put the result.  Pass NULL to query the required buffer size.  (k_ESteamNetworkingGetConfigValue_BufferTooSmall will be returned.)
/// - cbResult: IN: the size of your buffer.  OUT: the number of bytes filled in or required.
ESteamNetworkingGetConfigValueResult GetConfigValue( ESteamNetworkingConfigValue eValue, ESteamNetworkingConfigScope eScopeType, intptr_t scopeObj,
    ESteamNetworkingConfigDataType *pOutDataType, void *pResult, size_t *cbResult )
{
    PRINT_DEBUG("Steam_Networking_Utils::GetConfigValue\n");
}


/// Returns info about a configuration value.  Returns false if the value does not exist.
/// pOutNextValue can be used to iterate through all of the known configuration values.
/// (Use GetFirstConfigValue() to begin the iteration, will be k_ESteamNetworkingConfig_Invalid on the last value)
/// Any of the output parameters can be NULL if you do not need that information.
bool GetConfigValueInfo( ESteamNetworkingConfigValue eValue, const char **pOutName, ESteamNetworkingConfigDataType *pOutDataType, ESteamNetworkingConfigScope *pOutScope, ESteamNetworkingConfigValue *pOutNextValue )
{
    PRINT_DEBUG("Steam_Networking_Utils::GetConfigValueInfo\n");
}


/// Return the lowest numbered configuration value available in the current environment.
ESteamNetworkingConfigValue GetFirstConfigValue()
{
    PRINT_DEBUG("Steam_Networking_Utils::GetFirstConfigValue\n");
}


// String conversions.  You'll usually access these using the respective
// inline methods.
void SteamNetworkingIPAddr_ToString( const SteamNetworkingIPAddr &addr, char *buf, size_t cbBuf, bool bWithPort )
{
    PRINT_DEBUG("Steam_Networking_Utils::SteamNetworkingIPAddr_ToString\n");
}

bool SteamNetworkingIPAddr_ParseString( SteamNetworkingIPAddr *pAddr, const char *pszStr )
{
    PRINT_DEBUG("Steam_Networking_Utils::SteamNetworkingIPAddr_ParseString\n");
}

void SteamNetworkingIdentity_ToString( const SteamNetworkingIdentity &identity, char *buf, size_t cbBuf )
{
    PRINT_DEBUG("Steam_Networking_Utils::SteamNetworkingIdentity_ToString\n");
}

bool SteamNetworkingIdentity_ParseString( SteamNetworkingIdentity *pIdentity, const char *pszStr )
{
    PRINT_DEBUG("Steam_Networking_Utils::SteamNetworkingIdentity_ParseString\n");
}


void RunCallbacks()
{
}

void Callback(Common_Message *msg)
{
    if (msg->has_low_level()) {
        if (msg->low_level().type() == Low_Level::CONNECT) {
            
        }

        if (msg->low_level().type() == Low_Level::DISCONNECT) {

        }
    }

    if (msg->has_networking_sockets()) {

    }
}


};
