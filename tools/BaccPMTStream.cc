////////////////////////////////////////////////////////////////////////////////
/*	BaccPMTStream.cc
*
* A reader which converts an event root tree into a stream of PMT hits
*
*
********************************************************************************
* Change log
*	22 Jan 2016 - First go (ACK)
*/
////////////////////////////////////////////////////////////////////////////////

//
//  Executable includes
//
#include "BaccRootConverterEvent.hh"
//#include "BaccRootConverterEvent_dict.h"

//
//	ROOT includes
//
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

//
//	C/C++ includes
//
#include <fstream>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdlib>

//
//	Definitions
//
#define DEBUGGING 0

using namespace std;

int main(int argc, char *argv[])
{
   TFile* infile = new TFile(argv[1],"UPDATE");
   
   TTree* headertree = (TTree*)infile->Get("HeaderTree");
   TTree* datatree = (TTree*)infile->Get("DataTree");

   vector<string>* lookupTable=0;
   headertree->SetBranchAddress("componentLookupTable",&lookupTable);
   headertree->GetEntry(0);


   //scan for PMTs
   vector<bool> liveindices;
   vector<int> pmtbank;
   vector<string> banks;
   vector<int> index;
   std::cout << lookupTable->size() << std::endl;


   TBranch* b_banks =  headertree->Branch("PMT_Banks",&banks);

   for(int i=0; i<lookupTable->size(); i++)
   {
      if(i%100==0) std::cout << "Component N: " << i << std::endl;
      TString component = lookupTable->at(i);
      if(component.Contains("PMT_PhotoCathode") || component.Contains("PMT_Photocathode"))
      {
	 liveindices.push_back(true);
	 TString prefix = component.Copy();
	 prefix.Replace(prefix.Last('_'),prefix.Sizeof()-prefix.Last('_'),"",0);
	 if(banks.size()==0 || banks.back()!=prefix.Data())
	    banks.push_back(prefix.Data());
	 pmtbank.push_back(banks.size()-1);
	 TString sindex = component.Copy();
	 sindex.Replace(0,sindex.Last('_')+1,"",0);
	 index.push_back(sindex.Atoi());
	 std::cout << component << "," << i << "," << liveindices[i] << "," << pmtbank[i] << "," << index[i] << std::endl;

      }
      else
      {
	 liveindices.push_back(false);
	 pmtbank.push_back(-1);
	 index.push_back(-1);
      }
   }
   b_banks->Fill();
   headertree->Write("HeaderTree",TObject::kWriteDelete);
      


   BaccRootConverterEvent* event=0;
   datatree->SetBranchAddress("Event",&event);

   double time; 
   double wavelength;
   int ID;
   int eventN;
   int bankindex;
   int pmtindex;
   
   TTree* PMTStream = new TTree("PMTStreamTree","Time stream of PMTS");
   PMTStream->Branch("PhotonTime_ns",&time,"PhotonTime_ns/D");
   PMTStream->Branch("PhotonWavelength_nm",&wavelength,"PhotonWavelength_nm/D");
   PMTStream->Branch("PMTID",&ID,"PMTID/I");
   PMTStream->Branch("bankIndex",&bankindex,"bankIndex/I");
   PMTStream->Branch("pmtIndex",&pmtindex,"pmtIndex/I");
   PMTStream->Branch("originalEventID",&eventN,"originalEventID/I");
   
   for(int i=0; i<datatree->GetEntries(); i++)
   {
//      if(i%100==0) 
	 std::cout << "Entry N: " << i << std::endl;
      datatree->GetEntry(i);
      int ntrack = event->tracks.size();
      std::cout << ntrack << std::endl;
      for(int j=0; j<ntrack; j++)
      {
      	 if(liveindices[event->tracks[j].steps.back().iVolumeID-1]) //omg this is horrible, but does the track end in a step that is a PMT photocathode?
      	 {

	    time = event->tracks[j].steps.back().dTime_ns;
	    wavelength =  event->tracks[j].dWavelength_nm;
	    ID =  event->tracks[j].steps.back().iVolumeID-1;
	    eventN = i;
	    bankindex = pmtbank[ID];
	    pmtindex = index[ID];
//      	    std::cout << (*lookupTable)[event->tracks[j].steps.back().iVolumeID-1] << "," << event->tracks[j].steps.back().dTime_ns << "," 
//		      << event->tracks[j].dWavelength_nm << "," << event->tracks[j].steps.back().iVolumeID << "," << i 
//		      << "," << bankindex << "," << pmtindex << std::endl;

	    PMTStream->Fill();
	    
     	 }
	 
      }
      
   }
   PMTStream->BuildIndex("PMTID","originalEventID");


   PMTStream->Write("PMTStreamTree",TObject::kOverwrite);


}
