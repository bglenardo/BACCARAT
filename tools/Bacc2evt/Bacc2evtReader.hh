//////////////////////////////////////////////////////////////////////////////
//
//
//  Bacc2evtReader.hh
//
//  A support file containing various methods used to read the Bacc file

//////////////////////////////////////////////////////////////////////////////
//  
//  Change Log:
//
//   2 April 2010 - Initial Submission (Michael Woods)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef Bacc2EVTREADER_HH
#define Bacc2EVTREADER_HH 1

#include <string>
#include <vector>
#include <fstream>



        // ------+++++-----+++++-----+++++-----+++++-----+++++-----+++++-----
        // ------+++++ Read primary particle info.                 +++++-----
        // ------+++++-----+++++-----+++++-----+++++-----+++++-----+++++-----
    //bool has_xenon_records = file_has_xe_record_levels(inFilestream, iNumRecords);
    //has_xenon_records = false;
    //bool new_event, last_event = false;

struct volume_map {
  std::vector<std::string> vols;
  std::vector<int>ids;
};

struct primary_particle_spacial_and_energy {
  int num;
  char* name;
  double energy;
  double time;
  double pos[3];
  double dir[3];
};

struct answer_key {
  long long key_index;
  long long event_number_Bacc;
  long long event_number_analysis;
  unsigned short event_caused_trigger;
  std::string prim_particle_type;
  double energy_prim_par;
  double x_prim_par, y_prim_par, z_prim_par;
  double xdir_prim_par, ydir_prim_par, zdir_prim_par;
  std::string particle_type;
  double energy;    // total energy deposition in valid LXe vols.
  double x,y,z;     // first x,y,z (soon to be energy weighted x,y,z
  double missing_energy;
  std::vector<double> x_scats, y_scats, z_scats;
  std::vector<double> energy_deps;
  int photons_per_chan[122];
  long long timestamp;
  double msd;
};

std::string get_volume_name(int vol_id, std::vector<std::string> &vols, std::vector<int>&ids);
int get_volume_id(std::string vol_name, std::vector<std::string> &vols, std::vector<int>&ids);
bool is_xenon_vol(std::string vol_name);
bool file_has_xe_record_levels(std::ifstream& in_file, int iNumRecords, volume_map map, bool has_emission_time, int DEBUG=-1);
// Not being used. Should be kept until Fall 2013 in case it is reimplemented.
//answer_key build_answer_key(std::ifstream& in_file, size_t record_starting_point);
#endif
