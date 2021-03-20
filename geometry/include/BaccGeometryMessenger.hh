////////////////////////////////////////////////////////////////////////////////
/*	BaccGeometryMessenger.hh

This is the header file to define the Baccarat messenger commands for the 
geometry. The idea here is that all geometry commands to be routed through and 
stored in this management class, instead of the more general BaccMessenter 
class.  That way, users of this code don't have to edit any code in the 
management directory to add and configure the detector they just finished adding 
into the geometry directory.
********************************************************************************
Change log
  2013/09/26 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccGeometryMessenger_HH
#define BaccGeometryMessenger_HH 1

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
class BaccDetectorConstruction;
class BaccManager;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccGeometryMessenger : public G4UImessenger
{
	public:
		BaccGeometryMessenger( BaccManager*, BaccDetectorConstruction* );
		~BaccGeometryMessenger();
		
		void SetNewValue( G4UIcommand*, G4String );
	
	private:
		BaccManager *baccManager;
		BaccDetectorConstruction* baccDetCon;
	
		//	Geometry commands
		G4UIdirectory				*BaccDetectorDir;
		G4UIcmdWithoutParameter		*BaccUpdateCommand;
		G4UIcmdWithAString	       	*BaccDetectorSelectCommand;
        G4UIcmdWithABool            *BaccVolumeOverlapCheckCommand;
		G4UIcmdWithADoubleAndUnit	*BaccDetectorZCommand;
        G4UIcmdWithAString          *BaccComponentSetMassCommand;
        G4UIcmdWithAString          *BaccComponentSetVolumeCommand;
		G4UIcmdWithAString			*BaccRecordLevelCommand;
		G4UIcmdWithAString			*BaccRecordLevelOptPhotCommand;
		G4UIcmdWithAString			*BaccRecordLevelThermElecCommand;
		G4UIcmdWithABool	       	*BaccGNARRLIPMTCommand;

		//	Materials commands
		G4UIdirectory				*BaccMaterialsDir;	
		G4UIcmdWithADouble			*BaccLXeTeflonReflCommand;
        G4UIcmdWithADouble          *BaccLXeTeflonReflLinerCommand;
        G4UIcmdWithADouble			*BaccLXeTeflonReflPMTCommand;
        G4UIcmdWithADouble			*BaccLXeTeflonReflPlateCommand;
        G4UIcmdWithADouble			*BaccLXeTeflonReflCablesCommand;
        G4UIcmdWithADouble			*BaccLXeTeflonReflTrussCommand;
        G4UIcmdWithADouble			*BaccLXeTeflonReflBskinPMTCommand;
        G4UIcmdWithADouble			*BaccLXeTeflonReflBplateCommand;
        G4UIcmdWithADouble			*BaccLXeTeflonReflTskinPMTCommand;
        G4UIcmdWithADouble			*BaccLXeTeflonReflWeirCommand;
        G4UIcmdWithADouble			*BaccLXeTeflonReflDomePMTCommand;
		G4UIcmdWithADouble			*BaccLXeSteelReflCommand;
		G4UIcmdWithADoubleAndUnit	*BaccLXeAbsCommand;
		G4UIcmdWithADoubleAndUnit	*BaccLXeRayleighCommand;
		G4UIcmdWithADouble			*BaccGXeTeflonReflCommand;
		G4UIcmdWithADouble			*BaccGXeSteelReflCommand;
		G4UIcmdWithADoubleAndUnit	*BaccGXeAbsCommand;
		G4UIcmdWithADoubleAndUnit	*BaccGXeRayleighCommand;
        G4UIcmdWithADoubleAndUnit   *BaccQuartzAbsCommand;
        G4UIcmdWithADouble			*BaccAlUnoxQuartzReflCommand;
};

#endif
