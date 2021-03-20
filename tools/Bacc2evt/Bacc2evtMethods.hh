
//////////////////////////////////////////////////////////////////////////////
//
//
//  Bacc2evtMethods.hh
//
//  A support file containing various methods used in Bacc2evt

//////////////////////////////////////////////////////////////////////////////
//  
//  Change Log:
//
//  ## Month 2010 - Initial Submission (Michael Woods)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef Bacc2EVTMETHODS_HH
#define Bacc2EVTMETHODS_HH 1

//  
//  C/C++ includes
//

#include <sstream>
#include <fstream>
//#include <stdio.h>
#include <string>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdlib.h>

#include "Bacc2evt.hh"
#include "Bacc2evtReader.hh"


void determineSensitiveVolume(char* InputCommands,
                    char* top_vol, char* bot_vol);
int* makePMTLookupTable(char* DetComp, int* PmtLookup, bool oldPMTStyle);
int GetPMTNumber(int* PMTLookupTable, int Geant4VolID);
void createPositionLookup(int* pmt_relations);
double minElement(std::vector<double> &vec);
double maxElement(std::vector<double>  &ec);
unsigned short mVtoADC(double mV);
unsigned short VtoADC(double V);
double round(double a);
void GetPMTCoords(double* pmtX, double* pmtY);
double GetOffset();
double Rand();
double RandN();
double Gauss(double x, double mean, double height, double sigma);
int Poisson(double mean);
unsigned short vec_avg(std::vector<unsigned short> &vec);
int find_last_evt_file(std::string filename, std::string outPath);
std::vector<std::string> get_cwd_bins();
std::vector<std::string> get_dir_bins(std::string dir);
std::vector<std::string> get_cli_bins(int arg, int argc, char** argv);
double randn_trig(double mu,double sigma);
void write_event_to_disk(std::vector< std::vector<double> >timingInfoVec, std::ofstream& phot_time_file);
void makeVolumeLookupTable(std::string DetComp, std::vector<std::string> &vols, std::vector<int>&ids);
double calculate_max_scattering_distance(answer_key key);
bool is_in_active_region(double x,double y,double z);

#endif
