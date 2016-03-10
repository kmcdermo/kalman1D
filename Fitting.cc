#include "Fitting.h"
#include "Matrix.h"
#include "Hit.h"
#include "Propagation.h"
#include "Update.h"

void trackFit(const TrackVec& evt_mc_tracks, TrackVec& evt_reco_tracks) {

  for (auto&& mctrack : evt_mc_tracks){ // loop over tracks in event
    // declare recotrack
    Track recotrack;

    // get measurements + initial "guess" --> here we will just MC truth
    const auto& hits  = mctrack.getMSVec();
    TrackState tempState = mctrack.getTSgen();
    
    for (auto&& hit : hits) {
      // first predict (first step will be equivalent to simulation)
      const TrackState propState    = propagateTrackToNextState(tempState);

      // then do the update with measurement
      const TrackState updatedState = updateTrackState(propState,hit);
      float chi2 = computeChi2(updatedState,hit);
      
      tempState = updatedState; // now the input for next layer will be filtered state!
      
      // now give the track all the lovely stuff
      recotrack.addTS(updatedState);
      recotrack.addMS(hit); // extra copy of hits
      recotrack.addChi2(chi2);
    } // end loop over hits

    evt_reco_tracks.push_back(recotrack);
  } // end loop over tracks
}
