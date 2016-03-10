#include "ROOTValidation.h"
#include "Config.h"

Validation::Validation(TString fileName) {
  // define output file
  f_ = new TFile(fileName.Data(),"RECREATE");
  
  // define output tree
  tree_ = new TTree("tree","tree");

  // define branches
  
  // bookkeeping
  tree_->Branch("evtID",&evtID_);
  tree_->Branch("trackID",&trackID_);
  tree_->Branch("layer",&layer_);

  // values for each layer for a given mc-reco track pair
  // parameters
  tree_->Branch("x_mc", &x_mc_);
  tree_->Branch("vx_mc",&vx_mc_);
  tree_->Branch("x_up", &x_up_);
  tree_->Branch("vx_up",&vx_up_);
  tree_->Branch("x_hit",&x_hit_);

  // errors
  tree_->Branch("exx_up",  &exx_up_);
  tree_->Branch("evxvx_up",&evxvx_up_);
  tree_->Branch("exvx_up", &exvx_up_);
  tree_->Branch("exvx_up", &exvx_up_);
  tree_->Branch("exx_hit", &exx_hit_);

  //chi2
  tree_->Branch("chi2", &chi2_);
}

void Validation::fillTree(int evtID, const TrackVec& evt_mc_tracks, const TrackVec& evt_reco_tracks){
  // loop over tracks, then hits, fill once per hit
  for (int itrack = 0; itrack < Config::nTracks; itrack++){ // same ntracks for mc and reco
    evtID_   = evtID;
    trackID_ = itrack;
    
    const auto& mcTSVec   = evt_mc_tracks[itrack].getTSVec(); 
    const auto& hitVec    = evt_mc_tracks[itrack].getMSVec(); // stored in MC tracks
    const auto& recoTSVec = evt_reco_tracks[itrack].getTSVec(); 

    for (int ihit = 0; ihit < Config::nHits; ihit++){
      layer_ = ihit; // one hit per layer
      
      // parameters
      x_mc_  = mcTSVec[ihit].x();
      vx_mc_ = mcTSVec[ihit].vx();
      x_up_  = recoTSVec[ihit].x();
      vx_up_ = recoTSVec[ihit].vx();
      x_hit_ = hitVec[ihit].x();

      // errors
      exx_up_   = recoTSVec[ihit].exx();
      evxvx_up_ = recoTSVec[ihit].evxvx();
      exvx_up_  = recoTSVec[ihit].exvx();
      evxx_up_  = recoTSVec[ihit].evxx();
      exx_hit_  = hitVec[ihit].exx();

      // fill tree once per hit per track pair
      tree_->Fill();
    }
  }
}

void Validation::save() {
  f_->cd();
  f_->Write();
  f_->Close();
}
