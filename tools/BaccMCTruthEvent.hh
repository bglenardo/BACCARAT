#ifndef BACCMCTRUTHEVENT_HH
#define BACCMCTRUTHEVENT_HH 1

////////////////////////////////////////////////////////////////////////////////
/*
*  BaccMCTruthEvent.hh
*
*  This is a header file for the mctruth-event-oriented BaccMCTruth code.
*
*  Note that this file makes use of ROOT dictionaries. If you ever edit this
*  file to change the format, you'll have to re-create the dictionary using
*  the command
*
*         rootcint -f BaccMCTruthEvent_dict.cc -c -p \
*                     BaccMCTruthEvent.hh BaccMCTruthEvent_LinkDef.h
*
*  This dictionary is necessary to create the ClassDef macro that's at the end
*  of this class definition.
*
*  Kareem Kazkaz
*  kareem@llnl.gov, (925) 422-7208
*
********************************************************************************
*  Change log
*
*  20 Jan 2016 - Initial submission (Kareem)
*  13 Dec 2016 - Add charge and run number variables needed to work with
                 forthcoming MCTruth, DER, LZAP. Copied from LUXSim (Jim Dobson)
*  16 Dec 2016 - Define constructor and ClearEverything here to avoid compile 
*                undefined reference error (Jim Dobson)
*  23 Jan 2017 - BaccRootConverterEvent used as template for BaccMCTruthEvent (Thomas Davison)
*
*/
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>

#include "TObject.h"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//
//  Define the class and structure of the mctruth-event-centric format
//
//------++++++------++++++------++++++------++++++------++++++------++++++------
struct photonMCTruth {
  
  Double_t dPhotonTime_ns;
  Double_t dTimeSincePrimary_ns;
  Double_t dPhotonWavelength_nm;
  Int_t iPMTIndex;
  Int_t iInteractionIdentifier;
  Int_t iVertexNumber;
};

struct vertexMCTruth {
    std::string sVolumeName;
    Int_t iTrackID;
    Int_t iParentID;
    std::string sParticleName;
    Double_t dPosition_mm[3];
    Double_t dEnergyDep_keV;
    Double_t dTime_ns;
    
    Double_t dElectricFieldDirection[3];
    Double_t dElectricFieldStrength_Vcm;
	
    Int_t s1Quanta;
    Int_t s2Quanta;
	
};

struct pmtMCTruth {
   Int_t numPhotonHits;
};

class BaccMCTruthEvent : public TObject
{
    public:
        
        BaccMCTruthEvent(){
          ClearEverything(); 
        };
        
        Int_t iOriginalEventID;
        Int_t iOriginalEventTreeIndex;
	Int_t iRunNumber;
		
	Double_t dEventFirstPhotonTime_ns;
	Double_t dEventLastPhotonTime_ns;
		
        std::vector<photonMCTruth> pmthits;
        std::vector<vertexMCTruth> vertices;
	std::vector<pmtMCTruth> pmts;
    
        
        void ClearEverything(){
          iOriginalEventID = -1;
          iOriginalEventTreeIndex = -1;
	  iRunNumber = -1;
		  
          pmthits.clear();
          vertices.clear();
	  pmts.clear();
        };
        
    private:
    
ClassDef( BaccMCTruthEvent, 3 )

};

#endif

