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

  int evtID() {return evtID;}
 private:
  TrackVec mcTracks, recoTracks;
  int evtID;
};

#endif 
