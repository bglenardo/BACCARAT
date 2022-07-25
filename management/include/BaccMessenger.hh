////////////////////////////////////////////////////////////////////////////////
/*	BaccMessenger.hh

This is the header file to define the Baccarat messenger commands. The idea 
behind this class is that all commands (except geometry commands) get routed 
and stored in this management class, whether those commands are related to 
the physics list, output specification, or whatever. In this way, all parts 
of the simulation have access to all simulation settings without having to 
re-write any code.
********************************************************************************
Change log
  2013/09/26 - Initial submission (Vic)

  2014/10/05 - Added flag for checking overlaps for all volumes (automatic
               checking off by default) from LUXSim (Brian)
  2015/03/03 - Added component-wise SetMass and SetVolume commands (Brian, from LUXSim)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccMessenger_HH
#define BaccMessenger_HH 1

//
//	GEANT4 includes
//
#include "globals.hh"
#include "G4UImessenger.hh"

//
//	Class forwarding
//
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
//class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADoubleAndUnit;
//class G4UIcmdWith3Vector;
class G4UIcmdWithADouble;

class BaccManager;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccMessenger : public G4UImessenger
{
	public:
		BaccMessenger( BaccManager* );
		~BaccMessenger();
		
		void SetNewValue( G4UIcommand*, G4String );
	
	private:
		BaccManager *baccManager;
	
		//	General commands
		G4UIdirectory				*BaccDir;
		G4UIcmdWithAnInteger		*BaccBeamOnCommand;
		G4UIcmdWithAnInteger		*BaccRandomSeedCommand;
		G4UIcmdWithABool		*BaccInputEventSeedCommand;
		G4UIcmdWithAnInteger		*BaccEventSeedIndexCommand;
		G4UIcmdWithAnInteger		*BaccEventSeed1Command;
		G4UIcmdWithAnInteger		*BaccEventSeed2Command;
		
		//	Input/output commands
		G4UIdirectory				*BaccFileDir;
		G4UIcmdWithAString			*BaccOutputDirCommand;
		G4UIcmdWithAString			*BaccOutputNameCommand;
		G4UIcmdWithABool			*BaccAlwaysRecordPrimaryCommand;
        G4UIcmdWithAnInteger        *BaccEventProgressCommand;

        // User defined variables commands.
		G4UIcmdWithADouble			*BaccUserVar1Command;
		G4UIcmdWithADouble			*BaccUserVar2Command;
		G4UIcmdWithADouble			*BaccUserVar3Command;
		G4UIcmdWithADouble			*BaccUserVar4Command;
		G4UIcmdWithADouble			*BaccUserVar5Command;

                G4UIcmdWithABool                        *BaccVolumeOverlapCheckCommand;
                G4UIcmdWithAString                      *BaccComponentSetMassCommand;
                G4UIcmdWithAString                      *BaccComponentSetVolumeCommand;
	
		//	Source commands
		G4UIdirectory				*BaccSourceDir;
		G4UIcmdWithAString			*BaccSourceSetCommand;
		G4UIcmdWithoutParameter		*BaccSourceResetCommand;
		G4UIcmdWithABool         	*BaccSourcePrintCommand;
		
		//	Physics list commands
		G4UIdirectory				*BaccPhysicsListDir;
		G4UIcmdWithABool			*BaccOpticalPhotonsCommand;
		G4UIcmdWithABool			*BaccOpticalDebugCommand;
        G4UIcmdWithADouble          *BaccS1GainCommand;
        G4UIcmdWithADouble          *BaccS2GainCommand;
		G4UIcmdWithADoubleAndUnit	*BaccDriftingElectronAttenuationCommand;
};

#endif
