#!/bin/bash
set -e
cmake --build cmake-build-debug
sudo cmake --install cmake-build-debug
kbuildsycoca6 --noincremental
kquitapp6 krunner 2>/dev/null
krunner &