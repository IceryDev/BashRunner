#!/bin/bash
set -e

if [ ! -d build ]
then
    cmake -B build -DCMAKE_BUILD_TYPE=Release
fi
cmake --build build
sudo cmake --install build

kbuildsycoca6 --noincremental
kquitapp6 krunner 2>/dev/null
krunner &