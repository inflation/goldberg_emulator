#!/bin/bash
APP_PATH=./example_executable
APP_ID=480

set -e
mkdir -p ~/.steam/sdk64
mkdir -p ~/.steam/sdk32
#make a backup of original files
mv ~/.steam/steam.pid ~/.steam/steam.pid.orig || true
mv ~/.steam/sdk64/steamclient.so ~/.steam/sdk64/steamclient.so.orig || true
mv ~/.steam/sdk32/steamclient.so ~/.steam/sdk32/steamclient.so.orig || true
#copy our files
cp x86/steamclient.so ~/.steam/sdk32/steamclient.so
cp x86_64/steamclient.so ~/.steam/sdk64/steamclient.so
echo $BASHPID > ~/.steam/steam.pid
SteamAppId=$APP_ID SteamGameId=$APP_ID $APP_PATH

#restore original
rm -f ~/.steam/steam.pid
rm -f ~/.steam/sdk64/steamclient.so
rm -f ~/.steam/sdk32/steamclient.so
mv ~/.steam/steam.pid.orig ~/.steam/steam.pid
mv ~/.steam/sdk64/steamclient.so.orig ~/.steam/sdk64/steamclient.so || true
mv ~/.steam/sdk32/steamclient.so.orig ~/.steam/sdk32/steamclient.so || true
