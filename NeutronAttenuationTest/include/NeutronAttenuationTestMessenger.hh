////////////////////////////////////////////////////////////////////////////////
//
// This code is automatically generated by BACCARAT to define the messenger
// commands for the NeutronAttenuationTest project.
// All geometry, physics, io managements, etc, happen here
//
////////////////////////////////////////////////////////////////////////////////
//
//      Change log
// 17 March 2016 - Initial Template submission (Jingke)
//
////////////////////////////////////////////////////////////////////////////////

#ifndef NeutronAttenuationTestMessenger_HH
#define NeutronAttenuationTestMessenger_HH 1

//
//      GEANT4 includes
//
#include "globals.hh"
#include "G4UImessenger.hh"

//
//      Class forwarding
//
class G4UIdirectory;
// Uncomment the class that you will need
//class G4UicmdWithAnInteger;
//class G4UicmdWithoutParameter;
//class G4UicmdWithAString;
//class G4UicmdWithABool;
class G4UIcmdWithADoubleAndUnit;
//class G4UicmdWithADouble;

class NeutronAttenuationTestDetector;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class NeutronAttenuationTestMessenger : public G4UImessenger
{
public:
  NeutronAttenuationTestMessenger( NeutronAttenuationTestDetector* );
  ~NeutronAttenuationTestMessenger();
  
  void SetNewValue( G4UIcommand*, G4String );
  
private:
  NeutronAttenuationTestDetector *NeutronAttenuationTestdetector;
  
  //      The directory
  G4UIdirectory *NeutronAttenuationTestDir;

  // Declare the UI commands below
  G4UIcmdWithADoubleAndUnit * ShieldingThicknessCommand;

  
};

#endif
