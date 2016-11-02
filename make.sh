#!/bin/bash
cd Toolchain
make clean
make all
cd ..
make clean
make all