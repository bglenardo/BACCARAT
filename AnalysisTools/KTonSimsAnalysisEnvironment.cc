#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <map>

#include "BaccRootConverterEvent.hh"

#include "KTonSimsAnalysisEnvironment.hh"

#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TString.h"
#include "TRandom.h"
#include "TMath.h"
#include "TGraphErrors.h"
#include "TLine.h"

using namespace std;
#define PI 3.14159265


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
KTonSimsAnalysisEnvironment::KTonSimsAnalysisEnvironment() {

   inputFile = NULL;
   eventsTree = NULL;
   headerTree = NULL;
   eventsChain = NULL;
   current_evt = NULL;
   volumes = NULL;
   r.SetSeed(0);

   chainsAdded = 0;

}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
KTonSimsAnalysisEnvironment::~KTonSimsAnalysisEnvironment() {
//   if( inputFile ) delete inputFile;
//   if( eventsTree ) delete eventsTree;
//   if( headerTree ) delete headerTree;
//   if( eventsChain ) delete eventsChain;
//   if( current_evt ) delete current_evt;

}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
void KTonSimsAnalysisEnvironment::SetCurrentEvt(int evt_num) {
//   if( inputFile ) delete inputFile;
//   if( eventsTree ) delete eventsTree;
//   if( headerTree ) delete headerTree;
//   if( eventsChain ) delete eventsChain;
//   if( current_evt ) delete current_evt;
   if (!eventsChain) {
      std::cout << "No events loaded in chain! Aborting..." << std::endl;
      return;
   }
   eventsChain->GetEntry(evt_num);

}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
bool KTonSimsAnalysisEnvironment::IsDataLoaded() {
  if( !eventsTree && !eventsChain ){ std::cout << "Tree or chain not loaded." << std::endl; return false; }
//  if( !headerTree ) return false;
//  if( !inputFile ) return false;
 
  return true;

}



//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int KTonSimsAnalysisEnvironment::PrintOutEnergyDepInfo() {

  if( !IsDataLoaded() ) {
     cout << "No data loaded! Please load data before running this function." << endl;
     return -1;
  } 

    for(int tr=0; tr<current_evt->tracks.size(); tr++) {
      printf("\tTrack %d,\t particle ID: %d,\t sCreatorProcess: %s\n",
             current_evt->tracks[tr].iTrackID,
             current_evt->tracks[tr].iParticleID,
             current_evt->tracks[tr].sCreatorProcess.c_str());
      for(int st=0; st<current_evt->tracks[tr].steps.size(); st++) {
         printf("\t\tVolume: %d, %s, Time: %f\n", current_evt->tracks[tr].steps[st].iVolumeID,
                                                  current_evt->tracks[tr].steps[st].sVolumeName.c_str(),
                                                  current_evt->tracks[tr].steps[st].dTime_ns );
      }
    }

  return 0;
}


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int KTonSimsAnalysisEnvironment::LoadInputRootFile( string fname ) {

  inputFile = new TFile(fname.c_str());
  if(!inputFile || !(inputFile->IsOpen()) || inputFile->IsZombie()){
    std::cerr<<fname<<" can not be opened!"<<std::endl;
    return -1;
  } 
  inputFileName = fname;

  eventsTree = (TTree *)inputFile->Get("DataTree");
  if(!eventsTree || eventsTree->IsZombie() || eventsTree->GetEntries()<1){
    std::cerr<<"No data available. "<<std::endl;
    return -1;
  }
  cout << "eventsTree pointer: " << eventsTree << endl;


  headerTree = (TTree *)inputFile->Get("HeaderTree");
  if(!headerTree || headerTree->IsZombie() || headerTree->GetEntries()<1){
    std::cerr<<"No data available. "<<std::endl;
    return -1;
  }
  std::cout << "Successfully loaded root file and trees." << std::endl;
  std::cout << "Setting addresses..." << std::endl;
  // Set the address of Events to evt
  eventsTree->SetBranchAddress("Event",&current_evt);

  headerTree->SetBranchAddress("componentLookupTable",&volumes);
  std::cout << "headerTree->GetEntry(0);" << std::endl;
  headerTree->GetEntry(0); 

  //eventsTree->Print();

}


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int KTonSimsAnalysisEnvironment::PrintTree() {
  cout << "eventsTree pointer: " << eventsTree << endl;
  if( !eventsTree && !eventsChain ) {
     cout << "No data loaded!" << endl;
     return -1;
  }

  if( eventsTree ) eventsTree->Print();
  if( eventsChain ) eventsChain->Print();
  return 1;
}


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int KTonSimsAnalysisEnvironment::PrintCurrentEvtDetails() {
  
  if( !IsDataLoaded() ) {
     cout << "No data loaded! Please load data before running this function." << endl;
     return -1;
  } 

    for(int tr=0; tr<current_evt->tracks.size(); tr++) {
      printf("\tTrack %d,\t particle ID: %d,\t sCreatorProcess: %s,\t iParentID: %d\n",
             current_evt->tracks[tr].iTrackID,
             current_evt->tracks[tr].iParticleID,
             current_evt->tracks[tr].sCreatorProcess.c_str(),
             current_evt->tracks[tr].iParentID);
      for(int st=0; st<current_evt->tracks[tr].steps.size(); st++) {
         printf("\tVolume: %d, %s\n",current_evt->tracks[tr].steps[st].iVolumeID,current_evt->tracks[tr].steps[st].sVolumeName.c_str());
      }
  }

  return 0;

}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int KTonSimsAnalysisEnvironment::FindVolumeID( string vname ) {
   
  if( !volumes ) {
    cout << "Volumes ID vector not loaded. Error..." << endl;
    return -1;
  } 

  for( int i=0; i<volumes[0].size(); i++) {
    if( vname == volumes[0][i] ) return i+1;
  }
  cout << "Couldn't find the volume specified. Check the name and try again." << endl;
  return -1;  

}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int KTonSimsAnalysisEnvironment::LoadInputChain( string dir ) {

   eventsChain = new TChain("DataTree");
 
   int numLoaded = (int) eventsChain->Add( (dir+"*root").c_str() );
   if( !eventsChain || eventsChain->IsZombie() || numLoaded == 0 ) {
      cout << "No files loaded into TChain! There must be a problem." << endl;
      return -1;
   }
   else {
      eventsChainVec.push_back( eventsChain );
      cout << numLoaded << " files loaded into TChain." << endl;
      eventsChainEntriesVec.push_back( (int) eventsChainVec[chainsAdded]->GetEntries() );
      totalEvents += eventsChainEntriesVec[chainsAdded];
      cout << eventsChainEntriesVec[chainsAdded] << " entries added to chain; " << totalEvents << " total." << endl;
      chainsAdded++;
   }
   inputDirName = dir;
  
 
   eventsChain->SetBranchAddress("Event",&current_evt);  
   
   return 0;
}


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int KTonSimsAnalysisEnvironment::AddInputChain( string dir ) {

   eventsChainVec.push_back( new TChain("DataTree") );

   int numLoaded = (int) eventsChainVec[ chainsAdded ]->Add( (dir+"*root").c_str() );
   if( !eventsChainVec[chainsAdded] || eventsChainVec[chainsAdded]->IsZombie() || numLoaded == 0 ) {
      cout << "No files loaded into TChain! There must be a problem." << endl;
      return -1;
   }
   else { 
      cout << numLoaded << " files loaded into TChain." << endl;
      eventsChainEntriesVec.push_back( (int) eventsChainVec[chainsAdded]->GetEntries() );
      totalEvents += eventsChainEntriesVec[chainsAdded];
      cout << eventsChainEntriesVec[chainsAdded] << " entries added to chain; " << totalEvents << " total." << endl;
      chainsAdded++;
   }

   return 0;
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
double KTonSimsAnalysisEnvironment::ComputeDistance( double x1[3], double x2[3] ) {

  double r = sqrt( (x1[0] - x2[0])*(x1[0] - x2[0]) +
                   (x1[1] - x2[1])*(x1[1] - x2[1]) +
                   (x1[2] - x2[2])*(x1[2] - x2[2]) );
  return r;
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int KTonSimsAnalysisEnvironment::ReduceSimulatedData( string outfilename ) {
    
   if( !eventsChain || eventsChain->IsZombie() || eventsChainVec.size() == 0 ) {
      cout << "No files loaded into TChain! Nothing to reduce." << endl;
      return -1;
   }

   //int nentries = eventsChain->GetEntries();
   //printf("%d entries in chain.\n",nentries);

   TFile * outfile = new TFile(outfilename.c_str(),"RECREATE");
   TTree * data = new TTree();
   data->SetName("data");

  double standoff, energy, eventsizeRMS, eventsize_max_distance;
 
  data->Branch("standoff",&standoff,"standoff/D");
  data->Branch("eventsizeRMS",&eventsizeRMS,"eventsizeRMS/D");
  data->Branch("eventsize_max_distance",&eventsize_max_distance,"eventsize_max_distance/D");
  data->Branch("energy",&energy,"energy/D");


  for( int chidx =0; chidx < (int)eventsChainVec.size(); chidx++ ){

    eventsChain = eventsChainVec[chidx];

    for(int i=0; i<eventsChainEntriesVec[chidx]; i++){
    //for(int i=0; i<10; i++) { 
        if( i % 10000 == 0 ) printf("i = %d\n",i);
        eventsChain->GetEntry(i);
        eventsizeRMS = 0.;
        eventsize_max_distance = 0.;
        vector<double> edep_position_x;
        vector<double> edep_position_y;
        vector<double> edep_position_z; 
        vector<double> edep_energy;

	if( current_evt->primaryParticles.size() == 0 ) continue;
 
        vector<stepInfo> steps;
        vector<trackInfo> tracks = current_evt->tracks;
        primaryParticleInfo primaryParticle = current_evt->primaryParticles[0];

        standoff = 0.;
        energy = 0.;
        double standoff_r = 0.;
        double standoff_z = 0.; 
 
        for(size_t tr=0; tr<tracks.size(); tr++) {
            steps = tracks[tr].steps;
            for(size_t st=0; st<steps.size(); st++) {
                
                // If this step's radial coordinate is the largest one yet, set "standoff_r" to this one
                if( TMath::Sqrt( TMath::Power(steps[st].dPosition_mm[0],2.) + TMath::Power(steps[st].dPosition_mm[1],2.) ) > standoff_r ) {
                    standoff_r = TMath::Sqrt( TMath::Power(steps[st].dPosition_mm[0],2.) + TMath::Power(steps[st].dPosition_mm[1],2.) );
                }
                // If this step's z coordinate is the largest one yet, set "standoff_z" to this one
                if( TMath::Abs( steps[st].dPosition_mm[2] ) > standoff_z ){
                    standoff_z = TMath::Abs( steps[st].dPosition_mm[2] );
                }

                if( steps[st].sVolumeName == "liquid_xenon_target" || steps[st].sVolumeName == "liquid_xenon_above_eg" || steps[st].sVolumeName == "xe_target" ) {
                    energy += steps[st].dEnergyDep_keV;
                    edep_position_x.push_back(steps[st].dPosition_mm[0]);
                    edep_position_y.push_back(steps[st].dPosition_mm[1]);
                    edep_position_z.push_back(steps[st].dPosition_mm[2]);
                    edep_energy.push_back(steps[st].dEnergyDep_keV);
                } // end if( volume == LXetarget )
            } // end steps loop         
        } // end tracks loop

        // Loop through event positions and calculate size variables
        double mean = 0., meansquared = 0.;
        double meanx = 0., meany = 0., meanz = 0.;
        bool mean_calculated = false;
        int num_edep = (int) edep_position_x.size();
        double tot_e = 0.;
        for(int i=0; i<num_edep; i++){
            double x = edep_position_x[i];
            double y = edep_position_y[i];
            double z = edep_position_z[i];
            //printf("\t energy: %f X: %f \t Y: %f \t Z: %f \n",edep_energy[i],x,y,z);
            for(int j=0; j<num_edep; j++) { 
                double this_distance = TMath::Sqrt( TMath::Power(x - edep_position_x[j], 2.) +
                                                    TMath::Power(y - edep_position_y[j], 2.) +
                                                    TMath::Power(z - edep_position_z[j], 2.) );
                if( eventsize_max_distance < this_distance )
                    eventsize_max_distance = this_distance;
                if( !mean_calculated ){
                   meanx += edep_position_x[j]*edep_energy[j]/energy;
                   meany += edep_position_y[j]*edep_energy[j]/energy;
                   meanz += edep_position_z[j]*edep_energy[j]/energy;
                   tot_e += edep_energy[j];
                }
                
            }
            mean_calculated = true;
             
            eventsizeRMS += TMath::Sqrt( (x-meanx)*(x-meanx) + 
                                         (y-meany)*(y-meany) +
                                         (z-meanz)*(z-meanz) ) * edep_energy[i]/energy;
        }
        //printf("TOtal energy from vector: %f, total energy from dTotEDepXe_keV: %f\n",tot_e,energy);
        //printf("Mean position: (%f,%f,%f,), RMS size: %f\n",meanx,meany,meanz,eventsizeRMS); 


 
        if (standoff_z > standoff_r)
             standoff = standoff_z;
        else
             standoff = standoff_r;
 
        data->Fill();
        //printf("XeEnergy: %f\t, LSEnergy: %f\n",dTotEDepXe_keV,dTotEDepLS_keV); 
    } // end events loop
  } // end chains loop
  data->Write();
  outfile->Close();
  return 0;
} 


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int KTonSimsAnalysisEnvironment::SetOutputDir( string outDir_temp ) {
   outputDir = outDir_temp;
   return 1;
}

