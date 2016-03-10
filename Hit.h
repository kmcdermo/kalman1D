#ifndef _hit_
#define _hit_

#include "Matrix.h"
#include <vector>

struct MeasurementState{
MeasurementState(const SVector1 & par, const SMatrix11 & cov) : parameters(par), errors(cov) {}
  SVector1  parameters;
  SMatrix11 errors;
};

typedef std::vector<MeasurementState> MeasurementStateVec;

#endif
