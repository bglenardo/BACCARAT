#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include <vector>
#include <string>
//#include "BaccRootConverterEvent.hh"

class BaccRootConverterEvent;

class TMSSimsAnalysisEnvironment 
{
  public:
      TMSSimsAnalysisEnvironment();
      ~TMSSimsAnalysisEnvironment();  

      int LoadInputRootFile( std::string fname );
      int LoadInputChain( std::string dir );

      int PrintOutEnergyDepInfo(int event_num);
      int PrimaryNeutronScatteringVolumes();
      int DirectionOfPrimariesForScatteringEvents();
      int PrintTree();
      int PrintCurrentEvtDetails();
      int FindVolumeID( std::string vname );

      void SetOutputDir( std::string outDir_temp );
      int SetCurrentEvt( int evt ) { eventsChain->GetEntry(evt); };

      int ReduceSimulatedData(string outfileName);
      int ReduceSimulatedDataByFile();
      
      double FindMaxDistance( vector<vector<double>> positionsVector );
//      void PrintEventInfo();
      bool IsSingleScatter(); 

      TTree * GetEventsTreePtr() { return eventsTree; };
      TTree * GetEventsChainPtr() { return eventsChain; };
      TTree * GetHeaderTreePtr() { return headerTree; };


  private:
      TFile * inputFile;

      TTree * eventsTree;
      TTree * headerTree;
      TChain * eventsChain;

      BaccRootConverterEvent * current_evt;

      std::vector<std::string> * volumes;

      bool IsDataLoaded();
      double ComputeDistance( double x1[3], double x2[3] );

      std::string inputFileName;
      std::string inputDirName;
      std::string outputDir;

};
