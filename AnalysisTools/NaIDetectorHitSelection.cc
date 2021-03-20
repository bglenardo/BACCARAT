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
    cout << "./NaIDetectorHitSelection <path/to/rootfile> <outfileName>" << endl;
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

  int nai_id = 57;

  TFile * outfile = new TFile(argv[2],"RECREATE");
  TTree * newTree = inTree->CloneTree(0);  

  bool hit_nai = false;

  for(int i=0; i<inTree->GetEntries(); i++) {
//  for(int i=0; i<100; i++) {
     if( i % 50000 == 0 ) printf("Processing %d\n",i);
     inTree->GetEntry(i);
     hit_nai = false;
     //printf("tracks...\n");
     evt->tracks.size();
     //printf("done\n");
     //printf("Event: %d\n",i);
     for(int tr=0; tr<evt->tracks.size(); tr++) { 
         //printf("\tTrack: %d\n",tr);
         for(int st=0; st<evt->tracks[tr].steps.size(); st++) { 
               //printf("\t\tStep: %d, volume: %s\n",st,evt->tracks[tr].steps[st].sVolumeName.c_str());
               if( evt->tracks[tr].steps[st].sVolumeName.find("tof_target")!=std::string::npos ||
                   evt->tracks[tr].steps[st].sVolumeName.find("nai_target")!=std::string::npos ){  

                    hit_nai = true;
                    //cout << "Volume: " << evt->tracks[tr].steps[st].sVolumeName << endl;
               }
        }
     }
     if( hit_nai ) newTree->Fill();
  }

  printf("Beginning...\n");
  outfile->Write();
  outfile->Close();
  return 1;
}



//void HighLevelDiagnosticPlots(char* rootFilePath, char * outFileName) {
////commenting out all NR for now since just looking at Tritium data
//  //TFile * outFile = new TFile("PhotonTimingAnalysisPlots_Brian_LookingForAfterpulsing_2.root","RECREATE");  //change this depending on the dataset so you can differentiate
//
//  printf("Creating TChain...\n");
//
//  TChain * chain = new TChain("events");
//  chain->Add(rootFilePath);
//
//  printf("\n%d events loaded.\n",chain->GetEntries());
//
//  // Variables for storing RQ's
//  int    aft_t25_samples[10]; 
//  UInt_t pulse_classification[10],
//         z_drift_samples[10];
//  float  pulse_area_phe[10], 
//         x_cm_tmplt[10], 
//         y_cm_tmplt[10],
//         x_cm_del[10],
//         y_cm_del[10],
//         x_cm[10],
//         y_cm[10],
//         peak_area_phe[10][122],
//         xyz_corrected_pulse_area_all_phe[10];
//  UInt_t pulse_length[10];
//  int	 pulse_end[10];
//  float earliest_by_ch[10][122];
//  float delta_t_in_same_ch[10][122][10];
//  unsigned long long luxstamp_samples;
//
//
//
//  chain->SetBranchAddress("pulse_area_phe",&pulse_area_phe);
//  chain->SetBranchAddress("peak_area_phe",&peak_area_phe);
//  chain->SetBranchAddress("pulse_classification",&pulse_classification);
//  chain->SetBranchAddress("aft_t25_samples",&aft_t25_samples); 
//  chain->SetBranchAddress("x_cm_tmplt",&x_cm_tmplt); 
//  chain->SetBranchAddress("y_cm_tmplt",&y_cm_tmplt); 
//  chain->SetBranchAddress("pulse_length_samples",&pulse_length); 
//  chain->SetBranchAddress("pulse_end_samples",&pulse_end); 
//  chain->SetBranchAddress("luxstamp_samples",&luxstamp_samples);
//  chain->SetBranchAddress("z_drift_samples",&z_drift_samples);
//  chain->SetBranchAddress("x_cm",&x_cm);
//  chain->SetBranchAddress("y_cm",&y_cm);
//  chain->SetBranchAddress("x_cm_del",&x_cm_del);
//  chain->SetBranchAddress("y_cm_del",&y_cm_del);
//  printf("Branch addresses set...\n");
//
//
//  TFile * outFile = new TFile(outFileName,"RECREATE");
//  TTree * cutTree = chain->CloneTree(0); 
//  printf("Output tree initialized. Begin loop...\n");
//
//  // ER event loop..
//  for(int i=0; i<chain->GetEntries(); i++) {
//
//     chain->GetEntry(i);
//     if (i % 10000 == 0) printf("Processing event %d/%d...\n",i,chain->GetEntries());
//
//     // First, loop over pulses to find the S1 and S2.  This will form the basis
//     // of some high-level cuts.
//     int S1ind=-1, S2ind=-1, S1count=0, S2count=0;
//     for(int pulse=0; pulse<10; pulse++) {
//       if (pulse_classification[pulse] == 1 ) {
//          S1ind = pulse;
//          S1count++;
//       } else if (pulse_classification[pulse] == 2) {
//          S2ind = pulse;
//          S2count++;
//       }
//     }
//
//     if(S1ind < 0 || S2ind < 0)
//       continue;
//
//     // Define event-level cuts:
//     bool S2area = pulse_area_phe[S2ind] > 50. && pulse_area_phe[S2ind] < 20000.;
//     bool golden = S1count==1 && S2count==1; 
//
//     bool per_event_cuts = S2area && golden;
//
//
//
//     // Make cuts
//     if( per_event_cuts ) {
//        cutTree->Fill(); 
//     } // end if( per_event_cuts )
//
//  } // end ER event loop
//
//   
//outFile->Write();
//outFile->Close();
//
//
//} // End of script
