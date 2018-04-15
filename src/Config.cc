#include "../interface/Config.hh"

namespace Config
{
  bool debug       = false;
  bool useLineEst  = true;
  bool useSmoother = true;

  int  nEvents = 100;
  int  nTracks = 500;
  int  nHits   = 20;

  float startpos = 1.0;  // 1 cm
  float startvel = 20.0; // 20 cm/s
  float deltaT   = 0.1;  // 0.1 s

  int  diff_ticks = nHits - 1;

  float processNoisePos = 0.1; // 1 mm 
  float varPNPos        = processNoisePos * processNoisePos; 
  float processNoiseVel = 0.5; // 5 mm/s
  float varPNVel        = processNoiseVel * processNoiseVel; 
  float measNoisePos    = 0.1; // 1 mm
  float varMNPos        = measNoisePos * measNoisePos;

  float processNoisePosSF = 1.0;
  float processNoiseVelSF = 1.0;
  float measNoisePosSF    = 1.0;

  SMatrix22 idenMatrix22     = ROOT::Math::SMatrixIdentity();
  SMatrix12 projMatrix       = ROOT::Math::SMatrixIdentity();
  SMatrix21 projMatrixT      = ROOT::Math::SMatrixIdentity();
  SMatrix22 transitionMatrix = ROOT::Math::SMatrixIdentity();
  SMatrix22 processNoise     = ROOT::Math::SMatrixIdentity();
  SMatrix11 measurementNoise = ROOT::Math::SMatrixIdentity();
  
  void AssignDependentConfig()
  {
    // Dependent Variables
    if (diff_ticks > nHits) diff_ticks = nHits - 1;

    varPNPos   = processNoisePos * processNoisePos; 
    varPNVel   = processNoiseVel * processNoiseVel; 
    varMNPos   = measNoisePos * measNoisePos;

    // Assign Special Matrix Elements

    transitionMatrix(0,1) = deltaT;

    processNoise(0,0)     = varPNPos * processNoisePosSF;
    processNoise(1,1)     = varPNVel * processNoiseVelSF;

    measurementNoise(0,0) = varMNPos * measNoisePosSF;
  }  
}
