////////////////////////////////////////////////////////////////////////////////
/*BaccMCTruth.cc
 *
 * A reader whose input is a BaccRootConverter ROOT file, and whose output is 3 TTrees.
 * These trees contain Monte Carlo truth information.
 * PMTBanks TTree contains names of PMT types. There is just one entry.
 * PhotonMCTruth TTree contains photon level information, i.e. one entry per event with one vector element per PMT photon hit.
 * VertexMCTruth TTree contains vertex level information, i.e. one entry per event with one vector element per interaction in the LXe target. This uses a clustering algorithm
 *
 * N.B. This code contains some hardcoded physics, namely "clusterRadius = 0.4 mm".
 *
 ********************************************************************************
 * Change log
 *      (old change log from LUXMCTruth) 6th Oct 2016 - PMTStream copy created
 *      (from Gitlab branch ACK-RunNumber) under new name LUXMCTruth, MCTruth information
 *      added (TD)
 *      13th Dec 2016 - Initial import copied from LUXSim's LUXMCTruth.cc
 *      (commit 5e32d73aacde9167dd1cd4d8dbf1e98d18daa7c6)
 *      25th Jan 2017 - Data structure changed to use BaccMCTruthEvent object
 */
////////////////////////////////////////////////////////////////////////////////

//
//  Executable includes
//
#include "BaccRootConverterEvent.hh"
//#include "BaccRootConverterEvent_dict.h"
#include "BaccMCTruthEvent.hh"
//#include "BaccMCTruthEvent_dict.h"


//
//ROOT includes
//
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TObject.h"
//
//C/C++ includes
//
#include <fstream>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <sstream>
#include <numeric>

//
//Definitions
//
#define DEBUGGING 0

using namespace std;

//------++++++------++++++------++++++------++++++------++++++------++++++------
//
//  BaccRootConverterEvent()
//
//------++++++------++++++------++++++------++++++------++++++------++++++------
// BaccRootConverterEvent::BaccRootConverterEvent() { ClearEverything(); }
// void BaccRootConverterEvent::ClearEverything() {
//     iEventNumber = -1;
//     primaryParticles.clear();
//     tracks.clear();
//     volumes.clear();
// };

struct PMTPhoton //for internal use only
{
  int index;

  double time;
  double wavelength;

  int iInteractionIdentifier;
  int iVertexNumber;
  double arrivaltime;
};


bool comparePhotons(PMTPhoton A, PMTPhoton B)
{
  if(A.index != B.index) //index = Geant4 volume number of photocathodes
    return A.index < B.index;
  else
    return A.time < B.time;
}


int main(int argc, char *argv[])
{
  TFile* infile = new TFile(argv[1]);

  TString outfilename = argv[1];
  outfilename.ReplaceAll(".root","_mctruth.root");
  TFile* outfile = new TFile(outfilename,"RECREATE");

  TTree* headertree = (TTree*)infile->Get("HeaderTree");
  TTree* headertree2 = headertree->CloneTree();

  TTree* datatree = (TTree*)infile->Get("DataTree");



  /////// Check that record levels have been set correct ///////
  string* inputCommands=0;
  headertree->SetBranchAddress("sInputCommands",&inputCommands);
  headertree->GetEntry(0);

  vector<TString> rec;
  vector<TString> vol;
  vector<TString> lvl;

  std::istringstream iss(inputCommands->c_str());
  for (std::string line; std::getline(iss, line); ){
    TString tline(line);
    if(tline.Contains(" ")){

      TString front = tline.Copy();
      TString mid = tline.Copy();
      TString end = tline.Copy();
      front.Replace(front.First(' '),front.Sizeof()-front.First(' '),"",0); //strip to first argument

      if (front.Contains("/Bacc/detector/recordLevel")){
	front.Replace(0,front.Last('/')+1,"",0); //remove "/Bacc/detector/" leaving rectype
	mid.Replace(mid.Last(' '),mid.Sizeof()-mid.Last(' '),"",0); //strip to "/Bacc/detector/recordLevel volume"
	if (mid.Contains(" ")){
	  mid.Replace(0,mid.Last(' ')+1,"",0); //take volume
	  end.Replace(0,end.Last(' ')+1,"",0); //take reclvl

	  rec.push_back(front);
	  vol.push_back(mid);
	  lvl.push_back(end);
	} else {
	  cout << " ::WARNING:: Volumes should not use default record levels. " << endl;
	  rec.push_back(front);
	  vol.push_back(mid);
          lvl.push_back("0");
	}//end if else
      }//end if rec lvl
    }//end if
  }//end loop lines

  cout << endl;
  cout << "Record Levels: " << endl;
  for(int i=0;i<rec.size();i++) cout << vol[i] << " " << rec[i] << " " << lvl[i] << endl;

  //Check PMTs have correct record levels.
  bool pmtrec = false;
  for(int i=0;i<rec.size();i++) if(vol[i] == "PMT_Photocathode" && rec[i] == "recordLevelOptPhot" && lvl[i] == "3" ) pmtrec = true; //look for correct values for this volume
  for(int i=0;i<rec.size();i++) if(vol[i] == "PMT_Photocathode" && ((rec[i] != "recordLevelOptPhot" && lvl[i] != "0") || (rec[i] == "recordLevelOptPhot" && lvl[i] != "3")) ) pmtrec = false; //look for incorrect values for this volume

  if (pmtrec == false) cout << " ::WARNING:: PMT_Photocathode should have recordLevelOptPhot=3 only " << endl;
  else cout << " *Recording MCTruth in PMT_Photocathode" << endl;

  //Check sensitive volumes have correct record levels.
  bool sensrec;
  for (int k=0;k<4;k++){
    TString sensvol;
    if(k==0) sensvol = "LiquidXenonTarget";
    else if (k==1) sensvol = "ScintillatorCenter";
    else if (k==2) sensvol = "LiquidSkinXenonBank";
    else if (k==3) sensvol = "LiquidSkinXenon";

    bool recvol = false;
    bool recset = false;
    bool recsetPhot = false;
    bool recsetElec = false;
    for(int i=0;i<rec.size();i++) { //look for correct values for this volume
      if(vol[i] == sensvol) recvol = true;
      if(vol[i] == sensvol && rec[i] == "recordLevel" && lvl[i] == "3" ) recset = true;
      if(vol[i] == sensvol && rec[i] == "recordLevelOptPhot" && lvl[i] == "5" ) recsetPhot = true;
      if(vol[i] == sensvol && rec[i] == "recordLevelThermElec" && lvl[i] == "5" ) recsetElec = true;
    }
    for(int i=0;i<rec.size();i++) { //look for incorrect correct values for this volume
      if(vol[i] == sensvol && rec[i] == "recordLevel" && lvl[i] != "3" ) recset = false;
      if(vol[i] == sensvol && rec[i] == "recordLevelOptPhot" && lvl[i] != "5" ) recsetPhot = false;
      if(vol[i] == sensvol && rec[i] == "recordLevelThermElec" && lvl[i] != "5" ) recsetElec = false;
    }

    if (recvol){
      if ( !(recset == true && recsetPhot == true && recsetElec == true) ){
	cout << " ::WARNING:: " << sensvol << " must have: " << endl;
	cout << " recordLevel=3 " << endl;
	cout << " recordLevelOptPhot=5 " << endl;
	cout << " recordLevelThermElec=5 " << endl;
      } else {
      sensrec = true;
      cout << " *Recording MCTruth in " << sensvol << endl;
      }
    }
  }//end loop sensitive volumes
  if (!sensrec) cout << " ::WARNING:: no mctruth recorded for any sensitive volumes " << endl;
  //end record level checks



  vector<string>* lookupTable=0;
  headertree->SetBranchAddress("componentLookupTable",&lookupTable);
  headertree->GetEntry(0);

  //scan for PMTs
  vector<bool> liveindices;
  vector<string> banks;
  vector<int> index;
  cout << endl;
  std::cout << "Look Up Table Size: " << lookupTable->size() << std::endl;
  cout << endl;
  int maxindex=-1; //max pmt index number

  TTree* t_banks = new TTree("PMTBanks","PMTBanks");
  TBranch* b_banks =  t_banks->Branch("PMT_Banks",&banks); //record of all PMT_photocathode types (e.g. water, top_skin, bottom_skin, dome_skin, bottom, top )

  for(int i=0; i<lookupTable->size(); i++)
    {
      if(i%10000==0) std::cout << "Component N: " << i << std::endl;
      TString component = lookupTable->at(i); //grab current volume name.
      //if component is a PMT_Photocathode, record "true" in liveindicies. Else record "false".
      if(component.Contains("PMT_PhotoCathode") || component.Contains("PMT_Photocathode"))
	{
	  liveindices.push_back(true);

	  TString prefix = component.Copy();
	  prefix.Replace(prefix.Last('_'),prefix.Sizeof()-prefix.Last('_'),"",0); //leaves just the PMT type name.
	  if(banks.size()==0 || banks.back()!=prefix.Data()) banks.push_back(prefix.Data()); //record new PMT types

	  TString sindex = component.Copy();
	  sindex.Replace(0,sindex.Last('_')+1,"",0); //leaves just the appended number of the volume (previously removed for "prefix")
	  index.push_back(sindex.Atoi());

	  if(index[i] > maxindex)  maxindex=index[i];

	  if (DEBUGGING)cout << "index: " << index[i] << endl;
	  if (DEBUGGING) std::cout << component << "," << i << "," << liveindices[i] << "," << index[i] << std::endl;
	}
      else
	{
	  liveindices.push_back(false);
	  index.push_back(-1);//no PMT found, record jibberish
	}
    }

  cout << endl;
  cout << "PMT Types (not necessarily hits, just in the geometry): " << endl;
  for (int i=0; i<banks.size();i++) cout << banks[i] << endl;
  cout << "Total PMTs: " << maxindex+1 << endl;
  cout << endl;


  t_banks->Fill(); //just one entry
  t_banks->Write();

  ////////////////////////////////
  /////// Search for Truth ///////
  ////////////////////////////////

  BaccRootConverterEvent* event=0;
  datatree->SetBranchAddress("Event",&event);

  TTree* mcTruthTree = new TTree("MCTruthTree","MCTruth Tree");
  BaccMCTruthEvent *mcTruthEvent = new BaccMCTruthEvent();
  mcTruthTree->Branch( "MCTruthEvent", "An MC Truth Event", mcTruthEvent );
  photonMCTruth eventPhotons;
  vertexMCTruth eventVertices;
  pmtMCTruth eventPMTs;


  //Internal loop variables
  int ntrack;

  int pmt_counter;

  PMTPhoton photon;
  std::vector<PMTPhoton> photonvec;
  double firsttime;
  double lasttime;
  int ident;
  int ptIndx;
  int pt2Indx;

  int numVertices;
  bool new_cluster;
  Double_t creationPositionX;
  Double_t creationPositionY;
  Double_t creationPositionZ;
  Double_t creationTime;
  int creationIndx;

  int energy_deps;
  int parentVertex;
  vector<bool> initialised;
  double testX;
  double testY;
  double testZ;
  double clusterRadius = 0.4; 

  cout << " Loop Events. N=" << datatree->GetEntries() << endl;
  for(int i=0; i<datatree->GetEntries(); i++) { //loop over entries and search for The Truth!


    if(i%100==0) std::cout << "Entry N: " << i << std::endl;
    if (DEBUGGING) std::cout << "Entry N: " << i << std::endl;
    datatree->GetEntry(i);

    if (DEBUGGING)  cout << "Primary: " <<  event->primaryParticles[0].dPosition_mm[0] << " " << event->primaryParticles[0].dPosition_mm[1] << " " <<event->primaryParticles[0].dPosition_mm[2] << " " << event->primaryParticles[0].dTime_ns << " " << event->primaryParticles[0].dEnergy_keV << endl;

    ntrack = event->tracks.size(); //track = complete paths of individual particles, across all volumes



    ///////Look for Photon Truth ///////
    if (DEBUGGING) cout << "Find Photon Truth ..." << endl;
    for(int j=0; j<event->tracks.size(); j++){

      if ((i%10000==0 || i==datatree->GetEntries()-1) && j%10000==0) cout << "Photon Truth Search. Track " << j << " of " << ntrack << " complete in Event " << i << " of " << datatree->GetEntries()<< endl;
      if (DEBUGGING) cout << " Volumes in event: " << i << ", track: " << j << endl;
      if (DEBUGGING) for(int k=0; k<event->tracks[j].steps.size(); k++) cout << event->tracks[j].steps[k].sVolumeName << endl;


      //liveindices check, if true track is an optical photon photocathode hit
      //How that works: get volumeID (int) of last step of this track (-1 since liveindices start from 0 and volumeID start from 1).
      //Livedicies has one entry per volume and returns true if photocathode, else false.
      if( liveindices[event->tracks[j].steps.back().iVolumeID-1] ){

	photon.time = event->tracks[j].steps.back().dTime_ns;
	photon.arrivaltime = event->tracks[j].steps.back().dTime_ns - event->primaryParticles[0].dTime_ns;
	if (DEBUGGING) cout << "   " << j << " " << event->tracks[j].steps.back().dTime_ns  << " " <<  event->primaryParticles[0].dTime_ns << " " << endl;

	if (photonvec.size() == 0){
	  firsttime = photon.time; //first and last time need some initial value
	  lasttime = photon.time;
	}
	if (photon.time < firsttime) firsttime = photon.time;
	if (photon.time > lasttime) lasttime = photon.time;

	photon.wavelength =  event->tracks[j].dWavelength_nm;

	//the original appended GEANT4 volume number, from the volume name string. index.size() = number Geant4 volumes.
	photon.index = index[ event->tracks[j].steps.back().iVolumeID -1 ]; //again get volumeID of last step of this track

	// S1 S2 identification. S1=1. S2=2.
	//loop through all other tracks to find parentID matching this trackID
	ident = -1;
	ptIndx = -1;
	for(int k=0;k<ntrack;k++) if (event->tracks[k].iTrackID == event->tracks[j].iParentID){
	    ptIndx = k; //index of parent
	    if (event->tracks[ptIndx].sParticleName == "thermalelectron" )  ident = 2;  //S2 if parent is thermal electron
	    else  ident = 1; //else S1
	    k = ntrack; // break loop
	  }

	photon.iInteractionIdentifier = ident;

	if ( ident == -1 || ptIndx == -1 ) cout << "::ERROR:: Event: " << i << ". S1/S2 Indentification Fail. Investigate further." << endl;
	if (event->tracks[j].sCreatorProcess == "S1" && ident != 1) cout << "::WARNING:: S1 misidentification. Investigate further." << endl;
	if (event->tracks[j].sCreatorProcess == "S2" && ident != 2) cout << "::WARNING:: S2 misidentification. Investigate further." << endl;


	/////// Find Vertices (creation positions) ///////
	creationIndx = j; //set index of creation track
	if (ident == 2) creationIndx = ptIndx;


	//check parent of creation position is an energy deposit
	pt2Indx = -1; //parent of parent
	for(int k=0; k<ntrack;k++) if( event->tracks[creationIndx].iParentID == event->tracks[k].iTrackID){
	    pt2Indx = k;
	    //creationIndx = pt2Idnx;
	    if (event->tracks[pt2Indx].sParticleName == "thermalelectron" || event->tracks[ptIndx].sParticleName == "opticalphoton") cout << "::ERROR:: THIS SHOULD NOT HAPPEN. Ident: " << ident << endl;
	    k = ntrack; //break loop
	  }
	if (pt2Indx == -1) cout << "::ERROR:: THIS CHECK FAILED " << endl;


	creationPositionX = event->tracks[creationIndx].steps[0].dPosition_mm[0];
	creationPositionY = event->tracks[creationIndx].steps[0].dPosition_mm[1];
	creationPositionZ = event->tracks[creationIndx].steps[0].dPosition_mm[2];
	//creationTime = event->tracks[creationIndx].steps[0].dTime_ns;


	if (0 != event->tracks[creationIndx].steps[0].iStepNumber ) cout << "::ERROR:: Creation step is not step 0. iStepNumber = " << event->tracks[creationIndx].steps[0].iStepNumber << endl;
//	if (!("LiquidXenonTarget" == event->tracks[creationIndx].steps[0].sVolumeName || "LiquidSkinXenon" == event->tracks[creationIndx].steps[0].sVolumeName || "LiquidSkinXenonBank" == event->tracks[creationIndx].steps[0].sVolumeName || "ScintillatorCenter" == event->tracks[creationIndx].steps[0].sVolumeName )) cout << "::ERROR:: Creation step is not in a sensitive volume. Volume = " << event->tracks[creationIndx].steps[0].sVolumeName << endl;

	//all photons are created at the same position as their parent step,
	//   which is the same position of its vertex/creation positions,
	//   which is the same position as an energy deposit in that vertex. 
	//electrons  are created at positions with some Gaussian distribution around this position,
	//   however BACCARAT has been modified to override creation positon with vertex position.
	//the following code defines vertices with unique creation positions, and counts their S1/S2 quanta.

	new_cluster = true;

	for(int k = 0; k<mcTruthEvent->vertices.size(); k++) {

	  if ( abs(creationPositionX - mcTruthEvent->vertices[k].dPosition_mm[0]) < 1e-12 && abs(creationPositionY - mcTruthEvent->vertices[k].dPosition_mm[1]) < 1e-12 && abs(creationPositionZ - mcTruthEvent->vertices[k].dPosition_mm[2])<1e-12){

	  new_cluster = false;
	  photon.iVertexNumber = k;
	  if (ident == 1) mcTruthEvent->vertices[k].s1Quanta++;
	  else if (ident == 2) mcTruthEvent->vertices[k].s2Quanta++;
	  k = mcTruthEvent->vertices.size(); //break loop
	}//end repeat vertex
      }//end vertex loop


      if (new_cluster) {

	photon.iVertexNumber = mcTruthEvent->vertices.size();
	eventVertices.sVolumeName = "";
	eventVertices.sParticleName = "";
	eventVertices.iTrackID = -1;
	eventVertices.iParentID = -1;
	eventVertices.dTime_ns = -1.;
	eventVertices.dPosition_mm[0] = creationPositionX;
	eventVertices.dPosition_mm[1] = creationPositionY;
	eventVertices.dPosition_mm[2] = creationPositionZ;
	eventVertices.dElectricFieldDirection[0] = 0; //TODO// Needs Field Model
	eventVertices.dElectricFieldDirection[1] = 0; //Would be defined in macro
	eventVertices.dElectricFieldDirection[2] = 1; //need way to pass info here
	eventVertices.dElectricFieldStrength_Vcm = 310; //
	eventVertices.dEnergyDep_keV = 0.;
	eventVertices.s1Quanta = 0;
	eventVertices.s2Quanta = 0;
	if (ident == 1) eventVertices.s1Quanta++;
	else if (ident == 2) eventVertices.s2Quanta++;
 
	mcTruthEvent->vertices.push_back(eventVertices);
      }

      photonvec.push_back(photon);

      if (DEBUGGING) cout << " end photon track " << endl;

    }//end if. photon
  }//end loop tracks

  if (DEBUGGING)  cout << "Photon Truth Complete" << endl;

  numVertices = mcTruthEvent->vertices.size();
  for (int j=0; j<numVertices; j++) initialised.push_back(false);


  ///////Look for Vertex Truth ///////
  if (DEBUGGING)  cout << "Find Vertex Truth ... " << endl;
  energy_deps = 0;

  for(int j=0; j<ntrack; j++) {

    if ((i%10000==0 || i==datatree->GetEntries()-1) && j%10000==0) cout << "Vertex Truth Search. Track " << j << " of " << ntrack << " complete in Event " << i << " of " << datatree->GetEntries()<< endl;
    if (DEBUGGING) cout << " Volumes in event: " << i << ", track: " << j << endl;
    if (DEBUGGING) for(int k=0; k<event->tracks[j].steps.size(); k++)       cout << event->tracks[j].steps[k].sVolumeName << endl;


    //iParticleID 0 = optical photon
    if (!(event->tracks[j].sParticleName == "thermalelectron" || event->tracks[j].iParticleID == 0 )){
      for(int k=0; k<event->tracks[j].steps.size(); k++){


	//fill cluster information by
	//looping over all steps looking for none-zero energy deposits in sensitive volumes
	if ( 0 < event->tracks[j].steps[k].dEnergyDep_keV ) {
	  if ("ReverseFieldRegion" == event->tracks[j].steps[k].sVolumeName || "LiquidXenonTarget" == event->tracks[j].steps[k].sVolumeName || "LiquidSkinXenon" == event->tracks[j].steps[k].sVolumeName || "LiquidSkinXenonBank" == event->tracks[j].steps[k].sVolumeName || "ScintillatorCenter" == event->tracks[j].steps[k].sVolumeName ) {

	    if (DEBUGGING) cout << "e/t/s:" << i << "/" << j << "/" << k << " " << event->tracks[j].steps.size() << " " << event->tracks[j].sParticleName << " " << event->tracks[j].steps[k].dEnergyDep_keV << " " << event->tracks[j].steps[k].dPosition_mm[0] << " " << event->tracks[j].steps[k].dPosition_mm[1]<< " " << event->tracks[j].steps[k].dPosition_mm[2] <<  " " << event->tracks[j].steps[k].dTime_ns << endl;

	    energy_deps++;
	    parentVertex = -1;
	    
	    testX = event->tracks[j].steps[k].dPosition_mm[0];
	    testY = event->tracks[j].steps[k].dPosition_mm[1];
	    testZ = event->tracks[j].steps[k].dPosition_mm[2];

	    //TrackID/ParentID/VolumeName/Time all come from
	    //       the first hit in the cluster/position of cluster/creation position.
	    for(int m = 0; m<numVertices; m++) if( abs(mcTruthEvent->vertices[m].dPosition_mm[0] - testX) < 1e-12 && abs(mcTruthEvent->vertices[m].dPosition_mm[1] - testY) < 1e-12 &&  abs(mcTruthEvent->vertices[m].dPosition_mm[2] - testZ ) < 1e-12 ){
	      mcTruthEvent->vertices[m].iTrackID = event->tracks[j].iTrackID;
	      mcTruthEvent->vertices[m].iParentID = event->tracks[j].iParentID;
	      mcTruthEvent->vertices[m].dTime_ns = event->tracks[j].steps[k].dTime_ns;
	      mcTruthEvent->vertices[m].sVolumeName = event->tracks[j].steps[k].sVolumeName;
	      if(false == initialised[m]) initialised[m] = true;
	      else cout << "::ERROR:: Attempt to initialise vertex truth twice. Investigate further." << endl;
	      parentVertex = m;
	    }

	    if (parentVertex == -1) for(int m = 0; m<numVertices; m++)  if(sqrt(pow(mcTruthEvent->vertices[m].dPosition_mm[0]-testX,2)+pow(mcTruthEvent->vertices[m].dPosition_mm[1]-testY,2)+pow(mcTruthEvent->vertices[m].dPosition_mm[2]-testZ,2))-clusterRadius< 1e-12) {

		if (parentVertex == -1) parentVertex = m;
		else  cout << "::WARNING:: E/T/S:" << i << "/" << j << "/" << k << ". Energy deposit clustering degeneracy." << endl; 
		//m = numVertices; //break loop.
  
	    }// end parent search

	    if (parentVertex == -1){
	      cout << "::ERROR:: Energy deposit not clustered. Investigate further." << endl;
	    } else {
	      if (mcTruthEvent->vertices[parentVertex].sParticleName == "" ) mcTruthEvent->vertices[parentVertex].sParticleName = event->tracks[j].sParticleName; //set vertex name
	      if (mcTruthEvent->vertices[parentVertex].sParticleName != event->tracks[j].sParticleName) mcTruthEvent->vertices[parentVertex].sParticleName = "mixed";

	  //Clusters' energy is cumulative
       	  mcTruthEvent->vertices[parentVertex].dEnergyDep_keV += event->tracks[j].steps[k].dEnergyDep_keV;
	    }//end clustering info fill
	      
		
	  }//end if. volume check
	}//end if. energy check
      }//end steps loop
    } else { //end if energy deposit
      j = ntrack; //break loop at first non-energy-deposit, there will be no more after this.
    }
  }// end tracks

  for(int m = 0; m<numVertices; m++) if (!initialised[m]) {
  cout << "::ERROR:: Cluster " << m << "/" << numVertices << " Primary variables not found. Investigate further." << endl;
  if (false) for(int j=0; j<ntrack; j++) { // remove "if (false)" to investigate further.
   if (!(event->tracks[j].sParticleName == "thermalelectron" || event->tracks[j].iParticleID == 0 )) for(int k=0; k<event->tracks[j].steps.size(); k++){
    if ( 0 < event->tracks[j].steps[k].dEnergyDep_keV ) {
     if ("ReverseFieldRegion" == event->tracks[j].steps[k].sVolumeName || "LiquidXenonTarget" == event->tracks[j].steps[k].sVolumeName || "LiquidSkinXenon" == event->tracks[j].steps[k].sVolumeName || "LiquidSkinXenonBank" == event->tracks[j].steps[k].sVolumeName || "ScintillatorCenter" == event->tracks[j].steps[k].sVolumeName ) {
      testX = event->tracks[j].steps[k].dPosition_mm[0];
      testY = event->tracks[j].steps[k].dPosition_mm[1];
      testZ = event->tracks[j].steps[k].dPosition_mm[2];
      if(sqrt(pow(mcTruthEvent->vertices[m].dPosition_mm[0]-testX,2)+pow(mcTruthEvent->vertices[m].dPosition_mm[1]-testY,2)+pow(mcTruthEvent->vertices[m].dPosition_mm[2]-testZ,2))-clusterRadius< 1e-12){
       cout << i << " " <<event->tracks[j].sParticleName << " " << j << " " << k << "  " << m << endl;
       cout << "cluster: " << mcTruthEvent->vertices[m].dPosition_mm[0] << " " << mcTruthEvent->vertices[m].dPosition_mm[1] << " " << mcTruthEvent->vertices[m].dPosition_mm[2] << endl;
       cout << "enedepp: " << testX << " " << testY << " " << testZ << endl;
       cout << endl;
      }//cluster match
     }//vol
    }//ene
   }//step
  }//track
 }// end m 

  if (DEBUGGING)    cout << "Vertex Truth Complete" << endl;


  /////// Validate Vertices ///////
  for(int m = 0; m<numVertices; m++){
    //check S2 quanta
       if (mcTruthEvent->vertices[m].s2Quanta == 0 ) cout << "::S2 WARNING:: Event: " << i << ". Vertex " << m << "/" << numVertices << " has s2Quanta=0. s1Quanta=" << mcTruthEvent->vertices[m].s1Quanta << ". Investigate further." << endl;
    //check S1 quanta
    if (mcTruthEvent->vertices[m].s1Quanta == 0 ) cout << "::S1 WARNING:: Event: " << i << ". Vertex " << m << "/" << numVertices << " has s1Quanta=0. s2Quanta=" << mcTruthEvent->vertices[m].s2Quanta << ". Investigate further. " << endl;
    //check energy
    if (mcTruthEvent->vertices[m].dEnergyDep_keV == 0. ) cout << "::WARNING:: Event: " << i << ". Vertex " << m << "/" << numVertices << " has dEnergyDep_keV=0. Investigate further. " << endl;
  }// end vertex loop



  for (int p=0; p<maxindex+1;p++){ //one vector element per PMT
    eventPMTs.numPhotonHits = 0;
    mcTruthEvent->pmts.push_back(eventPMTs);
  }

  pmt_counter = 0;
  if(photonvec.size()>0)  {
    std::sort(photonvec.begin(),photonvec.end(),comparePhotons);//orders by Geant4 volume number, and then by time.
    for(int j=0; j<(int)photonvec.size(); j++) {

      eventPhotons.iPMTIndex = photonvec[j].index;
      eventPhotons.dPhotonTime_ns = photonvec[j].time;
      eventPhotons.dPhotonWavelength_nm = photonvec[j].wavelength;
      eventPhotons.iInteractionIdentifier = photonvec[j].iInteractionIdentifier;
      eventPhotons.iVertexNumber = photonvec[j].iVertexNumber;
      eventPhotons.dTimeSincePrimary_ns = photonvec[j].arrivaltime;

      mcTruthEvent->pmthits.push_back(eventPhotons);
      pmt_counter++;

      if(photonvec[j+1].index > photonvec[j].index || photonvec.size()-1 == j){
	//photons grouped by index=Geant4 PMT number. When next photon hits different PMT (or end of vector)...
	mcTruthEvent->pmts[photonvec[j].index].numPhotonHits = pmt_counter; //index starts at zero.
	pmt_counter = 0;
      }
    }//end loop
  } else {
    firsttime = 0;
    lasttime = 0;
  }


  mcTruthEvent->dEventFirstPhotonTime_ns = firsttime;
  mcTruthEvent->dEventLastPhotonTime_ns = lasttime;
  mcTruthEvent->iOriginalEventID = event->iEventNumber;
  mcTruthEvent->iOriginalEventTreeIndex = i;
  mcTruthEvent->iRunNumber = event->iRunNumber;

  mcTruthTree->Fill();
  mcTruthEvent->ClearEverything();

  //clear vectors ahead of entering next entry
  photonvec.clear();
  initialised.clear();

}// end event loop

cout << " Writing... " << endl;
headertree2->Write("HeaderTree");
mcTruthTree->Write();
outfile->Close();

}//end mains
