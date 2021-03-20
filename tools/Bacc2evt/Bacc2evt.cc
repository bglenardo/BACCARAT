//////////////////////////////////////////////////////////////////////////////
//
//
//  Bacc2evt.cc
//
//  This program accepts the .bin files produced by Bacc. The output of this
//  program is a/several .evt files. This can then be analyzed with the LUX
//  analysis chains.
//
//  
//  
//////////////////////////////////////////////////////////////////////////////
//  
//  Change Log:
//
//  01 March 2012 - Initial Submission (Michael Woods)
//
//////////////////////////////////////////////////////////////////////////////

//  
//  C/C++ includes
//

#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <time.h>
#include <sstream>



//
//  Bacc2evt includes
//
#include "Bacc2evt.hh"
#include "Bacc2evtMethods.hh"
#include "Bacc2evtPulse.hh"
#include "Bacc2evtTrigger.hh"
#include "Bacc2evtReader.hh"
#include "XMLtoVector.hh"

#define ReadSize() inFilestream.read((char*)(&Size), sizeof(int))
// This defines a debug mode. Negative numbers do not activate debug. Numbers
// divisible by x enable debug mode.
#define DEBUG(x) !(DEBUG < 0 || DEBUG%x)

#define SP(x) cout << #x":\t" << x << endl

#define UPDATE true

using namespace std;

/*
struct primary_particle_spacial_and_energy {
  int num;
  char* name;
  double energy;
  double pos[3];
  double dir[3];
};

struct answer_key {
  long long event_number_Bacc;
  long long event_number_analysis;
  bool event_caused_trigger;
  string particle_type;
  double energy;
  double x,y,z;
  vector<double> scat_x, scat_y, scat_z;
  int photons_per_chan[122];
  long long timestamp;
}
*/


//------++++++------++++++------++++++------++++++------++++++------++++++------

int main (int argc, char** argv){

  //cout << "Args: " << endl;
  //for(int i=0; i< argc; i++)
  //     cout << argv[i] << " ";
  //cout << "" << endl;

      // Much has been reused from the LUXRootReader.cc file (Chao)

    // NOTE: MORE HAS BEEN ADDED. See Bacc_help() at bottom of this file.
    // DEBUG is defined as:                                           == old!
    //  D%2 -> General IO                                             == old!
    //  D%3 -> Header from .bin file                                  == old!
    //  D%5 -> Details of event (PrimPar not included)                == old!
    //  D%7 -> Raw Dump of step times.                                == old!
    //  D%11-> Raw Pulse Data                                         == old!
    // So then DEBUG=2*7=14 will give General and Raw Pulse Data since== old!
    // 14%2 == True and 14%7 == True.
    //
    // NOTE: MORE HAS BEEN ADDED. See Bacc_help() at bottom of this file.
    //

    // Read /dev/random for a random series of bits to make the random seed.
    // At run time the -s flag can pass in a seed that may override this
    // value.
    ifstream devrandom("/dev/random");
    unsigned int seed;
    devrandom.read( (char*)(&seed), sizeof( seed ) );
    if( seed < 0 ) seed = -seed;
    devrandom.close();
    // Set the random number seed that is used for Bacc2evt random number
    // generation.
    srand(seed);

    int DEBUG = 2;     // Some large prime number
    int arg = 0;
    int input_arg_pos = 0;
    std::vector<int> xmlPMTs;
    std::vector<double> xmlGainVariations;
    std::vector<double> xmlGains;
    bool useXMLGains = false;
    bool oldPMTStyle = false;
    std::vector<std::string> input_files;
    string inFilename;
    string permFilename;
    string evtfilename="";

    ifstream inFilestream;

    bool ActAsManager = false;
    //bool LookForFiles = false; //Removed because it wasn't used

    // Check for proper runtime: If you don't specifiy any arguments (1)
    // then you're informed.
    if(argc==1){
        Bacc_usage();
        exit(0);
    }
    // Loop through and process arguments.
    for(arg=1; arg<argc; arg++){
        if(strcmp(argv[arg], "--help")==0 || strcmp(argv[arg], "-h")==0){
            Bacc_help();
            exit(0);
        }

        // Check for verbosity.
        if(strcmp(argv[arg], "-v") == 0 || strcmp(argv[arg], "--verbose")==0){
            DEBUG = atoi(argv[arg+1]);
            arg++;
            continue;
        }

        //Check if the old PMT arrangement was marked as used in the sim
        if(strcmp(argv[arg],"-p")==0 || strcmp(argv[arg],"--pmtOldStyle")==0){
          oldPMTStyle = true;
        }

        //Check if an xml input was specified - if so, load it.
        if(strcmp(argv[arg],"-x") == 0 || strcmp(argv[arg],"--xml") == 0){
          useXMLGains = true;
          if(arg == argc - 1){
            cout << "The -x option requires an xml filename after it"
                 << endl;
            exit(1);
          }
          string* xmlString = NULL;
          int buffSize = 50000;
          while(true){
            ifstream inXML;
            inXML.open(argv[arg+1],ios::in);
            char xmlbuffer[buffSize];
            inXML.get(xmlbuffer,buffSize,'\0');
            xmlString = new string(xmlbuffer);
            if((int)xmlString->size()==buffSize-1){
              delete xmlString;
              buffSize *= 2;
            }
            else break;
          }
          if(xmlString->size()<=0){
            cout << "Couldn't read xml file: " << argv[arg+1] << endl;
            delete xmlString;
            exit(1);
          }
          xmlPMTs = XMLtoVectorInt(*xmlString,"PMT_number");
          xmlGains = XMLtoVectorDbl(*xmlString,"mVns_per_phe");
          xmlGainVariations = XMLtoVectorDbl(*xmlString,"sigma_mVns_per_phe");
          delete xmlString;
          arg++;

          continue;
        }

        if(strcmp(argv[arg], "-a") == 0 || strcmp(argv[arg], "--append")==0){
            //LookForFiles = true;
            continue;
        }

        if(strcmp(argv[arg], "-i") == 0 || strcmp(argv[arg], "--input")==0){
            ActAsManager = true;
            //LookForFiles = true;
            input_arg_pos = arg;
            // There are three options for input.
            // The --input switch is the last argument. Use cwd files.
            if(input_arg_pos == argc - 1) { 
                input_files = get_cwd_bins();
            }
            // The --input switch is followed by one arg. Assumed to be a dir.
            else if(input_arg_pos == argc - 2) { 
                input_files = get_dir_bins(argv[arg+1]);
            }
            else {
                input_files = get_cli_bins(arg+1, argc, argv);
            }
            arg++;
            break;
        }

        if(strcmp(argv[arg], "-o") == 0 || strcmp(argv[arg], "--output")==0){
            evtfilename = argv[arg+1];
            arg++;
            continue;
        }

        if(strcmp(argv[arg], "-s") == 0 || strcmp(argv[arg], "--seed")==0){
            seed = atoi(argv[arg+1]);
            srand(seed);
            arg++;
            continue;
        }

        // If we've gotten to this point, then it wasn't a runtime a switch
        // or its options, so it must be the file name.
        inFilename = argv[arg];
        input_files.push_back(inFilename);

    }
    


    //Print gains debug information
    if(DEBUG(31)){
      cout << "PMT\tGain\tWidth of sphe area distribution" << endl;
      for(size_t c = 0; c < xmlGains.size(); c++){
        cout << xmlPMTs[c] << "\t" << xmlGains[c] << "\t" << xmlGainVariations[c] << endl;
      }
    }


    if(ActAsManager == true){
      //cout << "ActAsManager == true" << endl;
        run_managed(input_arg_pos, argc, argv, input_files);
        return 0;
    }
    else {
      //cout << "ActAsManager == false"<< endl;
        //evtfilename = input_files[0];
    }

    // If debugging is enabled, print debugging level.
    if(DEBUG==0) DEBUG = 103;   // Treat 0 debugging case as no output.
    //if(DEBUG!=103) cout << "DEBUG level: " << DEBUG << endl;

    // Open Bacc binary.
    inFilestream.open(inFilename.c_str(), ios::binary|ios::in);

    // Determine the random seed used for the Bacc file.
    //    Some_CUSTOM__-_FileName--###.bin
    //                             ### is the seed.
    string random_seed_str;
    size_t bin = inFilename.rfind(".bin");
    cout << "inFilename = " << inFilename << endl;
    random_seed_str = inFilename.substr(0, bin);
    random_seed_str = string(random_seed_str.rbegin(), random_seed_str.rend());
    size_t num = random_seed_str.find_first_not_of("0123456789");
    random_seed_str = random_seed_str.substr(0, num);
    random_seed_str = string(random_seed_str.rbegin(), random_seed_str.rend());
    long long random_seed = atoi(random_seed_str.c_str());

    if(!inFilestream.is_open()) {
        cerr << "File " << inFilename << " could not be found." << endl;
        exit(0);
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

    int Size;  // An int buffer to read sizes into. Used throughout code.
    int iNumRecords;
    inFilestream.read((char*)(&iNumRecords),sizeof(int));

    inFilestream.read((char*)(&Size), sizeof(int));
    char* productionTime = new char [Size+1];
    inFilestream.read((char*)(productionTime), Size);
    productionTime[Size]='\0';

    //   Need to format the production time for creation of .evt file. This   //
    //   looks like "GMT: Fri Apr 16 20:21:16 2010\n" and it will need to     //
    //   be formated as to produce filenames like luxsm_YYYYMMDDTHHMM.evt     //
    string buff;   // Temp buffer to work with.
    string year;
    string month;
    string day;
    string hour;
    string minute;

    buff = "00";
    for(int i=0; i<2; i++) buff[i] = productionTime[i+13];  // Get day, but
    for(int i=0; i<2; i++) if(buff[i] ==' ') buff[i] = '0'; // days can be " 8"
    day = buff;
    for(int i=0; i<2; i++) buff[i] = productionTime[i+16];  // Get hour
    hour = buff;
    for(int i=0; i<2; i++) buff[i] = productionTime[i+19];  // Get minute
    minute = buff;
    buff = "000";
    for(int i=0; i<3; i++) buff[i] = productionTime[i+9];   // Get month
    if(buff == "Jan") month = "01";
    if(buff == "Feb") month = "02";
    if(buff == "Mar") month = "03";
    if(buff == "Apr") month = "04";
    if(buff == "May") month = "05";
    if(buff == "Jun") month = "06";
    if(buff == "Jul") month = "07";
    if(buff == "Aug") month = "08";
    if(buff == "Sep") month = "09";
    if(buff == "Oct") month = "10";
    if(buff == "Nov") month = "11";
    if(buff == "Dec") month = "12";
    buff = "0000";
    for(int i=0; i<4; i++) buff[i] = productionTime[i+25];  // Get year
    year = buff;

    string outPath;         // outPath used for .evt file creation.
    int path_end = inFilename.find_last_of('/');
    if(path_end == -1)      // -1 signifies char not found (so no path given)
        outPath = ".";     // so we just use the cwd.
    else outPath = inFilename.substr(0,path_end);

    // Appending the _f000000001.evt portion of the filename. The option
    // exists to do this while looking at the cwd of the output so that
    // files are not overwritten.

    //string run_datetime =  get_Bacc_bin_datetime(input_files[0]);
    //cout << year+month+day+"T"+hour+minute << endl;
    //cout << get_Bacc_bin_datetime(input_files[0]) << endl;
    if(evtfilename == "") {
        evtfilename = "luxsm_";
        evtfilename += get_Bacc_bin_datetime(input_files[0]);
        evtfilename += "_f000000001.evt";
    }

    string date_string = evtfilename.substr(6,8) + evtfilename.substr(15,4);
    evtfilename = outPath + '/' + evtfilename;

    // OK, now that the filenames have been parsed, output them if requested
    if(!(DEBUG%2)) cout << endl << "Input Filename: " << inFilename << endl;
    if(!(DEBUG%2)) cout << "Output Filename: " << evtfilename << endl << endl;

    //exit(0);

    
    ReadSize();
    char* geant4Version = new char [Size+1];
    inFilestream.read(geant4Version, Size);
    geant4Version[Size]='\0';

    ReadSize();
    char* svnVersion = new char [Size+1];
    inFilestream.read(svnVersion, Size);
    svnVersion[Size]='\0';

    ReadSize();
    char* uName = new char [Size+1];
    inFilestream.read(uName, Size);
    uName[Size]='\0';
    
    ReadSize();
    char* inputCommands = new char [Size+1];
    inFilestream.read(inputCommands, Size);
    inputCommands[Size]='\0';

    ReadSize();
    char* diffs = new char [Size+1];
    inFilestream.read(diffs, Size);
    diffs[Size]='\0';

    ReadSize();
    char* DetCompo = new char [Size+1];
    inFilestream.read(DetCompo, Size);
    DetCompo[Size]='\0';

    if(!(DEBUG%3)) {
        cout << "Number of Records: " << iNumRecords << endl;
        cout << "Production time: " << productionTime;
        cout << "Geant4 Version: " << geant4Version << endl;
        cout << "Svn Version: " << svnVersion << endl;
        cout << "Hostname: " << uName;
        cout << "Input Commands: " << inputCommands << endl;
        cout << "svn diff: " << diffs << endl;
        cout << "Detector Components: " << DetCompo << endl;
    }


    // SVN Revision Recording
    // This converts the string read from "Revision: 594" to "594"
    svnVersion = svnVersion + 10; 
    int svnVersion_int = atoi(svnVersion);
    // After svn revision 605, a new field in the binary field was introduced
    // that keeps the primary particle emission time. This notices if reading
    // that field is necessary.
    bool has_emission_time = (svnVersion_int > 606) ? true : false;
    if( svnVersion_int == 0 )
        has_emission_time = true;

    // Ok, now let's take the time and loop through the data and determine
    // the volume name that is being used for storing data so that an
    // assumption on the name doesn't have to be made.
    // We'll return to the current_pos after our examination.
    char* top_vol = new char [100];
    char* bot_vol = new char [100];
    determineSensitiveVolume(inputCommands, top_vol, bot_vol);
    delete [] top_vol;
    delete [] bot_vol;

    // The above did a basic check if the file should be written, so now let's
    // open the evt file for writing to create it on disk.
    ofstream evtfile;
    evtfile.open(evtfilename.c_str(), ios::out | ios::binary );
    evtfile.write("",0);        // Force the file to write.

    // Now that we have the list of the Geant4 detector components, make
    // the PMT lookup table. See documentation in the function for more.
    const int numPmts = 122;
    int PMTLookupTable[numPmts];
    string DetCompoStr = DetCompo;
    makePMTLookupTable(DetCompo, PMTLookupTable, oldPMTStyle);

    // Create the vector array we'll be using to store the times at which
    // PMTs are hit for each event.
    // The structure of this when we are going to be accessing it is:
    // timingInfoVec[event_num][chan_num][photon_num] = a_photon_arrival_time
    vector< vector< vector<double> > > timingInfoVec;
    // And also a 

    // Create a variable to temporarily store an event number so we know when
    // we start a new event in the loop.
    int previous_event_number=-1;
    int iEvtIndex=0;

    vector<string> volumes;
    vector<int> volume_ids;
    makeVolumeLookupTable(DetCompoStr, volumes, volume_ids);
    
        // ------+++++-----+++++-----+++++-----+++++-----+++++-----+++++-----
        // ------+++++ NEW READ STRUCTURE                          +++++-----
        // ------+++++-----+++++-----+++++-----+++++-----+++++-----+++++-----
        
    volume_map map;
    map.vols = volumes;
    map.ids = volume_ids;
    bool has_xenon_records = file_has_xe_record_levels(inFilestream, iNumRecords, map, has_emission_time);

    bool new_event = true;
    bool need_answer_key = true;
    answer_key key;
    for(size_t clear_pmt_i=0; clear_pmt_i<122; clear_pmt_i++)
      key.photons_per_chan[clear_pmt_i] = 0;
    vector<answer_key> keys;
    for(int i=0; i<iNumRecords; i++) {
        if(UPDATE && i%1000 == 0) {
          printf("\rReading records from simulation file... %.2f%% ", 100.*i/iNumRecords);
          cout.flush();
        }
        if(DEBUG(5)) {
            cout << "" << endl;
            cout << "=== Main Loop ===========================" << endl;
            cout << "i = " << i << " of iNumRecords(" << iNumRecords << ")" << endl;
            cout << "=========================================" << endl;
            cout << "Main loop, file position: " << inFilestream.tellg() << endl;
        }




        // ------+++++-----+++++-----+++++-----+++++-----+++++-----+++++-----
        // ------+++++ Read primary particle info.                 +++++-----
        // ------+++++-----+++++-----+++++-----+++++-----+++++-----+++++-----
        int iPrimaryParNum;
        inFilestream.read((char*)(&iPrimaryParNum),sizeof(int));
        if(DEBUG(5)) cout << "iPrimParNum:\t"        << iPrimaryParNum << endl;
        vector<primary_particle_spacial_and_energy> primary_particles;
        //cout << "iNR: " << iNumRecords << endl;
        //cout << "PPN: " << iPrimaryParNum << endl;
        for(int j=0; j<iPrimaryParNum; j++) {
            struct primary_particle_spacial_and_energy prim_par;
            ReadSize();
            char* particleName = new char [Size+1];
            inFilestream.read((char*)(particleName),Size);
            particleName[Size] = '\0';
            prim_par.num = j;
            prim_par.name = particleName;
            //delete[] particleName;

            // Read in Primary Particle Energy, Pos(3), and Dir(3).
            inFilestream.read((char* )(&prim_par.energy), sizeof(prim_par.energy));
            if(has_emission_time)
              inFilestream.read((char* )(&prim_par.time), sizeof(prim_par.time));
            inFilestream.read((char* )(prim_par.pos), sizeof(double)*3);
            inFilestream.read((char* )(prim_par.dir), sizeof(double)*3);
            //cout << "Size: " << Size << endl;
            //cout << "struct: " << sizeof(prim_par) << endl;
            //cout << "num: " << sizeof(prim_par.num) << endl;
            //cout << "name: " << sizeof(prim_par.name) << endl;
            //cout << "energy: " << sizeof(prim_par.energy) << endl;
            //inFilestream.read((char* )(&prim_par.energy), sizeof(prim_par));
            /*
            cout << endl;
            cout << "i:   " << i << endl;
            cout << "j:   " << j << endl;
            cout << "NME: " << prim_par.name<< endl;
            cout << "Enr: " << prim_par.energy << endl;
            if(has_emission_time)
              cout << "Tim: " << prim_par.time << endl;
            cout << "POS: " << prim_par.pos[0] << "\t" << prim_par.pos[1] << "\t" << prim_par.pos[2] << endl;
            cout << "DIR: " << prim_par.dir[0] << "\t" << prim_par.dir[1] << "\t" << prim_par.dir[2] << endl;
            cout << endl;
            */

            // Save the primary particle info but only for the first one.
            // Usually there is only one.
            if( j==0 ) 
              primary_particles.push_back(prim_par);
        }
        // ------+++++-----+++++-----+++++-----+++++-----+++++-----+++++-----
       
      
        // ------+++++-----+++++-----+++++-----+++++-----+++++-----+++++-----
        // ------+++++ Read Records                                +++++-----
        // ------+++++-----+++++-----+++++-----+++++-----+++++-----+++++-----


        int recordLevel;
        int optPhotRecordLevel;
        int thermElecRecordLevel;
        int iVolume;
        int iEvtNb;
        double fTotEnergyDep_keV;
        int iTotalOptPhotNumber;
        int iTotalThermElecNumber;
        int iRecordSize;
        inFilestream.read((char*)(&recordLevel),sizeof(int));
        inFilestream.read((char*)(&optPhotRecordLevel),sizeof(int));
        inFilestream.read((char*)(&thermElecRecordLevel),sizeof(int));
        inFilestream.read((char*)(&iVolume),sizeof(int));
        inFilestream.read((char*)(&iEvtNb),sizeof(int));
        iEvtNb += 1;        // EvtNb starts at 1.
        if (recordLevel>0)
            inFilestream.read((char*)(&fTotEnergyDep_keV),sizeof(double));
        if (optPhotRecordLevel>0)
            inFilestream.read((char*)(&iTotalOptPhotNumber),sizeof(int));
        if (thermElecRecordLevel>0)
            inFilestream.read((char*)(&iTotalThermElecNumber),sizeof(int));
        inFilestream.read((char*)(&iRecordSize),sizeof(int));

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

        // Let the user know if their record level is set wrong to avoid
        // ugly errors later and to suggest corrections.
        // SPOCK I added the && recordLevel part to see I can use thermal e-'s
        if(optPhotRecordLevel <3 && recordLevel <2 && thermElecRecordLevel <3) {
          cerr << 
            "The record level in Bacc has not been set properly. Set a " <<
            "record level\nof at least three in the PMT_PhotoCathode " <<
            "instead of the provided level: "<<
            optPhotRecordLevel << endl;
          exit(0);
        }

        // Make some variables that we'll need for storing the PMT information
        int current_pmt = GetPMTNumber(PMTLookupTable, iVolume);

        // Keep an eye out for starting a new event. if(new_event)
        if(previous_event_number != iEvtNb) {
          new_event = true;
        }

        if(new_event) {
          new_event = false;
          need_answer_key = true;
          timingInfoVec.push_back(vector< vector<double> >() );
          iEvtIndex = timingInfoVec.size() -1;
          timingInfoVec[iEvtIndex].assign( numPmts, vector<double> ());
          if(previous_event_number != -1) { // This avoids the zero-th case.
            keys.push_back(key);
            key.x_scats.clear();
            key.y_scats.clear();
            key.z_scats.clear();
            key.energy_deps.clear();
            for(size_t clear_pmt_i=0; clear_pmt_i<122; clear_pmt_i++)
              key.photons_per_chan[clear_pmt_i] = 0;
          }
          previous_event_number = iEvtNb;
        }


        /*
        if(need_answer_key) {
          need_answer_key = false;
          //answer_key key = build_answer_key(inFilestream, record_start_position);
          // Let's print out the answer key:
          cout << "event_number_Bacc:   " << "\t" << key.event_number_Bacc << endl;
          cout << "event_number_analysis: " << "\t" << key.event_number_analysis << endl;
          cout << "event_caused_trigger:  " << "\t" << key.event_caused_trigger << endl;
          cout << "particle_type:         " << "\t" << key.particle_type << endl;
          cout << "energy:                " << "\t" << key.energy << endl;
          cout << "x,y,z:                 " << "\t" << key.x << " " << key.y << " " << key.z << endl;
          //cout << "scat_x:                " << "\t" << key.scat_x[0] << " " << key.scat_x[1] << " " << key.scat_x[2] << endl;
          //cout << "scat_y:                " << "\t" << key.scat_y[0] << " " << key.scat_y[1] << " " << key.scat_y[2] << endl;
          //cout << "scat_z:                " << "\t" << key.scat_z[0] << " " << key.scat_z[1] << " " << key.scat_z[2] << endl;
          cout << "photons_per_chan[122]: " << "\t" << key.photons_per_chan[0] << " & " <<  key.photons_per_chan[1] << endl;
          cout << "timestamp:             " << "\t" << key.timestamp << endl;
        }
        */
	   
        
        for(int j=0; j<iRecordSize; j++){
            ReadSize();
            char* particleName = new char [Size+1];
            inFilestream.read((char*)(particleName), Size);
            particleName[Size] = '\0';
            ReadSize();
            char* creatorProcessName = new char [Size+1];
            inFilestream.read((char*)(creatorProcessName),Size);
            creatorProcessName[Size] = '\0';
            cout << "creatorProcessName = " << creatorProcessName << endl;
            ReadSize();
            char* stepProcessName = new char [Size+1];
	    if( Size > 1 ) {
                inFilestream.read((char*)(stepProcessName),Size);
                stepProcessName[Size] = '\0';
            }
            cout << "stepProcessName = " << stepProcessName << endl;
            inFilestream.read((char*)(&data),sizeof(data));
            if (DEBUG(5)) {
                cout << "data.stepNumber:\t" << data.stepNumber << endl;
                cout << "data.particleID:\t" << data.particleID << endl;
                cout << "data.trackID:\t" << data.trackID << endl;
                cout << "data.parentID:\t" << data.parentID << endl;
                cout << "data.particleEnergy:\t" << data.particleEnergy << endl;
                if(has_emission_time)
                  cout << "data.particleEnergy:\t" << data.particleEnergy << endl;
                cout << "data.particleDirection[0]:\t" << data.particleDirection[0] << endl;
                cout << "data.energyDeposition:\t" << data.energyDeposition << endl;
                cout << "data.position[0]:\t" << data.position[0] << endl;
                cout << "data.stepTime:\t" << data.stepTime << endl;

            }
            // ========================================================
            // This is where PMT hits are stored for later processing!
            // ========================================================
            if(current_pmt >= 0) {
                // The if statement is a check on if the current volume is
                // a pmt or not.

                // Implement a straggler cut for photons from either photons
                // bouncing around for a long time or from zero field recomb.
                // The max Run3 drift time is ~2.75e5 ns.
                //if(data.stepTime < 1e6)
                timingInfoVec[iEvtIndex][current_pmt].push_back(data.stepTime);
                key.photons_per_chan[current_pmt]++;
            }

            string current_vol_name;
            current_vol_name = get_volume_name(iVolume, volumes, volume_ids);
            /*
            cout << "1) Event num:            " << iEvtNb << endl;
            cout << "2) Volume name:          " << current_vol_name << endl;
            cout << "3) Need answer key:      " << need_answer_key << endl;
            cout << "4) File has Xe rec:      " << has_xenon_records << endl;
            cout << "5) Is xenon vol:         " << is_xenon_vol(current_vol_name) << endl;
            cout << endl;
            */
            // First, the case where Xe records hold the useful info.
            if(has_xenon_records && is_xenon_vol(current_vol_name)) {
              // We are looking to add to the record here.
              //cout << "--@   Xenon     @--" << endl;
              if(!need_answer_key) {
                key.energy += data.energyDeposition;
                key.x_scats.push_back(data.position[0]);
                key.y_scats.push_back(data.position[1]);
                key.z_scats.push_back(data.position[2]);
                key.energy_deps.push_back(data.energyDeposition);
                //cout << "filling up... E=" << key.energy << endl;
              }
              // We build most of the answer key here.
              if(need_answer_key) {
                need_answer_key = false;
                key.event_number_Bacc = iEvtNb;
                key.event_number_analysis = 1;
                key.event_caused_trigger = 1;
                key.particle_type = particleName;
                key.x = data.position[0];
                key.y = data.position[1];
                key.z = data.position[2];
                key.x_scats.push_back(data.position[0]);
                key.y_scats.push_back(data.position[1]);
                key.z_scats.push_back(data.position[2]);
                key.energy = data.energyDeposition;
                key.energy_deps.push_back(data.energyDeposition);
                // Primary particle
                struct primary_particle_spacial_and_energy prim_par;
                prim_par = primary_particles[primary_particles.size() - 1];
                key.prim_particle_type = prim_par.name;
                key.x_prim_par = prim_par.pos[0]/10.; // convert from mm to
                key.y_prim_par = prim_par.pos[1]/10.; // cm for all primary
                key.z_prim_par = prim_par.pos[2]/10.; // prim par dists.
                key.xdir_prim_par = prim_par.dir[0];
                key.ydir_prim_par = prim_par.dir[1];
                key.zdir_prim_par = prim_par.dir[2];
                key.energy_prim_par = prim_par.energy;
                key.timestamp = 1;
              }
            }
            // Second, the case where no Xe records exist and we use primary
            // particle information.
            if(!has_xenon_records && need_answer_key) {
              // Build this from the primary particle info and that's it!
//#primary_particles.push_back(prim_par);
//typedef struct primary_particle_spacial_and_energy {
              struct primary_particle_spacial_and_energy prim_par;
              prim_par = primary_particles[primary_particles.size() - 1];
              need_answer_key = false;
              key.event_number_Bacc = iEvtNb;
              key.event_number_analysis = 2;
              key.event_caused_trigger = 1;
              key.prim_particle_type = prim_par.name;
              key.particle_type = prim_par.name;
              key.energy = prim_par.energy;
              key.x_prim_par = prim_par.pos[0]/10.; // convert from mm to
              key.y_prim_par = prim_par.pos[1]/10.; // cm for all primary
              key.z_prim_par = prim_par.pos[2]/10.; // prim par dists.
              key.x_scats.push_back(key.x);
              key.y_scats.push_back(key.y);
              key.z_scats.push_back(key.z);
              // Primary particle
              key.energy_prim_par = prim_par.energy;
              key.x = prim_par.pos[0]/10.; // convert from mm to
              key.y = prim_par.pos[1]/10.; // cm for all primary
              key.z = prim_par.pos[2]/10.; // prim par dists.
              key.xdir_prim_par = prim_par.dir[0];
              key.ydir_prim_par = prim_par.dir[1];
              key.zdir_prim_par = prim_par.dir[2];
              key.timestamp = 1;
            }

            delete[] creatorProcessName;
            delete[] stepProcessName;
            delete[] particleName;
        }   // End loop over j, iRecordSize
      // In the case of the final event we never see a "new" event!
      if(i==iNumRecords-1)
        keys.push_back(key);
    }   // End loop over j, iNumRecords
    if(UPDATE) 
      cout << "\rReading records from simulation file... Done      " << endl;
    
    ///////////////////////////////////////////
    // We are done reading the file!     //////
    inFilestream.close();                //////
    ///////////////////////////////////////////


    // Dump raw pmt timing info by pmt. The delimiter of pmts is a semicolon
    // and of hits, the comma.
    if(DEBUG(7)) for(int i=0;i<numPmts;i++){
        if(!i) cout << "Raw PMT Dump for event 0 ";
        if(!i) cout << "(t1pmt1,t2pmt1,t3pmt1;t1pmt2,t2pmt2,...)" << endl;
            for(unsigned int j = 0; j<timingInfoVec[0][i].size();j++){
                cout << timingInfoVec[0][i][j] << ",";
                }
                cout << ";";
            }
    // !!! Just some tallying of the sizes of the dimensions for the vector of
    // vectors of something that holds the photon arrival times.
    int phtot=0;
    for(unsigned int e=0; e<timingInfoVec.size(); e++) {
      int phsum=0;
      for(unsigned int c=0; c<timingInfoVec[e].size(); c++) {
        phsum += timingInfoVec[e][c].size();
        phtot += timingInfoVec[e][c].size();
      }
      //cout << "Event: " << e << " \t" << "Photons: " << phsum << endl;
    }

    
    // 
    // We are now left with a single variable with pertinent information. The
    // timingInfoVec contains the hits all ordered by channel. Let's get to
    // processing them.

  // Now let's also look for events that are greater than 10 ms in length.
  // Those long events (well, ones that extend in to the territory of seconds)
  // take many many giga or terabytes of memory to create those giant
  // waveforms.
  // A warning is printed to the user if this occurs.
  int dropped_events_due_to_length = 0;
  for(unsigned int e=timingInfoVec.size()-1; e>0; e--) {
    double min_time = 1e34;
    double max_time = -1e34;
    double temp_time;
    for(int c=0; c<numPmts; c++) {
      if(timingInfoVec[e][c].size() > 0) {
        temp_time = minElement(timingInfoVec[e][c]);
        if(temp_time < min_time) min_time = temp_time;
        temp_time = maxElement(timingInfoVec[e][c]);
        if(temp_time > max_time) max_time = temp_time;
      }
    }
    if( max_time - min_time > 10e9 ) { // 10 seconds in nanoseconds
      dropped_events_due_to_length++;
      timingInfoVec.erase(timingInfoVec.begin()+ e);
      keys.erase(keys.begin() + e);
      //cout << "I think I want to drop answer key " << e+1 << endl;
    }
  }
  if(dropped_events_due_to_length > 0)
    cout << dropped_events_due_to_length
         << " events dropped due to being > 10 sec in length." << endl;


  // 
  // Shift events that happen at time = t0 to time = zero. This might not
  // be necessary since looping is now done over min to max times.
  // 
  
	for(unsigned int e=0; e<timingInfoVec.size(); e++) {
    double min_time = 1e34;
    double temp_time;
    for(int c=0; c<numPmts; c++) {
      if(timingInfoVec[e][c].size() > 0) {
        temp_time = minElement(timingInfoVec[e][c]);
        if(temp_time < min_time) min_time = temp_time;
      }
    }
    for(int c=0; c<numPmts; c++) {
      for(unsigned int s=0; s < timingInfoVec[e][c].size(); s++){
        timingInfoVec[e][c][s] -= min_time;
      }
    }
  }

    //
    // The first thing to do is to switch the order of the PMTs. Since Bacc
    // uses a "grid" of pmt numbers and the LUX detector uses a "pizza"
    // arrangement (for saturation and trigger purposes), we must translate
    // the pmts to the proper order.
    //

  if(oldPMTStyle){
    int pmt_relations[numPmts];
    createPositionLookup(pmt_relations);

    vector< vector< vector<double> > > timingInfoVec_switcher;
    timingInfoVec_switcher = timingInfoVec;
    // Loop over each event. timingInfoVec.size() is the number of events.
    for(unsigned int e=0; e<timingInfoVec.size(); e++) {
      // Loop over all numPmts channels. There are exactly numPmts channels,
      // even if it is empty of photons.
      for(int c=0; c<numPmts; c++) {
        int chan_to_use = pmt_relations[c];
        timingInfoVec[e][chan_to_use] = timingInfoVec_switcher[e][c];
      }
    }
    //timingInfoVec_switcher.clear();
  }

	
	

    if(DEBUG(11)) {
        cout << endl << "Time series (in ns) for a selection of pmts." << endl;
        int pmtSelection[5] = {1, 30, 31, 32, 56}; // Which pmts to view.
        unsigned long maxVecSize = 0;
        for(int p=0; p<5; p++){     // Get the longest column
            if(timingInfoVec[0][pmtSelection[p]].size() > maxVecSize) 
                maxVecSize = timingInfoVec[0][pmtSelection[p]].size();
        }
        cout << "pmt " << pmtSelection[0] << "\t\tpmt " << pmtSelection[1] 
             << "\t\tpmt " << pmtSelection[2] << "\t\t" << "pmt " 
             << pmtSelection[3] << "\t\tpmt " << pmtSelection[4] << endl;

        for(unsigned int e=0; e<maxVecSize; e++){
            try {cout << setprecision(3) <<
                timingInfoVec[0][pmtSelection[0]].at(e) << "\t\t";}
            catch (exception &e) {cout << "\t\t";}
            try {cout << setprecision(3) <<
                timingInfoVec[0][pmtSelection[1]].at(e) << "\t\t";}
            catch (exception &e) {cout << "\t\t";}
            try {cout << setprecision(3) <<
                timingInfoVec[0][pmtSelection[2]].at(e) << "\t\t";}
            catch (exception &e) {cout << "\t\t";}
            try {cout << setprecision(3) <<
                timingInfoVec[0][pmtSelection[3]].at(e) << "\t\t";}
            catch (exception &e) {cout << "\t\t";}
            try {cout << setprecision(3) <<
                timingInfoVec[0][pmtSelection[4]].at(e) << "\t\t";}
            catch (exception &e) {cout << "\t\t";}
            cout << " " << endl;
        }
    }


    // =================================================================== //
    // SINGLE PHOTOELECTRON NOISE                                          //
    //     We have calculated single photoelectron rates in the            //
    //     detector. Here, I will sprinkle those photons across this       //
    //     event.                                                          //
    //                                                                     //
    // =================================================================== //
    //
    // Using eb14 for pretrigger and posttrigger
    // Watch out for the above preTrigger definition
    int pretrigger = 50000;     // samples
    int posttrigger = 50000;    // samples
    pretrigger *= 10;   // Convert to ns
    posttrigger *= 10;  // Convert to ns

    if(UPDATE) cout << "Starting sphe sprinkling... "; cout.flush();
    double global_sphe_rate = 2.25;  // Hz per chan. J. Cutter plot, May 17, 2013
    global_sphe_rate *= 1e-9;       // Convert to phe/ns.


    //===========================================================||           
    //          S1                           S2                  ||
    //  pretrig                                   posttrig       ||
    //  |----|                                    |-------|      ||
    //                                      /`\                  ||
    //       ___/\__  __|__      __|__  ___/   \__   __|__       ||
    //                                                           ||
    //                  spe       spe                 spe        ||           
    //                                                           ||           
    //===========================================================||           
    for(unsigned int e=0; e<timingInfoVec.size(); e++) {
      double min_time = 1e34;
      double max_time = -1e34;
      double temp_time;
      for(int c=0; c<numPmts; c++) {
        if(timingInfoVec[e][c].size() > 0) {
          temp_time = minElement(timingInfoVec[e][c]);
          if(temp_time < min_time) min_time = temp_time;
        }
        if(timingInfoVec[e][c].size() > 0) {
          temp_time = maxElement(timingInfoVec[e][c]);
          if(temp_time > max_time) max_time = temp_time;
        }
      }
      double event_window = (max_time - min_time) + (posttrigger + pretrigger);
      //cout << event_window << "\t" << global_sphe_rate << "\t" << event_window * global_sphe_rate << endl;
      //cout << endl << "num_sphe: " << num_sphe << "\tUnfluctuated num was: " << event_window * global_sphe_rate<< endl;
      
      for(int c=0; c<numPmts; c++) {
        int num_sphe = Poisson(event_window * global_sphe_rate);
        //cout << "num_sphe: " << num_sphe << "\t" << event_window << endl;
        for(int ph=0; ph<num_sphe; ph++) {
          double rand_time = Rand() * event_window + min_time - pretrigger;
          timingInfoVec[e][c].push_back(rand_time);
        }
      }
    }
    if(UPDATE) cout << "Done." << endl;


    // =================================================================== //
    // TRIGGER SIMULATION --- Phase 1                                      //
    //     The first phase of trigger simulation is to veto events that    //
    //     have too few numbers of photons in specified groups of pmts.    //
    //                                                                     //
    // =================================================================== //

    Bacc2evtTrigger * the_trigger = new Bacc2evtTrigger();
    // Just showing how to disable/enable the trigger.
    the_trigger->DisableTrigger();
    //the_trigger->EnableTrigger();
    for(unsigned int event=0; event<timingInfoVec.size(); event++) {
      if(UPDATE) {
        cout << "\r" << "Triggering: Pt. 1 - Event " << 
        event+1 << " of " << timingInfoVec.size() << " ";
      }
      bool triggered = the_trigger->Phase1Trigger(timingInfoVec[event], event);
      triggered = triggered;

      if(UPDATE && event == timingInfoVec.size()-1) {
        cout << "- Trigger dropped " << the_trigger->GetDroppedEvents().size();
        cout << " events.";
      }
      if(UPDATE) cout.flush();
    }
    if(UPDATE) cout << endl;


    // =================================================================== //
    // PULSE RESPONSE & DIGITIZATION                                       //
    //     Creating a pulse response and digitizing it. The digitzer       //
    //     samples every 10 ns (100 MHz frequency), but due to the         //
    //     pulse shaping it sample at about ever ~6 ns. Pulses are         //
    //     digitized by the 14-bit Struck in the range +100 mV to          //
    //     -1900 mV at 122.07 mV/ADC.                                      //
    //                                                                     //
    // =================================================================== //

    // Create a random offset to the digitization (so that we don't always
    // digitize the peak of the gaussian, for example.
    double rTimeOffset = GetOffset() * 1./100e6 * 1e9 /* 1.000 ns */;

    // Here's a vector (events) of vectors (channels) of Pulses.
    unsigned int numEvts_from_geant = timingInfoVec.size();
    vector< vector<Bacc2evtPulse*> > raw_pmt_pulses2;
    raw_pmt_pulses2.assign(numEvts_from_geant, vector<Bacc2evtPulse*> () );
    vector< vector< vector<Bacc2evtPulse*> > > pod_pulses2;
    pod_pulses2.assign(numEvts_from_geant,vector< vector<Bacc2evtPulse*> >());
    for(unsigned int event=0; event<numEvts_from_geant; event++) {
      if(UPDATE) cout << "\r" << "Generating Pulses - Event " << event+1 << " of " << numEvts_from_geant << " ";
      if(UPDATE) cout.flush();
      pod_pulses2[event].assign(numPmts, vector<Bacc2evtPulse*>() );
      for(int chnl=0; chnl < numPmts; chnl++) {
            vector<double> hit_vector = timingInfoVec[event][chnl];
            Bacc2evtPulse* pulse = new Bacc2evtPulse();
            pulse->SetLocalEventID(event);
            pulse->SetTimeOffset(rTimeOffset);
            pulse->SetChannelNumber(chnl);
            pulse->SetPulseLength(0);
            pulse->SetPulseNumber(0);
            if(useXMLGains){
              if(xmlPMTs[chnl] == chnl + 1){ //Set the gain
                pulse->SetGain(xmlGains[chnl]);
                pulse->SetSpheSigma(xmlGainVariations[chnl]);
              }
              else{
                cout << endl << "Couldn't find gain for PMT " << chnl+1 << endl;
                exit(1);
              }
              if(DEBUG(13)){
                cout << "Starting pulse generation for channel " << chnl+1;
                cout << "...";
                cout.flush();
              }
            }
            else{
              pulse->SetGain(16); // mVns
              pulse->SetSpheSigma(7.5); // Since the size of a sphe varies 
              // normally about a mean ~16 mVns and width ~7.5 mVns for the 
              // 4e6 gain (see Franks UCSB collb mtg talk). 
            }
            // --------------------------------------------------------- //
            // THIS IS THE MEAT                                          //
            //     This is where waveforms are generated.                //
            // --------------------------------------------------------- //
            if(hit_vector.size() > 0) {
              // Optimizations are the pre-POD separation of photon clusters.
              //bool enable_optimizations = false;
              bool enable_optimizations = true;
              if(enable_optimizations){
                vector<vector<double> > pre_pods = pulse->GeneratePrePods(hit_vector);
                vector<Bacc2evtPulse*> current_pulse;
                for(size_t pre_pod=0; pre_pod<pre_pods.size(); pre_pod++) {
                  pulse->GeneratePulseVec(pre_pods[pre_pod]);
                  vector<Bacc2evtPulse*> temp_pulses = pulse->SeparatePulses();
                  for(size_t p=0; p<temp_pulses.size(); p++) {
                    current_pulse.push_back(temp_pulses[p]);
                  }
                }
                delete pulse;
                hit_vector.clear();
                vector<double>().swap(hit_vector);
                pod_pulses2[event][chnl] = current_pulse;
                for(size_t writer=0; writer<current_pulse.size(); writer++) {
                  current_pulse[writer]->SetPulseNumber(writer);
                }
              }
              else{
                sort(hit_vector.begin(), hit_vector.end());
                pulse->GeneratePulseVec(hit_vector);
                vector<Bacc2evtPulse*> current_pulse;
                current_pulse = pulse->SeparatePulses();
                delete pulse;
                hit_vector.clear();
                vector<double>().swap(hit_vector);
                pod_pulses2[event][chnl] = current_pulse;
                for(size_t writer=0; writer<current_pulse.size(); writer++) {
                  current_pulse[writer]->SetPulseNumber(writer);
                }
              }
            }

            if(DEBUG(13)) cout << "Done!" << endl;
      }
    }
    if(UPDATE) cout << endl;

    // =================================================================== //
    // PULSE SEPARATION (POD MODE)                                         //
    //     We now need to find the individual pulses in the "raw pmt       //
    //     pulse" response. This is an implementation of the POD (pulse    //
    //     only digitization) mode. SeparatePulses returns a vector of     //
    //     Bacc2evtPulse instances. Each instance is a POD               //
    // =================================================================== //

    // The updated storage device.
    // A vector(events) of vectors (channels) of vectors (pulses).
    //vector< vector< vector<Bacc2evtPulse*> > > pod_pulses2;
    //pod_pulses2.assign(numEvts_from_geant,vector< vector<Bacc2evtPulse*> >());
    ///*
    /*
    for(unsigned int event=0; event<numEvts_from_geant; event++) {
      if(UPDATE) cout << "\r" << "Separating Pulses - Event " 
        << event+1 << " of " << numEvts_from_geant << " ";
      if(UPDATE) cout.flush();
      pod_pulses2[event].assign(numPmts, vector<Bacc2evtPulse*>() );
      for(int chnl=0; chnl<numPmts; chnl++) {
        vector<Bacc2evtPulse*> current_pulse;
        current_pulse = raw_pmt_pulses2[event][chnl]->SeparatePulses();
        pod_pulses2[event][chnl] = current_pulse;
        for(size_t writer=0; writer<current_pulse.size(); writer++) {
          current_pulse[writer]->SetPulseNumber(writer);
          //current_pulse[writer]->Write();   
        }
        //pod_pulses2[event][chnl].push_back(raw_pmt_pulses2[event][chnl]->SeparatePulses());
      }
    }
    if(UPDATE) cout << endl;
    */


    // Let's free up some memory because we are at the point that we have our
    // POD separated pulses. We can delete the photon arrival times and we can
    // delete the raw waveforms. If things get intense with memory, these can
    // be put in the loops where they're used so that immediately after
    // conversion from one container to the next form in the next container the
    // (sub)vector is deleted.
    timingInfoVec.clear();
    for(size_t i_del1=0; i_del1<raw_pmt_pulses2.size(); i_del1++) {
      for(size_t i_del2=0; i_del2<raw_pmt_pulses2[i_del1].size(); i_del2++)
        delete raw_pmt_pulses2[i_del1][i_del2];
    }
    raw_pmt_pulses2.clear();

    // =================================================================== //
    // TRIGGER SIMULATION --- Phase 2                                      //
    //     The second phase of trigger simulation is now going to look at  //
    //     pulse shapes and make a decision.                               //
    //                                                                     //
    // =================================================================== //

    // Just showing how to disable/enable the trigger.
    the_trigger->DisableTrigger();
    //the_trigger->EnableTrigger();
    //vector< vector< vector<Bacc2evtPulse*> > > pod_pulses2;
    for(unsigned int event=0; event<pod_pulses2.size(); event++) {
      if(UPDATE) {
        cout << "\r" << "Triggering: Pt. 2 - Event " << 
        event+1 << " of " << pod_pulses2.size() << " ";
      }
      bool triggered = the_trigger->Phase2Trigger(pod_pulses2[event], event);
      triggered = triggered;

      // Last loop: print out how many events did not satisfy trigger conds.
      if(UPDATE && event == pod_pulses2.size()-1) {
        cout << "- Trigger dropped " << the_trigger->GetDroppedEvents().size();
        cout << " events.";
      }
      if(UPDATE) cout.flush();
    }
    if(UPDATE) cout << endl;

    // Drop all the deleted events.
    for(unsigned int event=0; event<the_trigger->GetDroppedEvents().size(); event++) {
      for(unsigned int pmt=0; pmt<pod_pulses2[event].size(); pmt++) {
        for(unsigned int pod=0; pod<pod_pulses2[event][pmt].size(); pod++) {
          delete pod_pulses2[event][pmt][pod];
          pod_pulses2[event][pmt].erase( pod_pulses2[event][pmt].begin() + pod);
        }
      }
    }

    // Do a check against the number of events from Geant vs. number of answer
    // keys as a sanity check. I believe the logic for correlating this is
    // sound but without an appropriate sim (hard to generate purposefully)
    // it is difficult to test.
    if(numEvts_from_geant != keys.size() ) {
      cerr << "** [WARNING] There are " << numEvts_from_geant << " events ";
      cerr << "from Bacc and " << keys.size() << " answer keys. TELL MIKE ";
      cerr << " immediately!" << endl;
    }

    // =================================================================== //
    // WRITE TO .evt FILE                                                  //
    //     Source: LUX Event Builder Data Format PDF v5.1 - JC             //
    //                                                                     //
    // =================================================================== //
        

    // A few hanging variable declarations.
    signed char binDataType = 14; // I have no idea.. from EVT builder
    double voltageRes = 2./16383; // 2volts/(2**14-1) ADC
    double voltageOff = 0;
    double timeRes = 1./(100e6);
    int preTrigger = 50000;
    unsigned int eventSize_for_header = 0;     // Ugh.. not sure
    unsigned int pulseDetect = 24;
    unsigned int pulseEnd = 31;

    // The locations of each event in the file. Will be written to disk
    // just before closing the file.
    vector<unsigned int> EventByteLocs;
    unsigned int fileHeaderLocation;


    string tmpxmlheader = 
    "<?xml version=\"1.0\"?>\n\
    <evt_settings>\n\
      <event_builder_version>9.0</event_builder_version>\n\
      <pretrigger>50000</pretrigger>\n\
      <posttrigger>50000</posttrigger>\n\
    </evt_settings>\n\
    <?xml version=\"1.0\"?>\n\
    <daq_settings>\n\
      <global>\n\
        <filename_prefix>luxsm_20120000T0000</filename_prefix>\n\
        <source>BG</source>\n\
        <notes>Bacc Output Simulation</notes>\n\
        <preamp>5</preamp>\n\
        <postamp>1.5</postamp>\n\
        <daq_version>7.1</daq_version>\n\
      </global>\n\
      <sis3301>\n\
        <global>\n\
          <acq_mode>multi</acq_mode>\n\
          <nb_evts>100</nb_evts>\n\
          <nb_chs>136</nb_chs>\n\
          <chs>0:135</chs>\n\
          <trig_ch>127</trig_ch>\n\
          <data_chs>0:121</data_chs>\n\
          <pmt_chs>1:122</pmt_chs>\n\
          <pulse_detect_pretrigger>24</pulse_detect_pretrigger>\n\
          <pulse_end_posttrigger>31</pulse_end_posttrigger>\n\
          <pulse_thresh_detect>1.5</pulse_thresh_detect>\n\
          <pulse_end_thresh>0.5</pulse_end_thresh>\n\
          <pulse_overshot_thresh>1000</pulse_overshot_thresh>\n\
          <baseline_average_samples>32</baseline_average_samples>\n\
          <sampling_freq>100000000</sampling_freq>\n\
          <vrange_top>0.1</vrange_top>\n\
          <vrange_bot>-1.9</vrange_bot>\n\
          <delay_buffer>254</delay_buffer>\n\
          <vptg>1</vptg>\n\
          <save_pair>1</save_pair>\n\
          <read_xlm>1</read_xlm>\n\
          <xenon_daq_chs>1:122</xenon_daq_chs>\n\
          <water_daq_chs>129:136</water_daq_chs>\n\
          <xenon_pmt_map>1:122</xenon_pmt_map>\n\
          <water_pmt_map>129:136</water_pmt_map>\n\
        </global>\n\
      </sis3301>\n\
    </daq_settings>\n";
/*
    </daq_settings>\n\
    <daq_settings>\n\
    <!-- This is a settings file -->\n\
        <global>\n\
            <notes>Bacc Output Simulation</notes>\n\
            <daq_version>-1.0</daq_version>\n\
            <event_builder_version>-1.0</event_builder_version>\n\
            <data_chs>0:121</data_chs> \n\
        </global>\n\
        <sis3301>\n\
            <global>\n\
                <data_chs>0:121</data_chs>\n\
                <pmt_chs>1:122</pmt_chs>\n\
                <nb_chs>122</nb_chs>\n\
            </global>\n\
        </sis3301>\n\
*/
   
    tmpxmlheader +=  "\t<simulation>\n";
    tmpxmlheader +=  "\t\t<binary_source>";
    tmpxmlheader +=  inFilename;
    tmpxmlheader +=  "</binary_source>\n";
    tmpxmlheader +=  "\t\t<production_time>";
    tmpxmlheader +=  productionTime;
    tmpxmlheader +=  "</production_time>\n";
    //tmpxmlheader +=  "\t\t<Geant4_version>";
    //tmpxmlheader +=  geant4Version;
    //tmpxmlheader +=  "</Geant4_version>\n";
    //tmpxmlheader +=  "\t\t<Bacc_version>";
    //tmpxmlheader +=  svnVersion;
    //tmpxmlheader +=  "</Bacc_version>\n";
    //tmpxmlheader +=  "\t\t<code_diffs>";
    //tmpxmlheader +=  diffs;
    //tmpxmlheader +=  "</code_diffs>\n";
    //tmpxmlheader +=  "\t\t<inputCommands>";
    //tmpxmlheader +=  inputCommands; 
    //tmpxmlheader +=  "</inputCommands>\n";
    tmpxmlheader +=  "\t</simulation>\n";

    delete[] productionTime;

        
    unsigned int Endianness = 0x01020304;
    evtfile.write((char*) &Endianness, sizeof(unsigned int));     // Endianness
    if(DEBUG(17)) cout << "Endianness:\t" << Endianness << endl;

    unsigned int xmlLength = (unsigned int) tmpxmlheader.length();
    evtfile.write((char*) &xmlLength, sizeof(xmlLength));
    if(DEBUG(17)) cout << "XML Length:\t" << xmlLength << endl;
    evtfile.write(tmpxmlheader.c_str(), tmpxmlheader.length());
    if(DEBUG(17)) cout << tmpxmlheader << endl;

    // File Header[5]
    // The eventPosition is written assuming one sequence. The current position
    // is advanced the size of eventPosition, numSeqs, TSLatch, and TSEnd.
    evtfile.write((char*) &Endianness, sizeof(unsigned int));     // Endianness
    if(DEBUG(17)) cout << "Endianness:\t" << Endianness << endl;
    unsigned int dateTime = atoi(date_string.c_str());
    evtfile.write((char*) &dateTime, sizeof(dateTime));
    if(DEBUG(17)) cout << "Date/Time:\t" << dateTime << endl;

    // This is the flag that now indicates whether the .evt file has come from
    // simulation and has the monte carlo truth values at the end of the
    // evt structure.
    unsigned int is_Bacc = 1;
    evtfile.write((char*) &is_Bacc, sizeof(is_Bacc));
    if(DEBUG(17)) cout << "Is Bacc evt:\t" << is_Bacc << endl;
    unsigned int numEvents = numEvts_from_geant;
    evtfile.write((char*) &numEvents, sizeof(numEvents));
    if(DEBUG(17)) cout << "Num Events:\t" << numEvents << endl;

    // Note the file pos for later filling.
    fileHeaderLocation = evtfile.tellp();
    unsigned int tempEventByteLoc = 0;
    for(unsigned int tempEventNum=1; tempEventNum<numEvents+1; tempEventNum++){
        evtfile.write((char*) &tempEventNum, sizeof(tempEventNum));
        evtfile.write((char*) &tempEventByteLoc, sizeof(tempEventByteLoc));
        if(DEBUG(17))
            cout << "Event Number (file position):\t" << tempEventNum << " ("
                << tempEventByteLoc << ")" << endl;
    }


    // Live Time Header
    unsigned short numSeqs = 1;             // sequences are Struck mem buffers
    evtfile.write((char*) &numSeqs, sizeof(short));
    if(DEBUG(17)) cout << "Num Seqs:\t" << numSeqs << endl;

    unsigned long long TSLatch[numSeqs];    // Timestap latch/end pair. Time of
    unsigned long long TSEnd[numSeqs];      // the begin/end of acquisition.
    if(DEBUG(17)) cout << "Timestamp Latches and Ends" <<  endl;

    for(unsigned short s=0; s<numSeqs; s++) {
        TSLatch[s] =13408765;           // These two are stolen from
        TSEnd[s] =123286566;         // another .evt 
        evtfile.write((char*) TSLatch+s, sizeof(unsigned long long));
        evtfile.write((char*) TSEnd + s, sizeof(unsigned long long));
        if(DEBUG(17)) cout << TSLatch[s] << "\t" << TSEnd[s] << endl;
    }


    // Event GID

    for(unsigned int e=0; e<numEvents; e++){
        EventByteLocs.push_back(evtfile.tellp());
        if(UPDATE) cout << "\r" << "Writing to Disk   - Event " << e+1 ;
        if(UPDATE) cout << " of " << numEvents << "." << " ";
        if(UPDATE) cout.flush();
        if(DEBUG(17)) cout << "Actual Event Position:\t" << evtfile.tellp() << endl;
        unsigned int eDateTime = dateTime;
        evtfile.write((char*) &eDateTime, sizeof(eDateTime));
        if(DEBUG(17)) cout << "Date/Time:\t" << dateTime << endl;

        unsigned int eLocation = 0;
        evtfile.write((char*) &eLocation, sizeof(eLocation));
        if(DEBUG(17)) cout << "Location:\t" << eLocation << endl;

        unsigned int eventNum = e+1;
        evtfile.write((char*) &eventNum, sizeof(eventNum));
        if(DEBUG(17)) cout << "Event Number:\t" << eventNum
            << " of " << numEvents << endl;

        unsigned int numChannels = numPmts;
        evtfile.write((char*) &numChannels, sizeof(numChannels));
        if(DEBUG(17)) cout << "Num of Chans:\t" << numChannels << endl;

        unsigned int eventSize_for_GID = 0;
        evtfile.write((char*) &eventSize_for_GID, sizeof(eventSize_for_GID));
        if(DEBUG(17)) cout << "Event Size:\t" << eventSize_for_GID << endl;

        // ================================================================
        // This info isn't used in the pre-v5.0 version of event builder.
        // ================================================================
        // Trigger Info.
        unsigned long long ddcTimestamp = 0;
        evtfile.write((char*) &ddcTimestamp, sizeof(ddcTimestamp));
        if(DEBUG(17)) cout << "DDC Trig Timestamp:\t" << ddcTimestamp << endl;

        unsigned int ddcSeqNum = 0;
        evtfile.write((char*) &ddcSeqNum, sizeof(ddcSeqNum));
        if(DEBUG(17)) cout << "DDC Trig SequenceN:\t" << ddcTimestamp << endl;

        unsigned int maxFilterResp = 0;
        evtfile.write((char*) &maxFilterResp, sizeof(maxFilterResp));
        if(DEBUG(17)) cout << "Max Filter Response:\t" <<maxFilterResp << endl;

        signed char maxChanID = 0; 
        evtfile.write((char*) &maxChanID, sizeof(maxChanID));
        if(DEBUG(17)) cout << "Max Channel ID:\t" << maxChanID << endl;

        unsigned short numDDCBoards = 1;
        evtfile.write((char*) &numDDCBoards, sizeof(numDDCBoards));
        if(DEBUG(17)) cout << "Number of DDC Boards:\t" << numDDCBoards<< endl;

        for(unsigned short board=0; board < numDDCBoards; board++) {
            signed char s1_hit_vector = 1;
            signed char s2_hit_vector = 1;
            evtfile.write((char*) &s1_hit_vector, sizeof(s1_hit_vector));
            evtfile.write((char*) &s2_hit_vector, sizeof(s2_hit_vector));
            if(DEBUG(17)) cout << "\tBoard " << board << ":\t";
            if(DEBUG(17)) cout << "\tS1 Hit Vector:\t" << s1_hit_vector<< endl;
            if(DEBUG(17)) cout << "\tS2 Hit Vector:\t" << s2_hit_vector<< endl;
        }

        signed char DDCcheckbyte = 0; 
        evtfile.write((char*) &DDCcheckbyte, sizeof(DDCcheckbyte));
        if(DEBUG(17)) cout << "DDC Check byte:\t" << DDCcheckbyte << endl;

        // Record Info

        unsigned int recordForm = 0;
        evtfile.write((char*) &recordForm, sizeof(recordForm));
        if(DEBUG(17)) cout << "Record Format:\t" << recordForm << endl;

        unsigned int recordSize = 0;
        evtfile.write((char*) &recordSize, sizeof(recordSize));
        if(DEBUG(17)) cout << "Record Size:\t" << recordSize << endl;

        // Event Trigger Timestamp

        unsigned long long timeStamp = 0;
        evtfile.write((char*) &timeStamp, sizeof(timeStamp));
        if(DEBUG(17)) cout << "TimeStamp:\t" << timeStamp << endl;

        for(int c=0; c<numPmts; c++){
            // This will continue the loop to make sure we only write
            // pulses that have data in them but also to make sure the
            // c still means the right pmt.

            if(DEBUG(17)) cout << "Starting channel:\t" << c+1 << endl;

            evtfile.write((char*) &binDataType, sizeof(binDataType));
            if(DEBUG(17)) cout << "Binary Data Type:\t" << binDataType<< endl;

            evtfile.write((char*) &voltageRes, sizeof(voltageRes));
            if(DEBUG(17)) cout << "Voltage Res:\t" << voltageRes << endl;

            evtfile.write((char*) &voltageOff, sizeof(voltageOff));
            if(DEBUG(17)) cout << "Voltage Off:\t" << voltageOff << endl;

            evtfile.write((char*) &timeRes, sizeof(timeRes));
            if(DEBUG(17)) cout << "Time Res:\t" << timeRes << endl;

            evtfile.write((char*) &preTrigger, sizeof(preTrigger));
            if(DEBUG(17)) cout << "Pretrigger:\t" << preTrigger << endl;

            evtfile.write((char*) &eventSize_for_header, 
                                        sizeof(eventSize_for_header));
            if(DEBUG(17)) cout << "Event Size (Ch Hdr):\t" << 
                                        eventSize_for_header << endl;

            evtfile.write((char*) &pulseDetect, sizeof(pulseDetect));
            if(DEBUG(17)) cout << "Pulse Detect:\t" << pulseDetect << endl;

            evtfile.write((char*) &pulseEnd, sizeof(pulseEnd));
            if(DEBUG(17)) cout << "Pulse End:\t" << pulseEnd << endl;

            //vector<Bacc2evtPulse*> pod_pulse = pod_pulses[c];
            vector<Bacc2evtPulse*> pod_pulse = pod_pulses2[e][c];
            //unsigned int num_pulses = pod_pulses[c].size();
            unsigned int num_pulses = pod_pulse.size();
            evtfile.write((char*) &num_pulses, sizeof(num_pulses));
            if(DEBUG(17)) cout << "Num Pulses:\t" << num_pulses << endl;

            // Per channel waveforms

            for(unsigned int p=0; p<num_pulses; p++){
              //signed int pulse_start = pod_pulses[c][p]->GetPulseStart();
                signed int pulse_start = pod_pulse[p]->GetPulseStart();
                    evtfile.write((char*)&pulse_start,sizeof(pulse_start));
                    if(DEBUG(19))
                        cout<<"\tChan "<<c+1<<" Pulse Start:\t"<< pulse_start<<endl;
            }

            for(unsigned int p=0; p<num_pulses; p++){
              //unsigned int pulse_length = pod_pulses[c][p]->GetPulseLength();
              unsigned int pulse_length = pod_pulse[p]->GetPulseLength();
              evtfile.write((char*)&pulse_length,sizeof(pulse_length));
              if(DEBUG(19))
                cout<<"\tChan "<<c+1<<" Pulse Length:\t"<<
                  pulse_length<<endl;
            }

            for(unsigned int p=0; p<num_pulses; p++){
	      //unsigned int pulse_baseline = pod_pulses[c][p]->GetBaseline();
                unsigned int pulse_baseline = pod_pulse[p]->GetBaseline();
                evtfile.write((char*)&pulse_baseline,
                    sizeof(pulse_baseline));
                if(DEBUG(19))
                    cout<<"\tChan "<<c+1<<" Pulse Baseline:\t"<<
                        pulse_baseline<<endl;
            }


            for(unsigned int p=0; p<num_pulses; p++){
              //unsigned int pulse_length = pod_pulses[c][p]->GetPulseLength();
                unsigned int pulse_length = pod_pulse[p]->GetPulseLength();
                //vector<unsigned short> data = pod_pulses[c][p]->GetData();
                vector<unsigned short>& data = pod_pulse[p]->GetData();
                for(unsigned long long dp=0; dp<pulse_length; dp++) {
                    evtfile.write( (char*)&data[dp], sizeof(unsigned short));
                    if(DEBUG(23)) cout << data[dp] << ",";
                }
                if(DEBUG(23)) cout << ";";
            }
	    if(DEBUG(23)) cout << endl;

            if(DEBUG(29)) {
                for(unsigned int p=0; p<num_pulses; p++){
                    // Need to get the start time.
                    // These commened out lines are from the old one event/.bin
                    // style. Delete them before release!
                    //unsigned int pulse_baseline = pod_pulse[p]->GetBaseline();
                    unsigned int pulse_length = pod_pulse[p]->GetPulseLength();
                    int pulse_start = pod_pulse[p]->GetPulseStart();
                    vector<unsigned short>& data = pod_pulse[p]->GetData();
                    for(unsigned long long dp=0; dp<pulse_length; dp++) {
                        double scaled_dp = (double) data[dp];
                        //scaled_dp -= (double) pulse_baseline;
                        ////scaled_dp -= 0.5;
                        //scaled_dp *= (2000. / 16383.);
                        ////scaled_dp *= (2000. / 16384.);
                        //scaled_dp *= 10.; // ns
                        //scaled_dp /= ( 1e12 * 2e6 * 1.6e-19 * 25. * 1.5 * 5. );
                        //scaled_dp *= -1.;

                        ////cerr << pulse_start << endl;
                        ////cout<<"(" << (pulse_start + dp)*10.000000000 << ","
                        ////cerr<<data[dp] << endl;
                        cout<<"(" << (pulse_start + dp) << ","
                            << scaled_dp << ");";
                    }
                    cout << ":";
                }
                if(DEBUG(29)) cout << "@";
            }
        }
        if(DEBUG(29)) cout << "!";
    }

    // Free up some more memory. This deletes the series of POD pulses
    // since they have already been writtent to disk.
    for(size_t i_del1=0; i_del1<pod_pulses2.size(); i_del1++) {
      for(size_t i_del2=0; i_del2<pod_pulses2[i_del1].size(); i_del2++)
        for(size_t i_del3=0; i_del3<pod_pulses2[i_del1][i_del2].size(); i_del3++)
          {}
          //delete pod_pulses2[i_del1][i_del2][i_del3];
    }
    pod_pulses2.clear();

    if(UPDATE) cout << endl;

    // The writing of the evt file has finished. However, now we need to go
    // back in through the file and write out the even positions in the
    // right spots.

    // Go to the start of event number/byte location of gid pairs.
    size_t current_loc = evtfile.tellp();
    evtfile.seekp( fileHeaderLocation );
    for(unsigned int event_num=1; event_num<numEvents+1; event_num++){
        evtfile.write((char*)&event_num,sizeof(event_num));
        evtfile.write((char*) &EventByteLocs[event_num-1], 
            sizeof(EventByteLocs[event_num-1]));
    }
    evtfile.seekp(current_loc);


    // Now let's write the monte carlo truth values to the evt file.
    //
    // First, let's calculate some extra variables.
    unsigned int num_answer_keys = keys.size();
    for(unsigned int key_index = 0; key_index<num_answer_keys; key_index++) {
      answer_key& a_key = keys[key_index];   // Pointer so we can edit keys.
      a_key.key_index = key_index;
      a_key.msd = calculate_max_scattering_distance(a_key);  
      size_t num_scats = a_key.energy_deps.size();
      for(unsigned int scat_ind = 0; scat_ind<num_scats; scat_ind++) {
        double x = a_key.x_scats[scat_ind];
        double y = a_key.y_scats[scat_ind];
        double z = a_key.z_scats[scat_ind];
        bool in_active_region = is_in_active_region(x,y,z);
        if(!in_active_region) {
          a_key.missing_energy += a_key.energy_deps[scat_ind];
        }
      }
      if(a_key.missing_energy < 1e-15) a_key.missing_energy = 0;
    }

    /********************************************************************
     * PRINT OUT THE ANSWER KEYS!
    ********************************************************************/
    if(DEBUG(37)) {
      //cout << "There are " << keys.size() << " keys and " << primary_particles.size() << " primary particles." << endl;
      for(size_t key_index=0; key_index<keys.size(); key_index++) {
        key = keys[key_index];
        cout << "************************************" << endl;
        cout << "Answer Key " << key.key_index << endl;
        cout << "Address of this key:   " << "\t" << &key << endl;
        cout << "event_number_Bacc:   " << "\t" << key.event_number_Bacc << endl;
        cout << "event_number_analysis: " << "\t" << key.event_number_analysis << endl;
        cout << "event_caused_trigger:  " << "\t" << key.event_caused_trigger << endl;
        cout << "prim_particle_type:    " << "\t" << key.prim_particle_type << endl;
        cout << "energy_prim_par:       " << "\t" << key.energy_prim_par << endl;
        cout << "prim_particle_x,y,z:   " << "\t" << 
          key.x_prim_par << " " << key.y_prim_par << " " << key.z_prim_par << endl;
        cout << "prim_par_dir_x,y,z:    " << "\t" << 
          key.xdir_prim_par << " " << key.ydir_prim_par << " " << key.zdir_prim_par << endl;
        cout << "particle_type:         " << "\t" << key.particle_type << endl;
        cout << "energy:                " << "\t" << key.energy << endl;
        cout << "x,y,z:                 " << "\t" << key.x << " " << key.y << " " << key.z << endl;
        cout << "Missing energy:        " << "\t" << key.missing_energy << endl;
        cout << "Number of x scatters:  " << "\t" << key.x_scats.size() << endl;
        cout << "Number of y scatters:  " << "\t" << key.y_scats.size() << endl;
        cout << "Number of z scatters:  " << "\t" << key.z_scats.size() << endl;
        cout << "photons_per_chan[122]: " << "\t" << 
          key.photons_per_chan[0] << " & " <<  key.photons_per_chan[1] << " -----> " << 
          key.photons_per_chan[120] << " & " << key.photons_per_chan[121] << endl;
        cout << "timestamp:             " << "\t" << key.timestamp << endl;
        cout << "************************************" << endl;
        cout << endl;
        cout << endl;
      }
    }
    /********************************************************************
    ********************************************************************/
    evtfile.write((char*) &num_answer_keys, sizeof(num_answer_keys));
    // Write byte locations
    // First, remember where this is.
    long long start_of_byte_locs = evtfile.tellp();
    vector<long long>key_locations;;
    for(unsigned int key_index = 0; key_index<num_answer_keys+1; key_index++) {
      long long fake_byte_loc = 0;
      evtfile.write((char*) &fake_byte_loc, sizeof(fake_byte_loc));
    }
    key_locations.push_back(evtfile.tellp());
    for(unsigned int key_index = 0; key_index<num_answer_keys; key_index++) {
      key = keys[key_index];
      key_locations.push_back(evtfile.tellp());
      evtfile.write((char*) &key.key_index, sizeof(key.key_index));
      unsigned int num_scats = key.energy_deps.size();
      const char* prim_particle_type_char = key.prim_particle_type.c_str();
      int size_of_prim_particle_type = key.prim_particle_type.size();
      const char* particle_type_char = key.particle_type.c_str();
      int size_of_particle_type = key.particle_type.size();
      evtfile.write((char*) &random_seed, sizeof(random_seed)); // Bacc random seed.
      evtfile.write((char*) &key.event_number_Bacc, sizeof(key.event_number_Bacc));
      evtfile.write((char*) &key.event_caused_trigger, sizeof(key.event_caused_trigger));
      evtfile.write((char*) &size_of_prim_particle_type, sizeof(size_of_prim_particle_type));
      evtfile.write((char*) prim_particle_type_char, size_of_prim_particle_type);
      evtfile.write((char*) &key.energy_prim_par, sizeof(key.energy_prim_par));
      evtfile.write((char*) &key.x_prim_par, sizeof(key.x_prim_par));
      evtfile.write((char*) &key.y_prim_par, sizeof(key.y_prim_par));
      evtfile.write((char*) &key.z_prim_par, sizeof(key.z_prim_par));
      evtfile.write((char*) &key.xdir_prim_par, sizeof(key.xdir_prim_par));
      evtfile.write((char*) &key.ydir_prim_par, sizeof(key.ydir_prim_par));
      evtfile.write((char*) &key.zdir_prim_par, sizeof(key.zdir_prim_par));
      evtfile.write((char*) &size_of_particle_type, sizeof(size_of_particle_type));
      evtfile.write((char*) particle_type_char, size_of_particle_type);
      int NEST_num_photons = -1;
      int NEST_num_electrons = -1;
      evtfile.write((char*) &(NEST_num_photons), sizeof(NEST_num_photons));
      evtfile.write((char*) &(NEST_num_electrons), sizeof(NEST_num_electrons));
      evtfile.write((char*) &key.energy, sizeof(key.energy));
      evtfile.write((char*) &key.x, sizeof(key.x));
      evtfile.write((char*) &key.y, sizeof(key.y));
      evtfile.write((char*) &key.z, sizeof(key.z));
      evtfile.write((char*) &key.missing_energy, sizeof(key.missing_energy));
      evtfile.write((char*) &num_scats, sizeof(num_scats));
      int size_of_scat_var = sizeof(double);
      for(unsigned int scat_ind = 0; scat_ind<num_scats; scat_ind++) {
        evtfile.write((char*) &(NEST_num_photons), sizeof(NEST_num_photons));
        evtfile.write((char*) &(NEST_num_electrons), sizeof(NEST_num_electrons));
        evtfile.write((char*) &key.energy_deps[scat_ind], size_of_scat_var);
        evtfile.write((char*) &key.x_scats[scat_ind], size_of_scat_var);
        evtfile.write((char*) &key.y_scats[scat_ind], size_of_scat_var);
        evtfile.write((char*) &key.z_scats[scat_ind], size_of_scat_var);
        // Write 10 empty doubles for future expansion.
        double empty_field = 0;
        for(unsigned int i_empty=0; i_empty<10; i_empty++)
        evtfile.write((char*) &empty_field, sizeof(empty_field));
      }
      evtfile.write((char*) &key.msd, sizeof(key.msd));
      for(unsigned int pmt_index = 0; pmt_index<122; pmt_index++) {
        int num_photons = key.photons_per_chan[pmt_index];
        evtfile.write((char*) &num_photons, sizeof(num_photons));
      }
      evtfile.write((char*) &key.timestamp, sizeof(key.timestamp));
      long long long_seed = (long long) seed; // Bacc2evt seed.
      evtfile.write((char*) &long_seed, sizeof(long_seed));
      // Reserve extra space for up to 366 future doubles/3 kB per key for
      // future expansion. I've already started to use it...
      double empty_field = 0;
      for(int num_empty_fields=0; num_empty_fields<(122*3-1); num_empty_fields++) {
        evtfile.write((char*) &empty_field, sizeof(empty_field));
      }
    key_locations.push_back(evtfile.tellp());
    }
    evtfile.seekp(start_of_byte_locs);
    for(unsigned int key_index = 0; key_index<num_answer_keys; key_index++) {
      long long byte_loc = key_locations[key_index];
      evtfile.write((char*) &byte_loc, sizeof(byte_loc));
    }


        

    evtfile.close();
    if(evtfile.is_open()) cout << ".evt file not closed!" << endl;

    // DONE!

    // Ok, let's see if I can decrease the memory demands by deleting stuff.

    return 0;

}

// End Main

void Bacc_usage(){
    cout << " " << endl;
    cout << "Syntax:" << endl;
    cout << "Bacc2evt [-v #] [-o datetime] <input filename>" << endl;
    cout << "Bacc2evt [-v #] [-o datetime] -i <input filenames>" << endl;
    cout << "Bacc2evt [-v #] [-o datetime] -i <dir to bin files>" << endl;
    cout << "Bacc2evt [-v #] [-n #] -i <input filenames>" << endl;
    cout << "Bacc2evt [-v #] [-n #] -i <dir to bin files>" << endl;
    cout << "Bacc2evt [-v #] [-x <xml filename>]";
    cout << " -i <input filenames>" << endl;
    cout << endl;
    cout << "Bacc2evt [-h,--help]\tPrint out usage info for debug modes.";
    cout << endl;
}

void Bacc_help(){

    Bacc_usage();

    cout << "Bacc2evt: " << endl;
    cout << "  " << "A program in C++ to convert Bacc binary output\n";
    cout << "files (.bin) to the first stage of DAQ file formats\n";
    cout << "that users of LUX have access to reading/writing (.evt).";
    cout << "\n  " << "When using -o, a line like \"-o 20150101T0259\" will";
    cout << "\nproduce filenames like luxsm_20150101T0259_f000000001.evt,";
    cout << "\netc. It is a datetime stamp. File numbering is taken care of.";
    cout << "\n  " << "Full documentation can be found at" << endl;
    cout << "\thttp://alongannoyinglink.com" << endl;
    cout << "Quick note: Record levels must be at least 3 and set in";
    cout << "\nthe PhotoCathode volume." << endl;
    cout << " " << endl;
    cout << "DEBUG is defined as:" << endl;
    cout << " D%2 -> General IO" << endl;
    cout << " D%3 -> Header from .bin file" << endl;
    cout << " D%5 -> Details of event (PrimPar not included)" << endl;
    cout << " D%7 -> Raw Dump of pmt hits. Semicolon/comma delim'ed" << endl;
    cout << " D%11-> Random Sampling of pmt hits." << endl;
    cout << " D%13-> Updates while digitizing channels" << endl;
    cout << " D%17-> .evt channel header." << endl;
    cout << " D%19-> Pulse information as it is written to evt file" << endl;
    cout << " D%23-> Dump of digitized samples as written to evt   " << endl;
    cout << "        file. Delimited by comma/semicolon (smpl,chan)" << endl;
    cout << " D%29-> (time,phe) tuple for channel plotting.        " << endl;
    cout << " D%31-> Print any gains data read from the XML file.  " << endl;
    cout << " D%31-> Print any gains data read from the XML file.  " << endl;
    cout << " D%37-> Print the monte carlos truth (answer keys).   " << endl;
    cout << "Thus DEBUG=2*7=14 gives both General IO and hit times." << endl;
    cout << "\t--> 14%2 == True and 14%7 == True." << endl;
}

void run_managed(int input_arg_pos, int argc, char** argv,
                                std::vector<std::string>input_files){

    // Here's an experiment on making this program work with multiple
    //.bin files specified. Maybe implement later.
    // The run_managed function will call the "main" function multiple
    // times, once for each of the files in the input_files vector.
    // Most of the code is for transfering any run time switches/options
    // to the new char** argv. A very simple example is shown below.
    //      char* ddd[2];
    //      ddd[0] = new char[100];
    //      ddd[1] = new char[100];
    //      strcpy(ddd[0],"./Bacc2evt");
    //      strcpy(ddd[1],"-h");
    //      main(2,ddd);

    // First determine the runtime output. Use either the switch given or find
    // it from the file.
    string run_datetime =  get_Bacc_bin_datetime(input_files[0]);
    for(int arg=0; arg<argc; arg++){
        if(strcmp(argv[arg], "-o") == 0 || strcmp(argv[arg], "--output")==0)
            run_datetime = argv[arg+1];
    }


    int new_argc = input_arg_pos + 3;
    char* new_argv[new_argc];    // Args plus -i, -o, and 2 strings.


    // Apply the first arguments (-v, etc)
    for(int i=0; i<input_arg_pos; i++){
        new_argv[i] = new char[200];
        strcpy(new_argv[i],argv[i]);
    }


    //unsigned int tot_num_events = input_files.size();

    // Now apply the output switch (-o), the output name, and the input file.
    new_argv[input_arg_pos] = new char[200];
    new_argv[input_arg_pos+1] = new char[200];
    new_argv[input_arg_pos+2] = new char[200];
    for(unsigned int file=0; file < input_files.size(); file++){
        string evtfilename = "luxsm_" + run_datetime;
        std::stringstream sstr;
        sstr.fill('0');
        sstr << "_f";
        sstr << setw(9) << file+1 << ".evt";
        evtfilename += sstr.str();
        strcpy(new_argv[input_arg_pos], "-o");

        strcpy(new_argv[input_arg_pos+1], evtfilename.c_str());

        strcpy(new_argv[input_arg_pos+2], input_files[file].c_str());
        cout << "Working on : " << input_files[file] << "\t---> ";
        cout << evtfilename << endl;
        
        ///////////////////////////
        ///// CALL TO MAIN ////////
        main(new_argc, new_argv);//
        ///////////////////////////
    }
    return;
}

std::string get_Bacc_bin_datetime(std::string filename) {
    ifstream inFilestream;
    inFilestream.open(filename.c_str(), ios::binary|ios::in);
    int Size;  // An int buffer to read sizes into. Used throughout code.
    int iNumRecords;
    inFilestream.read((char*)(&iNumRecords),sizeof(int));

    inFilestream.read((char*)(&Size), sizeof(int));
    char* productionTime = new char [Size+1];
    inFilestream.read((char*)(productionTime), Size);
    inFilestream.close();

    //   Need to format the production time for creation of .evt file. This   //
    //   looks like "GMT: Fri Apr 16 20:21:16 2010\n" and it will need to     //
    //   be formated as to produce filenames like luxsm_YYYYMMDDTHHMM.evt     //
    string buff;   // Temp buffer to work with.
    string year;
    string month;
    string day;
    string hour;
    string minute;

    buff = "00";
    for(int i=0; i<2; i++) buff[i] = productionTime[i+13];  // Get day, but
    for(int i=0; i<2; i++) if(buff[i] ==' ') buff[i] = '0'; // days can be " 8"
    day = buff;
    for(int i=0; i<2; i++) buff[i] = productionTime[i+16];  // Get hour
    hour = buff;
    for(int i=0; i<2; i++) buff[i] = productionTime[i+19];  // Get minute
    minute = buff;
    buff = "000";
    for(int i=0; i<3; i++) buff[i] = productionTime[i+9];   // Get month
    if(buff == "Jan") month = "01";
    if(buff == "Feb") month = "02";
    if(buff == "Mar") month = "03";
    if(buff == "Apr") month = "04";
    if(buff == "May") month = "05";
    if(buff == "Jun") month = "06";
    if(buff == "Jul") month = "07";
    if(buff == "Aug") month = "08";
    if(buff == "Sep") month = "09";
    if(buff == "Oct") month = "10";
    if(buff == "Nov") month = "11";
    if(buff == "Dec") month = "12";
    buff = "0000";
    for(int i=0; i<4; i++) buff[i] = productionTime[i+25];  // Get year
    year = buff;

    delete[] productionTime;
    
    string evtfilename = year+month+day+"T"+hour+minute;
    return evtfilename;
    //return (string) "20000000T0000";
}

