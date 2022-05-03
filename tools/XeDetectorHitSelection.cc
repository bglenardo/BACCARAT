#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TF1.h"
#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include "TObject.h"
#include "TCanvas.h"
#include <iostream>
#include <fstream>
#include <string>
#include "BaccRootConverterEvent.hh"

using namespace std;

int main(int argc, char *argv[]){


  if (argc != 3) {
    cout << "Wrong number of arguments." << endl;
    cout << "./XeDetectorHitSelection <path/to/rootfile> <outfileName>" << endl;
    return 0;
  }

  TChain * inTree = new TChain("DataTree");
  char fname[200];
  sprintf(fname,"%s",argv[1]);

  inTree->Add(fname);
  printf("Added %s\n",argv[1]);
//  return 1;

//  TFile * inFile = new TFile(argv[1]);
//  TTree * inTree = (TTree *)inFile->Get("DataTree");

  BaccRootConverterEvent * evt = NULL;
  inTree->SetBranchAddress("Event",&evt);

  int ls_id = 39;

  TFile * outfile = new TFile(argv[2],"RECREATE");
  TTree * newTree = inTree->CloneTree(0);  

  bool hit_xe = false;

  for(int i=0; i<inTree->GetEntries(); i++) {
  //for(int i=0; i<1000; i++) {
     if( i % 50000 == 0 ) printf("Processing %d\n",i);
     inTree->GetEntry(i);
     hit_xe = false;
     //printf("tracks...\n");
     evt->tracks.size();
     //printf("done\n");
     //printf("Event: %d\n",i);
     for(int tr=0; tr<evt->tracks.size(); tr++) { 
         //printf("\tTrack: %d\n",tr);
         for(int st=0; st<evt->tracks[tr].steps.size(); st++) { 
               //printf("\t\tStep: %d, volume: %s\n",st,evt->tracks[tr].steps[st].sVolumeName.c_str());
               if( evt->tracks[tr].steps[st].sVolumeName.find("liquid_xenon_target")!=std::string::npos ||
                   evt->tracks[tr].steps[st].sVolumeName.find("liquid_xenon_above_eg")!=std::string::npos ||
                   evt->tracks[tr].steps[st].sVolumeName.find("xe_target")!=std::string::npos ){  

                    hit_xe = true;
                    //cout << "Volume: " << evt->tracks[tr].steps[st].sVolumeName << endl;
               }
        }
     }
     if( hit_xe ) newTree->Fill();
  }

  printf("Beginning...\n");
  outfile->Write();
  outfile->Close();
  return 1;
}



