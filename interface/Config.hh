#ifndef _config_
#define _config_

#include "Matrix.hh"

namespace Config
{
  // General config
  extern bool debug;   // turn on printouts
  extern bool useLineEst;  // use initial guess with straight line
  extern bool useSmoother; // turn on Kalman smoothing

  // Event config
  extern int  nEvents; // number of events to simulate + filter [+ smooth]
  extern int  nTracks; // number of tracks / event
  extern int  nHits;   // number of hits / track

  // Simulation config
  extern float startpos; // max starting position per track [cm] 
  extern float startvel; // max starting velocity per track [cm/s]
  extern float deltaT;   // time between hits (constant) [s]

  // Initial estimation options
  extern int  diff_ticks;  // for line guess (nHits - 1 ensures the largest lever arm)

  // Noise uncertainty + variances
  extern float processNoisePos; // noise on position propagation [cm]
  extern float varPNPos;        // processNoisePos * processNoisePos [cm^2] 
  extern float processNoiseVel; // noise on velocity propagation [cm/s]
  extern float varPNVel;        // processNoiseVel * processNoiseVel [cm^2/s^2]
  extern float measNoisePos;    // uncertainty from measurements (constant) [cm]
  extern float varMNPos;        // measNoisePos * measNoisePos [cm^2]

  // Scale factors on over/under estimating variances
  extern float processNoisePosSF; // position variance fudge factor
  extern float processNoiseVelSF; // velocity variance fudge factor
  extern float measNoisePosSF;    // measurement noise fudge factor

  // Special Matrices: set in defineSpecialMatries() as SMatrix is finicky
  extern SMatrix22 idenMatrix22;     // 2x2 identity matrix
  extern SMatrix12 projMatrix;       // define projection matrices
  extern SMatrix21 projMatrixT;      // transpose of projection matrix
  extern SMatrix22 transitionMatrix; // define transition matrix --> Jacobian in other words (i.e. propagation model)
  extern SMatrix22 processNoise;     // define process noise (i.e. disturbances in propagation)
  extern SMatrix11 measurementNoise; // define measurement noise matrix

  // Assign values to parameters based on inputs
  void AssignDependentConfig();
};

#endif
