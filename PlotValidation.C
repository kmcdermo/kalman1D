#define PlotValidation_cxx
#include "PlotValidation.hh"
#include <TH1.h>
#include <TGraph.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void PlotValidation::Loop()
{
  gStyle->SetOptStat("emou"); // set style

  // declare and initialize pulls
  TH1F * x_pull  = new TH1F("x_pull","Position Pull (Layer 20 only)",100,-5,5);
  x_pull->GetXaxis()->SetTitle("x_{mc} - x_{update} / #sigma(x_{update})");
  x_pull->GetYaxis()->SetTitle("nTracks");
  TH1F * vx_pull = new TH1F("vx_pull","Velocity Pull (Layer 20 only)",100,-5,5);
  vx_pull->GetXaxis()->SetTitle("vx_{mc} - vx_{update} / #sigma(vx_{update})");  
  vx_pull->GetYaxis()->SetTitle("nTracks");

  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;
  // loop over entries
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    
    // actually do the physics
    if (layer != 19) continue; // only look at pulls of last layer for now
     x_pull.Fill( (x_mc  - x_up)  / sqrtf(exx_up));
     vx_pull.Fill((vx_mc - vx_up) / sqrtf(evxvx_up));
  }

  // fit pulls
  x_pull ->Fit("gaus","","",-3.,3.);
  vx_pull->Fit("gaus","","",-3.,3.);

  // now draw and save pulls
  TCanvas * canvas = new TCanvas();
  canvas->cd();
  
  x_pull->Draw();
  canvas->SaveAs("x_pull.png");
  
  vx_pull->Draw();
  canvas->SaveAs("vx_pull.png");

}

