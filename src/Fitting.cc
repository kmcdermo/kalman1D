#include "../interface/Fitting.h"
#include "../interface/Matrix.h"
#include "../interface/Track.h"
#include "../interface/Hit.h"
#include "../interface/LineEstimator.h"
#include "../interface/Propagation.h"
#include "../interface/Smoother.h"
#include "../interface/Update.h"
#include "../interface/Config.h"
#include "TString.h"

void trackFitter(const TrackVec& evt_mc_tracks, TrackVec& evt_filtered_tracks, TrackVec& evt_smoothed_tracks) {
  for (auto&& mctrack : evt_mc_tracks){ // loop over tracks in event
    
    Track filteredtrack; // declare filtered track for each mc track
    filterTrack(mctrack,filteredtrack); // perform filtering
    evt_filtered_tracks.push_back(filteredtrack); // store filtered tracks

    if (Config::useSmoother) { // do smoothing if specified
      Track smoothedtrack; // declare smoothed track
      smoothTrack(mctrack,filteredtrack,smoothedtrack);// now perform the smoothing
      evt_smoothed_tracks.push_back(smoothedtrack);
    }
    else { // just make the filtered track the same as the smoothed track if smoothing not specified 
      evt_smoothed_tracks.push_back(filteredtrack); 
    }
  } // end loop over mctracks
}

void filterTrack(const Track& mctrack, Track& filteredtrack) {
  // get measurements
  const auto& hits  = mctrack.getMSVec();
  
  // get initial guess for Kalman filter
  TrackState tempState;
  if (Config::useLineEst){ // use straight line fit to two points to extropalate KF initial guess 
    tempState = lineSegmentPredictor(hits[0],hits[Config::diff_ticks]);
  }
  else{ // use MC initial value
    tempState = mctrack.getTSinit();
  }
  // store initial value
  filteredtrack.setTSinit(tempState);
    
  if (Config::debug) {
    tempState.dumpTrackState("Initial input before filter");
  }

  for (int ihit = 0; ihit < Config::nHits; ihit++){ // one hit per layer
    const auto& hit = hits[ihit]; // get the hit for this layer

    // first predict (first step will be equivalent to simulation)
    const TrackState propState = propagateTrackToNextState(tempState);

    if (Config::debug) {
      propState.dumpTrackState(Form("Propagated State at layer: %i",ihit));
    }
      
    // then do the update with measurement
    const TrackState updatedState = updateTrackState(propState,hit);
    float chi2 = computeChi2(updatedState,hit);

    if (Config::debug) {
      updatedState.dumpTrackState(Form("Updated State at layer: %i",ihit));
    }
      
    tempState = updatedState; // now the input for next layer will be filtered state!
      
    // now give the track all the lovely stuff
    filteredtrack.addTS(updatedState);
    filteredtrack.addChi2(chi2);
  } // end loop over layers
}

void smoothTrack(const Track& mctrack, const Track& filteredtrack, Track& smoothedtrack) {
  // Use the already made filtered states
  const auto& filteredStates = filteredtrack.getTSVec();

  // First smoothed state is by definition the last filtered state
  TrackState smoothedState = filteredStates.back(); 
  smoothedtrack.addTS(smoothedState); // save it anyways

  if (Config::debug) {
    smoothedState.dumpTrackState("Smoothed state input (last filtered state)");
  }
      
  // loop over all layers starting at the second to last and end up at the production point
  for (int ilayer = filteredStates.size()-1; ilayer >= 0; ilayer--){ 
    if (Config::debug) {
      std::cout << "Layer: " << ilayer << std::endl;
      if (ilayer != 0) {
	filteredStates[ilayer-1].dumpTrackState("Input Filtered State");
      }
      else { // ilayer == 0 for the loop actually means we are at the production point
	filteredtrack.getTSinit().dumpTrackState("Initial Guess!");
      }
    }

    // perform the smoothing based on layer
    if (ilayer != 0) {
      smoothedState = smoothState(smoothedState,filteredStates[ilayer-1]);
    }
    else{ // ilayer == 0 for the loop actually means we are at the production point, so use the initial guess as the filtered state
      smoothedState = smoothState(smoothedState,filteredtrack.getTSinit());
    }

    if (Config::debug) {
      if (ilayer !=0 ) {
	smoothedState.dumpTrackState("Output Smoothed State");
	mctrack.getTSVec()[ilayer-1].dumpTrackState("MC State");
      }
      else {
	smoothedState.dumpTrackState("Best vertex estimate");
	mctrack.getTSinit().dumpTrackState("MC Generator State");
      }
    }

    // store the TS for the smoothed track
    if (ilayer != 0) {
      smoothedtrack.addTS(smoothedState);
    }
    else{ // set the last layer as the "initial value" of the track at the production point
      smoothedtrack.setTSinit(smoothedState);
    }
  } // end loop over layers
} 
