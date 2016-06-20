#include "../interface/Track.h"

float Track::getSumChi2() const {
  float sumChi2 = 0.;
  for (auto&& chi2 : chi2Vec_){
    sumChi2 += chi2;
  }
  return sumChi2;
}
