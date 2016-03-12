#ifndef _config_
#define _config_

#include "Matrix.h"

namespace Config
{
  // Simulation Config
  constexpr float startpos = 1.0; // 1cm
  constexpr float startvel = 20.0; // 20cm/s
  constexpr int   nHits    = 20;

  // noise uncertainty + variances
  constexpr float processNoisePos = 0.1; // 1mm 
  constexpr float varPNPos        = processNoisePos * processNoisePos; 
  constexpr float processNoiseVel = 0.5; // 5mm/s
  constexpr float varPNVel        = processNoiseVel * processNoiseVel; 
  constexpr float measNoisePos    = 0.1; // 1mm
  constexpr float varMNPos        = measNoisePos * measNoisePos;

  // Scale factors on over/under estimating variances
  constexpr float processNoisePosSF = 1.0; // position variance fudge factor
  constexpr float processNoiseVelSF = 1.0; // velocity variance fudge factor
  constexpr float measNoisePosSF    = 1.0; // measurement noise fudge factor

  // General config
  extern    bool debug;
  constexpr int  nEvents = 100;
  constexpr int  nTracks = 500;

  // Propagation config
  constexpr float deltaT = 0.1;   // 0.1s

  // Fitting config
  extern    bool useLineEst;
  constexpr int  diff_ticks = nHits - 1;  // for line guess (nHits - 1 ensures the largest lever arm)
  extern    bool useSmoother;

  // Special Matrices: set in defineSpecialMatries() as SMatrix is finicky
  extern SMatrix22 idenMatrix22; // 2x2 identity matrix
  extern SMatrix12 projMatrix;   // define projection matrices
  extern SMatrix21 projMatrixT;  // transpose
  extern SMatrix22 transitionMatrix; // define transition matrix --> Jacobian in other words (i.e. propagation model)
  extern SMatrix22 processNoise;     // define process noise (i.e. disturbances in propagation)
  extern SMatrix11 measurementNoise; // define measurement noise matrix
  void defineSpecialMatrices();
};



#endif 
