#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include <vector>
#include <string>
//#include "BaccRootConverterEvent.hh"

class BaccRootConverterEvent;

class Xe127SimsAnalysisEnvironment 
{
  public:
      Xe127SimsAnalysisEnvironment();
      ~Xe127SimsAnalysisEnvironment();  

      int LoadInputRootFile( std::string fname );
      int LoadInputChain( std::string dir );

      int PrintOutEnergyDepInfo(int event_num);
      int PrimaryNeutronScatteringVolumes();
      int DirectionOfPrimariesForScatteringEvents();
      int PrintTree();
      int PrintCurrentEvtDetails();
      int FindVolumeID( std::string vname );

      void SetOutputDir( std::string outDir_temp );

      int ReduceSimulatedData(string outfileName);

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
