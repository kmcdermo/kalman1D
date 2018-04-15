#ifndef _update_
#define _update_

#include "Matrix.hh"
#include "Hit.hh"
#include "Track.hh"

TrackState updateTrackState(const TrackState & propState, const MeasurementState & hit);
float computeChi2(const TrackState & updatedState, const MeasurementState & hit);

#endif
