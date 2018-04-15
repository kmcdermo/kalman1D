#include "../interface/ROOTValidation.hh"
#include "../interface/Config.hh"

Validation::Validation(TString fileName) 
{
  // define output file
  f_ = new TFile(fileName.Data(),"RECREATE");
  
  initializeTree();
  initializeConfigTree();
}

void Validation::initializeTree()
{
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
  tree_->Branch("x_filter", &x_filter_);
  tree_->Branch("vx_filter",&vx_filter_);
  tree_->Branch("x_smooth", &x_smooth_);
  tree_->Branch("vx_smooth",&vx_smooth_);
  tree_->Branch("x_hit",&x_hit_);

  // errors
  tree_->Branch("exx_filter",  &exx_filter_);
  tree_->Branch("evxvx_filter",&evxvx_filter_);
  tree_->Branch("exvx_filter", &exvx_filter_);
  tree_->Branch("evxx_filter", &evxx_filter_);
  tree_->Branch("exx_smooth",  &exx_smooth_);
  tree_->Branch("evxvx_smooth",&evxvx_smooth_);
  tree_->Branch("exvx_smooth", &exvx_smooth_);
  tree_->Branch("evxx_smooth", &evxx_smooth_);
  tree_->Branch("exx_hit", &exx_hit_);

  //chi2
  tree_->Branch("chi2", &chi2_);
}

void Validation::initializeConfigTree()
{
  // define config tree;
  configtree_ = new TTree("configtree","configtree");
  // define branches
  configtree_->Branch("startpos",&startpos_);
  configtree_->Branch("startvel",&startvel_);
  configtree_->Branch("nHits",&nHits_);
  configtree_->Branch("processNoisePos",&processNoisePos_);
  configtree_->Branch("processNoiseVel",&processNoiseVel_);
  configtree_->Branch("measNoisePos",&measNoisePos_);
  configtree_->Branch("processNoisePosSF",&processNoisePosSF_);
  configtree_->Branch("processNoiseVelSF",&processNoiseVelSF_);
  configtree_->Branch("measNoisePosSF",&measNoisePosSF_);
  configtree_->Branch("nEvents",&nEvents_);
  configtree_->Branch("nTracks",&nTracks_);
  configtree_->Branch("deltaT",&deltaT_);
  configtree_->Branch("useLineEst",&useLineEst_);
  configtree_->Branch("diff_ticks",&diff_ticks_);
  configtree_->Branch("useSmoother",&useSmoother_);
}

void Validation::fillTree(int evtID, const TrackVec& evt_mc_tracks, const TrackVec& evt_filtered_tracks, const TrackVec& evt_smoothed_tracks)
{
  // loop over tracks, then hits, fill once per "layer", same ntracks for mc and reco
  for (int itrack = 0; itrack < Config::nTracks; itrack++)
  { 
    evtID_   = evtID;
    trackID_ = itrack;

    // store additional "starting info" about the mc and reco track pair, 
    // label the the layer as -1, hit values will be -1.0.
    // fill immediately after setting these values
    // smoothed value is the last one, best estimate of production point

    const auto& mcTSinit     = evt_mc_tracks[itrack].getTSinit();
    const auto& filterTSinit = evt_filtered_tracks[itrack].getTSinit();
    const auto& smoothTSinit = evt_smoothed_tracks[itrack].getTSinit();

    // unfilled values at guess
    layer_ = -1; 
    x_hit_   = -1.0;
    exx_hit_ = -1.0;
    chi2_    = -1.0;

    // parameters of guess / vertex
    x_mc_  = mcTSinit.x();
    vx_mc_ = mcTSinit.vx();
    x_filter_  = filterTSinit.x();
    vx_filter_ = filterTSinit.vx();
    x_smooth_  = smoothTSinit.x();
    vx_smooth_ = smoothTSinit.vx();
    
    // errors of guess / vertex
    exx_filter_   = filterTSinit.exx();
    evxvx_filter_ = filterTSinit.evxvx();
    exvx_filter_  = filterTSinit.exvx();
    evxx_filter_  = filterTSinit.evxx();
    exx_smooth_   = smoothTSinit.exx();
    evxvx_smooth_ = smoothTSinit.evxvx();
    exvx_smooth_  = smoothTSinit.exvx();
    evxx_smooth_  = smoothTSinit.evxx();

    // fill tree for the guess / vertex
    tree_->Fill();
    
    // store the mc prop values and reco filter/smooth values and measurement info
    const auto& mcTSVec   = evt_mc_tracks[itrack].getTSVec(); 
    const auto& hitVec    = evt_mc_tracks[itrack].getMSVec(); // stored in MC tracks
    const auto& filterTSVec = evt_filtered_tracks[itrack].getTSVec(); 
    const auto& filterchi2  = evt_filtered_tracks[itrack].getChi2Vec();
    const auto& smoothTSVec = evt_smoothed_tracks[itrack].getTSVec(); 

    // loop over layers forwards
    // use dummy counter to tally smoother counter which goes in the other direction
    // fill tree over per layer
    int ihit_rev = Config::nHits-1;
    for (int ihit = 0; ihit < Config::nHits; ihit++, ihit_rev--)
    {
      layer_ = ihit; // one hit per layer
      
      // parameters
      x_mc_  = mcTSVec[ihit].x();
      vx_mc_ = mcTSVec[ihit].vx();
      x_filter_  = filterTSVec[ihit].x();
      vx_filter_ = filterTSVec[ihit].vx();
      x_smooth_  = smoothTSVec[ihit_rev].x();
      vx_smooth_ = smoothTSVec[ihit_rev].vx();
      x_hit_ = hitVec[ihit].x();

      // errors
      exx_filter_   = filterTSVec[ihit].exx();
      evxvx_filter_ = filterTSVec[ihit].evxvx();
      exvx_filter_  = filterTSVec[ihit].exvx();
      evxx_filter_  = filterTSVec[ihit].evxx();
      exx_smooth_   = smoothTSVec[ihit_rev].exx();
      evxvx_smooth_ = smoothTSVec[ihit_rev].evxvx();
      exvx_smooth_  = smoothTSVec[ihit_rev].exvx();
      evxx_smooth_  = smoothTSVec[ihit_rev].evxx();
      exx_hit_  = hitVec[ihit].exx();

      // filtered chi2
      chi2_ = filterchi2[ihit];

      // fill tree once per hit per track pair
      tree_->Fill();
    }
  } //end loop over nTracks
}

void Validation::fillConfigTree()
{
  // store all config values in a seperate tree 
  startpos_          = Config::startpos;
  startvel_          = Config::startvel;
  nHits_             = Config::nHits;
  processNoisePos_   = Config::processNoisePos;
  processNoiseVel_   = Config::processNoiseVel;
  measNoisePos_      = Config::measNoisePos;
  processNoisePosSF_ = Config::processNoisePosSF;
  processNoiseVelSF_ = Config::processNoiseVelSF;
  measNoisePosSF_    = Config::measNoisePosSF;
  nEvents_           = Config::nEvents;
  nTracks_           = Config::nTracks;
  deltaT_            = Config::deltaT;
  useLineEst_        = Config::useLineEst;
  diff_ticks_        = Config::diff_ticks;
  useSmoother_       = Config::useSmoother;
  
  // fill at end of main()
  configtree_->Fill();
}

void Validation::saveValidation()
{
  f_->cd();
  f_->Write();
  f_->Close();
}
