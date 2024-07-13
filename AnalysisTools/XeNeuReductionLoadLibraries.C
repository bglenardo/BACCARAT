//#include "Xe127SimsAnalysisEnvironment.hh"

void XeNeuReductionLoadLibraries() {

  cout << "Inside LoadLibraries script..." << endl;
  cerr << "Inside LoadLibraries script..." << endl;
  //gSystem->Load("BACCARAT/tools/libBaccRootConverterEvent.so");
  //gSystem->Load("Xe127SimsAnalysisEnvironment_cc.so");
  gROOT->ProcessLine(".L BACCARAT/tools/BaccRootConverterEvent.cc+");
  gROOT->ProcessLine(".L XeNeuSimsAnalysisEnvironment.cc+");

}
