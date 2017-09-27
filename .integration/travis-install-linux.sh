#!/bin/bash

apt-get update && apt-get install -y cmake libgtksourceviewmm-3.0-dev freeglut3 freeglut3-dev mesa-common-dev libglu1-mesa-dev libglew-dev libglfw3-dev libdw-dev libcppunit-dev gcovr libglm-dev libsoil-dev git xvfb checkinstall rpm g++ clang llvm-4.0-tools

ln -s /usr/bin/llvm-symbolizer-4.0 /usr/bin/llvm-symbolizer