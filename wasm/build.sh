#!/bin/bash

# Generating the bindings glue code
python emscripten/tools/webidl_binder.py cloth_simulator.idl glue

echo "Hello World !"

mkdir build
cd build
emcmake cmake ..
cmake --build .
cd ..


# input_file="build/PBD.js"
# old_text="const { createRequire } = await import"
# new_text="const { createRequire } = await require"
# escaped_old_text=$(printf '%s\n' "$old_text" | sed 's/[[\/.*^$]/\\&/g')
# escaped_new_text=$(printf '%s\n' "$new_text" | sed 's/[\/&]/\\&/g')
# sed -i "s/$escaped_old_text/$escaped_new_text/g" "$input_file"

# echo "替换完成"

# echo "替换完成"

cp build/PBD.js ../src/
cp build/PBD.wasm ../src/
# npm run dev
