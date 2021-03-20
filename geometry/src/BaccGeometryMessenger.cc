////////////////////////////////////////////////////////////////////////////////
/*	BaccGeometryMessenger.cc

This is the source file to define the Baccarat messenger commands for the 
geometry. The idea here is that all geometry commands to be routed through and 
stored in this management class, instead of the more general BaccMessenter 
class.  That way, users of this code don't have to edit any code in the 
management directory to add and configure the detector they just finished adding
into the geometry directory.
********************************************************************************
Change log
  2013/09/26 - Initial submission (Vic)
  2016/02/02 - Added VolumeOverlap, SetMass, SetVolume, QuartzAbs (Shaun)
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
//#include "BaccMessenger.hh"
#include "BaccManager.hh"
#include "BaccGeometryMessenger.hh"
#include "BaccDetectorConstruction.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccGeometryMessenger()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeometryMessenger::BaccGeometryMessenger( BaccManager *man, BaccDetectorConstruction *bdc )
: baccManager( man ), baccDetCon( bdc )
{
	//	Geometry commands
    BaccVolumeOverlapCheckCommand = new G4UIcmdWithABool( "/Bacc/detector/checkForOverlaps", this );
    BaccVolumeOverlapCheckCommand->SetGuidance( "Forces a volume overlap check for all components" );
    BaccVolumeOverlapCheckCommand->SetGuidance( "Default value = false" );
    BaccVolumeOverlapCheckCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

    BaccComponentSetMassCommand = new G4UIcmdWithAString( "/Bacc/detector/setComponentMass", this );
    BaccComponentSetMassCommand->SetGuidance( "Sets the mass of a detector component." );
    BaccComponentSetMassCommand->SetGuidance( "Usage: /Bacc/detector/setComponentMass <component name> <mass> <units>" );
    BaccComponentSetMassCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

    BaccComponentSetVolumeCommand = new G4UIcmdWithAString( "/Bacc/detector/setComponentVolume", this );
    BaccComponentSetVolumeCommand->SetGuidance( "Sets the mass of a detector component." );
    BaccComponentSetVolumeCommand->SetGuidance( "Usage: /Bacc/detector/setComponentVolume <component name> <volume> <units>" );
    BaccComponentSetVolumeCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

	BaccDetectorDir = new G4UIdirectory( "/Bacc/detector/" );
	BaccDetectorDir->SetGuidance( "Commands to control the Bacc geometry" );
	
	BaccUpdateCommand = new G4UIcmdWithoutParameter( "/Bacc/detector/update", this );
	BaccUpdateCommand->SetGuidance( "Update Bacc detector - used if the detector has been altered on-the-fly (i.e.," );
	BaccUpdateCommand->SetGuidance( "between runs, but without re-executing the simulation" );
	BaccUpdateCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
	
	BaccDetectorSelectCommand = new G4UIcmdWithAString( "/Bacc/detector/select", this );
	BaccDetectorSelectCommand->SetGuidance( "Selects the detector by name." );
	BaccDetectorSelectCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

	BaccDetectorZCommand = new G4UIcmdWithADoubleAndUnit( "/Bacc/detector/detectorZ", this );
	BaccDetectorZCommand->SetGuidance( "Sets the absolute Z position of any detector" );
	BaccDetectorZCommand->SetGuidance( "relative to water shield or universe center" );
	BaccDetectorZCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
	
	BaccRecordLevelCommand = new G4UIcmdWithAString( "/Bacc/detector/recordLevel", this );
	BaccRecordLevelCommand->SetGuidance( "Sets the record level of a volume according to the volume name." );
	BaccRecordLevelCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

	BaccRecordLevelOptPhotCommand = new G4UIcmdWithAString( "/Bacc/detector/recordLevelOptPhot", this );
	BaccRecordLevelOptPhotCommand->SetGuidance( "Sets the optical photon record level of a volume according to the volume name." );
	BaccRecordLevelOptPhotCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
	
	BaccRecordLevelThermElecCommand = new G4UIcmdWithAString( "/Bacc/detector/recordLevelThermElec", this );
	BaccRecordLevelThermElecCommand->SetGuidance( "Sets the thermal electron record level of a volume according to the volume name." );
	BaccRecordLevelThermElecCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
	
	BaccGNARRLIPMTCommand = new G4UIcmdWithABool( "/Bacc/detector/fourPMTs", this );
	BaccGNARRLIPMTCommand->SetGuidance( "Selects 1 or 4 PMT geometry of GNARRLI detector." );
	BaccGNARRLIPMTCommand->SetGuidance( "true for 4 PMT geometry, false for 1 PMT geometry (default)" );
	BaccGNARRLIPMTCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
	
	//	Materials commands
	BaccMaterialsDir = new G4UIdirectory( "/Bacc/materials/" );
	BaccMaterialsDir->SetGuidance( "Commands to control material properties" );
	
	BaccLXeTeflonReflCommand = new G4UIcmdWithADouble( "/Bacc/materials/LXeTeflonRefl", this );
	BaccLXeTeflonReflCommand->SetGuidance( "Sets LXe/Teflon reflectivity (value between 0 and 1)" );
	BaccLXeTeflonReflCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
	
	BaccLXeSteelReflCommand = new G4UIcmdWithADouble( "/Bacc/materials/LXeSteelRefl", this );
	BaccLXeSteelReflCommand->SetGuidance( "Sets LXe/steel (i.e. grid) reflectivity (value between 0 and 1)" );
	BaccLXeSteelReflCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
	
	BaccLXeAbsCommand = new G4UIcmdWithADoubleAndUnit( "/Bacc/materials/LXeAbsorption", this );
	BaccLXeAbsCommand->SetGuidance( "Sets the LXe photon absorption length" );
	BaccLXeAbsCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
	
	BaccLXeRayleighCommand = new G4UIcmdWithADoubleAndUnit( "/Bacc/materials/LXeRayleigh", this );
	BaccLXeRayleighCommand->SetGuidance( "Sets the LXe Rayleigh scattering length" );
	BaccLXeRayleighCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
	
	BaccGXeTeflonReflCommand = new G4UIcmdWithADouble( "/Bacc/materials/GXeTeflonRefl", this );
	BaccGXeTeflonReflCommand->SetGuidance( "Sets GXe/Teflon reflectivity (value between 0 and 1)" );
	BaccGXeTeflonReflCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
	
	BaccGXeSteelReflCommand = new G4UIcmdWithADouble( "/Bacc/materials/GXeSteelRefl", this );
	BaccGXeSteelReflCommand->SetGuidance( "Sets GXe/steel (i.e. grid) reflectivity (value between 0 and 1)" );
	BaccGXeSteelReflCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
	
	BaccGXeAbsCommand = new G4UIcmdWithADoubleAndUnit( "/Bacc/materials/GXeAbsorption", this );
	BaccGXeAbsCommand->SetGuidance( "Sets the GXe photon absorption length" );
	BaccGXeAbsCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
	
	BaccGXeRayleighCommand = new G4UIcmdWithADoubleAndUnit( "/Bacc/materials/GXeRayleigh", this );
	BaccGXeRayleighCommand->SetGuidance( "Sets the GXe Rayleigh scattering length" );
	BaccGXeRayleighCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
	
	BaccAlUnoxQuartzReflCommand = new G4UIcmdWithADouble( "/Bacc/materials/AlUnoxidizedQuartzRefl", this );
	BaccAlUnoxQuartzReflCommand->SetGuidance( "Sets the unoxidized Al / quartz reflectivity (value between 0 and 1)" );
	BaccAlUnoxQuartzReflCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

    BaccQuartzAbsCommand = new G4UIcmdWithADoubleAndUnit( "/Bacc/materials/QuartzAbsorption", this );
    BaccQuartzAbsCommand->SetGuidance( "Sets the quartz photon absorption length" );
    BaccQuartzAbsCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
    
    BaccLXeTeflonReflLinerCommand = new G4UIcmdWithADouble( "/Bacc/materials/LXeTeflonReflLiner", this );
    BaccLXeTeflonReflLinerCommand->SetGuidance( "Sets LXe/Teflon reflectivity (value between 0 and 1)" );
    BaccLXeTeflonReflLinerCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
    
    BaccLXeTeflonReflPMTCommand = new G4UIcmdWithADouble( "/Bacc/materials/LXeTeflonReflPMT", this );
    BaccLXeTeflonReflPMTCommand->SetGuidance( "Sets LXe/Teflon reflectivity (value between 0 and 1)" );
    BaccLXeTeflonReflPMTCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
    
    BaccLXeTeflonReflPlateCommand = new G4UIcmdWithADouble( "/Bacc/materials/LXeTeflonReflPlate", this );
    BaccLXeTeflonReflPlateCommand->SetGuidance( "Sets LXe/Teflon reflectivity (value between 0 and 1)" );
    BaccLXeTeflonReflPlateCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
    
    BaccLXeTeflonReflCablesCommand = new G4UIcmdWithADouble( "/Bacc/materials/LXeTeflonReflCables", this );
    BaccLXeTeflonReflCablesCommand->SetGuidance( "Sets LXe/Teflon reflectivity (value between 0 and 1)" );
    BaccLXeTeflonReflCablesCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
    
    BaccLXeTeflonReflTrussCommand = new G4UIcmdWithADouble( "/Bacc/materials/LXeTeflonReflTruss", this );
    BaccLXeTeflonReflTrussCommand->SetGuidance( "Sets LXe/Teflon reflectivity (value between 0 and 1)" );
    BaccLXeTeflonReflTrussCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
    
    BaccLXeTeflonReflBskinPMTCommand = new G4UIcmdWithADouble( "/Bacc/materials/LXeTeflonReflBskinPMT", this );
    BaccLXeTeflonReflBskinPMTCommand->SetGuidance( "Sets LXe/Teflon reflectivity (value between 0 and 1)" );
    BaccLXeTeflonReflBskinPMTCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
    
    BaccLXeTeflonReflBplateCommand = new G4UIcmdWithADouble( "/Bacc/materials/LXeTeflonReflBplate", this );
    BaccLXeTeflonReflBplateCommand->SetGuidance( "Sets LXe/Teflon reflectivity (value between 0 and 1)" );
    BaccLXeTeflonReflBplateCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
    
    BaccLXeTeflonReflTskinPMTCommand = new G4UIcmdWithADouble( "/Bacc/materials/LXeTeflonReflTskinPMT", this );
    BaccLXeTeflonReflTskinPMTCommand->SetGuidance( "Sets LXe/Teflon reflectivity (value between 0 and 1)" );
    BaccLXeTeflonReflTskinPMTCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
    
    BaccLXeTeflonReflWeirCommand = new G4UIcmdWithADouble( "/Bacc/materials/LXeTeflonReflWeir", this );
    BaccLXeTeflonReflWeirCommand->SetGuidance( "Sets LXe/Teflon reflectivity (value between 0 and 1)" );
    BaccLXeTeflonReflWeirCommand->AvailableForStates( G4State_PreInit, G4State_Idle );
    
    BaccLXeTeflonReflDomePMTCommand = new G4UIcmdWithADouble( "/Bacc/materials/LXeTeflonReflDomePMT", this );
    BaccLXeTeflonReflDomePMTCommand->SetGuidance( "Sets LXe/Teflon reflectivity (value between 0 and 1)" );
    BaccLXeTeflonReflDomePMTCommand->AvailableForStates( G4State_PreInit, G4State_Idle );

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccGeometryMessenger()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeometryMessenger::~BaccGeometryMessenger()
{
	//	Geometry commands
    delete BaccVolumeOverlapCheckCommand;
    delete BaccComponentSetMassCommand;
    delete BaccComponentSetVolumeCommand;
	delete BaccDetectorDir;
	delete BaccUpdateCommand;
	delete BaccDetectorSelectCommand;
	delete BaccDetectorZCommand;
	delete BaccRecordLevelCommand;
	delete BaccRecordLevelOptPhotCommand;
	delete BaccRecordLevelThermElecCommand;
    delete BaccGNARRLIPMTCommand;
    
	//	Materials commands
    delete BaccQuartzAbsCommand;
	delete BaccMaterialsDir;	
	delete BaccLXeTeflonReflCommand;
    delete BaccLXeTeflonReflLinerCommand;
    delete BaccLXeTeflonReflPMTCommand;
    delete BaccLXeTeflonReflPlateCommand;
    delete BaccLXeTeflonReflCablesCommand;
    delete BaccLXeTeflonReflTrussCommand;
    delete BaccLXeTeflonReflBskinPMTCommand;
    delete BaccLXeTeflonReflBplateCommand;
    delete BaccLXeTeflonReflTskinPMTCommand;
    delete BaccLXeTeflonReflWeirCommand;
    delete BaccLXeTeflonReflDomePMTCommand;
	delete BaccLXeSteelReflCommand;
	delete BaccLXeAbsCommand;
	delete BaccLXeRayleighCommand;
	delete BaccGXeTeflonReflCommand;
	delete BaccGXeSteelReflCommand;
	delete BaccGXeAbsCommand;
	delete BaccGXeRayleighCommand;
	delete BaccAlUnoxQuartzReflCommand;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetNewValue()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeometryMessenger::SetNewValue( G4UIcommand *command, G4String newValue ){
	//	Geometry commands
	if( command == BaccUpdateCommand )
		baccManager->UpdateGeometry();
	
	else if( command == BaccDetectorSelectCommand )
		baccDetCon->SetDetectorSelection( newValue );

	else if( command == BaccVolumeOverlapCheckCommand )
	  baccDetCon->SetCheckVolumeOverlaps( BaccVolumeOverlapCheckCommand->GetNewBoolValue(newValue) );

	else if( command == BaccComponentSetMassCommand )
	  baccManager->SetComponentMass( newValue );

	else if( command == BaccComponentSetVolumeCommand )
	  baccManager->SetComponentVolume( newValue );
	
	else if( command == BaccDetectorZCommand )
		baccDetCon->SetDetectorZ( G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( newValue.data() ) );
	
	else if( command == BaccRecordLevelCommand )
		baccManager->SetRecordLevel( newValue );
	
	else if( command == BaccRecordLevelOptPhotCommand )
		baccManager->SetRecordLevelOptPhot( newValue );
	
	else if( command == BaccRecordLevelThermElecCommand )
		baccManager->SetRecordLevelThermElec( newValue );
	
	//	Materials commands
    else if ( command == BaccQuartzAbsCommand )
        baccDetCon->SetQuartzAbs( G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( newValue.data() ) );

	else if ( command == BaccLXeTeflonReflCommand )
		baccDetCon->SetLXeTeflonRefl( G4UIcmdWithADouble::GetNewDoubleValue( newValue.data() ) );
	
    else if ( command == BaccLXeTeflonReflLinerCommand )
        baccDetCon->SetLXeTeflonLinerRefl( G4UIcmdWithADouble::GetNewDoubleValue( newValue.data() ) );
    
    else if ( command == BaccLXeTeflonReflPMTCommand )
        baccDetCon->SetLXeTeflonPMTRefl( G4UIcmdWithADouble::GetNewDoubleValue( newValue.data() ) );
    
    else if ( command == BaccLXeTeflonReflPlateCommand )
        baccDetCon->SetLXeTeflonPlateRefl( G4UIcmdWithADouble::GetNewDoubleValue( newValue.data() ) );
    
    else if ( command == BaccLXeTeflonReflCablesCommand )
        baccDetCon->SetLXeTeflonCablesRefl( G4UIcmdWithADouble::GetNewDoubleValue( newValue.data() ) );
    
    else if ( command == BaccLXeTeflonReflTrussCommand )
        baccDetCon->SetLXeTeflonTrussRefl( G4UIcmdWithADouble::GetNewDoubleValue( newValue.data() ) );
    
    else if ( command == BaccLXeTeflonReflBskinPMTCommand )
        baccDetCon->SetLXeTeflonBskinPMTRefl( G4UIcmdWithADouble::GetNewDoubleValue( newValue.data() ) );
    
    else if ( command == BaccLXeTeflonReflBplateCommand )
        baccDetCon->SetLXeTeflonBplateRefl( G4UIcmdWithADouble::GetNewDoubleValue( newValue.data() ) );
    
    else if ( command == BaccLXeTeflonReflTskinPMTCommand )
        baccDetCon->SetLXeTeflonTskinPMTRefl( G4UIcmdWithADouble::GetNewDoubleValue( newValue.data() ) );
    
    else if ( command == BaccLXeTeflonReflWeirCommand )
        baccDetCon->SetLXeTeflonWeirRefl( G4UIcmdWithADouble::GetNewDoubleValue( newValue.data() ) );
    
    else if ( command == BaccLXeTeflonReflDomePMTCommand )
        baccDetCon->SetLXeTeflonDomePMTRefl( G4UIcmdWithADouble::GetNewDoubleValue( newValue.data() ) );
    
	else if ( command == BaccLXeSteelReflCommand )
		baccDetCon->SetLXeSteelRefl( G4UIcmdWithADouble::GetNewDoubleValue( newValue.data() ) );
	
	else if ( command == BaccLXeAbsCommand )
		baccDetCon->SetLXeAbs( G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( newValue.data() ) );
	
	else if ( command == BaccLXeRayleighCommand )
		baccDetCon->SetLXeRayleigh( G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( newValue.data() ) );
	
	else if ( command == BaccGXeTeflonReflCommand )
		baccDetCon->SetGXeTeflonRefl( G4UIcmdWithADouble::GetNewDoubleValue( newValue.data() ) );
	
	else if ( command == BaccGXeSteelReflCommand )
		baccDetCon->SetGXeSteelRefl( G4UIcmdWithADouble::GetNewDoubleValue( newValue.data() ) );
	
	else if ( command == BaccGXeAbsCommand )
		baccDetCon->SetGXeAbs( G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( newValue.data() ) );
		
	else if ( command == BaccGXeRayleighCommand )
		baccDetCon->SetGXeRayleigh( G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( newValue.data() ) );
		
	else if ( command == BaccAlUnoxQuartzReflCommand )
		baccDetCon->SetAlUnoxQuartzRefl( G4UIcmdWithADouble::GetNewDoubleValue( newValue.data() ) );	
}
