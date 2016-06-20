#include "../interface/Propagation.h"
#include "../interface/Config.h"

TrackState propagateTrackToNextState(const TrackState& inputState){

  // get initial values
  const SVector2&  inparams = inputState.parameters;
  const SMatrix22& inerrs   = inputState.errors;

  //perform propagation of state vector and state covariance
  SVector2  propparams = Config::transitionMatrix * inparams;
  SMatrix22 properrs   = Config::transitionMatrix*inerrs*ROOT::Math::Transpose(Config::transitionMatrix) + Config::processNoise;

  TrackState propState(propparams,properrs);
  return propState;
}

