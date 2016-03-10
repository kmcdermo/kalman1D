#ifndef _event_
#define _event_

#include "Hit.h"
#include "Track.h"

class Event{

 public:
  Event(int id);
  void Simulate();
  void Fit();
  void Validate();

  int evtID() const {return evtID_;}
 private:
  TrackVec mcTracks_, recoTracks_;
  int evtID_;
};

#endif 
