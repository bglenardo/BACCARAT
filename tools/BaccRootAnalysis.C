/*

 BaccRootAnalysis.C
 Kareem Kazkaz
 kareem@llnl.gov, (925) 422-7208
 
 This is an analysis file that is meant to work on data created by the
 LZQuickCheckNoVis.mac macro file. Once BACCARAT is built, start up ROOT and
 type in
 
    .L tools/libBaccRootConverterEvent.so
    .L BaccRootAnalysis.C+
    BaccRootAnalysis( <filename> )
 
*/

#include <string>
#include <iostream>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TCanvas.h"

#include "BaccRootConverterEvent.hh"

using namespace std;

void BaccRootAnalysis( string inputFile )
{
    //  Open up the file and set up variables to access the data
    TFile *f = new TFile( inputFile.c_str() );
    TTree *data = (TTree*)f->FindObjectAny( "DataTree" );
    TBranch *eventBranch = data->GetBranch( "Event" );
    
    BaccRootConverterEvent *anEvent = new BaccRootConverterEvent();
    eventBranch->SetAddress( &anEvent );
    
    Int_t numEvents = data->GetEntries();
    cout << "Processing " << numEvents << " events..." << endl;

    //  Set up variables for storage and analysis
    vector<Int_t> photonVec(numEvents);
    vector<Int_t> elecVec(numEvents);
    
    //  Loop over all the entries. Note that 1 ROOT entry is isomorphic with
    //  one Geant4-like event
    for( Int_t i=0; i<numEvents; i++ ) {
        
        eventBranch->GetEntry(i);
        cout << "Event " << i+1 << ":" << endl;
        
        for( Int_t j=0; j<(Int_t)anEvent->primaryParticles.size(); j++ )
            cout << "\tPrimary particle " << j+1 << " = "
                 << anEvent->primaryParticles[j].sName << endl;
        
        for( Int_t j=0; j<(Int_t)anEvent->volumes.size(); j++ ) {
            cout << "\tVolume " << anEvent->volumes[j].sName
                 << ", photons = " << anEvent->volumes[j].iTotalOptPhotNumber
                 << ", electrons = " << anEvent->volumes[j].iTotalThermElecNumber
                 << endl;
            
            photonVec.push_back( anEvent->volumes[j].iTotalOptPhotNumber );
            elecVec.push_back( anEvent->volumes[j].iTotalThermElecNumber );
        }
        
        cout << endl;
    }
    f->Close();

    //  Now actually do something with the data we've recorded in the vectors
    TGraph *effS2S1 = new TGraph( (Int_t)photonVec.size(), &photonVec[0],
            &elecVec[0] );
    effS2S1->SetTitle( "Electrons vs. photons" );
    effS2S1->GetXaxis()->SetTitle( "Number of photons" );
    effS2S1->GetYaxis()->SetTitle( "Number of electrons" );
    effS2S1->GetYaxis()->SetTitleOffset( 1.3 );
    effS2S1->SetMarkerStyle(4);
    effS2S1->SetMarkerSize(0.4);
    effS2S1->SetMarkerColor(2);
    
    TCanvas *effS2S1Can = new TCanvas( "effS2S1Can", "effS2S1Can" );
    effS2S1Can->cd(0);
    effS2S1->Draw( "AP" );
    effS2S1Can->SaveAs( "EffS2S1.pdf" );
    effS2S1Can->SaveAs( "EffS2S1.png" );
    
}
