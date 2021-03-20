////////////////////////////////////////////////////////////////////////////////
//
// This code is automatically generated by BACCARAT to define TMS materials
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
#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4SystemOfUnits.hh"

//
//	Project-specific includes
//
#include "TMSMaterials.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					TMSMaterials()
//------++++++------++++++------++++++------++++++------++++++------++++++------
TMSMaterials *TMSMaterials::TMSmaterials = 0;
TMSMaterials::TMSMaterials()
{
  if(TMSmaterials){
    G4cerr<<"TMSMaterials is a singleton:: an instance already exists!"<<G4endl;
    //    delete TMSmaterials;
    exit(1);
  }  
  CreateMaterials();
//  CreateOpticalSurfaces();
  CreateVisualAttributes();
  
  TMSmaterials = this;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~TMSMaterials()
//------++++++------++++++------++++++------++++++------++++++------++++++------
TMSMaterials::~TMSMaterials()
{


  delete tms;

  delete tmsVis;


  
  TMSmaterials = 0;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetInstance()
//------++++++------++++++------++++++------++++++------++++++------++++++------
TMSMaterials *TMSMaterials::GetInstance()
{
  return TMSmaterials;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				   GetMaterialByName()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4Material *TMSMaterials::GetMaterialByName(G4String mat_name)
{
  for (material_map::iterator it=materials.begin(); it!=materials.end(); it++){
    if(it->first == mat_name) return it->second;
  }//end for
  return NULL;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				   GetVisAttributesByName()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4VisAttributes *TMSMaterials::GetVisAttributesByName(G4String mat_name)
{
  for (vis_map::iterator it=visattributes.begin(); it!=visattributes.end(); it++){
    if(it->first == mat_name) return it->second;
  }//end for
  return NULL;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				   GetPropertiesTableByName()
//------++++++------++++++------++++++------++++++------++++++------++++++------
// G4MaterialPropertiesTable *TMSMaterials::GetPropertiesTableByName(G4String mat_name)
// {
//   for (property_map::iterator it=properties.begin(); it!=properties.end(); it++){
//     if(it->first == mat_name) return it->second;
//   }//end for
//   return NULL;
// }

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				   GetOpticalSurfaceByName()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4OpticalSurface *TMSMaterials::GetOpticalSurfaceByName(G4String optical_name)
{
  for (optical_map::iterator it=opticals.begin(); it!=opticals.end(); it++){
    if(it->first == optical_name) return it->second;
  }//end for
  return NULL;
}


void TMSMaterials::CreateMaterials()
{
  //
  //	Isotopes
  //	Atomic weights come from webelements.com
  //
  
  G4Isotope *Xe124 = new G4Isotope( "Xe124", 54, 124, 123.9058942 *g/mole );
  G4Isotope *Xe126 = new G4Isotope( "Xe126", 54, 126, 125.904281 *g/mole );
  G4Isotope *Xe128 = new G4Isotope( "Xe128", 54, 128, 127.9035312 *g/mole );
  G4Isotope *Xe129 = new G4Isotope( "Xe129", 54, 129, 128.9047801 *g/mole );
  G4Isotope *Xe130 = new G4Isotope( "Xe130", 54, 130, 129.9035094 *g/mole );
  G4Isotope *Xe131 = new G4Isotope( "Xe131", 54, 131, 130.905072 *g/mole );
  G4Isotope *Xe132 = new G4Isotope( "Xe132", 54, 132, 131.904144 *g/mole );
  G4Isotope *Xe134 = new G4Isotope( "Xe134", 54, 134, 133.905395 *g/mole );
  G4Isotope *Xe136 = new G4Isotope( "Xe136", 54, 136, 135.907214 *g/mole );
 
  G4Isotope *Si28 = new G4Isotope( "Si28", 14, 28, 27.977 *g/mole );
  G4Isotope *Si29 = new G4Isotope( "Si29", 14, 29, 28.976 *g/mole );
  G4Isotope *Si30 = new G4Isotope( "Si30", 14, 30, 29.974 *g/mole );
 
  G4Isotope *C12 = new G4Isotope( "C12", 6, 12, 12. *g/mole );
  G4Isotope *C13 = new G4Isotope( "C13", 6, 13, 13. *g/mole );

  G4Isotope *H1 = new G4Isotope( "H1", 1, 1, 1.008*g/mole );
 
  G4Isotope *Na23 = new G4Isotope("Na23", 11, 23, 23.*g/mole );
  
  G4Isotope *B10 = new G4Isotope("B10", 5, 10, 10.*g/mole );
  G4Isotope *B11 = new G4Isotope("B11", 5, 11, 11.*g/mole );

  G4Isotope *O16 = new G4Isotope("O16", 8, 16, 16.*g/mole );
  G4Isotope *O18 = new G4Isotope("O18",8, 18, 18.*g/mole );
  G4Isotope *O17 = new G4Isotope("O17",8,17,17.*g/mole );

  G4Isotope *I127 = new G4Isotope("I127",53,127,127.*g/mole);


  //
  //	Elements
  //
	
  G4Element *natXe = new G4Element( "Natural Xe", "natXe", 9 );
  natXe->AddIsotope( Xe124, 0.09*perCent );
  natXe->AddIsotope( Xe126, 0.09*perCent );
  natXe->AddIsotope( Xe128, 1.92*perCent );
  natXe->AddIsotope( Xe129, 26.44*perCent );
  natXe->AddIsotope( Xe130, 4.08*perCent );
  natXe->AddIsotope( Xe131, 21.18*perCent );
  natXe->AddIsotope( Xe132, 26.89*perCent );
  natXe->AddIsotope( Xe134, 10.44*perCent );
  natXe->AddIsotope( Xe136, 8.87*perCent );

  G4Element *natC = new G4Element("Natural Carbon", "natC", 2);
  natC->AddIsotope( C12, 98.9*perCent );
  natC->AddIsotope( C13, 1.1*perCent );

  G4Element *natSi = new G4Element("Natural Silicon","natSi",3);
  natSi->AddIsotope( Si28, 92.2*perCent );
  natSi->AddIsotope( Si29, 4.7*perCent );
  natSi->AddIsotope( Si30, 3.1*perCent );

  G4Element *natH = new G4Element("Natural Hydrogen","natH",1);
  natH->AddIsotope( H1, 100.*perCent);  


  G4Element * natNa = new G4Element("Natural Sodium", "natNa",1);
  natNa->AddIsotope( Na23, 100.*perCent );

  G4Element * natB = new G4Element("Natural Boron", "natB", 2);
  natB->AddIsotope( B10, 20.*perCent);
  natB->AddIsotope( B11, 80.*perCent);
	
  G4Element * natO = new G4Element("Natural Oxygen", "natO", 3);
  natO->AddIsotope( O16, 99.76 * perCent);
  natO->AddIsotope( O17, 0.04 * perCent);
  natO->AddIsotope( O18, 0.2 * perCent);

  G4Element * natI = new G4Element("Natural Iodine", "natI", 1);
  natI->AddIsotope( I127, 100.*perCent );

  //
  //	Materials
  //
  G4Material * mat_ = NULL;
  
//  G4Material * liquidXe = new G4Material( "liquidXe", 2.888*g/cm3, 1, kStateLiquid,
//					  173*kelvin, 1.57*bar );
//  liquidXe->AddElement( natXe, 1 );
//  mat_ = GetMaterialByName("liquidXe");
//  if(mat_) delete mat_;
//  materials ["liquidXe"] = liquidXe;
  
  tms = new G4Material( "tms", 0.651*g/cm3, 3, kStateLiquid,
                                     295*kelvin, 1.*bar );
  tms->AddElement( natSi, 1 );
  tms->AddElement( natC, 4 );
  tms->AddElement( natH, 12 );

  mat_ = GetMaterialByName("tms");
  if(mat_) delete mat_;
  materials["tms"] = tms; 


  // Add Paraffin
  paraffin = new G4Material("paraffin",0.9*g/cm3,2,kStateSolid,
                             295.*kelvin,1.*bar);
  paraffin->AddElement( natC, 31 );
  paraffin->AddElement( natH, 64 );

  mat_ = GetMaterialByName("paraffin");
  if(mat_) delete mat_;
  materials["paraffin"] = paraffin;

  // Add borated water (assumed 10lbs borax dissolved in 3500lbs of water)
  boratedWater = new G4Material("boratedWater",0.997*g/cm3,4,kStateLiquid,
                                  295.*kelvin,1.*bar);
  boratedWater->AddElement( natNa, 5 );
  boratedWater->AddElement( natB, 10 );
  boratedWater->AddElement( natO, 7 + 10 + 7428 );
  boratedWater->AddElement( natH, 20 + 7428*2 );
  
  mat_ = GetMaterialByName("boratedWater");
  if(mat_) delete mat_;
  materials["boratedWater"] = boratedWater;  
  
  boratedPE_HD = new G4Material("boratedPE_HD",1.17*g/cm3,3,kStateSolid,
                                 295.*kelvin,1.*bar);
  boratedPE_HD->AddElement( natB, 5.*perCent );
  boratedPE_HD->AddElement( natC, 81.4*0.95*perCent );
  boratedPE_HD->AddElement( natH, 18.6*0.95*perCent );
 
  boratedPE_LD = new G4Material("boratedPE_LD",1.0*g/cm3,4,kStateSolid,
                                 295.*kelvin,1.*bar);
  boratedPE_LD->AddElement( natB, 5.*perCent );
  boratedPE_LD->AddElement( natC, 61.13*perCent );
  boratedPE_LD->AddElement( natO, 22.20*perCent );
  boratedPE_LD->AddElement( natH, 11.67*perCent );

  boratedPE_WD = new G4Material("boratedPE_WD",0.997*g/cm3,3,kStateSolid,
                                 295.*kelvin,1.*bar);
  boratedPE_WD->AddElement( natB, 5.*perCent );
  boratedPE_WD->AddElement( natC, 81.4*0.95*perCent );
  boratedPE_WD->AddElement( natH, 18.6*0.95*perCent );


  sodiumIodide = new G4Material("sodiumIodide",3.67*g/cm3,2,kStateSolid,
                                  295.*kelvin,1.*bar);
  sodiumIodide->AddElement( natI, 50.*perCent );
  sodiumIodide->AddElement( natNa, 50.*perCent );
  
  mat_ = GetMaterialByName("sodiumIodide");
  if(mat_) delete mat_;
  materials["sodiumIodide"] = sodiumIodide;

  ej276 = new G4Material("ej276", 1.096*g/cm3,2,kStateSolid,
                                  295.*kelvin,1.*bar);
  ej276->AddElement( natH, 453 );
  ej276->AddElement( natC, 489 );


}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//					CreateOpticalSurfaces()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void TMSMaterials::CreateOpticalSurfaces()
{
	
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					CreateVisualAttributes()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void TMSMaterials::CreateVisualAttributes()
{

  G4VisAttributes * vis_ = NULL;

//  G4VisAttributes * liquidXeVis = new G4VisAttributes();
//  liquidXeVis->SetColor( 0.0, 0.0, 1.0, 0.4 );
//  liquidXeVis->SetVisibility( true );

//  vis_ = GetVisAttributesByName("liquidXe");
//  if(vis_) delete vis_;
//  visattributes ["liquidXe"] = liquidXeVis;

  tmsVis = new G4VisAttributes();
  tmsVis->SetColor(0.1,0.3,0.3,0.1);
  tmsVis->SetVisibility( true );

  vis_ = GetVisAttributesByName("tms");
  if(vis_) delete vis_;
  visattributes["tms"] = tmsVis;

  paraffinVis = new G4VisAttributes();
  paraffinVis->SetColor(0.2,0.2,0.2,0.6);
  paraffinVis->SetVisibility( true );
 
  vis_ = GetVisAttributesByName("paraffin");
  if(vis_) delete vis_;
  visattributes["paraffin"] = paraffinVis;
  
  boratedWaterVis = new G4VisAttributes();
  boratedWaterVis->SetColor(0.,0.,1.,0.1);
  boratedWaterVis->SetVisibility( true );

  vis_ = GetVisAttributesByName("boratedWater");
  if(vis_) delete vis_;
  visattributes["boratedWater"] = boratedWaterVis;


}

