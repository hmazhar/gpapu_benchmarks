

#ifndef BLAZE_H
#define BLAZE_H

#include <blaze/math/CompressedMatrix.h>
#include <blaze/math/DynamicVector.h>
#include <blaze/math/DenseSubvector.h>

#include <blaze/util/serialization/Archive.h>
#include <blaze/math/serialization/MatrixSerializer.h>
#include <blaze/math/serialization/VectorSerializer.h>

#include "utils.h"

class BlazeTest {
public:

  BlazeTest(){}

  static blaze::DynamicVector<double> ConvertVector(const std::vector<double> & data);
  static blaze::CompressedMatrix<double> ConvertMatrix(const COO& data);
  void RunSPMV(blaze::CompressedMatrix<double>& D_T, blaze::CompressedMatrix<double>& M_invD, blaze::DynamicVector<double>& gamma);
};
#endif
