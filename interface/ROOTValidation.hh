#ifndef _rootvalidation_
#define _rootvalidation_

#include "TString.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include "Track.hh"
#include "Hit.hh"

class ROOTValidation
{
public:
  ROOTValidation(TString fileName);
  ~ROOTValidation();

  void initializeTree();
  void initializeConfigTree();
  void fillTree(int evtID, const TrackVec& evt_mc_tracks, 
		const TrackVec& evt_filtered_tracks, const TrackVec& evt_smoothed_tracks);
  void fillConfigTree();
  void saveValidation();

private:
  TFile * f_;

  // track tree
  TTree * tree_;

  int evtID_;
  int trackID_; // mc == reco id
  int layer_;

  float x_mc_, vx_mc_, x_filter_, vx_filter_, x_smooth_, vx_smooth_;
  float exx_filter_, evxvx_filter_, exvx_filter_, evxx_filter_;
  float exx_smooth_, evxvx_smooth_, exvx_smooth_, evxx_smooth_;
  float x_hit_, exx_hit_; 
  float chi2_; 

  // configuration tree
  TTree * configtree_;

  float startpos_;
  float startvel_;
  int   nHits_;
  float processNoisePos_;
  float processNoiseVel_;
  float measNoisePos_;
  float processNoisePosSF_;
  float processNoiseVelSF_;
  float measNoisePosSF_;
  int   nEvents_;
  int   nTracks_;
  float deltaT_;
  bool  useLineEst_;
  int   diff_ticks_;
  bool  useSmoother_;
};

#endif 
