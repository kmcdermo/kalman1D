#include "Update.h"
#include "Config.h"

TrackState updateTrackState(const TrackState & propState, const MeasurementState & hit){
  // get inputs
  const SVector2&  propparams = propState.parameters;
  const SMatrix22& properrs   = propState.errors;
  const SVector1&  measparams = hit.parameters;
  const SMatrix11& measerrs   = hit.errors;
      
  //make kalman gain matrix -- explicit calculations
  int invFail(0);
  const SMatrix11 resErr     = measerrs + Config::projMatrix*properrs*Config::projMatrixT;
  const SMatrix11 resErrInv  = resErr.InverseFast(invFail); // unnecessary as a rank1 tensor is itself the inverse
  const SMatrix21 KalmanGain = properrs*Config::projMatrixT*resErrInv;
    
  //perform propagation of state vector and state covariance
  const SVector2  upparams = propparams + KalmanGain*(measparams - Config::projMatrix*propparams);
  const SMatrix22 uperrs   = (Config::idenMatrix22 - KalmanGain*Config::projMatrix) * properrs;

  TrackState updatedState(upparams,uperrs);
  return updatedState;
}

float computeChi2(const TrackState & updatedState, const MeasurementState & hit){
  int invFail(0);
  const SVector1  residualparams  = hit.parameters - Config::projMatrix*updatedState.parameters;  
  const SMatrix11 residualerrs    = hit.errors     - Config::projMatrix*updatedState.errors*Config::projMatrixT; //covariance of filtered residauls

  return ROOT::Math::Similarity(residualparams,residualerrs.InverseFast(invFail)); // in this case SimilarityT() == Similarity()
}
