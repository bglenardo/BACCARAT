#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <map>

#include "BaccRootConverterEvent.hh"

#include "XeNeuSimsAnalysisEnvironment.hh"

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
XeNeuSimsAnalysisEnvironment::XeNeuSimsAnalysisEnvironment() {

   inputFile = NULL;
   eventsTree = NULL;
   headerTree = NULL;
   eventsChain = NULL;
   current_evt = NULL;
   for(int i=0; i<10; i++) reducedEventsTree[i] = NULL;
   for(int i=0; i<10; i++) dataHistograms[i] = NULL;
   volumes = NULL;
   r.SetSeed(0);

   yieldDist = NULL;
   simBGDist = NULL;
   simSigDist = NULL;
   dataHist = NULL;

   ch_map.insert(std::make_pair(0,5));
   ch_map.insert(std::make_pair(1,6));
   ch_map.insert(std::make_pair(2,8));
   ch_map.insert(std::make_pair(3,9));
   ch_map.insert(std::make_pair(4,10));
   ch_map.insert(std::make_pair(5,11));
   ch_map.insert(std::make_pair(6,12));
   ch_map.insert(std::make_pair(7,13));
   ch_map.insert(std::make_pair(8,14));
   ch_map.insert(std::make_pair(9,15));

   computedTOFCuts = false;

   UseFiniteMCStats( false );

   SetEEE( 0.95 ); // fractional efficiency of electron extraction
   SetELifetime( 158. ); // us
   SetSESize( 57. );
   SetSEWidth( 0.203 ); // fractional width of SE
   SetDriftVelocity( 1.5 ); // mm/us
   SetFitMin( 1.5 ); // electrons
   SetFitMax( 70. ); // electrons
   SetScaleMean( 0.133 );
   SetScaleSig( 0.012 );
 

   doGaussianFit = false;
   applyScalePrior = false;

   chainsAdded = 0;

}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
XeNeuSimsAnalysisEnvironment::~XeNeuSimsAnalysisEnvironment() {
//   if( inputFile ) delete inputFile;
//   if( eventsTree ) delete eventsTree;
//   if( headerTree ) delete headerTree;
//   if( eventsChain ) delete eventsChain;
//   if( current_evt ) delete current_evt;

}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
void XeNeuSimsAnalysisEnvironment::SetCurrentEvt(int evt_num) {
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
bool XeNeuSimsAnalysisEnvironment::IsDataLoaded() {
  if( !eventsTree && !eventsChain ){ std::cout << "Tree or chain not loaded." << std::endl; return false; }
//  if( !headerTree ) return false;
//  if( !inputFile ) return false;
 
  return true;

}



//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int XeNeuSimsAnalysisEnvironment::PrintOutEnergyDepInfo() {

  if( !IsDataLoaded() ) {
     cout << "No data loaded! Please load data before running this function." << endl;
     return -1;
  } 

    for(int tr=0; tr<current_evt->tracks.size(); tr++) {
      printf("\n\tTrack %d,\t particle ID: %d,\t sCreatorProcess: %s, \t parent: %d\n",
             current_evt->tracks[tr].iTrackID,
             current_evt->tracks[tr].iParticleID,
             current_evt->tracks[tr].sCreatorProcess.c_str(),
             current_evt->tracks[tr].iParentID);
      for(int st=0; st<current_evt->tracks[tr].steps.size(); st++) {
         printf("\t\tVolume: %d, %s, Time: %f, Edep: %f, CurrentE: %f, Process: %s\n", current_evt->tracks[tr].steps[st].iVolumeID,
                                                  current_evt->tracks[tr].steps[st].sVolumeName.c_str(),
                                                  current_evt->tracks[tr].steps[st].dTime_ns,
                                                  current_evt->tracks[tr].steps[st].dEnergyDep_keV,
                                                  current_evt->tracks[tr].steps[st].dParticleEnergy_keV,
                                                  current_evt->tracks[tr].steps[st].sProcess.c_str()  );
      }
    }

  return 0;
}


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int XeNeuSimsAnalysisEnvironment::PrimaryNeutronScatteringVolumes() {

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

  return 0;
}


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int XeNeuSimsAnalysisEnvironment::DirectionOfPrimariesForScatteringEvents() {

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

  return 0;
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
bool XeNeuSimsAnalysisEnvironment::IsSingleScatter() {

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
int XeNeuSimsAnalysisEnvironment::LoadInputRootFile( string fname ) {

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
  return 0;
}


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int XeNeuSimsAnalysisEnvironment::PrintTree() {
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
int XeNeuSimsAnalysisEnvironment::PrintCurrentEvtDetails() {
  
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
int XeNeuSimsAnalysisEnvironment::FindVolumeID( string vname ) {
   
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
int XeNeuSimsAnalysisEnvironment::LoadInputChain( string dir ) {

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
int XeNeuSimsAnalysisEnvironment::AddInputChain( string dir ) {

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
double XeNeuSimsAnalysisEnvironment::ComputeDistance( double x1[3], double x2[3] ) {

  double r = sqrt( (x1[0] - x2[0])*(x1[0] - x2[0]) +
                   (x1[1] - x2[1])*(x1[1] - x2[1]) +
                   (x1[2] - x2[2])*(x1[2] - x2[2]) );
  return r;
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int XeNeuSimsAnalysisEnvironment::ReduceSimulatedData( string outfilename ) {
    
   if( !eventsChain || eventsChain->IsZombie() || eventsChainVec.size() == 0 ) {
      cout << "No files loaded into TChain! Nothing to reduce." << endl;
      return -1;
   }

   //int nentries = eventsChain->GetEntries();
   //printf("%d entries in chain.\n",nentries);

   TFile * outfile = new TFile(outfilename.c_str(),"RECREATE");
   TTree * data = new TTree();
   data->SetName("data");

  double dTotEDepXe_keV,
         dTotEDepXe_ER_keV,
         dTotEDepXe_NR_keV,
         dDeltaTXeLS_ns,
         dTLS_ns,
         dFirstXeT_ns,
         dLastXeT_ns,
         dFirstAlphaTAnticoincidenceDetector_ns,
         dFirstTAnticoincidenceDetector_ns,
         dTotEDepLS_keV,
         dFirstXeScatterEDep_keV,
         dSecondXeScatterEDep_keV,
         dTotEDepAnticoincidenceDetector_keV;
  double dElectrons;
  int nElectrons;
  double firstXeScatterPos_x;
  double firstXeScatterPos_y;
  double firstXeScatterPos_z;
  double firstLSScatterPos_x;
  double firstLSScatterPos_y;
  double firstLSScatterPos_z;
  double primaryEmissionAngle_deg;
  vector<string> sXeProcessNames;
  string sFileName;
  int    iEvtNum;
  bool   bSSInXe,
         bTrueSSInXe,
         bMSOtherMat,
         bHitMultipleBD,
         bGammaInXe,
         bGammaInLS,
         bNuclearRecoilInLS,
         bEventAboveEG,
         bAlphaInAnticoincidenceDetector;
  vector<int> sOtherMaterials;
  int bd_id;
  int bd_ch;
  int nScattersXe; 
  int creationVolumeID;
  double creationPos_x;
  double creationPos_y;
  double creationPos_z;
  int primaryAbsorptionVolumeID;
  double primaryAbsorptionPos_x;
  double primaryAbsorptionPos_y;
  double primaryAbsorptionPos_z;
  double standoff, energy, eventsizeRMS, eventsize_max_distance;
 
  data->Branch("dTotEDepXe_keV",&dTotEDepXe_keV,"dTotEDepXe_keV/D");
  data->Branch("dTotEDepXe_ER_keV",&dTotEDepXe_ER_keV,"dTotEDepXe_ER_keV/D");
  data->Branch("dTotEDepXe_NR_keV",&dTotEDepXe_NR_keV,"dTotEDepXe_NR_keV/D");
  data->Branch("dDeltaTXeLS_ns",&dDeltaTXeLS_ns,"dDeltaTXeLS_ns/D");
  data->Branch("dTotEDepAnticoincidenceDetector_keV",&dTotEDepAnticoincidenceDetector_keV,"dTotEDepAnticoincidenceDetector_keV/D");
  data->Branch("dFirstAlphaTAnticoincidenceDetector_ns",&dFirstAlphaTAnticoincidenceDetector_ns,"dFirstAlphaTAnticoincidenceDetector_ns/D");
  data->Branch("dFirstTAnticoincidenceDetector_ns",&dFirstTAnticoincidenceDetector_ns,"dFirstTAnticoincidenceDetector_ns/D");
  data->Branch("dTLS_ns",&dTLS_ns,"dTLS_ns/D");
  data->Branch("dFirstXeT_ns",&dFirstXeT_ns,"dFirstXeT_ns/D");
  data->Branch("dLastXeT_ns",&dLastXeT_ns,"dLastXeT_ns/D");
  data->Branch("dTotEDepLS_keV",&dTotEDepLS_keV,"dTotEDepLS_keV/D");
  data->Branch("sXeProcessNames",&sXeProcessNames);
  data->Branch("sFileName",&sFileName);
  data->Branch("iEvtNum",&iEvtNum);
  data->Branch("sOtherMaterials",&sOtherMaterials);
  data->Branch("bSSInXe",&bSSInXe,"bSSInXe/O");
  data->Branch("bTrueSSInXe",&bTrueSSInXe,"bTrueSSInXe/O");
  data->Branch("bMSOtherMat",&bMSOtherMat,"bMSOtherMat/O");
  data->Branch("bHitMultipleBD",&bHitMultipleBD,"bHitMultipleBD/O");
  data->Branch("bd_id",&bd_id,"bd_id/I");
  data->Branch("bd_ch",&bd_ch,"bd_ch/I");
  data->Branch("creationVolumeID",&creationVolumeID,"creationVolumeID/I");
  data->Branch("creationPos_x",&creationPos_x,"creationPos_x/D");
  data->Branch("creationPos_y",&creationPos_y,"creationPos_y/D");
  data->Branch("creationPos_z",&creationPos_z,"creationPos_z/D");
  data->Branch("primaryAbsorptionVolumeID",&primaryAbsorptionVolumeID,"primaryAbsorptionVolumeID/I");
  data->Branch("primaryAbsorptionPos_x",&primaryAbsorptionPos_x,"primaryAbsorptionPos_x/D");
  data->Branch("primaryAbsorptionPos_y",&primaryAbsorptionPos_y,"primaryAbsorptionPos_y/D");
  data->Branch("primaryAbsorptionPos_z",&primaryAbsorptionPos_z,"primaryAbsorptionPos_z/D");
  data->Branch("dElectrons",&dElectrons,"dElectrons/D");
  data->Branch("nElectrons",&nElectrons,"nElectrons/I");
  data->Branch("firstXeScatterPos_x",&firstXeScatterPos_x,"firstXeScatterPos_x/D");
  data->Branch("firstXeScatterPos_y",&firstXeScatterPos_y,"firstXeScatterPos_y/D");
  data->Branch("firstXeScatterPos_z",&firstXeScatterPos_z,"firstXeScatterPos_z/D");
  data->Branch("firstLSScatterPos_x",&firstLSScatterPos_x,"firstLSScatterPos_x/D");
  data->Branch("firstLSScatterPos_y",&firstLSScatterPos_y,"firstLSScatterPos_y/D");
  data->Branch("firstLSScatterPos_z",&firstLSScatterPos_z,"firstLSScatterPos_z/D");
  data->Branch("bGammaInXe",&bGammaInXe,"bGammaInXe/O");
  data->Branch("bGammaInLS",&bGammaInLS,"bGammaInLS/O");
  data->Branch("bNuclearRecoilInLS",&bNuclearRecoilInLS,"bNuclearRecoilInLS/O");
  data->Branch("bAlphaInAnticoincidenceDetector",&bAlphaInAnticoincidenceDetector,"bAlphaInAnticoincidenceDetector/O");
  data->Branch("nScattersXe",&nScattersXe,"nScattersXe/I");
  data->Branch("dFirstXeScatterEDep_keV",&dFirstXeScatterEDep_keV,"dFirstXeScatterEDep_keV/D");
  data->Branch("dSecondXeScatterEDep_keV",&dSecondXeScatterEDep_keV,"dSecondScatterEDep_keV/D");
  data->Branch("primaryEmissionAngle_deg",&primaryEmissionAngle_deg,"primaryEmissionAngle_deg/D");
  data->Branch("bEventAboveEG",&bEventAboveEG,"bEventAboveEG/O");
  data->Branch("standoff",&standoff,"standoff/D");
  data->Branch("eventsizeRMS",&eventsizeRMS,"eventsizeRMS/D");
  data->Branch("eventsize_max_distance",&eventsize_max_distance,"eventsize_max_distance/D");
  data->Branch("energy",&energy,"energy/D");

  bSSInXe = false;
  bMSOtherMat = false;

  for( int chidx =0; chidx < (int)eventsChainVec.size(); chidx++ ){

    eventsChain = eventsChainVec[chidx];

    for(int i=0; i<eventsChainEntriesVec[chidx]; i++){
    //for(int i=0; i<10; i++) { 
        if( i % 10000 == 0 ) printf("i = %d\n",i);
        eventsChain->GetEntry(i);
        sFileName = outfilename;
        iEvtNum = i;
        dTotEDepXe_keV = 0.;
        dTotEDepXe_ER_keV = 0.;
        dTotEDepXe_NR_keV = 0.;
        dTotEDepAnticoincidenceDetector_keV = 0.;
        dTLS_ns = -1000.;
        dFirstXeT_ns = -1000.;
        dLastXeT_ns = -1000.;
        dFirstAlphaTAnticoincidenceDetector_ns = -1000.;
        dFirstTAnticoincidenceDetector_ns = -1000.;
        dDeltaTXeLS_ns = -1000.;
        dTotEDepLS_keV = 0.;
        dFirstXeScatterEDep_keV = 0.;
        dSecondXeScatterEDep_keV = 0.;
        sXeProcessNames.clear();
        sOtherMaterials.clear();
        bSSInXe = true;
        bTrueSSInXe = false;
        bMSOtherMat = false;
        bHitMultipleBD = false;      
        bGammaInLS = false;
        bGammaInXe = false;
        bNuclearRecoilInLS = false;
        bAlphaInAnticoincidenceDetector = false;
        bEventAboveEG = false;
        firstXeScatterPos_x = -1000.;
        firstXeScatterPos_y = -1000.;
        firstXeScatterPos_z = -1000.; 
        firstLSScatterPos_x = -1000.;
        firstLSScatterPos_y = -1000.;
        firstLSScatterPos_z = -1000.; 
        nScattersXe = 0;
        eventsizeRMS = 0.;
        eventsize_max_distance = 0.;
        vector<double> edep_position_x;
        vector<double> edep_position_y;
        vector<double> edep_position_z; 
        vector<double> edep_energy;
        creationVolumeID = -1;
        creationPos_x = -1000.;
        creationPos_y = -1000.;
        creationPos_z = -1000.;

	if( current_evt->primaryParticles.size() == 0 ) continue;
 
        vector<stepInfo> steps;
        vector<trackInfo> tracks = current_evt->tracks;
        primaryParticleInfo primaryParticle = current_evt->primaryParticles[0];
        primaryEmissionAngle_deg = TMath::ACos(primaryParticle.dDirection[0])*180./TMath::Pi(); // r = 1
        primaryAbsorptionVolumeID = primaryParticle.iAbsorptionVolumeID;
        primaryAbsorptionPos_x = primaryParticle.dAbsorptionPos_mm[0];
        primaryAbsorptionPos_y = primaryParticle.dAbsorptionPos_mm[1];
        primaryAbsorptionPos_z = primaryParticle.dAbsorptionPos_mm[2];
        bool hitLSAlready = false;
        bool hitXeAlready = false;
        double tLS = 0.;
        double tXe = 0.;
        bd_ch = -1;
        bd_id = -1;
        standoff = 0.;
        energy = 0.;
        double standoff_r = 0.;
        double standoff_z = 0.; 
 
        for(size_t tr=0; tr<tracks.size(); tr++) {
            steps = tracks[tr].steps;
            for(size_t st=0; st<steps.size(); st++) {
                
                if( TMath::Sqrt( TMath::Power(steps[st].dPosition_mm[0],2.) + TMath::Power(steps[st].dPosition_mm[1],2.) ) > standoff_r ) {
                    standoff_r = TMath::Sqrt( TMath::Power(steps[st].dPosition_mm[0],2.) + TMath::Power(steps[st].dPosition_mm[1],2.) );
                }
                if( TMath::Abs( steps[st].dPosition_mm[2] ) > standoff_z ){
                    standoff_z = TMath::Abs( steps[st].dPosition_mm[2] );
                }

                //printf("Volume: %s\n",steps[st].sVolumeName.c_str());
                if( steps[st].sVolumeName == "liquid_xenon_target" || steps[st].sVolumeName == "liquid_xenon_above_eg" || 
                  steps[st].sVolumeName == "xe_target" ) {
                    energy += steps[st].dEnergyDep_keV;
                    //printf("Energy: %f \t X: %f \t Y: %f \t Z: %f \n",
                    //        steps[st].dEnergyDep_keV,steps[st].dPosition_mm[0],
                    //        steps[st].dPosition_mm[1],steps[st].dPosition_mm[2]);
                    edep_position_x.push_back(steps[st].dPosition_mm[0]);
                    edep_position_y.push_back(steps[st].dPosition_mm[1]);
                    edep_position_z.push_back(steps[st].dPosition_mm[2]);
                    edep_energy.push_back(steps[st].dEnergyDep_keV);
                    if( steps[st].dTime_ns > dLastXeT_ns ) {
                        dLastXeT_ns = steps[st].dTime_ns;
                    }
                    if( !hitXeAlready ) {
                      firstXeScatterPos_x = steps[st].dPosition_mm[0];
                      firstXeScatterPos_y = steps[st].dPosition_mm[1];
                      firstXeScatterPos_z = steps[st].dPosition_mm[2];
                      tXe = steps[st].dTime_ns;
                      dFirstXeT_ns = tXe;
                      dTotEDepXe_keV += steps[st].dEnergyDep_keV;
                      bTrueSSInXe = true;
                      dFirstXeScatterEDep_keV = steps[st].dEnergyDep_keV;
                      nScattersXe += 1;
                    } else {
                      if( TMath::Abs(firstXeScatterPos_z - steps[st].dPosition_mm[2]) < 3. ){
                          dTotEDepXe_keV += steps[st].dEnergyDep_keV;
                          dSecondXeScatterEDep_keV = steps[st].dEnergyDep_keV;
                          nScattersXe += 1;
                      }
                      if( steps[st].dPosition_mm[2] - firstXeScatterPos_z > 3. ) {
                          bSSInXe = false;
                          firstXeScatterPos_x = steps[st].dPosition_mm[0];
                          firstXeScatterPos_y = steps[st].dPosition_mm[1];
                          firstXeScatterPos_z = steps[st].dPosition_mm[2]; 
                          tXe = steps[st].dTime_ns;
                          dTotEDepXe_keV += steps[st].dEnergyDep_keV;
                          dSecondXeScatterEDep_keV = dFirstXeScatterEDep_keV;
                          dFirstXeScatterEDep_keV = steps[st].dEnergyDep_keV;
                          nScattersXe += 1;
                      }
                      bTrueSSInXe = false;
                    }
                    sXeProcessNames.push_back( steps[st].sProcess );
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ){ 
                        bGammaInXe = true;
                        dTotEDepXe_ER_keV += steps[st].dEnergyDep_keV;
                    } else {
                        dTotEDepXe_NR_keV += steps[st].dEnergyDep_keV;
                    }
                    creationVolumeID = tracks[tr].iCreationVolumeID;
                    creationPos_x = tracks[tr].dCreationPosition[0];
                    creationPos_y = tracks[tr].dCreationPosition[1];
                    creationPos_z = tracks[tr].dCreationPosition[2];
                    
                    if( steps[st].sVolumeName == "liquid_xenon_above_eg" )
                        bEventAboveEG = true;
                    hitXeAlready = true;
                } else // end if( volume == LXetarget )
                if( steps[st].sVolumeName == "migdal_neutron_tagging_detector" ){
                    //printf("tracks[tr].iParticleID: %d\n",tracks[tr].iParticleID);
                    dTotEDepAnticoincidenceDetector_keV += steps[st].dEnergyDep_keV;
                    if( tracks[tr].iParticleID == 1000020040 ){
                        //printf("Yes!\n");
                        bAlphaInAnticoincidenceDetector = true;
                        if( dFirstAlphaTAnticoincidenceDetector_ns < 0. )
                            dFirstAlphaTAnticoincidenceDetector_ns = steps[st].dTime_ns;
                    }     
                    if( dFirstTAnticoincidenceDetector_ns < 0. )
                        dFirstTAnticoincidenceDetector_ns = steps[st].dTime_ns;
                } else                
                if( steps[st].sVolumeName == "ls_target_71" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        if( hitXeAlready )
                            dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 71;
                        bd_ch = 5;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                //} else if( steps[st].sVolumeName.find("ls_target_81")!=std::string::npos ) {
                //   std::cout << steps[st].sVolumeName << std::endl;
                } else if( steps[st].sVolumeName == "ls_target_81" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 81;
                        bd_ch = 6;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    }                  
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else if( steps[st].sVolumeName == "tof_target_5" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 5;
                        bd_ch = 13;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else if( steps[st].sVolumeName == "tof_target_15" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 15;
                        bd_ch = 14;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else if( steps[st].sVolumeName == "tof_target_2" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 2;
                        bd_ch = 15;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else if( steps[st].sVolumeName == "tof_target_11" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 11;
                        bd_ch = 8;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else if( steps[st].sVolumeName == "tof_target_20" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 20;
                        bd_ch = 9;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else if( steps[st].sVolumeName == "tof_target_14" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 14;
                        bd_ch = 10;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else if( steps[st].sVolumeName == "tof_target_8" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 8;
                        bd_ch = 11;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else if( steps[st].sVolumeName == "tof_target_21" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 21;
                        bd_ch = 12;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else if( steps[st].sVolumeName == "tof_target_0" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        dTLS_ns = tLS;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 0;
                        bd_ch = 0;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    } else {
                        if( steps[st].dTime_ns < dTLS_ns )
                            dTLS_ns = steps[st].dTime_ns;
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else if( steps[st].sVolumeName == "ls_target_1" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        dTLS_ns = tLS;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 1;
                        bd_ch = 1;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    } else {
                        if( steps[st].dTime_ns < dTLS_ns )
                            dTLS_ns = steps[st].dTime_ns;
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else if( steps[st].sVolumeName == "ls_target_2" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        dTLS_ns = tLS;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 2;
                        bd_ch = 2;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    } else {
                        if( steps[st].dTime_ns < dTLS_ns )
                            dTLS_ns = steps[st].dTime_ns;
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else if( steps[st].sVolumeName == "ls_target_3" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        dTLS_ns = tLS;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 3;
                        bd_ch = 3;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    } else {
                        if( steps[st].dTime_ns < dTLS_ns )
                            dTLS_ns = steps[st].dTime_ns;
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else if( steps[st].sVolumeName == "ls_target_4" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        dTLS_ns = tLS;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 4;
                        bd_ch = 4;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    } else {
                        if( steps[st].dTime_ns < dTLS_ns )
                            dTLS_ns = steps[st].dTime_ns;
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else if( steps[st].sVolumeName == "ls_target_5" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        dTLS_ns = tLS;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 5;
                        bd_ch = 5;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    } else {
                        if( steps[st].dTime_ns < dTLS_ns )
                            dTLS_ns = steps[st].dTime_ns;
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else if( steps[st].sVolumeName == "ls_target_6" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        dTLS_ns = tLS;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 6;
                        bd_ch = 6;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    } else {
                        if( steps[st].dTime_ns < dTLS_ns )
                            dTLS_ns = steps[st].dTime_ns;
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else if( steps[st].sVolumeName == "ls_target_7" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        dTLS_ns = tLS;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 7;
                        bd_ch = 7;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    } else {
                        if( steps[st].dTime_ns < dTLS_ns )
                            dTLS_ns = steps[st].dTime_ns;
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else if( steps[st].sVolumeName == "ls_target_8" ) {
                    dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                    if( !hitLSAlready ) {
                        tLS = steps[st].dTime_ns;
                        dTLS_ns = tLS;
                        if(hitXeAlready)
                           dDeltaTXeLS_ns = tLS;
                        hitLSAlready = true;
                        bd_id = 8;
                        bd_ch = 8;
                        firstLSScatterPos_x = steps[st].dPosition_mm[0];
                        firstLSScatterPos_y = steps[st].dPosition_mm[1];
                        firstLSScatterPos_z = steps[st].dPosition_mm[2];
                    } else {
                        if( steps[st].dTime_ns < dTLS_ns )
                            dTLS_ns = steps[st].dTime_ns;
                    }
                    if( tracks[tr].iParticleID == 11 || tracks[tr].iParticleID == 22 ) 
                        bGammaInLS = true;
                    else if( tracks[tr].iParticleID == 2212 || tracks[tr].iParticleID > 1000020040 )
                        bNuclearRecoilInLS = true;
                } else { // end if volume = ls_target
                    //std::cout << steps[st].sVolumeName << std::endl;
                    if( steps[st].dEnergyDep_keV > 0. ) {
                        if( (!hitLSAlready || steps[st].dTime_ns < tLS) ) {
                            bMSOtherMat = true;
                            sOtherMaterials.push_back( steps[st].iVolumeID );
                        }
                    }
               } // end else statement
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
 
        // This block simulates a number of electrons from the energy
        double charge_yield = 6.2; // electrons per keV
        nElectrons = r.Poisson( charge_yield * dTotEDepXe_keV );
        nElectrons = r.Binomial( nElectrons, 0.95 );
        nElectrons = r.Binomial( nElectrons, 0.95 );
        dElectrons = r.Gaus((double)nElectrons,0.17*sqrt((double)nElectrons));
  
  
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
int XeNeuSimsAnalysisEnvironment::SetOutputDir( string outDir_temp ) {
   outputDir = outDir_temp;
   return 1;
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int XeNeuSimsAnalysisEnvironment::ReduceSimulatedDataByFile() {
    
   if( !eventsTree || eventsTree->IsZombie() ) {
      cout << "No files loaded into the eventsTree! Nothing to reduce." << endl;
      return -1;
   }

   int nentries = eventsTree->GetEntries();
   printf("%d entries in chain.\n",nentries);

   string outfileName = outputDir + "/Reduced_" + inputFileName;

   TFile * outfile = new TFile(outfileName.c_str(),"RECREATE");
   TTree * data = new TTree();
   data->SetName("data");

  double dTotEDepXe_keV,
         dDeltaTXeLS_ns,
         dTotEDepLS_keV;
  vector<string> sXeProcessNames;
  bool   bSSInXe,
         bMSOtherMat;
  vector<int> sOtherMaterials;
 
  double firstXeScatterPos[3];

  data->Branch("dTotEDepXe_keV",&dTotEDepXe_keV,"dTotEDepXe_keV/D");
  data->Branch("dDeltaTXeLS_ns",&dDeltaTXeLS_ns,"dDeltaTXeLS_ns/D");
  data->Branch("dTotEDepLS_keV",&dTotEDepLS_keV,"dTotEDepLS_keV/D");
  data->Branch("sXeProcessNames",&sXeProcessNames);
  data->Branch("sOtherMaterials",&sOtherMaterials);
  data->Branch("bSSInXe",&bSSInXe,"bSSInXe/O");
  data->Branch("bMSOtherMat",&bMSOtherMat,"bMSOtherMat/O");

  bSSInXe = false;
  bMSOtherMat = false;



  for(int i=0; i<nentries; i++){
      if( i % 10000 == 0 ) printf("i = %d\n",i);
      eventsTree->GetEntry(i);
      dTotEDepXe_keV = 0.;
      dDeltaTXeLS_ns = -1000.;
      dTotEDepLS_keV = 0.;
      sXeProcessNames.clear();
      sOtherMaterials.clear();
      bSSInXe = true;
      bMSOtherMat = false;
      
      vector<stepInfo> steps;
      vector<trackInfo> tracks = current_evt->tracks;
      bool hitLSAlready = false;
      bool hitXeAlready = false;
      double tLS = 0.;
      double tXe = 0.;

      for(size_t tr=0; tr<tracks.size(); tr++) {
          steps = tracks[tr].steps;
          for(size_t st=0; st<steps.size(); st++) {
             printf("Volume: %s\n",steps[st].sVolumeName.c_str());

              if( steps[st].sVolumeName == "liquid_xenon_target" ) {
                  dTotEDepXe_keV += steps[st].dEnergyDep_keV;
                  if( !hitXeAlready ) {
                    firstXeScatterPos[0] = steps[st].dPosition_mm[0];
                    firstXeScatterPos[1] = steps[st].dPosition_mm[1];
                    firstXeScatterPos[2] = steps[st].dPosition_mm[2];
                    tXe = steps[st].dTime_ns;
                  } else {
                    if( ComputeDistance(firstXeScatterPos,steps[st].dPosition_mm) > 2. )
                        bSSInXe = false;
                  }
                  sXeProcessNames.push_back( steps[st].sProcess );
                  hitXeAlready = true;
              } else // end if( volume == LXetarget ){
                if( steps[st].sVolumeName == "ls_target" ) {
                  dTotEDepLS_keV += steps[st].dEnergyDep_keV;
                  if( !hitLSAlready ) {
                      tLS = steps[st].dTime_ns;
                      if( hitXeAlready )
                          dDeltaTXeLS_ns = tLS - tXe;
                      hitLSAlready = true;
                  }
                
                } else { // end if volume = ls_target
                  if( steps[st].dEnergyDep_keV > 0. ) {
                      if( !hitLSAlready || steps[st].dTime_ns < tLS ) {
                          bMSOtherMat = true;
                          sOtherMaterials.push_back( steps[st].iVolumeID );
                      }
                  }
                
             } // end else statement
          } // end steps loop         
      } // end tracks loop
      data->Fill();
      //printf("XeEnergy: %f\t, LSEnergy: %f\n",dTotEDepXe_keV,dTotEDepLS_keV); 
  } // end events loop
  data->Write();
  outfile->Close();
  return 0;
} 

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int XeNeuSimsAnalysisEnvironment::AddReducedEventsFile( std::string reducedFileName ){
   
  reducedEventsFile = new TFile(reducedFileName.c_str());
  if(!reducedEventsFile || !(reducedEventsFile->IsOpen()) || reducedEventsFile->IsZombie()){
    std::cerr<<reducedFileName<<" can not be opened!"<<std::endl;
    return -1;
  } 

  char histname[100];

  for(int tr=0; tr<10; tr++) {

     sprintf(histname,"data_%02d",ch_map[tr]);
     printf("Loading %s...\n",histname);

     reducedEventsTree[tr] = (TTree *)reducedEventsFile->Get(histname);
     if(!reducedEventsTree[tr] || reducedEventsTree[tr]->IsZombie() || reducedEventsTree[tr]->GetEntries()<1){
       std::cerr<<"No data available. "<<std::endl;
       return -1;
     }
     cout << "reducedEventsTree[" << tr << "] pointer: " << reducedEventsTree[tr] << endl;
   
     std::cout << "Successfully loaded root file and trees." << std::endl;
     std::cout << "Setting addresses..." << std::endl;
     // Set the address of Events to evt
     reducedEventsTree[tr]->SetBranchAddress("firstXeScatterPos_x",&firstXeScatterPos_x);
     reducedEventsTree[tr]->SetBranchAddress("firstXeScatterPos_y",&firstXeScatterPos_y);
     reducedEventsTree[tr]->SetBranchAddress("firstXeScatterPos_z",&firstXeScatterPos_z);
     reducedEventsTree[tr]->SetBranchAddress("bd_ch",&bd_ch);
     reducedEventsTree[tr]->SetBranchAddress("dDeltaTXeLS_ns",&dDeltaTXeLS_ns);
     reducedEventsTree[tr]->SetBranchAddress("dTotEDepXe_keV",&dTotEDepXe_keV);
     reducedEventsTree[tr]->SetBranchAddress("dTotEDepLS_keV",&dTotEDepLS_keV);
     reducedEventsTree[tr]->SetBranchAddress("dFirstXeScatterEDep_keV",&dFirstXeScatterEDep_keV);
     reducedEventsTree[tr]->SetBranchAddress("dSecondXeScatterEDep_keV",&dSecondXeScatterEDep_keV);
     reducedEventsTree[tr]->SetBranchAddress("nScattersXe",&nScattersXe);
     reducedEventsTree[tr]->SetBranchAddress("bMSOtherMat",&bMSOtherMat);

  }

  return 0;

}
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int XeNeuSimsAnalysisEnvironment::GetIndFromChannelNum( int ch_num ){
    for( std::map<int,int>::iterator it = ch_map.begin(); it != ch_map.end(); it++ )
       if( it->second == ch_num ) return it->first;
}
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int XeNeuSimsAnalysisEnvironment::CheckForReducedEvents(){
  char histname[100];
  for(int tr=0; tr<10; tr++) {
     sprintf(histname,"data_%02d",ch_map[tr]);
     reducedEventsTree[tr] = (TTree *)reducedEventsFile->Get(histname);
     if(!reducedEventsTree[tr] || reducedEventsTree[tr]->IsZombie() || reducedEventsTree[tr]->GetEntries()<1){
       std::cerr << "No reducedEventTree's loaded. abort..." << std::endl;
       return -1;
     }
  }
  return 1;
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
void XeNeuSimsAnalysisEnvironment::ComputeTOFCuts() {
     if( CheckForReducedEvents() < 0 ) return;

     char tofHist_Name[100];

     for(int tr=0; tr<10; tr++ ){
         sprintf(tofHist_Name,"tof_%02d",ch_map[tr]);
         tofHistograms[tr] = new TH1F(tofHist_Name,tofHist_Name,400,0.,200.);
         char drawCmd[100];
         sprintf(drawCmd,"dDeltaTXeLS_ns>>%s",tofHist_Name);
         reducedEventsTree[tr]->Draw(drawCmd,"dTotEDepXe_keV>0. && dTotEDepXe_keV < 30.","goff");
         
         int thisbin = 1;
         while( tofHistograms[tr]->GetBinContent(thisbin) < tofHistograms[tr]->GetMaximum()/2. )
             thisbin++;
         tof_cut_lo[tr] = tofHistograms[tr]->GetBinLowEdge(thisbin);
         while( tofHistograms[tr]->GetBinContent(thisbin) > tofHistograms[tr]->GetMaximum()/2. )
            thisbin++;
         tof_cut_hi[tr] = tofHistograms[tr]->GetBinLowEdge(thisbin);
     } 
     computedTOFCuts = true;
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
void XeNeuSimsAnalysisEnvironment::PrintTOFCuts(){
  for(int tr=0; tr<10; tr++) {
      printf("Ch %d:\t%f\t%f\n",ch_map[tr],tof_cut_lo[tr],tof_cut_hi[tr]);
  }
}
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
void XeNeuSimsAnalysisEnvironment::GenerateYieldDist( double inputYield, double width, int ch ){

   double hmin,hmax;
   int num_bins;
   double yield = inputYield;

   if( dataHistograms[GetIndFromChannelNum(ch)] ){
       num_bins = dataHistograms[GetIndFromChannelNum(ch)]->GetNbinsX();
       hmin = dataHistograms[GetIndFromChannelNum(ch)]->GetBinLowEdge(1);
       hmax = dataHistograms[GetIndFromChannelNum(ch)]->GetBinLowEdge( num_bins ) + 
              dataHistograms[GetIndFromChannelNum(ch)]->GetBinWidth(1);
   } else {
       num_bins = 400;
       hmin = 0.;
       hmax = 80.;
   }

   if( !computedTOFCuts ) ComputeTOFCuts();

   if( yieldDist ) delete yieldDist;
   if( simBGDist ) delete simBGDist;
   if( simSigDist ) delete simSigDist;

   bool background_flag = false;


   int tree_idx = GetIndFromChannelNum( ch );
   if( !reducedEventsTree[tree_idx] ) {
       std::cerr << "No simulation data available." << std::endl;
       return;
   }

   yieldDist = new TH1F("yieldDist","yieldDist",num_bins,hmin,hmax);
   simBGDist = new TH1F("simBGDist","simBGDist",num_bins,hmin,hmax);
   simSigDist = new TH1F("simSigDist","simSigDist",num_bins,hmin,hmax);

   for(int i=0; i<reducedEventsTree[tree_idx]->GetEntries()*3./4.; i++) {
       background_flag = false; // Reset the background_flag to the default value.
       yield = inputYield; // Reset the yield to the default value.
       reducedEventsTree[tree_idx]->GetEntry(i);
       if( bd_ch != ch || dTotEDepXe_keV < 0.01 ||
           tof_cut_hi[tree_idx] < dDeltaTXeLS_ns || 
           tof_cut_lo[tree_idx] > dDeltaTXeLS_ns ||
           //pow(firstXeScatterPos_x,2.) + pow(firstXeScatterPos_y,2.) > (0.5*25.4)*(0.5*25.4) ||
           dTotEDepLS_keV < 300. ) 
           continue;

       // Turn energy into number of electrons
       int nelectrons=-1;
       if( doGaussianFit ){
           int iterations = 0;
           //printf("iteration = %d\n",iterations);
           //printf("nScattersXe = %d\n",nScattersXe);
           //printf("bMSOtherMat = %d\n",(int)bMSOtherMat);
           while( nelectrons < 0 ) {
             if( nScattersXe == 1 ){
                if( bMSOtherMat ) { yield = ComputeBGYield( dTotEDepXe_keV ); background_flag = true; } 
                //printf("yield: %f\n",yield);             
                nelectrons = (int) TMath::Nint( r.Gaus( yield*dTotEDepXe_keV, 
                                                        width*TMath::Sqrt(yield*dTotEDepXe_keV) ) );
             } else if ( nScattersXe > 1 ) {
                background_flag = true;

                yield = ComputeBGYield( dFirstXeScatterEDep_keV );
                nelectrons += (int) TMath::Nint( r.Gaus( yield*dFirstXeScatterEDep_keV, 
                                                         width*TMath::Sqrt(yield*dFirstXeScatterEDep_keV) ) );
                yield = ComputeBGYield( dSecondXeScatterEDep_keV );
                nelectrons += (int) TMath::Nint( r.Gaus( yield*dSecondXeScatterEDep_keV, 
                                                         width*TMath::Sqrt(yield*dSecondXeScatterEDep_keV) ) );
             } else continue;
             if( iterations > 100 ) {
                 printf("More than 100 iterations! Mean: %3.3f Sig: %3.3f\n",
                         yield*dTotEDepXe_keV,width);
                 printf("nScattersXe = %d\n",nScattersXe);
                 printf("Energy: %f\n",dTotEDepXe_keV);
                 nelectrons = 0;
                 break;
             }
             iterations++;
           }
       } else { 
           nelectrons = r.Poisson( yield * dTotEDepXe_keV );
             if( nScattersXe == 1 ){
                if( bMSOtherMat ) { yield = ComputeBGYield( dTotEDepXe_keV ); background_flag=true; }
                nelectrons = (int) r.Poisson( yield*dTotEDepXe_keV ) ;
             } else if ( nScattersXe > 1 ) {
                background_flag = true;
                yield = ComputeBGYield( dFirstXeScatterEDep_keV );
                nelectrons += (int) r.Poisson( yield*dFirstXeScatterEDep_keV );
                yield = ComputeBGYield( dSecondXeScatterEDep_keV );
                nelectrons += (int) r.Poisson( yield*dSecondXeScatterEDep_keV );
             } else continue;
       }

       // Fluctuate to account for eee
       nelectrons = r.Binomial(nelectrons,eee);

       // Fluctuate according to electron lifetime
       // The liquid surface is at 16.1mm in the simulation
       double delectrons = (double) r.Binomial( nelectrons,
                                    TMath::Exp(-((16.1-firstXeScatterPos_z)/drift_speed)
                                                 /elifetime) );

       delectrons = r.Gaus(se_size*delectrons, se_width*TMath::Sqrt(delectrons) );
       yieldDist->Fill(delectrons);
       if( background_flag ) simBGDist->Fill(delectrons);
       else simSigDist->Fill(delectrons);
   }
   yieldDist->SetLineWidth(2);
   yieldDist->Sumw2();
   simSigDist->SetLineWidth(2);
   simSigDist->Sumw2();
   simBGDist->SetLineWidth(2);
   simBGDist->Sumw2();

}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
void XeNeuSimsAnalysisEnvironment::GenerateDataFromSim( double inputYield, double width, double scale, int ch ){

   double hmin,hmax;
   int num_bins;
   double yield = inputYield;

   if( dataHistograms[GetIndFromChannelNum(ch)] ){
       num_bins = dataHistograms[GetIndFromChannelNum(ch)]->GetNbinsX();
       hmin = dataHistograms[GetIndFromChannelNum(ch)]->GetBinLowEdge(1);
       hmax = dataHistograms[GetIndFromChannelNum(ch)]->GetBinLowEdge( num_bins ) + 
              dataHistograms[GetIndFromChannelNum(ch)]->GetBinWidth(1);
   } else {
       num_bins = 400;
       hmin = 0.;
       hmax = 80.;
   }

   if( !computedTOFCuts ) ComputeTOFCuts();

   if( simDataDist ) delete simDataDist;

   bool background_flag = false;


   int tree_idx = GetIndFromChannelNum( ch );
   if( !reducedEventsTree[tree_idx] ) {
       std::cerr << "No simulation data available." << std::endl;
       return;
   }

   simDataDist = new TH1F("simDataDist","simDataDist",num_bins,hmin,hmax);

   for(int i=0; i<reducedEventsTree[tree_idx]->GetEntries()*3./4.; i++) {
       if( r.Uniform() > scale ) continue;
       background_flag = false; // Reset the background_flag to the default value.
       yield = inputYield; // Reset the yield to the default value.
       reducedEventsTree[tree_idx]->GetEntry(i);
       if( bd_ch != ch || dTotEDepXe_keV < 0.01 ||
           tof_cut_hi[tree_idx] < dDeltaTXeLS_ns || 
           tof_cut_lo[tree_idx] > dDeltaTXeLS_ns ||
           //pow(firstXeScatterPos_x,2.) + pow(firstXeScatterPos_y,2.) > (0.5*25.4)*(0.5*25.4) ||
           dTotEDepLS_keV < 300. ) 
           continue;

       // Turn energy into number of electrons
       int nelectrons=-1;
       if( doGaussianFit ){
           int iterations = 0;
           //printf("iteration = %d\n",iterations);
           //printf("nScattersXe = %d\n",nScattersXe);
           //printf("bMSOtherMat = %d\n",(int)bMSOtherMat);
           while( nelectrons < 0 ) {
             if( nScattersXe == 1 ){
                if( bMSOtherMat ) { yield = ComputeBGYield( dTotEDepXe_keV ); background_flag = true; } 
                //printf("yield: %f\n",yield);             
                nelectrons = (int) TMath::Nint( r.Gaus( yield*dTotEDepXe_keV, 
                                                        width*TMath::Sqrt(yield*dTotEDepXe_keV) ) );
             } else if ( nScattersXe > 1 ) {
                background_flag = true;

                yield = ComputeBGYield( dFirstXeScatterEDep_keV );
                nelectrons += (int) TMath::Nint( r.Gaus( yield*dFirstXeScatterEDep_keV, 
                                                         width*TMath::Sqrt(yield*dFirstXeScatterEDep_keV) ) );
                yield = ComputeBGYield( dSecondXeScatterEDep_keV );
                nelectrons += (int) TMath::Nint( r.Gaus( yield*dSecondXeScatterEDep_keV, 
                                                         width*TMath::Sqrt(yield*dSecondXeScatterEDep_keV) ) );
             } else continue;
             if( iterations > 100 ) {
                 printf("More than 100 iterations! Mean: %3.3f Sig: %3.3f\n",
                         yield*dTotEDepXe_keV,width);
                 printf("nScattersXe = %d\n",nScattersXe);
                 printf("Energy: %f\n",dTotEDepXe_keV);
                 nelectrons = 0;
                 break;
             }
             iterations++;
           }
       } else { 
           nelectrons = r.Poisson( yield * dTotEDepXe_keV );
             if( nScattersXe == 1 ){
                if( bMSOtherMat ) { yield = ComputeBGYield( dTotEDepXe_keV ); background_flag=true; }
                nelectrons = (int) r.Poisson( yield*dTotEDepXe_keV ) ;
             } else if ( nScattersXe > 1 ) {
                background_flag = true;
                yield = ComputeBGYield( dFirstXeScatterEDep_keV );
                nelectrons += (int) r.Poisson( yield*dFirstXeScatterEDep_keV );
                yield = ComputeBGYield( dSecondXeScatterEDep_keV );
                nelectrons += (int) r.Poisson( yield*dSecondXeScatterEDep_keV );
             } else continue;
       }

       // Fluctuate to account for eee
       nelectrons = r.Binomial(nelectrons,eee);

       // Fluctuate according to electron lifetime
       // The liquid surface is at 16.1mm in the simulation
       double delectrons = (double) r.Binomial( nelectrons,
                                    TMath::Exp(-((16.1-firstXeScatterPos_z)/drift_speed)
                                                 /elifetime) );

       delectrons = r.Gaus(se_size*delectrons, se_width*TMath::Sqrt(delectrons) );
       simDataDist->Fill(delectrons);
   }
   simDataDist->SetLineWidth(2);
   simDataDist->Sumw2();

}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
double XeNeuSimsAnalysisEnvironment::ComputeBGYield( double energy ){
  // Empirical function fitted to first round of computed yields
  return TMath::Exp( 1.82635 + 
                     0.307611*TMath::Log(energy) - 
                     0.12926*TMath::Power( TMath::Log(energy), 2.) );
}
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int XeNeuSimsAnalysisEnvironment::AddDataHistFile( std::string dataHistFileName ) {

  dataHistFile = new TFile(dataHistFileName.c_str());
  if(!dataHistFile || !(dataHistFile->IsOpen()) || dataHistFile->IsZombie()){
    std::cerr<<dataHistFileName<<" can not be opened!"<<std::endl;
    return -1;
  } 

  dataHist = (TH1F *)dataHistFile->Get("hefinal");
  if(!dataHist){
    std::cerr<<"No data available. "<<std::endl;
    return -1;
  }
  cout << "dataHist pointer: " << dataHist << endl;

  std::cout << "Successfully loaded root file and trees." << std::endl;
  std::cout << "Setting addresses..." << std::endl;

  dataHist->Rebin(1);
  dataHist->SetLineColor(1);
  dataHist->SetLineWidth(2);  
//  delete dataHistFile;
  return 0;

}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
int XeNeuSimsAnalysisEnvironment::AddDataHistFiles( int dataset_id ) {

    char dataHistFileName[100];
    for( int i=0; i<10; i++) {

       if( dataHistograms[i] ) { 
           std::cout << "Data histograms already loaded! Clearing and reloading..." << std::endl;
           delete dataHistograms[i];
       }
       sprintf(dataHistFileName,"/p/lustre1/xu12/spect4/Spect_Ch%d_%d.root",ch_map[i],dataset_id);
       if( AddDataHistFile( dataHistFileName ) < 0 ) return -1;
       dataHistograms[i] = (TH1F *)dataHist->Clone();
       dataHistograms[i]->SetDirectory(0);
       // Rebin the histograms so they're not so weird.
       int rebin_value = TMath::Nint((dataHistograms[i]->GetMean()/58.-4.)/5.) * 4;
       if( rebin_value == 0 ) rebin_value = 1;
       if( rebin_value == 4 ) rebin_value = 2;
       if( rebin_value == 12 ) rebin_value = 16;
       dataHistograms[i]->Rebin(rebin_value);
    }
    if( dataset_id == 101 ){
      SetDriftVelocity( 1.546 );
      SetELifetime( 426. ); 
    } else if( dataset_id == 102 ) {
      SetDriftVelocity( 1.883 );
      SetELifetime( 158. );
    } else if( dataset_id == 103 ) {
      SetDriftVelocity( 2.284 );
      SetELifetime( 154. );
    } else printf("\nWARNING!!! UNKNOWN DATASET. DRIFT SPEED AND PURITY WILL BE INCORRECT.\n\n");
    return 0;
}
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
TH1F * XeNeuSimsAnalysisEnvironment::GetDataHistByChannel( int ch_num ){
     if( !dataHistograms[GetIndFromChannelNum(ch_num)] ) {
          std::cerr << "Data hist empty." << std::endl;
          return NULL;
     }
     return dataHistograms[GetIndFromChannelNum(ch_num)];
}
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
TH1F * XeNeuSimsAnalysisEnvironment::GetDataHistByIdx( int ch_idx ){
     if( !dataHistograms[ch_idx] ) {
          std::cerr << "Data hist empty." << std::endl;
          return NULL;
     }
     return dataHistograms[ch_idx];
}
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
TH1F * XeNeuSimsAnalysisEnvironment::GetYieldDist() {
  return yieldDist;
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
TH1F * XeNeuSimsAnalysisEnvironment::GetSimBGDist() {
  return simBGDist;
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
TH1F * XeNeuSimsAnalysisEnvironment::GetSimDataDist() {
  return simDataDist;
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
TH1F * XeNeuSimsAnalysisEnvironment::GetSimSigDist() {
  return simSigDist;
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
TH1F * XeNeuSimsAnalysisEnvironment::GetDataHist() {
  return dataHist;
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
double XeNeuSimsAnalysisEnvironment::ComputeNegativeLogLikelihood( double scale, double yield, 
                                                                   double width, int ch_num ){
   GenerateYieldDist( yield, width, ch_num );

//   double fit_min = 6.; // electrons
//   double fit_max = 50.; // electrons

   int dist_idx = GetIndFromChannelNum( ch_num );

   double logLikelihood = 0.;

   if( !dataHistograms[dist_idx] ) {
       std::cerr << "No data histograms loaded! Abort ComputeNegativeLogLikelihood()." << std::endl;
       return 0.;
   }
   if( !yieldDist ) {
       std::cerr << "No yield distribution computed! Abort ComputeNegativeLogLikelihood()." << std::endl;
       return 0.;
   }      
    
   // Loop over all bins
   for(int i=1; i<dataHistograms[dist_idx]->GetNbinsX(); i++) {
       // skip bins outside the fit range
       if( dataHistograms[dist_idx]->GetBinCenter(i) < fit_min ||
           dataHistograms[dist_idx]->GetBinCenter(i) > fit_max ) 
           continue;
        
       // For now, skip MC bins with no events.
       if( yieldDist->GetBinContent(i) == 0 ) continue;
       

        // Using the notation from Barlow and Beeston '93
        //    f_i = predicted data in bin i (equal to MC value divided by scale factor)
        //    d_i = observed data in bin i 
        //    a_i = MC number of events in bin i
        //    A_i = predicted number of MC events in bin i
        //    t_i = (1 - d_i/f_i)
        double f_i = yieldDist->GetBinContent(i) * scale;
        double d_i = dataHistograms[dist_idx]->GetBinContent(i);

        double a_i = yieldDist->GetBinContent(i);
        double t_i = (1. - d_i/f_i);
        double A_i = a_i/(1 + scale*t_i);

       if( yieldDist->GetBinCenter(i) > se_size + 2*se_width ) 
           logLikelihood += d_i * TMath::Log( f_i ) - f_i;
       else
           logLikelihood += -(d_i - f_i)*(d_i - f_i) / 
                             (2 * TMath::Power(dataHistograms[dist_idx]->GetBinError(i),2.) );

       if( finiteMCStatsFit ) {
           printf("NOTE: finiteMCStats not implemented! Returning garbage.\n");
           return -1.;
           //A_i = yieldDist->GetBinContent(i) / 
       } 
       if( applyScalePrior ) {
           logLikelihood -= (scale - scaleMean)*(scale - scaleMean) / 
                            ( 2. * scaleSig * scaleSig );
       }

   }

   return (-1.)*logLikelihood;
   
}
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
double XeNeuSimsAnalysisEnvironment::ComputeLikelihoodLambda( double scale, double yield, 
                                                              double width, int ch_num ){
   GenerateYieldDist( yield, width, ch_num );

//   double fit_min = 6.; // electrons
//   double fit_max = 50.; // electrons

   int dist_idx = GetIndFromChannelNum( ch_num );

   double negLogLambda = 0.;
   int nbins_in_fit = 0;

   if( !dataHistograms[dist_idx] ) {
       std::cerr << "No data histograms loaded! Abort ComputeNegativeLogLikelihood()." << std::endl;
       return 0.;
   }
   if( !yieldDist ) {
       std::cerr << "No yield distribution computed! Abort ComputeNegativeLogLikelihood()." << std::endl;
       return 0.;
   }      
    
   // Loop over all bins
   for(int i=1; i<dataHistograms[dist_idx]->GetNbinsX(); i++) {
       // skip bins outside the fit range
       if( dataHistograms[dist_idx]->GetBinCenter(i) < fit_min ||
           dataHistograms[dist_idx]->GetBinCenter(i) > fit_max ) 
           continue;
       nbins_in_fit++;        

       // For now, skip MC bins with no events.
       if( yieldDist->GetBinContent(i) == 0 ) continue;
       // For now, also skip data bins with no events.
       if( dataHistograms[dist_idx]->GetBinContent(i) == 0) {
           ;//printf("WARNING: data bins have 0 events.\n");
           //printf("This means that the statistical test may not be totally defensible.\n");
           // continue;
       } 

        // Using the notation from Barlow and Beeston '93
        //    y_i = predicted data in bin i (equal to MC value divided by scale factor)
        //    n_i = observed data in bin i 
        double y_i = yieldDist->GetBinContent(i) * scale;
        double n_i = dataHistograms[dist_idx]->GetBinContent(i);


       if( yieldDist->GetBinCenter(i) > se_size + 2*se_width ) {
           if( n_i > 0 )
             negLogLambda += 2 * ( y_i - n_i + n_i * TMath::Log(n_i/y_i) );
           else
             negLogLambda += 2 * ( y_i );
       } else {
           negLogLambda += (n_i - y_i)*(n_i - y_i) / 
                             ( TMath::Power(dataHistograms[dist_idx]->GetBinError(i),2.) );
       }
       if( negLogLambda != negLogLambda ){
           printf("NAN occured at step number %d\n",i);
           printf("n_i = %f\n",n_i);
           printf("y_i = %f\n",y_i);
           printf("Bin value: %f\n",dataHistograms[dist_idx]->GetBinCenter(i));
           printf("Exiting....\n");
           return -1.;
       }


       if( finiteMCStatsFit ) {
           printf("NOTE: finiteMCStats not implemented! Returning garbage.\n");
           return -1.;
           //A_i = yieldDist->GetBinContent(i) / 
       } 
   }

   printf("********************************************************\n");
   printf("* Goodness of fit evaluation for Ch %d\n",ch_num);
   printf("********************************************************\n");
   printf("-2 ln(lambda): %f\n",negLogLambda);
   printf("Num Bins in fit: %d\n",nbins_in_fit);
   printf("Num parameters: 3\n");
   printf("DOF: %d\n",nbins_in_fit-3);
   printf("P-value, assuming chi2 limit: %f\n",TMath::Prob(negLogLambda,nbins_in_fit-3));

   return negLogLambda;
   
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
double XeNeuSimsAnalysisEnvironment::ComputeSimLikelihoodLambda( double scale, double yield, 
                                                              double width, int ch_num ){
   GenerateYieldDist( yield, width, ch_num );
   GenerateDataFromSim( yield,width,scale,ch_num); 

//   double fit_min = 6.; // electrons
//   double fit_max = 50.; // electrons

   int dist_idx = GetIndFromChannelNum( ch_num );

   double negLogLambda = 0.;
   int nbins_in_fit = 0;

   if( !simDataDist ) {
       std::cerr << "No data histograms loaded! Abort ComputeNegativeLogLikelihood()." << std::endl;
       return 0.;
   }
   if( !yieldDist ) {
       std::cerr << "No yield distribution computed! Abort ComputeNegativeLogLikelihood()." << std::endl;
       return 0.;
   }      
    
   // Loop over all bins
   for(int i=1; i<simDataDist->GetNbinsX(); i++) {
       // skip bins outside the fit range
       if( simDataDist->GetBinCenter(i) < fit_min ||
           simDataDist->GetBinCenter(i) > fit_max ) 
           continue;
       nbins_in_fit++;        

       // For now, skip MC bins with no events.
       if( yieldDist->GetBinContent(i) == 0 ) continue;
       // For now, also skip data bins with no events.
//       if( simDataDist->GetBinContent(i) == 0) {
//           printf("WARNING: data bins have 0 events.\n");
//           printf("This means that the statistical test may not be totally defensible.\n");
//           // continue;
//       } 

        // Using the notation from Barlow and Beeston '93
        //    y_i = predicted data in bin i (equal to MC value divided by scale factor)
        //    n_i = observed data in bin i 
        double y_i = yieldDist->GetBinContent(i) * scale;
        double n_i = simDataDist->GetBinContent(i);


       if( yieldDist->GetBinCenter(i) > se_size + 2*se_width ) {
           if( n_i > 0 )
             negLogLambda += 2 * ( y_i - n_i + n_i * TMath::Log(n_i/y_i) );
           else
             negLogLambda += 2 * ( y_i );
       } else {
           negLogLambda += (n_i - y_i)*(n_i - y_i) / 
                             ( TMath::Power(simDataDist->GetBinError(i),2.) );
       }
       if( negLogLambda != negLogLambda ){
           printf("NAN occured at step number %d\n",i);
           printf("n_i = %f\n",n_i);
           printf("y_i = %f\n",y_i);
           printf("Bin value: %f\n",simDataDist->GetBinCenter(i));
           printf("Exiting....\n");
           return -1.;
       }


       if( finiteMCStatsFit ) {
           printf("NOTE: finiteMCStats not implemented! Returning garbage.\n");
           return -1.;
           //A_i = yieldDist->GetBinContent(i) / 
       } 
   }

//   printf("********************************************************\n");
//   printf("* Goodness of fit evaluation for Ch %d\n",ch_num);
//   printf("********************************************************\n");
//   printf("-2 ln(lambda): %f\n",negLogLambda);
//   printf("Num Bins in fit: %d\n",nbins_in_fit);
//   printf("Num parameters: 3\n");
//   printf("DOF: %d\n",nbins_in_fit-3);
//   printf("P-value, assuming chi2 limit: %f\n",TMath::Prob(negLogLambda,nbins_in_fit-3));

   return negLogLambda;
   
}


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------

TGraphErrors * XeNeuSimsAnalysisEnvironment::DrawAndPrintPlots( std::string outfilename, int ch_num ,double scale_factor=1.){

   int idx = GetIndFromChannelNum(ch_num);

   if( !dataHistograms[idx] ) {
       std::cerr << "No data histograms loaded! Abort DrawAndPrintPlots()." << std::endl;
       return NULL;
   }
   if( !yieldDist ) {
       std::cerr << "No yield distribution computed! Abort DrawAndPrintPlots()." << std::endl;
       return NULL;
   }      

   yieldDist->Scale(scale_factor);

   TCanvas * c1 = new TCanvas("c1","c1",750,600);

   TPad * pad1 = new TPad("pad1","pad1",0.,0.4,1,1);
   TPad * pad2 = new TPad("pad2","pad2",0.,0.,1,0.4);
   pad1->SetBottomMargin(0.01);
   pad1->SetBorderMode(0);
   pad2->SetTopMargin(0.01);
   pad2->SetBorderMode(0);
   pad1->Draw();
   pad2->Draw();
   

   pad1->cd();
   double histYmax = yieldDist->GetMaximum() * 1.5;
   double histXmax;
   if( yieldDist->GetMean() > 5. ) 
        histXmax = yieldDist->GetMean() * 2.;
   else
        histXmax = 10.;
   yieldDist->GetXaxis()->SetTitle("");
   yieldDist->GetYaxis()->SetTitle("Counts");
   yieldDist->SetTitle("");
   yieldDist->SetStats(0);
   dataHistograms[idx]->SetStats(0);
   yieldDist->GetXaxis()->SetRangeUser(0.,histXmax);
   yieldDist->GetYaxis()->SetTitleSize(0.05);
   yieldDist->GetYaxis()->SetLabelSize(0.05);
   yieldDist->GetYaxis()->SetRangeUser(0.,histYmax);
   yieldDist->Draw("hist");
   dataHistograms[idx]->Draw("e0same");

   TGraphErrors * gres = new TGraphErrors(); 
   gres->SetName("gres");
   for(int i=1; i<yieldDist->GetNbinsX(); i++){
       gres->SetPoint(i-1, yieldDist->GetBinCenter(i), 
                      (dataHistograms[idx]->GetBinContent(i)-yieldDist->GetBinContent(i)) /
                      (yieldDist->GetBinContent(i)+0.1) );
       double edat = dataHistograms[idx]->GetBinError(i);
       double esim = yieldDist->GetBinError(i);
       gres->SetPointError( i-1, yieldDist->GetBinWidth(i)/2.,
                       TMath::Sqrt( edat*edat + esim*esim )/(yieldDist->GetBinContent(i)+0.1) );
   }
   gres->SetLineColor(1);
   gres->SetLineWidth(2);
   gres->GetYaxis()->SetRangeUser(-3.,3.);
   gres->GetXaxis()->SetRangeUser(0.,histXmax);
   gres->GetXaxis()->SetLabelSize(0.1);
   gres->GetXaxis()->SetTitle("Detected electrons");
   gres->GetXaxis()->SetTitleSize(0.1);
   gres->GetYaxis()->SetLabelSize(0.05); 
 
   pad2->cd();
   gres->Draw("AP");
   TLine * line = new TLine(0.,0.,histXmax,0.);
   line->SetLineColor(13);
   line->SetLineWidth(2);
   line->Draw("SAME");

   c1->Print(outfilename.c_str());

   return gres;

}
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
//int XeNeuSimsAnalysisEnvironment::PrintTree() {
//  cout << "eventsTree pointer: " << eventsTree << endl;
//  if( !eventsTree && !eventsChain ) {
//     cout << "No data loaded!" << endl;
//
//
//
//
//}
//}
