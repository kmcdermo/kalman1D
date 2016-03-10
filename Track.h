#ifndef _track_
#define _track_

#include "Matrix.h"
#include "Hit.h"
#include <vector>

struct TrackState{
TrackState(const SVector2 & par, const SMatrix22 & cov) : parameters(par), errors(cov) {}
  SVector2  parameters;
  SMatrix22 errors;
};

typedef std::vector<TrackState> TrackStateVec;

class Track{
 public:
 Track(const TrackStateVec& tsvec, const MeasurementStateVec& hitvec) : tsVec(tsvec), hitVev(hitvec) {}

  TrackStateVec       getTSVec() const {return tsVec;}
  MeasurementStateVec getMSVec() const {return msVec;}

  void addTS(const TrackState& ts)       {tsVec.push_back(ts);}
  void addMS(const MeasurementState& ms) {hitVec.push_back(ms);}

  TrackState       getTS(int layer) const {return tsVec[layer];}
  MeasurementState getMS(int layer) const {return msVec[layer];}

  float position(int layer) const {return tsVec[layer].parameters.At(0);}
  float velocity(int layer) const {return tsVec[layer].parameters.At(1);}

  float exx  (int layer) const {return msVec[layer].errors.At(0,0);}
  float evxvx(int layer) const {return tsVec[layer].errors.At(1,1);}
  float exvx (int layer) const {return msVec[layer]errors.At(0,1);}
  float evxx (int layer) const {return tsVec[layer]errors.At(1,0);}

  float measpos(int layer) const {return msVec[layer].parameters.At(0);}
  float measexx(int layer) const {return msVec[layer].errors.At(0,0);}

 private:
  TrackStateVec       tsVec;
  MeasurementStateVec hitVec;
};


typedef std::vector<Track> TrackVec;

#endif
