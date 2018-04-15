#include "../interface/Event.hh"
#include "../interface/Simulation.hh"
#include "../interface/Fitting.hh"
#include "../interface/Config.hh"

Event::Event(int id, Validation& v) : evtID_(id), val_(v) 
{
  mcTracks_.clear();
  filteredTracks_.clear();
  smoothedTracks_.clear();
}

void Event::Simulate()
{
  for (int itrack = 0; itrack < Config::nTracks; itrack++)
  {
    TrackState          mcgen;
    TrackStateVec       mcTruthTSVec;
    MeasurementStateVec hitVec;
    setupTrackByToyMC(mcgen,mcTruthTSVec,hitVec);
    Track mcTrack(mcgen,mcTruthTSVec,hitVec);
    mcTracks_.push_back(mcTrack);
  } 
}

void Event::Fit()
{
  trackFitter(mcTracks_,filteredTracks_,smoothedTracks_);
}

void Event::Validate()
{
  val_.fillTree(evtID_,mcTracks_,filteredTracks_,smoothedTracks_);
}
