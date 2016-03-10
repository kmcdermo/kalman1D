#include "Simulation.h"
#include "Propagation.h"

void setupTrackByToyMC(TrackStateVec& mctruth, MeasurementStateVec& hits){

  // generate starting position, velocity near origin
  float pos = Config::startpos*g_unif(g_gen);
  float vel = Config::startvel*g_unif(g_gen);

  if (g_unif(g_gen)>0.5) pos*=-1.;
  if (g_unif(g_gen)>0.5) vel*=-1.;

  // set initial cov to be maximal --> not really needed
  SMatrix22 cov = ROOT::Math::SMatrixIdentity();
  cov.At(0,0) = pos * pos;
  cov.At(1,1) = vel * vel;

  // make trackstates to begin saving
  TrackState initState;
  initState.parameters=SVector2(pos,vel);
  initState.errors=cov;

  // store MC truth here
  mctruth.push_back(initState);

  TrackState tmpState = initState;

  for (int ihit = 0; ihit < Config::nHits; ihit++){
    // First do "prediction"
    TrackState propState = propagateTrackToNexState(tmpState);

    // Then do process noise --> just gaussians
    propState.parameters.At(0) += Config::processNoisePos * g_gaus(g_gen);
    propState.parameters.At(1) += Config::processNoiseVel * g_gaus(g_gen);
    
    // store info in MC after process noise
    mctruth.push_back(propState); 
    
    // now smear the measurement state
    propState.parameters.At(0) += Config::measNoisePos * g_gaus(g_gen);

    // push back measurements + covariance (uncertainty)
    MeasurementState hit;
    hit.parameters=propState.parameters.At(0);
    hit.errors=Config::measNoisePos*Config::measNoisePos;
    hits.push_back(hit);

    tmpState = propState;
  }
}
