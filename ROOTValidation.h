#ifndef _rootvalidation_
#define _rootvalidation_

#include "TString.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include "Track.h"
#include "Hit.h"

class Validation{
 public:
  Validation(TString fileName);
  void initializeTree();
  void initializeConfigTree();
  void fillTree(int evtID, const TrackVec& evt_mc_tracks, const TrackVec& evt_reco_tracks);
  void fillConfigTree();
  void saveValidation();
 private:
  TFile * f_;

  // track tree
  TTree * tree_;

  int evtID_;
  int trackID_; // mc == reco
  int layer_;

  float x_mc_, vx_mc_, x_up_, vx_up_, x_hit_, chi2_;
  float exx_up_, evxvx_up_, exvx_up_, evxx_up_, exx_hit_;

  // configuration tree
  TTree * configtree_;

  float startpos_;
  float startvel_;
  int   nHits_;
  float processNoisePos_;
  float processNoiseVel_;
  float measNoisePos_;
  int   nEvents_;
  int   nTracks_;
  float deltaT_;
};

#endif 
