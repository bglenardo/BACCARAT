#include "TFile.h"
#include "TTree.h"

int main() {
    TFile* file = TFile::Open("test.root", "RECREATE");
    TTree* tree = new TTree("tree", "Test Tree");
    tree->Write();
    file->Close();
    return 0;
}
