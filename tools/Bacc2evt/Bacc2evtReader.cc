//////////////////////////////////////////////////////////////////////////////
//
//
//  Bacc2evtReader.cc
//
//  A support file containing various methods used to read the Bacc file

//////////////////////////////////////////////////////////////////////////////
//  
//  Change Log:
//
//   4 April 2010 - Initial Submission (Michael Woods)
//
//////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "Bacc2evtReader.hh"

// This defines a debug mode. Negative numbers do not activate debug. Numbers
// divisible by x enable debug mode.
#define DEBUG(x) !(DEBUG < 0 || DEBUG%x)

using namespace std;

string get_volume_name(int vol_id, vector<string> &vols, vector<int>&ids) {
  for(unsigned int i=0; i< vols.size(); i++) {
    if(vol_id == ids[i])
      return vols[i];
  }
  cerr << "Did not find volume id " << vol_id << " in list of " << vols.size();
  cerr << " volume ids." << endl;
  return "";
}

int get_volume_id(string vol_name, vector<string> &vols, vector<int>&ids) {
  for(unsigned int i=0; i< vols.size(); i++) {
    if(vol_name == vols[i])
      return ids[i];
  }
  cerr << "Did not find volume name " << vol_name << " in list of " << vols.size();
  cerr << " volumes." << endl;
  return -1;
}

bool is_xenon_vol(std::string vol_name) {
  if(vol_name == "LiquidXenon") return true;
  if(vol_name == "Bottom_XenonSpace") return true;
  if(vol_name == "LiquidHolderXenon") return true;
  return false;
}

bool file_has_xe_record_levels(std::ifstream& in_file, int iNumRecords, volume_map map, bool has_emission_time, int DEBUG) {
  // Determine if the binary file on hand has Xe record levels stored within.
  int Size;  // An int buffer to read sizes into. Used throughout code.
  //DEBUG=5;
  // in_file is the Bacc binary file.
  size_t start_point = in_file.tellg();
  // The file is assumed to be just before you start looping over NumRecords.

  for(int i=0; i<iNumRecords; i++) {
    int iPrimaryParNum;
    in_file.read((char*)(&iPrimaryParNum),sizeof(int));
    if(DEBUG(5)) cout << "iPrimParNum:\t"        << iPrimaryParNum << endl;
    vector<primary_particle_spacial_and_energy> primary_particles;
    for(int j=0; j<iPrimaryParNum; j++) {
      //cout << "We're at j=" << j << endl;
      struct primary_particle_spacial_and_energy prim_par;
      in_file.read((char*)(&Size),sizeof(Size));
      char* particleName = new char [Size+1];
      in_file.read((char*)(particleName),Size);
      particleName[Size] = '\0';
      prim_par.num = j;
      prim_par.name = particleName;
      //delete[] particleName;

      // Read in Primary Particle Energy, Pos(3), and Dir(3).
      in_file.read((char* )(&prim_par.energy), sizeof(prim_par.energy));
      if(has_emission_time)
        in_file.read((char* )(&prim_par.time), sizeof(prim_par.time));
      in_file.read((char* )(prim_par.pos), sizeof(double)*3);
      in_file.read((char* )(prim_par.dir), sizeof(double)*3);
      /*
      cout << "Size: " << Size << endl;
      cout << "struct: " << sizeof(prim_par) << endl;
      cout << "num: " << sizeof(prim_par.num) << endl;
      cout << "name: " << sizeof(prim_par.name) << endl;
      cout << "energy: " << sizeof(prim_par.energy) << endl;
      cout << "NME: " << prim_par.name<< endl;
      cout << "Enr: " << prim_par.energy << endl;
      cout << "POS: " << prim_par.pos[0] << "\t" << prim_par.pos[1] << "\t" << prim_par.pos[2] << endl;
      cout << "DIR: " << prim_par.dir[0] << "\t" << prim_par.dir[1] << "\t" << prim_par.dir[2] << endl;
      cout << endl;
      */

      // Save the primary particle info but only for the first one.
      // Usually there is only one.
      if( j==0 ) 
        primary_particles.push_back(prim_par);
    }

    int recordLevel;
    int optPhotRecordLevel;
    int thermElecRecordLevel;
    int iVolume;
    int iEvtNb;
    double fTotEnergyDep_keV;
    int iTotalOptPhotNumber;
    int iTotalThermElecNumber;
    int iRecordSize;
    in_file.read((char*)(&recordLevel),sizeof(int));
    in_file.read((char*)(&optPhotRecordLevel),sizeof(int));
    in_file.read((char*)(&thermElecRecordLevel),sizeof(int));
    in_file.read((char*)(&iVolume),sizeof(int));
    // No we have the two pieces of information we care about for this
    // function. We have the normal record level "recordLevel" and we have
    // the volume in which this is recorded "iVolume". If this record is in a
    // LXe volume that we think matters, then return true.
    string volume_name = get_volume_name(iVolume, map.vols, map.ids);
    if( is_xenon_vol(volume_name) ) {
      // We have our answer. The file has xenon record levels.
      // Return the file pointer to where it was before we started.
      in_file.seekg(start_point);
      return true;
    }
    in_file.read((char*)(&iEvtNb),sizeof(int));
    iEvtNb += 1;        // EvtNb starts at 1.
    if (recordLevel>0)
        in_file.read((char*)(&fTotEnergyDep_keV),sizeof(double));
    if (optPhotRecordLevel>0)
        in_file.read((char*)(&iTotalOptPhotNumber),sizeof(int));
    if (thermElecRecordLevel>0)
        in_file.read((char*)(&iTotalThermElecNumber),sizeof(int));
    in_file.read((char*)(&iRecordSize),sizeof(int));

    if(DEBUG(5)) {
        cout << "recordLevel:\t"        << recordLevel << endl;
        cout << "optPhotRecordLevel:\t" << optPhotRecordLevel << endl;
        cout << "thermElecRecordLevel:\t" << thermElecRecordLevel << endl;
        cout << "iVolume:\t"            << iVolume << endl;
        cout << "iEvtNb:\t"             << iEvtNb << endl;
        if (recordLevel > 0)
        cout << "fTotEnergyDep_keV:\t"  << fTotEnergyDep_keV << endl;
        if (optPhotRecordLevel>0)
        cout << "iTotalOptPhotNumber:\t"<< iTotalOptPhotNumber << endl;
        if (thermElecRecordLevel>0)
        cout << "iTotalThermElecNumber:\t"<< iTotalThermElecNumber << endl;
        cout << "iRecordSize:\t"        << iRecordSize << endl;
    }

    struct datalevel {
        int stepNumber;
        int particleID;
        int trackID;
        int parentID;
        double particleEnergy;
        double particleDirection[3];
        double energyDeposition;
        double position[3];
        double stepTime;
    } data;

    
    for(int j=0; j<iRecordSize; j++){
        in_file.read((char*)(&Size),sizeof(Size));
        char* particleName = new char [Size+1];
        in_file.read((char*)(particleName), Size);
        in_file.read((char*)(&Size),sizeof(Size));
        char* processName = new char [Size+1];
        in_file.read((char*)(processName),Size);
        
        in_file.read((char*)(&Size),sizeof(Size));
        char* stepName = new char [Size+1];
        in_file.read((char*)(stepName),Size);
        
        in_file.read((char*)(&data),sizeof(data));
	delete[] stepName;
        delete[] processName;
        delete[] particleName;
    }   // End loop over j, iRecordSize
  }

  // Return the file pointer to where it was before we started.
  in_file.seekg(start_point);
  return false;
}

// Not being used. Should be kept until Fall 2013 in case it is reimplemented.
//answer_key build_answer_key(std::ifstream& in_file, size_t record_starting_point) {
//  // Since the answer key needs to built, we'll need to gain access to the
//  // beginning of the record that started this event hence we'll need to go
//  // to that point in the file using record_starting_point.
//
//  answer_key key;
//
//  // in_file is the Bacc binary file. Save the current location.
//  size_t start_point = in_file.tellg();
//  
//  key.event_number_Bacc = -1;
//  key.event_number_analysis = -2;
//  key.event_caused_trigger = -3;
//  key.particle_type = "-4";
//  key.energy = -5;
//  key.x = -6;
//  key.y = -7;
//  key.z = -8;
//  //key.scat_x, scat_y, scat_z;
//  key.photons_per_chan[0] = -9;
//  key.timestamp = -10;
//
//  /*
//  for(int j=0; j<iRecordSize; j++){
//    ReadSize();
//    char* particleName = new char [Size+1];
//    inFilestream.read((char*)(particleName), Size);
//    ReadSize();
//    char* processName = new char [Size+1];
//    inFilestream.read((char*)(processName),Size);
//    inFilestream.read((char*)(&data),sizeof(data));
//    delete[] processName;
//    delete[] particleName;
//    if (DEBUG(5)) {
//      cout << "data.stepNumber:\t" << data.stepNumber << endl;
//      cout << "data.particleID:\t" << data.particleID << endl;
//      cout << "data.trackID:\t" << data.trackID << endl;
//      cout << "data.parentID:\t" << data.parentID << endl;
//      cout << "data.particleEnergy:\t" << data.particleEnergy << endl;
//      cout << "data.particleDirection[0]:\t" << data.particleDirection[0] << endl;
//      cout << "data.energyDeposition:\t" << data.energyDeposition << endl;
//      cout << "data.position[0]:\t" << data.position[0] << endl;
//      cout << "data.stepTime:\t" << data.stepTime << endl;
//    }
//    if(current_pmt >= 0) {
//      timingInfoVec[iEvtIndex][current_pmt].push_back(data.stepTime);
//    }
//
//    */
//
//  // Return the file pointer to where it was before we started.
//  in_file.seekg(start_point);
//  return key;
//}
