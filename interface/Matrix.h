#ifndef _matrix_
#define _matrix_

#include "Math/SMatrix.h"

// Matrices are defined as: <rows,columns>

typedef ROOT::Math::SMatrix<float,2> SMatrix22;
typedef ROOT::Math::SVector<float,2> SVector2;

typedef ROOT::Math::SMatrix<float,1> SMatrix11;
typedef ROOT::Math::SVector<float,1> SVector1;

typedef ROOT::Math::SMatrix<float,1,2> SMatrix12;
typedef ROOT::Math::SMatrix<float,2,1> SMatrix21;

template<typename Vector>
void dumpVector(Vector v)
{
  for (int s=0;s<v.kSize;++s) {
    std::cout << std::setw(12) << v.At(s) << " ";
  }
}

// should work with any SMatrix
template<typename Matrix>
void dumpMatrix(Matrix m)
{
  for (int r=0;r<m.kRows;++r) {
    for (int c=0;c<m.kCols;++c) {
      std::cout << std::setw(12) << m.At(r,c) << " ";
    }
    std::cout << std::endl;
  }
}

#include <random>

extern std::default_random_engine            g_gen;
extern std::normal_distribution<float>       g_gaus;
extern std::uniform_real_distribution<float> g_unif;

#endif
