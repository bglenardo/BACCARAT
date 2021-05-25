#ifndef BACCROOTCONVERTEREVENT_HH
#define BACCROOTCONVERTEREVENT_HH 1
 
////////////////////////////////////////////////////////////////////////////////
/*
*  BaccRootConverterEvent.hh
*
*  This is a header file for the event-oriented BaccRootConverter code.
*
*  Note that this file makes use of ROOT dictionaries. If you ever edit this
*  file to change the format, you'll have to re-create the dictionary using
*  the command
*
*         rootcint -f BaccRootConverterEvent_dict.cc -c -p \
*                     BaccRootConverterEvent.hh BaccRootConverterEvent_LinkDef.h
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
*
*/
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>

#include "TObject.h"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//
//  Define the class and structure of the event-centric format
//
//------++++++------++++++------++++++------++++++------++++++------++++++------
struct volumeInfo {
    std::string sName;
    Int_t iVolumeID;
    Double_t dTotalEnergyDep_keV;
    Int_t iTotalOptPhotNumber;
    Int_t iTotalThermElecNumber;
};

struct primaryParticleInfo {
    std::string sName;
    Double_t dPosition_mm[3];
    Double_t dDirection[3];
    Double_t dEnergy_keV;
    Double_t dTime_ns;
    std::string sVolumeName;
    Int_t iVolumeID;
    Int_t iAbsorptionVolumeID;
    Double_t dAbsorptionPos_mm[3];
};

struct stepInfo {
    Int_t iStepNumber;
    std::string sProcess;
    std::string sVolumeName;
    Int_t iVolumeID;
    Double_t dPosition_mm[3];
    Double_t dDirection[3];
    Double_t dTime_ns;
    Double_t dParticleEnergy_keV;
    Double_t dEnergyDep_keV;
};

struct trackInfo {
    std::string sParticleName;
    long iParticleID;
    Int_t iTrackID;
    Int_t iParentID;
    Int_t iCreationVolumeID;
    Double_t dCreationPosition[3];
    std::string sCreatorProcess;
    std::vector<stepInfo> steps;
    Double_t dWavelength_nm;
    Double_t dCharge;
};

class BaccRootConverterEvent : public TObject
{
    public:
        
        BaccRootConverterEvent(){
          ClearEverything(); 
        };
        
        Int_t iEventNumber;
        Int_t iRunNumber;
        
        std::vector<volumeInfo> volumes;
        std::vector<primaryParticleInfo> primaryParticles;
        std::vector<trackInfo> tracks;
        
        void ClearEverything(){
          iEventNumber = -1;
          iRunNumber = -1;
          primaryParticles.clear();
          tracks.clear();
          volumes.clear();
        };
        
    private:
    
ClassDef( BaccRootConverterEvent, 6 )

};

#endif

