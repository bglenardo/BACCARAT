#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TRandom3.h"
#include "TH1.h"
#include "TGraphErrors.h"

#include <vector>
#include <string>
#include <map>
//#include "BaccRootConverterEvent.hh"

class BaccRootConverterEvent;

class KTonSimsAnalysisEnvironment 
{
  public:
      KTonSimsAnalysisEnvironment();
      ~KTonSimsAnalysisEnvironment();  

      int LoadInputRootFile( std::string fname );
      int LoadInputChain( std::string dir );
      int AddInputChain( std::string dir );

      int PrintTree();
      int PrintCurrentEvtDetails();
      int FindVolumeID( std::string vname );
      int PrintOutEnergyDepInfo();

      int SetOutputDir( std::string outDir_temp );
      void SetCurrentEvt(int evt_num);

      int ReduceSimulatedData(std::string outfilename);

      TTree * GetEventsChainPtr() { return eventsChain; };

  private:
      TFile * inputFile;
      TFile * reducedEventsFile;

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

      TRandom3 r;

      std::map<int,int> ch_map;

      int chainsAdded;
      int totalEvents;

      std::vector<TChain*> eventsChainVec;
      std::vector<int> eventsChainEntriesVec;
   

};
