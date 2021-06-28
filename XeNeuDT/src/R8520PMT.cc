/*      R8520PMT.cc
 *     
 *     This is the code file to build the R8520 model PMT by Hamamatsu.
 *
 ********************************************************************************
* Change log
*  
*      2019-12-13 - Initial submission (Daniel N)
*     
*        
*********************************************************************************
*/

//
//      C/C++ includes
//

#include <sstream>
#include <string>


         
// GEANT4 includes
//
 #include "G4Box.hh"
 #include "G4Color.hh"
 #include "G4LogicalBorderSurface.hh"
 #include "G4LogicalVolume.hh"
 #include "G4OpticalSurface.hh"
 #include "G4SubtractionSolid.hh"
 #include "G4SystemOfUnits.hh"
 #include "G4Tubs.hh"
 #include "G4UnionSolid.hh"
 #include "G4VisAttributes.hh"
 #include "globals.hh"



//      BACCARAT includes
//
#include "BaccDetectorComponent.hh"
#include "BaccMaterials.hh"
#include "R8520PMT.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              R8520PMT.cc
//------++++++------++++++------++++++------++++++------++++++------++++++------

R8520PMT::R8520PMT(G4bool buildGeometry)
{
    BACCmaterials = BaccMaterials::GetMaterials();


  G4Box * detectorVolume_box = new G4Box("detector", 10. * cm, 10. * cm, 10. * cm);

          G4LogicalVolume * logicalVolume = new G4LogicalVolume(detectorVolume_box, BACCmaterials->Aluminum(),"logicalVolume");

                  logicalVolume->SetVisAttributes(BACCmaterials->SteelVis());


                          G4Box * vacuumbox = new G4Box("vacuumbox", 8. * cm, 8. * cm, 8. * cm);

                                  G4LogicalVolume * vacuumbox_log = new G4LogicalVolume(vacuumbox, BACCmaterials->Vacuum(),"vacuumbox_log");

                                  


        G4Box * measurementbox = new G4Box("measurementbox", 8. * cm, 8. * cm, 1 * mm);

        G4LogicalVolume * measurementbox_log = new G4LogicalVolume(measurementbox, BACCmaterials->Vacuum(),"measurementbox_log");

        measurementbox_log->SetVisAttributes(BACCmaterials->WaterVis());

        BaccDetectorComponent * measurementbox_obj = new BaccDetectorComponent(0,
                                                                        G4ThreeVector(0,0,0),
                                                                        measurementbox_log,
                                                                        "measurementbox_obj",
                                                                        vacuumbox_log,
                                                                        0,0,true); 




        BaccDetectorComponent * vacuumbox_obj = new BaccDetectorComponent(0,
                                                                        G4ThreeVector(0,0,0),
                                                                        vacuumbox_log,
                                                                        "vacuumbox_obj",
                                                                        logicalVolume,
                                                                        0,0,true);


        

/*

    //At the moment, this is just a PMT photocathode
    G4double windowThickness = 1.2 * mm; //from datasheet
    G4double windowWidth = 23 * mm; //guess
    pmtHeight = 2.825 * cm;
    G4double skinPhotocathodeThickness = 1. * mm;
    G4double skinPhotocathodeWidth = 20.5 * mm; //from datasheet
    bodyWidth = 2.57 * cm;
    G4double bodyThickness = 1 * mm; //guess
    G4double vacuumWidth = bodyWidth - 2 * bodyThickness;
    G4double vacuumHeight = pmtHeight - bodyThickness;
    //G4double windowRadius = vacuumRadius;
    G4double xOffset, yOffset, zOffset;


        //Make the PMT Body, for now out of steel

	G4Box* skinPMT_solid = new G4Box("skinPMT_solid",
            				bodyWidth / 2,
                        		bodyWidth / 2,
                                    	pmtHeight / 2.);
        logicalVolume = new G4LogicalVolume(skinPMT_solid,
				            BACCmaterials->Steel(),
            				    "skinPMT_log");

        logicalVolume->SetVisAttributes(BACCmaterials->SteelVis());


        //put vacuum inside
                G4Box* skinPMTvacuum_solid = new G4Box("skinPMTvacuum_solid",
                            				vacuumWidth / 2,
                                        		vacuumWidth / 2,
                                               		vacuumHeight / 2.);
        
            G4LogicalVolume* skinPMTvacuum_log = new G4LogicalVolume(skinPMTvacuum_solid,BACCmaterials->Vacuum(),"skinPMTvacuum_log");

             skinPMTvacuum_log->SetVisAttributes(BACCmaterials->VacuumVis());


        //do a quartz window at the front         
        zOffset = 0.5 * (vacuumHeight - windowThickness);

        G4Box* skinPMTwindow_solid = new G4Box("skinPMTwindow_solid",
            windowWidth / 2,
            windowWidth / 2,
            windowThickness / 2.);

        G4LogicalVolume* skinPMTwindow_log = new G4LogicalVolume(skinPMTwindow_solid,
            BACCmaterials->Quartz(),
            "skinPMTwindow_log");
        skinPMTwindow_log->SetVisAttributes(BACCmaterials->QuartzVis());


        skinPMTwindow = new BaccDetectorComponent(0,
            G4ThreeVector(xOffset, yOffset, zOffset),
            "skinPMTwindow",
            skinPMTwindow_log,
            skinPMTvacuum,
            0, 0, false);


        //photocathode                                                                                                     
        zOffset = 0; // ( - 0.5*skinPhotocathodeThickness );                                                               
        
        G4Box* skinPMTphotocathode_solid = new G4Box("skinPMTphotocathode_solid",
            skinPhotocathodeWidth / 2,
            skinPhotocathodeWidth / 2,
            skinPhotocathodeThickness / 2);

        G4LogicalVolume* skinPMTphotocathode_log = new G4LogicalVolume(skinPMTphotocathode_solid,
            BACCmaterials->Quartz(),
            "skinPMTphotocathode_log");
        G4VisAttributes* photocathodeVis = new G4VisAttributes(G4Color::Magenta());
        skinPMTphotocathode_log->SetVisAttributes(photocathodeVis);

      BaccDetectorComponent *  skinPMTphotocathode = new BaccDetectorComponent(0,
            G4ThreeVector(xOffset, yOffset, zOffset),
            "skinPMTphotocathode",
            skinPMTphotocathode_log,
            skinPMTwindow,
            0, 0, false);

*/
}

//------++++++------++++++------++++++------++++++------++++++------++++++------                                            
////                              ~R8520PMT.cc                                                                                
////------++++++------++++++------++++++------++++++------++++++------++++++------       


R8520PMT::~R8520PMT()
{
}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              Misc Class Functions
//------++++++------++++++------++++++------++++++------++++++------++++++------
/*
G4LogicalVolume* R8520PMT::GetLogicalVolume()
{
    return logicalVolume;
}
*/
/*

BaccDetectorComponent* R8520PMT::GetSkinPMTPhotocathode()
{
    return skinPMTphotocathode;
}
BaccDetectorComponent* R8520PMT::GetSkinPMTWindow()
{
    return skinPMTwindow;
}
BaccDetectorComponent* R8520PMT::GetSkinPMTVacuum()
{
    return skinPMTvacuum;
}
G4double R8520PMT::GetSkinPMTWidth()
{
    return bodyWidth;
}
G4double R8520PMT::GetSkinPMTHeight()
{
    return pmtHeight;
}
               

*/










