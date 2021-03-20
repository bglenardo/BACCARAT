////////////////////////////////////////////////////////////////////////////////
/*  BaccExampleNobleTPC.cc

This is the code file to define an "simple" example detector.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Kareem)
*/
////////////////////////////////////////////////////////////////////////////////

//
//  Geant4 includes
//
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"


//
//  BACCARAT includes
//
#include "BaccExampleNobleTPC.hh"
#include "BaccDetectorComponent.hh"
#include "BaccExample8778PMT.hh"
#include "BaccMaterials.hh"

using namespace std;

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				BaccExampleNobleTPC
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccExampleNobleTPC::BaccExampleNobleTPC( G4String detName,
        G4String outVolName )
{
    detectorName = detName;
    outermostVolumeName = outVolName;

    //  Define the relevant parameters. This example vessel has the following
    //  measurements that should be readable on any engineering diagram if this
    //  detector did indeed exist. Which it doesn't.
    BaccExample8778PMT testPMT;
    pmtHeight = testPMT.GetPMTHeight();
    pmtRadius = testPMT.GetPMTRadius();

    spaceBelowBottomPMTs = 2.5*cm;
    reverseFieldHeight = 2.5*cm;
    activeTargetHeight = 15.*cm;
    gainVolumeHeight = 1.*cm;
    spaceAboveTopPMTs = 2.5*cm;
    
    G4double totalCentralVolumeHeight = spaceBelowBottomPMTs + pmtHeight +
            reverseFieldHeight + activeTargetHeight + gainVolumeHeight +
            pmtHeight + spaceAboveTopPMTs;
    totalCentralVolumeHalfHeight = totalCentralVolumeHeight/2.;
    totalCentralVolumeRadius = 15.*cm;

    reflectorThickness = 0.5*cm;
    reflectorHalfHeight = totalCentralVolumeHalfHeight + reflectorThickness;
    reflectorOuterRadius = totalCentralVolumeRadius + reflectorThickness;
    
    cryostatWallThickness = 1.*cm;
    cryostatHalfHeight = reflectorHalfHeight + cryostatWallThickness;
    cryostatOuterRadius = reflectorOuterRadius + cryostatWallThickness;

    vacuumSpaceThickness = 5.*cm;
    vacuumHalfHeight = cryostatHalfHeight + vacuumSpaceThickness;
    vacuumOuterRadius = cryostatOuterRadius + vacuumSpaceThickness;
    
    vacuumVesselThickness = 1.*cm;
    vacuumVesselHalfHeight = vacuumHalfHeight + vacuumVesselThickness;
    vacuumVesselOuterRadius = vacuumOuterRadius + vacuumVesselThickness;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~BaccExampleNobleTPC
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccExampleNobleTPC::~BaccExampleNobleTPC() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				BuildDetector
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccExampleNobleTPC::BuildDetector()
{
    //
    //  Build up the detector, starting from the outer-most volume and working
    //  in. Make sure each volume is wholly contained within the mother volume.
    //

    //  Start with the vacuum vessel
    G4Tubs *vacuumVessel_solid = new G4Tubs( "vacuumVessel_solid", 0,
            vacuumVesselOuterRadius, vacuumVesselHalfHeight, 0, 360.*deg );
    logicalVolume = new G4LogicalVolume( vacuumVessel_solid,
            baccMaterials->Copper(), "vacuumVessel_log" );
    logicalVolume->SetVisAttributes( baccMaterials->CopperVis() );
    outermostVolumeName = "vacuumVessel";    

    //  Put a vacuum in the vacuum vessel
    G4Tubs *vacuum_solid = new G4Tubs( "vacuum_solid", 0, vacuumOuterRadius,
            vacuumHalfHeight, 0, 360.*deg );
    G4LogicalVolume *vacuum_log = new G4LogicalVolume( vacuum_solid,
            baccMaterials->Vacuum(), "vacuum_log" );
    vacuum_log->SetVisAttributes( baccMaterials->VacuumVis() );
    BaccDetectorComponent *vacuum = new BaccDetectorComponent( 0,
            G4ThreeVector(), vacuum_log, "Vacuum", logicalVolume, 0, 0, false );
    
    //  Put a cryostat inside the vacuum space
    G4Tubs *cryostat_solid = new G4Tubs( "cryostat_solid", 0,
            cryostatOuterRadius, cryostatHalfHeight, 0, 360.*deg );
    G4LogicalVolume *cryostat_log = new G4LogicalVolume( cryostat_solid,
            baccMaterials->Copper(), "cryostat_log" );
    cryostat_log->SetVisAttributes( baccMaterials->CopperVis() );
    BaccDetectorComponent *cryostat = new BaccDetectorComponent( 0,
            G4ThreeVector(), "Cryostat", cryostat_log, vacuum, 0, 0, false );
    
    //  Put a PTFE reflector inside the cryostat
    G4Tubs *reflector_solid = new G4Tubs( "reflector_solid", 0,
            reflectorOuterRadius, reflectorHalfHeight, 0, 360.*deg );
    G4LogicalVolume *reflector_log = new G4LogicalVolume( reflector_solid,
            baccMaterials->Teflon(), "reflector_log" );
    reflector_log->SetVisAttributes( baccMaterials->TeflonVis() );
    BaccDetectorComponent *reflector = new BaccDetectorComponent( 0,
            G4ThreeVector(), "Reflector", reflector_log, cryostat, 0, 0, false);
    
    //  Fill the entire central space with liquid. The active liquid volume,
    //  bottom PMTs, and entire gas volume will be immediate daughters of this
    //  central liquid volume. (The top PMTs will be immediate daughters of the
    //  gas volume.)
    G4Tubs *centralVolume_solid = new G4Tubs( "centralVolume_solid", 0,
            totalCentralVolumeRadius, totalCentralVolumeHalfHeight, 0, 360*deg);
    G4LogicalVolume *centralVolume_log = new G4LogicalVolume(
            centralVolume_solid, baccMaterials->LiquidXe(),
            "centralVolume_log" );
    centralVolume_log->SetVisAttributes( baccMaterials->LiquidXeVis() );
    BaccDetectorComponent *centralVolume = new BaccDetectorComponent(
            0, G4ThreeVector(), "LXeVolume", centralVolume_log,
            reflector, 0, 0, false );
    
    //	Optical boundary between the reflector and the liquid volume
	G4LogicalBorderSurface *teflonLiquidSurface;
    teflonLiquidSurface = new G4LogicalBorderSurface( "teflonLiquidSurface",
            centralVolume, reflector, baccMaterials->LXeTeflonSurface() );

    //  The liquid PMTs. We had to make some assumptions about distance between
    //  the PMTs. The X and Y "padding" is not identical, but this is, after
    //  all, only an example geometry. Do what thou wilt.
    G4double pmtPaddingSpace = 0.2*cm;
    G4double pmtXOffset[7], pmtYOffset[7];

    pmtXOffset[1] = pmtXOffset[6] = (2.*pmtRadius + pmtPaddingSpace)/2;
    pmtXOffset[0] = pmtXOffset[5] = -pmtXOffset[1];
    pmtXOffset[4] = pmtXOffset[1]*2.;
    pmtXOffset[2] = -pmtXOffset[4];
    pmtXOffset[3] = 0;
    
    pmtYOffset[0] = pmtYOffset[1] = 2*pmtRadius + pmtPaddingSpace;
    pmtYOffset[2] = pmtYOffset[3] = pmtYOffset[4] = 0;
    pmtYOffset[5] = pmtYOffset[6] = -pmtYOffset[0];
    
    G4double bottomPMTZOffset = -totalCentralVolumeHalfHeight + pmtHeight/2. +
            spaceBelowBottomPMTs;

    BaccExample8778PMT *bottomPMTs[7];
    BaccDetectorComponent *bottomPMT[7];
    G4LogicalBorderSurface *pmtCanLiquidSurface[7];
    for( G4int i=0; i<7; i++ ) {
        bottomPMTs[i] = new BaccExample8778PMT();
        
        stringstream name;
        name << "BottomPMTCan_" << i+1;
        bottomPMT[i] = new BaccDetectorComponent( 0,
                G4ThreeVector(pmtXOffset[i],pmtYOffset[i],bottomPMTZOffset),
                name.str().c_str(), bottomPMTs[i]->GetLogicalVolume(),
                centralVolume, 0, 0, false );
        
        name.str("");
        name << "BottomPMTCanLiquidSurface_" << i+1;
        pmtCanLiquidSurface[i] = new G4LogicalBorderSurface(
                name.str().c_str(), centralVolume, bottomPMT[i],
                baccMaterials->LXeSteelSurface() );

        name.str("");
        name << "BottomPMTWindow_" << i+1;
        bottomPMTs[i]->GetPMTWindow()->SetName( name.str() );
        
        name.str("");
        name << "BottomPMTPhotocathode_" << i+1;
        bottomPMTs[i]->GetPMTPhotocathode()->SetName( name.str() );
        
        name.str("");
        name << "BottomPMTFlashing_" << i+1;
        bottomPMTs[i]->GetPMTFlashing()->SetName( name.str() );
        
        name.str("");
        name << "BottomPMTVacuum_" << i+1;
        bottomPMTs[i]->GetPMTVacuum()->SetName( name.str() );
    }
    
    //  Reverse-field volume
    G4Tubs *reverseFieldVol_solid = new G4Tubs( "reverseFieldVol_solid", 0,
            totalCentralVolumeRadius, reverseFieldHeight/2., 0, 360.*deg );
    G4LogicalVolume *reverseFieldVol_log = new G4LogicalVolume(
            reverseFieldVol_solid, baccMaterials->LiquidXe(),
            "reverseFieldVol_log" );
    reverseFieldVol_log->SetVisAttributes( baccMaterials->LiquidXeVis() );
    G4double reverseFieldZOffset = -totalCentralVolumeHalfHeight +
            reverseFieldHeight/2. + spaceBelowBottomPMTs + pmtHeight;
    BaccDetectorComponent *reverseFieldVol = new BaccDetectorComponent(
            0, G4ThreeVector( 0, 0, reverseFieldZOffset ), "ReverseFieldVolume",
            reverseFieldVol_log, centralVolume, 0, 0, false );
    
    //	Optical boundary between the reflector and the reverse-field volume
	G4LogicalBorderSurface *teflonReverseFieldSurface;
    teflonReverseFieldSurface = new G4LogicalBorderSurface(
            "teflonReverseFieldSurface", reverseFieldVol, reflector,
            baccMaterials->LXeTeflonSurface() );

    //  Active liquid volume
    G4Tubs *activeLiquid_solid = new G4Tubs( "activeLiquid_solid", 0,
            totalCentralVolumeRadius, activeTargetHeight/2., 0, 360.*deg );
    G4LogicalVolume *activeLiquid_log = new G4LogicalVolume(
            activeLiquid_solid, baccMaterials->LiquidXe(),
            "activeLiquid_log" );
    activeLiquid_log->SetVisAttributes( baccMaterials->LiquidXeVis() );
    G4double activeLiquidZOffset = -totalCentralVolumeHalfHeight +
            activeTargetHeight/2. + spaceBelowBottomPMTs + pmtHeight +
            reverseFieldHeight;
    BaccDetectorComponent *activeLXe = new BaccDetectorComponent(
            0, G4ThreeVector( 0, 0, activeLiquidZOffset ), "ActiveLXe",
            activeLiquid_log, centralVolume, 0, 0, false );
    
    //	Optical boundary between the reflector and the active liquid volume
	G4LogicalBorderSurface *teflonActiveLiquidSurface;
    teflonActiveLiquidSurface = new G4LogicalBorderSurface(
            "teflonActiveLiquidSurface", activeLXe, reflector,
            baccMaterials->LXeTeflonSurface() );
    
    //  Top gas volume
    G4double gasVolumeHalfHeight = ( gainVolumeHeight + pmtHeight +
            spaceAboveTopPMTs )/2.;
    G4Tubs *topGasVolume_solid = new G4Tubs( "topGasVolume_solid", 0,
            totalCentralVolumeRadius, gasVolumeHalfHeight, 0, 360.*deg );
    G4LogicalVolume *topGasVolume_log = new G4LogicalVolume( topGasVolume_solid,
            baccMaterials->GasXe(), "topGasVolume_log" );
    topGasVolume_log->SetVisAttributes( baccMaterials->GasXeVis() );
    
    G4double topGasZOffset = totalCentralVolumeHalfHeight - gasVolumeHalfHeight;
    BaccDetectorComponent *topGasVolume = new BaccDetectorComponent(
            0, G4ThreeVector(0,0,topGasZOffset), "GXeVolume",
            topGasVolume_log, centralVolume, 0, 0, false );
    
    //	Optical boundary between the reflector and the gas volume
	G4LogicalBorderSurface *teflonGasSurface;
    teflonGasSurface = new G4LogicalBorderSurface( "teflonGasSurface",
            topGasVolume, reflector, baccMaterials->GXeTeflonSurface() );

    //  Top PMTs
    G4double topPMTZOffset = -gasVolumeHalfHeight + pmtHeight/2. +
            gainVolumeHeight;

    BaccExample8778PMT *topPMTs[7];
    BaccDetectorComponent *topPMT[7];
    G4LogicalBorderSurface *pmtCanGasSurface[7];
    G4RotationMatrix *rotX180 = new G4RotationMatrix();
    rotX180->rotateX(180.*deg);
    for( G4int i=0; i<7; i++ ) {
        topPMTs[i] = new BaccExample8778PMT();

        stringstream name;
        name << "TopPMTCan_" << i+1;
        topPMT[i] = new BaccDetectorComponent( rotX180,
                G4ThreeVector(pmtXOffset[i],pmtYOffset[i],topPMTZOffset),
                name.str().c_str(), topPMTs[i]->GetLogicalVolume(),
                topGasVolume, 0, 0, false );

        name.str("");
        name << "topPMTCanGasSurface" << i+1;
        pmtCanGasSurface[i] = new G4LogicalBorderSurface(
                name.str().c_str(), topGasVolume, topPMT[i],
                baccMaterials->GXeSteelSurface() );
        
        name.str("");
        name << "TopPMTWindow_" << i+1;
        bottomPMTs[i]->GetPMTWindow()->SetName( name.str() );
        
        name.str("");
        name << "TopPMTPhotocathode_" << i+1;
        bottomPMTs[i]->GetPMTPhotocathode()->SetName( name.str() );
        
        name.str("");
        name << "TopPMTFlashing_" << i+1;
        bottomPMTs[i]->GetPMTFlashing()->SetName( name.str() );
        
        name.str("");
        name << "TopPMTVacuum_" << i+1;
        bottomPMTs[i]->GetPMTVacuum()->SetName( name.str() );
    }
    
    //  And finally, the gas gain volume
    G4Tubs *gasGainVol_solid = new G4Tubs( "gasGainVol_solid", 0,
            totalCentralVolumeRadius, gainVolumeHeight/2., 0, 360.*deg );
    G4LogicalVolume *gasGainVol_log = new G4LogicalVolume(
            gasGainVol_solid, baccMaterials->GasXe(),
            "gasGainVol_log" );
    gasGainVol_log->SetVisAttributes( baccMaterials->GasXeVis() );
    G4double gasGainZOffset = -gasVolumeHalfHeight + gainVolumeHeight/2.;
    BaccDetectorComponent *gasGainVol = new BaccDetectorComponent(
            0, G4ThreeVector( 0, 0, gasGainZOffset ), "GasGainVolume",
            gasGainVol_log, topGasVolume, 0, 0, false );
    
    //	Optical boundary between the reflector and the gas gain volume
	G4LogicalBorderSurface *teflonGasGainSurface;
    teflonGasGainSurface = new G4LogicalBorderSurface( "teflonGasGainSurface",
            gasGainVol, reflector, baccMaterials->GXeTeflonSurface() );

}
