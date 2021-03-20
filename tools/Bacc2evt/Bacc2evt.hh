//////////////////////////////////////////////////////////////////////////////
//
//
//  Bacc2evt.hh
//
//  This program accepts the .bin files produced by Bacc. The output of this
//  program is a .evt file. This can then be analyzed with the full LUX
//  analysis chains.

//////////////////////////////////////////////////////////////////////////////
//  
//  Change Log:
//
//  ## Month 2010 - Initial Submission (Michael Woods)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef Bacc2EVT_HH
#define Bacc2EVT_HH 1

//  
//  C/C++ includes
//

#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <stdlib.h>


void Bacc_usage();
void Bacc_help();
void run_managed(int arg, int argc, char** argv,
    std::vector<std::string> input_files);
std::string get_Bacc_bin_datetime(std::string filename);

#endif
