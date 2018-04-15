#ifndef _smoother_
#define _smoother_

#include "Track.hh"

TrackState smoothState(const TrackState& inSmoothedState, const TrackState& inFilteredState);

#endif
