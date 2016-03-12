#include "Smoother.h"
#include "Matrix.h"
#include "Track.h"
#include "Config.h"
#include "Propagation.h"

TrackState smoothState(const TrackState& inSmoothedState, const TrackState& inFilteredState){

  // define references
  const auto& inSmoothPars = inSmoothedState.parameters;
  const auto& inSmoothErrs = inSmoothedState.errors;
  const auto& inFilterPars = inFilteredState.parameters;
  const auto& inFilterErrs = inFilteredState.errors;

  // perform propagation of filtered state to next layer
  const TrackState propagatedState = propagateTrackToNextState(inFilteredState);

  // define Smoother Gain and other useful matrices
  int invFail(0);
  const SMatrix22 propErrs     = propagatedState.errors;
  const SMatrix22 propErrsInv  = propErrs.InverseFast(invFail);
  const SMatrix22 SmootherGain = inFilterErrs*ROOT::Math::Transpose(Config::transitionMatrix)*propErrsInv;

  // perform the smoothing step
  TrackState outSmoothedState; // declare output track state
  outSmoothedState.parameters = inFilterPars + SmootherGain*(inSmoothPars - propagatedState.parameters);
  outSmoothedState.errors     = inFilterErrs + SmootherGain*(inSmoothErrs - propErrs)*ROOT::Math::Transpose(SmootherGain);
    
  return outSmoothedState;
}
