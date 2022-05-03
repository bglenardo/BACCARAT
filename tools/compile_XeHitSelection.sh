#!/bin/bash

g++ -g -o XeDetectorHitSelection `root-config --cflags --libs` -L/g/g20/lenardo1/Simulations/BACCARAT/tools -lBaccRootConverterEvent XeDetectorHitSelection.cc
export LD_LIBRARY_PATH=/g/g20/lenardo1/Simulations/BACCARAT/tools:$LD_LIBRARY_PATH
#cp ./BACCARAT/tools/libBaccRootConverterEvent.so ./
