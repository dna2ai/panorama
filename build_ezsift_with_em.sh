#!/bin/bash

set -xe

SELF=$(cd `dirname $0`; pwd)

pushd $SELF
mkdir -p local
emcc -I ezSIFT/include -I ezSIFT/src ezSIFT/src/ezsift.cpp -c -o local/ezsift.o
emcc -I ezSIFT/include -I ezSIFT/src ezSIFT/src/image_utility.cpp -c -o local/image_utility.o
emcc -I ezSIFT/include -I ezSIFT/src \
     -O3 \
     ezsiftwrapper.cpp local/ezsift.o local/image_utility.o \
     -o local/ezsift.js \
     -s WASM=0 \
     -s ALLOW_MEMORY_GROWTH=1 \
     --memory-init-file 0
popd
