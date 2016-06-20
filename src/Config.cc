#include "../interface/Config.h"

namespace Config{
  bool debug       = false;
  bool useLineEst  = true;
  bool useSmoother = true;

  SMatrix22 idenMatrix22     = ROOT::Math::SMatrixIdentity();
  SMatrix12 projMatrix       = ROOT::Math::SMatrixIdentity();
  SMatrix21 projMatrixT      = ROOT::Math::SMatrixIdentity();
  SMatrix22 transitionMatrix = ROOT::Math::SMatrixIdentity();
  SMatrix22 processNoise     = ROOT::Math::SMatrixIdentity();
  SMatrix11 measurementNoise = ROOT::Math::SMatrixIdentity();
  
  void defineSpecialMatrices(){
    // set individual elements
    transitionMatrix(0,1) = deltaT;

    processNoise(0,0)     = varPNPos * processNoisePosSF;
    processNoise(1,1)     = varPNVel * processNoiseVelSF;

    measurementNoise(0,0) = varMNPos * measNoisePosSF;
  }  
}
