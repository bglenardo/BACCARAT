////////////////////////////////////////////////////////////////////////////////
//
// This code is automatically generated by BACCARAT to define Xe127Radioassay materials
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
#include "Xe127RadioassayMaterials.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					Xe127RadioassayMaterials()
//------++++++------++++++------++++++------++++++------++++++------++++++------
Xe127RadioassayMaterials *Xe127RadioassayMaterials::Xe127Radioassaymaterials = 0;
Xe127RadioassayMaterials::Xe127RadioassayMaterials()
{
  if(Xe127Radioassaymaterials){
    G4cerr<<"Xe127RadioassayMaterials is a singleton:: an instance already exists!"<<G4endl;
    //    delete Xe127Radioassaymaterials;
    exit(1);
  }  
  CreateMaterials();
  CreateOpticalSurfaces();
  CreateVisualAttributes();
  
  Xe127Radioassaymaterials = this;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~Xe127RadioassayMaterials()
//------++++++------++++++------++++++------++++++------++++++------++++++------
Xe127RadioassayMaterials::~Xe127RadioassayMaterials()
{

  //delete all materials
  if(materials.size()>0){
    for (material_map::iterator it=materials.begin(); it!=materials.end(); it++){
      if(it->second){
	delete it->second;
	it->second = NULL;
      }//end if second
    }//end for
  }//end if size

  //delete all material properties
  //   if(properties.size()>0){
  //     for (property_map::iterator it=properties.begin(); it!=properties.end(); it++){
  //       if(it->second){
  // 	delete it->second;
  // 	it->second = NULL;
  //       }//end if second
  //     }//end for
  //   }//end if size

  //delete all visattributes
  if(visattributes.size()>0){
    for (vis_map::iterator it=visattributes.begin(); it!=visattributes.end(); it++){
      if(it->second){
	delete it->second;
	it->second = NULL;
      }//end if second
    }//end for
  }//end if size

  //delete all opticals
  if(opticals.size()>0){
    for (optical_map::iterator it=opticals.begin(); it!=opticals.end(); it++){
      if(it->second){
	delete it->second;
	it->second = NULL;
      }//end if second
    }//end for
  }//end if size
  
  Xe127Radioassaymaterials = 0;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetInstance()
//------++++++------++++++------++++++------++++++------++++++------++++++------
Xe127RadioassayMaterials *Xe127RadioassayMaterials::GetInstance()
{
  return Xe127Radioassaymaterials;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				   GetMaterialByName()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4Material *Xe127RadioassayMaterials::GetMaterialByName(G4String mat_name)
{
  for (material_map::iterator it=materials.begin(); it!=materials.end(); it++){
    if(it->first == mat_name) return it->second;
  }//end for
  return NULL;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				   GetVisAttributesByName()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4VisAttributes *Xe127RadioassayMaterials::GetVisAttributesByName(G4String mat_name)
{
  for (vis_map::iterator it=visattributes.begin(); it!=visattributes.end(); it++){
    if(it->first == mat_name) return it->second;
  }//end for
  return NULL;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				   GetPropertiesTableByName()
//------++++++------++++++------++++++------++++++------++++++------++++++------
// G4MaterialPropertiesTable *Xe127RadioassayMaterials::GetPropertiesTableByName(G4String mat_name)
// {
//   for (property_map::iterator it=properties.begin(); it!=properties.end(); it++){
//     if(it->first == mat_name) return it->second;
//   }//end for
//   return NULL;
// }

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				   GetOpticalSurfaceByName()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4OpticalSurface *Xe127RadioassayMaterials::GetOpticalSurfaceByName(G4String optical_name)
{
  for (optical_map::iterator it=opticals.begin(); it!=opticals.end(); it++){
    if(it->first == optical_name) return it->second;
  }//end for
  return NULL;
}


void Xe127RadioassayMaterials::CreateMaterials()
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
	
  //
  //	Materials
  //
  G4Material * mat_ = NULL;
  
  G4Material * liquidXe = new G4Material( "liquidXe", 2.888*g/cm3, 1, kStateLiquid,
					  173*kelvin, 1.57*bar );
  liquidXe->AddElement( natXe, 1 );
  mat_ = GetMaterialByName("liquidXe");
  if(mat_) delete mat_;
  materials ["liquidXe"] = liquidXe;
  
}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//					CreateOpticalSurfaces()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void Xe127RadioassayMaterials::CreateOpticalSurfaces()
{
	
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					CreateVisualAttributes()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void Xe127RadioassayMaterials::CreateVisualAttributes()
{

  G4VisAttributes * vis_ = NULL;
  G4VisAttributes * liquidXeVis = new G4VisAttributes();
  liquidXeVis->SetColor( 0.0, 0.0, 1.0, 0.4 );
  liquidXeVis->SetVisibility( true );
  vis_ = GetVisAttributesByName("liquidXe");
  if(vis_) delete vis_;
  visattributes ["liquidXe"] = liquidXeVis;
}

