#include "Event.h"
#include "Simulation.h"
#include "Fitting.h"

Event::Event(int id, Validation& v) : evtID_(id), val_(v) {
  mcTracks_.clear();
  recoTracks_.clear();
}

void Event::Simulate(){
  for (int i = 0; i < Config::nTracks; i++){
    TrackState          mcgen;
    TrackStateVec       mcTruthTSVec;
    MeasurementStateVec hitVec;
    setupTrackByToyMC(mcgen,mcTruthTSVec,hitVec);
    Track mcTrack(mcgen,mcTruthTSVec,hitVec);
    mcTracks_.push_back(mcTrack);
  }  
}

void Event::Fit(){
  trackFit(mcTracks_,recoTracks_);
}

void Event::Validate(){
  val_.fillTree(evtID_,mcTracks_,recoTracks_);
}
