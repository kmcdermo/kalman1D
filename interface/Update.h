#ifndef _update_
#define _update_

#include "Matrix.h"
#include "Hit.h"
#include "Track.h"

TrackState updateTrackState(const TrackState & propState, const MeasurementState & hit);
float computeChi2(const TrackState & updatedState, const MeasurementState & hit);

#endif
