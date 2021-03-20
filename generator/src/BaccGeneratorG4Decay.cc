////////////////////////////////////////////////////////////////////////////////
/*   BaccGeneratorG4Decay.cc
*
* This is the code file for the G4Decay generator.
*
********************************************************************************
* Change log
*   06-Oct-2015 - Initial submission (David W)
*/
////////////////////////////////////////////////////////////////////////////////

//
//   General notes on this generator
//

/*   This generator is a alternative way of dealing with decay chains, using the
standard Geant4 methods. It is based largely on the SingleDecay generator but
will not impose limits on when the chain stops.
*/

//
//   GEANT4 includes
//
#include "globals.hh"
#include "G4Neutron.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4Gamma.hh"
#include "G4GenericIon.hh"
#include <G4RadioactiveDecay.hh>
#include <G4DecayTable.hh>
#include <G4ParticleDefinition.hh>

//
//   Bacc includes
//
#include "BaccGeneratorG4Decay.hh"

//
//   C++ includes & definitions
//
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#define PI 3.14159265358979312

using namespace std;

//------++++++------++++++------++++++------++++++------++++++------++++++------
//               BaccGeneratorG4Decay()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorG4Decay::BaccGeneratorG4Decay()
{
   name = "G4Decay";
   activityMultiplier = 1;
   ion = G4GenericIon::Definition();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//               ~BaccGeneratorG4Decay()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorG4Decay::~BaccGeneratorG4Decay() {}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorG4Decay::GenerateEventList(G4ThreeVector position,
    G4int sourceByVolumeID, G4int sourcesID, G4int a, G4int z, G4double time)
{
  //name, z, a, particlename, energy                                                                                    
  G4double halflife=0;
  Isotope *currentIso = new Isotope(name, z, a, halflife);
  baccManager->RecordTreeInsert( currentIso, time, position,
				sourceByVolumeID, sourcesID );
  
}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateFromEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorG4Decay::GenerateFromEventList( G4GeneralParticleSource
       *particleGun, G4Event *event, decayNode *firstNode  )
{

  nucleusZ = firstNode->Z;
  nucleusA = firstNode->A;
  G4ThreeVector pos = G4ThreeVector(firstNode->pos );
  G4double time = (firstNode->timeOfEvent)/ns;

  particleGun->GetCurrentSource()->SetParticleDefinition( ion );
  particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
  particleGun->GetCurrentSource()->SetParticleTime( time*ns );

  std::stringstream uiStream ;
  uiStream.str("") ;
  uiStream << "/gps/ion " << nucleusZ << " " << nucleusA << " 0 0" ;
  uiString = uiStream.str() ;
  UI->ApplyCommand( uiString );

  particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection( GetRandomDirection() );
  particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( 0.*keV );
  particleGun->GeneratePrimaryVertex( event );

  baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;

  if(baccManager->GetRadioIsotopeList().size() == 0)
    baccManager->SetRadioIsotopeList( GetDecayChainPDGNumbers(particleGun) );
}


//------++++++------++++++------++++++------++++++------++++++------++++++-----
//                              GetDecayChainPDGNumbers()
//------++++++------++++++------++++++------++++++------++++++------++++++-----
std::vector<G4int> BaccGeneratorG4Decay::GetDecayChainPDGNumbers(G4GeneralParticleSource *particleGun){

  //---- Get the decay table info;
  //---- The first step is to get a vector of all the daughter particles
  //---- for a radioisotope, then pass these to the ProcessParticleDefs
  //---- to determine which ones are on the chain...
  G4RadioactiveDecay *radDecay = new G4RadioactiveDecay( "RadioactiveDecay" );
  G4DecayTable *radTable = new G4DecayTable();  
  G4DecayTable *radTableCopy = new G4DecayTable();  
  std::vector<G4ParticleDefinition*> particleDefs;
  std::vector<G4ParticleDefinition*> particleDefsKeep;
  std::vector<G4int> radioisotopeList;

  particleDefsKeep.push_back(particleGun->GetParticleDefinition());
  
  while( particleDefsKeep.size() ){  
    for(int i=0; i<(int)particleDefsKeep.size(); i++){

      radTable = radDecay->LoadDecayTable(*particleDefsKeep[i]);
      
      for(int ii=0; ii<radTable->entries(); ii++){      
	for(int jj=0; jj<radTable->GetDecayChannel(ii)->GetNumberOfDaughters(); jj++){
	  	  
	  if( radTable->GetDecayChannel(ii)->GetDaughter(jj)->GetPDGEncoding()!=particleDefsKeep[i]->GetPDGEncoding() 
	      && radTable->GetDecayChannel(ii)->GetDaughter(jj)->GetPDGEncoding() > 1e9 ) 
	    particleDefs.push_back(radTable->GetDecayChannel(ii)->GetDaughter(jj));
	  
	}
      }            
    }
    
    particleDefsKeep.clear();

    particleDefsKeep = ProcessParticleDefs(particleDefs,particleGun);
    for(int ivec=0; ivec<(int)particleDefsKeep.size(); ivec++){
      //--- Only save particles with a lifetime > 1e6 ns
      if(particleDefsKeep[ivec]->GetPDGLifeTime() > 1e06 || particleDefsKeep[ivec]->GetPDGLifeTime() < 0)
	radioisotopeList.push_back(particleDefsKeep[ivec]->GetPDGEncoding());
    }
    
    particleDefs.clear();

  }

  //--- Uncomment to print a list of the saved radioisotopes
  // G4cout << "\n\n\n--------------------------------\nList of Radioisotopes\n";
  // for(int ii=0; ii<radioisotopeList.size(); ii++)
  //   G4cout << radioisotopeList[ii] << G4endl;

  //---- End
  return radioisotopeList;

}



//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              ProcessParticleDefs()
//------++++++------++++++------++++++------++++++------++++++------++++++------
std::vector<G4ParticleDefinition*> BaccGeneratorG4Decay::ProcessParticleDefs(std::vector<G4ParticleDefinition*> particleDefs, G4GeneralParticleSource *particleGun){
  
  //--- Takes a vector of ALL the daughter particle definitions from a nuclei
  //--- then returns the ones that themselves have more than one daughter 
  //--- i.e. they are on radioactive nuclei on the decay chain...
  G4RadioactiveDecay *radDecay = new G4RadioactiveDecay( "RadioactiveDecay" );
  G4DecayTable *radTableCopy = new G4DecayTable();
  std::vector<G4ParticleDefinition*> particleDefsKeep_tmp;
  std::vector<G4ParticleDefinition*> particleDefsKeep;
  bool found = false;
  bool stable = false;

  for(int i=0; i<(int)particleDefs.size(); i++){
    radTableCopy = radDecay->LoadDecayTable(*particleDefs[i]);
    if(particleDefs[i]->GetPDGEncoding() > 1000100000 && radTableCopy->entries()==0) 
      stable = true;
    if(radTableCopy->entries() > 1 || (radTableCopy->entries()==1 && radTableCopy->GetDecayChannel(0)->GetNumberOfDaughters() > 1)
       || (particleDefs[i]->GetPDGEncoding() > 1000100000 && radTableCopy->entries()==0) ){
      particleDefsKeep_tmp.push_back(particleDefs[i]);
    }
  }


  //--- Sometimes there won't be any particles with more than one daughter, but no stable particles
  //--- either (e.g. Ra228 on the Th232 decay chain are all excited states)... 
  //--- if this happens, for each of those particles, invent their ground state...
  if(particleDefsKeep_tmp.size()==0 && !stable){
    for(int i=0; i<(int)particleDefs.size(); i++){
      radTableCopy = radDecay->LoadDecayTable(*particleDefs[i]);
      if(radTableCopy->entries()==1){
	G4ParticleDefinition *fakeParticle;
	std::stringstream fakeUiStream;
	G4String fakeUiString;
	G4int ZZ;
	G4int AA;
	std::vector<int> v;
	particleGun->GetCurrentSource()->SetParticleDefinition( ion );	
	fakeUiStream.str("");
	GetPDG(particleDefs[i]->GetPDGEncoding(),v);  // get PDG 
	ZZ=100*v[3]+10*v[4]+v[5];                     // get atomic number 
	AA=100*v[6]+10*v[7]+v[8];                     // get mass number    
	fakeUiStream << "/gps/ion " << ZZ << " " << AA << " 0 0" ;
	fakeUiString = fakeUiStream.str() ;
	UI->ApplyCommand( fakeUiString );
	fakeParticle = particleGun->GetParticleDefinition();	
	particleDefsKeep_tmp.push_back(fakeParticle);
      }
    }
  }

  //--- Remove duplicates (caused by more than one decay branch to an isotope)
  for(int i=0; i<(int)particleDefsKeep_tmp.size(); i++){
    found = false;
    if(i==0) particleDefsKeep.push_back(particleDefsKeep_tmp[i]);
    else{
      for(int j=0; j<(int)particleDefsKeep.size(); j++)
	if(particleDefsKeep[j]->GetPDGEncoding() == particleDefsKeep_tmp[i]->GetPDGEncoding() ) found = true;      
      if(!found) particleDefsKeep.push_back(particleDefsKeep_tmp[i]);
    }    
  }
      
  return particleDefsKeep;

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              GetPDG()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorG4Decay::GetPDG(int x, std::vector<int> &v){
  if(x >= 10) GetPDG(x/10,v);
  v.push_back(x % 10);
}


