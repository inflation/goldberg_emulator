====Goldberg Steam Emulator====

An emulator that supports LAN multiplayer without steam.

How to use: Replace the steam_api(64).dll (libsteam_api.so on linux) from the game with mine. (For linux make sure to use the right build).
Put a steam_appid.txt file that contains the appid of the game right beside it if there isn't one already.


If your game has an original steam_api(64).dll older than may 2016 (Properties->Digital Signatures->Timestamp) you might have to add a steam_interfaces.txt beside my emulator library if the game isn't working. 
This file contains interface versions, an example (for resident evil 5) is provided. To get those you can just get a hex editor and search for them (Search for the string: SteamUser0) in the original steam_api dll (or libsteam_api.so on linux) or look in the ini of a crack that works on that game.


If the game has DRM (other than steamworks) you need to remove/crack it first.


Default save location: C:\Users\<Your windows user name>\AppData\Roaming\Goldberg SteamEmu Saves\
For linux: $HOME/Goldberg SteamEmu Saves/

In the settings folder in that save location you will find 3 files (if you have used the emulator at least once):
account_name.txt (Edit this file to change your name)
listen_port.txt (Edit this file if you want to change the UDP/TCP port the emulator listens on (You should probably not change this because everyone needs to use the same port or you won't find yourselves on the network))
user_steam_id.txt (this is where your steam id is saved, you can change it (if your saves for a game are locked to a specific steam id see below for a way to change it on a per game basis) but it has to be valid)
language.txt (Edit this to change the language the emulator will report to the game, default is english, it must be a valid steam language name or the game might have weird behaviour (list provided at the end of this readme))

Note that these are global so you won't have to change them for each game. For game unique stuff (stats and remote storage) a folder is created with the appid of the game.
If you want to change your steam_id on a per game basis, simply create a settings folder in the game unique directory (Full path: C:\Users\<Your windows user name>\AppData\Roaming\Goldberg SteamEmu Saves\<appid>\settings)
In that settings folder create a user_steam_id.txt file that contains the valid steam id that you want to use for that game only.

If for some reason you want it to save in the game directory you can create a file named local_save.txt right beside steam_api(64).dll (libsteam_api.so on linux)
The only thing that file should contain is the name of the save directory. This can be useful if you want to use different global settings like a different account name or steam id for a particular game.
Note that this save directory will be beside where the emu dll (or .so) is which may not be the same as the game path.

DLC:
By default the emulator will try to unlock all DLCs (by returning true when the game calls the BIsDlcInstalled function). If the game uses the other function you will need to
provide a list of DLCs to my emulator. To do this first create a steam_settings folder right beside where you put my emulator. 
In this folder, put a DLC.txt file. (path will be <path where my emu is>\steam_settings\DLC.txt)
If the DLC file is present, the emulator will only unlock the DLCs in that file. If the file is empty all DLCs will be locked.
The contents of this file are: appid=DLC name
See the steam_settings.EXAMPLE folder for an example.

Mods:
Put your mods in the steam_settings\mods\ folder. The steam_settings folder must be placed right beside my emulator dll.
Mod folders must be a number corresponding to the file id of the mod.
See the steam_settings.EXAMPLE folder for an example.

Steam appid:
The steam_appid.txt can be put in the steam_settings folder if for some reason you can't put it beside the steam api dll.
The steam appid can also be set using the SteamAppId or SteamGameId env variables (this is how real steam tells games what their appid is).


Support for CPY steam_api(64).dll cracks: See the build in the experimental folder.

Notes:
You must all be on the same LAN for it to work. This is an early work so a lot of games will likely not work.

IMPORTANT:
Do not run more than one steam game with the same appid at the same time on the same computer with my emu or there might be network issues (dedicated servers should be fine though).




List of valid steam languages:
arabic
bulgarian
schinese
tchinese
czech
danish
dutch
english
finnish
french
german
greek
hungarian
italian
japanese
koreana
norwegian
polish
portuguese
brazilian
romanian
russian
spanish
swedish
thai
turkish
ukrainian
