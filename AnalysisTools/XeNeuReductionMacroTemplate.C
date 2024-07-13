void XeNeuReductionMacroTemplate(string input_file, string output_file){

//gROOT->ProcessLine(".x XeNeuReductionLoadLibraries.C");
//gROOT->ProcessLine(".L /g/g20/lenardo1/Simulations/BACCARAT/tools/BaccRootConverterEvent_cc.so");
//gROOT->ProcessLine(".L XeNeuSimsAnalysisEnvironment_cc.so"); 
//gSystem->AddIncludePath(" -I$HOME/Simulations");
//gSystem->AddIncludePath(" -I$HOME/Simulations/BACCARAT/tools");
//gROOT->ProcessLine(".L /g/g20/lenardo1/Simulations/BACCARAT/tools/BaccRootConverterEvent_cc.so");
//gROOT->ProcessLine(".L XeNeuSimsAnalysisEnvironment_cc.so"); 


XeNeuSimsAnalysisEnvironment* x = new XeNeuSimsAnalysisEnvironment();

x->LoadInputChain(input_file.c_str());
x->ReduceSimulatedData(output_file.c_str());


}
