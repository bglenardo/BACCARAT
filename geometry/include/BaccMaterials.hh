////////////////////////////////////////////////////////////////////////////////
/*	BaccMaterials.hh
*
* This is the header file to contain all the material definitions in BACCARAT.
* These definitions include optical properties.
*
********************************************************************************
* Change log
*	13 Mar 2009 - Initial submission (Kareem)
*	25 Mar 2009 - Added GasXe, Polyethylene (Dave M.)
*	25 Oct 2009 - Added vacuumDetector material for PMT sensing of 
*                 opticalphotons (Dave M.)
*	13 Dec 2009 - Put Get methods in for the material properties tables (Kareem)
*	09 Feb 2010 - Added the material "air" (Kareem)
*	10 Feb 2010 - Added the material "leadGlass" (Kareem)
*	18 Feb 2010 - Added the InitializeMatrix methods and removed the
*				  vacuumDetector material (Kareem)
*	07 Apr 2010 - Added the material "acrylic" (Dave)
*	19 May 2010 - Added doped water materials (Melinda)
*	15 Jun 2010 - Added Halon (Melinda)
*	19 Jun 2010 - Added blackium (Kareem)
*	20 Sep 2010 - Added a hook to the LUXSimManager (Kareem)
*   16 Nov 2010 - Added optical surfaces to the LUXMaterials class (Dave)
*	24 Nov 2010 - Added gas Ar, liquid Ar, gas N2, liquid N2 (Kareem)
*	24 Nov 2010 - Added gold (Kareem)
*	25 Nov 2010 - Added gadolinium (Kareem)
*	17 Dec 2010 - Added gasArN2 for use in the LLNL single-phase detector
*				  (Kareem)
*	20 Dec 2010 - Added optical surfaces for steel/gas Xe and steel/liquid Xe
*				  interfaces (Kareem)
*	16 Feb 2011 - Added beryllium and sapphire (Kareem)
*	31 Mar 2011 - Added optical surfaces for Al / gas Ar and Au / gas Ar
*				  interfaces (Kareem)
*	04 Apr 2011 - Added optical surface for unoxidized Al / quartz for PMT 
*				  flashing (Dave)
*	21 Jul 2011 - Added optical surface for quartz-PTFE interface (Dave)
*	24 Aug 2011 - Added material PEEK (Tenzing)
*	12 Jan 2012 - Added methods for setting materials properties (Dave)
*	24 Feb 2012 - Added a generic mineral oil, but without scintillation
*				  properties (Kareem)
*	25 Feb 2012 - Added isohexane, but without scintillation properties (Kareem)
*       06 Mar 2012 - Added GS-20 and KG-2 enriched 6Li doped glass (Tenzing)
*	09 Mar 2012 - Added support for Rayleigh UI hooks (Dave)
*	26 Mar 2012 - Added several materials (various types of doped isohexane) to
*				  class, for LZ studies (Dave)
*	14 Apr 2012 - Added TPB (Kareem)
*       07 Nov 2012 - Added Teflon absorption length (Matthew)
*       18 Jul 2013 - Added PVC for the neutron collimator tube (Kevin)
*       21 Mar 2014 - Added linear akyl benze (LAB) and LAB doped with .1% Gd by
*                     mass (DopedLABGd) (Kevin)
*	03 Apr 2014 - Added tyvek for water tank. Optical properties copied from
*	              halon example. Wavelength-dependent diffuse reflectivity
*                     added. (Curt)
*       30 Apr 2014 - Added material table for dopedLABGd including abs. data.
*		          Also added material for specular reflectors in LZ. (Scott Ha.)
*       08 Jan 2015 - Added variables to hold the group velocity arrays of materials
*                     with indices of refraction that have wavelength dependence
*                     (Kareem)
*       06 Feb 2015 - Added materials for LZ conduits: bncCable, lzPMTCablingConduit and
*	              lzThermosyphonConduit.
*	16 Oct 2015 - Modified averaged material density for top PMT conduit, bottom PMT conduit, 
*	              HV Umbilical, two thermo siphon conduit/tube and PMT cable (Joseph Hor)              
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef BaccMaterials_HH
#define BaccMaterials_HH 1

//
//	Class forwarding
//
class G4Material;
class G4VisAttributes;
class G4MaterialPropertiesTable;
class G4OpticalSurface;

class BaccManager;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccMaterials
{
	public:
		BaccMaterials();
		~BaccMaterials();
	
		static BaccMaterials *GetMaterials();
		
		inline G4Material *Acrylic() { return acrylic; };
		inline G4Material *Air() { return air; };
		inline G4Material *Aluminum() { return aluminum; };
		inline G4Material *Beryllium() { return beryllium; };
		inline G4Material *BeCu() { return berylliumCopper; };
		inline G4Material *Blackium() { return blackium; };
		inline G4Material *Copper() { return copper; };
		inline G4Material *Gadolinium() { return gadolinium; };
		inline G4Material *GasAr() { return gasAr; };
		inline G4Material *GasArN2() { return gasArN2; };
		inline G4Material *GasN2() { return gasN2; };
		inline G4Material *GasXe() { return gasXe; };
		inline G4Material *HighPressureGasXe() { return highPressureGasXe; };
		inline G4Material *Gold() { return gold; };
		inline G4Material *GS20Glass() { return gs20Glass; };
		inline G4Material *Halon() {return halon; };
		inline G4Material *Iron() { return iron; };
		inline G4Material *Isohexane() { return isohexane; };
		inline G4Material *DopedIsohexaneTMB5() { return dopedIsohexaneTMB5; };
		inline G4Material *DopedIsohexaneTMB50() { return dopedIsohexaneTMB50;};
		inline G4Material *DopedIsohexaneGd3() { return dopedIsohexaneGd3; };
		inline G4Material *DopedIsohexaneGd7() { return dopedIsohexaneGd7; };
		inline G4Material *LAB() { return lab; };
		inline G4Material *EJ301() { return ej301; };
		inline G4Material *EJ200() { return ej200; };
		inline G4Material *DopedLABGd() { return dopedLABGd; };
		inline G4Material *Kapton() { return kapton; };
		inline G4Material *KG2Glass() { return kg2Glass; };
		inline G4Material *Lead() { return lead; };
		inline G4Material *LeadGlass() { return leadGlass; };
		inline G4Material *LiquidAr() { return liquidAr; };
		inline G4Material *LiquidN2() { return liquidN2; };
		inline G4Material *LiquidXe() { return liquidXe; };
		inline G4Material *LiquidXeCold() { return liquidXeCold; };
		inline G4Material *GasXe15bar() { return gasXe15bar; };
		inline G4Material *GasXe50bar() { return gasXe50bar; };
		inline G4Material *MineralOil() { return mineralOil; };
		inline G4Material *Peek() { return peek; };
		inline G4Material *Polyethylene() { return polyethylene; };
		inline G4Material *PVC() { return pvc; };
		inline G4Material *PolyethyleneBorated() { return polyethyleneBorated;};
		inline G4Material *Quartz() { return quartz; };
		inline G4Material *Rock() { return rock; };
		inline G4Material *Sapphire() { return sapphire; };
        inline G4Material *Alumina() { return alumina; };
        inline G4Material *Elastomer() { return elastomer; };
        inline G4Material *Nicomic() { return nicomic; };
        inline G4Material *Inconel() { return inconel; };
        inline G4Material *Silicon() { return silicon; };
		inline G4Material *SS316() { return ss316; };
		inline G4Material *Steel() { return steel; };
		inline G4Material *Teflon() { return teflon; };
		inline G4Material *Titanium() { return titanium; };
		inline G4Material *TMB() { return tmb; };
		inline G4Material *TPB() { return tpb; };
		inline G4Material *Tungsten() { return tungsten; };
        inline G4Material *HeavyMet() { return heavyMet; };
		inline G4Material *Tyvek() { return tyvek; };
		inline G4Material *LZSpecRef() { return lzSpecRef; };
        inline G4Material *LZPMTCablingConduit() { return lzPMTCablingConduit; };
        inline G4Material *LZPMTbottomConduit() { return lzPMTBottomConduit; };
        inline G4Material *LZHVUmbilicalConduit() { return lzHVUmbilicalConduit; };
        inline G4Material *LZSiphonTube() { return lzSiphonTube; };
        inline G4Material *LZThermosyphonConduit() { return lzThermosyphonConduit; };
        inline G4Material *BNCCable() { return bncCable; };
		inline G4Material *Tyvek600() { return tyvek600; };
		inline G4Material *Vacuum() { return vacuum; };
		inline G4Material *Water() { return water; };
		inline G4Material *DopedWater3() {return dopedWater3; };
		inline G4Material *DopedWater7() {return dopedWater7; };
		inline G4Material *PolyFoam() {return polyFoam; };
        inline G4Material *CavRock() {return cavRock; };
        inline G4Material *PortlandConcrete() {return portlandConcrete; };
        inline G4Material *Kovar() {return kovar; };

		inline G4VisAttributes *AcrylicVis() { return acrylicVis; };
		inline G4VisAttributes *AirVis() { return airVis; };
		inline G4VisAttributes *AluminumVis() { return aluminumVis; };
		inline G4VisAttributes *BerylliumVis() { return berylliumVis; };
		inline G4VisAttributes *BeCuVis() { return berylliumCopperVis; };
		inline G4VisAttributes *BlackiumVis() { return blackiumVis; };
		inline G4VisAttributes *CopperVis() { return copperVis; };
		inline G4VisAttributes *GasArVis() { return gasArVis; };
		inline G4VisAttributes *GasArN2Vis() { return gasArN2Vis; };
		inline G4VisAttributes *GasN2Vis() { return gasN2Vis; };
		inline G4VisAttributes *GasXeVis() { return gasXeVis; };
		inline G4VisAttributes *HighPressureGasXeVis() { return highPressureGasXeVis; };
		inline G4VisAttributes *GoldVis() { return goldVis; };
		inline G4VisAttributes *IronVis() { return ironVis; };
		inline G4VisAttributes *IsohexaneVis() { return isohexaneVis; };
		inline G4VisAttributes *LABVis() { return labVis; };
		inline G4VisAttributes *EJ301Vis() { return ej301Vis; };
		inline G4VisAttributes *EJ200Vis() { return ej200Vis; };
		inline G4VisAttributes *KaptonVis() { return kaptonVis; };
		inline G4VisAttributes *LeadVis() { return leadVis; };
		inline G4VisAttributes *LeadGlassVis() { return leadGlassVis; };
		inline G4VisAttributes *LiquidArVis() { return liquidArVis; };
		inline G4VisAttributes *LiquidN2Vis() { return liquidN2Vis; };
		inline G4VisAttributes *LiquidXeVis() { return liquidXeVis; };
		inline G4VisAttributes *MineralOilVis() { return mineralOilVis; };
		inline G4VisAttributes *PeekVis() { return peekVis; };
		inline G4VisAttributes *PolyethyleneVis() { return polyethyleneVis; };
		inline G4VisAttributes *PolyethyleneBoratedVis()
				{ return polyethyleneBoratedVis; };
		inline G4VisAttributes *PVCVis() { return pvcVis; };
		inline G4VisAttributes *QuartzVis() { return quartzVis; };
		inline G4VisAttributes *RockVis() { return rockVis; };
		inline G4VisAttributes *SapphireVis() { return sapphireVis; };
        inline G4VisAttributes *AluminaVis() { return aluminaVis; };
        inline G4VisAttributes *ElastomerVis() { return elastomerVis; };
        inline G4VisAttributes *NicomicVis() { return nicomicVis; };
        inline G4VisAttributes *InconelVis() { return inconelVis; };
		inline G4VisAttributes *SteelVis() { return steelVis; };
		inline G4VisAttributes *TeflonVis() { return teflonVis; };
		inline G4VisAttributes *TitaniumVis() { return titaniumVis; };
		inline G4VisAttributes *TPBVis() { return tpbVis; };
		inline G4VisAttributes *TungstenVis() { return tungstenVis; };
		inline G4VisAttributes *VacuumVis() { return vacuumVis; };
		inline G4VisAttributes *WaterVis() { return waterVis; };
        inline G4VisAttributes *CavRockVis() { return cavRockVis; };
        inline G4VisAttributes *PortlandConcreteVis() { return portlandConcreteVis; };
        inline G4VisAttributes *KovarVis() { return kovarVis; };

        //--- added for geometry parsing project (tms)
        inline G4VisAttributes *TestRedVis() { return testRedVis; };
        inline G4VisAttributes *TestCopperVis() { return testCopperVis; };
        inline G4VisAttributes *TestBlueVis() { return testBlueVis; };
        inline G4VisAttributes *TestMagentaVis() { return testMagentaVis; };
        inline G4VisAttributes *TestGreenVis() { return testGreenVis; };
        inline G4VisAttributes *TestPurpleVis() { return testPurpleVis; };
        inline G4VisAttributes *TestDkTealVis() { return testDkTealVis; };
        
        
        //--- end geometry parsing code (tms)
    
		inline G4MaterialPropertiesTable *AcrylicTable() { return acrylicMat; };
		inline G4MaterialPropertiesTable *AirTable() { return airMat; };
		inline G4MaterialPropertiesTable *AluminumTable() {return aluminumMat;};
		inline G4MaterialPropertiesTable *BerylliumTable()
				{ return berylliumMat; };
		inline G4MaterialPropertiesTable *BeCuTable()
				{ return berylliumCopperMat; };
		inline G4MaterialPropertiesTable *BlackiumTable() {return blackiumMat;};
		inline G4MaterialPropertiesTable *CopperTable() { return copperMat; };
		inline G4MaterialPropertiesTable *GasArTable() { return gasArMat; };
		inline G4MaterialPropertiesTable *GasArN2Table() { return gasArN2Mat; };
		inline G4MaterialPropertiesTable *GasN2Table() { return gasN2Mat; };
		inline G4MaterialPropertiesTable *GasXeTable() { return gasXeMat; };
		inline G4MaterialPropertiesTable *HighPressureGasXeTable() { return highPressureGasXeMat; };
		inline G4MaterialPropertiesTable *GoldTable() {return goldMat; };
		inline G4MaterialPropertiesTable *HalonTable() {return halonMat; };
		inline G4MaterialPropertiesTable *IronTable() { return ironMat; };
		inline G4MaterialPropertiesTable *IsohexaneTable()
				{ return isohexaneMat; };
		inline G4MaterialPropertiesTable *LABTable()
				{ return labMat; };
		inline G4MaterialPropertiesTable *EJ301Table()
				{ return ej301Mat; };
		inline G4MaterialPropertiesTable *EJ200Table()
				{ return ej200Mat; };
		inline G4MaterialPropertiesTable *DopedLABGdTable() { return dopedLABGdMat; };
		inline G4MaterialPropertiesTable *KaptonTable() { return kaptonMat; };
		inline G4MaterialPropertiesTable *LeadTable() { return leadMat; };
		inline G4MaterialPropertiesTable *LeadGlassTable()
				{ return leadGlassMat; };
		inline G4MaterialPropertiesTable *LArTable() { return liquidArMat; };
		inline G4MaterialPropertiesTable *LN2Table() { return liquidN2Mat; };
		inline G4MaterialPropertiesTable *LXeTable() { return liquidXeMat; };
		inline G4MaterialPropertiesTable *MineralOilTable()
				{ return mineralOilMat; };
		inline G4MaterialPropertiesTable *PeekTable() { return peekMat; };
		inline G4MaterialPropertiesTable *PolyTable() {return polyethyleneMat;};
		inline G4MaterialPropertiesTable *PVCTable() {return pvcMat;};
		inline G4MaterialPropertiesTable *QuartzTable() { return quartzMat; };
		inline G4MaterialPropertiesTable *RockTable() { return rockMat; };
		inline G4MaterialPropertiesTable *SapphireTable() {return sapphireMat;};
        inline G4MaterialPropertiesTable *AluminaTable() {return aluminaMat;};
        inline G4MaterialPropertiesTable *ElastomerTable() {return elastomerMat;};
        inline G4MaterialPropertiesTable *NicomicTable() {return nicomicMat;};
        inline G4MaterialPropertiesTable *InconelTable() {return inconelMat;};
		inline G4MaterialPropertiesTable *SS316Table() { return ss316Mat; };
		inline G4MaterialPropertiesTable *SteelTable() { return steelMat; };
		inline G4MaterialPropertiesTable *TeflonTable() { return teflonMat; };
		inline G4MaterialPropertiesTable *TitaniumTable() {return titaniumMat;};
		inline G4MaterialPropertiesTable *TPBTable() { return tpbMat; };
		inline G4MaterialPropertiesTable *TungstenTable() {return tungstenMat;};
		inline G4MaterialPropertiesTable *HeavyMetTable() {return heavyMetMat;};
		inline G4MaterialPropertiesTable *TyvekTable() {return tyvekMat;};
		inline G4MaterialPropertiesTable *LZSpecRefTable() {return lzSpecRefMat;};
		inline G4MaterialPropertiesTable *Tyvek600Table() {return tyvek600Mat;};
		inline G4MaterialPropertiesTable *VacuumTable() { return vacuumMat; };
		inline G4MaterialPropertiesTable *WaterTable() { return waterMat; };
		inline G4MaterialPropertiesTable *PolyFoamTable() { return polyFoamMat; };
                inline G4MaterialPropertiesTable *CavRockTable() { return cavRockMat; };
                inline G4MaterialPropertiesTable *PortlandConcreteTable() { return portlandConcreteMat; };
		
		inline G4OpticalSurface *AlQuartzSurface()
				{ return aluminumQuartzSurface; };
		inline G4OpticalSurface *GXeCopperSurface()
				{ return gasXeCopperSurface; };
		inline G4OpticalSurface *LXeCopperSurface()
				{ return liquidXeCopperSurface; };
		inline G4OpticalSurface *GXeHDPESurface()
				{ return gasXeHDPESurface; };
		inline G4OpticalSurface *LXeHDPESurface()
				{ return liquidXeHDPESurface; };
		inline G4OpticalSurface *GXeTeflonSurface()
				{ return gasXeTeflonSurface; };
		inline G4OpticalSurface *GXeBlackSurface()
				{ return gasXeBlackSurface; };
		inline G4OpticalSurface *LXeTeflonSurface()
				{ return liquidXeTeflonSurface; };
		inline G4OpticalSurface *LXeBlackSurface()
				{ return liquidXeBlackSurface; };
        inline G4OpticalSurface *LXeTeflonSurfaceLiner()
				{ return liquidXeTeflonSurfaceLiner; };
        inline G4OpticalSurface *LXeTeflonSurfaceSkin()
				{ return liquidXeTeflonSurfaceSkin; };
		inline G4OpticalSurface *GXeSteelSurface()
				{ return gasXeSteelSurface; };
		inline G4OpticalSurface *LXeSteelSurface()
				{ return liquidXeSteelSurface; };
		inline G4OpticalSurface *GXeTiSurface()
				{ return gasXeTitaniumSurface; };
		inline G4OpticalSurface *LXeTiSurface()
				{ return liquidXeTitaniumSurface; };
		inline G4OpticalSurface *GArGoldSurface()
				{ return gasArN2GoldSurface; };
		inline G4OpticalSurface *GArAluminumSurface()
				{ return gasArN2AluminumSurface; };
		inline G4OpticalSurface *GArN2GoldSurface()
				{ return gasArN2GoldSurface; };
		inline G4OpticalSurface *GArN2AluminumSurface()
				{ return gasArN2AluminumSurface; };
		inline G4OpticalSurface *QuartzTeflonSurface()
				{ return quartzTeflonSurface; };
		
        inline G4OpticalSurface *LXeTeflonSurfacePMT()
                { return liquidXeTeflonSurfacePMT; };
        inline G4OpticalSurface *LXeTeflonSurfacePlate()
                { return liquidXeTeflonSurfacePlate; };
        inline G4OpticalSurface *LXeTeflonSurfaceCables()
                { return liquidXeTeflonSurfaceCables; };
        inline G4OpticalSurface *LXeTeflonSurfaceTruss()
                { return liquidXeTeflonSurfaceTruss; };
        inline G4OpticalSurface *LXeTeflonSurfaceBskinPMT()
                { return liquidXeTeflonSurfaceBskinPMT; };
        inline G4OpticalSurface *LXeTeflonSurfaceBplate()
                { return liquidXeTeflonSurfaceBplate; };
        inline G4OpticalSurface *LXeTeflonSurfaceTplate()
                { return liquidXeTeflonSurfaceTplate; };
        inline G4OpticalSurface *LXeTeflonSurfaceTskinPMT()
                { return liquidXeTeflonSurfaceTskinPMT; };
        inline G4OpticalSurface *LXeTeflonSurfaceWeir()
                { return liquidXeTeflonSurfaceWeir; };
        inline G4OpticalSurface *LXeTeflonSurfaceDomePMT()
                { return liquidXeTeflonSurfaceDomePMT; };

        void UpdateElectricField();
		void SetOpticalDebugging( G4bool );
		G4bool GetOpticalDebugging() { return opticalDebugging; };
		
		//	Materials methods
		void SetQuartzAbs( G4double l );
		void SetLXeTeflonRefl( G4double );
        void SetLXeTeflonLinerRefl(G4double);
    
		void SetLXeSteelRefl( G4double );
		void SetLXeAbs( G4double );
		void SetLXeRayleigh( G4double );
		void SetGXeTeflonRefl( G4double );
		void SetGXeBlackWallRefl( G4double );
		void SetGXeSteelRefl( G4double );
		void SetGXeAbs( G4double );
		void SetGXeRayleigh( G4double );
		void SetAlUnoxQuartzRefl( G4double );

        void SetLXeTeflonPMTRefl(G4double);
        void SetLXeTeflonPlateRefl(G4double);
        void SetLXeTeflonCablesRefl(G4double);
        void SetLXeTeflonTrussRefl(G4double);
        void SetLXeTeflonBskinPMTRefl(G4double);
        void SetLXeTeflonBplateRefl(G4double);
        void SetLXeTeflonTskinPMTRefl(G4double);
        void SetLXeTeflonWeirRefl(G4double);
        void SetLXeTeflonDomePMTRefl(G4double);

        void SetGasXeMat( G4MaterialPropertiesTable *mat ) { gasXeMat = mat; };
        void SetHighPressureGasXeMat( G4MaterialPropertiesTable *mat ) { highPressureGasXeMat = mat; };
        void SetLiquidXeMat( G4MaterialPropertiesTable *mat )
                { liquidXeMat = mat; };

	private:
		void DefineArrays();
		void CreateMaterials();
		void SetOpticalProperties();
		void CreateOpticalSurfaces();
		void CreateVisualAttributes();
		void InitializeMatrix( G4int, G4int* );
		void InitializeMatrix( G4int, G4double* );

	private:
		static BaccMaterials *baccMaterials;

		G4Material *acrylic;
		G4Material *air;		
		G4Material *aluminum;
		G4Material *beryllium;
		G4Material *berylliumCopper;
		G4Material *blackium;
		G4Material *copper;
		G4Material *gadolinium;
		G4Material *gasAr;
		G4Material *gasArN2;
		G4Material *gasN2;
		G4Material *gasXe;
		G4Material *highPressureGasXe;
		G4Material *gold;
		G4Material *gs20Glass;
		G4Material *halon;
		G4Material *iron;
		G4Material *isohexane;
		G4Material *dopedIsohexaneTMB5;
		G4Material *dopedIsohexaneTMB50;
		G4Material *dopedIsohexaneGd3;
		G4Material *dopedIsohexaneGd7;
		G4Material *lab;
		G4Material *ej301;
                G4Material *ej200;
		G4Material *dopedLABGd;
		G4Material *kapton;
		G4Material *kg2Glass;
		G4Material *lead;
		G4Material *leadGlass;
		G4Material *liquidAr;
		G4Material *liquidN2;
		G4Material *liquidXe;
		G4Material *liquidXeCold;
		G4Material *gasXe15bar;
		G4Material *gasXe50bar;
		G4Material *mineralOil;
		G4Material *peek;
		G4Material *polyethylene;
		G4Material *polyethyleneBorated;
		G4Material *pvc;
		G4Material *quartz;
		G4Material *rock;
		G4Material *sapphire;
        G4Material *silicon;
        G4Material *alumina;
        G4Material *elastomer;
        G4Material *nicomic;
        G4Material *inconel;
		G4Material *ss316;
		G4Material *steel;
		G4Material *teflon;
		G4Material *titanium;
		G4Material *tmb;
		G4Material *tpb;
		G4Material *tungsten;
        G4Material *heavyMet;
		G4Material *tyvek;
		G4Material *lzSpecRef;
		G4Material *bncCable;
		G4Material *lzPMTCablingConduit;
		G4Material *lzPMTBottomConduit;
		G4Material *lzHVUmbilicalConduit;
		G4Material *lzSiphonTube;
		G4Material *lzThermosyphonConduit;
		G4Material *tyvek600;
		G4Material *vacuum;
		G4Material *water;
		G4Material *dopedWater3;
		G4Material *dopedWater7;
		G4Material *polyFoam;		
        G4Material *cavRock;
        G4Material *portlandConcrete;
        G4Material *kovar;

		G4VisAttributes *acrylicVis;
		G4VisAttributes *airVis;
		G4VisAttributes *aluminumVis;
		G4VisAttributes *berylliumVis;
		G4VisAttributes *berylliumCopperVis;
		G4VisAttributes *blackiumVis;
		G4VisAttributes *copperVis;
		G4VisAttributes *gasArVis;
		G4VisAttributes *gasArN2Vis;
		G4VisAttributes *gasN2Vis;
		G4VisAttributes *gasXeVis;
		G4VisAttributes *highPressureGasXeVis;
		G4VisAttributes *goldVis;
		G4VisAttributes *gs20GlassVis;
		G4VisAttributes *ironVis;
		G4VisAttributes *isohexaneVis;
		G4VisAttributes *labVis;
		G4VisAttributes *ej301Vis;
                G4VisAttributes *ej200Vis;
		G4VisAttributes *kaptonVis;
		G4VisAttributes *kg2GlassVis;
		G4VisAttributes *leadVis;
		G4VisAttributes *leadGlassVis;
		G4VisAttributes *liquidArVis;
		G4VisAttributes *liquidN2Vis;
		G4VisAttributes *liquidXeVis;
		G4VisAttributes *mineralOilVis;
		G4VisAttributes *peekVis;
		G4VisAttributes *polyethyleneVis;
		G4VisAttributes *polyethyleneBoratedVis;
		G4VisAttributes *pvcVis;
		G4VisAttributes *quartzVis;
		G4VisAttributes *rockVis;
		G4VisAttributes *sapphireVis;
        G4VisAttributes *aluminaVis;
        G4VisAttributes *elastomerVis;
        G4VisAttributes *nicomicVis;
        G4VisAttributes *inconelVis;
		G4VisAttributes *steelVis;
		G4VisAttributes *teflonVis;
		G4VisAttributes *titaniumVis;
		G4VisAttributes *tpbVis;
		G4VisAttributes *tungstenVis;
		G4VisAttributes *vacuumVis;
		G4VisAttributes *waterVis;
        G4VisAttributes *cavRockVis;
        G4VisAttributes *portlandConcreteVis;
        G4VisAttributes *kovarVis;

        //--- added for geometry parsing project (tms)
        G4VisAttributes *testRedVis;
        G4VisAttributes *testCopperVis;
        G4VisAttributes *testBlueVis;
        G4VisAttributes *testMagentaVis;
        G4VisAttributes *testGreenVis;
        G4VisAttributes *testPurpleVis;
        G4VisAttributes *testDkTealVis;
        //--- end geometry parsing code (tms)
    
        G4MaterialPropertiesTable *acrylicMat;
		G4MaterialPropertiesTable *airMat;
		G4MaterialPropertiesTable *aluminumMat;
		G4MaterialPropertiesTable *berylliumMat;
		G4MaterialPropertiesTable *berylliumCopperMat;
		G4MaterialPropertiesTable *blackiumMat;
		G4MaterialPropertiesTable *copperMat;
		G4MaterialPropertiesTable *gasArMat;
		G4MaterialPropertiesTable *gasArN2Mat;
		G4MaterialPropertiesTable *gasN2Mat;
		G4MaterialPropertiesTable *gasXeMat;
		G4MaterialPropertiesTable *highPressureGasXeMat;
		G4MaterialPropertiesTable *goldMat;
		G4MaterialPropertiesTable *gs20GlassMat;
		G4MaterialPropertiesTable *halonMat;
		G4MaterialPropertiesTable *ironMat;
		G4MaterialPropertiesTable *isohexaneMat;
		G4MaterialPropertiesTable *labMat;
		G4MaterialPropertiesTable *ej301Mat;
		G4MaterialPropertiesTable *ej200Mat;
		G4MaterialPropertiesTable *dopedLABGdMat;
		G4MaterialPropertiesTable *kaptonMat;
		G4MaterialPropertiesTable *kg2GlassMat;
		G4MaterialPropertiesTable *leadMat;
		G4MaterialPropertiesTable *leadGlassMat;
		G4MaterialPropertiesTable *liquidArMat;
		G4MaterialPropertiesTable *liquidN2Mat;
		G4MaterialPropertiesTable *liquidXeMat;
		G4MaterialPropertiesTable *mineralOilMat;
		G4MaterialPropertiesTable *peekMat;
		G4MaterialPropertiesTable *polyethyleneMat;
		G4MaterialPropertiesTable *pvcMat;
		G4MaterialPropertiesTable *quartzMat;
		G4MaterialPropertiesTable *rockMat;
		G4MaterialPropertiesTable *sapphireMat;
        G4MaterialPropertiesTable *aluminaMat;
        G4MaterialPropertiesTable *elastomerMat;
        G4MaterialPropertiesTable *nicomicMat;
        G4MaterialPropertiesTable *inconelMat;
		G4MaterialPropertiesTable *ss316Mat;
		G4MaterialPropertiesTable *steelMat;
		G4MaterialPropertiesTable *teflonMat;
		G4MaterialPropertiesTable *titaniumMat;
		G4MaterialPropertiesTable *tpbMat;
		G4MaterialPropertiesTable *tungstenMat;
        G4MaterialPropertiesTable *heavyMetMat;
		G4MaterialPropertiesTable *tyvekMat;
		G4MaterialPropertiesTable *lzSpecRefMat;
		G4MaterialPropertiesTable *tyvek600Mat;
		G4MaterialPropertiesTable *vacuumMat;
		G4MaterialPropertiesTable *waterMat;
		G4MaterialPropertiesTable *polyFoamMat;
        G4MaterialPropertiesTable *cavRockMat;
        G4MaterialPropertiesTable *portlandConcreteMat;

		G4OpticalSurface *aluminumQuartzSurface;
		G4OpticalSurface *gasXeCopperSurface;
		G4OpticalSurface *liquidXeCopperSurface;
		G4OpticalSurface *gasXeHDPESurface;
		G4OpticalSurface *liquidXeHDPESurface;
		G4OpticalSurface *gasXeTeflonSurface;
		G4OpticalSurface *gasXeBlackSurface;
		G4OpticalSurface *liquidXeTeflonSurface;
		G4OpticalSurface *liquidXeBlackSurface;
        G4OpticalSurface *liquidXeTeflonSurfaceLiner;
        G4OpticalSurface *liquidXeTeflonSurfaceSkin;
		G4OpticalSurface *gasXeSteelSurface;
		G4OpticalSurface *liquidXeSteelSurface;
		G4OpticalSurface *gasXeTitaniumSurface;
		G4OpticalSurface *liquidXeTitaniumSurface;
		G4OpticalSurface *gasArGoldSurface;
		G4OpticalSurface *gasArAluminumSurface;
		G4OpticalSurface *gasArN2GoldSurface;
		G4OpticalSurface *gasArN2AluminumSurface;
		G4OpticalSurface *quartzTeflonSurface;
		
        G4OpticalSurface *liquidXeTeflonSurfacePMT;
        G4OpticalSurface *liquidXeTeflonSurfacePlate;
        G4OpticalSurface *liquidXeTeflonSurfaceCables;
        G4OpticalSurface *liquidXeTeflonSurfaceTruss;
        G4OpticalSurface *liquidXeTeflonSurfaceBskinPMT;
        G4OpticalSurface *liquidXeTeflonSurfaceBplate;
        G4OpticalSurface *liquidXeTeflonSurfaceTplate;
        G4OpticalSurface *liquidXeTeflonSurfaceTskinPMT;
        G4OpticalSurface *liquidXeTeflonSurfaceWeir;
        G4OpticalSurface *liquidXeTeflonSurfaceDomePMT;

        //	These arrays are used to hold the optical parameters of the various
		//	materials.
		G4bool opticalDebugging;
		
		G4double *photonWavelengths;
		G4double *photonEnergies;
		G4double *photonEnergies_ConstProp;
		
		G4double *optDebugRefIndex;
        G4double *optDebugGroupVel;
		G4double *optDebugReflection;
		G4double *optDebugAbsLength;
		G4double *optDebugRayleighLength;
		
		G4int num_pp;
		G4int num_pp_constProp;
		G4int num_water;
		G4int num_tpbAbs;
		G4int num_tpbEmi;
		G4int num_labAbs;
		G4int num_labEmi;
        G4int num_lab_Rayleigh;
        G4int num_acrylicTransmittance;

		G4double *acrylicRefractiveIndex;
		G4double *airRefractiveIndex;
		G4double *aluminumRefl;
		G4double *aluminumUnoxidizedRefl;
		G4double *berylliumRefl;
		G4double *berylliumCopperRefl;
		G4double *copperRefl;
		G4double *gasArRindex;
		G4double *gasArAbslength;
		G4double *gasArN2Rindex;
		G4double *gasArN2Abslength;
		G4double *gasN2Rindex;
		G4double *gasN2Abslength;
		G4double *gasXeScint;
		G4double *gasXeRindex;
		G4double *gasXeAbslength;
		G4double *goldRefl;
		G4double *ironRefl;
		G4double *isohexaneRindex;
		G4double *labRindex;
		G4double *leadRefl;
		G4double *leadGlassRefractiveIndex;
		G4double *leadGlassGroupVel;
		G4double *leadGlassAbsorption;
		G4double *liquidArScint;
		G4double *liquidArRindex;
		G4double *liquidArAbslength;
		G4double *liquidA4Rayleigh;
		G4double *liquidN2Scint;
		G4double *liquidN2Rindex;
		G4double *liquidN2Abslength;
		G4double *liquidN2Rayleigh;
		G4double *liquidXeScint;
		G4double *liquidXeRindex;
        G4double *liquidXeGroupVel;
		G4double *liquidXeAbslength;
		G4double *liquidXeRayleigh;
		G4double *mineralOilRIndex;
		G4double *peekRefl;
		G4double *polyethyleneRefl;
		G4double *polyethyleneBoratedRefl;
		G4double *pvcRefl;
		G4double *quartzRefractiveIndex;
		G4double *quartzGroupVel;
		G4double *quartzAbsorption;
		G4double *quartzConstAbsorption;
        G4double *teflonAbsorption;
		G4double *rockRefl;
		G4double *sapphireRefractiveIndex;
		G4double *sapphireGroupVel;
        G4double *teflonRefractiveIndex;
        G4double *teflonGroupVel;
		G4double *sapphireAbsorption;
		G4double *ss316Refl;
		G4double *steelRefl;
		G4double *teflonRefl;
		G4double *titaniumRefl;
		G4double *tpbAbsorptionEnergies;
		G4double *tpbAbsorption;
		G4double *tpbEmissionEnergies;
		G4double *tpbEmission;
		G4double *labAbsorptionEnergies;
		G4double *ej301AbsorptionEnergies;
		G4double *labAbsorption;
		G4double *ej301Absorption;
		G4double *tempLABAbsLength;//temp
		G4double *tempej301AbsLength;//temp
		G4double *tempLABAbsEnergies;//temp
		G4double *labEmissionEnergies;
		G4double *labEmission;
        G4double *lab_Rayleigh_length;
        G4double *lab_Rayleigh_energy;
        G4double *acrylicTransmittance_energies;
        G4double *acrylicTransmittance;
        G4double *acrylicReflectivity;
		G4double *tungstenRefl;
		G4double *waterPhotonEnergies;	//	Note that water gets its own
		G4double *waterRefractiveIndex;	//	energies matrix.
        G4double *waterGroupVel;
		G4double *waterAbsorption;
		G4double *halonRefl;
		G4double *tyvekRefl;
		G4double *lzSpecRefRefl;
		G4double *tyvek600Refl;
		
		//	Support for internal workings
		BaccManager *baccManager;
};

#endif
