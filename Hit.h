#ifndef _hit_
#define _hit_

#include "Matrix.h"
#include <vector>

struct MeasurementState{
  MeasurementState() {}
  MeasurementState(const SVector1 & par, const SMatrix11 & cov) : parameters(par), errors(cov) {}
  SVector1  parameters;
  SMatrix11 errors;

  float x()   const {return parameters.At(0);}
  float exx() const {return errors.At(0,0);}
};

typedef std::vector<MeasurementState> MeasurementStateVec;

#endif
