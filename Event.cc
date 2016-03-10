#include "Event.h"
#include "Simulation.h"
#include "Propagation.h"

Event::Event(int id){
  evtID_ = id;
  mcTracks_.clear();
  recoTracks_.clear();
}

void Event::Simulate(){
  for (int i = 0; i < Config::nTracks; i++){
    TrackStateVec       mcTruthTSVec;
    MeasurementStateVec hitVec;
    setupTrackByToyMC(mcTruthTSVec,hitVec);
    Track mcTrack(mcTruthTSVec,hitVec);
    mcTracks_.push_back(mcTrack);
  }  
}

void Event::Fit(){}
void Event::Validate(){}
