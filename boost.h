

#ifndef BOOST_H
#define BOOST_H

#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/operation_sparse.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include "utils.h"
#include "blaze.h"

class BoostTest {
public:

  BoostTest(){}

  boost::numeric::ublas::vector<double> ConvertVector(blaze::DynamicVector<double> & vec);
  boost::numeric::ublas::compressed_matrix<double> ConvertMatrix(CSR & data);

  void RunSPMV(boost::numeric::ublas::compressed_matrix<double>& D_T, boost::numeric::ublas::compressed_matrix<double>& M_invD, boost::numeric::ublas::vector<double>& gamma);
};
#endif
