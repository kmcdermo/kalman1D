#ifndef _propagation_
#define _propagation_

#include "Matrix.hh"
#include "Hit.hh"
#include "Track.hh"

TrackState propagateTrackToNextState(const TrackState& inputState);

#endif
