#include "PlotValidation.hh"
#include <TH1.h>
#include <TGraphErrors.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TLegend.h>

PlotValidation::PlotValidation(TString filename, TString outdir, Bool_t mvinput) : fChain(0), inFileName(filename), outDir(outdir), mvInput(mvinput) {
  file = TFile::Open(inFileName.Data());

  // make output directory if it does not exist
  FileStat_t dummyFileStat;
  if (gSystem->GetPathInfo(outDir.Data(), dummyFileStat) == 1){
    TString mkDir = "mkdir -p ";
    mkDir += outDir.Data();
    gSystem->Exec(mkDir.Data());
  }

  TTree * tree = (TTree*)file->Get("tree");
  Init(tree);
}

PlotValidation::~PlotValidation(){
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t PlotValidation::GetEntry(Long64_t entry){
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

Long64_t PlotValidation::LoadTree(Long64_t entry){
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
  }
  return centry;
}

void PlotValidation::Init(TTree *tree){
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
  fChain->SetBranchAddress("x_up", &x_up, &b_x_up);
  fChain->SetBranchAddress("vx_up", &vx_up, &b_vx_up);
  fChain->SetBranchAddress("x_hit", &x_hit, &b_x_hit);
  fChain->SetBranchAddress("exx_up", &exx_up, &b_exx_up);
  fChain->SetBranchAddress("evxvx_up", &evxvx_up, &b_evxvx_up);
  fChain->SetBranchAddress("exvx_up", &exvx_up, &b_exvx_up);
  fChain->SetBranchAddress("evxx_up", &evxx_up, &b_evxx_up);
  fChain->SetBranchAddress("exx_hit", &exx_hit, &b_exx_hit);
  fChain->SetBranchAddress("chi2", &chi2, &b_chi2);
}

void PlotValidation::Validation(){
  // quick grab of configtree 
  TTree * configtree = (TTree*)file->Get("configtree");
  Float_t deltaT = 0.;
  Int_t   nHits  = 0.;
  configtree->SetBranchAddress("deltaT",&deltaT);
  configtree->SetBranchAddress("nHits",&nHits);
  configtree->GetEntry(0);

  //     This is the loop skeleton where:
  //    jentry is the global entry number in the chain
  //    ientry is the entry number in the current Tree
  //  Note that the argument to GetEntry must be:
  //    jentry for TChain::GetEntry
  //    ientry for TTree::GetEntry and TBranch::GetEntry
  
  gStyle->SetOptStat("emou"); // set style
  gStyle->SetOptFit(1011); // set fit style

  // declare and initialize pulls
  TH1F * x_pull  = new TH1F("x_pull",Form("Filtered Position Pull (Layer %s only)",atoi(nHits-1)),100,-5,5);
  x_pull->GetXaxis()->SetTitle("x_{mc} - x_{update} / #sigma(x_{update})");
  x_pull->GetYaxis()->SetTitle("nTracks");
  TH1F * vx_pull = new TH1F("vx_pull",Form("Filtered Velocity Pull (Layer %s only)",atoi(nHits-1)),100,-5,5);
  vx_pull->GetXaxis()->SetTitle("vx_{mc} - vx_{update} / #sigma(vx_{update})");  
  vx_pull->GetYaxis()->SetTitle("nTracks");

  // follow trajectory of single track!
  TGraphErrors * tg_mctrack   = new TGraphErrors(20);
  tg_mctrack->SetTitle("Position vs. Time of a single track;Time [s];Position [cm]");
  //  tg_mctrack->SetMarkerColor(kRed);
  //  tg_mctrack->SetMarkerStyle(kFullDotMedium);
  tg_mctrack->SetLineColor(kRed);
  TGraphErrors * tg_recotrack = new TGraphErrors(20);
  tg_recotrack->SetMarkerColor(kBlue);
  tg_recotrack->SetLineColor(kBlue);
  TGraphErrors * tg_hits      = new TGraphErrors(20);
  tg_hits->SetMarkerColor(kGreen+2);
  tg_hits->SetLineColor(kGreen+2);
  tg_hits->SetMarkerStyle(kFullDotMedium);

  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;
  // loop over entries
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    
    // track one particle
    if (evtID == 0 && trackID == 0){
      tg_mctrack  ->SetPoint(layer+1,(layer+1)*deltaT,x_mc);

      tg_recotrack->SetPoint     (layer+1,(layer+1)*deltaT,x_up);
      tg_recotrack->SetPointError(layer+1,0.,exx_up);

      tg_hits     ->SetPoint     (layer+1,(layer+1)*deltaT,x_hit);
      tg_hits     ->SetPointError(layer+1,0.,exx_hit);
    }

    // only look at pulls of last layer for now
    if (layer != nHits-1) continue;
    x_pull ->Fill((x_mc  - x_up)  / std::sqrt(exx_up));
    vx_pull->Fill((vx_mc - vx_up) / std::sqrt(evxvx_up));
  }

  // fit pulls
  x_pull ->Fit("gaus","","",-3.,3.);
  vx_pull->Fit("gaus","","",-3.,3.);

  // now draw and save pulls
  TCanvas * canvas = new TCanvas();
  canvas->cd();
  
  x_pull->Draw();
  canvas->SaveAs(Form("%s/x_pull.png",outDir.Data()));
  
  vx_pull->Draw();
  canvas->SaveAs(Form("%s/vx_pull.png",outDir.Data()));

  delete canvas;
  delete x_pull;
  delete vx_pull;

  // now draw overlay of track and save it

  TCanvas * canvas_track = new TCanvas();
  canvas_track->cd();

  tg_mctrack  ->Draw("AL");
  tg_recotrack->Draw("SAME LP");
  tg_hits     ->Draw("SAME P");

  TLegend * leg = new TLegend(0.7,0.7,0.9,0.9);
  leg->AddEntry(tg_mctrack,"MC Trajectory","L");
  leg->AddEntry(tg_recotrack,"Filtered Trajectory","ELP");
  leg->AddEntry(tg_hits,"Measurements","ELP");
  leg->Draw("SAME");

  canvas_track->SaveAs(Form("%s/track.png",outDir.Data()));
 
  delete leg;
  delete canvas_track;
  delete tg_mctrack;
  delete tg_recotrack;
  delete tg_hits;

  if (mvInput) {MoveInput();}
}

void PlotValidation::MoveInput(){
  TString mvin = "mv ";
  mvin += inFileName.Data();
  mvin += " ";
  mvin += outDir.Data();
  gSystem->Exec(mvin.Data());
}
