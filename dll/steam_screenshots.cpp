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

#include "steam_screenshots.h" 

// Writes a screenshot to the user's screenshot library given the raw image data, which must be in RGB format.
// The return value is a handle that is valid for the duration of the game process and can be used to apply tags.
ScreenshotHandle Steam_Screenshots::WriteScreenshot( void *pubRGB, uint32 cubRGB, int nWidth, int nHeight )
{
    PRINT_DEBUG("WriteScreenshot\n");
    return INVALID_SCREENSHOT_HANDLE;
}


// Adds a screenshot to the user's screenshot library from disk.  If a thumbnail is provided, it must be 200 pixels wide and the same aspect ratio
// as the screenshot, otherwise a thumbnail will be generated if the user uploads the screenshot.  The screenshots must be in either JPEG or TGA format.
// The return value is a handle that is valid for the duration of the game process and can be used to apply tags.
// JPEG, TGA, and PNG formats are supported.
ScreenshotHandle Steam_Screenshots::AddScreenshotToLibrary( const char *pchFilename, const char *pchThumbnailFilename, int nWidth, int nHeight )
{
    PRINT_DEBUG("AddScreenshotToLibrary\n");
    return INVALID_SCREENSHOT_HANDLE;
}


// Causes the Steam overlay to take a screenshot.  If screenshots are being hooked by the game then a ScreenshotRequested_t callback is sent back to the game instead. 
void Steam_Screenshots::TriggerScreenshot()
{
    PRINT_DEBUG("TriggerScreenshot\n");
}


// Toggles whether the overlay handles screenshots when the user presses the screenshot hotkey, or the game handles them.  If the game is hooking screenshots,
// then the ScreenshotRequested_t callback will be sent if the user presses the hotkey, and the game is expected to call WriteScreenshot or AddScreenshotToLibrary
// in response.
void Steam_Screenshots::HookScreenshots( bool bHook )
{
    PRINT_DEBUG("HookScreenshots\n");
    hooked = bHook;
}


// Sets metadata about a screenshot's location (for example, the name of the map)
bool Steam_Screenshots::SetLocation( ScreenshotHandle hScreenshot, const char *pchLocation )
{
    PRINT_DEBUG("SetLocation\n");
    return false;
}


// Tags a user as being visible in the screenshot
bool Steam_Screenshots::TagUser( ScreenshotHandle hScreenshot, CSteamID steamID )
{
    PRINT_DEBUG("TagUser\n");
    return false;
}


// Tags a published file as being visible in the screenshot
bool Steam_Screenshots::TagPublishedFile( ScreenshotHandle hScreenshot, PublishedFileId_t unPublishedFileID )
{
    PRINT_DEBUG("TagPublishedFile\n");
    return false;
}


// Returns true if the app has hooked the screenshot
bool Steam_Screenshots::IsScreenshotsHooked()
{
    PRINT_DEBUG("IsScreenshotsHooked\n");
    return hooked;
}


// Adds a VR screenshot to the user's screenshot library from disk in the supported type.
// pchFilename should be the normal 2D image used in the library view
// pchVRFilename should contain the image that matches the correct type
// The return value is a handle that is valid for the duration of the game process and can be used to apply tags.
// JPEG, TGA, and PNG formats are supported.
ScreenshotHandle Steam_Screenshots::AddVRScreenshotToLibrary( EVRScreenshotType eType, const char *pchFilename, const char *pchVRFilename )
{
    PRINT_DEBUG("AddVRScreenshotToLibrary\n");
    return INVALID_SCREENSHOT_HANDLE;
}

