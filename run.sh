#!/bin/bash
set -e

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B build
mv -f build/compile_commands.json .

printf "\n\n=============================================================================\n\n"

cmake --build build --config Release -j 16

printf "\n\n=============================================================================\n\n"

cd build/test && ctest -j 16 --output-on-failure
cd ../..


./build/simple_slide
# ./build/gltk_test_app
