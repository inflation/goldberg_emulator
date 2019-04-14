# Goldberg Steam Emulator

This is a steam emulator that emulates steam online features on a LAN. It works on both Linux and Windows. For a readme on how to use it see: [The Release Readme](Readme_release.txt)

You replace the steam api .dll or .so with mine (for complete steps see the [Release Readme](Readme_release.txt)) and then you can put steam in the trash and play your games either in single player on on LAN without steam (Assuming the games have no DRM and use Steam for online).

If you are a game developper and made the mistake of depending too much on the steam api and want to release of version of your game without it and don't want to rewrite your game, this is for you. It is licenced LGPLv3+ so the only source code you need to publish is the source code of this emulator (and only if you make modification to it).

## How to use

Replace the steam_api(64).dll (libsteam_api.so on linux) from the game with mine. For linux make sure that if the original api is 32 bit you use a 32 bit build and if it's 64 bit you use a 64 bit build.

Put a steam_appid.txt file that contains the appid of the game right beside it if there isn't one already.

If your game has an original steam_api(64).dll or libsteam_api.so older than may 2016 (On windows: Properties->Digital Signatures->Timestamp) you might have to add a steam_interfaces.txt beside my emulator library if the game isn't working. There is a linux script to generate it in the scripts folder of this repo.

For more information see: [The Release Readme](Readme_release.txt)

## Download Binaries

You can download the latest binaries for Linux and Windows in the [release section](https://gitlab.com/Mr_Goldberg/goldberg_emulator/releases) of this repo.

## Contributions

One of the reasons I made this code open source is because I want contributions. Unless your code is related to the experimental stuff it needs to work on both Linux and Windows. Having accurate behavior is more important than making games work. Having inaccurate behavior might fix one game but it will break others.

## IRC Channel

[#goldberg_emulator@freenode](https://webchat.freenode.net/?channels=goldberg_emulator)

## Building

Dependencies: protobuf-lite

#### Linux
Install protobuf-lite (the dev package) and protoc (or protobuf-compiler or whatever it's called in your distro) using your package manager.

Then do: `make`

And it will build the release build (Don't forget to to add something like `-j8` if your computer isn't a piece of shit and you want it to build at a decent speed).

To build the debug build: `make debug`

My makefile sucks so you might need to do: `make clean` if you want to build the debug build after building the release build or the opposite.

For my release build I build it on steamOS using the `build_steamos.sh` script. For it to work you need a x86 version of protobuf installed to: `../protobuf/prefix_x86/` and a x64 version installed to: `../protobuf/prefix/`

#### Windows

The first thing you should do is install git for windows. [Git for Windows](https://git-scm.com/download/win)

Then install visual studio build tools: [Microsoft Build tools](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=BuildTools&rel=16) (Make sure you install the C++ build tools. Just select `C++ build tools` in the installer and press install.)


Create a new folder somewhere on your computer.

Go in that folder then right-click open the git command prompt. (Right click in folder->Git Bash Here)

Run the commands:

```
git clone https://github.com/Microsoft/vcpkg
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg install protobuf --triplet x86-windows-static
./vcpkg install protobuf --triplet x64-windows-static
cd ..
git clone https://gitlab.com/Mr_Goldberg/goldberg_emulator.git
cd goldberg_emulator
```

This should build and install all the dependencies and clone the repo. Some commands like the bootstrap-vcpkg.bat and vcpkg install might take a while.


Then to build the debug experimental version run: `build_win_debug_experimental.bat`

To build the release version run: `build_win_release.bat`

If for some reason you want to set the protobuf directories to something different you can edit: `build_set_protobuf_directories.bat`

##### Pulling the latest code

Go in the goldberg_emulator folder then right-click open the git command prompt. (Right click in folder->Git Bash Here)

Run the command:
```
git pull
```


## Design Choices / FAQ

##### Why are there no ini files like all the other Steam Emulators?

I think that the way other steam emulators have an ini when you set everything on a per game setting is dumb. The only things that should be set on a per game setting is the things that are specific to that game like the appid, DLC, mods, interface versions, etc...

The rest like your name should be set in a global place because I don't like having to set every fucking name of everyone in an ini for every game I copy to people when I want to copy them games to play on my LAN.

My emu is made in a way that you can just install it on a game and then copy the game to people and they don't have to change anything.

I agree that the fact that I have multiple files might be dumb but it's actually not. Your filesystem is a database so why would you have to replicate that by making one config file when you can just have many. It's a lot easier to manage coding wise.

##### Is there any difference between the Windows and Linux versions?

There is no difference in functionality between the normal windows version and the linux version. Windows has a experimental build which has features that only makes sense on windows. 

##### What is the experimental version?

Read this if you want to know what it is: [The Experimental Readme](Readme_experimental.txt)

##### Is this illegal?

It's as illegal as Wine or any HLE console emulator. All this does is remove the steam dependency from your steam games.

##### But it breaks Steam DRM ?

It doesn't break any DRM. If the game has a protection that doesn't let you use a custom steam api dll it needs to be cracked before you use my emulator. Steam is a DRM as much as any API is a DRM. Steam has actual DRM called steamstub which can easily be cracked but this won't crack it for you.


##### Will you support CS:GO or Dota 2?

No, I don't care about making these games work because they use apis like the game coordinator that no other game uses. Also valve keeps changing them.

However if someone else wastes their time to get them working and I will happily merge their work.
