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

class Steam_Controller :
public ISteamController001,
public ISteamController003,
public ISteamController004,
public ISteamController005,
public ISteamController006,
public ISteamController,
public ISteamInput
{
public:
	
// Init and Shutdown must be called when starting/ending use of this interface
bool Init()
{
    PRINT_DEBUG("Steam_Controller::Init()\n");
    return true;
}

bool Init( const char *pchAbsolutePathToControllerConfigVDF )
{
    PRINT_DEBUG("Steam_Controller::Init() old\n");
    return Init();
}

bool Shutdown()
{
    PRINT_DEBUG("Steam_Controller::Shutdown()\n");
    return true;
}

void SetOverrideMode( const char *pchMode )
{
    PRINT_DEBUG("Steam_Controller::SetOverrideMode\n");
}

// Synchronize API state with the latest Steam Controller inputs available. This
// is performed automatically by SteamAPI_RunCallbacks, but for the absolute lowest
// possible latency, you call this directly before reading controller state.
void RunFrame()
{
    PRINT_DEBUG("Steam_Controller::RunFrame()\n");
}

bool GetControllerState( uint32 unControllerIndex, SteamControllerState001_t *pState )
{
    PRINT_DEBUG("Steam_Controller::GetControllerState()\n");
    return false;
}

// Enumerate currently connected controllers
// handlesOut should point to a STEAM_CONTROLLER_MAX_COUNT sized array of ControllerHandle_t handles
// Returns the number of handles written to handlesOut
int GetConnectedControllers( ControllerHandle_t *handlesOut )
{
    PRINT_DEBUG("GetConnectedControllers\n");
    return 0;
}


// Invokes the Steam overlay and brings up the binding screen
// Returns false is overlay is disabled / unavailable, or the user is not in Big Picture mode
bool ShowBindingPanel( ControllerHandle_t controllerHandle )
{
    PRINT_DEBUG("ShowBindingPanel\n");
    return false;
}


// ACTION SETS
// Lookup the handle for an Action Set. Best to do this once on startup, and store the handles for all future API calls.
ControllerActionSetHandle_t GetActionSetHandle( const char *pszActionSetName )
{
    PRINT_DEBUG("GetActionSetHandle %s\n", pszActionSetName);
    return 124;
}


// Reconfigure the controller to use the specified action set (ie 'Menu', 'Walk' or 'Drive')
// This is cheap, and can be safely called repeatedly. It's often easier to repeatedly call it in
// your state loops, instead of trying to place it in all of your state transitions.
void ActivateActionSet( ControllerHandle_t controllerHandle, ControllerActionSetHandle_t actionSetHandle )
{
    PRINT_DEBUG("ActivateActionSet\n");
}

ControllerActionSetHandle_t GetCurrentActionSet( ControllerHandle_t controllerHandle )
{
    PRINT_DEBUG("GetCurrentActionSet\n");
    return 124;
}


void ActivateActionSetLayer( ControllerHandle_t controllerHandle, ControllerActionSetHandle_t actionSetLayerHandle )
{
    PRINT_DEBUG("ActivateActionSetLayer\n");
}

void DeactivateActionSetLayer( ControllerHandle_t controllerHandle, ControllerActionSetHandle_t actionSetLayerHandle )
{
    PRINT_DEBUG("DeactivateActionSetLayer\n");
}

void DeactivateAllActionSetLayers( ControllerHandle_t controllerHandle )
{
    PRINT_DEBUG("DeactivateAllActionSetLayers\n");
}

int GetActiveActionSetLayers( ControllerHandle_t controllerHandle, ControllerActionSetHandle_t *handlesOut )
{
    PRINT_DEBUG("GetActiveActionSetLayers\n");
    return 0;
}



// ACTIONS
// Lookup the handle for a digital action. Best to do this once on startup, and store the handles for all future API calls.
ControllerDigitalActionHandle_t GetDigitalActionHandle( const char *pszActionName )
{
    PRINT_DEBUG("GetDigitalActionHandle %s\n", pszActionName);
    return 123;
}


// Returns the current state of the supplied digital game action
ControllerDigitalActionData_t GetDigitalActionData( ControllerHandle_t controllerHandle, ControllerDigitalActionHandle_t digitalActionHandle )
{
    PRINT_DEBUG("GetDigitalActionData\n");
    ControllerDigitalActionData_t digitalData;
    digitalData.bActive = false;
    return digitalData;
}


// Get the origin(s) for a digital action within an action set. Returns the number of origins supplied in originsOut. Use this to display the appropriate on-screen prompt for the action.
// originsOut should point to a STEAM_CONTROLLER_MAX_ORIGINS sized array of EControllerActionOrigin handles
int GetDigitalActionOrigins( ControllerHandle_t controllerHandle, ControllerActionSetHandle_t actionSetHandle, ControllerDigitalActionHandle_t digitalActionHandle, EControllerActionOrigin *originsOut )
{
    PRINT_DEBUG("GetDigitalActionOrigins\n");
    return 0;
}

int GetDigitalActionOrigins( InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle, InputDigitalActionHandle_t digitalActionHandle, EInputActionOrigin *originsOut )
{
    PRINT_DEBUG("GetDigitalActionOrigins steaminput\n");
    return 0;
}

// Lookup the handle for an analog action. Best to do this once on startup, and store the handles for all future API calls.
ControllerAnalogActionHandle_t GetAnalogActionHandle( const char *pszActionName )
{
    PRINT_DEBUG("GetAnalogActionHandle %s\n", pszActionName);
    return 125;
}


// Returns the current state of these supplied analog game action
ControllerAnalogActionData_t GetAnalogActionData( ControllerHandle_t controllerHandle, ControllerAnalogActionHandle_t analogActionHandle )
{
    PRINT_DEBUG("GetAnalogActionData\n");
    ControllerAnalogActionData_t data;
    data.eMode = k_EInputSourceMode_None;
    data.x = data.y = 0;
    data.bActive = false;
    return data;
}


// Get the origin(s) for an analog action within an action set. Returns the number of origins supplied in originsOut. Use this to display the appropriate on-screen prompt for the action.
// originsOut should point to a STEAM_CONTROLLER_MAX_ORIGINS sized array of EControllerActionOrigin handles
int GetAnalogActionOrigins( ControllerHandle_t controllerHandle, ControllerActionSetHandle_t actionSetHandle, ControllerAnalogActionHandle_t analogActionHandle, EControllerActionOrigin *originsOut )
{
    PRINT_DEBUG("GetAnalogActionOrigins\n");
    return 0;
}

int GetAnalogActionOrigins( InputHandle_t inputHandle, InputActionSetHandle_t actionSetHandle, InputAnalogActionHandle_t analogActionHandle, EInputActionOrigin *originsOut )
{
    PRINT_DEBUG("GetAnalogActionOrigins steaminput\n");
    return 0;
}

    
void StopAnalogActionMomentum( ControllerHandle_t controllerHandle, ControllerAnalogActionHandle_t eAction )
{
    PRINT_DEBUG("StopAnalogActionMomentum\n");
}


// Trigger a haptic pulse on a controller
void TriggerHapticPulse( ControllerHandle_t controllerHandle, ESteamControllerPad eTargetPad, unsigned short usDurationMicroSec )
{
    PRINT_DEBUG("TriggerHapticPulse\n");
}

void TriggerHapticPulse( uint32 unControllerIndex, ESteamControllerPad eTargetPad, unsigned short usDurationMicroSec )
{
    PRINT_DEBUG("TriggerHapticPulse old\n");
    TriggerHapticPulse(unControllerIndex, eTargetPad, usDurationMicroSec );
}

// Trigger a pulse with a duty cycle of usDurationMicroSec / usOffMicroSec, unRepeat times.
// nFlags is currently unused and reserved for future use.
void TriggerRepeatedHapticPulse( ControllerHandle_t controllerHandle, ESteamControllerPad eTargetPad, unsigned short usDurationMicroSec, unsigned short usOffMicroSec, unsigned short unRepeat, unsigned int nFlags )
{
    PRINT_DEBUG("TriggerRepeatedHapticPulse\n");
}


// Tigger a vibration event on supported controllers.  
void TriggerVibration( ControllerHandle_t controllerHandle, unsigned short usLeftSpeed, unsigned short usRightSpeed )
{
    PRINT_DEBUG("TriggerVibration\n");
}


// Set the controller LED color on supported controllers.  
void SetLEDColor( ControllerHandle_t controllerHandle, uint8 nColorR, uint8 nColorG, uint8 nColorB, unsigned int nFlags )
{
    PRINT_DEBUG("SetLEDColor\n");
}


// Returns the associated gamepad index for the specified controller, if emulating a gamepad
int GetGamepadIndexForController( ControllerHandle_t ulControllerHandle )
{
    PRINT_DEBUG("GetGamepadIndexForController\n");
    return 0;
}


// Returns the associated controller handle for the specified emulated gamepad
ControllerHandle_t GetControllerForGamepadIndex( int nIndex )
{
    PRINT_DEBUG("GetControllerForGamepadIndex\n");
    return 0;
}


// Returns raw motion data from the specified controller
ControllerMotionData_t GetMotionData( ControllerHandle_t controllerHandle )
{
    PRINT_DEBUG("GetMotionData\n");
    ControllerMotionData_t data = {};
    return data;
}


// Attempt to display origins of given action in the controller HUD, for the currently active action set
// Returns false is overlay is disabled / unavailable, or the user is not in Big Picture mode
bool ShowDigitalActionOrigins( ControllerHandle_t controllerHandle, ControllerDigitalActionHandle_t digitalActionHandle, float flScale, float flXPosition, float flYPosition )
{
    PRINT_DEBUG("ShowDigitalActionOrigins\n");
    return true;
}

bool ShowAnalogActionOrigins( ControllerHandle_t controllerHandle, ControllerAnalogActionHandle_t analogActionHandle, float flScale, float flXPosition, float flYPosition )
{
    PRINT_DEBUG("ShowAnalogActionOrigins\n");
    return true;
}


// Returns a localized string (from Steam's language setting) for the specified origin
const char *GetStringForActionOrigin( EControllerActionOrigin eOrigin )
{
    PRINT_DEBUG("GetStringForActionOrigin\n");
    return "Button String";
}

const char *GetStringForActionOrigin( EInputActionOrigin eOrigin )
{
    PRINT_DEBUG("GetStringForActionOrigin steaminput\n");
    return "Button String";
}


// Get a local path to art for on-screen glyph for a particular origin 
const char *GetGlyphForActionOrigin( EControllerActionOrigin eOrigin )
{
    PRINT_DEBUG("GetGlyphForActionOrigin\n");
    return "";
}

const char *GetGlyphForActionOrigin( EInputActionOrigin eOrigin )
{
    PRINT_DEBUG("GetGlyphForActionOrigin steaminput\n");
    return "";
}

// Returns the input type for a particular handle
ESteamInputType GetInputTypeForHandle( ControllerHandle_t controllerHandle )
{
    PRINT_DEBUG("GetInputTypeForHandle\n");
    return k_ESteamInputType_Unknown;
}

const char *GetStringForXboxOrigin( EXboxOrigin eOrigin )
{
    PRINT_DEBUG("GetStringForXboxOrigin\n");
    return "";
}

const char *GetGlyphForXboxOrigin( EXboxOrigin eOrigin )
{
    PRINT_DEBUG("GetGlyphForXboxOrigin\n");
    return "";
}

EControllerActionOrigin GetActionOriginFromXboxOrigin_( ControllerHandle_t controllerHandle, EXboxOrigin eOrigin )
{
    PRINT_DEBUG("GetActionOriginFromXboxOrigin\n");
    return k_EControllerActionOrigin_None;
}

EInputActionOrigin GetActionOriginFromXboxOrigin( InputHandle_t inputHandle, EXboxOrigin eOrigin )
{
    PRINT_DEBUG("GetActionOriginFromXboxOrigin steaminput\n");
    return k_EInputActionOrigin_None;
}

EControllerActionOrigin TranslateActionOrigin( ESteamInputType eDestinationInputType, EControllerActionOrigin eSourceOrigin )
{
    PRINT_DEBUG("TranslateActionOrigin\n");
    return k_EControllerActionOrigin_None;
}

EInputActionOrigin TranslateActionOrigin( ESteamInputType eDestinationInputType, EInputActionOrigin eSourceOrigin )
{
    PRINT_DEBUG("TranslateActionOrigin steaminput\n");
    return k_EInputActionOrigin_None;
}


};
