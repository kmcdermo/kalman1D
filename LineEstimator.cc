#include "LineEstimator.h"
#include "Config.h"

TrackState lineSegmentPredictor(const MeasurementState& hit0, const MeasurementState& hitn){
  // use this estimate the state at one clock tick before hit0
  const float x0 = hit0.x(); const float xn = hitn.x();

  const float pos = x0 - ((xn-x0)/Config::diff_ticks);
  const float vel = (xn-x0)/(Config::diff_ticks*Config::deltaT);
  SVector2  initpar(pos,vel);
  SMatrix22 initcov = ROOT::Math::SMatrixIdentity();
  initcov(0,0) = pos * pos;
  initcov(1,1) = vel * vel;

  return TrackState(initpar,initcov);
}
