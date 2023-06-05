////////////////////////////////////////////////////////////////////////////////
/*	BaccMessenger.cc

This is the code file to define the Bacc messenger commands. The idea behind 
this class is that  all commands (except geometry commands) get routed and 
stored in this management class, whether those commands are related to the 
physics list, output specification, or whatever. In this way, all parts of the 
simulation have access to all simulation settings without having to re-write any 
code.
********************************************************************************
Change log
  2013/09/26 - Initial submission (Vic)
  2014/10/05 - Added flag for checking overlaps for all volumes (automatic
               checking off by default) from LUXSim (Brian)
  2015/03/03 - Added component-wise SetMass and SetVolume commands (Brian, from LUXSim)
  2015/07/07 - Fixed a couple of legacy LUXSim references.  (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

//
//	GEANT4 includes
//
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
//#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
//#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithADouble.hh"
#include "globals.hh"

//
//	Bacc includes
//
#include "BaccMessenger.hh"
#include "BaccManager.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccMessenger()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccMessenger::BaccMessenger( BaccManager *man )
: baccManager( man )
{
	//	General commands
	BaccDir = new G4UIdirectory( "/Bacc/" );
	BaccDir->SetGuidance( "Commands specific to BACCARAT" );
	
	BaccBeamOnCommand = new G4UIcmdWithAnInteger( "/Bacc/beamOn", this );
	BaccBeamOnCommand->SetGuidance( "This command should be used rather than the stanard \"/run/beamOn\" command." );
	BaccBeamOnCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
	
	BaccRandomSeedCommand = new G4UIcmdWithAnInteger( "/Bacc/randomSeed", this );
	BaccRandomSeedCommand->SetGuidance( "Use this command to explicitly set the randomization seed used in the" );
	BaccRandomSeedCommand->SetGuidance( "simulation. By default, the randomization seed is itself random, so this" );
	BaccRandomSeedCommand->SetGuidance( "command is used for reproducing earlier data (e.g., for debugging)." );
	BaccRandomSeedCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

	BaccInputEventSeedCommand = new G4UIcmdWithABool( "/Bacc/inputEventRandomSeed", this);
	BaccInputEventSeedCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

	BaccEventSeedIndexCommand = new G4UIcmdWithAnInteger( "/Bacc/eventSeedIndex", this);
	BaccEventSeedIndexCommand->SetGuidance( "Use this command to set the event-level random seed");
	BaccEventSeedIndexCommand->SetGuidance( " Works only when using the RanecuEngine.");
	BaccEventSeedIndexCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

	BaccEventSeed1Command = new G4UIcmdWithADouble( "/Bacc/eventSeed1", this);
	BaccEventSeed1Command->SetGuidance( "Use this command to set the event-level random seed");
	BaccEventSeed1Command->SetGuidance( " Works only when using the RanecuEngine.");
	BaccEventSeed1Command->AvailableForStates( G4State_PreInit, G4State_Idle );

	BaccEventSeed2Command = new G4UIcmdWithADouble( "/Bacc/eventSeed2", this);
	BaccEventSeed2Command->SetGuidance( "Use this command to set the event-level random seed");
	BaccEventSeed2Command->SetGuidance( " Works only when using the RanecuEngine.");
	BaccEventSeed2Command->AvailableForStates( G4State_PreInit, G4State_Idle );

	//	Input/output commands
	BaccFileDir = new G4UIdirectory( "/Bacc/io/" );
	BaccFileDir->SetGuidance( "Controls the Bacc input and output files" );
	
	BaccOutputDirCommand = new G4UIcmdWithAString( "/Bacc/io/outputDir", this );
	BaccOutputDirCommand->SetGuidance( "Use this to set the name of the output directory. By default, this is set to the" );
	BaccOutputDirCommand->SetGuidance( "directory in which the executable is launched. If the directory does not end" );
	BaccOutputDirCommand->SetGuidance( "with a \"/\", one will be added." );

	BaccOutputNameCommand = new G4UIcmdWithAString( "/Bacc/io/outputName", this );
	BaccOutputNameCommand->SetGuidance( "This command sets the base name of the output file. The randomization seed is" );
	BaccOutputNameCommand->SetGuidance( "still incorporated into the file name so that files don't get overwritten." );
	
	BaccAlwaysRecordPrimaryCommand = new G4UIcmdWithABool( "/Bacc/io/alwaysRecordPrimary", this );
	BaccAlwaysRecordPrimaryCommand->SetGuidance( "Setting this command to true will record the primary particle information, even" );
	BaccAlwaysRecordPrimaryCommand->SetGuidance( "if no other hit or energy information is recorded. The default value is true." );

    BaccEventProgressCommand = new G4UIcmdWithAnInteger( "/Bacc/io/updateFrequency", this );
    BaccEventProgressCommand->SetGuidance( "This controls after how many events a progress report is printed to screen that" );
    BaccEventProgressCommand->SetGuidance( "includes the current event number and the total number of seconds that have" );
    BaccEventProgressCommand->SetGuidance( "elapsed since the begininning of Event 1." );

    // Set mass and volume commands
        BaccComponentSetMassCommand = new G4UIcmdWithAString( "/Bacc/detector/setComponentMass", this );
	BaccComponentSetMassCommand->SetGuidance( "Sets the mass of a detector component." );
	BaccComponentSetMassCommand->SetGuidance( "Usage: /Bacc/detector/setComponentMass <component name> <mass> <units>" );
	BaccComponentSetMassCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

        BaccComponentSetVolumeCommand = new G4UIcmdWithAString( "/Bacc/detector/setComponentVolume", this );
	BaccComponentSetVolumeCommand->SetGuidance( "Sets the mass of a detector component." );
	BaccComponentSetVolumeCommand->SetGuidance( "Usage: /Bacc/detector/setComponentVolume <component name> <volume> <units>" );
	BaccComponentSetVolumeCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

    // User defined variables commands.
	BaccUserVar1Command = new G4UIcmdWithADouble( "/Bacc/io/userVar1", this);
	BaccUserVar1Command->SetGuidance("Set userVar1 in the manager class. Useful for passing arbitrary parameters from macros." );
	BaccUserVar2Command = new G4UIcmdWithADouble( "/Bacc/io/userVar2", this);
	BaccUserVar2Command->SetGuidance("Set userVar2 in the manager class. Useful for passing arbitrary parameters from macros." );
	BaccUserVar3Command = new G4UIcmdWithADouble( "/Bacc/io/userVar3", this);
	BaccUserVar3Command->SetGuidance("Set userVar3 in the manager class. Useful for passing arbitrary parameters from macros." );
	BaccUserVar4Command = new G4UIcmdWithADouble( "/Bacc/io/userVar4", this);
	BaccUserVar4Command->SetGuidance("Set userVar4 in the manager class. Useful for passing arbitrary parameters from macros." );
	BaccUserVar5Command = new G4UIcmdWithADouble( "/Bacc/io/userVar5", this);
	BaccUserVar5Command->SetGuidance("Set userVar5 in the manager class. Useful for passing arbitrary parameters from macros." );
	
	//	Source commands
	BaccSourceDir = new G4UIdirectory( "/Bacc/source/" );
	BaccSourceDir->SetGuidance( "Commands to control the Bacc sources" );
	
	BaccSourceSetCommand = new G4UIcmdWithAString( "/Bacc/source/set", this );
	BaccSourceSetCommand->SetGuidance( "(with String) Adds a radioactive load to a BaccDetectorComponent. This command works only" );
	BaccSourceSetCommand->SetGuidance( "with geometry volumes that have been defined at BaccDetectorComponents." );
	BaccSourceSetCommand->SetGuidance( "The format of the string is <volume name> <source type> <activity>. The source" );
	BaccSourceSetCommand->SetGuidance( "type can only be one of Th, U, K, AmBe, Cf, or spallation. The activity is given" );
	BaccSourceSetCommand->SetGuidance( "in units of nCi, uCi, mCi, Bq, and kBq. If no unit is specified, Bq is assumed." );
	BaccSourceSetCommand->SetGuidance( "Example: /Bacc/source/set Top_PMT_Window_2 AmBe 0.5 kBq" );
 	BaccSourceSetCommand->SetGuidance( "If using the GammaX generator, the fiducial volume radial and Z bounds, the reverse field" );
 	BaccSourceSetCommand->SetGuidance( "radial and Z bounds, and the maximum energy of the scatters must be specified." );
 	BaccSourceSetCommand->SetGuidance( "In that case the string format would become");
 	BaccSourceSetCommand->SetGuidance( "'LiquidXenon gamma-X <activity> <fiducial radius> <lower fiducial Z> <upper fiducial Z>");
	BaccSourceSetCommand->SetGuidance( "<reverse field region (rfr) radius> <rfr lower Z> <rfr upper Z> <units> <max Energy> <units>");
	BaccSourceSetCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
    //print decay Chain
	BaccSourcePrintCommand = new G4UIcmdWithABool( "/Bacc/source/print", this );
    BaccSourcePrintCommand->SetGuidance("(Boolean) Prints Decay Chain Binary Tree to standard output; lines starts with NodeDecayChain");
    BaccSourcePrintCommand->AvailableForStates( G4State_PreInit, G4State_Idle);
    //reset
    BaccSourceResetCommand = new G4UIcmdWithoutParameter( "/Bacc/source/reset", this );
	BaccSourceResetCommand->SetGuidance( "Clears all previously set sources" );
	BaccSourceResetCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

        //      Geometry commands
        BaccVolumeOverlapCheckCommand = new G4UIcmdWithABool( "/Bacc/detector/checkForOverlaps", this );
        BaccVolumeOverlapCheckCommand->SetGuidance( "Forces a volume overlap check for all components" );
        BaccVolumeOverlapCheckCommand->SetGuidance( "Default value = false" );
        BaccVolumeOverlapCheckCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

	//	Physics list commands
	BaccPhysicsListDir = new G4UIdirectory( "/Bacc/physicsList/" );
	BaccPhysicsListDir->SetGuidance( "Commands to control the physics list" );
	
	BaccOpticalPhotonsCommand = new G4UIcmdWithABool( "/Bacc/physicsList/useOpticalProcesses", this );
	BaccOpticalPhotonsCommand->SetGuidance( "Turns optical processes (Scintillation and Cerenkov) on and off" );
	BaccOpticalPhotonsCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

	BaccOpticalDebugCommand = new G4UIcmdWithABool( "/Bacc/physicsList/opticalDebug", this );
	BaccOpticalDebugCommand->SetGuidance( "Turns on simplified optical parameters for debugging purposes" );
	BaccOpticalDebugCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
    
    BaccS1GainCommand = new G4UIcmdWithADouble( "/Bacc/physicsList/s1gain", this );
 	BaccS1GainCommand->SetGuidance( "Sets the gain for S1 light generation" );
	BaccS1GainCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
	
    BaccS2GainCommand = new G4UIcmdWithADouble( "/Bacc/physicsList/s2gain", this );
 	BaccS2GainCommand->SetGuidance( "Sets the gain for S2 light generation" );
	BaccS2GainCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
	
    BaccDriftingElectronAttenuationCommand = new G4UIcmdWithADoubleAndUnit( "/Bacc/physicsList/driftElecAttenuation", this );
 	BaccDriftingElectronAttenuationCommand->SetGuidance( "Sets the attenuation length for drifting electrons" );
	BaccDriftingElectronAttenuationCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccMessenger()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccMessenger::~BaccMessenger()
{
	//	General commands
	delete BaccDir;
	delete BaccBeamOnCommand;
	delete BaccRandomSeedCommand;
	delete BaccInputEventSeedCommand;
	delete BaccEventSeedIndexCommand;
	delete BaccEventSeed1Command;
	delete BaccEventSeed2Command;

	//	Input/output commands
	delete BaccFileDir;
	delete BaccOutputDirCommand;
	delete BaccOutputNameCommand;
	delete BaccAlwaysRecordPrimaryCommand;
    delete BaccEventProgressCommand;

	// User defined variables commands.
	delete BaccUserVar1Command;
	delete BaccUserVar2Command;
	delete BaccUserVar3Command;
	delete BaccUserVar4Command;
	delete BaccUserVar5Command;

        // Set mass and volume commands.
        delete BaccComponentSetMassCommand;
        delete BaccComponentSetVolumeCommand;

	//	Source commands
	delete BaccSourceDir;
	delete BaccSourceSetCommand;
	delete BaccSourceResetCommand;
	delete BaccSourcePrintCommand;
        
        //     Geometry commands
        delete BaccVolumeOverlapCheckCommand;

	//	Physics list commands
	delete BaccPhysicsListDir;
	delete BaccOpticalPhotonsCommand;
	delete BaccOpticalDebugCommand;
    delete BaccS1GainCommand;
    delete BaccS2GainCommand;
	delete BaccDriftingElectronAttenuationCommand;
	
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetNewValue()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMessenger::SetNewValue( G4UIcommand *command, G4String newValue )
{
	//	General commands
	if( command == BaccBeamOnCommand )
		baccManager->BeamOn( BaccBeamOnCommand->GetNewIntValue(newValue) );

	//	Commands related to random seeding	
	else if( command == BaccRandomSeedCommand )
		baccManager->SetRandomSeed( BaccRandomSeedCommand->GetNewIntValue(newValue) );

	else if( command == BaccInputEventSeedCommand )
		baccManager->SetUseInputEventSeed( BaccInputEventSeedCommand->GetNewBoolValue(newValue) );	
	else if( command == BaccEventSeedIndexCommand )
		baccManager->SetEventSeedIndex( BaccEventSeedIndexCommand->GetNewIntValue(newValue) );
	else if( command == BaccEventSeed1Command )
		baccManager->SetEventSeed1( BaccEventSeed1Command->GetNewDoubleValue(newValue) );
	else if( command == BaccEventSeed2Command )
		baccManager->SetEventSeed2( BaccEventSeed2Command->GetNewDoubleValue(newValue) );
	
	//	Input/output commands
	else if( command == BaccOutputDirCommand )
		baccManager->SetOutputDir( newValue );
	else if( command == BaccOutputNameCommand )
		baccManager->SetOutputName( newValue );
	else if( command == BaccAlwaysRecordPrimaryCommand )
		baccManager->SetAlwaysRecordPrimary( BaccAlwaysRecordPrimaryCommand->GetNewBoolValue(newValue) );
	else if( command == BaccEventProgressCommand)
		baccManager->SetEventProgressFrequency( BaccEventProgressCommand->GetNewIntValue(newValue) );

	// User defined variables commands
	else if( command == BaccUserVar1Command )
		baccManager->SetUserVar1( BaccUserVar1Command->GetNewDoubleValue( newValue.data() ) );
	else if( command == BaccUserVar2Command )
		baccManager->SetUserVar2( BaccUserVar2Command->GetNewDoubleValue( newValue.data() ) );
	else if( command == BaccUserVar3Command )
		baccManager->SetUserVar3( BaccUserVar3Command->GetNewDoubleValue( newValue.data() ) );
	else if( command == BaccUserVar4Command )
		baccManager->SetUserVar4( BaccUserVar4Command->GetNewDoubleValue( newValue.data() ) );
	else if( command == BaccUserVar5Command )
		baccManager->SetUserVar5( BaccUserVar5Command->GetNewDoubleValue( newValue.data() ) );

        // Set Volume and mass
        else if( command == BaccComponentSetVolumeCommand )
            baccManager->SetComponentVolume( newValue );
        else if( command == BaccComponentSetMassCommand )
            baccManager->SetComponentMass( newValue );

	//	Source commands
	else if( command == BaccSourceSetCommand )
		baccManager->SetSource( newValue );
        
	else if( command == BaccSourceResetCommand )
		baccManager->ResetSources();

	else if( command == BaccSourcePrintCommand )
		baccManager->SetPrintEventList( newValue );

        //      Geometry commands
        else if( command == BaccVolumeOverlapCheckCommand )
        baccManager->SetCheckVolumeOverlaps( BaccVolumeOverlapCheckCommand->GetNewBoolValue(newValue) );


	//	Physics list commands
	else if( command == BaccOpticalPhotonsCommand )
		baccManager->SetUseOpticalProcesses( BaccOpticalPhotonsCommand->GetNewBoolValue(newValue) );

	else if( command == BaccOpticalDebugCommand )
		baccManager->SetOpticalDebugging( BaccOpticalDebugCommand->GetNewBoolValue(newValue) );
    
    else if( command == BaccS1GainCommand )
        baccManager->SetS1Gain( G4UIcmdWithADouble::GetNewDoubleValue(newValue.data()) );
	
    else if( command == BaccS2GainCommand )
        baccManager->SetS2Gain( G4UIcmdWithADouble::GetNewDoubleValue(newValue.data()) );
	
	else if( command == BaccDriftingElectronAttenuationCommand )
		baccManager->SetDriftElecAttenuation( G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( newValue.data() ) );
	
}
