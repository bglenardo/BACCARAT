////////////////////////////////////////////////////////////////////////////////
/*	BACCARAT.cc

This is the main code file for Baccarat.
********************************************************************************
* Change log
  2013/09/26 - Initial submission (Vic)
  2014/01/04 - Code cleanup (Vic)
  2014/01/22 - Cosmetic shenannigans (Vic)
  2014/09/08 - Added code to check whether this checkout uses git or svn (Brian)
*/
////////////////////////////////////////////////////////////////////////////////

//
//	GEANT4 includes
//
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4Version.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//
//	Bacc includes
//
#include "BaccMaterials.hh"
#include "BaccPhysicsList.hh"
#include "BaccDetectorConstruction.hh"
#include "BaccPrimaryGeneratorAction.hh"
#include "BaccRunAction.hh"
#include "BaccEventAction.hh"
#include "BaccSteppingAction.hh"
#include "BaccStackingAction.hh"
#include "BaccOutput.hh"
#include "BaccSourceCatalog.hh"
#include "BaccGeometryCatalog.hh"
#include "BaccManager.hh"

//
//	C++ includes
//
#include <cstdlib>
#include <cstdio>
#include <iostream>

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					main()
//------++++++------++++++------++++++------++++++------++++++------++++++------
int main( int argc, char **argv )
{
	
	if( argc > 2 ) {
		G4cout << G4endl << G4endl << G4endl;
		G4cout << "You have supplied " << argc-1 << " arguments, but Bacc "
			   << "can only handle one." << G4endl;
		G4cout << G4endl << G4endl << G4endl;
		
		exit(0);
	}
    
    if( G4VERSION_NUMBER < 1020 ) {
        stringstream version;
        version << G4VERSION_NUMBER;
        G4String majorVersion = version.str().substr(0,1);
        G4String minorVersion = version.str().substr(1,1);
        G4String patchLevel = version.str().substr(2,1);

        G4cout << G4endl << G4endl << G4endl;
        G4cout << "You are running Geant4." << majorVersion << "."
        << minorVersion << ".p0" << patchLevel << G4endl;
        G4cout << "BACCARAT no longer operates with versions of Geant4 prior to "
        << "4.9.4.p00" << G4endl;
        G4cout << G4endl << G4endl << G4endl;

//        exit(0);
    }

    // Detect interactive mode (if no arguments) and define UI session
#ifdef G4UI_USE
    G4UIExecutive* ui_executive = 0;
    if( argc == 1 )
        ui_executive = new G4UIExecutive(argc, argv);
#endif

	//	Construct the default run manager
	G4RunManager* runManager = new G4RunManager;

	//	Instantiate and create all the required classes
	BaccManager *baccManager;
    baccManager = new BaccManager();
	
	//	For this next bit of code, COMPDIR is set at compilation time using the
	//	compilation switch -DCOMPDIR=\"`pwd`\". It is therefore a hard-coded
	//	string of where the executable was compiled. This is relevant for
	//	generating the SVN diffs if you're running the executable anywhere other
	//	than the top-level Bacc directory.
	G4String compDir = COMPDIR;
	if( compDir.substr(compDir.length()-1,1) == "/" )
		compDir = compDir.substr( 0, compDir.length()-1 );
	compDir = compDir.substr( 0, compDir.find_last_of( "/" ) );
	baccManager->SetCompilationDirectory( compDir );

    char repoTestCommand[100];
    sprintf(repoTestCommand, "ls -a %s/../ | grep svn", COMPDIR);
    baccManager->SetIsSVNRepo( (bool) !bool( system( repoTestCommand ) ) );
    sprintf(repoTestCommand, "ls -a %s/../ | grep git", COMPDIR);
    baccManager->SetIsGitRepo( (bool) !bool( system( repoTestCommand ) ) );

//        std::cerr << "\n\n\nConstructing BaccMaterials..." << std::endl;
	BaccMaterials *baccMaterials;
    baccMaterials = new BaccMaterials();

        //runManager->SetVerboseLevel(2); 
	
//        std::cerr << "\n\n\nConstructing BaccPhysicsList..." << std::endl;
	BaccPhysicsList *BaccPhysics = new BaccPhysicsList();
	runManager->SetUserInitialization( BaccPhysics );

//        std::cerr << "\n\n\nConstructing BaccGeometryCatalog..." << std::endl;
	BaccGeometryCatalog *BaccGeometryCat;
    BaccGeometryCat = new BaccGeometryCatalog();
	
//        std::cerr << "\n\n\nConstructing BaccDetector..." << std::endl;
	BaccDetectorConstruction *BaccDetector =
			new BaccDetectorConstruction();
	runManager->SetUserInitialization( BaccDetector );

//        std::cerr << "\n\n\nConstructing BaccGenerator..." << std::endl;
	BaccPrimaryGeneratorAction *BaccGenerator =
			new BaccPrimaryGeneratorAction();
	runManager->SetUserAction( BaccGenerator );

//        std::cerr << "\n\n\nConstructing BaccRunAction..." << std::endl;
	BaccRunAction *BaccRun = new BaccRunAction();
	runManager->SetUserAction( BaccRun );

	BaccEventAction *BaccEvent = new BaccEventAction();
	runManager->SetUserAction( BaccEvent );

	BaccSteppingAction *BaccStep = new BaccSteppingAction();
	runManager->SetUserAction( BaccStep );

	BaccStackingAction *BaccStack = new BaccStackingAction();
	runManager->SetUserAction( BaccStack );
	
//        std::cerr << "\n\n\nConstructing BaccSourceCatalog..." << std::endl;
	BaccSourceCatalog *BaccSourceCat;
    BaccSourceCat = new BaccSourceCatalog();

	//	Set up the visualization
#ifdef G4VIS_USE
	G4VisManager *visManager = new G4VisExecutive;
	visManager->Initialize();
#endif

//         std::cerr << "\n\nInitializing run..." << std::endl;
	// Initialize G4 kernel
	runManager->Initialize();

//        std::cerr << "\n\nRun initialized..." << std::endl;	
	//	Get the user interface manager
	G4UImanager *UI = G4UImanager::GetUIpointer();

	// Define (G)UI terminal for interactive mode
	G4UIsession *ui_session = 0;
	if(!ui_executive){
#ifdef G4UI_USE_TCSH
	  ui_session = new G4UIterminal(new G4UItcsh);      
#else
	  ui_session = new G4UIterminal();
#endif
	}
	
	// Execute input macro, if specified
	if( argc > 1 ){
		G4String commandFile = argv[1];
		G4String command = "/control/execute " + commandFile;
		UI->ApplyCommand(command);
	
		//	Read in the commands file, and if the last command is "exit", don't
		//	start the command session.
		ifstream commands( commandFile.c_str() );
		commands.seekg( 0, ios::end );
		int length = commands.tellg();
		commands.seekg( 0, ios::beg );
		char *inputBuffer;
		inputBuffer = new char[length+1];
		commands.read( inputBuffer, length );
		inputBuffer[length] = '\0';
		commands.close();
		G4String commandList = inputBuffer;
		delete [] inputBuffer;
		while( commandList.substr( commandList.length()-1 ) == "\n" ||
			   commandList.substr( commandList.length()-1 ) == "\t" ||
			   commandList.substr( commandList.length()-1 ) == " " )
			commandList = commandList.substr( 0, commandList.length()-1 );
		if( commandList.find_last_of("\n") < G4String::npos )
			commandList = commandList.substr( commandList.find_last_of("\n") );
		while( commandList.substr( 0,1 ) == "\n" ||
			   commandList.substr( 0,1 ) == "\t" ||
			   commandList.substr( 0,1 ) == " " )
			commandList = commandList.substr( 1 );
		if( commandList != "exit" ) {
			ui_session->SessionStart();
			delete ui_session;
		}
	}

	//if we want to run BACCARAT in the interative mode
	else if(ui_executive){//if we use ui executive
#ifdef G4UI_USE
#ifdef G4VIS_USE
	  UI->ApplyCommand("/control/execute BaccMacros/qt_init_vis.mac");
#endif
	  if (ui_executive->IsGUI()) {
	    UI->ApplyCommand("/control/execute BaccMacros/qt_gui.mac");
	  }
	  // start interactive session
	  ui_executive->SessionStart();
	  delete ui_executive;
#endif
	}//end if ui executive
	else if(ui_session){//if we just want to use a simple UI
	  ui_session->SessionStart();
 	  delete ui_session;
 	}


	//	Clean up
#ifdef G4VIS_USE
	delete visManager;
#endif
	
	delete runManager;
	delete baccManager;

	return 0;
}
