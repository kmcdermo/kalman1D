#ifndef _smoother_
#define _smoother_

#include "Track.h"

TrackState smoothState(const TrackState& inSmoothedState, const TrackState& inFilteredState);

#endif
