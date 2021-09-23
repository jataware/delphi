#!/bin/bash

rm -rf build
rm -rf delphi/cpp/*

mkdir -p build
cd build
cmake ..
cmake --build . -- -j12 DelphiPython
cp *.so ../delphi/cpp
cd ..

time python test.py

# w/ flags:
# Burning 1000 samples out...
# {100.0%} [##############################] ( 3.7s < 0.0s) 
# Sampling 200 samples from posterior...
# {100.0%} [##############################] ( 0.7s < 0.0s) 
# Predicting for 25 time steps...
# {100.0%} [##############################] ( 0.0s < 0.0s) 
# real    0m5.267s
# user    0m6.734s
# sys     0m2.643s