#include "PlotValidation.hh"

#include <TH1.h>
#include <TGraphErrors.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TLegend.h>

#include <iostream>
#include <iomanip>

PlotValidation::PlotValidation(TString filename, TString outdir, Bool_t mvinput) : fChain(0), inFileName(filename), outDir(outdir), mvInput(mvinput) 
{
  file = TFile::Open(inFileName.Data());

  // make output directory if it does not exist
  FileStat_t dummyFileStat;
  if (gSystem->GetPathInfo(outDir.Data(), dummyFileStat) == 1)
  {
    TString mkDir = "mkdir -p ";
    mkDir += outDir.Data();
    gSystem->Exec(mkDir.Data());
  }

  TTree * tree = (TTree*)file->Get("tree");
  Init(tree);
}

PlotValidation::~PlotValidation()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t PlotValidation::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

Long64_t PlotValidation::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
  }
  return centry;
}

void PlotValidation::Init(TTree *tree)
{
  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("evtID", &evtID, &b_evtID);
  fChain->SetBranchAddress("trackID", &trackID, &b_trackID);
  fChain->SetBranchAddress("layer", &layer, &b_layer);
  fChain->SetBranchAddress("x_mc", &x_mc, &b_x_mc);
  fChain->SetBranchAddress("vx_mc", &vx_mc, &b_vx_mc);
  fChain->SetBranchAddress("x_filter", &x_filter, &b_x_filter);
  fChain->SetBranchAddress("vx_filter", &vx_filter, &b_vx_filter);
  fChain->SetBranchAddress("x_smooth", &x_smooth, &b_x_smooth);
  fChain->SetBranchAddress("vx_smooth", &vx_smooth, &b_vx_smooth);
  fChain->SetBranchAddress("x_hit", &x_hit, &b_x_hit);
  fChain->SetBranchAddress("exx_filter", &exx_filter, &b_exx_filter);
  fChain->SetBranchAddress("evxvx_filter", &evxvx_filter, &b_evxvx_filter);
  fChain->SetBranchAddress("exvx_filter", &exvx_filter, &b_exvx_filter);
  fChain->SetBranchAddress("evxx_filter", &evxx_filter, &b_evxx_filter);
  fChain->SetBranchAddress("exx_smooth", &exx_smooth, &b_exx_smooth);
  fChain->SetBranchAddress("evxvx_smooth", &evxvx_smooth, &b_evxvx_smooth);
  fChain->SetBranchAddress("exvx_smooth", &exvx_smooth, &b_exvx_smooth);
  fChain->SetBranchAddress("evxx_smooth", &evxx_smooth, &b_evxx_smooth);
  fChain->SetBranchAddress("exx_hit", &exx_hit, &b_exx_hit);
  fChain->SetBranchAddress("chi2", &chi2, &b_chi2);
}

void PlotValidation::Validation()
{
  // set style of plots
  gStyle->SetOptStat("emou"); // set style
  gStyle->SetOptFit(1011); // set fit style

  // quick grab of configtree 
  TTree * configtree = (TTree*)file->Get("configtree");
  Float_t deltaT = 0.;
  Int_t   nHits  = 0.;
  configtree->SetBranchAddress("deltaT",&deltaT);
  configtree->SetBranchAddress("nHits",&nHits);
  configtree->GetEntry(0);

  // declare and initialize pulls
  TH1F * x_pull_filter  = new TH1F("x_pull_filter",Form("Filtered Position Pull (Layer %i only)",nHits-1),100,-5,5);
  x_pull_filter->GetXaxis()->SetTitle("x_{mc} - x_{filtered} / #sigma(x_{filtered})");
  x_pull_filter->GetYaxis()->SetTitle("nTracks");
  x_pull_filter->Sumw2();
  TH1F * vx_pull_filter = new TH1F("vx_pull_filter",Form("Filtered Velocity Pull (Layer %i only)",nHits-1),100,-5,5);
  vx_pull_filter->GetXaxis()->SetTitle("vx_{mc} - vx_{filtered} / #sigma(vx_{filtered})");  
  vx_pull_filter->GetYaxis()->SetTitle("nTracks");
  vx_pull_filter->Sumw2();
  TH1F * x_pull_smooth  = new TH1F("x_pull_smooth","Smoothed Position Pull (Layer 0 only)",100,-5,5);
  x_pull_smooth->GetXaxis()->SetTitle("x_{mc} - x_{smoothed} / #sigma(x_{smoothed})");
  x_pull_smooth->GetYaxis()->SetTitle("nTracks");
  x_pull_smooth->Sumw2();
  TH1F * vx_pull_smooth = new TH1F("vx_pull_smooth","Smoothed Velocity Pull (Layer 0 only)",100,-5,5);
  vx_pull_smooth->GetXaxis()->SetTitle("vx_{mc} - vx_{smoothed} / #sigma(vx_{smoothed})");  
  vx_pull_smooth->GetYaxis()->SetTitle("nTracks");
  vx_pull_smooth->Sumw2();
  TH1F * x_factor  = new TH1F("x_factor",Form("Position Factor (Layer %i only)",nHits/2),150,0,15);
  x_factor->GetXaxis()->SetTitle("|x_{s} - x_{mc} / x_{f} - x_{mc}|");
  x_factor->GetYaxis()->SetTitle("nTracks");
  x_factor->Sumw2();
  TH1F * vx_factor = new TH1F("vx_factor",Form("Velocity Factor (Layer %i only)",nHits/2),150,0,15);
  vx_factor->GetXaxis()->SetTitle("|vx_{s} - vx_{mc} / vx_{f} - vx_{mc}|");
  vx_factor->GetYaxis()->SetTitle("nTracks");
  vx_factor->Sumw2();
  
  // follow trajectory of single track!
  TGraphErrors * tg_mctrack   = new TGraphErrors(nHits);
  tg_mctrack->SetTitle("Position vs. Time of a single track;Time [s];Position [cm]");
  tg_mctrack->SetLineColor(kBlack);
  TGraphErrors * tg_filtertrack = new TGraphErrors(nHits);
  tg_filtertrack->SetMarkerColor(kRed);
  tg_filtertrack->SetLineColor(kRed);
  TGraphErrors * tg_smoothtrack = new TGraphErrors(nHits);
  tg_smoothtrack->SetMarkerColor(kBlue);
  tg_smoothtrack->SetLineColor(kBlue);
  TGraphErrors * tg_hits      = new TGraphErrors(nHits-1);
  tg_hits->SetMarkerColor(kGreen+2);
  tg_hits->SetLineColor(kGreen+2);
  tg_hits->SetMarkerStyle(kFullDotMedium);

  // This is the loop skeleton where:
  // jentry is the global entry number in the chain
  // ientry is the entry number in the current Tree
  // Note that the argument to GetEntry must be:
  // jentry for TChain::GetEntry
  // ientry for TTree::GetEntry and TBranch::GetEntry

  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;

  // loop over entries
  for (Long64_t jentry=0; jentry<nentries;jentry++) 
  {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    
    // track one particle
    if (evtID == 0 && trackID == 0)
    {
      tg_mctrack     ->SetPoint    (layer+1,(layer+1)*deltaT,x_mc);

      tg_filtertrack->SetPoint     (layer+1,(layer+1)*deltaT,x_filter);
      tg_filtertrack->SetPointError(layer+1,0.,exx_filter);

      tg_smoothtrack->SetPoint     (layer+1,(layer+1)*deltaT,x_smooth);
      tg_smoothtrack->SetPointError(layer+1,0.,exx_smooth);

      // skip the vertex layer ... no hit there!
      if (layer >= 0) 
      { 
	tg_hits       ->SetPoint     (layer,(layer+1)*deltaT,x_hit);
	tg_hits       ->SetPointError(layer,0.,exx_hit);
      }
    }

    // only look at filtered pulls of last layer for now
    if (layer == 0) 
    {
      x_pull_smooth ->Fill((x_mc  - x_smooth)  / std::sqrt(exx_smooth));
      vx_pull_smooth->Fill((vx_mc - vx_smooth) / std::sqrt(evxvx_smooth));
    }
    else if (layer == nHits -1) 
    {
      x_pull_filter ->Fill((x_mc  - x_filter)  / std::sqrt(exx_filter));
      vx_pull_filter->Fill((vx_mc - vx_filter) / std::sqrt(evxvx_filter));
    }

    if (layer == nHits / 2)
    {
      x_factor ->Fill(std::abs((x_smooth  - x_mc )/(x_filter  - x_mc )));
      vx_factor->Fill(std::abs((vx_smooth - vx_mc)/(vx_filter - vx_mc)));
    }
  }

  // scale and fit pulls
  x_pull_filter->Scale(1.f/x_pull_filter->Integral());
  x_pull_filter ->Fit("gaus","","",-3.,3.);

  vx_pull_filter->Scale(1.f/vx_pull_filter->Integral());
  vx_pull_filter->Fit("gaus","","",-3.,3.);

  x_pull_smooth->Scale(1.f/x_pull_smooth->Integral());
  x_pull_smooth ->Fit("gaus","","",-3.,3.);

  vx_pull_smooth->Scale(1.f/vx_pull_smooth->Integral());
  vx_pull_smooth->Fit("gaus","","",-3.,3.);

  x_factor->Scale(1.f/x_factor->Integral());
  vx_factor->Scale(1.f/vx_factor->Integral());

  std::cout << std::endl << std::endl;
  
  double integral = 0.;
  for (Int_t ibin = 1; ibin <= x_factor->GetNbinsX(); ibin++)
  {
    if (x_factor->GetXaxis()->GetBinUpEdge(ibin) >= 1.f) break;
    integral += x_factor->GetBinContent(ibin);
  }
  std::cout << "Position: percent smoothed better than filtered: " 
	    << std::setprecision(3) << integral*100. << "%" << std::endl;

  integral = 0.;
  for (Int_t ibin = 1; ibin <= vx_factor->GetNbinsX(); ibin++)
  {
    if (vx_factor->GetXaxis()->GetBinUpEdge(ibin) >= 1.f) break;
    integral += vx_factor->GetBinContent(ibin);
  }
  std::cout << "Velocity: percent smoothed better than filtered: " 
	    << std::setprecision(3) << integral*100. << "%" << std::endl;

  std::cout << std::endl << std::endl;

  // now draw and save pulls
  TCanvas * canvas = new TCanvas();
  canvas->cd();
  
  x_pull_filter->Draw();
  canvas->SaveAs(Form("%s/x_pull_filter.png",outDir.Data()));
  
  vx_pull_filter->Draw();
  canvas->SaveAs(Form("%s/vx_pull_filter.png",outDir.Data()));

  x_pull_smooth->Draw();
  canvas->SaveAs(Form("%s/x_pull_smooth.png",outDir.Data()));
  
  vx_pull_smooth->Draw();
  canvas->SaveAs(Form("%s/vx_pull_smooth.png",outDir.Data()));
  
  x_factor->Draw();
  canvas->SaveAs(Form("%s/x_factor.png",outDir.Data()));

  vx_factor->Draw();
  canvas->SaveAs(Form("%s/vx_factor.png",outDir.Data()));

  delete canvas;
  delete x_pull_filter;
  delete vx_pull_filter;
  delete x_pull_smooth;
  delete vx_pull_smooth;
  delete x_factor;
  delete vx_factor;

  // now draw overlay of track and save it

  TCanvas * canvas_track = new TCanvas();
  canvas_track->cd();

  tg_mctrack    ->Draw("AL");
  tg_filtertrack->Draw("SAME LP");
  tg_smoothtrack->Draw("SAME LP");
  tg_hits       ->Draw("SAME P");

  TLegend * leg = new TLegend(0.7,0.7,0.9,0.9);
  leg->AddEntry(tg_mctrack,"Truth Trajectory","L");
  leg->AddEntry(tg_filtertrack,"Filtered Trajectory","ELP");
  leg->AddEntry(tg_smoothtrack,"Smoothed Trajectory","ELP");
  leg->AddEntry(tg_hits,"Measurements","ELP");
  leg->Draw("SAME");

  canvas_track->SaveAs(Form("%s/track.png",outDir.Data()));
 
  delete leg;
  delete canvas_track;
  delete tg_mctrack;
  delete tg_filtertrack;
  delete tg_smoothtrack;
  delete tg_hits;

  if (mvInput) {MoveInput();}
}

void PlotValidation::MoveInput()
{
  TString mvin = "mv ";
  mvin += inFileName.Data();
  mvin += " ";
  mvin += outDir.Data();
  gSystem->Exec(mvin.Data());
}
