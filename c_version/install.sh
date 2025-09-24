#!/bin/bash
echo downloading sdl3
curl -L https://github.com/libsdl-org/SDL/releases/download/release-3.2.22/SDL3-3.2.22.tar.gz | tar zx

cd SDL3-3.2.22
echo installing sd3
cmake -S . -B build
cmake --build build

echo installing SDL3 locally in "$HOME/SDL"
cmake --install build --prefix $HOME/SDL

# TTF LIBRARY
# echo downloading TTF sdl3 library
# cd ..
# curl -L https://github.com/libsdl-org/SDL_ttf/releases/download/release-3.2.2/SDL3_ttf-3.2.2.tar.gz | tar zx

# cd SDL3_ttf-3.2.2
# echo installing sdl3 ttf library
# CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:../SDL3-3.2.22/cmake
# cmake -S . -B build
# cmake --build build

# echo installing SDL3_TTF locally in "$HOME/SDL_TTF"
# cmake --install build --prefix $HOME/SDL_TTF

echo removing installation directories
cd ..
rm -rf SDL3-3.2.22
# rm -rf SDL3_ttf-3.2.2