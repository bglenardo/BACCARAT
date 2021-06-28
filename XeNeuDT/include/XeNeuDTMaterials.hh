////////////////////////////////////////////////////////////////////////////////
//
// This code is automatically generated by BACCARAT to define XeNeu materials
//
////////////////////////////////////////////////////////////////////////////////
//
//      Change log
// 17 March 2016 - Initial Template submission (Jingke)
//
////////////////////////////////////////////////////////////////////////////////

#ifndef XeNeuDTMaterials_HH
#define XeNeuDTMaterials_HH 1

//
//	c++ includes
//
#include <map>

//
//	Geant4 includes
//
#include "G4String.hh"

//
//	Class forwarding
//
class G4Material;
class G4VisAttributes;
//class G4MaterialPropertiesTable;
class G4OpticalSurface;

typedef std::map<G4String, G4Material*> material_map;
//typedef std::map<G4String, G4MaterialPropertiesTable*> property_map;
typedef std::map<G4String, G4VisAttributes*> vis_map;
typedef std::map<G4String, G4OpticalSurface*> optical_map;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class XeNeuDTMaterials
{
public:
  XeNeuDTMaterials();
  ~XeNeuDTMaterials();
  
  static XeNeuDTMaterials *GetInstance();
  
  G4Material *GetMaterialByName(G4String);
  G4VisAttributes *GetVisAttributesByName(G4String);
  //  G4MaterialPropertiesTable *GetPropertiesTableByName(G4String);
  G4OpticalSurface *GetOpticalSurfaceByName(G4String);
  
  inline G4Material* LiquidXe() { return liquidXe; }
  inline G4Material* BoratedWater() { return boratedWater; }
  inline G4Material* Borax() { return borax; }
  inline G4Material* BPE5() { return bpe5; }
  inline G4Material* BPE10() { return bpe10; }
  inline G4Material* BPE15() { return bpe15; }
  inline G4Material* BPE20() { return bpe20; }
  inline G4Material* BPE30() { return bpe30; }
  inline G4Material* Concrete(){ return concrete; }
   
  inline G4VisAttributes* ConcreteVis(){ return concreteVis; }
  inline G4VisAttributes* LiquidXeVis() { return liquidXeVis; }
  inline G4VisAttributes* BoratedWaterVis() { return boratedWaterVis; }
  inline G4VisAttributes* BoraxVis() { return boraxVis; }
  inline G4VisAttributes* BPE5Vis() { return bpe5Vis; }
  inline G4VisAttributes* BPE10Vis() { return bpe10Vis; }
  inline G4VisAttributes* BPE15Vis() { return bpe15Vis; }
  inline G4VisAttributes* BPE20Vis() { return bpe20Vis; }
  inline G4VisAttributes* BPE30Vis() { return bpe30Vis; }

private:
  
  static XeNeuDTMaterials *XeNeumaterials;
  
  material_map materials;
  //  property_map properties;
  vis_map visattributes;
  optical_map opticals;
  
  void CreateMaterials();
  void CreateOpticalSurfaces();
  void CreateVisualAttributes();

  G4Material * boratedWater;
  G4Material * liquidXe;
  G4Material * borax;
  G4Material * bpe5;
  G4Material * bpe10;
  G4Material * bpe15;
  G4Material * bpe20;
  G4Material * bpe30;  
  G4Material * concrete;  

  G4VisAttributes* concreteVis;
  G4VisAttributes* liquidXeVis;
  G4VisAttributes* boratedWaterVis;
  G4VisAttributes* boraxVis;
  G4VisAttributes* bpe5Vis;
  G4VisAttributes* bpe10Vis;
  G4VisAttributes* bpe15Vis;
  G4VisAttributes* bpe20Vis;
  G4VisAttributes* bpe30Vis;
};

#endif
