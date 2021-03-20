////////////////////////////////////////////////////////////////////////////////
/*	BaccGeneratorEventsFile.cc
*
* This is the code file for the Gamma-X generator.
*
********************************************************************************
* Change log
*   12 May 14 - Initial submission, for gamma-X event generation. (Kevin)
*
*/
////////////////////////////////////////////////////////////////////////////////

//
//      C++ includes
//
#include <vector>

//
//	GEANT4 includes
//
#include "globals.hh"
#include "G4GenericIon.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"

//
//	Bacc includes
//
#include "BaccGeneratorEventsFile.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccGeneratorEventsFile()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorEventsFile::BaccGeneratorEventsFile()
{
	name = "EventsFile";
	activityMultiplier = 1;
	ionDef = G4GenericIon::Definition();
        electronDef = G4Electron::Definition();
	gammaDef = G4Gamma::Definition();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccGeneratorEventsFile()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorEventsFile::~BaccGeneratorEventsFile() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorEventsFile::GenerateEventList( G4ThreeVector position,
                G4int sourceByVolumeID, G4int sourcesID, G4double time)
{
    G4int a=-1; G4int z=-1;
    G4double hl=-1;
    Isotope *currentIso = new Isotope(name, z, a, hl);
    baccManager->RecordTreeInsert( currentIso, time, position, 
                  sourceByVolumeID, sourcesID );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateFromEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorEventsFile::GenerateFromEventList( G4GeneralParticleSource
       *particleGun, G4Event *event, decayNode *firstNode  )
{

    G4int particleID;
    G4ThreeVector pos;
    G4int numDeposits = baccManager->NextEventToGenerate();
    char command[100];
    for(G4int i = 0; i < numDeposits; i++) {
        pos = baccManager->NextPositionToGenerate();    
        G4double timeDelay = (firstNode->timeOfEvent)/ns;//stored as seconds
        timeDelay *= ns;

        particleGun->GetCurrentSource()->SetParticleTime( timeDelay );
    
        particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
        particleID = baccManager->NextParticleIDToGenerate();
        if(particleID == 11) {
            particleGun->GetCurrentSource()->SetParticleDefinition( electronDef );
        }
        else if(particleID == 22) {
            particleGun->GetCurrentSource()->SetParticleDefinition( gammaDef );
        }
        else if(particleID > 1e8) {
            particleGun->GetCurrentSource()->SetParticleDefinition( ionDef );
            particleID = particleID - 100000000;
            G4int charge = particleID%10;
            particleID = particleID/10;
            G4int isotope = particleID%1000;
            particleID = particleID/1000;
            G4int element = particleID;
            //Set the ion
            sprintf(command, "/gps/ion  %d %d %d %d", element, isotope, charge, 0);

           //Input the command string
           UI->ApplyCommand(command);
        }
        particleGun->GetCurrentSource()->GetAngDist()->
          SetParticleMomentumDirection( GetRandomDirection() );
        G4double monoEnergy = baccManager->NextEnergyDepToGenerate();
        particleGun->GetCurrentSource()->GetEneDist()->
          SetMonoEnergy( monoEnergy );
        particleGun->GeneratePrimaryVertex( event );
        baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) ) ;
    }

}

