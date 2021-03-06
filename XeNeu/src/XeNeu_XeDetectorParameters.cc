#include "XeNeu_XeDetectorParameters.hh"
#include "G4SystemOfUnits.hh"



namespace XeNeuDetectorParameters{
  
  //define your detector parameters here

  // Outer Steel Can 
  double outerCanInnerRadius = (8.0/2. - 1./16.) * 2.54 * cm;
  double outerCanOuterRadius = (8./2.) * 2.54 * cm;
  double outerCanHalfHeight = 14./2. * 2.54 * cm;

  // Inner Steel Can
  double innerCanInnerRadius = (2.0 - 1./16.) * 2.54 * cm;
  double innerCanOuterRadius = 2.0 * 2.54 * cm;
  double innerCanHalfHeight = 7. * 2.54 * cm;

  // Teflon block
  double teflonBlockOuterRadius = (2.0 - 1./8.) * 2.54 * cm;
  double teflonBlockInnerRadius = (2.303/2.) * 2.54 * cm;
  double teflonBlockHalfHeight = (5.65/2.) * 2.54 * cm;

  // Weir
  double weirCutoutAngle = 84.0 * deg;
  double weirInnerRadius = teflonBlockInnerRadius + 0.1*2.54*cm;
  double weirOuterRadius = teflonBlockOuterRadius;
  double weirHalfHeight = 5.79/2. * 2.54 * cm;
  double weirPocketOuterRadius = weirOuterRadius - 0.076*cm; //1.746 * 2.54 * cm;
  double weirPocketInnerRadius = weirInnerRadius + 0.076*cm;//1.396 * 2.54 * cm;
  double weirPocketCutoutAngle = 75. * deg;

  // Field ring
  double fieldRingInnerRadius = 1.95/2. * 2.54 * cm;
  double fieldRingOuterRadius = 2.4/2. * 2.54 * cm;
  double fieldRingHalfHeight = 0.05 * 2.54 * cm;
  double fieldGridHalfHeight = 0.075 * 2.54 * cm;

  // Bottom PMT
  double bottomPMTHalfHeight = 2.2 * 2.54 * cm;
  double bottomPMTRadius = 2.244/2. * 2.54 * cm;

  // LS detector
  double LSdetectorRadius = 2. * 2.54 * cm;
  double LSdetectorHalfHeight = 2. * 2.54 * cm;

}


