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

class XeNeuSimsAnalysisEnvironment 
{
  public:
      XeNeuSimsAnalysisEnvironment();
      ~XeNeuSimsAnalysisEnvironment();  

      int LoadInputRootFile( std::string fname );
      int LoadInputChain( std::string dir );
      int AddInputChain( std::string dir );

      int PrintOutEnergyDepInfo();
      int PrimaryNeutronScatteringVolumes();
      int DirectionOfPrimariesForScatteringEvents();
      int PrintTree();
      int PrintCurrentEvtDetails();
      int FindVolumeID( std::string vname );

      int SetOutputDir( std::string outDir_temp );
      void SetCurrentEvt(int evt_num);
      void SetELifetime( double elifetime_ ){ elifetime = elifetime_; };
      void SetEEE( double eee_ ){ eee = eee_; };
      void SetDriftVelocity( double drift_speed_ ) { drift_speed = drift_speed_; };
      void SetSEWidth( double se_width_ ) { se_width = se_width_; };
      void SetSESize( double se_size_ ) { se_size = se_size_; }; 
      void SetFitMin( double fit_min_ ) { fit_min = fit_min_; };
      void SetFitMax( double fit_max_ ) { fit_max = fit_max_; };
      void SetScaleMean( double scaleMean_ ) { scaleMean = scaleMean_; };
      void SetScaleSig( double scaleSig_ ) { scaleSig = scaleSig_; };


      void SetDoGaussianFit( bool doGaussianFit_ ) { doGaussianFit = doGaussianFit_; };
      void UseFiniteMCStats( bool finiteMCStatsFit_ ) { finiteMCStatsFit = finiteMCStatsFit_; };
      void SetApplyScalePrior( bool applyScalePrior_ ) { applyScalePrior = applyScalePrior_; };

      bool GetDoGaussianFit() { return doGaussianFit; };
      bool GetApplyScalePrior() { return applyScalePrior; };

      int ReduceSimulatedData(std::string outfilename);
      int ReduceSimulatedDataByFile();

      int AddReducedEventsFile( std::string reducedFileName );
      void GenerateYieldDist( double inputYield, double width, int ch );
      void GenerateDataFromSim( double inputYield, double width, double scale, int ch );
      int AddDataHistFile( std::string dataHistFileName );
      TH1F * GetYieldDist();      
      TH1F * GetSimBGDist();      
      TH1F * GetSimSigDist();
      TH1F * GetSimDataDist();      
      TH1F * GetDataHist();
      TH1F * GetDataHistByChannel( int ch_num );
      TH1F * GetDataHistByIdx( int ch_idx );
      int AddDataHistFiles( int dataset_id );

      TGraphErrors* DrawAndPrintPlots( std::string outfilename, int ch_num, double scale_factor );
      double ComputeNegativeLogLikelihood( double scale, double yield, double width, int ch_num );
      double ComputeLikelihoodLambda( double scale, double yield, double width, int ch_num);
      double ComputeSimLikelihoodLambda( double scale, double yield, double width, int ch_num);
//      void PrintEventInfo();
      bool IsSingleScatter(); 

      TTree * GetEventsTreePtr() { return eventsTree; };
      TTree * GetEventsChainPtr() { return eventsChain; };
      TTree * GetHeaderTreePtr() { return headerTree; };

      int GetIndFromChannelNum( int ch_num );
      void ComputeTOFCuts();
      void PrintTOFCuts();
      int CheckForReducedEvents();

      double ComputeBGYield( double energy );

  private:
      TFile * inputFile;
      TFile * reducedEventsFile;
      TFile * dataHistFile;

      TTree * eventsTree;
      TTree * headerTree;
      TChain * eventsChain;

      TTree * reducedEventsTree[10];
      TH1F * yieldDist;
      TH1F * simBGDist;
      TH1F * simSigDist;
      TH1F * simDataDist;
      TH1F * energyDist;
      TH1F * dataHist;
      TH1F * tofHistograms[10];
      TH1F * dataHistograms[10];


      double tof_cut_hi[10];
      double tof_cut_lo[10];

      BaccRootConverterEvent * current_evt;

      std::vector<std::string> * volumes;

      bool IsDataLoaded();
      double ComputeDistance( double x1[3], double x2[3] );

      std::string inputFileName;
      std::string inputDirName;
      std::string outputDir;

      TRandom3 r;

      double firstXeScatterPos_x,
             firstXeScatterPos_y,
             firstXeScatterPos_z;
      int bd_ch;
      int nScattersXe;
      double dDeltaTXeLS_ns;
      double dTotEDepXe_keV;
      double dTotEDepLS_keV;
      double dFirstXeScatterEDep_keV;
      double dSecondXeScatterEDep_keV;
      bool bMSOtherMat;


      std::map<int,int> ch_map;

      bool computedTOFCuts;

      bool finiteMCStatsFit;
      bool doGaussianFit;
      bool applyScalePrior;

      double se_width;
      double se_size;
      double drift_speed;
      double eee;
      double elifetime;
      double fit_min;
      double fit_max;      

      // Parameters for the scale prior
      double scaleMean;
      double scaleSig;

      int chainsAdded;
      int totalEvents;

      std::vector<TChain*> eventsChainVec;
      std::vector<int> eventsChainEntriesVec;
   

};
