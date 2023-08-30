#!/bin/bash
echo "Hello World !"

mkdir build
cd build
emcmake cmake ..
cmake --build .
cd ..

cp build/PBD.js ../src/
cp build/*.wasm ../static/
npm run dev
