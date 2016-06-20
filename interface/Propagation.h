#ifndef _propagation_
#define _propagation_

#include "Matrix.h"
#include "Hit.h"
#include "Track.h"

TrackState propagateTrackToNextState(const TrackState& inputState);

#endif
