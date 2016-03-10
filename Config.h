#ifndef _config_
#define _config_

namespace Config
{
  // Simulation Config

  constexpr float startpos        = 1.0; // 1cm
  constexpr float startvel        = 20.0; // 20cm/s
  constexpr int   nHits           = 20;
  constexpr float processNoisePos = 0.1; // 1mm 
  constexpr float processNoiseVel = 0.5; // 5mm/s
  constexpr float measNoisePos    = 0.1; // 1mm

  // General config
  constexpr int   nEvents         = 100;
  constexpr int   nTracks         = 500;

  // Propagation config
  constexpr float deltaT = 0.1;   // 0.1s
};
#endif 
