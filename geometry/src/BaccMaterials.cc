////////////////////////////////////////////////////////////////////////////////
/*	BaccMaterials.cc
 *
 * This is the code file to contain all the material definitions in BACCARAT.
 * These definitions include optical properties.
 *
 ********************************************************************************
 * Change log
 *	13 Mar 2009 - Initial submission (Kareem)
 *	25 Mar 2009 - Added GasXe, Polyethylene, and optical properties (Dave M.)
 *	26 May 2009 - Added alpha values to visualization attributes and took out
 *			      SetForceWireframe so that RayTracer will produce better
 *			      pictures (Melinda)
 *	25 Oct 2009 - Removed refractive indices from non-relevant materials
 *			      (Dave M.)
 *	13 Dec 2009 - Moved material properties tables to the header, and removed
 *				  material properties from the opaque materials (Kareem)
 *	09 Feb 2010 - Added the material "air" (Kareem)
 *	10 Feb 2010 - Added the material "leadGlass" and changed the optical
 *				  properties of quartz (Kareem)
 *	12 Feb 2010 - Changed the optical properties of lead glass to be very
 *				  similar to those of quartz. Also expanded the bounds of the
 *				  wavelengths for air to cover both scintillation and Cerenkov
 *				  light. (Kareem)
 *	18 Feb 2010 - Major rewrite of the optical properties. Now all properties
 *				  span from 176 nm to 2058 nm. (Kareem)
 *	19 Feb 2010 - Expanded the index of refraction for liquid xenon to more
 *				  realistic values in the full span of defined wavelengths
 *				  (Kareem)
 *	09 Mar 2010 - Added a few isotopes and elements for possible future use
 *				  (Kareem)
 *	18 Mar 2010 - Fixed bugs in the definitions of isotopes for Li, Al, S, and
 *				  Zn (Kareem)
 *	22 Mar 2010 - Fixed a bug in defining the index of refraction for liquid Xe
 *				  (Kareem)
 *	19 May 2010 - Added doped water materials (Melinda)
 *	15 Jun 2010 - Added halon material (Melinda)
 *	19 Jun 2010 - Added blackium (Kareem)
 *	23 Jun 2010 - Fixed energy bounds of halon/doped water (Melinda)
 *	29 Jun 2010 - Fixed a bug where I was trying to delete the same material
 *				  properties table twice in the destructor (Kareem)
 *	2 July 2010 - Adjusted the reflectivity of all metals to a maximum of 10%,
 *				  unless they were already set to something else (Kareem)
 *	20 Sep 2010 - Added a hook to, and registration with, the LUXSimManager
 *				  (Kareem)
 *	20 Sep 2010 - Added a switch to use simplified optical parameters for
 *				  debugging purposes (Kareem)
 *	27 Sep 2010 - Fixed a bug in the array size definitions, as well as a
 *				  repeated index in the photonWavelengths array (Kareem)
 *	16 Nov 2010 - Implementing optical surface definitions in LUuidXSimMaterials
 *				  class (Dave)
 *	24 Nov 2010 - Added gas Ar, liquid Ar, gas N2, liquid N2. As of this
 *				  submission, these materials do NOT have optical properties
 *				  built in...no scintillation, no scattering, no absorption, no
 *				  index of refraction. The variables exist in the header file
 *				  contain all this information, it's just not present yet.
 *				  PLEASE NOTE: right now, the optical properties have an lower
 *				  wavelength limit set by xenon scintillation (178 nm). If argon
 *				  scintillation is put in, ALL other optical properties have to
 *				  be extended down to 128 nm. (Kareem)
 *	24 Nov 2010 - Added gold. Optical properties are the same as copper (Kareem)
 *	25 Nov 2010 - Updated the natural gadolinium definition to be the same as
 *				  the other elements (Kareem)
 *	17 Dec 2010 - Added a "gasArN2" material definition and simple optical
 *				  properties for use in the LLNL single-phase detector (Kareem)
 *	21 Dec 2010 - Created G4OpticalSurfaces for the interfaces between gas or
 *				  liquid xenon and stainless steel (Kareem)
 *	16 Feb 2011 - Added beryllium and sapphire (Kareem)
 *	04 Apr 2011 - Added optical surface for unoxidized Al / quartz for PMT
 *				  flashing (Dave)
 *	31 Mar 2011 - Altered the gas density to reflect the experimental
 *				  conditions, and added gas Ar / Au and gas Ar / Al optical
 *				  boundaries (Kareem)
 *	19 Apr 2011 - Fixed a couple optical parameter bugs in the liquid xenon.
 *				  (scintillation light was being produced only at 176 nm instead
 *				  of a spread from 176 to 179 nm) (Kareem)
 *	21 Jul 2011 - Added optical surface for quartz-PTFE interface (Dave)
 *	24 Aug 2011 - Added material PEEK (Tenzing)
 *   13 Sep 2011 - Added material properties to LXe for NEST functionality
 *				  (Matthew)
 *	23 Sep 2011 - Added material borated polyethelyene and natural boron
 *				  (Tenzing)
 *   27 Sep 2011 - Added material SS316 (Tenzing)
 *   21 Sep 2011 - Added material properties to LXe for NEST functionality
 *				  (Matthew)
 *   05 Dec 2011 - Added additional wavelength to list for LXe index (Matthew)
 *   13 Dec 2011 - Refractive index additions to match up with last change
 *				  (Matthew)
 *	18 Jan 2012 - Added methods for changing materials properties (Dave)
 *	06 Feb 2012 - Changed the tungsten visualization default to wireframe
 *				  (Kareem)
 *	14 Feb 2012 - Moved LXe/GXe scint. properties to physicslist (Matthew)
 *	16 Feb 2012 - Changed the vacuum absorption length for all wavelengths to
 *				  1e20 meters (it was previously 0, which absolutely killed all
 *				  light propagation in a vacuum) (Kareem)
 *	21 Feb 2012 - Added temperature to quartz for QE purposes (Matthew)
 *	24 Feb 2012 - Added a generic mineral oil, but without scintillation
 *				  properties (Kareem)
 *	25 Feb 2012 - Added isohexane, but without scintillation properties (Kareem)
 *   06 Mar 2012 - Added GS-20 and KG-2 enriched 6Li doped glass (Tenzing)
 *   07 Mar 2012 - Cleaned up opt/mat prop based on surface data (Matthew)
 *	09 Mar 2012 - Added support for Rayleigh UI hooks (Dave)
 * 	23 Mar 2012 - Fixed mistaken reference to LXe in GXe abs. length function,
 *				  and removed some vestigial variables and comments. (Matthew)
 *	26 Mar 2012 - Added UI feedback when materials properties are set through
 *				  UI hooks (Dave)
 *	26 Mar 2012 - Added several materials (various types of doped isohexane) to
 *				  class, for LZ studies (Dave)
 *	14 Apr 2012 - Added TPB with WLS absorption / emission, scattering, and
 *				  absorption-to-kill length (Kareem)
 *	16 Apr 2012 - Changed grid and HDPE reflectivities to defaults of 10%
 *				  (Matthew)
 *	21 May 2012 - Fixed typo that lead to array being accessed out of bounds
 *				  (Rich)
 *	28 Jun 2012 - Increased accuracy for the refractive index of liquid xenon
 *				  (Matthew)
 *	11 Jul 2012 - Changed temperature to 178 K to reflect Run 02 stable mode
 *				  (Matthew)
 *	12 Jul 2012 - Electric field support by region as mat'l properties
 *				  (Nick+Matt)
 *	11 Sep 2012 - Made default steel 304 SS since the grids are made of it
 *				  (Matthew)
 *	19 Oct 2012 - Added index of refraction of PTFE (Matthew)
 *   02 Nov 2012 - Made quartz absorption length huge and added lambda-dep. for
 *                 PTFE n (Matthew)
 *   08 Nov 2012 - LXe Rayl mfp now function of wavelength and LXe, GXe densities
 *                 now correct for the LUX detector temperature (Matthew)
 *   21 Nov 2012 - Based on Claudio's thesis, the reflectivity of PTFE in GXe is
 *                 now a function of wavelength. (Matthew)
 *   25 Nov 2012 - De-convolved phe conv probability from the quartz absorption
 *                 length, using Hamamatsu data. (Matthew)
 *   30 Dec 2012 - Added support for lower wavelengths, down to 110 nm. (Matthew)
 *   29 Jan 2013 - Changed quartz absorption lengths. Not enough absorption
 *                 (Matthew)
 *   20 Feb 2013 - Made pressure universal at top like temp. (Matthew)
 *   18 Apr 2013 - Updated P, T, density hard-coded for Run 03 detector (Matthew)
 *
 *   11 Jun 2013 - Updated refl/abs optical parameters for Run03 (Matthew)
 *
 *   18 Jul 2013 - Added PVC for the neutron collimator tube (Kevin)
 *
 *   21 Mar 2014 - Added linear akyl benze (LAB) and LAB doped with .1% Gd by
 *                                 mass (DopedLABGd) (Kevin)
 *	03 Apr 2014 - Added tyvek for water tank. Optical properties copied from halon
 *	              example. Wavelength-dependent diffuse reflectivity added. (Curt)
 *	29 Apr 2014 - Added LZ veto tyvek (600 micron thickness). Fixed LAB index of
 *		          refraction bug (Curt)
 *   30 Apr 2014 - Added material table for dopedLABGd including abs. data.
 *		          Also added material for specular reflectors in LZ. (Scott Ha.)
 *	13 Oct 2014 - Corrected LAB and dopedLABGd compositions to give correct number
 *		          fractions of carbon, hydrogen and Gd.
 *   08 Jan 2015 - Added group velocities for all materials with a non-constant
 *                 index of refraction (Kareem)
 *   06 Feb 2015 - Added materials for LZ conduits: bncCable, lzPMTCablingConduit and
 *	         lzThermosyphonConduit. (Scott Ha.)
 *   12 Feb 2015 - Added "TOTALNUM_INT_SITES" parameter to GdLS for use with NEST (Scott Ha.)
 *
 *   02 Apr 2015 - Updating optical parameters with best fit values from simulation to calibration
 *                 comparisons.  Changed: gasXeAbslength, liquidXeAbslength, gasXeTeflonRefl,
 *                 liquidXeTeflonRefl, gasXeSteelRefl.  These may get updated again...  (Vic)
 *   15 Jul 2015 - Added cavern rock material (David W)
 *
 *   08 Oct 2015 - Added kovar (nickel–cobalt ferrous alloy), needed for LZ PMTs
 *                 cylinders (Maria Francesca M.)
 *   16 Oct 2015 - Modified averaged material density for top PMT conduit, bottom PMT conduit,
 *                 HV Umbilical, two thermo siphon conduit/tube and PMT cable (Joseph Hor)
 
 15 Jul 2016 - Added in additional surfaces related to the skin:
 
 LXeTeflonSurfaceLiner()
 Used to better control modeling of cracks in PTFE liner
 LXeTeflonSurfacePMT()
 Used to better control modeling of cracks in TPC PMT coverings
 LXeTeflonSurfacePlate()
 Used to control PTFE coverage of underside of TPC PMT support plate
 LXeTeflonSurfaceCables()
 Used to set reflectivity of cables, which are coated in PTFE
 G4OpticalSurface *LXeTeflonSurfaceTruss()
 Used to control the coverage of PTFE on the bottom truss
 G4OpticalSurface *LXeTeflonSurfaceBskinPMT()
 Used to control coverage of PTFE on bottom skin PMTs
 G4OpticalSurface *LXeTeflonSurfaceTskinPMT()
 Used to control coverage of PTFE on top skin PMTs
 G4OpticalSurface *LXeTeflonSurfaceWeir()
 Used to control reflectivity of weir (if implemented)
 G4OpticalSurface *LXeTeflonSurfaceDomePMT()
 Used to control coverage of PTFE on Dome skin PMTs
 
 
 
 */
////////////////////////////////////////////////////////////////////////////////

#define TEMPERATURE 173.*kelvin //Sets the T of the LXe,GXe,quartz,blackium,
//vacuum. T was 178 K for Run 02
#define PRESSURE 1.57*bar //~2 bar in Run02, 2.854 g/cm^3, 18.7E-3 g/cm^3

#define N_l 2.888*g/cm3	//	NIST LXe density (used 2 places because needed to
//	set refr. index and Rayl.)
#define N_g 15e-3*g/cm3	//	NIST GXe density for 1.57 bar and 173.1 K (Run03)

//
//	GEANT4 includes
//
#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

//
//	BACCARAT includes
//
#include "BaccMaterials.hh"
#include "BaccManager.hh"

G4double constC = 299792458.;

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccMaterials()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccMaterials *BaccMaterials::baccMaterials = 0;
BaccMaterials::BaccMaterials()
{
    baccManager = BaccManager::GetManager();
    baccManager->Register( this );

    
    DefineArrays();
    CreateMaterials();
    SetOpticalProperties();
    CreateOpticalSurfaces();
    SetOpticalDebugging( false );
    CreateVisualAttributes();
    
    baccMaterials = this;

    
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccMaterials()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccMaterials::~BaccMaterials()
{
    delete acrylic;
    delete air;
    delete aluminum;
    delete beryllium;
    delete berylliumCopper;
    delete blackium;
    delete copper;
    delete gasAr;
    delete gasArN2;
    delete gasN2;
    delete gasXe;
    delete highPressureGasXe;
    delete gold;
    delete gs20Glass;
    delete halon;
    delete iron;
    delete isohexane;
    delete dopedIsohexaneTMB5;
    delete dopedIsohexaneTMB50;
    delete dopedIsohexaneGd3;
    delete dopedIsohexaneGd7;
    delete lab;
    delete ej301;
    delete ej200;
    delete dopedLABGd;
    delete polystyrene;
    delete enrLiMaterial;
    delete liDopedPlasticScintillator;
    delete kapton;
    delete kg2Glass;
    delete lead;
    delete leadGlass;
    delete liquidAr;
    delete liquidN2;
    delete liquidXe;
    delete liquidXeCold;
    delete gasXe15bar;
    delete gasXe50bar;
    delete mineralOil;
    delete peek;
    delete polyethylene;
    delete polyethyleneBorated;
    delete pvc;
    delete quartz;
    delete rock;
    delete sapphire;
    delete alumina;
    delete elastomer;
    delete nicomic;
    delete inconel;
    delete ss316;
    delete steel;
    delete teflon;
    delete titanium;
    delete tmb;
    delete tpb;
    delete tungsten;
    delete heavyMet;
    delete tyvek;
    delete lzSpecRef;
    delete lzPMTCablingConduit;
    delete lzPMTBottomConduit;
    delete lzHVUmbilicalConduit;
    delete lzSiphonTube;
    delete lzThermosyphonConduit;
    delete bncCable;
    delete tyvek600;
    delete vacuum;
    delete water;
    delete dopedWater3;
    delete dopedWater7;
    delete cavRock;
    delete kovar;
    delete portlandConcrete;
    
    delete acrylicVis;
    delete airVis;
    delete aluminumVis;
    delete berylliumVis;
    delete berylliumCopperVis;
    delete blackiumVis;
    delete copperVis;
    delete gasArVis;
    delete gasArN2Vis;
    delete gasN2Vis;
    delete gasXeVis;
    delete highPressureGasXeVis;
    delete goldVis;
    delete gs20GlassVis;
    delete ironVis;
    delete isohexaneVis;
    delete labVis;
    delete ej301Vis;
    delete ej200Vis;
    delete kaptonVis;
    delete kg2GlassVis;
    delete leadVis;
    delete leadGlassVis;
    delete liquidArVis;
    delete liquidN2Vis;
    delete liquidXeVis;
    delete mineralOilVis;
    delete peekVis;
    delete polyethyleneVis;
    delete polyethyleneBoratedVis;
    delete pvcVis;
    delete quartzVis;
    delete rockVis;
    delete sapphireVis;
    delete aluminaVis;
    delete elastomerVis;
    delete nicomicVis;
    delete inconelVis;
    delete steelVis;
    delete teflonVis;
    delete titaniumVis;
    delete tpbVis;
    delete tungstenVis;
    delete vacuumVis;
    delete waterVis;
    delete cavRockVis;
    delete kovarVis;
    // added for geometry breakup
    delete testRedVis;
    delete testCopperVis;
    delete testBlueVis;
    delete testMagentaVis;
    delete testGreenVis;
    delete testPurpleVis;
    delete testDkTealVis;
    delete portlandConcreteVis; 
    
    //
    
    
    delete acrylicMat;
    delete airMat;
    delete aluminumMat;
    delete berylliumMat;
    delete berylliumCopperMat;
    delete copperMat;
    delete gasArMat;
    delete gasArN2Mat;
    delete gasN2Mat;
    delete gasXeMat;
    delete highPressureGasXeMat;
    delete goldMat;
    delete gs20GlassMat;
    delete halonMat;
    delete ironMat;
    delete isohexaneMat;
    delete labMat;
    delete ej301Mat;
    delete ej200Mat;
    delete kaptonMat;
    delete kg2GlassMat;
    delete leadMat;
    delete leadGlassMat;
    delete liquidArMat;
    delete liquidN2Mat;
    delete liquidXeMat;
    
    delete mineralOilMat;
    delete peekMat;
    delete polyethyleneMat;
    delete pvcMat;
    delete quartzMat;
    delete rockMat;
    delete sapphireMat;
    delete aluminaMat;
    delete elastomerMat;
    delete nicomicMat;
    delete inconelMat;
    delete ss316Mat;
    delete steelMat;
    delete teflonMat;
    delete titaniumMat;
    delete tpbMat;
    delete tungstenMat;
    delete heavyMetMat;
    delete tyvekMat;
    delete lzSpecRefMat;
    delete tyvek600Mat;
    delete vacuumMat;
    delete waterMat;
    delete cavRockMat;
    delete portlandConcreteMat;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetMaterials()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccMaterials *BaccMaterials::GetMaterials()
{
    return baccMaterials;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					DefineArrays()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::DefineArrays()
{
    //	This is a method for defining the arrays that are used in setting the
    //	optical parameters in SetOpticalProperties.
    
    //	Note: While xenon only scintillates in the ~170-180 nm range, optical
    //	properties are defined for the entire range of wavelengths so as to
    //	avoid the dreaded "G4Excpetion: OutOfRange" error that otherwise
    //	plagues the output.
    
    //	Note: 175.8641 nm = 7.05 eV
    //		  177.6278 nm = 6.98 eV
    //		  179.4272 nm = 6.91 eV
    
    
    num_pp = 23;
    num_pp_constProp = 2;
    
    const G4int NUM_PP = num_pp;
    const G4int NUM_PP_ConstProp = num_pp_constProp;
    
    photonWavelengths = new G4double[NUM_PP];
    photonWavelengths[0] = 110.0;
    photonWavelengths[1] = 144.5;
    photonWavelengths[2] = 175.8641;
    photonWavelengths[3] = 177.6278;
    photonWavelengths[4] = 179.4272;
    photonWavelengths[5] = 193.6;
    photonWavelengths[6] = 250.3;
    photonWavelengths[7] = 303.4;
    photonWavelengths[8] = 340.4;
    photonWavelengths[9] = 410.2;
    photonWavelengths[10] = 467.8;
    photonWavelengths[11] = 508.6;
    photonWavelengths[12] = 546.1;
    photonWavelengths[13] = 627.8;
    photonWavelengths[14] = 706.5;
    photonWavelengths[15] = 766.5;
    photonWavelengths[16] = 844.7;
    photonWavelengths[17] = 1000.0;
    photonWavelengths[18] = 1300.0;
    photonWavelengths[19] = 1529.6;
    photonWavelengths[20] = 1600.0;
    photonWavelengths[21] = 1800.0;
    photonWavelengths[22] = 2058.2;
    photonEnergies = new G4double[NUM_PP];
    for( G4int i=0; i<NUM_PP; i++ )
        photonEnergies[i] =
        (4.13566743E-15*constC/(photonWavelengths[i]*1.E-9))*eV;
    
    optDebugRefIndex = new G4double[NUM_PP_ConstProp];
    optDebugRefIndex[0] = optDebugRefIndex[1] = 1.;
    optDebugGroupVel = new G4double[NUM_PP_ConstProp];
    optDebugGroupVel[0] = optDebugGroupVel[1] = constC*m/s;
    optDebugReflection = new G4double[NUM_PP_ConstProp];
    optDebugReflection[0] = optDebugReflection[1] = 0;
    optDebugAbsLength = new G4double[NUM_PP_ConstProp];
    optDebugAbsLength[0] = optDebugAbsLength[1] = 1000.*km;
    optDebugRayleighLength = new G4double[NUM_PP_ConstProp];
    optDebugRayleighLength[0] = optDebugRayleighLength[1] = 1000.*km;
    
    // Taking average refractive index from
    //	http://en.wikipedia.org/wiki/Poly(methyl_methacrylate)
    acrylicRefractiveIndex = new G4double[NUM_PP_ConstProp];
    acrylicRefractiveIndex[0] = acrylicRefractiveIndex[1] = 1.4896;
    // Acrylic Transmittance Data from Minfang (2 inch Nakano UVT) Note that our acrylic is 1 inch thick
    G4double tmp_acrylicTransmittance [] = {0.000549 , 0.000549 , 0.000244 , 0.000244 , 4.6e-05 , 6.1e-05 , 0.000183 , 3.1e-05 , 6.1e-05 , 0 ,
        6.1e-05 , 6.1e-05 , 6.1e-05 , 0 , 0 , 3.1e-05 , 0 , 0.000168 , 9.2e-05 , 4.6e-05 ,
        6.1e-05 , 3.1e-05 , 0 , 4.6e-05 , 6.1e-05 , 0 , 7.6e-05 , 9.2e-05 , 0.000122 , 0.000122 ,
        0 , 0 , 1.5e-05 , 1.5e-05 , 0 , 3.1e-05 , 1.5e-05 , 6.1e-05 , 7.6e-05 , 4.6e-05 ,
        0 , 6.1e-05 , 6.1e-05 , 4.6e-05 , 4.6e-05 , 4.6e-05 , 0 , 3.1e-05 , 6.1e-05 , 9.2e-05 ,
        7.6e-05 , 3.1e-05 , 9.2e-05 , 7.6e-05 , 6.1e-05 , 9.2e-05 , 9.2e-05 , 9.2e-05 , 7.6e-05 , 0.000107 ,
        9.2e-05 , 6.1e-05 , 9.2e-05 , 0.000153 , 0.000168 , 9.2e-05 , 0.000107 , 0.000122 , 0.000183 , 0.000153 ,
        9.2e-05 , 0.000107 , 0.000198 , 0.000168 , 9.2e-05 , 0.000183 , 0.000183 , 0.000168 , 0.000137 , 0.000198 ,
        0.000153 , 0.000183 , 0.000259 , 0.000336 , 0.000305 , 0.00029 , 0.000259 , 0.000244 , 0.000275 , 0.000336 ,
        0.000259 , 0.000336 , 0.000549 , 0.00087 , 0.002029 , 0.005814 , 0.013901 , 0.028259 , 0.048477 , 0.07605 ,
        0.110367 , 0.147522 , 0.185928 , 0.226608 , 0.26564 , 0.301743 , 0.335648 , 0.368118 , 0.396729 , 0.422256 ,
        0.446976 , 0.470688 , 0.492615 , 0.513062 , 0.533142 , 0.550888 , 0.568039 , 0.583664 , 0.597855 , 0.610703 ,
        0.623901 , 0.635376 , 0.644943 , 0.653564 , 0.662216 , 0.67067 , 0.681198 , 0.692719 , 0.703125 , 0.712799 ,
        0.723053 , 0.732635 , 0.742172 , 0.750259 , 0.757523 , 0.764114 , 0.769989 , 0.775558 , 0.781067 , 0.785019 ,
        0.790588 , 0.796127 , 0.800125 , 0.803955 , 0.807892 , 0.811691 , 0.815552 , 0.819443 , 0.822662 , 0.825531 ,
        0.829422 , 0.832886 , 0.835892 , 0.839142 , 0.84201 , 0.844604 , 0.847672 , 0.850861 , 0.853622 , 0.855988 ,
        0.858368 , 0.86084 , 0.863098 , 0.865265 , 0.867691 , 0.869873 , 0.872009 , 0.873779 , 0.876114 , 0.877808 ,
        0.880005 , 0.881805 , 0.883484 , 0.885223 , 0.886597 , 0.888412 , 0.889725 , 0.891129 , 0.892303 , 0.89357 ,
        0.894867 , 0.895554 , 0.896362 , 0.897552 , 0.898178 , 0.898941 , 0.899643 , 0.90004 , 0.90065 , 0.901337 ,
        0.901703 , 0.902267 , 0.902802 , 0.903183 , 0.903458 , 0.903763 , 0.903824 , 0.903839 , 0.903931 , 0.903946 ,
        0.904205 , 0.904419 , 0.904587 , 0.904617 , 0.904907 , 0.904999 , 0.90509 , 0.905457 , 0.905777 , 0.905807 ,
        0.906113 , 0.906326 , 0.906464 , 0.906494 , 0.90683 , 0.907104 , 0.907242 , 0.907532 , 0.907623 , 0.907791 ,
        0.908096 , 0.908173 , 0.908401 , 0.908539 , 0.908798 , 0.908981 , 0.909332 , 0.909363 , 0.909531 , 0.909531 ,
        0.909851 , 0.910095 , 0.910172 , 0.910278 , 0.91037 , 0.910477 , 0.910446 , 0.910492 , 0.910645 , 0.910858 ,
        0.911057 , 0.911224 , 0.911331 , 0.911407 , 0.911636 , 0.911682 , 0.911728 , 0.911835 , 0.911987 , 0.91217 ,
        0.912338 , 0.912277 , 0.912277 , 0.91243 , 0.912476 , 0.912659 , 0.912735 , 0.912811 , 0.912811 , 0.913055 ,
        0.913193 , 0.91304 , 0.913361 , 0.91333 , 0.913437 , 0.913605 , 0.913635 , 0.913605 , 0.913864 , 0.913956 ,
        0.914017 , 0.914093 , 0.914307 , 0.914398 , 0.914627 , 0.914474 , 0.914383 , 0.914444 , 0.914612 , 0.914581 ,
        0.914627 , 0.914566 , 0.914536 , 0.914505 , 0.914459 , 0.914246 , 0.914154 , 0.914124 , 0.913879 , 0.913727 ,
        0.913559 , 0.913483 , 0.91333 , 0.913315 , 0.913284 , 0.91304 , 0.913086 , 0.91301 , 0.913055 , 0.913208 ,
        0.913467 , 0.913589 , 0.913712 , 0.914017 , 0.914108 , 0.914215 , 0.914291 , 0.914398 , 0.914566 , 0.914749 ,
        0.914841 , 0.914841 , 0.915161 , 0.915176 , 0.915085 , 0.915161 , 0.915405 , 0.915588 , 0.915771 , 0.915665 ,
        0.915665 , 0.91571 , 0.915924 , 0.915802 , 0.915848 , 0.916031 , 0.916107 , 0.916107 , 0.91626 , 0.916382 ,
        0.916351 , 0.916489 , 0.916473 , 0.916504 , 0.916595 , 0.916626 , 0.91655 , 0.916565 , 0.916534 , 0.916702 ,
        0.916748 , 0.916824 , 0.916672 , 0.916672 , 0.916733 , 0.916672 , 0.91655 , 0.916702 , 0.916748 , 0.916779 ,
        0.916565 , 0.91658 , 0.917007 , 0.917038 , 0.916931 , 0.916931 , 0.917007 , 0.91716 , 0.917023 , 0.917053 ,
        0.917038 , 0.917206 , 0.917191 , 0.917358 , 0.917374 , 0.917343 , 0.917435 , 0.917343 , 0.917435 , 0.917496 ,
        0.917679 , 0.917725 , 0.917572 , 0.917511 , 0.917511 , 0.917648 , 0.917679 , 0.917542 , 0.917603 , 0.917526 ,
        0.91748 , 0.917511 , 0.917603 , 0.91748 , 0.917542 , 0.91748 , 0.91745 , 0.91716 , 0.917084 , 0.916977 ,
        0.916763 , 0.916626 , 0.916351 , 0.916092 , 0.915802 , 0.915512 , 0.915131 , 0.914795 , 0.914429 , 0.914185 ,
        0.91394 , 0.913971 , 0.914001 , 0.913818 , 0.913864 , 0.913925 , 0.914276 , 0.914444 , 0.914673 , 0.914902 ,
        0.915146 , 0.915161 , 0.915237 , 0.915283 , 0.915359 , 0.915436 , 0.915497 , 0.915466 , 0.915588 , 0.915619 ,
        0.915695 , 0.915787 , 0.91597 , 0.915771 , 0.91597 , 0.915787 , 0.915848 , 0.91571 , 0.915634 , 0.915741 ,
        0.915894 , 0.915878 , 0.915833 , 0.915512 , 0.915604 , 0.915802 , 0.915741 , 0.915512 , 0.915192 , 0.915054 ,
        0.914825 , 0.915024 , 0.91478 , 0.914642 , 0.914337 , 0.914185 , 0.914154 , 0.914017 , 0.913834 , 0.913422 ,
        0.913193};
    
    num_acrylicTransmittance = 451;
    const G4int acrylicTransmittance_entries = num_acrylicTransmittance;
    G4double acrylicTransmittance_wavelength;
    acrylicTransmittance_energies = new G4double[acrylicTransmittance_entries];
    acrylicTransmittance = new G4double[acrylicTransmittance_entries];
    acrylicReflectivity = new G4double[acrylicTransmittance_entries];
    for( G4int i=0; i<acrylicTransmittance_entries; i++ ){
        acrylicTransmittance_wavelength = 200. + (G4double)i;
        acrylicTransmittance_energies[i] =
        (4.13566743E-15 * constC / (acrylicTransmittance_wavelength*1.E-9))*eV;
        acrylicTransmittance[i] = tmp_acrylicTransmittance[i];
        acrylicReflectivity[i] = 1- acrylicTransmittance[i];
    }
    //	Air is set to have a constant index of refraction that spans from the
    //	highest-energy scintillation photons to 2 microns to allow for
    //	refraction and reflection of Cerenkov photons.
    airRefractiveIndex = new G4double[NUM_PP_ConstProp];
    airRefractiveIndex[0] = airRefractiveIndex[1] = 1.000293;
    
    aluminumRefl = new G4double[NUM_PP];
    for( G4int i=0; i<NUM_PP; i++ )
        aluminumRefl[i] = 0.1;
    aluminumRefl[1] = aluminumRefl[2] = aluminumRefl[3] = aluminumRefl[4] = 0.7;
    
    aluminumUnoxidizedRefl = new G4double[NUM_PP_ConstProp];
    aluminumUnoxidizedRefl[0] = aluminumUnoxidizedRefl[1] = 1.0;
    
    berylliumRefl = new G4double[NUM_PP_ConstProp];
    berylliumRefl[0] = berylliumRefl[1] = 0.1;
    
    berylliumCopperRefl = new G4double[NUM_PP];
    for( G4int i=0; i<NUM_PP; i++ )
        berylliumCopperRefl[i] = 0.1;
    
    copperRefl = new G4double[NUM_PP];
    for( G4int i=0; i<NUM_PP; i++ )
        copperRefl[i] = 0.1;
    
    gasArRindex = new G4double[NUM_PP_ConstProp];
    gasArRindex[0] = gasArRindex[1] = 1.000293;
    gasArAbslength = new G4double[NUM_PP_ConstProp];
    gasArAbslength[0] = gasArAbslength[1] = 100.*km;
    
    gasArN2Rindex = new G4double[NUM_PP_ConstProp];
    gasArN2Rindex[0] = gasArN2Rindex[1] = 1.000293;
    gasArN2Abslength = new G4double[NUM_PP_ConstProp];
    gasArN2Abslength[0] = gasArN2Abslength[1] = 100.*km;
    
    gasN2Rindex = new G4double[NUM_PP_ConstProp];
    gasN2Rindex[0] = gasN2Rindex[1] = 1.000293;
    gasN2Abslength = new G4double[NUM_PP_ConstProp];
    gasN2Abslength[0] = gasN2Abslength[1] = 100.*km;
    
    gasXeRindex = new G4double[NUM_PP_ConstProp];
    gasXeRindex[0] = gasXeRindex[1] = 1.000702;
    gasXeAbslength = new G4double[NUM_PP_ConstProp];
    gasXeAbslength[0] = gasXeAbslength[1] = 3.0*m;
    
    goldRefl = new G4double[NUM_PP_ConstProp];
    goldRefl[0] = goldRefl[1] = 0.1;
    
    ironRefl = new G4double[NUM_PP_ConstProp];
    ironRefl[0] = ironRefl[1] = 0.001;
    
    isohexaneRindex = new G4double[NUM_PP_ConstProp];
    isohexaneRindex[0] = isohexaneRindex[1] = 1.379;
    
    labRindex = new G4double[NUM_PP_ConstProp];
    labRindex[0] = labRindex[1] = 1.485; // index of fraction for LAB is
    // 1.480-1.40, take the average
    
    leadRefl = new G4double[NUM_PP_ConstProp];
    leadRefl[0] = leadRefl[1] = 0.001;	//	Same as for Cu
    
    //	These definitions are, for now, blatantly ripped off from the quartz
    //	definitions, with the exception that the index of refraction has had
    //	0.15 added to every wavelength. I have no rational basis for believing
    //	this is the case other than the fact that Wikipedia says the index of
    //	refaction for quartz is "1.55" and "1.7" for lead glass.
    leadGlassRefractiveIndex = new G4double[NUM_PP];
    leadGlassRefractiveIndex[0] = 1.5317 + 4102.2/pow(photonWavelengths[0],2.) +
    1.8831e7/pow(photonWavelengths[0],4.);
    leadGlassRefractiveIndex[1] = 1.8; //guessed: makes the curve look smooth
    leadGlassRefractiveIndex[2] = 1.68175;
    leadGlassRefractiveIndex[3] = 1.67959;
    leadGlassRefractiveIndex[4] = 1.67738;
    leadGlassRefractiveIndex[5] = 1.65999;
    leadGlassRefractiveIndex[6] = 1.60032;
    leadGlassRefractiveIndex[7] = 1.576955;
    leadGlassRefractiveIndex[8] = 1.56747;
    leadGlassRefractiveIndex[9] = 1.556502;
    leadGlassRefractiveIndex[10] = 1.551027;
    leadGlassRefractiveIndex[11] = 1.548229;
    leadGlassRefractiveIndex[12] = 1.546174;
    leadGlassRefractiveIndex[13] = 1.542819;
    leadGlassRefractiveIndex[14] = 1.540488;
    leadGlassRefractiveIndex[15] = 1.539071;
    leadGlassRefractiveIndex[16] = 1.537525;
    leadGlassRefractiveIndex[17] = 1.53503;
    leadGlassRefractiveIndex[18] = 1.53102;
    leadGlassRefractiveIndex[19] = 1.52800;
    leadGlassRefractiveIndex[20] = 1.52703;
    leadGlassRefractiveIndex[21] = 1.52413;
    leadGlassRefractiveIndex[22] = 1.51998;
    leadGlassGroupVel = new G4double[NUM_PP];
    for( G4int i=0; i<NUM_PP; i++ ) {
        leadGlassRefractiveIndex[i] += 0.15;
        leadGlassGroupVel[i] = constC*m/s / leadGlassRefractiveIndex[i];
    }
    leadGlassAbsorption = new G4double[NUM_PP_ConstProp];
    leadGlassAbsorption[0] = 1.*m; leadGlassAbsorption[1] = 1./3*m;
    //taken from quartz
    //see http://www.mt-berlin.com/frames_cryst/descriptions/quartz%20.htm
    //for abs.
    
    //	These LXe properties are set by Matthew Szydagis, based on the
    //	most recent papers. Update from Dave Malling's original numbers.
    //	The equation for the index of refraction for liquid xenon comes from
    //	the Journal of Chemical Physics 123, 234508 (2005), and is a
    //	generalization for any density (hence any T and P) and any
    //	wavelength, but it has only been vetted at 161 to 170 K and at
    //      140 nm to 700 nm in wavelength range.
    liquidXeRindex = new G4double[NUM_PP];
    liquidXeRayleigh = new G4double[NUM_PP];
    liquidXeGroupVel = new G4double[NUM_PP];
    for( G4int i=0; i<NUM_PP; i++ ) {
        
        G4double term = 1.2055e-2*(2./3.)*(N_l/(5.8984e-3*g/cm3))*
        (0.26783/(43.741-pow(photonWavelengths[i]*1e-3,-2.))+
         0.29481/(57.480-pow(photonWavelengths[i]*1e-3,-2.))+
         5.03330/(112.74-pow(photonWavelengths[i]*1e-3,-2.)));
        liquidXeRindex[i] = sqrt(2*term+1)*sqrt(1/(1-term));
        if(i==1) liquidXeRindex[i]=1.8*(TEMPERATURE/(161.35*kelvin));
        if(i==0) liquidXeRindex[i]=1.000;
        //from L.M. Barkov et al., NIM A379 (1996), p.482 and ref. there-in


        liquidXeGroupVel[i] = constC*m/s / liquidXeRindex[i];
        
        G4double lightSpeed = constC*m/s;
        G4double mathPi = 3.141592653589793;
        G4double omega = (2*mathPi*lightSpeed)/(photonWavelengths[i]*nm);
        G4double kBoltz = 1.38e-23*joule/kelvin;
        G4double kappaT = 1.68e-5*cm2/newton;
        G4double epsilon = pow(liquidXeRindex[i],2.);
        //G4double constA = 0.0801*cm3/g;
        G4double dedpT = (epsilon-1)*(epsilon+2)/(3*N_l);
        G4double inverse = pow(omega,4.)/(6*mathPi*pow(lightSpeed,4.))*kBoltz*
        TEMPERATURE*pow(N_l,2.)*kappaT*pow(dedpT,2.);
        liquidXeRayleigh[i] = 1./(inverse); //based on NIM A 489 (2002) p.189
        
    }
    
    liquidXeAbslength = new G4double[NUM_PP_ConstProp];
    liquidXeAbslength[0] = liquidXeAbslength[1] = 70.*m;
    //liquidXeAbslength[0] = liquidXeAbslength[1] = 100.*m;
    
    //	Note that the refractive index is dirt simple and only roughly correct.
    //	This is just to make sure optical photons propagate at all.
    mineralOilRIndex = new G4double[NUM_PP_ConstProp];
    mineralOilRIndex[0] = mineralOilRIndex[1] = 1.53;
    
    // Zero for no reason other than it is grey
    peekRefl = new G4double[NUM_PP_ConstProp];
    peekRefl[0] = peekRefl[1] = 0.0;

    
    polyethyleneRefl = new G4double[NUM_PP_ConstProp];
    polyethyleneRefl[0] = polyethyleneRefl[1] = 0.10;	//	~10% acc. to
    //	Dr. White @175nm
    // Not accurate because dyes are put in borated poly.
    polyethyleneBoratedRefl = new G4double[NUM_PP_ConstProp];
    polyethyleneBoratedRefl[0] = polyethyleneBoratedRefl[1] = 0.10;
    
    // Zero for no other reason than it's black... yeah, I went there. 
    // > Crickets < -RL
    pvcRefl = new G4double[NUM_PP_ConstProp];
    pvcRefl[0] = pvcRefl[1] = 0.0;
    
    // The index of refraction of quartz agrees with Claudio Silva's (LIP-
    // Coimbra) report "Optical constants of some materials used in the LUX
    // detector" @178nm
    quartzRefractiveIndex = new G4double[NUM_PP];
    quartzGroupVel = new G4double[NUM_PP];
    for( G4int i=0; i<NUM_PP; i++ ) {//room temperature, but dn/dT is very small
        quartzRefractiveIndex[i] = leadGlassRefractiveIndex[i] - 0.15;
        quartzGroupVel[i] = constC*m/s / quartzRefractiveIndex[i];
    }
    quartzAbsorption = new G4double[NUM_PP];
    quartzAbsorption[0] =     1*m;
    quartzAbsorption[1] = 0.08*cm;
    quartzAbsorption[2] = 1.21*cm;
    quartzAbsorption[3] = 1.35*cm;
    quartzAbsorption[4] = 1.51*cm;
    quartzAbsorption[5] = 3.35*cm;
    quartzAbsorption[6] = 7.98*cm;
    quartzAbsorption[7] = 11.3*cm;
    quartzAbsorption[8] = 13.2*cm;
    quartzAbsorption[9] = 15.8*cm;
    quartzAbsorption[10] = 17.7*cm;
    quartzAbsorption[11] = 18.6*cm;
    quartzAbsorption[12] = 19.3*cm;
    quartzAbsorption[13] = 20.5*cm;
    quartzAbsorption[14] = 21.5*cm;
    quartzAbsorption[15] = 22.1*cm;
    quartzAbsorption[16] = 22.8*cm;
    quartzAbsorption[17] = 23.8*cm;
    quartzAbsorption[18] = 25.1*cm;
    quartzAbsorption[19] = 25.8*cm;
    quartzAbsorption[20] = 25.9*cm;
    quartzAbsorption[21] = 26.3*cm;
    quartzAbsorption[22] = 26.7*cm;
    
    quartzConstAbsorption = new G4double[NUM_PP_ConstProp];
    quartzConstAbsorption[0] = quartzConstAbsorption[1] = 1e3 * km;
    
    rockRefl = new G4double[NUM_PP_ConstProp];
    rockRefl[0] = rockRefl[1] = 0;
    
    //	Optical parameters for sapphire are (for now at least) taken to be the
    //	same as quartz
    sapphireRefractiveIndex = new G4double[NUM_PP];
    sapphireGroupVel = new G4double[NUM_PP];
    for( G4int i=0; i<NUM_PP; i++ ) {
        sapphireRefractiveIndex[i] = quartzRefractiveIndex[i];
        sapphireGroupVel[i] = constC*m/s / sapphireRefractiveIndex[i];
    }
    sapphireAbsorption = new G4double[NUM_PP_ConstProp];
    sapphireAbsorption[0] = 1.*m; sapphireAbsorption[1] = 1./3*m;
    
    teflonRefractiveIndex = new G4double[NUM_PP];
    teflonGroupVel = new G4double[NUM_PP];
    for( G4int i=0; i<NUM_PP; i++ ) {
        teflonRefractiveIndex[i] = 1.4536+1.3744/photonWavelengths[i]+5926.5/
        pow(photonWavelengths[i],2.)-2.1066e6/
        pow(photonWavelengths[i],3.)+2.5863e8/
        pow(photonWavelengths[i],4.);
        teflonGroupVel[i] = constC*m/s / teflonRefractiveIndex[i];
    }
    //http://teacher.pas.rochester.edu:8080/wiki/pub/Lux/LuxMaterial46/LUX_PTFE_Reflectance_InternalNote.pdf
    //says n=1.53 on average for PTFE @177nm
    //Used http://www.lrsm.upenn.edu/~frenchrh/download/0807JM3OptPropTeflonAF.pdf
    //to estimate the wavelength dependence. Fit data to Cauchy's formula
    
    // Taken from the steelRefl
    ss316Refl = new G4double[NUM_PP_ConstProp];
    ss316Refl[0] = ss316Refl[1] = 0.1;
    
    steelRefl = new G4double[NUM_PP_ConstProp];
    steelRefl[0] = steelRefl[1] = 0.1; //ansatz. Maybe much higher.
    
    teflonRefl = new G4double[NUM_PP_ConstProp];
    teflonRefl[0] = teflonRefl[1] = 0.98; //this is the reflectivity for
    //surfaces other than LXe or GXe, which are set separately below
    
    titaniumRefl = new G4double[NUM_PP_ConstProp];
    titaniumRefl[0] = titaniumRefl[1] = 0.1;
    
    //	TPB spectra come from a file I got from Kostas Mavrokoridis at
    //	Liverpool. I had to force them to span the full range of wavelengths
    //	defined in this file.
    G4double tmpTPBAbsEn[] = { 0.602391363477914, 3.06133803533393,
        3.07319513634028, 3.08941499610084, 3.10563473187722,
        3.12185459163779, 3.13807420342997, 3.15429418717473,
        3.1705140469353, 3.18673365872749, 3.20295327051967,
        3.21917325426443, 3.235393114025, 3.25161272581718,
        3.26783270956194, 3.28405256932251, 3.3002721811147,
        3.31649179290688, 3.33206495113016, 3.34821922325399,
        3.36920639912082, 3.3854261348972, 3.40164574668939,
        3.41786535848157, 3.43408534222633, 3.4503052019869,
        3.46652481377908, 3.48274467353965, 3.49896465728441,
        3.5151842690766, 3.53140388086878, 3.54762386461354,
        3.56384372437411, 3.5800633361663, 3.59628319592686,
        3.61250317967162, 3.62872279146381, 3.64494240325599,
        3.66116238700075, 3.67738224676132, 3.69360185855351,
        3.70982147034569, 3.72604145409045, 3.74226131385102,
        3.7584809256432, 3.77470090938796, 3.79092076914853,
        3.80714038094072, 3.8233599927329, 3.83957972850928,
        3.85579958826985, 3.87201944803041, 3.88823918380679,
        3.90445904356736, 3.92067890332793, 3.93689851512011,
        3.95311849886487, 3.96933835862544, 3.98555797041763,
        3.99772280324596, 4.01394266300652, 4.03016227479871,
        4.04638213455928, 4.06260187033565, 4.07882173009622,
        4.09504183782517, 4.11126144961736, 4.12748118539373,
        4.14370079718592, 4.15992040897811, 4.17614014475448,
        4.19235975654667, 4.20857986427562, 4.22479997200457,
        4.24101970778094, 4.25723931957313, 4.27345893136532,
        4.28967866714169, 4.30589877487064, 4.32211838666283,
        4.33833849439178, 4.35455823016815, 4.37077784196034,
        4.38699745375253, 4.40321718952891, 4.41943680132109,
        9.68626487742377 };
    G4double tpbMolarExtinction[] = { 0, 0, 3.051, 158.318, 251.477, 344.637,
        593.063,
        996.755, 1524.659, 2176.777, 3015.214, 3946.811, 5157.886, 6803.707,
        8480.581, 10405.880, 12548.553, 14598.066, 16577.320, 18556.701,
        21119.242, 22827.170, 24224.564, 25621.959, 27205.674, 28478.855,
        29689.932, 31180.486, 32484.721, 33633.688, 34441.070, 35217.402,
        35683.199, 36024.789, 36055.836, 35931.629, 35652.148, 35248.457,
        34720.551, 34161.598, 33602.637, 32950.520, 32236.295, 31273.645,
        30217.836, 29006.758, 27857.789, 26677.766, 25559.852, 24441.936,
        23448.232, 22392.424, 21336.615, 20280.805, 19162.889, 18076.027,
        17113.375, 16181.778, 15188.077, 14411.746, 13573.308, 12703.817,
        11927.488, 11120.104, 10561.146, 9877.976, 9381.124, 8884.272,
        8294.262, 7828.463, 7393.718, 6927.919, 6555.281, 6244.749,
        5965.270, 5623.684, 5406.312, 5126.833, 4940.514, 4754.194,
        4629.981, 4443.662, 4319.449, 4226.290, 4195.236, 4133.129,
        4100.0 };
    
    num_tpbAbs = sizeof( tmpTPBAbsEn ) / sizeof( G4double );
    tpbAbsorptionEnergies = new G4double[num_tpbAbs];
    tpbAbsorption = new G4double[num_tpbAbs];
    G4double concentration = 1.079e6;		//	mg/L
    G4double molecularWeight = 358.475e3;	//	mg/mol
    concentration /= molecularWeight;
    for( G4int i=0; i<num_tpbAbs; i++ ) {
        tpbAbsorptionEnergies[i] = tmpTPBAbsEn[i]*eV;
        tpbAbsorption[i] = ( 1. /
                            ( log(10)*tpbMolarExtinction[i]*concentration ) )*cm;
    }
    
    G4double tmpTPBEmiEn[] = { 0.602391363477914, 2.08376790640377,
        2.08378852632909,
        2.10000838608965, 2.11622799788184, 2.1324479816266,
        2.14866759341879, 2.16488745317935, 2.18110706497154,
        2.1973270487163, 2.21354666050848, 2.22976652026905,
        2.24598625604543, 2.262206115806, 2.27842597556656,
        2.29464558735875, 2.31086557110351, 2.32708518289569,
        2.34330504265626, 2.35952477843264, 2.37574463819321,
        2.39196424998539, 2.40818410974596, 2.42440384552234,
        2.4406237052829, 2.45684356504347, 2.47306330081985,
        2.48928316058042, 2.5055027723726, 2.52172263213317,
        2.53794236790955, 2.55416222767012, 2.57038208743068,
        2.58660182320706, 2.60282143499925, 2.61904129475981,
        2.63526115452038, 2.65148089029676, 2.66770075005733,
        2.68392036184951, 2.70014034559427, 2.71635995738646,
        2.73257981714703, 2.74879967690759, 2.76501941268397,
        2.78123902447616, 2.79745888423672, 2.81367886798148,
        2.82989847977367, 2.84611833953424, 2.86233795132642,
        2.87855793507118, 2.89477754686337, 2.91099740662393,
        2.92721714240031, 2.94343700216088, 2.95965661395307,
        2.97587647371363, 2.99209645745839, 3.00831606925058,
        3.02453568104276, 3.04075566478752, 3.05697552454809,
        3.07319513634028, 3.08941499610084, 3.10563473187722,
        3.12185459163779, 3.13807420342997, 3.15429418717473,
        3.1705140469353, 3.18673365872749, 3.20295327051967,
        3.21917325426443, 3.22036858262401, 9.68626487742377 };
    G4double tmpTPBEmi[] = { 0, 0, 0.085, 0.102, 0.120, 0.138, 0.155, 0.173,
        0.188,
        0.205, 0.225, 0.245, 0.262, 0.280, 0.301, 0.329, 0.364, 0.395,
        0.431, 0.482, 0.517, 0.556, 0.593, 0.629, 0.662, 0.692, 0.715,
        0.744, 0.783, 0.817, 0.852, 0.883, 0.899, 0.923, 0.942, 0.961,
        0.984, 0.990, 0.996, 0.996, 0.994, 0.990, 0.984, 0.969, 0.950,
        0.928, 0.902, 0.869, 0.840, 0.801, 0.764, 0.719, 0.676, 0.633,
        0.587, 0.548, 0.507, 0.453, 0.412, 0.368, 0.321, 0.284, 0.249,
        0.210, 0.179, 0.151, 0.122, 0.091, 0.071, 0.053, 0.039, 0.027,
        0.017, 0, 0 };
    
    num_tpbEmi = sizeof( tmpTPBEmiEn ) / sizeof( G4double );
    tpbEmissionEnergies = new G4double[num_tpbAbs];
    tpbEmission = new G4double[num_tpbEmi];
    for( G4int i=0; i<num_tpbEmi; i++ ) {
        tpbEmissionEnergies[i] = tmpTPBEmiEn[i]*eV;
        tpbEmission[i] = tmpTPBEmi[i];
    }
    
    
    //LAB absorption data from Mingfang
    
    G4double tmpLABAbsEn[] =  { 4.6788,4.6612,4.6437,4.6264,4.6092,4.5921,4.5752,4.5584,4.5417,
        4.5251,4.5086,4.4923,4.4761,4.4600,4.4440,4.4281,4.4124,4.3967,4.3812,4.3658,4.3504,4.3352,4.3201,
        4.3051,4.2902,4.2754,4.2607,4.2461,4.2316,4.2173,4.2030,4.1888,4.1747,4.1606,4.1467,4.1329,4.1192,
        4.1055,4.0920,4.0785,4.0652,4.0519,4.0387,4.0256,4.0125,3.9996,3.9867,3.9740,3.9613,3.9486,3.9361,3.9236,
        3.9113,3.8990,3.8868,3.8746,3.8625,3.8505,3.8386,3.8268,3.8150,3.8033,3.7917,3.7801,3.7686,3.7572,3.7458,3.7346,3.7233,
        3.7122,3.7011,3.6901,3.6791,3.6683,3.6574,3.6467,3.6360,3.6254,3.6148,3.6043,3.5938,3.5834,3.5731,3.5629,3.5526,3.5425,
        3.5324,3.5224,3.5124,3.5025,3.4926,3.4828,3.4730,3.4633,3.4537,3.4441,3.4346,3.4251,3.4156,3.4062,3.3969,3.3876,3.3784,
        3.3692,3.3601,3.3510,3.3420,3.3330,3.3241,3.3152,3.3063,3.2975,3.2888,3.2801,3.2714,3.2628,3.2543,3.2457,3.2373,3.2288,
        3.2205,3.2121,3.2038,3.1955,3.1873,3.1792,3.1710,3.1629,3.1549,3.1469,3.1389,3.1310,3.1231,3.1153,3.1075,3.0997,3.0920,
        3.0843,3.0766,3.0690,3.0614,3.0539,3.0464,3.0389,3.0315,3.0241,3.0167,3.0094,3.0021,2.9949,2.9876,2.9805,2.9733,2.9662,
        2.9591,2.9521,2.9451,2.9381,2.9311,2.9242,2.9173,2.9105,2.9037,2.8969,2.8901,2.8834,2.8767,2.8701,2.8634,2.8569,2.8503,
        2.8437,2.8372,2.8308,2.8243,2.8179,2.8115,2.8051,2.7988,2.7925,2.7862,2.7800,2.7738,2.7676,2.7614,2.7553,2.7492,2.7431,
        2.7370,2.7310,2.7250,2.7190,2.7131,2.7071,2.7012,2.6954,2.6895,2.6837,2.6779,2.6721,2.6664,2.6607,2.6550,2.6493,2.6437,
        2.6380,2.6324,2.6269,2.6213,2.6158,2.6103,2.6048,2.5993,2.5939,2.5885,2.5831,2.5777,2.5724,2.5670,2.5617,2.5564,2.5512,
        2.5459,2.5407,2.5355,2.5304,2.5252,2.5201,2.5150,2.5099,2.5048,2.4997,2.4947,2.4897,2.4847,2.4797,2.4748,2.4699,2.4650,
        2.4601,2.4552,2.4503,2.4455,2.4407,2.4359,2.4311,2.4264,2.4216,2.4169,2.4122,2.4075,2.4029,2.3982,2.3936,2.3890,2.3844,
        2.3798,2.3752,2.3707,2.3662,2.3617,2.3572,2.3527,2.3482,2.3438,2.3394,2.3350,2.3306,2.3262,2.3219,2.3175,2.3132,2.3089,
        2.3046,2.3003,2.2961,2.2918,2.2876,2.2834,2.2792,2.2750,2.2708,2.2667,2.2625,2.2584,2.2543,2.2502,2.2461,2.2421,2.2380,
        2.2340,2.2300,2.2260,2.2220,2.2180,2.2141,2.2101,2.2062,2.2023,2.1984,2.1945,2.1906,2.1867,2.1829,2.1790,2.1752,2.1714,
        2.1676,2.1638,2.1601,2.1563,2.1526,2.1488,2.1451,2.1414,2.1377,2.1340,2.1304,2.1267,2.1231,2.1194,2.1158,2.1122,2.1086,
        2.1050,2.1015,2.0979,2.0944,2.0908,2.0873,2.0838,2.0803,2.0768,2.0734,2.0699,2.0665 };
			 
    G4double labMolarExtinction[] = { 112.103405846,117.622251260,123.496417471,129.985608690,136.679043343,143.278974340,
        150.111627289,157.835824135,165.906182895,174.757848072,184.626569852,194.141403119,203.086620776,212.293410204,
        222.563322966,232.973044968,244.149935815,255.523429511,266.194402720,276.783268989,287.825557716,300.415534179,
        313.768883218,326.832125805,338.138579494,347.405400169,356.361047872,365.891166143,376.289756866,386.975182188,
        396.720677266,403.810224823,410.518532965,418.100527112,428.216510764,440.311111036,453.036023071,464.065330775,
        469.475184588,467.547669836,462.229460284,457.979974107,455.368833907,454.742321930,454.409189309,451.611358441,
        444.519497653,436.440109120,431.816769346,433.452433091,438.375051367,443.315357037,444.499113221,435.807102093,
        415.065892847,384.586416941,352.650962912,324.613727618,303.085383975,287.992530261,275.439874909,263.080273966,
        248.791986771,233.209687024,219.935684084,211.180248808,209.962597326,208.277793949,205.760315971,199.601177727,
        186.347381721,162.635213805,131.116287878,97.220426966,66.322562165,42.249174977,20.683350564,13.974799271,9.648014671,
        6.991782257,5.533595170,4.652491928,4.108836694,3.742887464,3.457808610,3.226452739,3.044382114,2.899478106,2.789347862,
        2.688477120,2.626427677,2.563388953,2.507686485,2.453434309,2.402796633,2.342115509,2.281987146,2.232595518,2.169905988,
        2.110951079,2.028585975,1.956876007,1.890164759,1.825159617,1.755242346,1.689595493,1.622504606,1.556936771,1.485407539,
        1.419457940,1.349271385,1.275562069,1.204806524,1.133612651,1.060917524,0.985350025,0.911605446,0.836783215,0.758493240,
        0.682177237,0.609952809,0.539406802,0.472244947,0.409391983,0.346735795,0.290680124,0.242088179,0.197134679,0.156166948,
        0.123162933,0.096053599,0.075420353,0.058038787,0.043368957,0.032087234,0.023531169,0.017167095,0.012581615,0.009268334,
        0.006864883,0.005121780,0.003923155,0.003051701,0.002444295,0.001999665,0.001662609,0.001421992,0.001231355,0.001090255,
        0.000949859,0.000862834,0.000780314,0.000735895,0.000671020,0.000637518,0.000621008,0.000577694,0.000560490,0.000543401,
        0.000534855,0.000527697,0.000515030,0.000513571,0.000512464,0.000508151,0.000502535,0.000507366,0.000515193,0.000498411,
        0.000484659,0.000493168,0.000479634,0.000494509,0.000482121,0.000482363,0.000488003,0.000465985,0.000462961,0.000456294,
        0.000471741,0.000471844,0.000461944,0.000444178,0.000454454,0.000464867,0.000448642,0.000452546,0.000462059,0.000450999,
        0.000451269,0.000452172,0.000440032,0.000437720,0.000426537,0.000420627,0.000439142,0.000433897,0.000432504,0.000446584,
        0.000427055,0.000427133,0.000437720,0.000431082,0.000414339,0.000421343,0.000408000,0.000425598,0.000390577,0.000419354,
        0.000399766,0.000398671,0.000384620,0.000380307,0.000397378,0.000383217,0.000369515,0.000363934,0.000373202,0.000378788,
        0.000362863,0.000381647,0.000360958,0.000368409,0.000381137,0.000368946,0.000372278,0.000357813,0.000377481,0.000372405,
        0.000371269,0.000384047,0.000368076,0.000373330,0.000365950,0.000351342,0.000377644,0.000355592,0.000361179,0.000351176,
        0.000357937,0.000361076,0.000353081,0.000358667,0.000368350,0.000367175,0.000364980,0.000359201,0.000351278,0.000350766,
        0.000349644,0.000359544,0.000363690,0.000362568,0.000355525,0.000358664,0.000355255,0.000349373,0.000353519,0.000353277,
        0.000372807,0.000353277,0.000356377,0.000380323,0.000375906,0.000374224,0.000373102,0.000383002,0.000393144,0.000397048,
        0.000384636,0.000380323,0.000377121,0.000373590,0.000374765,0.000364980,0.000365763,0.000361788,0.000352298,0.000374712,
        0.000384509,0.000364980,0.000370566,0.000360666,0.000370566,0.000387367,0.000384509,0.000388119,0.000409896,0.000421484,
        0.000413968,0.000408774,0.000432478,0.000412654,0.000404556,0.000417043,0.000426039,0.000425797,0.000417404,0.000419140,
        0.000423671,0.000423430,0.000409487,0.000434452,0.000427743,0.000423693,0.000403653,0.000425797,0.000415897,0.000426363,
        0.000421893,0.000415897,0.000405997,0.000399898,0.000409872,0.000397243,0.000387072,0.000375537,0.000391386,0.000395682,
        0.000402212,0.000399996,0.000385782,0.000399213,0.000404239,0.000398464,0.000398971,0.000403232,0.000420135,0.000416900,
        0.000427448,0.000433330,0.000433330,0.000439860,0.000453154,0.000464176,0.000490121 };
    
    num_labAbs = sizeof( tmpLABAbsEn ) / sizeof( G4double );
    labAbsorptionEnergies = new G4double[num_labAbs];
    labAbsorption = new G4double[num_labAbs];
    G4double LABconcentration = 0.86e6;		// mg/L
    G4double LABmolecularWeight = 260.465e3;	// mg/mol-assuming formula
    // C6H6-C13H27 per Minfang, was
    // 260.465e3,should be 261.465e3
    LABconcentration /= LABmolecularWeight;
    for( G4int i=0; i<num_labAbs; i++ ) {
        labAbsorptionEnergies[i] = tmpLABAbsEn[i]*eV;
        //labAbsorption[i] = ( 1. /
        //        ( log(10)*labMolarExtinction[i]*LABconcentration ) )*cm;
        labAbsorption[i] = ( (1. /  labMolarExtinction[i]) *
                            (1510.0/1989.911)  )*cm; //fudge factor
        
        //G4cout<<"Energy: "<<labAbsorptionEnergies[i]/eV<<" Abs length: "
        //<<labAbsorption[i]/cm<<G4endl;
    }
    
    
    
    tempLABAbsLength = new G4double[NUM_PP_ConstProp];
    tempLABAbsLength[0] = tempLABAbsLength[1] = 10.*m;
    tempLABAbsEnergies = new G4double[NUM_PP_ConstProp];
    tempLABAbsEnergies[0] = 4.6788*eV;
    tempLABAbsEnergies[1] = 2.0665*eV;
    
    //LAB emission data from M. Yeh
    G4double tmpLABEmiEn[] = { 4.6788,4.6612,4.6437,4.6264,4.6092,4.5921,4.5752,4.5584,4.5417,4.5251,4.5086,4.4923,4.4761,4.4600,4.4440,
        4.4281,4.4124,4.3967,4.3812,4.3658,4.3504,4.3352,4.3201,4.3051,4.2902,4.2754,4.2607,4.2461,4.2316,4.2173,4.2030,
        4.1888,4.1747,4.1606,4.1467,4.1329,4.1192,4.1055,4.0920,4.0785,4.0652,4.0519,4.0387,4.0256,4.0125,3.9996,3.9867,3.9740,
        3.9613,3.9486,3.9361,3.9236,3.9113,3.8990,3.8868,3.8746,3.8625,3.8505,3.8386,3.8268,3.8150,3.8033,3.7917,3.7801,3.7686,
        3.7572,3.7458,3.7346,3.7233,3.7122,3.7011,3.6901,3.6791,3.6683,3.6574,3.6467,3.6360,3.6254,3.6148,3.6043,3.5938,3.5834,
        3.5731,3.5629,3.5526,3.5425,3.5324,3.5224,3.5124,3.5025,3.4926,3.4828,3.4730,3.4633,3.4537,3.4441,3.4346,3.4251,3.4156,
        3.4062,3.3969,3.3876,3.3784,3.3692,3.3601,3.3510,3.3420,3.3330,3.3241,3.3152,3.3063,3.2975,3.2888,3.2801,3.2714,3.2628,
        3.2543,3.2457,3.2373,3.2288,3.2205,3.2121,3.2038,3.1955,3.1873,3.1792,3.1710,3.1629,3.1549,3.1469,3.1389,3.1310,3.1231,
        3.1153,3.1075,3.0997,3.0920,3.0843,3.0766,3.0690,3.0614,3.0539,3.0464,3.0389,3.0315,3.0241,3.0167,3.0094,3.0021,2.9949,
        2.9876,2.9805,2.9733,2.9662,2.9591,2.9521,2.9451,2.9381,2.9311,2.9242,2.9173,2.9105,2.9037,2.8969,2.8901,2.8834,2.8767,
        2.8701,2.8634,2.8569,2.8503,2.8437,2.8372,2.8308,2.8243,2.8179,2.8115,2.8051,2.7988,2.7925,2.7862,2.7800,2.7738,2.7676,
        2.7614,2.7553,2.7492,2.7431,2.7370,2.7310,2.7250,2.7190,2.7131,2.7071,2.7012,2.6954,2.6895,2.6837,2.6779,2.6721,2.6664,
        2.6607,2.6550,2.6493,2.6437,2.6380,2.6324,2.6269,2.6213,2.6158,2.6103,2.6048,2.5993,2.5939,2.5885,2.5831,2.5777,2.5724,
        2.5670,2.5617,2.5564,2.5512,2.5459,2.5407,2.5355,2.5304,2.5252,2.5201,2.5150,2.5099,2.5048,2.4997,2.4947,2.4897,2.4847,
        2.4797,2.4748,2.4699,2.4650,2.4601,2.4552,2.4503,2.4455,2.4407,2.4359,2.4311,2.4264,2.4216,2.4169,2.4122,2.4075,2.4029,
        2.3982,2.3936,2.3890,2.3844,2.3798,2.3752,2.3707,2.3662,2.3617,2.3572,2.3527,2.3482,2.3438,2.3394,2.3350,2.3306,2.3262,
        2.3219,2.3175,2.3132,2.3089,2.3046,2.3003,2.2961,2.2918,2.2876,2.2834,2.2792,2.2750,2.2708,2.2667,2.2625,2.2584,2.2543 };
    
    G4double tmpLABEmi[] = { 2261.661633,2290.708448,2584.518873,2156.111097,2235.014057,2392.056949,2189.069253,2397.193479,2057.636949,
        2159.952299,2106.798452,2075.024067,1937.452719,1986.98382,2187.030222,2114.385289,2216.940142,2282.202621,2047.624093,
        1823.5858,1810.870638,2113.905068,1894.398974,1805.894737,1641.443092,1751.377745,1769.872208,1890.106122,1798.735932,
        1654.869284,1653.527623,1669.713147,1818.869529,1540.895728,1739.599899,1524.601107,1628.146172,1475.519348,1561.708683,
        1495.979781,1719.387244,1729.135924,1681.659483,1608.187096,1514.339045,1573.7657,1561.406013,1658.988169,1504.572251,
        1559.537588,1434.682897,1570.088673,1510.883981,1622.345724,1420.769397,1433.364588,1416.825524,1276.121629,1433.4534,
        1473.066638,1431.528272,1356.860678,1473.832347,1416.012737,1461.918812,1400.988787,1438.016847,1527.344341,1480.596176,
        1487.019374,1580.020941,1573.348826,1628.760685,1672.363102,1852.461958,1933.472541,1705.525788,2044.774037,2092.019342,
        2187.13696,2199.344332,2424.661416,2439.79996,2608.58345,2676.486465,3453.625326,3748.99938,4674.991132,5276.417873,
        6176.348782,7091.424738,8050.470643,9210.481233,10469.73186,10906.64914,11546.60044,11640.41518,12316.9856,12349.71104,
        12566.77254,12412.8606,13153.20447,12859.9197,13614.72422,14146.06593,14807.3039,16154.25549,17390.47693,19481.17197,
        20669.85121,22485.48922,23939.17308,26292.9392,28976.86061,31590.68378,34711.72733,37929.31549,42782.45552,48897.41588,
        54310.33142,63230.81715,73822.15217,88219.08675,104444.1695,125192.5642,152742.6574,179922.6029,213582.1686,248575.048,
        286923.6218,319192.1789,352685.2053,386152.5878,407193.8967,423439.9145,432847.847,431191.0781,426214.6218,419159.7565,
        405419.9532,392270.5585,378312.6467,368129.2885,360529.2582,353126.7685,350097.4604,351582.9126,354854.77,361792.2466,
        370659.2938,381010.4562,389326.4971,399981.8489,411037.497,416853.094,420004.4914,424356.6988,423397.5814,424207.7612,
        418935.7338,406891.4688,397363.2824,385562.5664,370844.4388,355949.3438,339786.415,323745.76,309250.4526,292758.185,
        280573.5012,270493.68,260137.2962,252641.8532,244302.6906,238981.0492,234471.4788,231594.5588,231212.7876,228816.7562,
        223223.3458,224809.6056,221772.1288,222366.0812,218169.6788,214926.3762,211037.2252,208875.8376,202932.9732,197177.762,
        191002.503,184985.4596,177368.1464,170473.8958,163756.3626,156841.0656,148002.8038,142025.7844,135201.577,127527.811,
        121447.664,114857.594,108936.9126,106227.7916,101359.4142,97054.45572,94147.5037,90978.47088,88099.11418,85839.94906,
        84110.17794,81223.31412,79244.602,77928.51,76417.728,73861.52282,72152.294,70080.106,68212.444,65302.42298,64411.9486,
        62620.61872,60087.1817,58752.75414,56814.82234,54603.79138,52331.61856,50100.15778,47841.18522,46243.44788,43587.24494,
        40715.4553,39095.0891,37644.75468,35955.17808,34155.49456,32818.04704,30999.00262,29492.18672,28259.1589,27786.4211,
        25280.54662,25113.2203,23952.72114,23091.78,22018.47494,22114.0821,20800.37306,20121.87534,19056.28334,19009.7414,
        18228.12678,17341.66522,17271.66404,16576.23152,16517.09374,15541.44532,15136.10642,14370.12402,14144.96426,13110.32704,
        12854.95418,12994.9409,12673.90486,11899.40684,11384.6207,10424.39136,10569.83344,10448.54002,9617.80772,9599.04968,
        8960.0508,8972.0823,8107.191916,8229.8904,7743.304578,7407.972268,7221.438014,7058.716522,6895.298934,6640.89442,
        6839.760178,6152.568218,6112.168542,5839.69646,5367.784596,5464.296432 };
    
    num_labEmi = sizeof( tmpLABEmiEn ) / sizeof( G4double );
    labEmissionEnergies = new G4double[num_labEmi];
    labEmission = new G4double[num_labEmi];
    for( G4int i=0; i<num_labEmi; i++ ) {
        labEmissionEnergies[i] = tmpLABEmiEn[i]*eV;
        labEmission[i] = tmpLABEmi[i];
    }
    
    // LAB Rayleigh Scattering Length (Taken from M. Yeh)
    num_lab_Rayleigh = 17;
    const G4int lab_Rayleigh_entries = num_lab_Rayleigh;
    // LAB Scattering Length Array
    lab_Rayleigh_length = new G4double[lab_Rayleigh_entries];
    lab_Rayleigh_length[0] = 8.68734076274787*m;
    lab_Rayleigh_length[1] = 10.7254760102369*m;
    lab_Rayleigh_length[2] = 11.8674309747269*m;
    lab_Rayleigh_length[3] = 14.1623209773276*m;
    lab_Rayleigh_length[4] = 15.6868865658854*m;
    lab_Rayleigh_length[5] = 17.5282158242408*m;
    lab_Rayleigh_length[6] = 18.9349966517565*m;
    lab_Rayleigh_length[7] = 22.7906416002885*m;
    lab_Rayleigh_length[8] = 25.383814688718*m;
    lab_Rayleigh_length[9] = 29.0604129865767*m;
    lab_Rayleigh_length[10] = 28.9584092232602*m;
    lab_Rayleigh_length[11] = 30.1663636780663*m;
    lab_Rayleigh_length[12] = 32.9243735258919*m;
    lab_Rayleigh_length[13] = 24.6336366756871*m;
    lab_Rayleigh_length[14] = 42.5957555777744*m;
    lab_Rayleigh_length[15] = 47.145304652404*m;
    lab_Rayleigh_length[16] = 52.8491630451655*m;
    
    // LAB Rayleigh Photon Energies
    lab_Rayleigh_energy = new G4double[lab_Rayleigh_entries];
    lab_Rayleigh_energy[0] = 3.647*eV;
    lab_Rayleigh_energy[1] = 3.543*eV;
    lab_Rayleigh_energy[2] = 3.444*eV;
    lab_Rayleigh_energy[3] = 3.351*eV;
    lab_Rayleigh_energy[4] = 3.263*eV;
    lab_Rayleigh_energy[5] = 3.179*eV;
    lab_Rayleigh_energy[6] = 3.1*eV;
    lab_Rayleigh_energy[7] = 3.024*eV;
    lab_Rayleigh_energy[8] = 2.952*eV;
    lab_Rayleigh_energy[9] = 2.883*eV;
    lab_Rayleigh_energy[10] = 2.818*eV;
    lab_Rayleigh_energy[11] = 2.755*eV;
    lab_Rayleigh_energy[12] = 2.696*eV;
    lab_Rayleigh_energy[13] = 2.638*eV;
    lab_Rayleigh_energy[14] = 2.583*eV;
    lab_Rayleigh_energy[15] = 2.53*eV;
    lab_Rayleigh_energy[16] = 2.48*eV;
    
    //G4cout<<"num_labEmi = "<<num_labEmi<<G4endl;
    //G4cout<<"EmiData: "<<sizeof( tmpLABEmi ) / sizeof( G4double )<<G4endl;
    
    //G4cout<<"num_labAbs = "<<num_labAbs<<G4endl;
    //G4cout<<"AbsEnergies: "<< sizeof(tmpLABAbsEn)/sizeof(G4double) <<G4endl;
    //G4cout<<"AbsData: "<< sizeof(labMolarExtinction)/sizeof(G4double)<<G4endl;
    
    tungstenRefl = new G4double[NUM_PP_ConstProp];
    tungstenRefl[0] = tungstenRefl[1] = 0.1;
    
    //	Water code taken from Kareem's NeutronDetector simulation code
    //	53 entries creates wavelengths from 200 to 720 nm in 10 nm steps, and
    //	the first is set to the first and last elements of the photonEnergies
    //	array.
    num_water = 55;
    const G4int nWaterEntries = num_water;
    G4double wavelength;
    waterPhotonEnergies = new G4double[nWaterEntries];
    waterRefractiveIndex = new G4double[nWaterEntries];
    waterGroupVel = new G4double[nWaterEntries];
    for( G4int i=0; i<nWaterEntries; i++ ) {
        wavelength = 190. + 10.*(G4double)i;	//	This is the wavelength in nm
        waterPhotonEnergies[i] =
        (4.13566743E-15 * constC / (wavelength*1.E-9))*eV;
        if( i==0 ) waterPhotonEnergies[i] = photonEnergies[0];
        if( i==(nWaterEntries-1) ) waterPhotonEnergies[i] =
            photonEnergies[NUM_PP-1];
        
        //	The equation for the refractive index is evaluated by D. T. Huibers,
        //	"Models for the wavelength dependence of the index of refraction
        //	of water", Applied Optics 36 (1997) p.3785. The original equation
        //	comes from X. Qua and E. S. Fry, "Empirical equation for the index
        //	of refraction of seawater", Applied Optics 34 (1995) p.3477.
        waterRefractiveIndex[i] = 1.31279 + 15.762/wavelength
        - 4382./(pow(wavelength,2)) + 1.1455E6/(pow(wavelength,3));
        
        waterGroupVel[i] = constC*m/s / waterRefractiveIndex[i];
    }
    //	The absorption distances from from several references --
    //	200 - 320 nm:	T. I. Quickenden & J. A. Irvin, "The ultraviolet
    //					absorption spectrum of liquid water", J. Chem. Phys.
    //					72(8) (1980) p4416.
    //	330 nm:			A rough average between 320 and 340 nm. Very subjective.
    //	340 - 370 nm:	F. M. Sogandares and E. S. Fry,  "Absorption
    //					spectrum (340 - 640 nm) of pure water. I. Photothermal
    //					measurements", Applied Optics 36 (1997) p.8699.
    //	380 - 720 nm:	R. M. Pope and E. S. Fry, "Absorption spectrum (380 -
    //					700 nm) of pure water. II. Integrating cavity
    //					measurements", Applied Optics 36 (1997) p.8710.
    //	The first and last points are wild guesses based on a by-eye
    //	extrapolationof the curve from 200-320 nm.
    waterAbsorption = new G4double[nWaterEntries];
    waterAbsorption[0] = 0*m;
    waterAbsorption[1] = 3.086419753*m;
    waterAbsorption[2] = 7.936507937*m;
    waterAbsorption[3] = 12.42236025*m;
    waterAbsorption[4] = 16.80672269*m;
    waterAbsorption[5] = 20.70393375*m;
    waterAbsorption[6] = 26.59574468*m;
    waterAbsorption[7] = 32.46753247*m;
    waterAbsorption[8] = 42.37288136*m;
    waterAbsorption[9] = 45.04504505*m;
    waterAbsorption[10] = 61.34969325*m;
    waterAbsorption[11] = 80.64516129*m;
    waterAbsorption[12] = 89.28571429*m;
    waterAbsorption[13] = 100.*m;
    waterAbsorption[14] = 50.*m;
    waterAbsorption[15] = 30.76923077*m;
    waterAbsorption[16] = 49.01960784*m;
    waterAbsorption[17] = 64.1025641*m;
    waterAbsorption[18] = 87.71929825*m;
    waterAbsorption[19] = 87.95074758*m;
    waterAbsorption[20] = 117.5088132*m;
    waterAbsorption[21] = 150.8295626*m;
    waterAbsorption[22] = 211.4164905*m;
    waterAbsorption[23] = 220.2643172*m;
    waterAbsorption[24] = 202.020202*m;
    waterAbsorption[25] = 157.480315*m;
    waterAbsorption[26] = 108.4598698*m;
    waterAbsorption[27] = 102.145046*m;
    waterAbsorption[28] = 94.33962264*m;
    waterAbsorption[29] = 78.74015748*m;
    waterAbsorption[30] = 66.66666667*m;
    waterAbsorption[31] = 49.01960784*m;
    waterAbsorption[32] = 30.76923077*m;
    waterAbsorption[33] = 24.44987775*m;
    waterAbsorption[34] = 23.04147465*m;
    waterAbsorption[35] = 21.09704641*m;
    waterAbsorption[36] = 17.69911504*m;
    waterAbsorption[37] = 16.15508885*m;
    waterAbsorption[38] = 14.38848921*m;
    waterAbsorption[39] = 11.16071429*m;
    waterAbsorption[40] = 7.4019245*m;
    waterAbsorption[41] = 4.496402878*m;
    waterAbsorption[42] = 3.78214826*m;
    waterAbsorption[43] = 3.629764065*m;
    waterAbsorption[44] = 3.429355281*m;
    waterAbsorption[45] = 3.217503218*m;
    waterAbsorption[46] = 2.941176471*m;
    waterAbsorption[47] = 2.43902439*m;
    waterAbsorption[48] = 2.277904328*m;
    waterAbsorption[49] = 2.150537634*m;
    waterAbsorption[50] = 1.937984496*m;
    waterAbsorption[51] = 1.602564103*m;
    waterAbsorption[52] = 1.209189843*m;
    waterAbsorption[53] = 0.812347685*m;
    waterAbsorption[54] = 0*m;
    
    halonRefl = new G4double[nWaterEntries];
    for( G4int i=0; i<nWaterEntries; i++ )
        halonRefl[i] = 0.94;
    
    tyvekRefl = new G4double[nWaterEntries];
    tyvekRefl[0] = 0; // 110 nm
    tyvekRefl[1] = 0; // 200 nm
    tyvekRefl[2] = 0;
    tyvekRefl[3] = 0;
    tyvekRefl[4] = 0;
    tyvekRefl[5] = 0;
    tyvekRefl[6] = .577; // 250 nm (first data point)
    tyvekRefl[7] = .595;
    tyvekRefl[8] = .606;
    tyvekRefl[9] = .616;
    tyvekRefl[10] = .643;
    tyvekRefl[11] = .668; // 300 nm
    tyvekRefl[12] = .682;
    tyvekRefl[13] = .697;
    tyvekRefl[14] = .719;
    tyvekRefl[15] = .754;
    tyvekRefl[16] = .788; // 350 nm
    tyvekRefl[17] = .815;
    tyvekRefl[18] = .835;
    tyvekRefl[19] = .845;
    tyvekRefl[20] = .852;
    tyvekRefl[21] = .857; // 400 nm
    tyvekRefl[22] = .861;
    tyvekRefl[23] = .861;
    tyvekRefl[24] = .863;
    tyvekRefl[25] = .864;
    tyvekRefl[26] = .866; // 450 nm
    tyvekRefl[27] = .866;
    tyvekRefl[28] = .865;
    tyvekRefl[29] = .865;
    tyvekRefl[30] = .865;
    tyvekRefl[31] = .864; // 500 nm
    tyvekRefl[32] = .864;
    tyvekRefl[33] = .865;
    tyvekRefl[34] = .864;
    tyvekRefl[35] = .865;
    tyvekRefl[36] = .864; // 550 nm
    tyvekRefl[37] = .861;
    tyvekRefl[38] = .862;
    tyvekRefl[39] = .862;
    tyvekRefl[40] = .862;
    tyvekRefl[41] = .861; // 600 nm
    tyvekRefl[42] = .861;
    tyvekRefl[43] = .860;
    tyvekRefl[44] = .859;
    tyvekRefl[45] = .860;
    tyvekRefl[46] = .860; // 650 nm
    tyvekRefl[47] = .859;
    tyvekRefl[48] = .858;
    tyvekRefl[49] = .858;
    tyvekRefl[50] = .857;
    tyvekRefl[51] = .857; // 700 nm
    tyvekRefl[52] = .857;
    tyvekRefl[53] = .856;
    tyvekRefl[54] = .855; // 730 nm
    tyvekRefl[53] = 0; // 720 nm (unphysical, but don't want to interpolate all
    // the way to 2058 nm)
    tyvekRefl[54] = 0; // 2058.2 nm
    
    //Specular Reflector for LZ
    lzSpecRefRefl = new G4double[nWaterEntries];
    lzSpecRefRefl[0] = 0; // 110 nm
    lzSpecRefRefl[1] = 0; // 200 nm
    lzSpecRefRefl[2] = 0;
    lzSpecRefRefl[3] = 0;
    lzSpecRefRefl[4] = 0;
    lzSpecRefRefl[5] = 0;
    lzSpecRefRefl[6] = 0; // 250 nm
    lzSpecRefRefl[7] = 0.183; //first data point
    lzSpecRefRefl[8] = 0.133;
    lzSpecRefRefl[9] = 0.120;
    lzSpecRefRefl[10] = 0.131;
    lzSpecRefRefl[11] = 0.143; // 300 nm
    lzSpecRefRefl[12] = 0.139;
    lzSpecRefRefl[13] = 0.117;
    lzSpecRefRefl[14] = 0.107;
    lzSpecRefRefl[15] = 0.104;
    lzSpecRefRefl[16] = 0.102; // 350 nm
    lzSpecRefRefl[17] = 0.115;
    lzSpecRefRefl[18] = 0.123;
    lzSpecRefRefl[19] = 0.539;
    lzSpecRefRefl[20] = 0.954;
    lzSpecRefRefl[21] = 0.964; // 400 nm
    lzSpecRefRefl[22] = 0.965;
    lzSpecRefRefl[23] = 0.966;
    lzSpecRefRefl[24] = 0.960;
    lzSpecRefRefl[25] = 0.968;
    lzSpecRefRefl[26] = 0.964; // 450 nm
    lzSpecRefRefl[27] = 0.972;
    lzSpecRefRefl[28] = 0.975;
    lzSpecRefRefl[29] = 0.972;
    lzSpecRefRefl[30] = 0.981;
    lzSpecRefRefl[31] = 0.983; // 500 nm
    lzSpecRefRefl[32] = 0.983;
    lzSpecRefRefl[33] = 0.984;
    lzSpecRefRefl[34] = 0.983;
    lzSpecRefRefl[35] = 0.982;
    lzSpecRefRefl[36] = 0.977; // 550 nm
    lzSpecRefRefl[37] = 0.971;
    lzSpecRefRefl[38] = 0.976;
    lzSpecRefRefl[39] = 0.990;
    lzSpecRefRefl[40] = 0.994;
    lzSpecRefRefl[41] = 0.995; // 600 nm
    lzSpecRefRefl[42] = 0.994;
    lzSpecRefRefl[43] = 0.996;
    lzSpecRefRefl[44] = 0.994;
    lzSpecRefRefl[45] = 0.990;
    lzSpecRefRefl[46] = 0.952; // 650 nm
    lzSpecRefRefl[47] = 0.949;
    lzSpecRefRefl[48] = 0.945;
    lzSpecRefRefl[49] = 0.970;
    lzSpecRefRefl[50] = 0.974;
    lzSpecRefRefl[51] = 0.971; // 700 nm
    lzSpecRefRefl[52] = 0.968;
    lzSpecRefRefl[53] = 0.975; // 720 nm
    lzSpecRefRefl[54] = 0; // 2058.2 nm
    
    tyvek600Refl = new G4double[nWaterEntries];
    tyvek600Refl[0] = 0; // 110 nm
    tyvek600Refl[1] = 0; // 220 nm
    tyvek600Refl[2] = 0;
    tyvek600Refl[3] = 0;
    tyvek600Refl[4] = 0;
    tyvek600Refl[5] = 0;
    tyvek600Refl[6] = 0.622; // 250 nm
    tyvek600Refl[7] = 0.817;
    tyvek600Refl[8] = 0.906;
    tyvek600Refl[9] = 0.931;
    tyvek600Refl[10] = 0.944;
    tyvek600Refl[11] = 0.953; // 300 nm
    tyvek600Refl[12] = 0.959;
    tyvek600Refl[13] = 0.961;
    tyvek600Refl[14] = 0.962;
    tyvek600Refl[15] = 0.964;
    tyvek600Refl[16] = 0.966; // 350 nm
    tyvek600Refl[17] = 0.967;
    tyvek600Refl[18] = 0.967;
    tyvek600Refl[19] = 0.968;
    tyvek600Refl[20] = 0.968;
    tyvek600Refl[21] = 0.970; // 400 nm
    tyvek600Refl[22] = 0.970;
    tyvek600Refl[23] = 0.970;
    tyvek600Refl[24] = 0.970;
    tyvek600Refl[25] = 0.971;
    tyvek600Refl[26] = 0.971; // 450 nm
    tyvek600Refl[27] = 0.970;
    tyvek600Refl[28] = 0.970;
    tyvek600Refl[29] = 0.969;
    tyvek600Refl[30] = 0.968;
    tyvek600Refl[31] = 0.968; // 500 nm
    tyvek600Refl[32] = 0.967;
    tyvek600Refl[33] = 0.967;
    tyvek600Refl[34] = 0.966;
    tyvek600Refl[35] = 0.966;
    tyvek600Refl[36] = 0.966; // 550 nm
    tyvek600Refl[37] = 0.966;
    tyvek600Refl[38] = 0.965;
    tyvek600Refl[39] = 0.965;
    tyvek600Refl[40] = 0.965;
    tyvek600Refl[41] = 0.965; // 600 nm
    tyvek600Refl[42] = 0.965;
    tyvek600Refl[43] = 0.964;
    tyvek600Refl[44] = 0.964;
    tyvek600Refl[45] = 0.963;
    tyvek600Refl[46] = 0.963; // 650 nm
    tyvek600Refl[47] = 0.963;
    tyvek600Refl[48] = 0.963;
    tyvek600Refl[49] = 0.963;
    tyvek600Refl[50] = 0.963;
    tyvek600Refl[51] = 0.962; // 700 nm
    tyvek600Refl[52] = 0.962;
    tyvek600Refl[53] = 0; // 720 nm
    tyvek600Refl[54] = 0; // 2058.2 nm
    
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					CreateMaterials()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::CreateMaterials()
{
    //
    //	Isotopes
    //	Atomic weights come from webelements.com
    //
    G4Isotope *H1 = new G4Isotope( "H1", 1, 1, 1.007825035*g/mole );
    G4Isotope *H2 = new G4Isotope( "H2", 1, 2, 2.014101779*g/mole );
    
    G4Isotope *Li6 = new G4Isotope( "Li6", 3, 6, 6.0151223*g/mole );
    G4Isotope *Li7 = new G4Isotope( "Li7", 3, 7, 7.0160040*g/mole );
    
    G4Isotope *Be9 = new G4Isotope( "Be9", 4, 9, 9.012182*g/mole );
    
    G4Isotope *B10 = new G4Isotope( "B10", 5, 10, 10.012936992*g/mole );
    G4Isotope *B11 = new G4Isotope( "B11", 5, 11, 11.009305406*g/mole );
    
    G4Isotope *C12 = new G4Isotope( "C12", 6, 12, 12.*g/mole );
    G4Isotope *C13 = new G4Isotope( "C13", 6, 13, 13.003354826*g/mole );
    
    G4Isotope *N14 = new G4Isotope( "N14", 7, 14, 14.00307400478*g/mole );
    G4Isotope *N15 = new G4Isotope( "N15", 7, 15, 15.00010889823*g/mole );
    
    G4Isotope *O16 = new G4Isotope( "O16", 8, 16, 15.99491463*g/mole );
    G4Isotope *O17 = new G4Isotope( "O17", 8, 17, 16.9991312*g/mole );
    G4Isotope *O18 = new G4Isotope( "O18", 8, 18, 17.9991603*g/mole );
    
    G4Isotope *F19 = new G4Isotope( "F19", 9, 19, 18.99840322*g/mole );
    
    G4Isotope *Na23 = new G4Isotope( "Na23", 11, 23, 22.9897677*g/mole );
    
    G4Isotope *Mg24 = new G4Isotope( "Mg24", 12, 24, 23.9850417*g/mole );
    G4Isotope *Mg25 = new G4Isotope( "Mg25", 12, 25, 24.9858369*g/mole );
    G4Isotope *Mg26 = new G4Isotope( "Mg26", 12, 26, 25.9825929*g/mole );
    
    G4Isotope *Al27 = new G4Isotope( "Al27", 13, 27, 26.9815386*g/mole );
    
    G4Isotope *Si28 = new G4Isotope( "Si28", 14, 28, 27.9769271*g/mole );
    G4Isotope *Si29 = new G4Isotope( "Si29", 14, 29, 28.9764949*g/mole );
    G4Isotope *Si30 = new G4Isotope( "Si30", 14, 30, 29.9737707*g/mole );
    
    G4Isotope *S32 = new G4Isotope( "S32", 16, 32, 31.97207070*g/mole );
    G4Isotope *S33 = new G4Isotope( "S33", 16, 33, 32.97145843*g/mole );
    G4Isotope *S34 = new G4Isotope( "S34", 16, 34, 33.96786665*g/mole );
    G4Isotope *S36 = new G4Isotope( "S36", 16, 36, 35.96708062*g/mole );
    
    G4Isotope *Cl35 = new G4Isotope( "Cl35", 17, 35, 34.968852721*g/mole );
    G4Isotope *Cl37 = new G4Isotope( "Cl37", 17, 37, 36.96590262*g/mole );
    
    G4Isotope *Ar36 = new G4Isotope( "Ar36", 18, 36, 35.96754552*g/mole );
    G4Isotope *Ar38 = new G4Isotope( "Ar38", 18, 38, 37.9627325 *g/mole );
    G4Isotope *Ar40 = new G4Isotope( "Ar40", 18, 40, 39.9623837 *g/mole );
    
    G4Isotope *Ti46 = new G4Isotope( "Ti46", 22, 46, 45.9526294*g/mole );
    G4Isotope *Ti47 = new G4Isotope( "Ti47", 22, 47, 46.9517640*g/mole );
    G4Isotope *Ti48 = new G4Isotope( "Ti48", 22, 48, 47.9479473*g/mole );
    G4Isotope *Ti49 = new G4Isotope( "Ti49", 22, 49, 48.9478711*g/mole );
    G4Isotope *Ti50 = new G4Isotope( "Ti50", 22, 50, 49.9447921*g/mole );
    
    G4Isotope *Cr50 = new G4Isotope( "Cr50", 24, 50, 49.9460464*g/mole );
    G4Isotope *Cr52 = new G4Isotope( "Cr52", 24, 52, 51.9405098*g/mole );
    G4Isotope *Cr53 = new G4Isotope( "Cr53", 24, 53, 52.9406513*g/mole );
    G4Isotope *Cr54 = new G4Isotope( "Cr54", 24, 54, 53.9388825*g/mole );
    
    G4Isotope *Fe54 = new G4Isotope( "Fe54", 26, 54, 53.9396127*g/mole );
    G4Isotope *Fe56 = new G4Isotope( "Fe56", 26, 56, 55.9349393*g/mole );
    G4Isotope *Fe57 = new G4Isotope( "Fe57", 26, 57, 56.9353958*g/mole );
    G4Isotope *Fe58 = new G4Isotope( "Fe58", 26, 58, 57.9332773*g/mole );
    
    G4Isotope *Ni58 = new G4Isotope ("Ni58", 28, 58, 57.9353429*g/mole );
    G4Isotope *Ni60 = new G4Isotope ("Ni60", 28, 60, 59.9307863*g/mole );
    G4Isotope *Ni61 = new G4Isotope ("Ni61", 28, 61, 60.9310560*g/mole );
    G4Isotope *Ni62 = new G4Isotope ("Ni62", 28, 62, 61.9283451*g/mole );
    G4Isotope *Ni64 = new G4Isotope ("Ni64", 28, 64, 63.9279659*g/mole );
    
    G4Isotope *Cu63 = new G4Isotope( "Cu63", 29, 63, 62.92959898*g/mole );
    G4Isotope *Cu65 = new G4Isotope( "Cu65", 29, 65, 64.9277929*g/mole );
    
    G4Isotope *Zn64 = new G4Isotope( "Zn64", 30, 64, 63.9291448*g/mole );
    G4Isotope *Zn66 = new G4Isotope( "Zn66", 30, 66, 65.9260347*g/mole );
    G4Isotope *Zn67 = new G4Isotope( "Zn67", 30, 67, 66.9271291*g/mole );
    G4Isotope *Zn68 = new G4Isotope( "Zn68", 30, 68, 67.9248459*g/mole );
    G4Isotope *Zn70 = new G4Isotope( "Zn70", 30, 70, 69.925325*g/mole );
    
    G4Isotope *Mo92 = new G4Isotope( "Mo92", 42, 92, 91.906809*g/mole );
    G4Isotope *Mo94 = new G4Isotope( "Mo94", 42, 94, 93.9050853*g/mole );
    G4Isotope *Mo95 = new G4Isotope( "Mo95", 42, 95, 94.9058411*g/mole );
    G4Isotope *Mo96 = new G4Isotope( "Mo96", 42, 96, 95.9046785*g/mole );
    G4Isotope *Mo97 = new G4Isotope( "Mo97", 42, 97, 96.9060205*g/mole );
    G4Isotope *Mo98 = new G4Isotope( "Mo98", 42, 98, 97.9054073*g/mole );
    G4Isotope *Mo100 = new G4Isotope( "Mo100", 42, 100, 99.907477*g/mole );
    
    G4Isotope *Xe124 = new G4Isotope( "Xe124", 54, 124, 123.9058942 *g/mole );
    G4Isotope *Xe126 = new G4Isotope( "Xe126", 54, 126, 125.904281 *g/mole );
    G4Isotope *Xe128 = new G4Isotope( "Xe128", 54, 128, 127.9035312 *g/mole );
    G4Isotope *Xe129 = new G4Isotope( "Xe129", 54, 129, 128.9047801 *g/mole );
    G4Isotope *Xe130 = new G4Isotope( "Xe130", 54, 130, 129.9035094 *g/mole );
    G4Isotope *Xe131 = new G4Isotope( "Xe131", 54, 131, 130.905072 *g/mole );
    G4Isotope *Xe132 = new G4Isotope( "Xe132", 54, 132, 131.904144 *g/mole );
    G4Isotope *Xe134 = new G4Isotope( "Xe134", 54, 134, 133.905395 *g/mole );
    G4Isotope *Xe136 = new G4Isotope( "Xe136", 54, 136, 135.907214 *g/mole );
    
    G4Isotope *Ce136 = new G4Isotope( "Ce136", 58, 136, 135.907172*g/mole );
    G4Isotope *Ce138 = new G4Isotope( "Ce138", 58, 138, 137.905991*g/mole );
    G4Isotope *Ce140 = new G4Isotope( "Ce140", 58, 140, 139.905438*g/mole );
    G4Isotope *Ce142 = new G4Isotope( "Ce142", 58, 142, 141.909244*g/mole );
    
    G4Isotope *W180 = new G4Isotope( "W180", 74, 180, 179.946701*g/mole );
    G4Isotope *W182 = new G4Isotope( "W182", 74, 182, 181.948202*g/mole );
    G4Isotope *W183 = new G4Isotope( "W183", 74, 183, 182.950220*g/mole );
    G4Isotope *W184 = new G4Isotope( "W184", 74, 184, 183.950928*g/mole );
    G4Isotope *W186 = new G4Isotope( "W186", 74, 186, 185.954357*g/mole );
    
    G4Isotope *Au197 = new G4Isotope( "Au197", 79, 197, 196.966543*g/mole );
    
    G4Isotope *Pb204 = new G4Isotope( "Pb204", 82, 204, 203.973020*g/mole );
    G4Isotope *Pb206 = new G4Isotope( "Pb206", 82, 206, 205.974440*g/mole );
    G4Isotope *Pb207 = new G4Isotope( "Pb207", 82, 207, 206.975872*g/mole );
    G4Isotope *Pb208 = new G4Isotope( "Pb208", 82, 208, 207.976627*g/mole );
    
    G4Isotope *Gd152 = new G4Isotope( "Gd152", 64, 152, 151.919786*g/mole );
    G4Isotope *Gd154 = new G4Isotope( "Gd154", 64, 154, 153.920861*g/mole );
    G4Isotope *Gd155 = new G4Isotope( "Gd155", 64, 155, 154.922618*g/mole );
    G4Isotope *Gd156 = new G4Isotope( "Gd156", 64, 156, 155.922118*g/mole );
    G4Isotope *Gd157 = new G4Isotope( "Gd157", 64, 157, 156.923956*g/mole );
    G4Isotope *Gd158 = new G4Isotope( "Gd158", 64, 158, 157.924019*g/mole );
    G4Isotope *Gd160 = new G4Isotope( "Gd160", 64, 160, 159.927049*g/mole );
    
    G4Isotope *Ca40 = new G4Isotope( "Ca40", 20, 40, 39.9625906*g/mole );
    G4Isotope *Ca42 = new G4Isotope( "Ca42", 20, 42, 41.9586176*g/mole );
    G4Isotope *Ca43 = new G4Isotope( "Ca43", 20, 43, 42.9587662*g/mole );
    G4Isotope *Ca44 = new G4Isotope( "Ca44", 20, 44, 43.9554806*g/mole );
    //G4Isotope *Ca46 = new G4Isotope( "Ca46", 20, 46, 45.953689*g/mole );
    G4Isotope *Ca48 = new G4Isotope( "Ca48", 20, 48, 47.952533*g/mole );
    
    G4Isotope *K39 = new G4Isotope( "K39", 19, 39, 38.9637074*g/mole );
    G4Isotope *K40 = new G4Isotope( "K40", 19, 40, 39.9639992*g/mole );
    G4Isotope *K41 = new G4Isotope( "K41", 19, 41, 40.9618254*g/mole );
    
    G4Isotope *Mn55 = new G4Isotope( "Mn55", 25, 55, 54.9380471*g/mole );
    
    G4Isotope *P31 = new G4Isotope( "P31", 15, 31, 30.9737620*g/mole );
    
    G4Isotope *Co59 = new G4Isotope( "Co59", 27, 59, 58.9331950*g/mole);
    
    //
    //	Elements
    //
    G4Element *natH = new G4Element( "Natural H", "natH", 2 );
    natH->AddIsotope( H1, 99.9885*perCent );
    natH->AddIsotope( H2, 0.0115*perCent );
    
    G4Element *natLi = new G4Element( "Natural Li", "natLi", 2 );
    natLi->AddIsotope( Li6, 7.59*perCent );
    natLi->AddIsotope( Li7, 92.41*perCent );
    
    G4Element *enrLi = new G4Element( "Enriched Li", "enrLi", 2 );
    enrLi->AddIsotope( Li6, 95.00*perCent );
    enrLi->AddIsotope( Li7, 5.00*perCent );
    
    G4Element *natB = new G4Element( "Natural B", "natB", 2 );
    natB->AddIsotope( B10, 19.8*perCent );
    natB->AddIsotope( B11, 80.2*perCent );
    
    G4Element *natBe = new G4Element( "Natural Be", "natBe", 1 );
    natBe->AddIsotope( Be9, 100*perCent );
    
    G4Element *natC = new G4Element( "Natural C", "natC", 2 );
    natC->AddIsotope( C12, 98.93*perCent );
    natC->AddIsotope( C13, 1.07*perCent );
    
    G4Element *natN = new G4Element( "Natural N", "natN", 2 );
    natN->AddIsotope( N14, 99.632*perCent );
    natN->AddIsotope( N15, 0.368*perCent );
    
    G4Element *natO = new G4Element( "Natural O", "natO", 3 );
    natO->AddIsotope( O16, 99.757*perCent );
    natO->AddIsotope( O17, 0.038*perCent );
    natO->AddIsotope( O18, 0.205*perCent );
    
    G4Element *natF = new G4Element( "Natural F", "natF", 1 );
    natF->AddIsotope( F19, 100.*perCent );
    
    G4Element *natNa = new G4Element( "Natural Na", "natNa", 1 );
    natNa->AddIsotope( Na23, 100.*perCent );
    
    G4Element *natMg = new G4Element( "Natural Mg", "natMg", 3 );
    natMg->AddIsotope( Mg24, 78.99*perCent );
    natMg->AddIsotope( Mg25, 10.00*perCent );
    natMg->AddIsotope( Mg26, 11.01*perCent );
    
    G4Element *natAl = new G4Element( "Natural Al", "natAl", 1 );
    natAl->AddIsotope( Al27, 100.*perCent );
    
    G4Element *natSi = new G4Element( "Natural Si", "natSi", 3 );
    natSi->AddIsotope( Si28, 92.2297*perCent );
    natSi->AddIsotope( Si29, 4.6832*perCent );
    natSi->AddIsotope( Si30, 3.0872*perCent );
    
    G4Element *natS = new G4Element( "Natural S", "natS", 4 );
    natS->AddIsotope( S32, 94.93*perCent );
    natS->AddIsotope( S33, 0.76*perCent );
    natS->AddIsotope( S34, 4.29*perCent );
    natS->AddIsotope( S36, 0.02*perCent );
    
    G4Element *natCl = new G4Element( "Natural Cl", "natCl", 2 );
    natCl->AddIsotope( Cl35, 75.78*perCent );
    natCl->AddIsotope( Cl37, 24.22*perCent );
    
    G4Element *natAr = new G4Element( "Natural Ar", "natAr", 3 );
    natAr->AddIsotope( Ar36, 0.3365*perCent );
    natAr->AddIsotope( Ar38, 0.0632*perCent );
    natAr->AddIsotope( Ar40, 99.6003*perCent );
    
    G4Element *natTi = new G4Element( "Natural Ti", "natTi", 5 );
    natTi->AddIsotope( Ti46, 8.25*perCent );
    natTi->AddIsotope( Ti47, 7.44*perCent );
    natTi->AddIsotope( Ti48, 73.72*perCent );
    natTi->AddIsotope( Ti49, 5.41*perCent );
    natTi->AddIsotope( Ti50, 5.18*perCent );
    
    G4Element *natCr = new G4Element( "Natural Cr", "natCr", 4 );
    natCr->AddIsotope( Cr50, 4.345*perCent );
    natCr->AddIsotope( Cr52, 83.789*perCent );
    natCr->AddIsotope( Cr53, 9.501*perCent );
    natCr->AddIsotope( Cr54, 2.365*perCent );
    
    G4Element *natFe = new G4Element( "Natural Fe", "natFe", 4 );
    natFe->AddIsotope( Fe54, 5.845*perCent );
    natFe->AddIsotope( Fe56, 91.754*perCent );
    natFe->AddIsotope( Fe57, 2.119*perCent );
    natFe->AddIsotope( Fe58, 0.282*perCent );
    
    G4Element *natNi = new G4Element( "Natural Ni", "natNi", 5 );
    natNi->AddIsotope( Ni58, 68.077*perCent );
    natNi->AddIsotope( Ni60, 26.223*perCent );
    natNi->AddIsotope( Ni61, 1.14*perCent );
    natNi->AddIsotope( Ni62, 3.634*perCent );
    natNi->AddIsotope( Ni64, 0.926*perCent );
    
    G4Element *natCu = new G4Element( "Natural Cu", "natCu", 2 );
    natCu->AddIsotope( Cu63, 69.17*perCent );
    natCu->AddIsotope( Cu65, 30.83*perCent );
    
    G4Element *natZn = new G4Element( "Natural Zn", "natZn", 5 );
    natZn->AddIsotope( Zn64, 48.63*perCent );
    natZn->AddIsotope( Zn66, 27.90*perCent );
    natZn->AddIsotope( Zn67, 4.10*perCent );
    natZn->AddIsotope( Zn68, 18.75*perCent );
    natZn->AddIsotope( Zn70, 0.62*perCent );
    
    G4Element *natMo = new G4Element( "Natural Mo", "natMo", 7 );
    natMo->AddIsotope( Mo92, 14.84*perCent );
    natMo->AddIsotope( Mo94, 9.25*perCent );
    natMo->AddIsotope( Mo95, 15.92*perCent );
    natMo->AddIsotope( Mo96, 16.68*perCent );
    natMo->AddIsotope( Mo97, 9.55*perCent );
    natMo->AddIsotope( Mo98, 24.13*perCent );
    natMo->AddIsotope( Mo100, 9.63*perCent );
    
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
    
    G4Element *natCe = new G4Element( "Natural Ce", "natCe", 4 );
    natCe->AddIsotope( Ce136, 0.185*perCent );
    natCe->AddIsotope( Ce138, 0.251*perCent );
    natCe->AddIsotope( Ce140, 88.45*perCent );
    natCe->AddIsotope( Ce142, 11.114*perCent );
    
    G4Element *natW = new G4Element( "Natural W", "natW", 5 );
    natW->AddIsotope( W180, 0.12*perCent );
    natW->AddIsotope( W182, 26.50*perCent );
    natW->AddIsotope( W183, 14.31*perCent );
    natW->AddIsotope( W184, 30.64*perCent );
    natW->AddIsotope( W186, 28.43*perCent );
    
    G4Element *natAu = new G4Element( "Natural Au", "natAu", 1 );
    natAu->AddIsotope( Au197, 100.*perCent );
    
    G4Element *natPb = new G4Element( "Natural lead", "natPb", 4 );
    natPb->AddIsotope( Pb204, 1.4*perCent );
    natPb->AddIsotope( Pb206, 24.1*perCent );
    natPb->AddIsotope( Pb207, 22.1*perCent );
    natPb->AddIsotope( Pb208, 52.4*perCent );
    
    G4Element *natGd = new G4Element( "Natural Gd", "natGd", 7 );
    natGd->AddIsotope( Gd152, 0.20*perCent );
    natGd->AddIsotope( Gd154, 2.18*perCent );
    natGd->AddIsotope( Gd155, 14.80*perCent );
    natGd->AddIsotope( Gd156, 20.47*perCent );
    natGd->AddIsotope( Gd157, 15.65*perCent );
    natGd->AddIsotope( Gd158, 24.84*perCent );
    natGd->AddIsotope( Gd160, 21.86*perCent );
    

    G4Element *natCa = new G4Element( "Natural Ca", "natCa", 5);
    natCa->AddIsotope( Ca40, 96.94*perCent );
    natCa->AddIsotope( Ca42, 0.64*perCent );
    natCa->AddIsotope( Ca43, 0.14*perCent );
    natCa->AddIsotope( Ca44, 2.09*perCent );
    natCa->AddIsotope( Ca48, 0.19*perCent );
    
    G4Element* natK = new G4Element( "Natural K", "natK", 3);
    natK->AddIsotope( K39, 93.26*perCent );
    natK->AddIsotope( K40, 0.01*perCent );
    natK->AddIsotope( K41, 6.73*perCent );
    
    G4Element* natMn = new G4Element( "Natural Mn", "natMn", 1);
    natMn->AddIsotope( Mn55, 100.*perCent );
    
    G4Element* natP = new G4Element( "Natural P", "natP", 1);
    natP->AddIsotope( P31, 100.*perCent );
    
    G4Element *natCo = new G4Element( "Natural Co", "natCo", 1 );
    natCo->AddIsotope( Co59, 100*perCent );
    
    //
    //	Materials
    //
    
    // Assuming PMMA -- see
    //	http://en.wikipedia.org/wiki/Poly(methyl_methacrylate)
    acrylic = new G4Material( "acrylic", 1.17*g/cm3, 3 );
    acrylic->AddElement( natC, 5 );
    acrylic->AddElement( natO, 2 );
    acrylic->AddElement( natH, 8 );
    
    //	This composition for air comes from Wikipedia. Note that this assumes
    //	no water content.
    air = new G4Material( "air", 0.0012*g/cm3, 3 );
    air->AddElement( natO, 20.946*perCent );
    air->AddElement( natN, 78.12*perCent );
    air->AddElement( natAr, 0.9340*perCent );
    
    aluminum = new G4Material( "aluminum", 2.7*g/cm3, 1 );
    aluminum->AddElement( natAl, 1 );
    
    beryllium = new G4Material( "beryllium", 1.85*g/cm3, 1 );
    beryllium->AddElement( natBe, 1 );
    
    // using Cu density
    berylliumCopper = new G4Material( "berylliumCopper", 8.920*g/cm3, 2 );
    berylliumCopper->AddElement( natBe, 1 );
    berylliumCopper->AddElement( natCu, 1 );
    
    //	Blackium is defined as a vacuum, so that even on a particle's first step
    //	inside a part made of blackium, it's unlikely to interact.
    blackium = new G4Material( "blackium", 1., 1.008*g/mole, 1.e-25*g/cm3,
                              kStateGas, TEMPERATURE, 3.8e-18*pascal );
    
    copper = new G4Material( "copper", 8.920*g/cm3, 1 );
    copper->AddElement( natCu, 1 );
    
    gadolinium = new G4Material( "gadolinium", 157.25*g/mole, 1 );
    gadolinium->AddElement( natGd, 1 );

    //	For this density of the argon gas, the STP density is 1.784 g/L, but
    //	the pressure was 750 Torr and the temperature was 21.4 C.
    //	1.784 g/L * (750/760) * (273 / (273+21.4)) = 1.632 g/L
    gasAr = new G4Material( "gasAr", 0.00163255*g/cm3, 1 );
    gasAr->AddElement( natAr, 1 );
    
    gasN2 = new G4Material( "gasN2", 0.001251*g/cm3, 1 );
    gasN2->AddElement( natN, 1 );
    
    gasArN2 = new G4Material( "gasArN2", 9.3895e-04*g/cm3, 2 );
    gasArN2->AddMaterial( gasAr, 0.985 );
    gasArN2->AddMaterial( gasN2, 0.015 );
    
    gasXe = new G4Material( "gasXe", N_g, 1, kStateGas, TEMPERATURE,
                           PRESSURE );
    gasXe->AddElement( natXe, 1 );

    highPressureGasXe = new G4Material("highPressureGasXe", 0.41*g/cm3, 1,
                            kStateGas, 298.*kelvin, 48.2 * bar);
    highPressureGasXe->AddElement( natXe, 1 );

    
    gold = new G4Material( "gold", 19.3*g/cm3, 1 );
    gold->AddElement( natAu, 1 );
    
    halon = new G4Material( "halon", 2.16*g/cm3, 2 );
    halon->AddElement( natC, 1 );
    halon->AddElement( natF, 2 );
    
    iron = new G4Material( "iron", 7.874*g/cm3, 1 );
    iron->AddElement( natFe, 1 );
    
    isohexane = new G4Material( "isohexane", 0.653*g/cm3, 2 );
    isohexane->AddElement( natC, 6 );
    isohexane->AddElement( natH, 14 );
    
    tmb = new G4Material( "TMB", 0.932*g/cm3, 4 );
    tmb->AddElement( natC, 3 );
    tmb->AddElement( natH, 9 );
    tmb->AddElement( natB, 1 );
    tmb->AddElement( natO, 3 );
    
    //	Taking a random stab at what the density would be...
    dopedIsohexaneTMB5 = new G4Material( "dopedIsohexaneTMB5", 0.653*g/cm3, 2 );
    dopedIsohexaneTMB5->AddMaterial( isohexane, 95*perCent );
    dopedIsohexaneTMB5->AddMaterial( tmb, 5*perCent );
    
    //	Taking a random stab at what the density would be...
    dopedIsohexaneTMB50 = new G4Material( "dopedIsohexaneTMB50", 0.8*g/cm3, 2 );
    dopedIsohexaneTMB50->AddMaterial( isohexane, 50*perCent );
    dopedIsohexaneTMB50->AddMaterial( tmb, 50*perCent );
    
    dopedIsohexaneGd3 = new G4Material( "dopedIsohexaneGd3", 0.653*g/cm3, 2 );
    dopedIsohexaneGd3->AddMaterial( isohexane, 99.7*perCent );
    dopedIsohexaneGd3->AddMaterial( gadolinium, 0.3*perCent );
    
    dopedIsohexaneGd7 = new G4Material( "dopedIsohexaneGd7", 0.653*g/cm3, 2 );
    dopedIsohexaneGd7->AddMaterial( isohexane, 99.3*perCent );
    dopedIsohexaneGd7->AddMaterial( gadolinium, 0.7*perCent );
    
    // lineaer akyl benzene (LAB) is actually a generic term for a class of
    // chemical compositions
    // this is an avergae for "typical" LAB
    // update: was a percentage, now gives correct atoms/volume
    // using schematic formala CH_1.8
    lab = new G4Material( "lab", 0.860*g/cm3, 2 );
    lab->AddElement( natC, 10 ); //was 35.5*perCent
    lab->AddElement( natH, 18 ); //was 64.5*perCent
   
    polystyrene = new G4Material( "polystyrene", 1.0*g/cm3, 2 );
    polystyrene->AddElement( natC, 8 );
    polystyrene->AddElement( natH, 8 );

    enrLiMaterial = new G4Material("enrLiMaterial",6.*g/mole,1);
    enrLiMaterial->AddElement( enrLi, 1 );

    liDopedPlasticScintillator = new G4Material( "liDopedPlasticScintillator", 1.1*g/cm3, 3);
    liDopedPlasticScintillator->AddMaterial( polystyrene, 27.88*perCent );
    liDopedPlasticScintillator->AddMaterial( acrylic, 70*perCent );
    liDopedPlasticScintillator->AddMaterial( enrLiMaterial, 0.12*perCent );

    ej301 = new G4Material( "ej301", 0.876*g/cm3, 2);
    ej301->AddElement( natC, 199 );
    ej301->AddElement( natH, 241 );
 
    ej200 = new G4Material( "ej200", 1.023*g/cm3, 2);
    ej200->AddElement( natC, 469 );
    ej200->AddElement( natH, 517 );

    // Gd is .1% by mass, but LAB has a molecule mass between 372 and 540 amu
    // so percentage of Gd by number is between .23% and .34%, again take the
    // average
    // update: perCentage is mass fraction so should be .1% Gd
    dopedLABGd = new G4Material( "dopedLABGd", 0.860*g/cm3, 2, kStateLiquid,
                                295.*kelvin, 101325.*pascal );
    dopedLABGd->AddMaterial( lab, 99.9*perCent ); //was 99.71*perCent
    dopedLABGd->AddMaterial( gadolinium, 0.1*perCent ); //was 0.29*perCent
    
    kapton = new G4Material( "kapton", 1.43*g/cm3, 4);
    kapton->AddElement( natC, 22);
    kapton->AddElement( natH, 10);
    kapton->AddElement( natN, 2);
    kapton->AddElement( natO, 5);
    
    lead = new G4Material( "lead", 11.340*g/cm3, 1 );
    lead->AddElement( natPb, 1 );
    
    //	This definition of lead glass specifically comes from Ray-Bar
    //	Engineering Corporation.
    leadGlass = new G4Material( "leadGlass", 5.2*g/cm3, 3 );
    leadGlass->AddElement( natSi, 1 );
    leadGlass->AddElement( natO, 3 );
    leadGlass->AddElement( natPb, 1 );
    
    liquidAr = new G4Material( "liquidAr", 1.40*g/cm3, 1 );
    liquidAr->AddElement( natAr, 1 );
    
    liquidN2 = new G4Material( "liquidN2", 0.808*g/cm3, 1 );
    liquidN2->AddElement( natN, 1 );
    
    liquidXe = new G4Material( "liquidXe", 2.888*g/cm3, 1, kStateLiquid,
                              178.4*kelvin, 2.0*bar );
    liquidXe->AddElement( natXe, 1 );

    liquidXeCold = new G4Material( "liquidXeCold", 3.1*g/cm3, 1, kStateLiquid,
                              164.0*kelvin, 1.0*bar );
    liquidXeCold->AddElement( natXe, 1 );
   
    gasXe15bar = new G4Material( "gasXe15bar", 0.09*g/cm3, 1, kStateGas,
                                 295.*kelvin, 15.*bar );
    gasXe15bar->AddElement( natXe, 1 );
 
    gasXe50bar = new G4Material( "gasXe50bar", 0.43*g/cm3, 1, kStateGas,
                                 295.*kelvin, 50.*bar );
    gasXe50bar->AddElement( natXe, 1 );
   
 
    mineralOil = new G4Material( "mineralOil", 0.8*g/cm3, 2 );
    mineralOil->AddElement( natC, 1 );
    mineralOil->AddElement( natH, 2 );
    
    //  PEEK polyether ether ketone
    //  http://en.wikipedia.org/wiki/PEEK
    peek = new G4Material( "peek", 1.32*g/cm3, 3);
    peek->AddElement( natC, 19);
    peek->AddElement( natO, 3);
    peek->AddElement( natH, 12);
    
    polyethylene = new G4Material( "polyethylene", 0.94*g/cm3, 2 );
    polyethylene->AddElement( natC, 1 );
    polyethylene->AddElement( natH, 2 );
    
    polyFoam = new G4Material( "polyFoam", 0.032*g/cm3, 1 );
    polyFoam->AddMaterial( polyethylene, 100.0*perCent );
    
    polyethyleneBorated = new G4Material( "polyethyleneBorated", 1.19*g/cm3, 3);
    polyethyleneBorated->AddElement( natB, 5.*perCent );
    polyethyleneBorated->AddElement( natC, 81.4*0.95*perCent );
    polyethyleneBorated->AddElement( natH, 18.6*0.95*perCent );
    
    pvc = new G4Material( "pcv", 1.375*g/cm3, 3); // ranged of desnity for rigid
    // PVC is 1.3 to 1.45 g/cm^3,
    // I pick the middle value
    pvc->AddElement( natC, 2);
    pvc->AddElement( natH, 3);
    pvc->AddElement( natCl, 1);
    
    quartz = new G4Material( "quartz", 2.65*g/cm3, 2, kStateSolid, TEMPERATURE,
                            PRESSURE );
    quartz->AddElement( natSi, 1 );
    quartz->AddElement( natO, 2 );
    
    rock = new G4Material( "rock", 2.4*g/cm3, 2 );
    rock->AddElement( natSi, 1 );
    rock->AddElement( natO, 2 );
    
    sapphire = new G4Material( "sapphire", 3.98*g/cm3, 2 );
    sapphire->AddElement( natAl, 2 );
    sapphire->AddElement( natO, 3 );
    
    alumina = new G4Material( "alumina", 3.72*g/cm3, 2 );
    alumina->AddElement( natAl, 2 );
    alumina->AddElement( natO, 3 );
    
    elastomer = new G4Material( "elastomer", 1.17*g/cm3, 4 );
    elastomer->AddElement( natC, 2 );
    elastomer->AddElement( natH, 6 );
    elastomer->AddElement( natSi, 1 );
    elastomer->AddElement( natO, 1 );
    
    nicomic = new G4Material( "nicomic", 8.18*g/cm3, 5 );
    nicomic->AddElement( natSi, 25.46*perCent );
    nicomic->AddElement( natO, 64.09*perCent );
    nicomic->AddElement( natB, 7.09*perCent );
    nicomic->AddElement( natNa, 2.50*perCent );
    nicomic->AddElement( natAl, 0.86*perCent );
    
    inconel = new G4Material( "inconel", 8.22*g/cm3, 4 );
    inconel->AddElement( natNi,72.*perCent );
    inconel->AddElement( natCr,17.*perCent );
    inconel->AddElement( natFe, 10.*perCent );
    inconel->AddElement( natMn, 1.*perCent );
    
    //steel = new G4Material( "steel", 7.84*g/cm3, 4 );
    //steel->AddElement( natFe, 84.*perCent );
    //steel->AddElement( natC, 14.*perCent );
    //steel->AddElement( natCr, 1.*perCent );
    //steel->AddElement( natMo, 1.*perCent );
    
    steel = new G4Material( "steel", 8.03*g/cm3, 7 ); //AK Steel Corporation
    steel->AddElement( natC, 0.04*perCent );
    //steel->AddElement( natMn,1.00*perCent );
    //steel->AddElement( natP,0.022*perCent );
    steel->AddElement( natS,0.015*perCent );
    steel->AddElement( natSi,0.37*perCent );
    steel->AddElement( natCr, 19.*perCent );
    steel->AddElement( natNi, 10.*perCent );
    steel->AddElement( natN, 0.05*perCent );
    steel->AddElement( natFe, 70.525*perCent );
    
    ss316 = new G4Material( "ss316", 7.99*g/cm3, 4 );	//	Answers.com (not
    //	best source)
    ss316->AddElement( natFe, 68.5*perCent );
    ss316->AddElement( natCr, 17.*perCent );
    ss316->AddElement( natNi, 12.*perCent );
    ss316->AddElement( natMo, 2.5*perCent );
    
    teflon = new G4Material( "teflon", 2.16*g/cm3, 2 );
    teflon->AddElement( natC, 1 );
    teflon->AddElement( natF, 2 );
    
    titanium = new G4Material( "titanium", 4.507*g/cm3, 1 );
    titanium->AddElement( natTi, 1 );
    
    tpb = new G4Material( "tpb", 1.079*g/cm3, 2 );
    tpb->AddElement( natH, 22 );
    tpb->AddElement( natC, 28 );
    
    tungsten = new G4Material( "tungsten", 19.250*g/cm3, 1 );
    tungsten->AddElement( natW, 1 );
    
    heavyMet = new G4Material("heavyMet", 18.5*g/cm3, 3);
    heavyMet->AddElement( natW, 97*perCent );
    heavyMet->AddElement( natNi, 2.1*perCent );
    heavyMet->AddElement( natFe, .9*perCent );
    
    tyvek = new G4Material( "tyvek", 2.16*g/cm3, 2 );
    tyvek->AddElement( natC, 2 );
    tyvek->AddElement( natH, 4 );
    
    lzSpecRef = new G4Material( "lzSpecRef", 1.29*g/cm3, 2 );
    lzSpecRef->AddElement( natC, 2 );
    lzSpecRef->AddElement( natH, 6 );
    
    
    tyvek600 = new G4Material( "tyvek600", 2.16*g/cm3, 2 );
    tyvek600->AddElement( natC, 2 );
    tyvek600->AddElement( natH, 4 );
    
    vacuum = new G4Material( "vacuum", 1., 1.008*g/mole, 1.e-25*g/cm3,
                            kStateGas, TEMPERATURE, 3.8e-18*pascal );
    
    water = new G4Material( "water", 1*g/cm3, 2 );
    water->AddElement( natH, 2 );
    water->AddElement( natO, 1 );
    
    dopedWater3 = new G4Material( "dopedWater3", 1*g/cm3, 2);
    dopedWater3->AddMaterial( water, 99.7*perCent );
    dopedWater3->AddMaterial( gadolinium, 0.3*perCent );
    
    dopedWater7 = new G4Material( "dopedWater7", 1*g/cm3, 2);
    dopedWater7->AddMaterial( water, 99.3*perCent );
    dopedWater7->AddMaterial( gadolinium, 0.7*perCent );
    
    // Defining materials to be combined for enriched 6Li-doped glass
    G4Material *siO2 = new G4Material( "siO2", 2.65*g/cm3, 2);
    siO2->AddElement( natSi, 1 );
    siO2->AddElement( natO, 2 );
    
    G4Material *mgO = new G4Material( "mgO", 3.58*g/cm3, 2);
    mgO->AddElement( natMg, 1 );
    mgO->AddElement( natO, 1 );
    
    G4Material *al2O3 = new G4Material( "al2O3", 3.98*g/cm3, 2);
    al2O3->AddElement( natAl, 2 );
    al2O3->AddElement( natO, 3 );
    
    G4Material *ce2O3 = new G4Material( "ce2O3", 6.2*g/cm3, 2);
    ce2O3->AddElement( natCe, 2 );
    ce2O3->AddElement( natO, 3 );
    
    // density of this may be off, but it shouldn't matter since it is
    // combined into the GS-20 and KG-2 glass
    G4Material *li2OEnriched = new G4Material( "li2OEnriched", 2.0*g/cm3, 2);
    li2OEnriched->AddElement( enrLi, 2 );
    li2OEnriched->AddElement( natO, 1 );
    
    // The accuracy of these compositions is in doubt for the time being
    gs20Glass = new G4Material( "GS20Glass", 2.5*g/cm3, 5);
    gs20Glass->AddMaterial( siO2, 56.0*perCent );
    gs20Glass->AddMaterial( mgO, 4.0*perCent );
    gs20Glass->AddMaterial( al2O3, 18.0*perCent );
    gs20Glass->AddMaterial( ce2O3, 4.0*perCent );
    gs20Glass->AddMaterial( li2OEnriched, 18.0*perCent );
    
    // The accuracy of these compositions is in doubt for the time being
    kg2Glass = new G4Material( "KG2Glass", 2.42*g/cm3, 3);
    kg2Glass->AddMaterial( siO2, 75.0*perCent );
    kg2Glass->AddMaterial( ce2O3, 4.6*perCent );
    kg2Glass->AddMaterial( li2OEnriched, 20.4*perCent );
    
    //bnc cable: copper + steel + teflon
    //density is weighted avergae of all three
    bncCable = new G4Material( "bncCable", 3.66*g/cm3, 3 );
    bncCable->AddMaterial( copper, 2.5*perCent );
    bncCable->AddMaterial( steel, 22.8*perCent );
    bncCable->AddMaterial( teflon, 74.7*perCent );
    
    //material for LZ PMT cabling conduit: bnc cable + stainless steel + vacuum
    //density is weighted avergae of all three
    //based on 247+90 PMTs, 700 cables
    lzPMTCablingConduit = new G4Material( "lzPMTCablingConduit", 1.85*g/cm3, 3 );
    lzPMTCablingConduit->AddMaterial( bncCable, 3.6*perCent );
    lzPMTCablingConduit->AddMaterial( ss316, 21.5*perCent );
    lzPMTCablingConduit->AddMaterial( vacuum, 74.9*perCent );
    
    //material for LZ thermosyphon conduit: ss316 + liquid Xenon + vaccum
    //density is weighted avergae of the three
    lzThermosyphonConduit = new G4Material( "lzThermosyphonConduit", 1.15*g/cm3, 3 );
    lzThermosyphonConduit->AddMaterial( vacuum, 84.8*perCent );
    lzThermosyphonConduit->AddMaterial( ss316, 13.9*perCent );
    lzThermosyphonConduit->AddMaterial( liquidXe, 1.3*perCent );
    
    //material for LZ PMT bottom conduit: bnc cable + ss316 + vacuum + liquid xenon
    //density is cross-sectional area weighted average of all four
    //based on 247+102 PMTs, 730 cables
    lzPMTBottomConduit = new G4Material( "lzPMTBottomConduit", 1.45*g/cm3, 4 );
    lzPMTBottomConduit->AddMaterial( bncCable, 4.4*perCent );
    lzPMTBottomConduit->AddMaterial( ss316, 15.7*perCent );
    lzPMTBottomConduit->AddMaterial( vacuum, 78.7*perCent );
    lzPMTBottomConduit->AddMaterial( liquidXe, 1.2*perCent );
    
    //material for LZ HV Umbilical conduit: copper + plastic + ss316 + vacuum + liquid xenon
    //density is cross-sectional area weighted average of all five
    lzHVUmbilicalConduit = new G4Material( "lzHVUmbilicalConduit", 1.22*g/cm3, 5 );
    lzHVUmbilicalConduit->AddMaterial( copper, 1.0*perCent );
    lzHVUmbilicalConduit->AddMaterial( ss316, 3.9*perCent );
    lzHVUmbilicalConduit->AddMaterial( vacuum, 59.2*perCent );
    lzHVUmbilicalConduit->AddMaterial( liquidXe, 23.9*perCent );
    lzHVUmbilicalConduit->AddMaterial( polyethylene, 12.*perCent );
    
    //material for LZ siphon support tube: liquid xenon + steel
    //density is cross-sectional area weighted average of all two
    lzSiphonTube = new G4Material( "lzSiphonTube", 3.53*g/cm3, 2 );
    lzSiphonTube->AddMaterial( ss316, 11.1*perCent );
    lzSiphonTube->AddMaterial( liquidXe, 88.9*perCent );
    
    // The cavern rock is made up of 7.2% Al, 5.6% Ca, 9.9% Fe, 1.2% H, 0.2% K, 4.2% Mg,
    // 0.1% Mn, 2.1% Na, 48.4% O, 0.03% P, 20.4% Si, 0.7% Ti
    cavRock = new G4Material("cavRock",2.70*g/cm3,12);
    cavRock->AddElement(natAl, 0.072);
    cavRock->AddElement(natCa, 0.056);
    cavRock->AddElement(natFe, 0.0987);
    cavRock->AddElement(natH, 0.012);
    cavRock->AddElement(natK, 0.0017);
    cavRock->AddElement(natMg, 0.042);
    cavRock->AddElement(natMn, 0.0010);
    cavRock->AddElement(natNa, 0.021);
    cavRock->AddElement(natO, 0.484);
    cavRock->AddElement(natP, 0.00030);
    cavRock->AddElement(natSi, 0.204);
    cavRock->AddElement(natTi, 0.0073);
   
    portlandConcrete = new G4Material("portlandConcrete",2.3*g/cm3,10);
    portlandConcrete->AddElement(natH, 0.01);
    portlandConcrete->AddElement(natC, 0.001);
    portlandConcrete->AddElement(natO, 0.529107);
    portlandConcrete->AddElement(natNa, 0.016);
    portlandConcrete->AddElement(natMg, 0.002);
    portlandConcrete->AddElement(natAl, 0.033872);
    portlandConcrete->AddElement(natSi, 0.337021);
    portlandConcrete->AddElement(natK, 0.013);
    portlandConcrete->AddElement(natCa, 0.044);
    portlandConcrete->AddElement(natFe, 0.014);
 
    // The kovar is made up of 53.49% Fe, 29% Ni, 17% Co, <0.01% C, 0.2% Si, 0.3% Mn
    // (see https://en.wikipedia.org/wiki/Kovar)
    
    kovar = new G4Material("Kovar",8.0*g/cm3,6);
    kovar->AddElement(natFe,0.5349);
    kovar->AddElement(natNi,0.29);
    kovar->AddElement(natCo,0.17);
    kovar->AddElement(natC,0.0001);
    kovar->AddElement(natSi,0.002);
    kovar->AddElement(natMn,0.003);
    
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetOpticalProperties()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetOpticalProperties()
{
    //	Create material properties tables and assign values
    //	for optical properties
    
    //	For those properties that are constant, we simply define an energy
    //	array with the largest and smallest values.
    const G4int NUM_PP = num_pp;
    const G4int NUM_PP_ConstProp = num_pp_constProp;
    const G4int nWaterEntries = num_water;
    const G4int lab_Rayleigh_entries = num_lab_Rayleigh;
    const G4int acrylicTransmittance_entries = num_acrylicTransmittance;
    photonEnergies_ConstProp = new G4double[NUM_PP_ConstProp];
    photonEnergies_ConstProp[0] = photonEnergies[0];
    photonEnergies_ConstProp[1] = photonEnergies[NUM_PP-1];
    
    acrylicMat = new G4MaterialPropertiesTable();
    acrylicMat->AddProperty( "REFLECTIVITY", acrylicTransmittance_energies, acrylicReflectivity,
                            acrylicTransmittance_entries);
    acrylicMat->AddProperty( "TRANSMITTANCE", acrylicTransmittance_energies, acrylicTransmittance,
                            acrylicTransmittance_entries);
    acrylic->SetMaterialPropertiesTable( acrylicMat );
    
    airMat = new G4MaterialPropertiesTable();
    air->SetMaterialPropertiesTable( airMat );
    
    aluminumMat = new G4MaterialPropertiesTable();
    aluminum->SetMaterialPropertiesTable( aluminumMat );
    
    berylliumMat = new G4MaterialPropertiesTable();
    beryllium->SetMaterialPropertiesTable( berylliumMat );
    
    berylliumCopperMat = new G4MaterialPropertiesTable();
    berylliumCopper->SetMaterialPropertiesTable( berylliumCopperMat );
    
    copperMat = new G4MaterialPropertiesTable();
    copper->SetMaterialPropertiesTable( copperMat );
    
    //	The gas argon properties are set to those of air. This is a terrible
    //	approximation, but that's what we have for now.
    gasArMat = new G4MaterialPropertiesTable();
    gasAr->SetMaterialPropertiesTable( gasArMat );
    
    //	The gas argon/N2 properties are also set to those of air. This is still
    //	a terrible approximation, but that's what we have for now.
    gasArN2Mat = new G4MaterialPropertiesTable();
    gasArN2->SetMaterialPropertiesTable( gasArN2Mat );
    
    //	The gas nitrogen properties are set to those of air. This is a terrible
    //	approximation, but that's what we have for now.
    gasN2Mat = new G4MaterialPropertiesTable();
    gasN2->SetMaterialPropertiesTable( gasN2Mat );
    
    //	Gas Xe scintillation (S2 + small S1)
    gasXeMat = new G4MaterialPropertiesTable();
    highPressureGasXeMat = new G4MaterialPropertiesTable();
    
    //	Gas Electric Fields (all Gas is same field OR 3 different regions: pmt
    //	window to top to anode to surface)
//    G4double setField = baccManager->GetElectricFieldGas();
//    gasXeMat->AddConstProperty( "ELECTRICFIELD", setField  );
//    setField = baccManager->GetElectricFieldGasWindow();
//    gasXeMat->AddConstProperty( "ELECTRICFIELDWINDOW", setField  );
//    setField = baccManager->GetElectricFieldGasTop();
//    gasXeMat->AddConstProperty( "ELECTRICFIELDTOP", setField  );
//    setField = baccManager->GetElectricFieldGasAnode();
//    gasXeMat->AddConstProperty( "ELECTRICFIELDANODE", setField  );
//    
//    gasXeMat->AddConstProperty( "TOTALNUM_INT_SITES", -1 );
//    //save all properties
    gasXe->SetMaterialPropertiesTable( gasXeMat ); //I'm uncommenting this because it was commented for some reason, and if it's commented, optical propagation won't work... -Ryan
    highPressureGasXe->SetMaterialPropertiesTable( gasXeMat ); //I'm uncommenting this because it was commented for some reason, and if it's commented, optical propagation won't work... -Ryan
    
    goldMat = new G4MaterialPropertiesTable();
    gold->SetMaterialPropertiesTable( goldMat );
    
    gs20GlassMat = new G4MaterialPropertiesTable();
    gs20Glass->SetMaterialPropertiesTable( gs20GlassMat );
    
    //*** Halon material definitions ***//
    halonMat = new G4MaterialPropertiesTable();
    G4double *halonSpeclobe;
    halonSpeclobe = new G4double[nWaterEntries];
    G4double *halonSpecspike;
    halonSpecspike = new G4double[nWaterEntries];
    G4double *halonBackscat;
    halonBackscat = new G4double[nWaterEntries];
    G4double *halonEff;
    halonEff = new G4double[nWaterEntries];
    
    for(G4int i=0; i<nWaterEntries; i++){
        halonSpeclobe[i] = 0.;
        halonSpecspike[i] = 0.;
        halonBackscat[i] = 0.;
        halonEff[i] = 1.;
    }
    
    halonMat->AddProperty( "SPECULARLOBECONSTANT", waterPhotonEnergies,
                          halonSpeclobe, nWaterEntries );
    halonMat->AddProperty( "SPECULARSPIKECONSTANT", waterPhotonEnergies,
                          halonSpecspike, nWaterEntries );
    halonMat->AddProperty( "BACKSCATTERCONSTANT", waterPhotonEnergies,
                          halonBackscat, nWaterEntries );
    halonMat->AddProperty( "EFFICIENCY", waterPhotonEnergies, halonEff,
                          nWaterEntries );
    
    halon->SetMaterialPropertiesTable( halonMat );
    
    ironMat = new G4MaterialPropertiesTable();
    iron->SetMaterialPropertiesTable( ironMat );
    
    isohexaneMat = new G4MaterialPropertiesTable();
    isohexaneMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                              isohexaneRindex, NUM_PP_ConstProp );
    isohexane->SetMaterialPropertiesTable( isohexaneMat );
    
    labMat = new G4MaterialPropertiesTable();
    labMat->AddProperty( "RINDEX", photonEnergies_ConstProp,labRindex,
                        NUM_PP_ConstProp );
    lab->SetMaterialPropertiesTable( labMat );
    
    //*** DopedLABGd material definitions ***//
    // Scintillator
    dopedLABGdMat = new G4MaterialPropertiesTable();
    dopedLABGdMat->AddProperty( "RINDEX", photonEnergies_ConstProp, labRindex,
                               NUM_PP_ConstProp );
    dopedLABGdMat->AddProperty( "ABSLENGTH", labAbsorptionEnergies,
                               labAbsorption, num_labAbs );
    //dopedLABGdMat->AddProperty( "ABSLENGTH", tempLABAbsEnergies,
    //        tempLABAbsLength, NUM_PP_ConstProp );//temp
    dopedLABGdMat->AddProperty( "RAYLEIGH", lab_Rayleigh_energy,
                               lab_Rayleigh_length, lab_Rayleigh_entries );
    dopedLABGdMat->AddProperty("FASTCOMPONENT", labEmissionEnergies,
                               labEmission, num_labEmi);
    dopedLABGdMat->AddProperty("SLOWCOMPONENT", labEmissionEnergies,
                               labEmission, num_labEmi);
    
    dopedLABGdMat->AddConstProperty("SCINTILLATIONYIELD", 9000./MeV);
    dopedLABGdMat->AddConstProperty("RESOLUTIONSCALE", 2.5); //2.5 Minfang
    dopedLABGdMat->AddConstProperty("FASTTIMECONSTANT",  4.*ns);
    dopedLABGdMat->AddConstProperty("SLOWTIMECONSTANT", 17.*ns);
    dopedLABGdMat->AddConstProperty("YIELDRATIO", 0.9);
    dopedLABGdMat->AddConstProperty( "TOTALNUM_INT_SITES", -1 ); //for NEST
    dopedLABGd->SetMaterialPropertiesTable( dopedLABGdMat );
    //dopedLABGd->GetIonisation()->SetBirksConstant(0.1*mm/MeV);
    //Birk's Law constant
    
    kaptonMat = new G4MaterialPropertiesTable();
    kapton->SetMaterialPropertiesTable( kaptonMat );
    
    kg2GlassMat = new G4MaterialPropertiesTable();
    kg2Glass->SetMaterialPropertiesTable( kg2GlassMat );
    
    leadMat = new G4MaterialPropertiesTable();
    lead->SetMaterialPropertiesTable( leadMat );
    
    leadGlassMat = new G4MaterialPropertiesTable();
    leadGlass->SetMaterialPropertiesTable( leadGlassMat );
    
    liquidArMat = new G4MaterialPropertiesTable();
    liquidAr->SetMaterialPropertiesTable( liquidArMat );
    
    liquidN2Mat = new G4MaterialPropertiesTable();
    liquidN2->SetMaterialPropertiesTable( liquidN2Mat );
    
    //*** Liquid Xe material definitions ***//
    liquidXeMat = new G4MaterialPropertiesTable();
    
    // Electric field used to determine the scintillation quenching as the
    // recombination probability decreases with increasing field.
    //  Either set all Liq. Fields same OR use 4 regions (From surf to bot pmts)
//    setField = baccManager->GetElectricFieldLiquid();
//    liquidXeMat->AddConstProperty( "ELECTRICFIELD", setField  );
//    setField = baccManager->GetElectricFieldLiquidSurface();
//    liquidXeMat->AddConstProperty( "ELECTRICFIELDSURFACE", setField  );
//    setField = baccManager->GetElectricFieldLiquidGate();
//    liquidXeMat->AddConstProperty( "ELECTRICFIELDGATE", setField  );
//    setField = baccManager->GetElectricFieldLiquidCathode();
//    liquidXeMat->AddConstProperty( "ELECTRICFIELDCATHODE", setField  );
//    setField = baccManager->GetElectricFieldLiquidBottom();
//    liquidXeMat->AddConstProperty( "ELECTRICFIELDBOTTOM", setField  );
//    
//    liquidXeMat->AddConstProperty( "TOTALNUM_INT_SITES", -1 );
    liquidXe->SetMaterialPropertiesTable( liquidXeMat ); //Likewise added back in - this was commented for some reason. -Ryan
    
    
    //liquidXeColdMat = new G4MaterialsPropertiesTable();
    //liquidXeCold->SetMaterialPropertiesTable( liquidXeColdMat );
    

    //gasXe15barMat = new G4MaterialsPropertiesTable();
    //gasXe15bar->SetMaterialPropertiesTable( gasXe15barMat );
   
    //gasXe50barMat = new G4MaterialsPropertiesTable();
    //gasXe50barMat->SetMaterialPropertiesTable( gasXe50barMat );   

    mineralOilMat = new G4MaterialPropertiesTable();
    mineralOilMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                               mineralOilRIndex, NUM_PP_ConstProp );
    mineralOil->SetMaterialPropertiesTable( mineralOilMat );
    
    peekMat = new G4MaterialPropertiesTable();
    peek->SetMaterialPropertiesTable( peekMat );
    
    //	These values were stolen wholesale from the Teflon properties //
    polyethyleneMat = new G4MaterialPropertiesTable();
    G4double *polyethyleneSpeclobe;
    polyethyleneSpeclobe = new G4double[NUM_PP_ConstProp];
    polyethyleneSpeclobe[0] = polyethyleneSpeclobe[1] = 0;
    G4double *polyethyleneSpecspike;
    polyethyleneSpecspike = new G4double[NUM_PP_ConstProp];
    polyethyleneSpecspike[0] = polyethyleneSpecspike[1] = 0;
    G4double *polyethyleneBackscat;
    polyethyleneBackscat = new G4double[NUM_PP_ConstProp];
    polyethyleneBackscat[0] = polyethyleneBackscat[1] = 0;
    G4double *polyethyleneEff;
    polyethyleneEff = new G4double[NUM_PP_ConstProp];
    polyethyleneEff[0] = polyethyleneEff[1] = 1;
    
    polyethyleneMat->AddProperty( "SPECULARLOBECONSTANT",
                                 photonEnergies_ConstProp, polyethyleneSpeclobe, NUM_PP_ConstProp );
    polyethyleneMat->AddProperty( "SPECULARSPIKECONSTANT",
                                 photonEnergies_ConstProp, polyethyleneSpecspike, NUM_PP_ConstProp );
    polyethyleneMat->AddProperty( "BACKSCATTERCONSTANT",
                                 photonEnergies_ConstProp, polyethyleneBackscat, NUM_PP_ConstProp );
    polyethyleneMat->AddProperty( "EFFICIENCY", photonEnergies_ConstProp,
                                 polyethyleneEff, NUM_PP_ConstProp );
    
    polyFoamMat = new G4MaterialPropertiesTable();
    polyFoam->SetMaterialPropertiesTable( polyFoamMat );
    
    pvcMat = new G4MaterialPropertiesTable();
    pvc->SetMaterialPropertiesTable( pvcMat );
    
    quartzMat = new G4MaterialPropertiesTable();
    quartz->SetMaterialPropertiesTable( quartzMat );
    
    rockMat = new G4MaterialPropertiesTable();
    rockMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp, rockRefl,
                         NUM_PP_ConstProp );
    rock->SetMaterialPropertiesTable( rockMat );
    
    cavRockMat = new G4MaterialPropertiesTable();
    cavRockMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp, rockRefl,
                            NUM_PP_ConstProp );
    cavRock->SetMaterialPropertiesTable( cavRockMat );
   

    portlandConcreteMat = new G4MaterialPropertiesTable();
    portlandConcreteMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp, rockRefl,
                            NUM_PP_ConstProp );
    portlandConcrete->SetMaterialPropertiesTable( portlandConcreteMat );

 
    sapphireMat = new G4MaterialPropertiesTable();
    sapphire->SetMaterialPropertiesTable( sapphireMat );
    
    aluminaMat = new G4MaterialPropertiesTable();
    alumina->SetMaterialPropertiesTable( aluminaMat );
    
    elastomerMat = new G4MaterialPropertiesTable();
    elastomer->SetMaterialPropertiesTable( elastomerMat );
    
    nicomicMat = new G4MaterialPropertiesTable();
    nicomic->SetMaterialPropertiesTable( nicomicMat );
    
    inconelMat = new G4MaterialPropertiesTable();
    inconel->SetMaterialPropertiesTable( inconelMat );
    
    ss316Mat = new G4MaterialPropertiesTable();
    ss316->SetMaterialPropertiesTable( ss316Mat );
    
    steelMat = new G4MaterialPropertiesTable();
    steel->SetMaterialPropertiesTable( steelMat );
    
    //*** Teflon material definitions ***//
    teflonMat = new G4MaterialPropertiesTable();
    G4double *teflonSpeclobe;
    teflonSpeclobe = new G4double[NUM_PP_ConstProp];
    teflonSpeclobe[0] = teflonSpeclobe[1] = 0;
    G4double *teflonSpecspike;
    teflonSpecspike = new G4double[NUM_PP_ConstProp];
    teflonSpecspike[0] = teflonSpecspike[1] = 0;
    G4double *teflonBackscat;
    teflonBackscat = new G4double[NUM_PP_ConstProp];
    teflonBackscat[0] = teflonBackscat[1] = 0;
    G4double *teflonEff;
    teflonEff = new G4double[NUM_PP_ConstProp];
    teflonEff[0] = teflonEff[1] = 1;
    
    teflonMat->AddProperty( "SPECULARLOBECONSTANT", photonEnergies_ConstProp,
                           teflonSpeclobe, NUM_PP_ConstProp);
    teflonMat->AddProperty( "SPECULARSPIKECONSTANT", photonEnergies_ConstProp,
                           teflonSpecspike, NUM_PP_ConstProp );
    teflonMat->AddProperty( "BACKSCATTERCONSTANT", photonEnergies_ConstProp,
                           teflonBackscat, NUM_PP_ConstProp );
    teflonMat->AddProperty( "EFFICIENCY", photonEnergies_ConstProp, teflonEff,
                           NUM_PP_ConstProp );
    
    teflon->SetMaterialPropertiesTable( teflonMat );
    
    titaniumMat = new G4MaterialPropertiesTable();
    titanium->SetMaterialPropertiesTable( titaniumMat );
    
    //*** TPB material definitions ***//
    //	I can only imagine that the scattering and absorption length are going
    //	to be highly dependent on the method used to deposit the TPB (vacuum
    //	deposition vs. dipping vs. spray-on vs. whatever). So even though these
    //	values are SWAGs, they're actually double-suspect.
    tpbMat = new G4MaterialPropertiesTable();
    G4double tpbScattering[] = { 5.e-6*m, 5.e-6*mm };	//	This is a SWAG
    G4double tpbAbsorptionKill[] = { 1.*cm, 1.*cm };	//	Also a SWAG
    tpbMat->AddProperty( "RAYLEIGH", photonEnergies_ConstProp, tpbScattering,
                        NUM_PP_ConstProp );
    tpbMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                        quartzRefractiveIndex, NUM_PP_ConstProp );
    tpbMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                        tpbAbsorptionKill, NUM_PP_ConstProp );
    tpbMat->AddProperty( "WLSABSLENGTH", tpbAbsorptionEnergies, tpbAbsorption,
                        num_tpbAbs );
    tpbMat->AddProperty( "WLSCOMPONENT", tpbEmissionEnergies, tpbEmission,
                        num_tpbEmi );
    tpbMat->AddConstProperty( "WLSTIMECONSTANT", 2.*ns );
    tpb->SetMaterialPropertiesTable( tpbMat );
    
    //*** W material definitions ***//
    tungstenMat = new G4MaterialPropertiesTable();
    tungsten->SetMaterialPropertiesTable( tungstenMat );
    
    //*** Heavy Met defintions ***//
    heavyMetMat = new G4MaterialPropertiesTable();
    heavyMet->SetMaterialPropertiesTable( heavyMetMat );
    
    //*** Tyvek material definitions ***//
    tyvekMat = new G4MaterialPropertiesTable();
    G4double *tyvekSpeclobe;
    tyvekSpeclobe = new G4double[nWaterEntries];
    G4double *tyvekSpecspike;
    tyvekSpecspike = new G4double[nWaterEntries];
    G4double *tyvekBackscat;
    tyvekBackscat = new G4double[nWaterEntries];
    G4double *tyvekEff;
    tyvekEff = new G4double[nWaterEntries];
    
    for(G4int i=0; i<nWaterEntries; i++){
        tyvekSpeclobe[i] = 0.;
        tyvekSpecspike[i] = 0.;
        tyvekBackscat[i] = 0.;
        tyvekEff[i] = 1.;
    }
    
    tyvekMat->AddProperty( "SPECULARLOBECONSTANT", waterPhotonEnergies,
                          tyvekSpeclobe, nWaterEntries );
    tyvekMat->AddProperty( "SPECULARSPIKECONSTANT", waterPhotonEnergies,
                          tyvekSpecspike, nWaterEntries );
    tyvekMat->AddProperty( "BACKSCATTERCONSTANT", waterPhotonEnergies,
                          tyvekBackscat, nWaterEntries );
    tyvekMat->AddProperty( "EFFICIENCY", waterPhotonEnergies, tyvekEff,
                          nWaterEntries );
    
    tyvek->SetMaterialPropertiesTable( tyvekMat );
    
    //*** Tyvek600 material definitions ***//
    tyvek600Mat = new G4MaterialPropertiesTable();
    G4double *tyvek600Speclobe;
    tyvek600Speclobe = new G4double[nWaterEntries];
    G4double *tyvek600Specspike;
    tyvek600Specspike = new G4double[nWaterEntries];
    G4double *tyvek600Backscat;
    tyvek600Backscat = new G4double[nWaterEntries];
    G4double *tyvek600Eff;
    tyvek600Eff = new G4double[nWaterEntries];
    
    for(G4int i=0; i<nWaterEntries; i++){
        tyvek600Speclobe[i] = 0.;
        tyvek600Specspike[i] = 0.;
        tyvek600Backscat[i] = 0.;
        tyvek600Eff[i] = 1.;
    }
    
    tyvek600Mat->AddProperty( "SPECULARLOBECONSTANT", waterPhotonEnergies,
                             tyvek600Speclobe, nWaterEntries );
    tyvek600Mat->AddProperty( "SPECULARSPIKECONSTANT", waterPhotonEnergies,
                             tyvek600Specspike, nWaterEntries );
    tyvek600Mat->AddProperty( "BACKSCATTERCONSTANT", waterPhotonEnergies,
                             tyvek600Backscat, nWaterEntries );
    tyvek600Mat->AddProperty( "EFFICIENCY", waterPhotonEnergies, tyvek600Eff,
                             nWaterEntries );
    
    tyvek600->SetMaterialPropertiesTable( tyvek600Mat );
    
    //*** Vacuum material definitions ***//
    vacuumMat = new G4MaterialPropertiesTable();
    G4double *vacuumRindex;
    vacuumRindex = new G4double[NUM_PP_ConstProp];
    vacuumRindex[0] = vacuumRindex[1] = 1;
    G4double *vacuumAbsLength;
    vacuumAbsLength = new G4double[NUM_PP_ConstProp];
    vacuumAbsLength[0] = vacuumAbsLength[1] = 1e20;
    vacuumMat->AddProperty( "RINDEX", photonEnergies_ConstProp, vacuumRindex,
                           NUM_PP_ConstProp );
    vacuumMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                           vacuumAbsLength, NUM_PP_ConstProp );
    vacuum->SetMaterialPropertiesTable( vacuumMat );
    
    blackiumMat = vacuumMat;
    blackium->SetMaterialPropertiesTable( blackiumMat );
    
    //*** Water material definitions ***//
    waterMat = new G4MaterialPropertiesTable();
    water->SetMaterialPropertiesTable( waterMat );
    // overwrite computed meanExcitationEnergy with ICRU recommended value
    water->GetIonisation()->SetMeanExcitationEnergy(75.0*eV);
    
    dopedWater3->SetMaterialPropertiesTable( waterMat );
    dopedWater7->SetMaterialPropertiesTable( waterMat );
    dopedWater3->GetIonisation()->SetMeanExcitationEnergy(75.0*eV);
    dopedWater7->GetIonisation()->SetMeanExcitationEnergy(75.0*eV);
    
    //*** LZ specular reflector material definitions ***//
    lzSpecRefMat = new G4MaterialPropertiesTable();
    
    G4double *lzSpecRefSpeclobe;
    lzSpecRefSpeclobe = new G4double[nWaterEntries];
    G4double *lzSpecRefSpecspike;
    lzSpecRefSpecspike = new G4double[nWaterEntries];
    G4double *lzSpecRefBackscat;
    lzSpecRefBackscat = new G4double[nWaterEntries];
    G4double *lzSpecRefEff;
    lzSpecRefEff = new G4double[nWaterEntries];
    
    for(G4int i=0; i<nWaterEntries; i++){
        lzSpecRefSpeclobe[i] = 0.;
        lzSpecRefSpecspike[i] = 1.;
        lzSpecRefBackscat[i] = 0.;
        lzSpecRefEff[i] = 1.;
    }
    
    lzSpecRefMat->AddProperty( "SPECULARLOBECONSTANT", waterPhotonEnergies,
                              lzSpecRefSpeclobe, nWaterEntries );
    lzSpecRefMat->AddProperty( "SPECULARSPIKECONSTANT", waterPhotonEnergies,
                              lzSpecRefSpecspike, nWaterEntries );
    lzSpecRefMat->AddProperty( "BACKSCATTERCONSTANT", waterPhotonEnergies,
                              lzSpecRefBackscat, nWaterEntries );
    lzSpecRefMat->AddProperty( "EFFICIENCY", waterPhotonEnergies, lzSpecRefEff,
                              nWaterEntries );
    
    lzSpecRefMat->AddProperty( "REFLECTIVITY", waterPhotonEnergies,
                              lzSpecRefRefl, nWaterEntries );
    lzSpecRef->SetMaterialPropertiesTable( lzSpecRefMat );
    
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					CreateOpticalSurfaces()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::CreateOpticalSurfaces()
{
    /* ********* Create Al - quartz boundary ********** */
    aluminumQuartzSurface = new G4OpticalSurface("aluminumQuartzSurface",
                                                 unified, polished, dielectric_metal);
    // Create new materials table
    G4MaterialPropertiesTable * aluminumQuartzSurfaceMat =
    new G4MaterialPropertiesTable();
    aluminumQuartzSurfaceMat->AddProperty( "REFLECTIVITY",
                                          photonEnergies_ConstProp, aluminumUnoxidizedRefl, 2 );
    // Set surface material table
    aluminumQuartzSurface->SetMaterialPropertiesTable(aluminumQuartzSurfaceMat);
    /* ************************************************ */
    
    /* ********* Create gas Xe - copper boundary ********* */
    gasXeCopperSurface = new G4OpticalSurface("gasXeCopperSurface", unified,
                                              polished, dielectric_metal);
    // Create new materials table
    G4MaterialPropertiesTable * gasXeCopperSurfaceMat =
    new G4MaterialPropertiesTable();
    gasXeCopperSurfaceMat->AddProperty( "REFLECTIVITY",
                                       photonEnergies_ConstProp, copperRefl, 2 );
    // Set surface material table
    gasXeCopperSurface->SetMaterialPropertiesTable(gasXeCopperSurfaceMat);
    /* *************************************************** */
    
    /* ********* Create liquid Xe - copper boundary ********* */
    liquidXeCopperSurface = new G4OpticalSurface("liquidXeCopperSurface",
                                                 unified, polished, dielectric_metal);
    // Create new materials table
    G4MaterialPropertiesTable * liquidXeCopperSurfaceMat =
    new G4MaterialPropertiesTable();
    liquidXeCopperSurfaceMat->AddProperty( "REFLECTIVITY",
                                          photonEnergies_ConstProp, copperRefl, 2 );
    // Set surface material table
    liquidXeCopperSurface->SetMaterialPropertiesTable(liquidXeCopperSurfaceMat);
    /* *************************************************** */
    
    /* ********* Create gas Xe - HDPE boundary ********* */
    gasXeHDPESurface = new G4OpticalSurface("gasXeHDPESurface", unified,
                                            polished, dielectric_metal);
    // Create new materials table
    G4MaterialPropertiesTable * gasXeHDPESurfaceMat =
    new G4MaterialPropertiesTable();
    gasXeHDPESurfaceMat->AddProperty( "SPECULARLOBECONSTANT",
                                     polyethyleneMat->GetProperty("SPECULARLOBECONSTANT") );
    gasXeHDPESurfaceMat->AddProperty( "SPECULARSPIKECONSTANT",
                                     polyethyleneMat->GetProperty("SPECULARSPIKECONSTANT") );
    gasXeHDPESurfaceMat->AddProperty( "BACKSCATTERCONSTANT",
                                     polyethyleneMat->GetProperty("BACKSCATTERCONSTANT") );
    gasXeHDPESurfaceMat->AddProperty( "EFFICIENCY",
                                     polyethyleneMat->GetProperty("EFFICIENCY") );
    gasXeHDPESurfaceMat->AddProperty( "REFLECTIVITY",
                                     photonEnergies_ConstProp, polyethyleneRefl, 2 );
    // Set surface material table
    gasXeHDPESurface->SetMaterialPropertiesTable(gasXeHDPESurfaceMat);
    /* *************************************************** */
    
    /* ********* Create liquid Xe - HDPE boundary ********* */
    liquidXeHDPESurface = new G4OpticalSurface("liquidXeHDPESurface", unified,
                                               polished, dielectric_metal);
    // Create new materials table
    G4MaterialPropertiesTable * liquidXeHDPESurfaceMat =
    new G4MaterialPropertiesTable();
    liquidXeHDPESurfaceMat->AddProperty( "SPECULARLOBECONSTANT",
                                        polyethyleneMat->GetProperty("SPECULARLOBECONSTANT") );
    liquidXeHDPESurfaceMat->AddProperty( "SPECULARSPIKECONSTANT",
                                        polyethyleneMat->GetProperty("SPECULARSPIKECONSTANT") );
    liquidXeHDPESurfaceMat->AddProperty( "BACKSCATTERCONSTANT",
                                        polyethyleneMat->GetProperty("BACKSCATTERCONSTANT") );
    liquidXeHDPESurfaceMat->AddProperty( "EFFICIENCY",
                                        polyethyleneMat->GetProperty("EFFICIENCY") );
    liquidXeHDPESurfaceMat->AddProperty( "REFLECTIVITY",
                                        photonEnergies_ConstProp, polyethyleneRefl, 2 );
    // Set surface material table
    liquidXeHDPESurface->SetMaterialPropertiesTable(liquidXeHDPESurfaceMat);
    /* *************************************************** */
    
    /* ********* Create gas Xe - Teflon boundary ********* */
    gasXeTeflonSurface = new G4OpticalSurface("gasXeTeflonSurface", unified,
					      groundfrontpainted, dielectric_dielectric); //REAL

    // Create new materials table
    G4MaterialPropertiesTable * gasXeTeflonSurfaceMat =
    new G4MaterialPropertiesTable();
    gasXeTeflonSurfaceMat->AddProperty( "SPECULARLOBECONSTANT",
                                       teflonMat->GetProperty("SPECULARLOBECONSTANT"));
    gasXeTeflonSurfaceMat->AddProperty( "SPECULARSPIKECONSTANT",
                                       teflonMat->GetProperty("SPECULARSPIKECONSTANT"));
    gasXeTeflonSurfaceMat->AddProperty( "BACKSCATTERCONSTANT",
                                       teflonMat->GetProperty("BACKSCATTERCONSTANT"));
    gasXeTeflonSurfaceMat->AddProperty( "EFFICIENCY",
                                       teflonMat->GetProperty("EFFICIENCY"));

    G4double gasXeTeflonRefl[23] = { 0.00, 0.03, 0.65, 0.65, 0.65,
        0.67, 0.71, 0.73, 0.74, 0.99,
        0.99, 0.99, 0.99, 0.99, 0.99,
        0.99, 0.99, 0.99, 0.99, 0.99,
        0.99, 0.99, 1.00 };
    //Puja Kadkhoda, Detlev Ristau, Ferdinand von Alvensleben.
    //Total scatter measurements in the DUV/VUV. volume 3578,
    //pages 544–554. SPIE, 1999. Figure 7 was rule of thumb, but
    //Figures 1.12 and 1.13 of Silva's Ph.D. Thesis also (2009)
    if ( TEMPERATURE < 200*kelvin ) {
        for ( G4int i=0; i < 23; i++ )
            gasXeTeflonRefl[i] = 0.85;
    }

    gasXeTeflonSurfaceMat->AddProperty( "REFLECTIVITY",
					photonEnergies, gasXeTeflonRefl, 23 );

    // Set surface material table
    gasXeTeflonSurface->SetMaterialPropertiesTable(gasXeTeflonSurfaceMat);
    /* *************************************************** */
    
    
    /* ********* Create gas Xe - "Black" Teflon boundary ********* */
    /* bvv: Reusing all properties of gas Xe - Teflon boundary except reflectivity.
     The latter is made low and constant.
     */
    gasXeBlackSurface = new G4OpticalSurface("gasXeBlackSurface", unified,
                                             groundfrontpainted, dielectric_dielectric);
    // Create new materials table
    G4MaterialPropertiesTable * gasXeBlackSurfaceMat =
    new G4MaterialPropertiesTable();
    gasXeBlackSurfaceMat->AddProperty( "SPECULARLOBECONSTANT",
                                      teflonMat->GetProperty("SPECULARLOBECONSTANT"));
    gasXeBlackSurfaceMat->AddProperty( "SPECULARSPIKECONSTANT",
                                      teflonMat->GetProperty("SPECULARSPIKECONSTANT"));
    gasXeBlackSurfaceMat->AddProperty( "BACKSCATTERCONSTANT",
                                      teflonMat->GetProperty("BACKSCATTERCONSTANT"));
    gasXeBlackSurfaceMat->AddProperty( "EFFICIENCY",
                                      teflonMat->GetProperty("EFFICIENCY"));
    G4double *gasXeBlackWallRefl = new G4double[num_pp_constProp]; // The dimension size is expected to be "2" but lets be prepared for a more general case.
    //    for (int i = 0; i < num_pp_constProp; i++) gasXeBlackWallRefl[i] = 0.2; // "0.2" is "black" enough.
    for (int i = 0; i < num_pp_constProp; i++) gasXeBlackWallRefl[i] = 0.2; 
    gasXeBlackSurfaceMat->AddProperty( "REFLECTIVITY",
                                      photonEnergies_ConstProp, gasXeBlackWallRefl, num_pp_constProp );
    // Set surface material table
    gasXeBlackSurface->SetMaterialPropertiesTable(gasXeBlackSurfaceMat);
    /* *************************************************** */
    
    /* ********* Create liquid Xe - Teflon boundary ********* */
    liquidXeTeflonSurface = new G4OpticalSurface("liquidXeTeflonSurface",
                                                 unified, groundfrontpainted, dielectric_dielectric);
    // Create new materials table
    G4MaterialPropertiesTable * liquidXeTeflonSurfaceMat =
    new G4MaterialPropertiesTable();
    liquidXeTeflonSurfaceMat->AddProperty( "SPECULARLOBECONSTANT",
                                          teflonMat->GetProperty("SPECULARLOBECONSTANT"));
    liquidXeTeflonSurfaceMat->AddProperty( "SPECULARSPIKECONSTANT",
                                          teflonMat->GetProperty("SPECULARSPIKECONSTANT"));
    liquidXeTeflonSurfaceMat->AddProperty( "BACKSCATTERCONSTANT",
                                          teflonMat->GetProperty("BACKSCATTERCONSTANT"));
    liquidXeTeflonSurfaceMat->AddProperty( "EFFICIENCY",
                                          teflonMat->GetProperty("EFFICIENCY"));
    G4double liquidXeTeflonRefl[2] = { 0.95, 0.95 }; //true value may lie, this
    //might be problematic
    //between >~0.95 and 1.15 (EXO). Look at Vladimir's Sept. LZ meeting talk.
    liquidXeTeflonSurfaceMat->AddProperty( "REFLECTIVITY",
                                          photonEnergies_ConstProp, liquidXeTeflonRefl, 2 );
    // Set surface material table
    liquidXeTeflonSurface->SetMaterialPropertiesTable(liquidXeTeflonSurfaceMat);
    
    /* *************************************************** */
    
    
    /* ********* Create liquid Xe - "Black" Teflon boundary ********* */
    /* bvv: Reusing all properties of liquid Xe - Teflon boundary except reflectivity.
     The latter is made low and constant.
     */
    liquidXeBlackSurface = new G4OpticalSurface("liquidXeBlackSurface",
                                                unified, groundfrontpainted, dielectric_dielectric);
    // Create new materials table
    G4MaterialPropertiesTable * liquidXeBlackSurfaceMat =
    new G4MaterialPropertiesTable();
    liquidXeBlackSurfaceMat->AddProperty( "SPECULARLOBECONSTANT",
                                         teflonMat->GetProperty("SPECULARLOBECONSTANT"));
    liquidXeBlackSurfaceMat->AddProperty( "SPECULARSPIKECONSTANT",
                                         teflonMat->GetProperty("SPECULARSPIKECONSTANT"));
    liquidXeBlackSurfaceMat->AddProperty( "BACKSCATTERCONSTANT",
                                         teflonMat->GetProperty("BACKSCATTERCONSTANT"));
    liquidXeBlackSurfaceMat->AddProperty( "EFFICIENCY",
                                         teflonMat->GetProperty("EFFICIENCY"));
    
    G4double *liquidXeBlackWallRefl = new G4double[num_pp_constProp]; // The dimension size is expected to be "2" but lets be prepared for a more general case.
    for (int i = 0; i < num_pp_constProp; i++) liquidXeBlackWallRefl[i] = 0.2; // "0.2" is "black" enough.
        liquidXeBlackSurfaceMat->AddProperty( "REFLECTIVITY",
                                         photonEnergies_ConstProp, liquidXeBlackWallRefl, 2 );
    // Set surface material table
    liquidXeBlackSurface->SetMaterialPropertiesTable(liquidXeBlackSurfaceMat);
    
    /* *************************************************** */
    
    /* ********* Create secondary liquid Xe - Teflon boundary, used for liner ********* */
    liquidXeTeflonSurfaceLiner = new G4OpticalSurface("liquidXeTeflonSurfaceLiner",
                                                      unified, groundfrontpainted, dielectric_dielectric);
    // Create new materials table
    G4MaterialPropertiesTable * liquidXeTeflonSurfaceMatSec =
    new G4MaterialPropertiesTable();
    liquidXeTeflonSurfaceMatSec->AddProperty( "SPECULARLOBECONSTANT",
                                             teflonMat->GetProperty("SPECULARLOBECONSTANT"));
    liquidXeTeflonSurfaceMatSec->AddProperty( "SPECULARSPIKECONSTANT",
                                             teflonMat->GetProperty("SPECULARSPIKECONSTANT"));
    liquidXeTeflonSurfaceMatSec->AddProperty( "BACKSCATTERCONSTANT",
                                             teflonMat->GetProperty("BACKSCATTERCONSTANT"));
    liquidXeTeflonSurfaceMatSec->AddProperty( "EFFICIENCY",
                                             teflonMat->GetProperty("EFFICIENCY"));
    G4double liquidXeTeflonReflLiner[2] = { 0.93, 0.93 }; //true value may lie, this
    //might be problematic
    //between >~0.95 and 1.15 (EXO). Look at Vladimir's Sept. LZ meeting talk.
    liquidXeTeflonSurfaceMatSec->AddProperty( "REFLECTIVITY",
                                             photonEnergies_ConstProp, liquidXeTeflonReflLiner, 2 );
    // Set surface material table
    liquidXeTeflonSurfaceLiner->SetMaterialPropertiesTable(liquidXeTeflonSurfaceMatSec);
    /* *************************************************** */
    
    /* ********* Create secondary liquid Xe - Teflon boundary, used for other stuff in skin ********* */
    // liquidXeTeflonSurfaceSkin = new G4OpticalSurface("liquidXeTeflonSurfaceSkin",
    //                                                   unified, groundfrontpainted, dielectric_dielectric);
    // // Create new materials table
    // G4MaterialPropertiesTable * liquidXeTeflonSurfaceMatSkin =
    // new G4MaterialPropertiesTable();
    // liquidXeTeflonSurfaceMatSkin->AddProperty( "SPECULARLOBECONSTANT",
    //                                          teflonMat->GetProperty("SPECULARLOBECONSTANT"));
    // liquidXeTeflonSurfaceMatSkin->AddProperty( "SPECULARSPIKECONSTANT",
    //                                          teflonMat->GetProperty("SPECULARSPIKECONSTANT"));
    // liquidXeTeflonSurfaceMatSkin->AddProperty( "BACKSCATTERCONSTANT",
    //                                          teflonMat->GetProperty("BACKSCATTERCONSTANT"));
    // liquidXeTeflonSurfaceMatSkin->AddProperty( "EFFICIENCY",
    //                                          teflonMat->GetProperty("EFFICIENCY"));
    // G4double liquidXeTeflonReflSkin[2] = { 0.90, 0.90 }; //true value may lie, this
    // //might be problematic
    // //between >~0.95 and 1.15 (EXO). Look at Vladimir's Sept. LZ meeting talk.
    // liquidXeTeflonSurfaceMatSkin->AddProperty( "REFLECTIVITY",
    //                                          photonEnergies_ConstProp, liquidXeTeflonReflSkin, 2 );
    // // Set surface material table
    // liquidXeTeflonSurfaceSkin->SetMaterialPropertiesTable(liquidXeTeflonSurfaceMatSkin);
    /* *************************************************** */
    
    /* ********* Create secondary liquid Xe - Teflon boundary, used for other stuff in skin ********* */
    liquidXeTeflonSurfacePMT = new G4OpticalSurface("liquidXeTeflonSurfacePMT",
                                                    unified, groundfrontpainted, dielectric_dielectric);
    // Create new materials table
    G4MaterialPropertiesTable * liquidXeTeflonSurfaceMatPMT =
    new G4MaterialPropertiesTable();
    liquidXeTeflonSurfaceMatPMT->AddProperty( "SPECULARLOBECONSTANT",
                                             teflonMat->GetProperty("SPECULARLOBECONSTANT"));
    liquidXeTeflonSurfaceMatPMT->AddProperty( "SPECULARSPIKECONSTANT",
                                             teflonMat->GetProperty("SPECULARSPIKECONSTANT"));
    liquidXeTeflonSurfaceMatPMT->AddProperty( "BACKSCATTERCONSTANT",
                                             teflonMat->GetProperty("BACKSCATTERCONSTANT"));
    liquidXeTeflonSurfaceMatPMT->AddProperty( "EFFICIENCY",
                                             teflonMat->GetProperty("EFFICIENCY"));
    G4double liquidXeTeflonReflPMT[2] = { 0.90, 0.90 }; //true value may lie, this
    //might be problematic
    //between >~0.95 and 1.15 (EXO). Look at Vladimir's Sept. LZ meeting talk.
    liquidXeTeflonSurfaceMatPMT->AddProperty( "REFLECTIVITY",
                                             photonEnergies_ConstProp, liquidXeTeflonReflPMT, 2 );
    // Set surface material table
    liquidXeTeflonSurfacePMT->SetMaterialPropertiesTable(liquidXeTeflonSurfaceMatPMT);
    /* *************************************************** */
    
    /* ********* Create secondary liquid Xe - Teflon boundary, used for other stuff in skin ********* */
    liquidXeTeflonSurfacePlate = new G4OpticalSurface("liquidXeTeflonSurfacePlate",
                                                      unified, groundfrontpainted, dielectric_dielectric);
    // Create new materials table
    G4MaterialPropertiesTable * liquidXeTeflonSurfaceMatPlate =
    new G4MaterialPropertiesTable();
    liquidXeTeflonSurfaceMatPlate->AddProperty( "SPECULARLOBECONSTANT",
                                               teflonMat->GetProperty("SPECULARLOBECONSTANT"));
    liquidXeTeflonSurfaceMatPlate->AddProperty( "SPECULARSPIKECONSTANT",
                                               teflonMat->GetProperty("SPECULARSPIKECONSTANT"));
    liquidXeTeflonSurfaceMatPlate->AddProperty( "BACKSCATTERCONSTANT",
                                               teflonMat->GetProperty("BACKSCATTERCONSTANT"));
    liquidXeTeflonSurfaceMatPlate->AddProperty( "EFFICIENCY",
                                               teflonMat->GetProperty("EFFICIENCY"));
    G4double liquidXeTeflonReflPlate[2] = { 0.90, 0.90 }; //true value may lie, this
    //might be problematic
    //between >~0.95 and 1.15 (EXO). Look at Vladimir's Sept. LZ meeting talk.
    liquidXeTeflonSurfaceMatPlate->AddProperty( "REFLECTIVITY",
                                               photonEnergies_ConstProp, liquidXeTeflonReflPlate, 2 );
    // Set surface material table
    liquidXeTeflonSurfacePlate->SetMaterialPropertiesTable(liquidXeTeflonSurfaceMatPlate);
    /* *************************************************** */
    
    /* ********* Create secondary liquid Xe - Teflon boundary, used for other stuff in skin ********* */
    liquidXeTeflonSurfaceCables = new G4OpticalSurface("liquidXeTeflonSurfaceCables",
                                                       unified, groundfrontpainted, dielectric_dielectric);
    // Create new materials table
    G4MaterialPropertiesTable * liquidXeTeflonSurfaceMatCables =
    new G4MaterialPropertiesTable();
    liquidXeTeflonSurfaceMatCables->AddProperty( "SPECULARLOBECONSTANT",
                                                teflonMat->GetProperty("SPECULARLOBECONSTANT"));
    liquidXeTeflonSurfaceMatCables->AddProperty( "SPECULARSPIKECONSTANT",
                                                teflonMat->GetProperty("SPECULARSPIKECONSTANT"));
    liquidXeTeflonSurfaceMatCables->AddProperty( "BACKSCATTERCONSTANT",
                                                teflonMat->GetProperty("BACKSCATTERCONSTANT"));
    liquidXeTeflonSurfaceMatCables->AddProperty( "EFFICIENCY",
                                                teflonMat->GetProperty("EFFICIENCY"));
    G4double liquidXeTeflonReflCables[2] = { 0.10, 0.10 }; //true value may lie, this
    //might be problematic
    //between >~0.95 and 1.15 (EXO). Look at Vladimir's Sept. LZ meeting talk.
    liquidXeTeflonSurfaceMatCables->AddProperty( "REFLECTIVITY",
                                                photonEnergies_ConstProp, liquidXeTeflonReflCables, 2 );
    // Set surface material table
    liquidXeTeflonSurfaceCables->SetMaterialPropertiesTable(liquidXeTeflonSurfaceMatCables);
    /* *************************************************** */
    
    /* ********* Create secondary liquid Xe - Teflon boundary, used for other stuff in skin ********* */
    liquidXeTeflonSurfaceTruss = new G4OpticalSurface("liquidXeTeflonSurfaceTruss",
                                                      unified, groundfrontpainted, dielectric_dielectric);
    // Create new materials table
    G4MaterialPropertiesTable * liquidXeTeflonSurfaceMatTruss =
    new G4MaterialPropertiesTable();
    liquidXeTeflonSurfaceMatTruss->AddProperty( "SPECULARLOBECONSTANT",
                                               teflonMat->GetProperty("SPECULARLOBECONSTANT"));
    liquidXeTeflonSurfaceMatTruss->AddProperty( "SPECULARSPIKECONSTANT",
                                               teflonMat->GetProperty("SPECULARSPIKECONSTANT"));
    liquidXeTeflonSurfaceMatTruss->AddProperty( "BACKSCATTERCONSTANT",
                                               teflonMat->GetProperty("BACKSCATTERCONSTANT"));
    liquidXeTeflonSurfaceMatTruss->AddProperty( "EFFICIENCY",
                                               teflonMat->GetProperty("EFFICIENCY"));
    G4double liquidXeTeflonReflTruss[2] = { 0.85, 0.85 }; //true value may lie, this
    //might be problematic
    //between >~0.95 and 1.15 (EXO). Look at Vladimir's Sept. LZ meeting talk.
    liquidXeTeflonSurfaceMatCables->AddProperty( "REFLECTIVITY",
                                                photonEnergies_ConstProp, liquidXeTeflonReflTruss, 2 );
    // Set surface material table
    liquidXeTeflonSurfaceTruss->SetMaterialPropertiesTable(liquidXeTeflonSurfaceMatCables);
    /* *************************************************** */
    
    /* ********* Create secondary liquid Xe - Teflon boundary, used for other stuff in skin ********* */
    liquidXeTeflonSurfaceBskinPMT = new G4OpticalSurface("liquidXeTeflonSurfaceBskinPMT",
                                                         unified, groundfrontpainted, dielectric_dielectric);
    // Create new materials table
    G4MaterialPropertiesTable * liquidXeTeflonSurfaceMatBskinPMT =
    new G4MaterialPropertiesTable();
    liquidXeTeflonSurfaceMatBskinPMT->AddProperty( "SPECULARLOBECONSTANT",
                                                  teflonMat->GetProperty("SPECULARLOBECONSTANT"));
    liquidXeTeflonSurfaceMatBskinPMT->AddProperty( "SPECULARSPIKECONSTANT",
                                                  teflonMat->GetProperty("SPECULARSPIKECONSTANT"));
    liquidXeTeflonSurfaceMatBskinPMT->AddProperty( "BACKSCATTERCONSTANT",
                                                  teflonMat->GetProperty("BACKSCATTERCONSTANT"));
    liquidXeTeflonSurfaceMatBskinPMT->AddProperty( "EFFICIENCY",
                                                  teflonMat->GetProperty("EFFICIENCY"));
    G4double liquidXeTeflonReflBskinPMT[2] = { 0.90, 0.90 }; //true value may lie, this
    //might be problematic
    //between >~0.95 and 1.15 (EXO). Look at Vladimir's Sept. LZ meeting talk.
    liquidXeTeflonSurfaceMatCables->AddProperty( "REFLECTIVITY",
                                                photonEnergies_ConstProp, liquidXeTeflonReflBskinPMT, 2 );
    // Set surface material table
    liquidXeTeflonSurfaceBskinPMT->SetMaterialPropertiesTable(liquidXeTeflonSurfaceMatBskinPMT);
    /* *************************************************** */
    
    liquidXeTeflonSurfaceBplate = new G4OpticalSurface("liquidXeTeflonSurfaceBplate",
                                                       unified, groundfrontpainted, dielectric_dielectric);
    // Create new materials table
    G4MaterialPropertiesTable * liquidXeTeflonSurfaceMatBplate =
    new G4MaterialPropertiesTable();
    liquidXeTeflonSurfaceMatBplate->AddProperty( "SPECULARLOBECONSTANT",
                                                teflonMat->GetProperty("SPECULARLOBECONSTANT"));
    liquidXeTeflonSurfaceMatBplate->AddProperty( "SPECULARSPIKECONSTANT",
                                                teflonMat->GetProperty("SPECULARSPIKECONSTANT"));
    liquidXeTeflonSurfaceMatBplate->AddProperty( "BACKSCATTERCONSTANT",
                                                teflonMat->GetProperty("BACKSCATTERCONSTANT"));
    liquidXeTeflonSurfaceMatBplate->AddProperty( "EFFICIENCY",
                                                teflonMat->GetProperty("EFFICIENCY"));
    G4double liquidXeTeflonReflBplate[2] = { 0.90, 0.90 }; //true value may lie, this
    //might be problematic
    //between >~0.95 and 1.15 (EXO). Look at Vladimir's Sept. LZ meeting talk.
    liquidXeTeflonSurfaceMatBplate->AddProperty( "REFLECTIVITY",
                                                photonEnergies_ConstProp, liquidXeTeflonReflBplate, 2 );
    // Set surface material table
    liquidXeTeflonSurfaceBplate->SetMaterialPropertiesTable(liquidXeTeflonSurfaceMatBplate);
    /* *************************************************** */
    
    /* *************************************************** */
    
    liquidXeTeflonSurfaceTskinPMT = new G4OpticalSurface("liquidXeTeflonSurfaceTskinPMT",
                                                         unified, groundfrontpainted, dielectric_dielectric);
    // Create new materials table
    G4MaterialPropertiesTable * liquidXeTeflonSurfaceMatTskinPMT =
    new G4MaterialPropertiesTable();
    liquidXeTeflonSurfaceMatTskinPMT->AddProperty( "SPECULARLOBECONSTANT",
                                                  teflonMat->GetProperty("SPECULARLOBECONSTANT"));
    liquidXeTeflonSurfaceMatTskinPMT->AddProperty( "SPECULARSPIKECONSTANT",
                                                  teflonMat->GetProperty("SPECULARSPIKECONSTANT"));
    liquidXeTeflonSurfaceMatTskinPMT->AddProperty( "BACKSCATTERCONSTANT",
                                                  teflonMat->GetProperty("BACKSCATTERCONSTANT"));
    liquidXeTeflonSurfaceMatTskinPMT->AddProperty( "EFFICIENCY",
                                                  teflonMat->GetProperty("EFFICIENCY"));
    G4double liquidXeTeflonReflTskinPMT[2] = { 0.90, 0.90 }; //true value may lie, this
    //might be problematic
    //between >~0.95 and 1.15 (EXO). Look at Vladimir's Sept. LZ meeting talk.
    liquidXeTeflonSurfaceMatTskinPMT->AddProperty( "REFLECTIVITY",
                                                  photonEnergies_ConstProp, liquidXeTeflonReflTskinPMT, 2 );
    // Set surface material table
    liquidXeTeflonSurfaceTskinPMT->SetMaterialPropertiesTable(liquidXeTeflonSurfaceMatTskinPMT);
    /* *************************************************** */
    
    /* *************************************************** */
    
    liquidXeTeflonSurfaceWeir = new G4OpticalSurface("liquidXeTeflonSurfaceWeir",
                                                     unified, groundfrontpainted, dielectric_dielectric);
    // Create new materials table
    G4MaterialPropertiesTable * liquidXeTeflonSurfaceMatTskinWeir =
    new G4MaterialPropertiesTable();
    liquidXeTeflonSurfaceMatTskinWeir->AddProperty( "SPECULARLOBECONSTANT",
                                                   teflonMat->GetProperty("SPECULARLOBECONSTANT"));
    liquidXeTeflonSurfaceMatTskinWeir->AddProperty( "SPECULARSPIKECONSTANT",
                                                   teflonMat->GetProperty("SPECULARSPIKECONSTANT"));
    liquidXeTeflonSurfaceMatTskinWeir->AddProperty( "BACKSCATTERCONSTANT",
                                                   teflonMat->GetProperty("BACKSCATTERCONSTANT"));
    liquidXeTeflonSurfaceMatTskinWeir->AddProperty( "EFFICIENCY",
                                                   teflonMat->GetProperty("EFFICIENCY"));
    G4double liquidXeTeflonReflWeir[2] = { 0.10, 0.10 }; //true value may lie, this
    //might be problematic
    //between >~0.95 and 1.15 (EXO). Look at Vladimir's Sept. LZ meeting talk.
    liquidXeTeflonSurfaceMatTskinWeir->AddProperty( "REFLECTIVITY",
                                                   photonEnergies_ConstProp, liquidXeTeflonReflWeir, 2 );
    // Set surface material table
    liquidXeTeflonSurfaceWeir->SetMaterialPropertiesTable(liquidXeTeflonSurfaceMatTskinWeir);
    /* *************************************************** */
    
    /* *************************************************** */
    
    liquidXeTeflonSurfaceDomePMT = new G4OpticalSurface("liquidXeTeflonSurfaceDomePMT",
                                                        unified, groundfrontpainted, dielectric_dielectric);
    // Create new materials table
    G4MaterialPropertiesTable * liquidXeTeflonSurfaceMatDomePMT =
    new G4MaterialPropertiesTable();
    liquidXeTeflonSurfaceMatDomePMT->AddProperty( "SPECULARLOBECONSTANT",
                                                 teflonMat->GetProperty("SPECULARLOBECONSTANT"));
    liquidXeTeflonSurfaceMatDomePMT->AddProperty( "SPECULARSPIKECONSTANT",
                                                 teflonMat->GetProperty("SPECULARSPIKECONSTANT"));
    liquidXeTeflonSurfaceMatDomePMT->AddProperty( "BACKSCATTERCONSTANT",
                                                 teflonMat->GetProperty("BACKSCATTERCONSTANT"));
    liquidXeTeflonSurfaceMatDomePMT->AddProperty( "EFFICIENCY",
                                                 teflonMat->GetProperty("EFFICIENCY"));
    G4double liquidXeTeflonReflDomePMT[2] = { 0.90, 0.90 }; //true value may lie, this
    //might be problematic
    //between >~0.95 and 1.15 (EXO). Look at Vladimir's Sept. LZ meeting talk.
    liquidXeTeflonSurfaceMatDomePMT->AddProperty( "REFLECTIVITY",
                                                 photonEnergies_ConstProp, liquidXeTeflonReflDomePMT, 2 );
    // Set surface material table
    liquidXeTeflonSurfaceDomePMT->SetMaterialPropertiesTable(liquidXeTeflonSurfaceMatDomePMT);
    
    /* ********* Create gas Xe - Steel boundary ********* */
    gasXeSteelSurface = new G4OpticalSurface("gasXeSteelSurface", unified,
                                             polished, dielectric_metal);
    // Create new materials table
    G4MaterialPropertiesTable * gasXeSteelSurfaceMat =
    new G4MaterialPropertiesTable();
    G4double gasXeSteelRefl[2] = { 0.2, 0.2 };
    gasXeSteelSurfaceMat->AddProperty( "REFLECTIVITY",
                                      photonEnergies_ConstProp, gasXeSteelRefl, 2);
    // Set surface material table
    gasXeSteelSurface->SetMaterialPropertiesTable(gasXeSteelSurfaceMat);
    /* *************************************************** */
    
    /* ********* Create liquid Xe - Steel boundary ********* */
    liquidXeSteelSurface = new G4OpticalSurface("liquidXeSteelSurface",
                                                unified, polished, dielectric_metal);
    // Create new materials table
    G4MaterialPropertiesTable * liquidXeSteelSurfaceMat =
    new G4MaterialPropertiesTable();
    G4double liquidXeSteelRefl[2] = { 0.05, 0.05 };
    liquidXeSteelSurfaceMat->AddProperty( "REFLECTIVITY",
                                         photonEnergies_ConstProp, liquidXeSteelRefl, 2);
    // Set surface material table
    liquidXeSteelSurface->SetMaterialPropertiesTable(
                                                     liquidXeSteelSurfaceMat );
    
    /* *************************************************** */
    
    /* ********* Create gas Xe - Titanium boundary ********* */
    gasXeTitaniumSurface = new G4OpticalSurface("gasXeTitaniumSurface", unified,
                                                polished, dielectric_metal);
    // Create new materials table
    G4MaterialPropertiesTable * gasXeTitaniumSurfaceMat =
    new G4MaterialPropertiesTable();
    gasXeTitaniumSurfaceMat->AddProperty( "REFLECTIVITY",
                                         photonEnergies_ConstProp, titaniumRefl, 2);
    // Set surface material table
    gasXeTitaniumSurface->SetMaterialPropertiesTable(gasXeTitaniumSurfaceMat);
    /* *************************************************** */
    
    /* ********* Create liquid Xe - Titanium boundary ********* */
    liquidXeTitaniumSurface = new G4OpticalSurface("liquidXeTitaniumSurface",
                                                   unified, polished, dielectric_metal);
    // Create new materials table
    G4MaterialPropertiesTable * liquidXeTitaniumSurfaceMat =
    new G4MaterialPropertiesTable();
    liquidXeTitaniumSurfaceMat->AddProperty( "REFLECTIVITY",
                                            photonEnergies_ConstProp, titaniumRefl, 2);
    // Set surface material table
    liquidXeTitaniumSurface->SetMaterialPropertiesTable(
                                                        liquidXeTitaniumSurfaceMat );
    /* *************************************************** */
    
    /* ********* Create gas Ar - Gold boundary ********* */
    gasArGoldSurface = new G4OpticalSurface("gasArGoldSurface", unified,
                                            polished, dielectric_metal);
    // Create new materials table
    G4MaterialPropertiesTable *gasArGoldSurfaceMat =
    new G4MaterialPropertiesTable();
    gasArGoldSurfaceMat->AddProperty( "REFLECTIVITY",
                                     photonEnergies_ConstProp, goldRefl, 2);
    // Set surface material table
    gasArGoldSurface->SetMaterialPropertiesTable(gasArGoldSurfaceMat);
    /* *************************************************** */
    
    /* ********* Create gas Ar - Aluminum boundary ********* */
    gasArAluminumSurface = new G4OpticalSurface("gasArAluminumSurface",
                                                unified, polished, dielectric_metal);
    // Create new materials table
    G4MaterialPropertiesTable *gasArAluminumSurfaceMat =
    new G4MaterialPropertiesTable();
    gasArAluminumSurfaceMat->AddProperty( "REFLECTIVITY",
                                         photonEnergies_ConstProp, aluminumRefl, 2);
    // Set surface material table
    gasArAluminumSurface->SetMaterialPropertiesTable(
                                                     gasArAluminumSurfaceMat );
    /* *************************************************** */
    
    /* ********* Create gas ArN2 - Gold boundary ********* */
    gasArN2GoldSurface = new G4OpticalSurface("gasArN2GoldSurface", unified,
                                              polished, dielectric_metal);
    // Create new materials table
    G4MaterialPropertiesTable *gasArN2GoldSurfaceMat =
    new G4MaterialPropertiesTable();
    gasArN2GoldSurfaceMat->AddProperty( "REFLECTIVITY",
                                       photonEnergies_ConstProp, goldRefl, 2);
    // Set surface material table
    gasArN2GoldSurface->SetMaterialPropertiesTable(gasArN2GoldSurfaceMat);
    /* *************************************************** */
    
    /* ********* Create gas ArN2 - Aluminum boundary ********* */
    gasArN2AluminumSurface = new G4OpticalSurface("gasArN2AluminumSurface",
                                                  unified, polished, dielectric_metal);
    // Create new materials table
    G4MaterialPropertiesTable *gasArN2AluminumSurfaceMat =
    new G4MaterialPropertiesTable();
    gasArN2AluminumSurfaceMat->AddProperty( "REFLECTIVITY",
                                           photonEnergies_ConstProp, aluminumRefl, 2);
    // Set surface material table
    gasArN2AluminumSurface->SetMaterialPropertiesTable(
                                                       gasArN2AluminumSurfaceMat );
    /* *************************************************** */
    
    /* ********* Create quartz - PTFE boundary ********* */
    quartzTeflonSurface = new G4OpticalSurface("quartzTeflonSurface",
                                               unified, groundfrontpainted, dielectric_dielectric);
    // Create new materials table
    G4MaterialPropertiesTable * quartzTeflonSurfaceMat =
    new G4MaterialPropertiesTable();
    G4double quartzTeflonRefl[2] = { 1.0, 1.0 };
    quartzTeflonSurfaceMat->AddProperty( "REFLECTIVITY",
                                        photonEnergies_ConstProp, quartzTeflonRefl, 2 );
    // Set surface material table
    quartzTeflonSurface->SetMaterialPropertiesTable(quartzTeflonSurfaceMat);
    /* *************************************************** */
    
    
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					CreateVisualAttributes()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::CreateVisualAttributes()
{
    //	For SetColor, the order is Red, Green, Blue, Alpha
    //	Colors included in the G4Color class are:
    //	White(), Gray(), Black()
    //	Red(), Green(), Blue()
    //	Cyan(), Magenta(), Yellow()
    
    
    
    //--- added for geometry parsing project (tms)
    testRedVis = new G4VisAttributes();
    testRedVis->SetColor( 0.8, 0, 0, 0.1 );
    testRedVis->SetVisibility( true );
    
    testCopperVis = new G4VisAttributes();
    testCopperVis->SetColor( 1.0, 0.5, 0, 0.1 );
    testCopperVis->SetVisibility( true );
    
    testBlueVis = new G4VisAttributes();
    testBlueVis->SetColor( 0, 0, 0.8, 0.1);
    testBlueVis->SetVisibility( true );
    
    testMagentaVis = new G4VisAttributes();
    testMagentaVis->SetColor( G4Color::Magenta() );
    testMagentaVis->SetVisibility( true );
    
    testGreenVis = new G4VisAttributes();
    testGreenVis->SetColor( 0.1, 0.5, 0, 0.1 );
    testGreenVis->SetVisibility( true );
    
    testPurpleVis = new G4VisAttributes();
    testPurpleVis->SetColor( 0.5, 0, 0.9, 0.1);
    testPurpleVis->SetVisibility( true );
    
    testDkTealVis = new G4VisAttributes();
    testDkTealVis->SetColor( 0, 0.7, 0.7, 0.1);
    testDkTealVis->SetVisibility( true );
    
    //--- end geometry parsing code (tms)
    
    acrylicVis = new G4VisAttributes();
    acrylicVis->SetColor( 0.5, 0.5, 0.5, 0.1 );
    acrylicVis->SetVisibility( true );
    
    airVis = new G4VisAttributes();
    airVis->SetColor( 0.5, 0.5, 0.5, 0.1 );
    airVis->SetVisibility( false );
    
    aluminumVis = new G4VisAttributes();
    aluminumVis->SetColor( 0.5, 0.5, 0.5, 0.1 );
    aluminumVis->SetVisibility( true );
    
    berylliumVis = new G4VisAttributes();
    berylliumVis->SetColor( 0.9, 0.9, 0.9, 0.1 );
    berylliumVis->SetVisibility( true );
    
    berylliumCopperVis = new G4VisAttributes();
    berylliumCopperVis->SetColor( 1.0, 0.7, 0.3, 0.1 );
    berylliumCopperVis->SetVisibility( true );
    
    blackiumVis = new G4VisAttributes();
    blackiumVis->SetVisibility( false );
    
    copperVis = new G4VisAttributes();
    copperVis->SetColor( 1.0, 0.5, 0, 0.1 );
    copperVis->SetVisibility( true );
    
    gasArVis = new G4VisAttributes();
    gasArVis->SetColor( 0.9, 0.5, 0.3, 0.1 );
    gasArVis->SetVisibility( false );
    
    gasArN2Vis = new G4VisAttributes();
    gasArN2Vis->SetColor( 0.5, 0.5, 0.5, 0.1 );
    gasArN2Vis->SetVisibility( false );
    
    gasN2Vis = new G4VisAttributes();
    gasN2Vis->SetColor( 0.5, 0.5, 0.5, 0.1 );
    gasN2Vis->SetVisibility( false );
    
    gasXeVis = new G4VisAttributes();
    gasXeVis->SetColor( 0.0, 1.0, 1.0, 0.4 );
    gasXeVis->SetVisibility( true );
    
    goldVis = new G4VisAttributes();
    goldVis->SetColor( 1.0, 0.5, 0, 0.1 );
    goldVis->SetVisibility( true );
    
    gs20GlassVis = new G4VisAttributes();
    gs20GlassVis->SetColor( 0.6, 0.6, 0.9, 0.1 );
    gs20GlassVis->SetVisibility( true );
    
    ironVis = new G4VisAttributes();
    ironVis->SetColor( 1.0, 0.7, 0.0, 0.1 );
    ironVis->SetVisibility( true );
    
    isohexaneVis = new G4VisAttributes();
    isohexaneVis->SetColor( G4Color::Yellow() );
    isohexaneVis->SetVisibility( true );
    
    labVis = new G4VisAttributes();
    labVis->SetColor( G4Color::Yellow() );
    labVis->SetVisibility( true );

    ej301Vis = new G4VisAttributes();
    ej301Vis->SetColor( 1., 1., 0., 0.5 );
    ej301Vis->SetVisibility( true );
   
    ej200Vis = new G4VisAttributes();
    ej200Vis->SetColor( 1., 1., 0., 0.5 );
    ej200Vis->SetVisibility( true );
 
    kaptonVis = new G4VisAttributes();
    kaptonVis->SetColor( 0.1, 0.1, 0.1, 0.1 );
    kaptonVis->SetVisibility( true );
    
    kg2GlassVis = new G4VisAttributes();
    kg2GlassVis->SetColor( 0.6, 0.6, 0.9, 0.1 );
    kg2GlassVis->SetVisibility( true );
    
    leadVis = new G4VisAttributes();
    leadVis->SetColor( 0.2, 0.2, 0.2 );
    leadVis->SetForceWireframe( true );
    leadVis->SetVisibility( true );
    
    leadGlassVis = new G4VisAttributes();
    leadGlassVis->SetColor( 0.9, 0.9, 0.9 );
    leadGlassVis->SetForceSolid( true );
    leadGlassVis->SetVisibility( true );
    
    liquidArVis = new G4VisAttributes();
    liquidArVis->SetColor( 0.5, 0.5, 1, 0.4 );
    liquidArVis->SetVisibility( true );
    
    liquidN2Vis = new G4VisAttributes();
    liquidN2Vis->SetColor( 0.5, 0.5, 0.5, 0.4 );
    liquidN2Vis->SetVisibility( true );
    
    liquidXeVis = new G4VisAttributes();
    liquidXeVis->SetColor( 0.0, 0.0, 1.0, 0.4 );
    liquidXeVis->SetVisibility( true );
    
    mineralOilVis = new G4VisAttributes();
    mineralOilVis->SetColor( G4Color::Yellow() );
    mineralOilVis->SetVisibility( true );
    
    peekVis = new G4VisAttributes();
    peekVis->SetColor( 1.0, 0.3, 0.0 );
    peekVis->SetVisibility( true );
    
    polyethyleneVis = new G4VisAttributes();
    polyethyleneVis->SetColor( G4Color::White() );
    polyethyleneVis->SetForceWireframe( true );
    polyethyleneVis->SetVisibility( true );
    
    polyethyleneBoratedVis = new G4VisAttributes();
    polyethyleneBoratedVis->SetColor( G4Color::Green() );
    polyethyleneBoratedVis->SetForceWireframe( true );
    polyethyleneBoratedVis->SetVisibility( true );
    
    pvcVis = new G4VisAttributes();
    pvcVis->SetColor( G4Color::Magenta() );
    pvcVis->SetVisibility( true );
    
    quartzVis = new G4VisAttributes();
    quartzVis->SetColor( G4Color::Magenta() );
    quartzVis->SetForceSolid( true );
    quartzVis->SetVisibility( true );
    
    rockVis = new G4VisAttributes();
    rockVis->SetColor( 1.0, 0.3, 0.0 );
    rockVis->SetForceWireframe( true );
    rockVis->SetVisibility( true );
    
    sapphireVis = new G4VisAttributes();
    sapphireVis->SetColor( 0.5, 0.5, 1.0, 0.1 );
    sapphireVis->SetForceSolid( true );
    sapphireVis->SetVisibility( true );
    
    aluminaVis = new G4VisAttributes();
    aluminaVis->SetColor( 0.5, 0.5, 1.0, 0.1 );
    aluminaVis->SetForceSolid( true );
    aluminaVis->SetVisibility( true );
    
    nicomicVis = new G4VisAttributes();
    nicomicVis->SetColor( 0.5, 0.5, 0.4, 0.1 );
    nicomicVis->SetForceSolid( true );
    nicomicVis->SetVisibility( true );
    
    inconelVis = new G4VisAttributes();
    inconelVis->SetColor( 0.5, 0.5, 0.5, 0.1 );
    inconelVis->SetForceSolid( true );
    inconelVis->SetVisibility( true );
    
    elastomerVis = new G4VisAttributes();
    elastomerVis->SetColor( 0.5, 0.5, 1.0, 0.1 );
    elastomerVis->SetForceSolid( true );
    elastomerVis->SetVisibility( true );
    
    steelVis = new G4VisAttributes();
    steelVis->SetColor( 0.4, 0.4, 0.4, 0.1 );
    steelVis->SetVisibility( true );
    
    teflonVis = new G4VisAttributes();
    teflonVis->SetColor( 1.0, 1.0, 1.0, 0.5 );
    teflonVis->SetVisibility( true );
    
    titaniumVis = new G4VisAttributes();
    titaniumVis->SetColor( 0.6, 0.6, 0.6, 0.1 );
    titaniumVis->SetVisibility( true );
    
    tpbVis = new G4VisAttributes();
    tpbVis->SetColor( 0.1, 0.1, 1.0, 0.1 );
    tpbVis->SetVisibility( true );
    tpbVis->SetForceSolid( true );
    
    tungstenVis = new G4VisAttributes();
    tungstenVis->SetColor( G4Color::Gray() );
    tungstenVis->SetForceWireframe( true );
    tungstenVis->SetVisibility( true );
    
    vacuumVis = new G4VisAttributes();
    vacuumVis->SetVisibility( false );
    
    waterVis = new G4VisAttributes();
    waterVis->SetColor( 0.0, 0.0, 1.0, 0.1 );
    waterVis->SetVisibility( true );
    
    cavRockVis = new G4VisAttributes();
    cavRockVis->SetColor( 1.0, 0., 0., 1.0 );
    cavRockVis->SetVisibility( true );
   
    portlandConcreteVis = new G4VisAttributes();
    portlandConcreteVis->SetColor( 1.0, 0., 0., 1.0 );
    portlandConcreteVis->SetVisibility( true );
    
 
    kovarVis = new G4VisAttributes();
    kovarVis->SetColor( 1.0, 1.0, 1.0, 0.5 );
    kovarVis->SetVisibility( true );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					InitializeMatrix()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::InitializeMatrix( G4int numEntries, G4int* theMatrix)
{
    for( G4int i=0; i<numEntries; i++ )
        theMatrix[i] = 0;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					InitializeMatrix()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::InitializeMatrix( G4int numEntries, G4double* theMatrix)
{
    for( G4int i=0; i<numEntries; i++ )
        theMatrix[i] = 0;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                                      UpdateElectricField()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::UpdateElectricField()
{
    G4cout << G4endl << G4endl << G4endl;
    G4cout << "Whoops! Someone called UpdateElectricField(), and shouldn't have"
           << G4endl;
    G4cout << G4endl << G4endl << G4endl;
    exit(0);
    
    //---------- Gas ----------------------
//    gasXe->GetMaterialPropertiesTable();
//    if( gasXeMat->ConstPropertyExists( "ELECTRICFIELD" ))
//        gasXeMat->RemoveConstProperty( "ELECTRICFIELD" );
//    if( gasXeMat->ConstPropertyExists( "ELECTRICFIELDWINDOW" ))
//        gasXeMat->RemoveConstProperty( "ELECTRICFIELDWINDOW") ;
//    if( gasXeMat->ConstPropertyExists( "ELECTRICFIELDTOP" ))
//        gasXeMat->RemoveConstProperty( "ELECTRICFIELDTOP" );
//    if( gasXeMat->ConstPropertyExists( "ELECTRICFIELDANODE" ))
//        gasXeMat->RemoveConstProperty( "ELECTRICFIELDANODE" );
//    G4double setField = baccManager->GetElectricFieldGas();
//    gasXeMat->AddConstProperty("ELECTRICFIELD", setField );
//    //G4cout << "Setting gaseous xenon electric field to: "<< setField << G4endl;
//    setField = baccManager->GetElectricFieldGasWindow();
//    gasXeMat->AddConstProperty( "ELECTRICFIELDWINDOW", setField );
//    setField = baccManager->GetElectricFieldGasTop();
//    gasXeMat->AddConstProperty( "ELECTRICFIELDTOP", setField );
//    setField = baccManager->GetElectricFieldGasAnode();
//    gasXeMat->AddConstProperty( "ELECTRICFIELDANODE", setField );
//    //Update gasXe Properties
//    gasXe->SetMaterialPropertiesTable( gasXeMat );
//    
//    //---------- Liquid -------------------
//    liquidXe->GetMaterialPropertiesTable();
//    if( liquidXeMat->ConstPropertyExists( "ELECTRICFIELD" ))
//        liquidXeMat->RemoveConstProperty( "ELECTRICFIELD" );
//    if( liquidXeMat->ConstPropertyExists( "ELECTRICFIELDSURFACE" ))
//        liquidXeMat->RemoveConstProperty( "ELECTRICFIELDSURFACE" );
//    if( liquidXeMat->ConstPropertyExists( "ELECTRICFIELDGATE" ))
//        liquidXeMat->RemoveConstProperty( "ELECTRICFIELDGATE" );
//    if( liquidXeMat->ConstPropertyExists( "ELECTRICFIELDCATHODE" ))
//        liquidXeMat->RemoveConstProperty( "ELECTRICFIELDCATHODE" );
//    if( liquidXeMat->ConstPropertyExists( "ELECTRICFIELDBOTTOM" ))
//        liquidXeMat->RemoveConstProperty( "ELECTRICFIELDBOTTOM" );
//    setField = baccManager->GetElectricFieldLiquid();
//    liquidXeMat->AddConstProperty("ELECTRICFIELD", setField );
//    //G4cout << "Setting liquid xenon electric field to: "<< setField << G4endl;
//    setField = baccManager->GetElectricFieldLiquidSurface();
//    liquidXeMat->AddConstProperty( "ELECTRICFIELDSURFACE", setField );
//    setField = baccManager->GetElectricFieldLiquidGate();
//    liquidXeMat->AddConstProperty( "ELECTRICFIELDGATE", setField );
//    setField = baccManager->GetElectricFieldLiquidCathode();
//    liquidXeMat->AddConstProperty( "ELECTRICFIELDCATHODE", setField );
//    setField = baccManager->GetElectricFieldLiquidBottom();
//    liquidXeMat->AddConstProperty( "ELECTRICFIELDBOTTOM", setField );
//    //Update liquid xenon properties
//    liquidXe->SetMaterialPropertiesTable( liquidXeMat );
}
//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetQuartzAbs()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetQuartzAbs( G4double l )
{
    
    quartzConstAbsorption[0] = quartzConstAbsorption[1] = l;
    quartzMat->RemoveConstProperty( "ABSLENGTH" );
    quartzMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                           quartzConstAbsorption, 2 );
    G4cout << "Quartz absorption length set to " << 0.1*l << " cm" << G4endl;
    // G4cout << "Contents of quartzMat: " << G4endl;
    // quartzMat -> DumpTable(); // It works indeed.
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetLXeTeflonRefl( G4double r )
{
    G4MaterialPropertiesTable *tmp =
    liquidXeTeflonSurface->GetMaterialPropertiesTable();
    G4double liquidXeTeflonRefl[2];
    liquidXeTeflonRefl[0] = liquidXeTeflonRefl[1] = r;
    tmp->AddProperty( "REFLECTIVITY",
                     photonEnergies_ConstProp, liquidXeTeflonRefl, 2 );
    // Set surface material table
    liquidXeTeflonSurface->SetMaterialPropertiesTable(tmp);
    G4cout << "LXe/PTFE reflectivity set to " << 100.*r << "%" << G4endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonLinerRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetLXeTeflonLinerRefl( G4double r )
{
    G4MaterialPropertiesTable *tmp =
    liquidXeTeflonSurfaceLiner->GetMaterialPropertiesTable();
    G4double liquidXeTeflonRefl[2];
    liquidXeTeflonRefl[0] = liquidXeTeflonRefl[1] = r;
    tmp->AddProperty( "REFLECTIVITY",
                     photonEnergies_ConstProp, liquidXeTeflonRefl, 2 );
    // Set surface material table
    liquidXeTeflonSurfaceLiner->SetMaterialPropertiesTable(tmp);
    G4cout << "LXe/PTFE Liner reflectivity set to " << 100.*r << "%" << G4endl;
}
// //------++++++------++++++------++++++------++++++------++++++------++++++------
// //					SetLXeTeflonSkinRefl()
// //------++++++------++++++------++++++------++++++------++++++------++++++------
// void BaccMaterials::SetLXeTeflonSkinRefl( G4double r )
// {
//     G4MaterialPropertiesTable *tmp =
//     liquidXeTeflonSurfaceSkin->GetMaterialPropertiesTable();
//     G4double liquidXeTeflonRefl[2];
//     liquidXeTeflonRefl[0] = liquidXeTeflonRefl[1] = r;
//     tmp->AddProperty( "REFLECTIVITY",
//                      photonEnergies_ConstProp, liquidXeTeflonRefl, 2 );
//     // Set surface material table
//     liquidXeTeflonSurfaceSkin->SetMaterialPropertiesTable(tmp);
//     G4cout << "LXe/PTFE Liner reflectivity set to " << 100.*r << "%" << G4endl;
// }

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonPMTRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetLXeTeflonPMTRefl( G4double r )
{
    G4MaterialPropertiesTable *tmp =
    liquidXeTeflonSurfacePMT->GetMaterialPropertiesTable();
    G4double liquidXeTeflonRefl[2];
    liquidXeTeflonRefl[0] = liquidXeTeflonRefl[1] = r;
    tmp->AddProperty( "REFLECTIVITY",
                     photonEnergies_ConstProp, liquidXeTeflonRefl, 2 );
    // Set surface material table
    liquidXeTeflonSurfacePMT->SetMaterialPropertiesTable(tmp);
    G4cout << "LXe/PTFE PMT reflectivity set to " << 100.*r << "%" << G4endl;
}
//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonPlateRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetLXeTeflonPlateRefl( G4double r )
{
    G4MaterialPropertiesTable *tmp =
    liquidXeTeflonSurfacePlate->GetMaterialPropertiesTable();
    G4double liquidXeTeflonRefl[2];
    liquidXeTeflonRefl[0] = liquidXeTeflonRefl[1] = r;
    tmp->AddProperty( "REFLECTIVITY",
                     photonEnergies_ConstProp, liquidXeTeflonRefl, 2 );
    // Set surface material table
    liquidXeTeflonSurfacePlate->SetMaterialPropertiesTable(tmp);
    G4cout << "LXe/PTFE plate reflectivity set to " << 100.*r << "%" << G4endl;
}
//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonCablesRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetLXeTeflonCablesRefl( G4double r )
{
    G4MaterialPropertiesTable *tmp =
    liquidXeTeflonSurfaceCables->GetMaterialPropertiesTable();
    G4double liquidXeTeflonRefl[2];
    liquidXeTeflonRefl[0] = liquidXeTeflonRefl[1] = r;
    tmp->AddProperty( "REFLECTIVITY",
                     photonEnergies_ConstProp, liquidXeTeflonRefl, 2 );
    // Set surface material table
    liquidXeTeflonSurfaceCables->SetMaterialPropertiesTable(tmp);
    G4cout << "LXe/PTFE cables reflectivity set to " << 100.*r << "%" << G4endl;
}
//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonTrussRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetLXeTeflonTrussRefl( G4double r )
{
    G4MaterialPropertiesTable *tmp =
    liquidXeTeflonSurfaceTruss->GetMaterialPropertiesTable();
    G4double liquidXeTeflonRefl[2];
    liquidXeTeflonRefl[0] = liquidXeTeflonRefl[1] = r;
    tmp->AddProperty( "REFLECTIVITY",
                     photonEnergies_ConstProp, liquidXeTeflonRefl, 2 );
    // Set surface material table
    liquidXeTeflonSurfaceTruss->SetMaterialPropertiesTable(tmp);
    G4cout << "LXe/PTFE Truss reflectivity set to " << 100.*r << "%" << G4endl;
}
//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonBskinPMTRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetLXeTeflonBskinPMTRefl( G4double r )
{
    G4MaterialPropertiesTable *tmp =
    liquidXeTeflonSurfaceBskinPMT->GetMaterialPropertiesTable();
    G4double liquidXeTeflonRefl[2];
    liquidXeTeflonRefl[0] = liquidXeTeflonRefl[1] = r;
    tmp->AddProperty( "REFLECTIVITY",
                     photonEnergies_ConstProp, liquidXeTeflonRefl, 2 );
    // Set surface material table
    liquidXeTeflonSurfaceBskinPMT->SetMaterialPropertiesTable(tmp);
    G4cout << "LXe/PTFE BskinPMT reflectivity set to " << 100.*r << "%" << G4endl;
}
//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonBplateRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetLXeTeflonBplateRefl( G4double r )
{
    G4MaterialPropertiesTable *tmp =
    liquidXeTeflonSurfaceBplate->GetMaterialPropertiesTable();
    G4double liquidXeTeflonRefl[2];
    liquidXeTeflonRefl[0] = liquidXeTeflonRefl[1] = r;
    tmp->AddProperty( "REFLECTIVITY",
                     photonEnergies_ConstProp, liquidXeTeflonRefl, 2 );
    // Set surface material table
    liquidXeTeflonSurfaceBplate->SetMaterialPropertiesTable(tmp);
    G4cout << "LXe/PTFE Bplate reflectivity set to " << 100.*r << "%" << G4endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonTskinPMTRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetLXeTeflonTskinPMTRefl( G4double r )
{
    G4MaterialPropertiesTable *tmp =
    liquidXeTeflonSurfaceTskinPMT->GetMaterialPropertiesTable();
    G4double liquidXeTeflonRefl[2];
    liquidXeTeflonRefl[0] = liquidXeTeflonRefl[1] = r;
    tmp->AddProperty( "REFLECTIVITY",
                     photonEnergies_ConstProp, liquidXeTeflonRefl, 2 );
    // Set surface material table
    liquidXeTeflonSurfaceTskinPMT->SetMaterialPropertiesTable(tmp);
    G4cout << "LXe/PTFE TskinPMT reflectivity set to " << 100.*r << "%" << G4endl;
}
//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonWeirRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetLXeTeflonWeirRefl( G4double r )
{
    G4MaterialPropertiesTable *tmp =
    liquidXeTeflonSurfaceWeir->GetMaterialPropertiesTable();
    G4double liquidXeTeflonRefl[2];
    liquidXeTeflonRefl[0] = liquidXeTeflonRefl[1] = r;
    tmp->AddProperty( "REFLECTIVITY",
                     photonEnergies_ConstProp, liquidXeTeflonRefl, 2 );
    // Set surface material table
    liquidXeTeflonSurfaceWeir->SetMaterialPropertiesTable(tmp);
    G4cout << "LXe/PTFE Weir reflectivity set to " << 100.*r << "%" << G4endl;
}
//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeTeflonDomePMTRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetLXeTeflonDomePMTRefl( G4double r )
{
    G4MaterialPropertiesTable *tmp =
    liquidXeTeflonSurfaceDomePMT->GetMaterialPropertiesTable();
    G4double liquidXeTeflonRefl[2];
    liquidXeTeflonRefl[0] = liquidXeTeflonRefl[1] = r;
    tmp->AddProperty( "REFLECTIVITY",
                     photonEnergies_ConstProp, liquidXeTeflonRefl, 2 );
    // Set surface material table
    liquidXeTeflonSurfaceDomePMT->SetMaterialPropertiesTable(tmp);
    G4cout << "LXe/PTFE DomePMT reflectivity set to " << 100.*r << "%" << G4endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeSteelRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetLXeSteelRefl( G4double r )
{
    G4MaterialPropertiesTable *tmp =
    liquidXeSteelSurface->GetMaterialPropertiesTable();
    G4double liquidXeSteelRefl[2];
    liquidXeSteelRefl[0] = liquidXeSteelRefl[1] = r;
    tmp->RemoveConstProperty( "REFLECTIVITY" );
    tmp->AddProperty( "REFLECTIVITY",
                     photonEnergies_ConstProp, liquidXeSteelRefl, 2 );
    // Set surface material table
    liquidXeSteelSurface->SetMaterialPropertiesTable(tmp);
    G4cout << "LXe/steel reflectivity set to " << 100.*r << "%" << G4endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeAbs()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetLXeAbs( G4double l )
{
    liquidXeAbslength[0] = liquidXeAbslength[1] = l;
    liquidXeMat->RemoveConstProperty( "ABSLENGTH" );
    liquidXeMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                             liquidXeAbslength, 2 );
    /*liquidXeAbslength = l;
     SetOpticalDebugging( opticalDebugging );*/
    G4cout << "LXe absorption length set to " << 0.1*l << " cm" << G4endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetLXeRayleigh()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetLXeRayleigh( G4double l )
{
    G4double liquidXeRayleighlength[2];
    liquidXeRayleighlength[0] = liquidXeRayleighlength[1] = l;
    liquidXeMat->RemoveConstProperty( "RAYLEIGH" );
    liquidXeMat->AddProperty( "RAYLEIGH", photonEnergies_ConstProp,
                             liquidXeRayleighlength, 2 );
    G4cout << "LXe Rayleigh length set to " << 0.1*l << " cm" << G4endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetGXeTeflonRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetGXeTeflonRefl( G4double r )
{
    G4MaterialPropertiesTable *tmp =
    gasXeTeflonSurface->GetMaterialPropertiesTable();
    G4double gasXeTeflonRefl[2];
    gasXeTeflonRefl[0] = gasXeTeflonRefl[1] = r;
    tmp->AddProperty( "REFLECTIVITY",
                     photonEnergies_ConstProp, gasXeTeflonRefl, 2 );
    // Set surface material table
    gasXeTeflonSurface->SetMaterialPropertiesTable(tmp);
    G4cout << "GXe/PTFE reflectivity set to " << 100.*r << "%" << G4endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetGXeBlackWallRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetGXeBlackWallRefl( G4double r )
{
    G4MaterialPropertiesTable *tmp =
    gasXeBlackSurface->GetMaterialPropertiesTable();
    G4double gasXeBlackWallRefl[2];
    gasXeBlackWallRefl[0] = gasXeBlackWallRefl[1] = r;
    tmp->AddProperty( "REFLECTIVITY",
                     photonEnergies_ConstProp, gasXeBlackWallRefl, 2 );
    // Set surface material table
    gasXeBlackSurface->SetMaterialPropertiesTable(tmp);
    G4cout << "GXe/\"Black Wall\" reflectivity set to " << 100.*r << "%" << G4endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetGXeSteelRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetGXeSteelRefl( G4double r )
{
    G4MaterialPropertiesTable *tmp =
    gasXeSteelSurface->GetMaterialPropertiesTable();
    G4double gasXeSteelRefl[2];
    gasXeSteelRefl[0] = gasXeSteelRefl[1] = r;
    tmp->AddProperty( "REFLECTIVITY",
                     photonEnergies_ConstProp, gasXeSteelRefl, 2 );
    // Set surface material table
    gasXeSteelSurface->SetMaterialPropertiesTable(tmp);
    G4cout << "GXe/steel reflectivity set to " << 100.*r << "%" << G4endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetGXeAbs()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetGXeAbs( G4double l )
{
    gasXeAbslength[0] = gasXeAbslength[1] = l;
    gasXeMat->RemoveConstProperty( "ABSLENGTH" );
    gasXeMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                          gasXeAbslength, 2 );
    G4cout << "GXe absorption length set to " << 0.1*l << " cm" << G4endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetGXeRayleigh()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetGXeRayleigh( G4double l )
{
    G4double gasXeRayleighlength[2];
    gasXeRayleighlength[0] = gasXeRayleighlength[1] = l;
    gasXeMat->RemoveConstProperty( "RAYLEIGH" );
    gasXeMat->AddProperty( "RAYLEIGH", photonEnergies_ConstProp,
                          gasXeRayleighlength, 2 );
    G4cout << "GXe Rayleigh length set to " << 0.1*l << " cm" << G4endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetAlUnoxQuartzRefl()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetAlUnoxQuartzRefl( G4double r )
{
    G4MaterialPropertiesTable *tmp =
    aluminumQuartzSurface->GetMaterialPropertiesTable();
    G4double alUnoxQuartzRefl[2];
    alUnoxQuartzRefl[0] = alUnoxQuartzRefl[1] = r;
    tmp->AddProperty( "REFLECTIVITY",
                     photonEnergies_ConstProp, alUnoxQuartzRefl, 2 );
    // Set surface material table
    aluminumQuartzSurface->SetMaterialPropertiesTable(tmp);
    G4cout << "Unoxidized Al reflectivity set to " << 100.*r << "%" << G4endl;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetOpticalDebugging()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccMaterials::SetOpticalDebugging( G4bool debug )
{
    //	Set the flag that says whether we are using simplified optical
    //	properties for debugging purposes. These simplifications are:
    //		1. Set all indices of refraction to 1.
    //		2. Set the absorption and Rayleigh scattering lengths for water,
    //		   liquid xenon, gaseous xenon, and quartz (basically, all
    //		   transparent materials) to 1000 km.
    //		3. Set quartz transmission to 100%.
    //		4. Set the reflectivity of all opaque materials (including diffuse
    //		   reflectors such as PFTE and HDPE) to 0%.
    opticalDebugging = debug;
    
    const G4int NUM_PP_ConstProp = num_pp_constProp;
    const G4int NUM_PP = num_pp;
    const G4int nWaterEntries = num_water;
    const G4int lab_Rayleigh_entries = num_lab_Rayleigh;
    const G4int acrylicTransmittance_entries = num_acrylicTransmittance;
    
    std::vector<std::string> properties;
    properties.push_back( "RINDEX" );
    properties.push_back( "REFLECTIVITY" );
    properties.push_back( "ABSLENGTH" );
    properties.push_back( "RALEIGH" );
    properties.push_back( "GROUPVEL" );
    properties.push_back( "TRANSMITTANCE" );
    
    for( G4int i=0; i<(G4int)properties.size(); i++ ) {
        if( acrylicMat->ConstPropertyExists( properties[i].c_str() ) )
            acrylicMat->RemoveProperty( properties[i].c_str() );
        if( airMat->ConstPropertyExists( properties[i].c_str() ) )
            airMat->RemoveProperty( properties[i].c_str() );
        if( aluminumMat->ConstPropertyExists( properties[i].c_str() ) )
            aluminumMat->RemoveProperty( properties[i].c_str() );
        if( berylliumMat->ConstPropertyExists( properties[i].c_str() ) )
            berylliumMat->RemoveProperty( properties[i].c_str() );
        if( berylliumCopperMat->ConstPropertyExists( properties[i].c_str() ) )
            berylliumCopperMat->RemoveProperty( properties[i].c_str() );
        if( copperMat->ConstPropertyExists( properties[i].c_str() ) )
            copperMat->RemoveProperty( properties[i].c_str() );
        if( gasArMat->ConstPropertyExists( properties[i].c_str() ) )
            gasArMat->RemoveProperty( properties[i].c_str() );
        if( gasArN2Mat->ConstPropertyExists( properties[i].c_str() ) )
            gasArN2Mat->RemoveProperty( properties[i].c_str() );
        if( gasN2Mat->ConstPropertyExists( properties[i].c_str() ) )
            gasN2Mat->RemoveProperty( properties[i].c_str() );
        if( gasXeMat->ConstPropertyExists( properties[i].c_str() ) )
            gasXeMat->RemoveProperty( properties[i].c_str() );
        if( goldMat->ConstPropertyExists( properties[i].c_str() ) )
            goldMat->RemoveProperty( properties[i].c_str() );
        if( halonMat->ConstPropertyExists( properties[i].c_str() ) )
            halonMat->RemoveProperty( properties[i].c_str() );
        if( ironMat->ConstPropertyExists( properties[i].c_str() ) )
            ironMat->RemoveProperty( properties[i].c_str() );
        if( isohexaneMat->ConstPropertyExists( properties[i].c_str() ) )
            isohexaneMat->RemoveProperty( properties[i].c_str() );
        if( labMat->ConstPropertyExists( properties[i].c_str() ) )
            labMat->RemoveProperty( properties[i].c_str() );
        if( dopedLABGdMat->ConstPropertyExists( properties[i].c_str() ) )
            dopedLABGdMat->RemoveProperty( properties[i].c_str() );
        if( leadMat->ConstPropertyExists( properties[i].c_str() ) )
            leadMat->RemoveProperty( properties[i].c_str() );
        if( leadGlassMat->ConstPropertyExists( properties[i].c_str() ) )
            leadGlassMat->RemoveProperty( properties[i].c_str() );
        if( liquidArMat->ConstPropertyExists( properties[i].c_str() ) )
            liquidArMat->RemoveProperty( properties[i].c_str() );
        liquidArMat->RemoveProperty( properties[i].c_str() );
        if( liquidN2Mat->ConstPropertyExists( properties[i].c_str() ) )
            liquidN2Mat->RemoveProperty( properties[i].c_str() );
        if( liquidXeMat->ConstPropertyExists( properties[i].c_str() ) )
            liquidXeMat->RemoveProperty( properties[i].c_str() );
        if( mineralOilMat->ConstPropertyExists( properties[i].c_str() ) )
            mineralOilMat->RemoveProperty( properties[i].c_str() );
        if( peekMat->ConstPropertyExists( properties[i].c_str() ) )
            peekMat->RemoveProperty( properties[i].c_str() );
        if( polyethyleneMat->ConstPropertyExists( properties[i].c_str() ) )
            polyethyleneMat->RemoveProperty( properties[i].c_str() );
        if( pvcMat->ConstPropertyExists( properties[i].c_str() ) )
            pvcMat->RemoveProperty( properties[i].c_str() );
        if( quartzMat->ConstPropertyExists( properties[i].c_str() ) )
            quartzMat->RemoveProperty( properties[i].c_str() );
        if( ss316Mat->ConstPropertyExists( properties[i].c_str() ) )
            ss316Mat->RemoveProperty( properties[i].c_str() );
        if( steelMat->ConstPropertyExists( properties[i].c_str() ) )
            steelMat->RemoveProperty( properties[i].c_str() );
        if( teflonMat->ConstPropertyExists( properties[i].c_str() ) )
            teflonMat->RemoveProperty( properties[i].c_str() );
        if( titaniumMat->ConstPropertyExists( properties[i].c_str() ) )
            titaniumMat->RemoveProperty( properties[i].c_str() );
        if( tungstenMat->ConstPropertyExists( properties[i].c_str() ) )
            tungstenMat->RemoveProperty( properties[i].c_str() );
        if( heavyMetMat->ConstPropertyExists( properties[i].c_str() ) )
            heavyMetMat->RemoveProperty( properties[i].c_str() );
        if( tyvekMat->ConstPropertyExists( properties[i].c_str() ) )
            tyvekMat->RemoveProperty( properties[i].c_str() );
        if( lzSpecRefMat->ConstPropertyExists( properties[i].c_str() ) )
            lzSpecRefMat->RemoveProperty( properties[i].c_str() );
        if( tyvek600Mat->ConstPropertyExists( properties[i].c_str() ) )
            tyvek600Mat->RemoveProperty( properties[i].c_str() );
        if( waterMat->ConstPropertyExists( properties[i].c_str() ) )
            waterMat->RemoveProperty( properties[i].c_str() );
    }
    
    if( debug ) {
        acrylicMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                                optDebugRefIndex, NUM_PP_ConstProp);
        acrylicMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                                optDebugReflection, NUM_PP_ConstProp);
        acrylicMat->AddProperty( "TRANSMITTANCE", photonEnergies_ConstProp,
                                optDebugReflection, NUM_PP_ConstProp);
        airMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                            optDebugRefIndex, NUM_PP_ConstProp );
        aluminumMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                                 optDebugReflection, NUM_PP_ConstProp );
        berylliumMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                                  optDebugReflection, NUM_PP_ConstProp );
        berylliumCopperMat->AddProperty( "REFLECTIVITY",
                                        photonEnergies_ConstProp, optDebugReflection,
                                        NUM_PP_ConstProp );
        copperMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                               optDebugReflection, NUM_PP_ConstProp );
        gasArMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                              optDebugRefIndex, NUM_PP_ConstProp );
        gasArMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                              optDebugAbsLength, NUM_PP_ConstProp );
        gasArN2Mat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                                optDebugRefIndex, NUM_PP_ConstProp );
        gasArN2Mat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                                optDebugAbsLength, NUM_PP_ConstProp );
        gasN2Mat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                              optDebugRefIndex, NUM_PP_ConstProp );
        gasN2Mat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                              optDebugAbsLength, NUM_PP_ConstProp );
        gasXeMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                              optDebugRefIndex, NUM_PP_ConstProp );
        gasXeMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                              optDebugAbsLength, NUM_PP_ConstProp );
        goldMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                             optDebugReflection, NUM_PP_ConstProp );
        halonMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                              optDebugReflection, NUM_PP_ConstProp );
        ironMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                             optDebugReflection, NUM_PP_ConstProp );
        isohexaneMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                                  optDebugRefIndex, NUM_PP_ConstProp );
        labMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                            optDebugRefIndex, NUM_PP_ConstProp );
        dopedLABGdMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                                   optDebugRefIndex, NUM_PP_ConstProp );
        dopedLABGdMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                                   optDebugAbsLength, NUM_PP_ConstProp );
        dopedLABGdMat->AddProperty( "RAYLEIGH", photonEnergies_ConstProp,
                                   optDebugRayleighLength, NUM_PP_ConstProp );
        leadMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                             optDebugReflection, NUM_PP_ConstProp );
        leadGlassMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                                  optDebugRefIndex, NUM_PP_ConstProp );
        leadGlassMat->AddProperty( "GROUPVEL", photonEnergies_ConstProp,
                                  optDebugGroupVel, NUM_PP_ConstProp );
        leadGlassMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                                  optDebugAbsLength, NUM_PP_ConstProp );
        liquidXeMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                                 optDebugRefIndex, NUM_PP_ConstProp );
        liquidXeMat->AddProperty( "GROUPVEL", photonEnergies_ConstProp,
                                 optDebugGroupVel, NUM_PP_ConstProp );
        liquidXeMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                                 optDebugAbsLength, NUM_PP_ConstProp );
        liquidXeMat->AddProperty( "RAYLEIGH", photonEnergies_ConstProp,
                                 optDebugRayleighLength, NUM_PP_ConstProp );
        mineralOilMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                                   optDebugRefIndex, NUM_PP_ConstProp );
        peekMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                             optDebugReflection, NUM_PP_ConstProp);
        polyethyleneMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                                     optDebugReflection, NUM_PP_ConstProp);
        pvcMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                            optDebugReflection, NUM_PP_ConstProp);
        quartzMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                               optDebugRefIndex, NUM_PP_ConstProp );
        quartzMat->AddProperty( "GROUPVEL", photonEnergies_ConstProp,
                               optDebugGroupVel, NUM_PP_ConstProp );
        quartzMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                               optDebugAbsLength, NUM_PP_ConstProp );	
        sapphireMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                                 optDebugRefIndex, NUM_PP_ConstProp );
        sapphireMat->AddProperty( "GROUPVEL", photonEnergies_ConstProp,
                                 optDebugGroupVel, NUM_PP_ConstProp );
        sapphireMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                                 optDebugAbsLength, NUM_PP_ConstProp );	
        ss316Mat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                              optDebugReflection, NUM_PP_ConstProp );
        steelMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                              optDebugReflection, NUM_PP_ConstProp );
        teflonMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                               optDebugRefIndex, NUM_PP_ConstProp );
        teflonMat->AddProperty( "GROUPVEL", photonEnergies_ConstProp,
                               optDebugGroupVel, NUM_PP_ConstProp );
        teflonMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                               optDebugReflection, NUM_PP_ConstProp );
        titaniumMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                                 optDebugReflection, NUM_PP_ConstProp );
        tungstenMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                                 optDebugReflection, NUM_PP_ConstProp );
        tyvekMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                              optDebugReflection, NUM_PP_ConstProp );
        lzSpecRefMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                                  optDebugReflection, NUM_PP_ConstProp );
        tyvek600Mat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                                 optDebugReflection, NUM_PP_ConstProp );
        waterMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                              optDebugRefIndex, NUM_PP_ConstProp);
        waterMat->AddProperty( "GROUPVEL", photonEnergies_ConstProp,
                              optDebugGroupVel, NUM_PP_ConstProp);
        waterMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                              optDebugAbsLength, NUM_PP_ConstProp);
    } else {
        acrylicMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                                acrylicRefractiveIndex, NUM_PP_ConstProp);
        acrylicMat->AddProperty( "REFLECTIVITY", acrylicTransmittance_energies,
                                acrylicReflectivity, acrylicTransmittance_entries);
        acrylicMat->AddProperty( "TRANSMITTANCE", acrylicTransmittance_energies,
                                acrylicTransmittance, acrylicTransmittance_entries);
        airMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                            airRefractiveIndex, NUM_PP_ConstProp );
        aluminumMat->AddProperty( "REFLECTIVITY", photonEnergies, aluminumRefl,
                                 NUM_PP );
        berylliumMat->AddProperty( "REFLECTIVITY", photonEnergies,
                                  berylliumRefl, NUM_PP );
        berylliumCopperMat->AddProperty( "REFLECTIVITY", photonEnergies,
                                        berylliumCopperRefl, NUM_PP );
        copperMat->AddProperty( "REFLECTIVITY", photonEnergies, copperRefl,
                               NUM_PP );
        gasArMat->AddProperty( "RINDEX", photonEnergies_ConstProp, gasArRindex,
                              NUM_PP_ConstProp );
        gasArMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                              gasArAbslength, NUM_PP_ConstProp );
        gasArN2Mat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                                gasArN2Rindex, NUM_PP_ConstProp );
        gasArN2Mat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                                gasArN2Abslength, NUM_PP_ConstProp );
        gasN2Mat->AddProperty( "RINDEX", photonEnergies_ConstProp, gasN2Rindex,
                              NUM_PP_ConstProp );
        gasN2Mat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                              gasN2Abslength, NUM_PP_ConstProp );
        gasXeMat->AddProperty( "RINDEX", photonEnergies_ConstProp, gasXeRindex,
                              NUM_PP_ConstProp );
        gasXeMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                              gasXeAbslength, NUM_PP_ConstProp );
        goldMat->AddProperty( "REFLECTIVITY", photonEnergies, goldRefl,
                             NUM_PP );
        halonMat->AddProperty( "REFLECTIVITY", waterPhotonEnergies,
                              halonRefl, nWaterEntries );
        ironMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                             ironRefl, NUM_PP_ConstProp );
        isohexaneMat->AddProperty( "RINDEX", photonEnergies, isohexaneRindex,
                                  NUM_PP_ConstProp );
        labMat->AddProperty( "RINDEX", photonEnergies_ConstProp, labRindex,
                            NUM_PP_ConstProp );
        dopedLABGdMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                                   labRindex, NUM_PP_ConstProp );
        dopedLABGdMat->AddProperty( "ABSLENGTH", labAbsorptionEnergies,
                                   labAbsorption, num_labAbs );
        dopedLABGdMat->AddProperty( "RAYLEIGH", lab_Rayleigh_energy,
                                   lab_Rayleigh_length, lab_Rayleigh_entries );
        //dopedLABGdMat->AddProperty( "ABSLENGTH", tempLABAbsEnergies,
        //      tempLABAbsLength, NUM_PP_ConstProp ); //temp
        leadMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                             leadRefl, NUM_PP_ConstProp );
        leadGlassMat->AddProperty( "RINDEX", photonEnergies,
                                  leadGlassRefractiveIndex, NUM_PP );
        leadGlassMat->AddProperty( "GROUPVEL", photonEnergies,
                                  leadGlassGroupVel, NUM_PP );
        leadGlassMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                                  leadGlassAbsorption, NUM_PP_ConstProp );
        liquidXeMat->AddProperty( "RINDEX", photonEnergies, liquidXeRindex,
                                 NUM_PP );
        liquidXeMat->AddProperty( "GROUPVEL", photonEnergies, liquidXeGroupVel,
                                 NUM_PP );
        liquidXeMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                                 liquidXeAbslength, NUM_PP_ConstProp );
        liquidXeMat->AddProperty( "RAYLEIGH", photonEnergies,
                                 liquidXeRayleigh, NUM_PP );
        mineralOilMat->AddProperty( "RINDEX", photonEnergies_ConstProp,
                                   mineralOilRIndex, NUM_PP_ConstProp );
        peekMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                             peekRefl, NUM_PP_ConstProp );
        polyethyleneMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                                     polyethyleneRefl, NUM_PP_ConstProp);
        pvcMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                            pvcRefl, NUM_PP_ConstProp);
        quartzMat->AddProperty( "RINDEX", photonEnergies, quartzRefractiveIndex,
                               NUM_PP );
        quartzMat->AddProperty( "GROUPVEL", photonEnergies, quartzGroupVel,
                               NUM_PP );
        quartzMat->AddProperty( "ABSLENGTH", photonEnergies,
                               quartzAbsorption, NUM_PP );
        sapphireMat->AddProperty( "RINDEX", photonEnergies,
                                 sapphireRefractiveIndex, NUM_PP );
        sapphireMat->AddProperty( "GROUPVEL", photonEnergies,
                                 sapphireGroupVel, NUM_PP );
        sapphireMat->AddProperty( "ABSLENGTH", photonEnergies_ConstProp,
                                 sapphireAbsorption, NUM_PP_ConstProp );
        ss316Mat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                              ss316Refl, NUM_PP_ConstProp );
        steelMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                              steelRefl, NUM_PP_ConstProp );
        teflonMat->AddProperty( "RINDEX", photonEnergies, teflonRefractiveIndex,
                               NUM_PP );
        teflonMat->AddProperty( "GROUPVEL", photonEnergies, teflonGroupVel,
                               NUM_PP );
        teflonMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                               teflonRefl, NUM_PP_ConstProp );
        titaniumMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                                 titaniumRefl, NUM_PP_ConstProp );
        tungstenMat->AddProperty( "REFLECTIVITY", photonEnergies_ConstProp,
                                 tungstenRefl, NUM_PP_ConstProp );
        tyvekMat->AddProperty( "REFLECTIVITY", waterPhotonEnergies,
                              tyvekRefl, nWaterEntries );
        lzSpecRefMat->AddProperty( "REFLECTIVITY", waterPhotonEnergies,
                                  lzSpecRefRefl, nWaterEntries );
        tyvek600Mat->AddProperty( "REFLECTIVITY", waterPhotonEnergies,
                                 tyvek600Refl, nWaterEntries );
        waterMat->AddProperty( "RINDEX", waterPhotonEnergies,
                              waterRefractiveIndex, nWaterEntries);
        waterMat->AddProperty( "GROUPVEL", waterPhotonEnergies,
                              waterGroupVel, nWaterEntries);
        waterMat->AddProperty( "ABSLENGTH", waterPhotonEnergies,
                              waterAbsorption, nWaterEntries);
    }
    
}
