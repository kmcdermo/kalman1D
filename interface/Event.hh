#ifndef _event_
#define _event_

#include "Hit.hh"
#include "Track.hh"
#include "ROOTValidation.hh"

class Event
{
 public:
  Event(int id, Validation& v);
  void Simulate();
  void Fit();
  void Validate();

  int evtID() const {return evtID_;}
 private:
  TrackVec mcTracks_, filteredTracks_, smoothedTracks_;
  int evtID_;
  Validation& val_;
};

#endif 
