//////////////////////////////////////////////////////////////////////////////
//
//
//  Bacc2evtMethods.cc
//
//  A support file containing various methods used in Bacc2evt
//  
//////////////////////////////////////////////////////////////////////////////
//  
//  Change Log:
//
//  ## Month 2010 - Initial Submission (Michael Woods)
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
#include <math.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <stdlib.h>

//
//  Bacc2evt includes
//
#include "Bacc2evt.hh"
#include "Bacc2evtMethods.hh"

#define ReadSize() inFilestream.read((char*)(&Size), sizeof(int))
#define _USE_MATH_DEFINES

// //----------------------------------------------------
// ||         DETECTOR GEOMETRY CONSTANTS              ||
// ||                                                  ||
#define CATHODE_HEIGHT 5.6 //cm                        ||
#define INSCRIBED_RADIUS 23.485 //cm                   ||
// ||                                                  ||
// \\--------------------------------------------------//
using namespace std;


void determineSensitiveVolume(char* InputCommands,
                            char* top_vol, char* bot_vol) {
    string strInput(InputCommands);
    size_t start=0, end=0;
    string record_level_string = "/Bacc/detector/recordLevel";
    size_t len = record_level_string.length();
    vector<string> recorded_volumes;
    while(end != string::npos) {
        start = end+1;
        end = strInput.find_first_of("\n", start+1);
        string line = strInput.substr(start, end-start);
        if(line.substr(0, len) == record_level_string) {
            //size_t start_of_vol = len+1;
            size_t start_of_vol = line.find_first_of(" ");
            size_t length_of_vol = 
                line.find_first_of(" ", start_of_vol+1) - start_of_vol;
            string volume_name = line.substr(start_of_vol, length_of_vol);
            recorded_volumes.push_back(volume_name);
        }
    }
    
    bool record_photocathode = false;
    bool record_other = false;
    for(unsigned int v=0; v<recorded_volumes.size(); v++) {
        string cur = recorded_volumes[v];
        if( cur.find("PhotoCathode") != string::npos)
            record_photocathode = true;
        else
            record_other = true;
    }


    // We have photocathode records and that is all. All is as it should be
    // so lets get on our way.
    if( record_photocathode && !record_other ) {
        cout << "PMT records found. No information from energy depositions";
        cout << "in xenon are available." << endl;
        return;     // All is as it should be. Let's get going.
    }

    // We have the photocathode records and some other record. Print a warning
    // and continue on.
    if( record_photocathode && record_other ) {
        cout << "Records other than PMT available. Records in LXe volumes ";
        cout << "will be used for truth." << endl;
        return;
    }

    if( !record_photocathode ) {
        cerr << "** [ERROR] A record level in  PMT_PhotoCathode volume has ";
        cerr << "NOT been set. Creation" << "\n" << "of .evt file failed. ";
        cerr << "Ensure the Bacc macro has a record level set in" << "\n";
        cerr << "PMT_PhotoCathode by adding the following:\n\t";
        cerr << "/Bacc/detector/recordLevelThermElec PMT_PhotoCathode 3" << endl;
        exit(0);
    }

    return;
}

int* makePMTLookupTable(char*DetCompo, int *PMTLookupTable, bool oldPMTStyle) {

    /*
    A method that will parse the DetCompo string and match together the 
    Geant4 ID number (an incrementing integer decided upon at the runtime
    of the simulation) to the PMT identification number as to where it lies
    in the copper PMT banks decided during consturction time!

    Additions:
        Maybe add a loop to look for hits in a default volume, and if it
        isn't found give a warning about it, look for a second volume name,
        and lastly ask for what volume you want.

        Remove unneeded [top][bottom]PmtLookup arrays.
    */

    //const int numPmts = 122; 
    //int topPmtLookup[numPmts];  // Arrays to point PMT number to the geant4
    //int bottomPmtLookup[numPmts];   // volume number. 
    //int PMTLookupTable[numPmts];
    
    // This is a loop to check which of the following are the recorded volume
    // number. A priori we don't know if it is the PhotoCathode or the Vacuum,
    // or what!
    /*
    cout << DetCompo << endl;
    int photocathode_count = 0;
    int vacuum_count = 0;
    int other_count = 0;
    char * strck1 = strtok (DetCompo," ");
    char * strck2 = strtok (NULL," ");
    char * strck3 = strtok (NULL,"\n");
    while(strcmp(strck3,"VacuumVessel")!=0){
        strck1 = strtok (NULL," ");
        strck2 = strtok (NULL," ");
        strck3 = strtok (NULL,"\n");
        char topPhot[] = "Top_PMT_PhotoCathode_"; 
        char bottomPhot[] = "Bottom_PMT_PhotoCathode_";
        char topVac[] = "Top_PMT_Vacuum_"; 
        char bottomVac[] = "Bottom_PMT_Vacuum_";
        if(strncmp(strck3,topPhot,strlen(topPhot)) == 0 ||
             strncmp(strck3,bottomPhot,strlen(bottomPhot)) == 0) {
            photocathode_count += 1;
            cout << "Found a phocat hit! >> " << photocathode_count << endl;
        }
        else if(strncmp(strck3,topVac,strlen(topVac)) == 0 ||
             strncmp(strck3,bottomVac,strlen(bottomVac)) == 0) {
            vacuum_count += 1;
            cout << "Found a vacuum hit! >> " << vacuum_count << endl;
        }
        else {
            other_count += 1;
            cout << "Found an anoms hit..>> " << other_count << endl;
        }
    }
    */


    // This while loop is the meat of the matching. The string token (strtok)
    // is splitting the string at spaces and new lines to extract the G4
    // component number. For example,
    //      ID 357: Bottom_PMT_Vacuum_58            
    // will match pmt number 58 to the G4 volume number 357 (created when Bacc
    // was run).
    char * str1 = strtok (DetCompo," ");
    //removing str1 results in segfaults.
    str1 = str1; //Line exists solely to avoid unused variable warnings
    char * str2 = strtok (NULL," ");
    char * str3 = strtok (NULL,"\n");
    //cout << "DetComp: " << DetCompo << endl;
    //cout << "str1: " << str1 << endl;
    //cout << "str2: " << str2 << endl;
    //cout << "str3: " << str3 << endl;
    while(strcmp(str3,"VacuumVessel")!=0){
        str1 = strtok (NULL," ");
        str2 = strtok (NULL," ");
        str3 = strtok (NULL,"\n");
        //char topName[] = "Top_PMT_Window_Active_";
        //char bottomName[] = "Bottom_PMT_Window_Active_";
        //char topName[] = "Top_PMT_Window_Active"; 
        //char bottomName[] = "Bottom_PMT_Window_Active";
        char topName[] = "Top_PMT_PhotoCathode_"; 
        char bottomName[] = "Bottom_PMT_PhotoCathode_";
        //char topName[] = "Top_PMT_Vacuum_"; 
        //char bottomName[] = "Bottom_PMT_Vacuum_";
        if(oldPMTStyle){
          if(strncmp(str3,topName,strlen(topName)) == 0){
            size_t length;
            char pmtNumber[5];
            string componentName = (string)str3;
            length = componentName.copy(pmtNumber,
                strlen(str3)-strlen(topName),strlen(topName));
            pmtNumber[length] = '\0';
            char volumeNumber[5];
            strncpy(volumeNumber,str2,strlen(str2)-1);
            volumeNumber[strlen(str2)-1] = '\0';
            //topPmtLookup[atoi(pmtNumber)-1] = atoi(volumeNumber);
            PMTLookupTable[atoi(pmtNumber)-1] = atoi(volumeNumber);
            //cout << "Geant4VolID: " << volumeNumber << "\tPMT #: " << (atoi(pmtNumber)+61) << endl;
          }
          else if(strncmp(str3,bottomName,strlen(bottomName)) == 0){
            size_t length;
            char pmtNumber[5];
            string componentName = (string)str3;
            length = componentName.copy(pmtNumber,
                strlen(str3)-strlen(bottomName),strlen(bottomName));
            pmtNumber[length] = '\0';
            char volumeNumber[5];
            strncpy(volumeNumber,str2,strlen(str2)-1);
            volumeNumber[strlen(str2)-1] = '\0';
            //bottomPmtLookup[atoi(pmtNumber)-1] = atoi(volumeNumber);
            PMTLookupTable[atoi(pmtNumber)-1+61] = atoi(volumeNumber);
            //cout << "Geant4VolID: " << volumeNumber << "\tPMT #: " << pmtNumber << endl;
          }
        }
        else{ //Using the new, 1-to-1 pmt mapping
          size_t length;
          char pmtNumber[5];
          string componentName = (string)str3;
          if(strncmp(str3,topName,strlen(topName)) == 0){
            length = componentName.copy(pmtNumber,
                strlen(str3)-strlen(topName),strlen(topName));
          }
          else if(strncmp(str3,bottomName,strlen(bottomName)) == 0){
            length = componentName.copy(pmtNumber,
                strlen(str3)-strlen(bottomName),strlen(bottomName));
          }
          else
            continue;
          pmtNumber[length] = '\0';
          char volumeNumber[5];
          strncpy(volumeNumber,str2,strlen(str2)-1);
          volumeNumber[strlen(str2)-1] = '\0';
          //topPmtLookup[atoi(pmtNumber)-1] = atoi(volumeNumber);
          PMTLookupTable[atoi(pmtNumber)-1] = atoi(volumeNumber);
        }
    }

    //for(int qq=0; qq<122; qq++)
    //cout << "PMTLookup" << qq << "\t" << PMTLookupTable[qq] << endl;
    return PMTLookupTable;
}

int GetPMTNumber(int* PMTLookupTable, int Geant4VolID) {
    for(int pmt=0;pmt<122;pmt++)
        if(PMTLookupTable[pmt] == Geant4VolID)
            return pmt;
    //cout << "Error! Geant4VolID " << Geant4VolID << " not matched. ";
    return -1;
}

void createPositionLookup(int* pmt_relations) {
  // This is supposed to translate the "grid" like arrangement
  // of pmts used in Bacc to the "pizza" like arrangement used
  // in the LUX detector (as of Run02). If you supply a Bacc pmt
  // number, it will pop out the "real life" number.
  //
  // pmt_relations is assumed to be length 122. We will fill this
  // with the corresponding pmts.
  
  // If the old pmt style (grid, rather than pizza) is in use, the
  // relation is one-to-one.


  double pmtX=0, pmtY=0;
  double pmtX1[122], pmtY1[122];  // For grid pmt numbers
  GetPMTCoords(pmtX1, pmtY1);
  pmt_relations[30] = 120;
  pmt_relations[91] = 121;

    for (int s=0; s<60; s+=10){
	  double angle = 2*3.14159265/6*(s+10)/10;

	  for (int p=0; p<10; p++){
		if(p < 4) {
		  pmtY = 4 * sqrt(3.)/2 ;
		  pmtX = -(p - 2);
		}
		else if(p < 7) {
		  pmtY = 3 * sqrt(3.)/2;
		  pmtX = -(p - 5.5);
		}
		else if(p < 9) {
		  pmtY = 2 * sqrt(3.)/2;
		  pmtX = -(p - 8);
		}
		else if(p < 5+6+7+8) {
		  pmtY = 1 * sqrt(3.)/2;
		  pmtX = -(p - 9.5);
		}

		double Y = pmtY*cos(angle)+pmtX*sin(angle);
		double X = pmtX*cos(angle)-pmtY*sin(angle);
		for(int i=0; i<61; i++) {
		  if((fabs(X-pmtX1[i]) < .001) && (fabs(Y-pmtY1[i]) < .001)) {
			pmt_relations[i] = p+s;
			pmt_relations[i+61] = p+s+60;
			// To test, output the results of the (x,y) matching.
			//cout<<setw(15);
			//cout<<setprecision(8);
			//cout << "Found it: " << i << "\t" << p+s << "\t" 
			//<< X << "\t" << pmtX1[i] << "\t" << fabs(X-pmtX1[i]) 
			//<<"\t|\t" << Y << "\t" << pmtY1[i] << "\t" << fabs(Y-pmtY1[i]) 
			//<< endl;
			break;
		  }
		}
	  }
	}
	// Another test output. Final results.
	//for(int i=0; i<122; i++) cout << i+1 << "\t--->\t" << pmt_relations[i]+1 << endl;
}
#include <iomanip>


// Two functions used with vectors later.
double minElement(vector<double> & vec) {
    double min = 1e34;
    for(int i=0; i<(int)vec.size(); i++){
        if(vec.at(i) < min) min = vec.at(i);
    }
    return min;
    }


double maxElement(vector<double> & vec) {
    double max = -1e10;
    for(int i=0; i<(int)vec.size(); i++){
        if(vec.at(i) > max) max = vec.at(i);
    }
    return max;
    }

unsigned short mVtoADC(double mV) {
    unsigned short numBits = 16384;      // 2**14
    //std::cout << mV << "\t";
    //double digMin = -15563. / 8.2;
    //double digMax = 100;       // -1900 mV lower limit of digitizer
    double digMin = -1900;     //  +100 mV upper limit of digitizer
    double digMax = 100;       // -1900 mV lower limit of digitizer

    //double ADCpermV = 1.*numBits / (digMax - digMin);
    double ADCpermV = 1.*(numBits-1) / (digMax - digMin);

    //mV = mV - (1./ADCpermV)/2.;
    mV = mV - digMin;           // Offset (-1900 -> 100 mV)
    mV += 0.0183117;             // This addition comes from the following:
    //mV += .02;                  // This addition comes from the following:
                                // Any value between -0.04 and +0.08 mV is
                                // converted to 15564 ADC. The addition
                                // shifts 0 mV to the middle of this range
                                // to get rid of a bias problem.
    mV = mV * ADCpermV;         // Convert to ADC
    //std::cout << mV<< "\t-->\t";
    //if(mV - int(mV) > .5){      // Round the signal properly (instead of 
        //mV--;                   // always rounding down)
    //}
    //std::cout << mV << std::endl;
    //mV -= 0.5;
    mV = round(mV);             // Digitize
    //mV = floor(mV);             // Digitize
    if(mV > numBits-1)            // Maximum storage is 14-bits.
        return numBits-1;
    else if(mV < 0)             // Underflow
        return 0;
    else return (unsigned short) mV;
}

unsigned short VtoADC(double V) {
    unsigned short numBits = 16384;      // 2**14
    //std::cout << V << "\t";
    //double digMin = -15563. / 8.2 / 1000;
    //double digMax = 0.1;       // -1900 mV lower limit of digitizer
    double digMin = -1.9;       //  +100 mV upper limit of digitizer
    double digMax = 0.1;       // -1900 mV lower limit of digitizer

    //double ADCperV = 1.*numBits / (digMax - digMin);
    double ADCperV = 1.*(numBits-1) / (digMax - digMin);

    //V = V - (1./ADCperV)/2.;
    V = V - digMin;             // Offset (-1900 -> 100 mV)
    //V += .02/1000.;             // This addition comes from the following:
    V += 0.0183117/1000.;             // This addition comes from the following:
                                // Any value between -0.04 and +0.08 mV is
                                // converted to 15564 ADC. The addition
                                // shifts 0 mV to the middle of this range
                                // to get rid of a bias problem.
    V = V * ADCperV;            // Convert to ADC
    //std::cout << V << "\t  -->\t";
    //if(V - int(V) > .5){        // Round the signal properly (instead of 
        //V--;                    // always rounding down)
    //}
    //std::cout << V ;
    //V -= 0.5;
    V = round(V);               // Digitize
    if(V > numBits-1)           // Maximum storage is 14-bits.
        return numBits-1;
    else if(V < 0)              // Underflow
        return 0;
    else return (unsigned short) V;
}

double round(double a) {
    return floor(a+0.5);
}

void GetPMTCoords(double* pmtX, double* pmtY) {
    //   pmtX and pmtY are the x,y coordinates of the pmts. The index is     //
    //   for a pmt in these arrays is the pmt number minus one.              //
    //   This arrangement is for a "grid" like PMT arrangement, left to      //
    //   right, up to down. This is not the "pizza slice" configuration      //

    const int numPmts = 122;
    //double pmtSpace = 5.9;
    double pmtSpace = 1;

    for(int p=0; p<numPmts; p++){
		if(p < 5) {
			pmtY[p] = 4 * sqrt(3.)/2 * pmtSpace;
			pmtX[p] = (p - 2) * pmtSpace;
		}
		else if(p < 5+6) {
			pmtY[p] = 3 * sqrt(3.)/2 * pmtSpace;
			pmtX[p] = (p - 7.5) * pmtSpace;
		}
		else if(p < 5+6+7) {
			pmtY[p] = 2 * sqrt(3.)/2 * pmtSpace;
			pmtX[p] = (p - 14) * pmtSpace;
		}
		else if(p < 5+6+7+8) {
			pmtY[p] = 1 * sqrt(3.)/2 * pmtSpace;
			pmtX[p] = (p - 21.5) * pmtSpace;
		}
		else if(p < 5+6+7+8+9) {
			pmtY[p] = 0;
			pmtX[p] = (p - 30) * pmtSpace;
		}
		else if(p < 5+6+7+8+9+8) {
			pmtY[p] = -1 * sqrt(3.)/2 * pmtSpace;
			pmtX[p] = (p - 38.5) * pmtSpace;
		}
		else if(p < 5+6+7+8+9+8+7) {
			pmtY[p] = -2 * sqrt(3.)/2 * pmtSpace;
			pmtX[p] = (p - 46) * pmtSpace;
		}
		else if(p < 5+6+7+8+9+8+7+6) {
			pmtY[p] = -3 * sqrt(3.)/2 * pmtSpace;
			pmtX[p] = (p - 52.5) * pmtSpace;
		}
		else if(p < 5+6+7+8+9+8+7+6+5) {
			pmtY[p] = -4 * sqrt(3.)/2 * pmtSpace;
			pmtX[p] = (p - 58) * pmtSpace;
		}
        else {
            pmtX[p] = pmtX[p-61];
            pmtY[p] = pmtY[p-61];
        }
		//cout << p << "\t" << pmtX[p] << "\t" << pmtY[p] << "\t" << endl;
    }

    return;
}

double GetOffset() {
    return rand() / (1.0*RAND_MAX);
}

double Rand() {
    return rand() / (1.0*RAND_MAX);
}

double RandN() {
    double x1, x2, w, y1, y2;
    static int phase = 0;   // Static declaration will exist in mem until exit
    phase = 1 - phase;      // Switch the phase

    do {
        x1 = 2.0 * Rand() - 1.0;
        x2 = 2.0 * Rand() - 1.0;
        w = x1 * x1 + x2 * x2;
    } while ( w >= 1.0 );

    w = sqrt( (-2.0 * log( w ) ) / w );
    y1 = x1 * w;
    y2 = x2 * w;
    if(phase == 0) {
        return y1;
    }
    else {
        return y2;
    }
        
}

double Gauss(double x, double mean, double height, double sigma) {
    // Evaluates a gaussian curve with mean, height, sigma at x=x.
    return height * exp( - pow((x-mean),2) / (2. * sigma*sigma) );
}

int Poisson(double mean) {
  double R;
  double sum = 0;
  int i;
  i=-1;
  double z;
  while(sum <=mean) {
    R = (double)rand()/(double)(RAND_MAX);
    z = -log(R);
    sum+= z;
    i++;
  }
  return i;
}


unsigned short vec_avg(std::vector<unsigned short> &vec) {
    double avg = 0;
    if(vec.size() == 0) return 0;

    for(unsigned int i = 0; i < vec.size(); i++) avg+=vec[i];

    return (unsigned short) round((avg/vec.size()));
}

int find_last_evt_file(std::string filename, std::string outPath){
    // A method to examine the files in the directory where the .evt files
    // are and find out which number to use as _f00000000#.evt. I don't
    // want to overwrite anything that is already there.
    string cmd; // This will list all pertenent .evt files.
    cmd = "ls  " + outPath + "/*_f*evt > /tmp/luxsm2evt.txt 2>/dev/null";
    if (system(cmd.c_str()) != 0)   // Non-zero return indicates 0 files.
        return 1;                   // So return 1, since we'll start there.

    std::vector<double> filenumbers;

    // Ok, parse the file created with the list of .evt files.
    ifstream check_tmp("/tmp/luxsm2evt.txt");
    char line[300];    // 300 char buffer for each line.
    while(check_tmp.getline(line, sizeof(line))) {
        string sline = line;
        string filenumber;
        filenumber = sline.substr( sline.length() - 13, 9);
        filenumbers.push_back( atoi(filenumber.c_str()));
    }
    
    bool found_next_file = false;
    for(unsigned int i=1; i<maxElement(filenumbers)+2; i++) {
        found_next_file = true;
        for(unsigned int j=0; j< filenumbers.size(); j++) {
            if(i == (unsigned int)filenumbers[j]){
                found_next_file = false;
                break;
            }
        }
        if(found_next_file) return i;
    }

    // If there are no files, just return 1.
    return 1;
}

std::vector<std::string> get_cwd_bins(){
    // A method to examine the files in the cwd where the .evt files will
    // be looked for.
    string cmd; // This will list all pertenent .evt files.
    cmd = "ls -v `pwd`/*.bin > /tmp/luxsm2evt_filelist.txt 2>/dev/null";
    if (system(cmd.c_str()) != 0) {     // Non-zero return indicates 0 files.
        std::cerr << "No files in current working dir. Quitting." << std::endl;
        exit(0);
    }

    std::vector<std::string> vector_of_files;
    // Ok, parse the file created with the list of .evt files.
    ifstream check_tmp("/tmp/luxsm2evt_filelist.txt");
    char line[300];    // 300 char buffer for each line.
    while(check_tmp.getline(line, sizeof(line))) {
        string sline = line;
        vector_of_files.push_back(sline);
        std::cout << "GCWD Files: " << sline << std::endl;
    }
    

    return vector_of_files;
}

std::vector<std::string> get_dir_bins(std::string dir){
    // A method to examine the files in a distant directory where .evt files
    // will be be looked for.
    string cmd; // This will list all pertenent .evt files.
    cmd = "ls | sort -g  " + dir + "/*.bin > /tmp/luxsm2evt_filelist.txt 2>/dev/null";
    if (system(cmd.c_str()) != 0) {     // Non-zero return indicates 0 files.
        std::cerr << "No files in " << dir << "! Quitting." << std::endl;
        exit(0);
    }

    std::vector<std::string> vector_of_files;
    // Ok, parse the file created with the list of .evt files.
    ifstream check_tmp("/tmp/luxsm2evt_filelist.txt");
    char line[300];    // 300 char buffer for each line.
    while(check_tmp.getline(line, sizeof(line))) {
        string sline = line;
        vector_of_files.push_back(sline);
        //std::cout << "GDB Files: " << sline << std::endl;
    }
    

    return vector_of_files;
}

std::vector<std::string> get_cli_bins(int arg, int argc, char** argv) {
    // A method to examine the files as specified on the command line
    // interface (CLI). These arguments are parsed.
    string cmd; // This will list all pertenent .evt files.

    std::vector<std::string> vector_of_files;

    for(int i=arg; i<argc; i++){
        string single_file = argv[i];
        vector_of_files.push_back(single_file);
        //std::cout << "GETCLI Files: " << single_file << std::endl;
    }

    return vector_of_files;
}

double randn_trig(double mu=0.0, double sigma=1.0) {
    static bool deviateAvailable=false; //  flag
    static double storedDeviate;         //  deviate from previous calculation
    double dist, angle;
    
    //  If no deviate has been stored, the standard Box-Muller transformation is 
    //  performed, producing two independent normally-distributed random
    //  deviates.  One is stored for the next round, and one is returned.
    if (!deviateAvailable) {
        
        //  choose a pair of uniformly distributed deviates, one for the
        //  distance and one for the angle, and perform transformations
        dist=sqrt( -2.0 * log(double(rand()) / double(RAND_MAX)) );
        angle=2.0 * M_PI * (double(rand()) / double(RAND_MAX));
        
        //  calculate and store first deviate and set flag
        storedDeviate=dist*cos(angle);
        deviateAvailable=true;
        
        //  calcaulate return second deviate
        return dist * sin(angle) * sigma + mu;
    }
    
    //  If a deviate is available from a previous call to this function, it is
    //  returned, and the flag is set to false.
    else {
        deviateAvailable=false;
        return storedDeviate*sigma + mu;
    }
}

void write_event_to_disk(vector< vector<double> > times, ofstream& phot_time_file) {
  size_t num_chans = times.size();
  phot_time_file.write( (char*)&num_chans, sizeof(num_chans) );
  for(size_t chan=0; chan<times.size(); chan++) {
    size_t num_phots = times[chan].size();
    phot_time_file.write( (char*)&num_phots, sizeof(num_phots) );
    for(size_t ph=0; ph<times[chan].size(); ph++) {
      phot_time_file.write( (char*)&times[chan][ph], sizeof(times[chan][ph]) );
      //phot_time_file << times[chan][ph] << " ";
    }
    //phot_time_file << "\t";
  }
  //phot_time_file << "\n";
}

void makeVolumeLookupTable(string volume_list, vector<string> &vols, vector<int>&ids) {
  // Let's parse something like this:
  // ID 1: expHall
  // ID 2: Vacuum
  // ID 3: ThermalShield
  // ID 4: XenonVessel
  // ...
  
  bool done = false;
  size_t pos_of_ID = 0;   // Position of next "ID"
  size_t vol_id_s = 0;    // Volume id start
  size_t vol_id_e = 0;    // Volume id end
  size_t vol_name_s = 0;  // Volume name start
  size_t vol_name_e = 0;  // Volume name end
  while(!done) {
    pos_of_ID = volume_list.find("ID", vol_name_e);  // Finds next "ID" instance.
    if(pos_of_ID == string::npos) break;
    vol_id_s = pos_of_ID + 3;
    vol_id_e = volume_list.find(":", vol_id_s);
    vol_name_s = volume_list.find(" ", vol_id_e) + 1;
    vol_name_e = volume_list.find("\n", vol_name_s);

    int id = atoi(volume_list.substr(vol_id_s, vol_id_e - vol_id_s).c_str());
    string vol = volume_list.substr(vol_name_s, vol_name_e - vol_name_s);

    vols.push_back(vol);
    ids.push_back(id);

    //cout << "pos_of_ID: " << pos_of_ID << "\t" << volume_list[pos_of_ID] << endl;
    //cout << "vol_id_s: " << vol_id_s << "\t" << volume_list[vol_id_s] << endl;
    //cout << "vol_id_e: " << vol_id_e << "\t" << volume_list[vol_id_e] << endl;
    //cout << "vol_name_s: " << vol_name_s << "\t" << volume_list[vol_name_s] << endl;
    //cout << "vol_name_e: " << vol_name_e << "\t" << volume_list[vol_name_e] << endl;
    //cout << "VOLUME: " << id << "  ----  " << vol << endl;
  }
  return;
}

double calculate_max_scattering_distance(answer_key key) {
  // The key contains the x,y,z for each energy deposition.
  // Let's calculated the maximum distance between scatters.

  double msd = 0;

  vector<double>& x = key.x_scats;
  vector<double>& y = key.y_scats;
  vector<double>& z = key.z_scats;

  for(size_t i=0; i<x.size(); i++) {
    for(size_t j=i+1; j<x.size(); j++) {
      double dx = x[i] - x[j];
      double dy = y[i] - y[j];
      double dz = z[i] - z[j];
      if( dx*dx + dy*dy + dz*dz > msd )
        msd = dx*dx + dy*dy + dz*dz;
    }
  }
  return sqrt(msd);
}


bool is_in_active_region(double x,double y,double z) {
  /*
   * (Inside Dodecagon)  \                                          Outside
   * (Inside Dodecagon)    \                                        Outside
   * (Inside Dodecagon)      \                                      Outside
   * (Inside Dodecagon)   0    \        <------- Event modulated    Outside
   * (Inside Dodecagon)          \               to location of     Outside
   * (Inside Dodecagon)            \             X below and the    Outside
   * (Inside Dodecagon)              \           position compared  Outside
   * (Inside Dodecagon)                \         to the line def-   Outside
   * (Inside Dodecagon)                 \        ining that one     Outside
   * (Inside Dodecagon)                  \       sextant.           Outside
   * (Inside Dodecagon)                   \          |              Outside
   * (Inside Dodecagon)                 X  \      <--|              Outside
   * (Inside Dodecagon)                     \                       Outside
   * (Inside Dodecagon)                      \                      Outside
   * (Inside Dodecagon)                       \                     Outside
   * (Inside Dodecagon)     Event defined to   \  "slope" is this   Outside
   * (Inside Dodecagon)     be "in detector"    \ <--- line         Outside
  */
  static double thirty_degrees = 2.*M_PI/12;
  static double slope = -(INSCRIBED_RADIUS/2)/(INSCRIBED_RADIUS*(1-sqrt(3)/2));
  if(z < CATHODE_HEIGHT)
    return false;

  double theta = atan2(y,x);
  double r = sqrt(x*x + y*y);
  
  // Modulate it in to the first twelfth
  double theta_mod = fmod(theta, thirty_degrees);
  theta_mod = fabs(theta_mod);
  x = r*cos(theta_mod);
  y = r*sin(theta_mod);
  double calculated_y = slope*(x - (INSCRIBED_RADIUS/cos(thirty_degrees/2.)));
  if( y < calculated_y)
        return true;
  return false;
}
