////////////////////////////////////////////////////////////////////////////////
//
// This code is automatically generated by BACCARAT to define the messenger
// commands for the AmBeXe137Source project.
// All geometry, physics, io managements, etc, happen here
//
////////////////////////////////////////////////////////////////////////////////
//
//      Change log
// 17 March 2016 - Initial Template submission (Jingke)
//
////////////////////////////////////////////////////////////////////////////////

//
//	GEANT4 includes
//
#include "G4UIdirectory.hh"
// uncomment the headers that you will need
//#include "G4UIcmdWithAnInteger.hh"
//#include "G4UIcmdWithoutParameter.hh"
//#include "G4UIcmdWithAString.hh"
//#include "G4UIcmdWithABool.hh"
//#include "G4UIcmdWith3VectorAndUnit.hh"
//#include "G4UIcmdWithADoubleAndUnit.hh"
//#include "G4UIcmdWith3Vector.hh"
//#include "G4UIcmdWithADouble.hh"
#include "globals.hh"

//
// Project-specific includes
//
#include "AmBeXe137SourceMessenger.hh"
#include "AmBeXe137SourceDetector.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					Messenger()
//------++++++------++++++------++++++------++++++------++++++------++++++------
AmBeXe137SourceMessenger::AmBeXe137SourceMessenger( AmBeXe137SourceDetector *Det )
  : AmBeXe137Sourcedetector( Det )
{
  //  Set up the directory
  AmBeXe137SourceDir = new G4UIdirectory( "/AmBeXe137Source/" );
  AmBeXe137SourceDir->SetGuidance( "All AmBeXe137Source project commands are contained under /AmBeXe137Source/" );

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~Messenger()
//------++++++------++++++------++++++------++++++------++++++------++++++------
AmBeXe137SourceMessenger::~AmBeXe137SourceMessenger()
{
  //delete everything that you declared with new
  delete AmBeXe137SourceDir;

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetNewValue()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void AmBeXe137SourceMessenger::SetNewValue( G4UIcommand *command, G4String newValue )
{
  
}

