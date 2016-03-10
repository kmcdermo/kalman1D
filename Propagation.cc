#include "Propagation.h"

TrackState propagateTrackToNextState(const TrackState& inputState){

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
  
  //perform propagation of state vector and state covariance
  SVector2  propparams = transitionMatrix * inparams;
  SMatrix22 properrs   = transitionMatrix*inerrs*ROOT::Math::Transpose(transitionMatrix) + processNoise;

  TrackState propState(propparams,properrs);
  return propState;
}

