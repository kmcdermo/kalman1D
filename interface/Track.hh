#ifndef _track_
#define _track_

#include "Matrix.hh"
#include "Hit.hh"

#include <vector>
#include <string>
#include <iostream>

struct TrackState
{
  TrackState() {}
  TrackState(const SVector2 & par, const SMatrix22 & cov) : parameters(par), errors(cov) {}
  SVector2  parameters;
  SMatrix22 errors;

  //dump data members
  void dumpTrackState (const std::string & title) const {
    std::cout << title << std::endl;
    std::cout << "Parameters" << std::endl;
    dumpVector(parameters);
    std::cout << std::endl << "Errors" << std::endl;
    dumpMatrix(errors);
  }

  // TS accessors
  float x()     const {return parameters.At(0);}
  float vx()    const {return parameters.At(1);}
  float exx()   const {return errors.At(0,0);}
  float evxvx() const {return errors.At(1,1);}
  float exvx()  const {return errors.At(0,1);}
  float evxx()  const {return errors.At(1,0);}
};

typedef std::vector<TrackState> TrackStateVec;

class Track
{
 public:
  Track() {}
  Track(const TrackState &tsInit, const TrackStateVec& tsvec, const MeasurementStateVec& msvec) : tsInit_(tsInit), tsVec_(tsvec), hitVec_(msvec) {}
  
  TrackState          getTSinit() const {return tsInit_;}
  TrackStateVec       getTSVec()  const {return tsVec_;}
  MeasurementStateVec getMSVec()  const {return hitVec_;}

  void setTSinit(const TrackState&          tsInit) {tsInit_=tsInit;}
  void setTSVec (const TrackStateVec&       tsVec)  {tsVec_=tsVec;}
  void setMSVec (const MeasurementStateVec& msVec)  {hitVec_=msVec;}

  void addTS(const TrackState& ts)       {tsVec_.push_back(ts);}
  void addMS(const MeasurementState& ms) {hitVec_.push_back(ms);}

  TrackState       getTS(int layer) const {return tsVec_[layer];}
  MeasurementState getMS(int layer) const {return hitVec_[layer];}

  float x    (int layer) const {return tsVec_[layer].x();}
  float vx   (int layer) const {return tsVec_[layer].vx();}

  float exx  (int layer) const {return tsVec_[layer].exx();}
  float evxvx(int layer) const {return tsVec_[layer].evxvx();}
  float exvx (int layer) const {return tsVec_[layer].exvx();}
  float evxx (int layer) const {return tsVec_[layer].evxx();}

  float mx   (int layer) const {return hitVec_[layer].x();}
  float mexx (int layer) const {return hitVec_[layer].exx();}

  void  addChi2(float chi2)             {chi2Vec_.push_back(chi2);}
  float getLayerChi2(int layer)   const {return chi2Vec_[layer];}
  float getSumChi2() const;
  std::vector<float> getChi2Vec() const {return chi2Vec_;}

 private:
  TrackState          tsInit_;
  TrackStateVec       tsVec_;
  MeasurementStateVec hitVec_;
  std::vector<float>  chi2Vec_;
};

typedef std::vector<Track> TrackVec;

#endif
