////////////////////////////////////////////////////////////////////////////////
/*	BaccManager.cc

This is the code file to manage all the workings of Baccarat. This class is a 
clearing house for all internal controls, from geometry to output to event 
generation. All classes can and should have access to this manager class, and 
there should only be one instance of this class in the entire simulation.
********************************************************************************
Change log
  2013/09/26 - Initial submission (Vic)
  2014/01/22 - Removed a few vestigial variables (Vic)
  2014/09/08 - Added a switch to output diffs depending on whether the code is
               stored by git or svn (Brian)
  2014/09/08 - Added the SetUseOpticalProcesses method to the code file (Brian)
  2014/09/09 - Added commands for the EventsFile generator (Brian)
  2014/10/05 - Changed source per mass calculation to use Kareem's mass
               calculation rather than the stock Geant4 calculation (Brian)
  2014/10/05 - Added default value for checkVolumeOverlaps flag (Brian)
  2015/03/03 - Added SetComponentMass and SetComponentVolume methods (Brian, from Bacc)
  2015/03/03 - Added TwoElectrons generator command (Brian, from Bacc)
  2015/03/15 - Changed SetMass and SetVolume methods so that the supplied
               name is used to match any part of a volume name, so that the
               behavior is similar to the SetSource method (Kareem)
  2016-05-09 - Store command history file in $TMPDIR if defined, else /tmp           


*/
////////////////////////////////////////////////////////////////////////////////

//
//	C/C++ includes
//
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>

//
//	CLHEP includes
//
//#include "CLHEP/Matrix/Vector.h"
//#include "CLHEP/Matrix/Matrix.h"
#include "Randomize.hh"

//
//	GEANT4 includes
//
#include "G4UImanager.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Event.hh"
#include "globals.hh"

//
//	Bacc includes
//
#include "BaccManager.hh"
#include "BaccPhysicsList.hh"
#include "BaccPhysicsOpticalPhysics.hh"
#include "BaccDetectorConstruction.hh"
#include "BaccDetectorComponent.hh"
#include "BaccGeometryCatalog.hh"
#include "BaccPrimaryGeneratorAction.hh"
#include "BaccRunAction.hh"
#include "BaccEventAction.hh"
#include "BaccSteppingAction.hh"
#include "BaccOutput.hh"
#include "BaccSourceCatalog.hh"
#include "BaccSource.hh"
#include "BaccBST.hh"
#include "BaccMessenger.hh"
#include "BaccGeometryMessenger.hh"

using namespace std;
using namespace CLHEP;

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccManager()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccManager *BaccManager::baccManager = 0;
BaccManager::BaccManager()
{
	//	Set the random seed
	CLHEP::HepRandom::setTheEngine( &randomizationEngine );
	
	ifstream devurandom("/dev/urandom");
	G4int seed;
	devurandom.read( (char*)(&seed), sizeof( G4int ) );
	if( seed < 0 ) seed = -seed;
	devurandom.close();
	randomSeed = seed;
	CLHEP::HepRandom::setTheSeed( randomSeed );
	
	UI = G4UImanager::GetUIpointer();
	stringstream historyFileStream;
	const char* tPath=getenv("TMPDIR"); //if defined, use $TMPDIR over /tmp
	if(tPath)
      historyFileStream << tPath;
    else
      historyFileStream << "/tmp";
    historyFileStream << "/LUXSimCommandHistory_" << randomSeed << ".txt";
	historyFile = historyFileStream.str();
	UI->StoreHistory( historyFile.c_str() );

    // Set up baccManager...
	baccManager = this;
	baccMessenger = new BaccMessenger( this );
	BaccOut = NULL;
	
	BaccComponents.clear();
	
	outputDir = ".";
	
	alwaysRecordPrimary = true;
    eventProgressFrequency = 100000;

    hasBaccSources = false;
    isEventListBuilt = false;
    printEventList = false;
    hasDecayChainSources = false;
    windowEnd = 0.;
    checkVolumeOverlaps = false;	
	useOpticalProcesses = true;
	useInputEventSeed = false;
    
    s1gain = 1;
    s2gain = 1;
	
	driftElecAttenuation = 1.*m;

    currentEvtN = -1;
	
	BaccDetector = NULL;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccManager()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccManager::~BaccManager()
{
	if ( BaccOut ) delete BaccOut;
	if ( BaccSourceCat ) delete BaccSourceCat;
	if ( BaccGeometryCat ) delete BaccGeometryCat;
	
	stringstream rmCommand;
	rmCommand << "rm -rf " << historyFile;
	system( rmCommand.str().c_str() );

	baccManager = NULL;

	G4cout << "Bacc manager deleted." << G4endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetManager()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccManager *BaccManager::GetManager()
{
	return baccManager;
}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetUseOpticalProcesses()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::SetUseOpticalProcesses( G4bool use )
{
	useOpticalProcesses = use;
    
        BaccPhysics->SetCutsLong();
}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BeamOn()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::BeamOn( G4int numOfEvents )
{
	//	Record the total number of events to run
	numEvents = numOfEvents;

	//	Turn on radioactive decay in all volumes

	UI->ApplyCommand( "/grdm/allVolumes");
    
    if ( !BaccDetector->DetectorSpecified() )
    {
            G4cout<<"*************************************************" <<
            G4endl << "WARNING: geometry is updating for the first time, "
            << G4endl << "wiping record level settings." << G4endl <<
            "No hits will be recorded to output file." << G4endl <<
            "*************************************************" << 
            G4endl << G4endl;
            UpdateGeometry();
    }

	G4cout << "BEAM ON " << G4endl;
	
	// Create new BaccOutput object
	if (BaccOut)
		delete BaccOut;
	new BaccOutput();

	//	Print info to the screen, and assign volume IDs
	for( G4int i=0; i<(G4int)BaccComponents.size(); i++ ) {
		
		BaccComponents[i]->SetID( i+1 );
		
		G4cout << "Volume \"" <<  BaccComponents[i]->GetName() << "\" "
			   << "assigned ID " << i+1
			   << ", record level " << BaccComponents[i]->GetRecordLevel()
			   << ", opt record level "
			   << BaccComponents[i]->GetRecordLevelOptPhot()
			   << ", therm elec record level "
			   << BaccComponents[i]->GetRecordLevelThermElec()
			   << G4endl;
		BaccComponents[i]->DetermineCenterAndExtent(
				BaccDetector->GetWorldVolume());
	}
	G4cout << G4endl << G4endl;
	
	//	Calculate the final source ratios and print that info to the screen
	sourceByVolume.clear();
	totalSimulationActivity = 0;
	for( G4int i=0; i<(G4int)BaccComponents.size(); i++ ) {
		BaccComponents[i]->CalculateRatios();
		if( BaccComponents[i]->GetTotalActivity() ) {
			volumeSource tempVolumeSource;
			tempVolumeSource.component = BaccComponents[i];
			tempVolumeSource.totalVolumeActivity =
					BaccComponents[i]->GetTotalActivity();
			tempVolumeSource.totalVolumeActivityRatio = 0;
			sourceByVolume.push_back( tempVolumeSource );

			totalSimulationActivity += tempVolumeSource.totalVolumeActivity;
		}
	}
	
	G4cout << "Total activity in the entire simulation = "
		   << totalSimulationActivity << " Bq" << G4endl;
	for( G4int i=0; i<(G4int)sourceByVolume.size(); i++ ) {
		sourceByVolume[i].totalVolumeActivityRatio =
				sourceByVolume[i].totalVolumeActivity / totalSimulationActivity;
		G4cout << "\tActivity ratio for "
			   << sourceByVolume[i].component->GetName() << " = "
			   << sourceByVolume[i].totalVolumeActivityRatio << G4endl;
	}
	for( G4int i=1; i<(G4int)sourceByVolume.size(); i++ )
		sourceByVolume[i].totalVolumeActivityRatio +=
			sourceByVolume[i-1].totalVolumeActivityRatio;
	//	Turn the optical processes on or off
	G4cout << "useOpticalProcesses = " << useOpticalProcesses << G4endl;
	if( useOpticalProcesses ) {
		BaccPhysicsOptical->GetScintillation()->SetScintillationYieldFactor(
				1.);
//		UI->ApplyCommand( "/process/activate Scintillation" );
		UI->ApplyCommand( "/process/activate Cerenkov" );
	} else {
		BaccPhysicsOptical->GetScintillation()->SetScintillationYieldFactor(
				0);
//		UI->ApplyCommand( "/process/inactivate Scintillation" );
		UI->ApplyCommand( "/process/inactivate Cerenkov" );	
	}
	
	//	This next chunk of code opens the stored history of commands, and saves
	//	that list in the local string.
	ifstream commands( historyFile.c_str() );
	commands.seekg( 0, ios::end );
	int length = commands.tellg();
	commands.seekg( 0, ios::beg );
	char *inputBuffer;
	inputBuffer = new char[length];
	commands.read( inputBuffer, length );
	commands.close();
	listOfCommands = inputBuffer;
	delete [] inputBuffer;
	
	//	Next, create a master diff file relative to the current version of
	//	Bacc. Save this diff file in local memory for later access.
	//if ( IsSVNRepo ) {
	if ( false ) {
		stringstream diffFileName;
		diffFileName << "/tmp/diff_" << randomSeed << ".txt";
		stringstream diffCommand;
		diffCommand << "svn diff " << compilationDir << " > " << diffFileName.str();
		system( diffCommand.str().c_str() );
		ifstream diffs( diffFileName.str().c_str() );
		diffs.seekg( 0, ios::end );
		length = diffs.tellg();
		diffs.seekg( 0, ios::beg );
		inputBuffer = new char[length];
		diffs.read( inputBuffer, length );
		diffs.close();
		stringstream rmCommand;
		rmCommand.str("");
		rmCommand << "rm -rf " << diffFileName.str();
		system( rmCommand.str().c_str() );
		listOfDiffs = inputBuffer;
		delete [] inputBuffer;
	//} else if ( IsGitRepo ) {
	} else if ( false ) {
		stringstream diffFileName;
		diffFileName << "/tmp/diff_" << randomSeed << ".txt";
		stringstream diffCommand;
		diffCommand << "git diff " << compilationDir << " > " << diffFileName.str();
		system( diffCommand.str().c_str() );
		ifstream diffs( diffFileName.str().c_str() );
		diffs.seekg( 0, ios::end );
		length = diffs.tellg();
		diffs.seekg( 0, ios::beg );
		inputBuffer = new char[length];
		diffs.read( inputBuffer, length );
		diffs.close();
		stringstream rmCommand;
		rmCommand.str("");
		rmCommand << "rm -rf " << diffFileName.str();
		system( rmCommand.str().c_str() );
		listOfDiffs = inputBuffer;
		delete [] inputBuffer;
	} else {
        listOfDiffs = "No diffs available - ";
        listOfDiffs +=  "This was not recognized as either a git repository ";
        listOfDiffs +=  "or an svn repo. \n";
	}
    // All sources are added to a binary search tree ordered by time before
    // Geant begins to "generate events"
    G4cout << "Loading sources..." << G4endl;
    if( hasBaccSources ) {
        BuildEventList();
        GenerateEventList();
        TrimEventList();
        if( printEventList ) PrintEventList();
    }

    G4cout << "Recording input history" << G4endl;
	// Record input history before beamOn
	BaccOut->RecordInputHistory();
	//	Finally, run the beamOn command
	stringstream command;
	command << "/run/beamOn " << numOfEvents;
        G4cout << "Appliying beamOn command: " << command.str() << G4endl;
	UI->ApplyCommand( command.str().c_str() );


	//      Reset randomization seed for next beanOn
    G4cout << "Setting randomization engine for next beamOn." << G4endl;
    CLHEP::HepRandom::setTheEngine( &randomizationEngine );

    ifstream devrandom("/dev/random");
    G4int seed;
    devrandom.read( (char*)(&seed), sizeof( G4int ) );
    if( seed < 0 ) seed = -seed;
    devrandom.close();
    randomSeed = seed;
    CLHEP::HepRandom::setTheSeed( randomSeed );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetRandomSeed()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::SetRandomSeed( G4int seed )
{	
	randomSeed = seed;
	G4cout << "Randomization seed = " << randomSeed << G4endl;
	CLHEP::HepRandom::setTheSeed( randomSeed );
}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SaveEventSeedVals()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::SaveEventSeedVals( G4int _eventSeedIndex, G4int _eventSeed1, G4int _eventSeed2 )
{
	eventSeedIndex = _eventSeedIndex;
	eventSeed1 = _eventSeed1;
	eventSeed2 = _eventSeed2;

	G4cout << "\n\n\nEvent seeds (idx,1,2): (" << eventSeedIndex << "," << 
		eventSeed1 << "," << eventSeed2 << ")\n\n\n" << G4endl;

};	

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetOutputDir()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::SetOutputDir( G4String dir )
{	
	if( !dir.compare( dir.length()-1, 1, "/" ) )
		outputDir = dir;
	else
		outputDir = dir + "/";
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					UpdateGeometry()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::UpdateGeometry()
{
	//	First, record the current record levels and source types / strengths
	vector<G4String> volNames;
	vector<G4int> recordLevels;
	vector<G4int> recordLevelsOptPhot;
	vector<G4int> recordLevelsThermElec;
	BaccDetectorComponent::source tempSource;
	vector<BaccDetectorComponent::source> sources;
	vector<G4String> sourceVolNames;

    //G4S1Light update
//	BaccMat->UpdateElectricField();

	for( G4int i=0; i<(G4int)BaccComponents.size(); i++ ) {
		volNames.push_back( BaccComponents[i]->GetName() );
		recordLevels.push_back( BaccComponents[i]->GetRecordLevel() );
		recordLevelsOptPhot.push_back(
				BaccComponents[i]->GetRecordLevelOptPhot() );
		recordLevelsThermElec.push_back(
				BaccComponents[i]->GetRecordLevelThermElec() );

		vector<BaccDetectorComponent::source> origSources =
				BaccComponents[i]->GetSources();
		for( G4int j=0; j<(G4int)origSources.size(); j++ ) {
			tempSource.type = origSources[j].type;
			tempSource.activity = origSources[j].activity;
			tempSource.ratio = 0;
            tempSource.particleMass = origSources[j].particleMass;
            tempSource.particleNumber = origSources[j].particleNumber;
            tempSource.parentIsotope = origSources[j].parentIsotope;
            tempSource.sourceAge = origSources[j].sourceAge;
			sources.push_back( tempSource );
			sourceVolNames.push_back( BaccComponents[i]->GetName() );
		}
	}	

	//	Next, update the geometry, which wipes out all detector-component-
	//	related info
	BaccComponents.clear();
	BaccDetector->UpdateGeometry();
		
	//	Finally, reset the recorded record levels and source types / activities
	if( (recordLevels.size() != recordLevelsOptPhot.size()) ||
			(recordLevels.size() != volNames.size()) )
	{
		G4cout << G4endl << G4endl;
		G4cout << "Woah! Found a big bug in the manager. There is not the same"
			   << "number of record" << G4endl
			   << "levels and optical photon record levels. Send an e-mail to"
			   << "Kareem" << G4endl
			   << "(kareem@llnl.gov) and let him know this happened. Please."
			   << G4endl;
		exit(0);
	}
	
	stringstream info;
	
	for ( G4int i=0; i<(G4int)recordLevels.size(); i++ )
	{
        info.str("");
		info << volNames[i] << " " << recordLevels[i];
		SetRecordLevel( info.str() );

		info.str("");
		info << volNames[i] << " " << recordLevelsOptPhot[i];
		SetRecordLevelOptPhot( info.str() );

		info.str("");
		info << volNames[i] << " " << recordLevelsThermElec[i];
		SetRecordLevelThermElec( info.str() );
	}
	
	for ( G4int i=0; i<(G4int)sourceVolNames.size(); i++ )
	{
		info.str("");
	    info << sourceVolNames[i] << " " << sources[i].type->GetName();

		if (sources[i].type->GetName().find("DecayChain") < G4String::npos) 
            info << "_" << sources[i].parentIsotope << " " 
                 << sources[i].activity<< " Bq " << sources[i].sourceAge << " "
                 << " s" ;
		else if (sources[i].type->GetName().find("SingleParticle")
                                                          < G4String::npos)
            info << "_" << sources[i].particleName << " " 
                << sources[i].activity << " Bq " << sources[i].particleEnergy
                << " keV" ;
		else if (sources[i].type->GetName().find("SingleDecay") 
                           < G4String::npos || sources[i].type->GetName().find("G4Decay")	
				< G4String::npos ) 
		    info << "_" << sources[i].particleMass << "_" << sources[i].particleNumber
				 << " " << sources[i].activity << " Bq";
        else if (sources[i].type->GetName().find("Wimp") < G4String::npos)
            info << " " << sources[i].activity << " Bq " 
                 << sources[i].particleEnergy << " keV";
        else
            info << " " << sources[i].activity << " Bq";
        SetSource( info.str() );
	}
	
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					Deregister()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::Deregister( BaccDetectorComponent *component )
{
	for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
		if( BaccComponents[i] == component ) {
			BaccComponents.erase( BaccComponents.begin() + i );
			i = BaccComponents.size();
		}
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetDetectorComponentLookupTable()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4String BaccManager::GetDetectorComponentLookupTable()
{
	stringstream tableStream;
	for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
		tableStream << "ID " << BaccComponents[i]->GetID() << ": "
					<< BaccComponents[i]->GetName() << G4endl;
	return tableStream.str();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetRecordLevel()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::SetRecordLevel( G4String info )
{
    
	//	Start by splitting the G4String info into two parts: the volume name
	//	and the record level.
	
	G4String volName;
	volName = info.substr( 0, info.find_first_of(" ") );
	info = info.substr( info.find_first_of(" ") );
	G4int level = atoi( info.c_str() );
	
	if( volName == "***" ) {
		for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
			BaccComponents[i]->SetRecordLevel( level );
	} else
		for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
			if( BaccComponents[i]->GetName().find(volName) < G4String::npos )
				BaccComponents[i]->SetRecordLevel( level );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetRecordLevels()
//------++++++------++++++------++++++------++++++------++++++------++++++------
std::vector<G4int> BaccManager::GetRecordLevels( G4String volName )
{
	std::vector<G4int> levels;

	for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
		if( BaccComponents[i]->GetName().find(volName) < G4String::npos )
			levels.push_back( BaccComponents[i]->GetRecordLevel() );
	
	return levels;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetComponentRecordLevel()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4int BaccManager::GetComponentRecordLevel(
		BaccDetectorComponent *component )
{
	for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
		if( BaccComponents[i] == component )
			return( component->GetRecordLevel() );
	
	G4cout << "Warning! Looking for a record level in the"
		   << "\"" << ((BaccDetectorComponent*)component)->GetName() << "\" "
		   << "volume, which doesn't seem to" << G4endl
		   << "be a BaccDetectorComponent object." << G4endl;
	
	return 0;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetRecordLevelOptPhot()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::SetRecordLevelOptPhot( G4String info )
{
	//	Start by splitting the G4String info into two parts: the volume name
	//	and the record level.
	
	G4String volName;
	volName = info.substr( 0, info.find_first_of(" ") );
	info = info.substr( info.find_first_of(" ") );
	G4int level = atoi( info.c_str() );
	
	if( volName == "***" ) {
		for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
			BaccComponents[i]->SetRecordLevelOptPhot( level );
	} else
		for( G4int i=0; i<(G4int)BaccComponents.size(); i++ ) {
			if( BaccComponents[i]->GetName().find(volName) < G4String::npos )
				BaccComponents[i]->SetRecordLevelOptPhot( level );
		}
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetRecordLevelsOptPhot()
//------++++++------++++++------++++++------++++++------++++++------++++++------
std::vector<G4int> BaccManager::GetRecordLevelsOptPhot( G4String volName )
{
	std::vector<G4int> levels;

	for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
		if( BaccComponents[i]->GetName().find(volName) < G4String::npos )
			levels.push_back( BaccComponents[i]->GetRecordLevelOptPhot() );
	
	return levels;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetComponentRecordLevelOptPhot()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4int BaccManager::GetComponentRecordLevelOptPhot(
		BaccDetectorComponent *component )
{
	for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
		if( BaccComponents[i] == component )
			return( component->GetRecordLevelOptPhot() );
	
	G4cout << "Warning! Looking for an optical photon record level in the"
		   << "\"" << ((BaccDetectorComponent*)component)->GetName() << "\" "
		   << "volume, which doesn't seem to" << G4endl
		   << "be a BaccDetectorComponent object." << G4endl;
	
	return 0;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetRecordLevelThermElec()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::SetRecordLevelThermElec( G4String info )
{
	//	Start by splitting the G4String info into two parts: the volume name
	//	and the record level.
	
	G4String volName;
	volName = info.substr( 0, info.find_first_of(" ") );
	info = info.substr( info.find_first_of(" ") );
	G4int level = atoi( info.c_str() );
	
	if( volName == "***" ) {
		for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
			BaccComponents[i]->SetRecordLevelThermElec( level );
	} else
		for( G4int i=0; i<(G4int)BaccComponents.size(); i++ ) {
			if( BaccComponents[i]->GetName().find(volName) < G4String::npos )
				BaccComponents[i]->SetRecordLevelThermElec( level );
		}
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetRecordLevelsThermElec()
//------++++++------++++++------++++++------++++++------++++++------++++++------
std::vector<G4int> BaccManager::GetRecordLevelsThermElec( G4String volName )
{
	std::vector<G4int> levels;

	for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
		if( BaccComponents[i]->GetName().find(volName) < G4String::npos )
			levels.push_back( BaccComponents[i]->GetRecordLevelThermElec() );
	
	return levels;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetComponentRecordLevelThermElec()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4int BaccManager::GetComponentRecordLevelThermElec(
		BaccDetectorComponent *component )
{
	for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
		if( BaccComponents[i] == component )
			return( component->GetRecordLevelThermElec() );
	
	G4cout << "Warning! Looking for a thermal electron record level in the"
		   << "\"" << ((BaccDetectorComponent*)component)->GetName() << "\" "
		   << "volume, which doesn't seem to" << G4endl
		   << "be a BaccDetectorComponent object." << G4endl;
	
	return 0;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetComponentByName()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccDetectorComponent *BaccManager::GetComponentByName( G4String volName )
{
	for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
		if( BaccComponents[i]->GetName() == volName )
			return( BaccComponents[i] );
	
	G4cout << "Warning! Could not find component \"" << volName << "\""
		   << G4endl;
	
	return 0;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetSource()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::SetSource( G4String spec )
{
    //	First, tokenize the spec string to extract volume, source type, and
    //	activity (and optional arguemnts)
    //	input="volume sourcename activity act.Units" 
    //	  plus optionally ([age units] or [energy]) for decaychain and single
    //	  particle generators.
  	G4String origSpec = spec;
    G4String arg1;
    G4String volName, sourceTypeName;
    G4String activityUnit;
    G4double activity;
    G4double posX=0., posY=0., posZ=0.;
    G4ThreeVector posSource = G4ThreeVector(0,0,0);
    G4String volMassUnit = "0";  //optional acitivity unit
    G4String sourceAgeString = "0";//decay chain
    G4String sourceAgeUnit = "s";  //decay chain
    G4String singleParticleName="";//single particle
    G4double singleParticleEnergy = 0; //single particle
    G4String singleParticleEnergyUnit = "0";//single particle

    gammaXFiducialR = 20.1; //fiducial volume radius used by gammaX generator
    gammaXFiducialLoZ = 10; //fiducial volume lower Z bound used by gammaX generator
    gammaXFiducialHiZ = 50; //fiducial volume upper Z bound used by gammaX generator
    gammaXReverseFieldR = 23.5; //reverse field region radius used by gammaX generator
    gammaXReverseFieldLoZ = 1; //reverse field lower Z used by gammaX generator
    gammaXReverseFieldHiZ = 5; //reverse field upper Z used by gammaX generator
    G4String gammaXLengthUnit = "cm"; // length units used for preceeding six variables
    gammaXMaxEnergy = 10*keV; // gammaX scatters are assumed to have energies between 0 and gammaXMaxEnergy
    G4String gammaXEnergyUnit = "keV"; // energy units for preceeding variable
    G4String eventsFile;
    twoElectronsDeltaZMax = 1.;
    G4String twoElectronsLengthUnit = "cm"; // length units used for preceeding six variables
    twoElectronsEmin = 0.;
    twoElectronsEmax = 100.;
    G4String twoElectronsEnergyUnit = "keV"; // energy units for preceeding variable

  	//G4double wimpMass = 0; //Wimp
  	//G4String wimpMassUnit = "0"; //Wimp
    stringstream sspec;
    sspec.str(spec);
    G4cout << "Setting Source = \"" << sspec.str() << "\"" << G4endl;

    //    The source can be set by specifying a position (x,y,z). First 
    //    check to see if the first argument is a LUXSim source name
    //    or a volume name..
    sspec >> arg1;
    G4bool posDefined = false;
    if (arg1.find("SingleParticle") < G4String::npos || arg1.find("DecayChain") < G4String::npos 
	        || arg1.find("SingleDecay") < G4String::npos || arg1.find("G4Decay") < G4String::npos){
      sourceTypeName = arg1;
      sspec >> posX >> posY >> posZ >> activity >> activityUnit;
      volName = "universe";
      posSource = G4ThreeVector(posX, posY, posZ);
      posDefined = true;
    }
    if(!posDefined){
        for( G4int i=0; i<BaccSourceCat->GetNumSourceTypes(); i++ ){
	    if( BaccSourceCat->GetSourceType(i)->GetName() == arg1 ) {
	        i = BaccSourceCat->GetNumSourceTypes();
		sourceTypeName = arg1;
		if( sourceTypeName!="MUSUN" ) // MUSUN is volume independent
		  sspec >> posX >> posY >> posZ >> activity >> activityUnit;
		else{ activity = 1; activityUnit = "Bq"; }
		volName = "universe";
		posSource = G4ThreeVector(posX, posY, posZ);
		posDefined = true;
	    }
	}
    }    
    //    The first argument is a volume name, not a source name.
    if(!posDefined){
      volName = arg1;
      sspec >> sourceTypeName >> activity >> activityUnit;
    }
    

    if( sourceTypeName.find("SingleParticle") < G4String::npos ) 
        sspec >> singleParticleEnergy >> singleParticleEnergyUnit;
    if( sourceTypeName.find("DecayChain") < G4String::npos ) 
        sspec >> sourceAgeString >> sourceAgeUnit;
	  if( sourceTypeName.find("Wimp") < G4String::npos )
        sspec >> singleParticleEnergy >> singleParticleEnergyUnit;
        //a trick 
		//sspec >> wimpMass >> wimpMassUnit;
    if( sourceTypeName.find("gamma-X") < G4String::npos  ) {
        sspec >> gammaXFiducialR >> gammaXFiducialLoZ >> gammaXFiducialHiZ;
        sspec >> gammaXReverseFieldR >> gammaXReverseFieldLoZ >> gammaXReverseFieldHiZ;
        sspec >> gammaXLengthUnit;
        sspec >> gammaXMaxEnergy >> gammaXEnergyUnit;
    }
    if( sourceTypeName.find("TwoElectrons") < G4String::npos  ) {
        sspec >> twoElectronsDeltaZMax >> twoElectronsLengthUnit;
        sspec >> twoElectronsEmin >> twoElectronsEmax >> twoElectronsEnergyUnit;
    }
    if( sourceTypeName.find("EventsFile") < G4String::npos  ) {
        sspec >> eventsFile;
        G4cout<<eventsFile.data()<<G4endl;
        LoadEventsFile(eventsFile);
    }
    //Error in sspec reading:
    if( sspec.rdstate()!=0 && sspec.fail()!=0 ) { 
        G4cout << "ERROR in SetSource(): " << G4endl; 
        G4cout << "Usage: /Bacc/source/set VolName SourceName Activity "
               << "ActivityUnits " << G4endl;
        if( sourceTypeName.find("DecayChain")<G4String::npos)
            G4cout << "\tRequired for DecayChain: AgeOfSource AgeUnits" 
                   << G4endl;
        if( sourceTypeName.find("SingleParticle")<G4String::npos)
            G4cout << "\tRequired for SingleParticle: Energy EnergyUnits" 
                   << G4endl;
		if( sourceTypeName.find("Wimp")<G4String::npos)
			G4cout << "\tRequired for Wimp: Mass MassUnits" << G4endl;
        //G4cout << " contact Nick Walsh (niwalsh@ucdavis.edu)" << G4endl;
        exit(0);
    }
    G4cout << " Source parsed as: \"" ;
    G4cout << volName << " " << sourceTypeName << " " << activity << " "
           << activityUnit << " " ;
    if( sourceTypeName.find("SingleParticle") < G4String::npos ||
        sourceTypeName.find("Wimp") < G4String::npos  )
        G4cout << singleParticleEnergy << " " << singleParticleEnergyUnit;
    else if( sourceTypeName.find("DecayChain") < G4String::npos )
        G4cout << sourceAgeString << " " << sourceAgeUnit;
  	//else if( sourceTypeName.find("Wimp") < G4String::npos )
  	//	G4cout << wimpMass << " " << wimpMassUnit;
    G4cout << "\"" << G4endl;
    
    
    //  Check if ActivityUnit is specified per mass
	  spec = activityUnit;
    activityUnit = spec.substr(0,spec.find_first_of('/') );
    if( spec.length() == activityUnit.length() )
        G4cout << "  Total activity for entire mass is specified" << G4endl;
    else {
        spec = spec.substr(activityUnit.length()+1);
        volMassUnit = spec;
        G4cout << "  Activity per unit mass is specified: " << G4endl;
    }

    //  For SingleParticle find and check the specified particle name
    if (sourceTypeName.find("SingleParticle") < G4String::npos ) {
        G4String sourceTypeSingleParticle = sourceTypeName.substr(0,
                               sourceTypeName.find_first_of("_"));
        if( sourceTypeSingleParticle.length() == sourceTypeName.length() ) {
            G4cout << "No Particle Name given with SingleParticle_?: ERROR"
                   << G4endl;
            exit(0);
        }
        singleParticleName = sourceTypeName.substr(
                                sourceTypeSingleParticle.length()+1 );
        sourceTypeName = sourceTypeSingleParticle;
        // Now check against a manually identified list of /gps/particle options
        // excluding ion and genericIon
        G4String gpsmatch;
        G4String gpsParticleList1 = "alpha,anti_nu_e,anti_nu_mu,anti_nu_tau,"
            "chargedgeantino,deuteron,e+,e-,gamma,geantino,mu+,mu-,neutron,nu_e"
            ",nu_mu,nu_tau,opticalphoton,pi+,pi0,pi-,tau+,tau-,thermalelectron";
        stringstream gpsParticlesSS(gpsParticleList1);
        G4bool gpsIsGood = false;
        while ( getline( gpsParticlesSS, gpsmatch, ',') && !gpsIsGood ) {
            if( gpsmatch == singleParticleName )
                gpsIsGood = true;
        }
        if( !gpsIsGood ) {
            //try other matches (without exact match requirement)
            G4String gpsParticleList2 = "kaon,gluon,f0(,f1(,f2(,k0(,k1(,k2(,"
                "b1(,k_star,omega,rho,B+,B0,B-,D+,D0,D-,Ds+,Ds-,J/psi,He3,a0,N("
                ",a0(,a1(,a2(,quark,xi,eta,delta,lambda,sigma,triton";
            gpsParticlesSS.clear();
            gpsParticlesSS.str(gpsParticleList2);
            while ( getline( gpsParticlesSS, gpsmatch, ',') && !gpsIsGood ) {
                if( singleParticleName.find(gpsmatch) < G4String::npos )
                    gpsIsGood = true;
            }
        }
        if( !gpsIsGood ) {
            G4cout << "Error in SetSource: SingleParticle_particleName" 
                   << G4endl;
            G4cout << "\tparticleName: " << singleParticleName << " not found" 
                   << G4endl;
            G4cout << "\tuse names from /gps/List but NOT ion or Genericion"
                   << G4endl;
            exit(0);
        }
    }


    // If DecayChain find the specified isotope chain
    G4String parentIsotope = "0";
    if( sourceTypeName.find("DecayChain") < G4String::npos) {
        G4String sourceTypeDecayChain = sourceTypeName.substr(
                                0, sourceTypeName.find_first_of("_"));
        if( sourceTypeDecayChain.length() == sourceTypeName.length() ) {
            G4cout << "No Parent Isotope specified for DecayChain_?: ERROR1" 
                   << G4endl;
            exit(0);
        } 
        parentIsotope = sourceTypeName.substr(sourceTypeDecayChain.length()+1);
        sourceTypeName = sourceTypeDecayChain;
        hasDecayChainSources=true;
    }    

    // If SingleDecay or G4Decay find the specified A and Z of nucleus to decay
    G4int aa = 0;
    G4int zz = 0;
    if( sourceTypeName.find("SingleDecay") < G4String::npos ||
	    sourceTypeName.find("G4Decay") < G4String::npos ) {
        if( sourceTypeName.find("G4Decay") < G4String::npos ) SetG4DecayBool(true);
        G4String sourceTypeSingleDecay_G4Decay = sourceTypeName.substr(
                         0, sourceTypeName.find_first_of("_") );
        if( sourceTypeSingleDecay_G4Decay.length() == sourceTypeName.length() ) {
            G4cout << "No infomation for A and Z of SingleDecay_A_Z: ERROR!" 
                   << G4endl;
            exit(0);
        }
        sourceTypeName = sourceTypeName.substr( 
                         sourceTypeSingleDecay_G4Decay.length()+1 );
        G4String aaStr = sourceTypeName.substr( 
                         0, sourceTypeName.find_first_of("_") );
        G4String zzStr = sourceTypeName.substr( aaStr.length()+1 );
        if ( zzStr.length() > 0 ) 
            zzStr = sourceTypeName.substr( aaStr.length()+1 );
        else 
            G4cout << "error in assigning SingleDecay and/or G4Decay, check syntax" << G4endl;
        sourceTypeName = sourceTypeSingleDecay_G4Decay;
        aa = atoi( aaStr.c_str() ) ; 
        zz = atoi( zzStr.c_str() ) ;  
    }

  	//	Check to see that the source type is known
  	G4bool foundSource = false;
  	BaccSource *currentSource = 0;
  	for( G4int i=0; i<BaccSourceCat->GetNumSourceTypes(); i++ )
  		if( BaccSourceCat->GetSourceType(i)->GetName() == sourceTypeName ) {
  			currentSource = BaccSourceCat->GetSourceType(i);
  			foundSource = true;
  			i = BaccSourceCat->GetNumSourceTypes();
  		}
  	if( !foundSource ) {
  		G4cout << G4endl << G4endl
  			   << "Cannot find source type \"" << sourceTypeName << "\"!"
  			   << G4endl;
  		exit(0);
  	}
    
	  //	Verify that the activity-per-mass mass unit is understood
    vector<G4String> massUnits;
    massUnits.push_back("0");
    massUnits.push_back("g");
    massUnits.push_back("kg");
    massUnits.push_back("mg");
    massUnits.push_back("ug");
    G4bool goodSourceUnit = false;
    for (G4int i=0; i<(G4int)massUnits.size(); i++ )
        //if( volMassUnit.find(massUnits[i]) < G4String::npos )
        if( volMassUnit == massUnits[i] )
            goodSourceUnit = true;
    if( !goodSourceUnit ) {
        G4cout << G4endl << G4endl
               << "Unknown source strength mass unit \"" << volMassUnit 
               << "\"!" << G4endl;
        exit(0);
    }    
    // normalize activity-per-mass mass units
  	if( volMassUnit.find("m") < G4String::npos )
  		activity *= 1e+3;
  	if( volMassUnit.find("u") < G4String::npos )
  		activity *= 1e+6;
  	if( volMassUnit.find("k") < G4String::npos )
  		activity *= 1e-3;
    	
    //  Verify the singleparticle energy is understood
    vector<G4String> eunits;
    eunits.push_back("0"); //for non-singleparticle
    eunits.push_back("eV"); eunits.push_back("*eV");
    eunits.push_back("keV"); eunits.push_back("*keV");
    eunits.push_back("MeV"); eunits.push_back("*MeV");
    eunits.push_back("GeV"); eunits.push_back("*GeV");
    G4bool goodEnergyUnit = true;
    for( G4int i=0; i<(G4int)eunits.size(); i++)
        //if( singleParticleEnergyUnit.find(eunits[i]) < G4String::npos )
        if( singleParticleEnergyUnit == eunits[i] )
            goodEnergyUnit = true;
    if( !goodEnergyUnit) {
        G4cout << G4endl << G4endl;
        G4cout << "Unknown single particle energy unit \"" 
               << singleParticleEnergyUnit
               << "\n Proper Usage for example is 4*keV" << G4endl;
        exit(0);
    }	
    //normalize the energy to keV (from eV, MeV, or GeV)
    if( singleParticleEnergyUnit.find("0") < G4String::npos ) {;}
    else if( singleParticleEnergyUnit.find("keV") < G4String::npos ) {
        singleParticleEnergy = singleParticleEnergy * keV ;
        G4cout << "  SingleParticle energy in keV = " 
               << singleParticleEnergy/keV << G4endl;
    } else if( singleParticleEnergyUnit.find("MeV") < G4String::npos ) {
        singleParticleEnergy = singleParticleEnergy * 1.e3 * keV ; 
        G4cout << "  SingleParticle energy in 'keV' = " 
               << singleParticleEnergy/keV << "*keV " << G4endl;
    } else if( singleParticleEnergyUnit.find("GeV") < G4String::npos ) {
        singleParticleEnergy = singleParticleEnergy * 1.e6 * keV ; 
        G4cout << "  SingleParticle energy in 'keV' = " 
               << singleParticleEnergy/keV << "*keV " << G4endl;
    } else if( singleParticleEnergyUnit=="eV" || 
               singleParticleEnergyUnit=="*eV"      ) {
        singleParticleEnergy = singleParticleEnergy * 1.e-3 * keV; 
        G4cout << "  SingleParticle energy in 'keV' = " 
               << singleParticleEnergy/keV << "*keV " << G4endl;
    } else { 
        G4cout << "No energy scaling found for Single Particle " << G4endl;
        G4cout << " Usage: *eV *keV *MeV or *GeV " << G4endl;
        exit(0);
    }

    //  Verify the gammaX energy is understood
    goodEnergyUnit = false;
    for( G4int i=0; i<(G4int)eunits.size(); i++)
        if( gammaXEnergyUnit == eunits[i] )
            goodEnergyUnit = true;
    if( !goodEnergyUnit) {
        G4cout << G4endl << G4endl;
        G4cout << "Unknown GammaX energy unit \"" 
               << gammaXEnergyUnit
               << "\n Proper Usage for example is 4 keV" << G4endl;
        exit(0);
    }	
    //normalize the energy to keV (from eV, MeV, or GeV)
    if( gammaXEnergyUnit.find("0") < G4String::npos ) {;}
    else if( gammaXEnergyUnit.find("keV") < G4String::npos ) {
        gammaXMaxEnergy = gammaXMaxEnergy * keV ;
        G4cout << "  GammaX energy in keV = " 
               << gammaXMaxEnergy/keV << G4endl;
    } else if( gammaXEnergyUnit.find("MeV") < G4String::npos ) {
        gammaXMaxEnergy = gammaXMaxEnergy * 1e3 * keV ; 
        G4cout << "  GammaX energy in 'keV' = " 
               << gammaXMaxEnergy/keV << "*keV " << G4endl;
    } else if( gammaXEnergyUnit.find("GeV") < G4String::npos ) {
        gammaXMaxEnergy = gammaXMaxEnergy * 1e6 * keV ; 
        G4cout << "  GammaX energy in 'keV' = " 
               << gammaXMaxEnergy/keV << "*keV " << G4endl;
    } else if( gammaXEnergyUnit=="eV" || 
               gammaXEnergyUnit=="*eV"      ) {
        gammaXMaxEnergy = gammaXMaxEnergy * 1e-3 * keV; 
        G4cout << "  GammaX energy in 'keV' = " 
               << gammaXMaxEnergy/keV << "*keV " << G4endl;
    } else { 
        G4cout << "No energy scaling found for GammaX " << G4endl;
        G4cout << " Usage: eV keV MeV or GeV " << G4endl;
        exit(0);
    }

    //  Verify the two electron energy is understood
    goodEnergyUnit = false;
    for( G4int i=0; i<(G4int)eunits.size(); i++)
        if( twoElectronsEnergyUnit == eunits[i] )
            goodEnergyUnit = true;
    if( !goodEnergyUnit) {
        G4cout << G4endl << G4endl;
        G4cout << "Unknown Two Electrons energy unit \"" 
               << twoElectronsEnergyUnit
               << "\n Proper Usage for example is 4 keV" << G4endl;
        exit(0);
    }	
    //normalize the energy to keV (from eV, MeV, or GeV)
    if( twoElectronsEnergyUnit.find("0") < G4String::npos ) {;}
    else if( twoElectronsEnergyUnit.find("keV") < G4String::npos ) {
        twoElectronsEmin = twoElectronsEmin * keV ;
        twoElectronsEmax = twoElectronsEmax * keV ;
        G4cout << "  Two Electrons energy in keV = " 
               << twoElectronsEmax/keV << G4endl;
    } else if( twoElectronsEnergyUnit.find("MeV") < G4String::npos ) {
        twoElectronsEmin = twoElectronsEmin * 1e3 * keV ;
        twoElectronsEmax = twoElectronsEmax * 1e3 * keV ;
        G4cout << "  Two Electrons energy in 'keV' = " 
               << twoElectronsEmax/keV << "*keV " << G4endl;
    } else if( twoElectronsEnergyUnit.find("GeV") < G4String::npos ) {
        twoElectronsEmin = twoElectronsEmin * 1e6 * keV ;
        twoElectronsEmax = twoElectronsEmax * 1e6 * keV ;
        G4cout << "  Two Electrons energy in 'keV' = " 
               << twoElectronsEmax/keV << "*keV " << G4endl;
    } else if( twoElectronsEnergyUnit=="eV" || 
               twoElectronsEnergyUnit=="*eV"      ) {
        twoElectronsEmin = twoElectronsEmin * 1e-3 * keV ;
        twoElectronsEmax = twoElectronsEmax * 1e-3 * keV ;
        G4cout << "  Two Electrons energy in 'keV' = " 
               << twoElectronsEmax/keV << "*keV " << G4endl;
    } else { 
        G4cout << "No energy scaling found for Two Electrons " << G4endl;
        G4cout << " Usage: eV keV MeV or GeV " << G4endl;
        exit(0);
    }

    //  Verify the gammaX positions are understood
    vector<G4String> lunits;
    lunits.push_back("0"); //for non-singleparticle
    lunits.push_back("um"); eunits.push_back("*um");
    lunits.push_back("mm"); eunits.push_back("*mm");
    lunits.push_back("cm"); eunits.push_back("*cm");
    lunits.push_back("m"); eunits.push_back("*m");
    G4bool goodLengthUnit = false;
    for( G4int i=0; i<(G4int)lunits.size(); i++)
        if( gammaXLengthUnit == lunits[i] )
            goodLengthUnit = true;
    if( !goodLengthUnit) {
        G4cout << G4endl << G4endl;
        G4cout << "Unknown GammaX length unit \"" 
               << gammaXLengthUnit
               << "\n Proper Usage for example is 4 cm" << G4endl;
        exit(0);
    }	
    //normalize the energy to keV (from eV, MeV, or GeV)
    if( gammaXLengthUnit.find("0") < G4String::npos ) {;}
    else if( gammaXLengthUnit.find("mm") < G4String::npos ) {
        gammaXFiducialR = gammaXFiducialR * mm ;
        gammaXFiducialLoZ = gammaXFiducialLoZ * mm ;
        gammaXFiducialHiZ = gammaXFiducialHiZ * mm ;
        gammaXReverseFieldR = gammaXReverseFieldR * mm ;
        gammaXReverseFieldLoZ = gammaXReverseFieldLoZ * mm ;
        gammaXReverseFieldHiZ = gammaXReverseFieldHiZ * mm ;
    } else if( gammaXLengthUnit.find("cm") < G4String::npos ) {
        gammaXFiducialR = gammaXFiducialR * cm ;
        gammaXFiducialLoZ = gammaXFiducialLoZ * cm ;
        gammaXFiducialHiZ = gammaXFiducialHiZ * cm ;
        gammaXReverseFieldR = gammaXReverseFieldR * cm ;
        gammaXReverseFieldLoZ = gammaXReverseFieldLoZ * cm ;
        gammaXReverseFieldHiZ = gammaXReverseFieldHiZ * cm ;
    } else if( gammaXLengthUnit.find("m") < G4String::npos ) {
        gammaXFiducialR = gammaXFiducialR * 1e-2 * cm ;
        gammaXFiducialLoZ = gammaXFiducialLoZ * 1e2 * m ;
        gammaXFiducialHiZ = gammaXFiducialHiZ * 1e2 * m ;
        gammaXReverseFieldR = gammaXReverseFieldR * 1e2 * m ;
        gammaXReverseFieldLoZ = gammaXReverseFieldLoZ * 1e2 * m ;
        gammaXReverseFieldHiZ = gammaXReverseFieldHiZ * 1e2 * m ;
    } else if( gammaXLengthUnit=="um" || 
               gammaXLengthUnit=="*um"      ) {
        gammaXFiducialR = gammaXFiducialR * 1e-4 * um ;
        gammaXFiducialLoZ = gammaXFiducialLoZ * 1e-4 * um ;
        gammaXFiducialHiZ = gammaXFiducialHiZ * 1e-4 * um ;
        gammaXReverseFieldR = gammaXReverseFieldR * 1e-4 * um ;
        gammaXReverseFieldLoZ = gammaXReverseFieldLoZ * 1e-4 * um ;
        gammaXReverseFieldHiZ = gammaXReverseFieldHiZ * 1e-4 * um ;
    } else { 
        G4cout << "No length scaling found for GammaX " << G4endl;
        G4cout << " Usage: um mm cm or m " << G4endl;
        exit(0);
    }


    //  Verify the twoElectrons positions are understood
    goodLengthUnit = false;
    for( G4int i=0; i<(G4int)lunits.size(); i++)
        if( twoElectronsLengthUnit == lunits[i] )
            goodLengthUnit = true;
    if( !goodLengthUnit) {
        G4cout << G4endl << G4endl;
        G4cout << "Unknown Two Electrons length unit \"" 
               << twoElectronsLengthUnit
               << "\n Proper Usage for example is 4 cm" << G4endl;
        exit(0);
    }	
    //normalize the energy to keV (from eV, MeV, or GeV)
    if( twoElectronsLengthUnit.find("0") < G4String::npos ) {;}
    else if( twoElectronsLengthUnit.find("mm") < G4String::npos ) {
        twoElectronsDeltaZMax = twoElectronsDeltaZMax * mm;
    } else if( gammaXLengthUnit.find("cm") < G4String::npos ) {
        twoElectronsDeltaZMax = twoElectronsDeltaZMax * cm;
    } else if( gammaXLengthUnit.find("m") < G4String::npos ) {
        twoElectronsDeltaZMax = twoElectronsDeltaZMax * m;
    } else if( gammaXLengthUnit=="um" || 
               gammaXLengthUnit=="*um"      ) {
        twoElectronsDeltaZMax = twoElectronsDeltaZMax * um;
    } else { 
        G4cout << "No length scaling found for Two Electrons " << G4endl;
        G4cout << " Usage: um mm cm or m " << G4endl;
        exit(0);
    }



  	//	Verify that the source strength unit is understood
  	vector<G4String> units;
  	units.push_back("Ci");
  	units.push_back("mCi");
  	units.push_back("uCi");
  	units.push_back("nCi");
  	units.push_back("kBq");
  	units.push_back("Bq");
  	units.push_back("mBq");
  	units.push_back("uBq");
  	units.push_back("nBq");
  	goodSourceUnit = false;
  	for( G4int i=0; i<(G4int)units.size(); i++ )
  		if( activityUnit == units[i] )
  			goodSourceUnit = true;
  	if( !goodSourceUnit ) {
  		G4cout << G4endl << G4endl
  			   << "Unknown source strength unit \"" << activityUnit << "\"!"
  			   << G4endl;
  		exit(0);
  	}
  	//	Next, normalize the source activity
  	//G4double activity = atof( activityString.c_str() );
  	if( activityUnit.find("Ci") < G4String::npos )
  		activity *= 3.7e10;
  	if( activityUnit.find("m") < G4String::npos )
  		activity *= 1e-3;
  	if( activityUnit.find("u") < G4String::npos )
  		activity *= 1e-6;
  	if( activityUnit.find("n") < G4String::npos )
  		activity *= 1e-9;
  	if( activityUnit.find("k") < G4String::npos )
  		activity *= 1e3;

    //  Source Age checks
    vector<G4String> timeUnits;
    timeUnits.push_back("s");
    timeUnits.push_back("Ms");
    timeUnits.push_back("Gs");
    timeUnits.push_back("h");
    timeUnits.push_back("d");
    timeUnits.push_back("y");
    timeUnits.push_back("yr");
    timeUnits.push_back("ky");
    timeUnits.push_back("kyr");
    timeUnits.push_back("Myr");
    timeUnits.push_back("My");
    timeUnits.push_back("Gyr");
    timeUnits.push_back("Gy");
    goodSourceUnit = false;
    for( G4int i=0; i<(G4int)timeUnits.size(); i++ )
        if( sourceAgeUnit == timeUnits[i] )
            goodSourceUnit = true;
    if( !goodSourceUnit ) {
        G4cout << G4endl << G4endl << "Unknown source age units \""
               << sourceAgeUnit << "\"!" << G4endl;
        exit(0);
    }
    //  normalize source age to seconds
    G4double sourceAge = atof( sourceAgeString.c_str() );
    if( sourceAgeUnit.find("k") < G4String::npos )
        sourceAge *= 1e+3 ;
    if( sourceAgeUnit.find("M") < G4String::npos )
        sourceAge *= 1e+6 ;
    if( sourceAgeUnit.find("G") < G4String::npos )
        sourceAge *= 1e+9 ;
    if( sourceAgeUnit.find("d") < G4String::npos )
        sourceAge *= 3600*24 ;
    if( sourceAgeUnit.find("h") < G4String::npos )
        sourceAge *= 3600 ;
    if( sourceAgeUnit.find("y") < G4String::npos )
        sourceAge *= 3600*24*365.25 ;


    G4bool volumeFound = false;
    // the following variable is identical to activity, 
    // but allows for the case of multiply named
    // volumes set with activity-per-mass to be scale
    G4double activityToSet = activity ;
		for( G4int i=0; i<(G4int)BaccComponents.size(); i++ ) {
		    if( BaccComponents[i]->GetName().find(volName) < G4String::npos ) {
            if( volMassUnit != "0" ){
                G4double volMass = BaccComponents[i]->GetMass();
                activityToSet = activity * volMass ;
                G4cout << "   " << BaccComponents[i]->GetName() 
                       << " Mass = " << volMass << " g yields Total Activity of " 
                       << activityToSet << " Bq" << G4endl;
                //G4cout << "mass: " << volMass ; 
            }
            
            BaccComponents[i]->AddSource( currentSource, activityToSet, 
                aa, zz, parentIsotope, sourceAge, 
		singleParticleName, singleParticleEnergy, posDefined, posSource);//, wimpMass);
            volumeFound = true;
        }
    }
    if( !volumeFound ) {
        G4cout << "Error: Did not recognize the volume in which you wanted"
               << " to set the source, \"" << volName << "\"" << G4endl;
        G4cout << "Usage: /Bacc/source/set volName sourceName activity"
               << " activityUnits" << G4endl;
        exit(0);
    }
    G4cout << G4endl;

    hasBaccSources = true;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					ResetSources()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::ResetSources()
{
    for( G4int i=0; i<(G4int)sourceByVolume.size(); i++ )
      sourceByVolume[i].component->ResetSources();
    sourceByVolume.clear();
    totalSimulationActivity=0;
    hasBaccSources=false;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BuildEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::BuildEventList()
{
    // Builds a binary search tree ordered by time of the parent particle event
    // time. Begins at 0*ns and runs to a time determined by the total activity
    // and number of events requested. The purpose of the window width is to
    // create empty nodes in the BST to avoid quasi-degeneracy, but we won't
    // actually cut on the window end time when inserting events. Note that the 
    // window end time has a 10% pad on it, just to add a bit of overhead space
    G4int numEvts = GetNumEvents();
    G4double initialActivity = GetTotalSimulationActivity();

    //G4double windowStart, windowEnd; //units=seconds
    windowStart = 0;
    windowEnd   = 1e10;

    G4int numVols = (G4int)sourceByVolume.size();
    G4int recordTreeDepth = 20; 
    // For numEvents >> 1e7 increase depth 
    // and numVols > 1 increase depth and widen window
    if( numVols < 1 || initialActivity==0 ) {
        G4cout << "no activity registered"<<G4endl;
    }
    else {
        if(numEvents > 100) windowEnd = 2.*numEvts/initialActivity ;
        else                windowEnd = 4.*numEvts/initialActivity ;
       // if(numEvents > 1000)    windowEnd = 1.1*numEvts*numVols/initialActivity;
       // else if(numEvents > 50) windowEnd = 2.*numEvts*numVols/initialActivity ;
       // else                    windowEnd = 4.*numEvts*numVols/initialActivity ;
    }
    // The initialization levels(depth) created in the BST is defaulted to 20.
    // These levels contain empty nodes to subdivide the space to reduce
    // degeneracy of the BST. The number of nodes is 2^level-1 so be careful
    // about using many levels (i.e., 10 level will create 1023 empty nodes).
    // By experience 20 is reasonable for O(1e7) events. The number of events
    // added to the BST is trickier when multiple sources in multiple volumes
    // are requested. When using "PMT_Window" or similar each volume creates
    // numEvent of entries. 

    if( numEvts > 2.e6 ) {
        G4int npower = 1;
        while( numEvts/(2.e6*pow(2.,npower)) > 1. && recordTreeDepth < 25) { 
            npower++;
            recordTreeDepth++;
        }
    } 
    if( hasDecayChainSources ) recordTreeDepth++ ;//10-14 more decays/activity

    G4cout << "\n============================================================="
           << "========" << G4endl;
    G4cout << "Building binary search tree (BST) with depth " 
           << recordTreeDepth << G4endl; 
    G4cout << "  The BST time window runs from " << windowStart << " to " 
           << windowEnd << " s" << G4endl;

    recordTree = new BaccBST(recordTreeDepth, windowStart, windowEnd, numEvts);

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::GenerateEventList()
{
    // Each component and each source need to have population calculated
    // and the events generated
    for( G4int i=0; i<(G4int)sourceByVolume.size(); i++ ) 
        if( sourceByVolume[i].component ) 
            sourceByVolume[i].component->GenerateEventList(i);

    // Do this every so often, because PMT_Window has between 122-142 volumes!
    // Same as TrimEventList(); but with out terminating "======..."
    while( recordTree->GetNumNonemptyNodes()>numEvents ) recordTree->PopLast();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					LoadEventsFile()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::LoadEventsFile(G4String eventsFile)
{
    std::ifstream file;
    file.open(eventsFile);
    if (!file.is_open()) {
        G4cout<<G4endl<<G4endl<<G4endl;
        G4cout<<"Events File Not Found!"<<G4endl;
        G4cout<<G4endl<<G4endl<<G4endl;
        exit(0);
    }
    G4int evtN;
    G4int particleID;
    G4double energyDep_keV;
    G4double positionX_cm;
    G4double positionY_cm;
    G4double positionZ_cm;
    while( 1 ) {
        file >> evtN >> particleID >> energyDep_keV >> positionX_cm >> positionY_cm >> positionZ_cm;
        if( file.fail() ) {
             break;
        }
        qEvtN.push(evtN);
        qParticleID.push(particleID);
        qEnergyDep_keV.push(energyDep_keV*keV);
        qPositionX_cm.push(positionX_cm*cm);
        qPositionY_cm.push(positionY_cm*cm);
        qPositionZ_cm.push(positionZ_cm*cm);
    }
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					NextEventToGenerate()
//------++++++------++++++------++++++------++++++------++++++------++++++------

G4int BaccManager::NextEventToGenerate()
{

    currentEvtN = qEvtN.front();
    while ( qEvtN.front() == currentEvtN && 
          !qEvtN.empty() ) {
        qEvtN.pop();
        currentParticleID.push(qParticleID.front());
        qParticleID.pop();
        currentEnergyDep_keV.push(qEnergyDep_keV.front());
        qEnergyDep_keV.pop();
        currentX_cm.push(qPositionX_cm.front());
        qPositionX_cm.pop();
        currentY_cm.push(qPositionY_cm.front());
        qPositionY_cm.pop();
        currentZ_cm.push(qPositionZ_cm.front());
        qPositionZ_cm.pop();
        
    }

    return currentParticleID.size();

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					NextParticleIDToGenerate()
//------++++++------++++++------++++++------++++++------++++++------++++++------

G4int BaccManager::NextParticleIDToGenerate()
{

    G4int partilceID = currentParticleID.front();
    currentParticleID.pop();
    return partilceID;

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					NextEnergyDepToGenerate()
//------++++++------++++++------++++++------++++++------++++++------++++++------

G4double BaccManager::NextEnergyDepToGenerate()
{

    G4double energyDep = currentEnergyDep_keV.front();
    currentEnergyDep_keV.pop();
    return energyDep;

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					NextPositionToGenerate()
//------++++++------++++++------++++++------++++++------++++++------++++++------

G4ThreeVector BaccManager::NextPositionToGenerate()
{

    G4ThreeVector pos;
    pos[0] = currentX_cm.front();
    currentX_cm.pop();
    pos[1] = currentY_cm.front();
    currentY_cm.pop();
    pos[2] = currentZ_cm.front();
    currentZ_cm.pop();
    return pos;

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					TrimEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::TrimEventList()
{
    // At the end of this process there will be more events in the decay record
    // than requested, so the excess can be reduced.
    G4cout << "Reducing the number of events in the tree to " << numEvents
           << "..." ;
    while( recordTree->GetNumNonemptyNodes()>numEvents ) recordTree->PopLast();
    G4cout << "Done. " << recordTree->GetNumNonemptyNodes() <<" events remain."
           << "\n============================================================="
           << "========" << G4endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					RecordTreeInsert()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::RecordTreeInsert(Isotope *iso, G4double t, G4ThreeVector p,
                        G4int sourceByVolumeID, G4int sourcesID)
{
    // Time sent and received in *ns. RecordTree window is set in seconds.
    if( t/ns < windowEnd*1.e9 )
        recordTree->Insert(iso, t, p, sourceByVolumeID, sourcesID);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateEvent()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::GenerateEvent( G4GeneralParticleSource *particleGun,
        G4Event *event )
{
    if( hasBaccSources && recordTree->GetNumNonemptyNodes()>0 ) {
        decayNode* firstNode;
        G4bool searchingNodes = true;
        while(searchingNodes){
            firstNode = recordTree->GetEarliest();
            if(firstNode->Z) searchingNodes = false;
            else recordTree->PopEarliest();
        }

        sourceByVolume[firstNode->sourceByVolumeID].component->
              GenerateFromEventList(particleGun, event, firstNode);
        recordTree->PopEarliest();
    }
    else { 
        G4cout << "No more events found OR using macro command sources" << G4endl;

    }
}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//					AddDeposition()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::AddDeposition( BaccDetectorComponent* component,
				stepRecord aStep )
{
	//	Go through all the detector components, and if the volume being recorded
	//	is a registered detector component, pass that info to the correct object
	for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
		if( BaccComponents[i] == component ) {
			BaccComponents[i]->AddDeposition( aStep );
			i = (G4int)BaccComponents.size();
		}
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					CapturePhotons()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4bool BaccManager::CapturePhotons( BaccDetectorComponent* component )
{
	//	Go through all the detector components, and if the volume with the
	//	optical photon is set to capture optical photons, return true so that
	//	the optical photon track can be killed
	G4bool response = false;
	
	for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
		if( BaccComponents[i] == component ) {
			if( BaccComponents[i]->GetCapturePhotons() )
				response = true;
			i = (G4int)BaccComponents.size();
		}
	
	return response;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					RecordValues()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::RecordValues( G4int eventNum )
{
	//	Go through all the detector components, and if any have an record
	//	level greater than one, send the vector of steps to BaccOutput for
	//	recording.
	for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
		if( (BaccComponents[i]->GetRecordLevel() ||
				BaccComponents[i]->GetRecordLevelOptPhot() ||
				BaccComponents[i]->GetRecordLevelThermElec() )
				&& BaccComponents[i]->GetEventRecord().size() )
			BaccOut->RecordEventByVolume( BaccComponents[i], eventNum );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					ClearRecords()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::ClearRecords()
{
	//	Go through all the detector components and clear the information
	//	vectors.
	for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
			BaccComponents[i]->ClearRecord();
	
	//	Now clear all the primary particles
	primaryParticles.clear();

	//close the output file before next run
	if ( GetRunEndedCleanly() ){
		if ( BaccOut ) delete BaccOut;
		BaccOut = NULL;
	}
}

 //------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetComponentMass()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::SetComponentMass( G4String parameters )
{
    G4String componentName = parameters.substr( 0, parameters.find(" ") );
    parameters = parameters.substr( parameters.find(" ") + 1);
    G4String massString = parameters.substr( 0, parameters.find(" ") );
    parameters = parameters.substr( parameters.find(" ") + 1);
    G4String unitString = parameters.substr( 0, parameters.find(" ") );
    
    
    G4double cMass = atof( massString.c_str() );
    
    cMass *= g;
    if( unitString == "ug" ) cMass *= 1e-6;
    if( unitString == "mg" ) cMass *= 1e-3;
    if( unitString == "kg" ) cMass *= 1e3;

    for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
        if( BaccComponents[i]->GetName().find(componentName)<G4String::npos )
            BaccComponents[i]->SetMass( cMass );
    
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetComponentVolume()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccManager::SetComponentVolume( G4String parameters )
{	
    G4String componentName = parameters.substr( 0, parameters.find(" ") );
    parameters = parameters.substr( parameters.find(" ") + 1);
    G4String volumeString = parameters.substr( 0, parameters.find(" ") );
    parameters = parameters.substr( parameters.find(" ") + 1);
    G4String unitString = parameters.substr( 0, parameters.find(" ") );
    
    
    G4double volume = atof( volumeString.c_str() );
    
    volume *= cm3;
    if( unitString == "mm3" ) volume *= 1e-3;
    if( unitString == "m3" ) volume *= 1e6;
    
     for( G4int i=0; i<(G4int)BaccComponents.size(); i++ )
        if( BaccComponents[i]->GetName().find(componentName)<G4String::npos )
            BaccComponents[i]->SetVolume( volume );

}
