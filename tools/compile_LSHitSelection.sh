#!/bin/bash

g++ -g -o LSDetectorHitSelection `root-config --cflags --libs` -L/g/g20/lenardo1/Simulations/BACCARAT/tools -lBaccRootConverterEvent LSDetectorHitSelection.cc
export LD_LIBRARY_PATH=/g/g20/lenardo1/Simulations/BACCARAT/tools:$LD_LIBRARY_PATH
#cp ./BACCARAT/tools/libBaccRootConverterEvent.so ./
