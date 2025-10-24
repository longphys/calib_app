#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1.h"

#include "TStopwatch.h"

void refill_mbs()
{
  auto timer = new TStopwatch();
  timer->Start();

  //! Measurement directory
  TFile* fileGet = new TFile("~/data/neu_calib/ch0_9421_HV1950_ch1_9422_HV1800_ch2_9849_HV2000_ch3_9854_HV200000001.root", "read");
  TTree* treeGet = (TTree*) fileGet->Get("AnalysisxTree");

  UShort_t neutAmp[48];

  treeGet->SetBranchAddress("NeEvent.neutAmp[48]", &neutAmp);

  //! Files and trees
  TFile* fileOpen = new TFile("./build/neu_calib_0.root", "recreate");
  TTree* treeOpen = new TTree("Events", "Events");

  //! Set Branch
	double amp;
  treeOpen->Branch("Amplitude", &amp);
  
  //! Fill experiment histograms

  for (int i = 0; i < treeGet->GetEntries(); i++)
  {
    if(i%10000==0)std::cout << "entry no. " << i << "\n";
    treeGet->GetEntry(i);
    amp = neutAmp[0];
    if(amp > 100.)treeOpen->Fill();
  }

  treeOpen->Write();
  fileOpen->Close();

  std::cout << "time: " << timer->RealTime() << " seconds \n";
}