#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

#include "BaccRootConverterEvent.hh"

#include "Xe127SimsAnalysisEnvironment.hh"

#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TString.h"


using namespace std;
#define PI 3.14159265


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
Xe127SimsAnalysisEnvironment::Xe127SimsAnalysisEnvironment() {

   inputFile = NULL;
   eventsTree = NULL;
   headerTree = NULL;
   eventsChain = NULL;
   current_evt = NULL;
   volumes = NULL;

}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
Xe127SimsAnalysisEnvironment::~Xe127SimsAnalysisEnvironment() {
//   if( inputFile ) delete inputFile;
//   if( eventsTree ) delete eventsTree;
//   if( headerTree ) delete headerTree;
//   if( eventsChain ) delete eventsChain;
//   if( current_evt ) delete current_evt;

}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
bool Xe127SimsAnalysisEnvironment::IsDataLoaded() {
  if( !eventsTree && !eventsChain ) return false;
  if( !headerTree ) return false;
  if( !inputFile ) return false;
 
  return true;

}



//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int Xe127SimsAnalysisEnvironment::PrintOutEnergyDepInfo(int event_num) {

  if( !IsDataLoaded() ) {
     cout << "No data loaded! Please load data before running this function." << endl;
     return -1;
  } 

  //printf("Got here.\n");
  eventsTree->GetEntry(event_num);
  //printf("Event %d\n",event_num);
  for(int tr=0; tr<current_evt->tracks.size(); tr++) {
    printf("\tTrack %d:\n\t\t particle ID: %d,\t sCreatorProcess: %s \t ParentID: %d\n",
           current_evt->tracks[tr].iTrackID,
           current_evt->tracks[tr].iParticleID,
           current_evt->tracks[tr].sCreatorProcess.c_str(),
           current_evt->tracks[tr].iParentID);
    for(int st=0; st<current_evt->tracks[tr].steps.size(); st++) {
       printf("\t\tVolume: %d, %s\t Process: %s\t Energy dep: %f\t PartEn: %f\n",current_evt->tracks[tr].steps[st].iVolumeID,
                                                   current_evt->tracks[tr].steps[st].sVolumeName.c_str(),
                                                   current_evt->tracks[tr].steps[st].sProcess.c_str(),
                                                   current_evt->tracks[tr].steps[st].dEnergyDep_keV,
                                                   current_evt->tracks[tr].steps[st].dParticleEnergy_keV);
       printf("\t\tDirection: (%f, %f, %f)\n",current_evt->tracks[tr].steps[st].dDirection[0],
                                              current_evt->tracks[tr].steps[st].dDirection[1],
                                              current_evt->tracks[tr].steps[st].dDirection[2]);
    }
  }

  return 0;
}


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int Xe127SimsAnalysisEnvironment::PrimaryNeutronScatteringVolumes() {

  if( !IsDataLoaded() ) {
     cout << "No data loaded! Please load data before running this function." << endl;
     return -1;
  }

  bool DEBUG = false; 

  std::cout << "Building histograms..." << std::endl;
  TH1F * h_scattering_vols_ss = new TH1F("h_scattering_vols_ss","h_scattering_vols_ss",201,-1.,200.);
  TH1F * h_scattering_vols_ms = new TH1F("h_scattering_vols_ms","h_scattering_vols_ms",201,-1.,200.);
  TH1F * h_scattering_vols_first = new TH1F("h_scattering_vols_first","h_scattering_vols_first",201,-1.,200.);
  TH1F * h_energy_in_peek = new TH1F("h_energy_in_peek","h_energy_in_peek",50000,0.,500.);
  stepInfo step;
  trackInfo track;
  int num_events = eventsTree->GetEntries(),
      num_scatters,
      num_single_scatters = 0,
      num_single_scatters_in_target = 0,
      num_multiple_scatters = 0,
      num_multiple_scatters_in_target = 0, 
      num_no_scatters = 0;
//  num_events = 100;
  vector<int> thisVolumeID;

  std::cout << "Beginning event loop..." << std::endl;
  for(int e=0; e < num_events; e++) {
     if( e % 10000 == 0 ) cout << "Processing event " << e << " of " << num_events << endl;
  
     num_scatters = 0;
     thisVolumeID.clear();
     eventsTree->GetEntry(e);

     if( IsSingleScatter() ) {
        h_scattering_vols_ss->Fill(current_evt->tracks[0].steps[0].iVolumeID);
        h_scattering_vols_first->Fill(current_evt->tracks[0].steps[0].iVolumeID);
        if( current_evt->tracks[0].steps[0].iVolumeID == FindVolumeID("liquid_xenon_target") )
          num_single_scatters_in_target++;
     } else {

         // For the first track, get the scattering volume
         h_scattering_vols_first->Fill(current_evt->tracks[0].steps[0].iVolumeID);

         // Find out if there are any interactions in the liquid xenon target
         bool scatters_in_target_vol = false;
         for(int tr=0; tr < current_evt->tracks.size(); tr++) {
            track = current_evt->tracks[tr];
            for(int st=0; st < track.steps.size(); st++) {
                if( track.steps[st].iVolumeID == FindVolumeID("liquid_xenon_target") )
                    scatters_in_target_vol = true;
            }
         }
         if( scatters_in_target_vol ) num_multiple_scatters_in_target++;
     }
  }

  TCanvas * c1 = new TCanvas();
  h_scattering_vols_ss->SetLineWidth(2);
  h_scattering_vols_ss->SetLineColor(2);
  h_scattering_vols_ss->Draw();
  h_scattering_vols_ms->SetLineWidth(2);
  h_scattering_vols_ms->SetLineColor(4);
  h_scattering_vols_ms->Draw("SAME");

  printf("************************************************************************************\n"); 
  printf("Total number of events: %d\n",num_events );
  printf("Number of SS in LXe target: %d\n",num_single_scatters_in_target);
  printf("Number of MS in LXe target: %d\n",num_multiple_scatters_in_target);
  printf("************************************************************************************\n"); 
  printf("Rates of first-scattering in various volumes: \n");
  bool human_readable = false;
  for(int vol=0; vol < volumes[0].size(); vol++) {
    if( human_readable ) {
       cout << setw(7) << left << "Volume " << setw(3) << left << vol << ": " << setw(30) << left << volumes[0][vol]
            << setw(7) << left << " Hits: " << setw(8) << right << h_scattering_vols_first->GetBinContent(vol+3) << "/" << num_events
            << setw(12) << left << " Hit fraction: " << setw(10) << left << float(h_scattering_vols_first->GetBinContent(vol+3)/num_events) << endl;
    } else {
       cout << vol << "," << volumes[0][vol] << "," << float(h_scattering_vols_first->GetBinContent(vol+3)/num_events) << endl;
    }
    
  } 
  printf("************************************************************************************\n"); 


}


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int Xe127SimsAnalysisEnvironment::DirectionOfPrimariesForScatteringEvents() {

  if( !IsDataLoaded() ) {
     cout << "No data loaded! Please load data before running this function." << endl;
     return -1;
  } 
  
  TH2F * h_position_dist_ss = new TH2F("h_position_dist_ss","h_position_dist_ss",100,-180.,180.,100,-90.,90.);
  TH2F * h_position_dist_ms = new TH2F("h_position_dist_ms","h_position_dist_ms",100,-180.,180.,100,-90.,90.);
  TH2F * h_position_dist = new TH2F("h_position_dist","h_position_dist",100,-180.,180.,100,0.,180.);
  TH2F * h_all_events = new TH2F("h_all_events","h_all_events",100,-180.,180.,100,0.,180.);

  vector<volumeInfo> volumes;
  vector<stepInfo> steps;
  vector<trackInfo> tracks;
  vector<primaryParticleInfo> primaries;
  double x, y, z;
  bool scatters_in_detector_vols = false;
  int num_events = eventsTree->GetEntries();

  for(int e=0; e<num_events; e++) {
     if( e % 10000 == 0 ) cout << "Processing event " << e << "/" << num_events << endl;
     eventsTree->GetEntry(e);
     volumes = current_evt->volumes;
     tracks = current_evt->tracks;
     primaries = current_evt->primaryParticles;
     x = primaries[0].dDirection[0];
     y = primaries[0].dDirection[1];
     z = primaries[0].dDirection[2];
//     cout << " x: " << x << " y: " << y << " z: " << z << endl;
     double r = sqrt( x*x + y*y + z*z );
//     cout << "r: " << r << endl;
     scatters_in_detector_vols = false;
     if( tracks.size() == 0 ) continue;

     for(int tr=0; tr < tracks.size(); tr++) {
        steps = tracks[tr].steps;
        for(int st=0; st < steps.size(); st++) {
            if( steps[st].sVolumeName == "liquid_xenon_target" ||
                steps[st].sVolumeName == "ls_target" )
                scatters_in_detector_vols = true;
        }
     }
     double thetaPrime = acos(z/r);
     double phiPrime = asin( y / (r*sin(thetaPrime)) );
     if( phiPrime < 0. && x < 0. )
       phiPrime = -PI - phiPrime;
     if( phiPrime > 0. && x < 0. )
       phiPrime = PI - phiPrime;    
 
//     cout << "thetaPrime: " << thetaPrime << " phiPrime: " << phiPrime << " sin(thetaPrime) = " << sin(thetaPrime) << endl;
     h_all_events->Fill(phiPrime*180./PI,thetaPrime*180./PI);
     if( scatters_in_detector_vols ){
        // theta' is measured from positive x-axis, phi' is measured from positive y-axis
        // Coordinate transformation is: x = z', y = x', z = y'
        h_position_dist->Fill(phiPrime*180./PI,thetaPrime*180./PI);
     }
     // Need an IsSingleScatter function
     // Need an IsSecondary function, maybe  

  } 
//  gStyle->SetPalette(1);
  TCanvas * c1 = new TCanvas();
  h_all_events->Draw("colz");
  TCanvas * c2 = new TCanvas();
  h_position_dist->Draw("colz");


}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
bool Xe127SimsAnalysisEnvironment::IsSingleScatter() {

  if( !IsDataLoaded() ) {
     cout << "No data loaded! Please load data before running this function." << endl;
     return -1;
  } 

  if( current_evt->tracks.size() == 1 ) return true;
  
  if( current_evt->tracks.size() > 1 ) {
    for( int i=0; i<current_evt->tracks.size(); i++) {
       if( current_evt->tracks[i].sCreatorProcess != "neutronInelastic" ) return false;
    }
    return true;
  }  
  if( current_evt->tracks.size() < 1 ) return false;
}


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int Xe127SimsAnalysisEnvironment::LoadInputRootFile( string fname ) {

  if( inputFile ) {
      delete inputFile;
      delete eventsTree;
  }

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
int Xe127SimsAnalysisEnvironment::PrintTree() {
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
int Xe127SimsAnalysisEnvironment::PrintCurrentEvtDetails() {
  
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
int Xe127SimsAnalysisEnvironment::FindVolumeID( string vname ) {
   
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
int Xe127SimsAnalysisEnvironment::LoadInputChain( string dir ) {

   eventsChain = new TChain("DataTree");

   int numLoaded = (int) eventsChain->Add( (dir+"*root").c_str() );
   if( !eventsChain || eventsChain->IsZombie() || numLoaded == 0 ) {
      cout << "No files loaded into TChain! There must be a problem." << endl;
      return -1;
   }
   else 
      cout << numLoaded << " files loaded into TChain." << endl;
   inputDirName = dir;
  
 
   eventsChain->SetBranchAddress("Event",&current_evt);  
   

   return 0;
}


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
double Xe127SimsAnalysisEnvironment::ComputeDistance( double x1[3], double x2[3] ) {

  double r = sqrt( (x1[0] - x2[0])*(x1[0] - x2[0]) +
                   (x1[1] - x2[1])*(x1[1] - x2[1]) +
                   (x1[2] - x2[2])*(x1[2] - x2[2]) );
  return r;
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int Xe127SimsAnalysisEnvironment::ReduceSimulatedData(string outfilePrefix = "TMSReducedData") {
    
//   if( !eventsChain || eventsChain->IsZombie() ) {
//      cout << "No files loaded into TChain! Nothing to reduce." << endl;
//      return -1;
//   }

//   int nentries = eventsChain->GetEntries();
   int nentries = eventsTree->GetEntries();
   printf("%d entries in chain.\n",nentries);

   char outfileName[200];
   char infileName[200];
   sprintf(outfileName,"%s_reduced.root",outfilePrefix.c_str() );
   sprintf(infileName,"%s.root",outfilePrefix.c_str());

   TFile * outfile = new TFile(outfileName,"RECREATE");
   TTree * data = new TTree();
   data->SetName("data");

  double dTotEDepActive_keV,
         dEscapeTime_ns,
         dTimeCapture_ns,
         dPosX, dPosY, dPosZ;
  vector<string> sActiveProcessNames;
  bool   bSSInActive,
         bUnresolvedMSInActive,
         bMSOtherMat,
         bEMEnergyDepActive,
         bEMEnergyDepPSD1,
         bEMEnergyDepPSD2,
         bNeutronCaptureInEvent,
         bNeutronHitPSD1,
         bNeutronHitPSD2,
         bNeutronEscape,
         bGammaEscape;
  vector<int> sOtherMaterials;
  string sCreatorProcess;
  int nEnergyDepositsActive;
  int nCloseEnergyDepositsActive;
  int nEnergyDepositsTotal;
  int nNuclearRecoilsActive;
  int nNuclearRecoilsInactiveTMS;
  int iNeutronCaptureVolume;
  long iNeutronCaptureNucleus;
  int iActiveRecoilNucleus; 
  int event_num;
  unsigned long long iParentParticleID;
  double firstActiveScatterPos[3];

  data->Branch("event_num",&event_num,"event_num/I");
  data->Branch("dTotEDepActive_keV",&dTotEDepActive_keV,"dTotEDepActive_keV/D");
  data->Branch("dTimeCapture_ns",&dTimeCapture_ns,"dTimeCapture_ns/D");
  data->Branch("sActiveProcessNames",&sActiveProcessNames);
  data->Branch("sOtherMaterials",&sOtherMaterials);
  data->Branch("bNeutronCaptureInEvent",&bNeutronCaptureInEvent,"bNeutronCaptureInEvent/O");
  data->Branch("iNeutronCaptureVolume",&iNeutronCaptureVolume,"iNeutronCaptureVolume/I");
  data->Branch("iNeutronCaptureNucleus",&iNeutronCaptureNucleus,"iNeutronCaptureNucleus/I");
  data->Branch("iActiveRecoilNucleus",&iActiveRecoilNucleus,"iActiveRecoilNucleus/I");
  data->Branch("sCreatorProcess",&sCreatorProcess);
  data->Branch("iParentParticleID",&iParentParticleID,"iParentParticleID/L");
  data->Branch("dPosX",&dPosX,"dPosX/D");
  data->Branch("dPosY",&dPosY,"dPosY/D");
  data->Branch("dPosZ",&dPosZ,"dPosZ/D");

  bSSInActive = false;
  bMSOtherMat = false;
  bEMEnergyDepPSD1 = false;
  bEMEnergyDepPSD2 = false;
  bEMEnergyDepActive = false;


  for(int i=0; i<nentries; i++){
      if( i % 10000 == 0 ) printf("i = %d\n",i);
      //eventsChain->GetEntry(i);
      eventsTree->GetEntry(i);
      event_num = i;

      dTotEDepActive_keV = 0.;
      dTimeCapture_ns = -1000.;
      dEscapeTime_ns = -1000.;
      sActiveProcessNames.clear();
      sOtherMaterials.clear();
      bSSInActive = false;
      bUnresolvedMSInActive = false;
      bMSOtherMat = false;
      bEMEnergyDepActive = false;
      bEMEnergyDepPSD1 = false;
      bEMEnergyDepPSD2 = false;
      bNeutronHitPSD1 = false;
      bNeutronHitPSD2 = false;
      bNeutronCaptureInEvent = false;
      iNeutronCaptureVolume = -1;
      iNeutronCaptureNucleus = -1;
      nEnergyDepositsActive = 0;
      nEnergyDepositsTotal = 0;
      nNuclearRecoilsActive = 0;
      nNuclearRecoilsInactiveTMS = 0;     
      iActiveRecoilNucleus = -1;
      bNeutronEscape = false;
      bGammaEscape = false;
      sCreatorProcess.clear();

      vector<stepInfo> steps;
      vector<trackInfo> tracks = current_evt->tracks;
      bool hitPSDAlready = false;
      bool hitActiveAlready = false;
      for(size_t tr=0; tr<tracks.size(); tr++) {
          steps = tracks[tr].steps;
          //if( steps[0].dTime_ns > 1.e5 ) continue; // Skip tracks that start 100us after event

          // Check for neutron capture
          if( tracks[tr].sCreatorProcess == "nCapture" ){
              bNeutronCaptureInEvent = true;
              if( tracks[tr].iParticleID > 1000000000 ){ 
                 iNeutronCaptureVolume = steps[0].iVolumeID;
                 iNeutronCaptureNucleus = (long) tracks[tr].iParticleID;
                 dTimeCapture_ns = steps[0].dTime_ns;
                 dPosX = steps[0].dPosition_mm[0];
                 dPosY = steps[0].dPosition_mm[1];
                 dPosZ = steps[0].dPosition_mm[2];
              }
          }
          // Record information if track is a recoiling nucleus in the active volume
          if( (tracks[tr].iParticleID > 1000000000 || tracks[tr].iParticleID == 2212) && tracks[tr].steps[0].sVolumeName == "active_tms_volume" ){
              nNuclearRecoilsActive++;
              iActiveRecoilNucleus = tracks[tr].iParticleID; 
          }
           


          // Loop over steps for each track
          for(size_t st=0; st<steps.size(); st++) {
              if( steps[st].sVolumeName == "active_tms_volume" ) {
                  if( tracks[tr].iParentID == 0 ) nNuclearRecoilsActive++; // count the non-tracked (<1keV) recoils
                  nEnergyDepositsTotal++;
                  dTotEDepActive_keV += steps[st].dEnergyDep_keV;
                  nEnergyDepositsActive++;
                  if( !hitActiveAlready ) {
                    firstActiveScatterPos[0] = steps[st].dPosition_mm[0];
                    firstActiveScatterPos[1] = steps[st].dPosition_mm[1];
                    firstActiveScatterPos[2] = steps[st].dPosition_mm[2];
                    bSSInActive = true;
                  } else {
                    //if( ComputeDistance(firstActiveScatterPos,steps[st].dPosition_mm) > 0.1 )
                        bSSInActive = false;
                        double d_from_first = sqrt( pow(steps[st].dPosition_mm[0] - firstActiveScatterPos[0],2.) +
                                                    pow(steps[st].dPosition_mm[1] - firstActiveScatterPos[1],2.) +
                                                    pow(steps[st].dPosition_mm[2] - firstActiveScatterPos[2],2.));
                        if( d_from_first < 10. ) { 
                            bUnresolvedMSInActive = true;
                        }
                        dTotEDepActive_keV += steps[st].dEnergyDep_keV;
                        nEnergyDepositsTotal++;
                        nEnergyDepositsActive++;
                  }
                  if( tracks[tr].iParticleID == 11 ||
                      tracks[tr].iParticleID == -11 ||
                      tracks[tr].iParticleID == 22 )
                      bEMEnergyDepActive = true;
                  sActiveProcessNames.push_back( steps[st].sProcess );
                  hitActiveAlready = true;
              } else { // end if volume = ls_target
                  if( steps[st].dEnergyDep_keV > 0. ) {
                      nEnergyDepositsTotal++;
                      if( steps[st].dTime_ns < 1.e6 ) {
                          bMSOtherMat = true;
                          sOtherMaterials.push_back( steps[st].iVolumeID );
                      }
                  }
             } // end else statement
             //printf("event_num: %d\t sVolumeName: %s\t track escaped = %d\n",i,steps[st].sVolumeName.c_str(),track_escaped);
          } // end steps loop  
      } // end tracks loop
      data->Fill();
  } // end events loop
  data->Write();
  outfile->Close();
  return 0;
} 


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
void Xe127SimsAnalysisEnvironment::SetOutputDir( string outDir_temp ) {
   outputDir = outDir_temp;
}
