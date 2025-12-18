#!/bin/bash
set -e

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B build
mv -f build/compile_commands.json .

printf "\n\n=============================================================================\n\n"

cmake --build build --config Debug -j 16

printf "\n\n=============================================================================\n\n"

cd build/test && ctest -j 16 --output-on-failure
cd ../..

if [ "$1" == "dbg" ]; then
    WAYLAND_DEBUG=1 ./build/gltk_test_app 2>&1 | tee gltk_wl_debug.log
    exit 0
fi
./build/gltk_test_app
