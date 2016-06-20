#ifndef _line_estimator_
#define _line_estimator_

#include "Hit.h"
#include "Track.h"

TrackState lineSegmentPredictor(const MeasurementState& hit0, const MeasurementState& hit1);

#endif
