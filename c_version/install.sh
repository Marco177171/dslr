#!/bin/bash
echo downloading sdl3
curl -L https://github.com/libsdl-org/SDL/releases/download/release-3.2.22/SDL3-3.2.22.tar.gz | tar zx

cd SDL3-3.2.22
echo installing sd3
cmake -S . -B build
cmake --build build

echo installing SDL3 locally in "$HOME/SDL"
cmake --install build --prefix $HOME/SDL


echo removing installation dir
cd ..
rm -rf SDL3-3.2.22
