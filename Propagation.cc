#include "Propagation.h"

TrackState propagateTrackToNexState(const TrackState& inputState){

  // get initial values
  const SVector2&  inparams = inputState.parameters;
  const SMatrix22& inerrs   = inputState.errors;
  
  // define transition matrix --> Jacobian in other words (i.e. propagation model)
  SMatrix22 transitionMatrix = ROOT::Math::SMatrixIdentity();
  transitionMatrix(0,1) = Config::deltaT;

  // define process noise (i.e. disturbances in propagation)
  SMatrix22 processNoise = ROOT::Math::SMatrixIdentity();
  processNoise(0,0) = Config::processNoisePos * Config::processNoisePos;
  processNoise(1,1) = Config::processNoiseVel * Config::processNoiseVel;

  // declare outputs
  SVector2  outparams;
  SMatrix22 outerrs;
  
  //perform propagation of state vector and state covariance
  outparams = transitionMatrix * inparams;
  outerrs   = transitionMatrix*inerrs*ROOT::Math::Transpose(transitionMatrix) + processNoise;

  TrackState outputState(outparams,outerrs);
  return outputState;
}

