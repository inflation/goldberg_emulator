#!/bin/bash

rm -rf glew*

URL="https://downloads.sourceforge.net/project/glew/glew/2.1.0/glew-2.1.0-win32.zip?r=https%3A%2F%2Fsourceforge.net%2Fprojects%2Fglew%2Ffiles%2Fglew%2F2.1.0%2Fglew-2.1.0-win32.zip%2Fdownload&ts=1565791827"

wget "$URL" -O glew.zip
unzip glew.zip
rm glew.zip
mv glew-* glew
# Fix libraries name
sed -i "s/LIBCMT/libcmt/;s/OLDNAMES/oldnames/" glew/lib/Release/*/glew32s.lib
