#ifndef _fitting_
#define _fitting_

#include "Track.h"

void trackFitter(const TrackVec& evt_mc_tracks, TrackVec& evt_filtered_tracks, TrackVec& evt_smoothed_tracks);
void filterTrack(const Track& mctrack, Track& filteredtrack);
void smoothTrack(const Track& mctrack, const Track& filteredtrack, Track& smoothedtrack);

#endif
