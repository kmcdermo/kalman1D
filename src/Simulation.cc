#include "../interface/Simulation.h"
#include "../interface/Propagation.h"
#include "../interface/Config.h"
#include <iostream>
#include <string>

void setupTrackByToyMC(TrackState& mcgen, TrackStateVec& mcTruthTSVec, MeasurementStateVec& hits){
  // generate starting position, velocity near origin
  float pos = Config::startpos*g_unif(g_gen);
  float vel = Config::startvel*g_unif(g_gen);

  if (g_unif(g_gen)>0.5) pos*=-1.;
  if (g_unif(g_gen)>0.5) vel*=-1.;

  // set initial cov to be maximal --> not really needed
  SMatrix22 cov = ROOT::Math::SMatrixIdentity();
  cov(0,0) = pos * pos;
  cov(1,1) = vel * vel;

  // store MC generator truth here
  mcgen.parameters=SVector2(pos,vel);
  mcgen.errors=cov;

  TrackState tmpState = mcgen; // input into propagation
  for (int ihit = 0; ihit < Config::nHits; ihit++){
    if (Config::debug) { 
      std::cout << "Layer: " << ihit << std::endl;
      tmpState.dumpTrackState("initState");
    }

    // First do "prediction"
    TrackState propState = propagateTrackToNextState(tmpState);

    if (Config::debug) { 
      propState.dumpTrackState("propState before process noise");
    }

    // Then do process noise --> just gaussians
    propState.parameters(0) += Config::processNoisePos * g_gaus(g_gen);
    propState.parameters(1) += Config::processNoiseVel * g_gaus(g_gen);

    if (Config::debug) { 
      propState.dumpTrackState("propState after process noise");
    }
    
    // store info in MC after process noise
    mcTruthTSVec.push_back(propState); 

    // scattered track state input for next layer
    tmpState = propState; 
    
    // push back measurements + covariance (uncertainty) after making noisy measurement
    MeasurementState hit;
    hit.parameters=propState.parameters(0) + (Config::measNoisePos * g_gaus(g_gen));  //  smear the measurement state --> noisy gaussian
    hit.errors=Config::measurementNoise;
    hits.push_back(hit);

    if (Config::debug) { 
      std::cout << "Hit measurement" << std::endl;
      dumpVector(hit.parameters);
      std::cout << std::endl;
    }
  }
}
