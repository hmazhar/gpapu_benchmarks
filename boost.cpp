
#include "boost.h"
#include "tests.h"
boost::numeric::ublas::vector<double> ConvertVector(blaze::DynamicVector<double>& blaze_vec) {
  unsigned int num_rows = blaze_vec.size();

  boost::numeric::ublas::vector<double> temp(num_rows);

  for (int i = 0; i < num_rows; i++) {
    temp[i] = blaze_vec[i];
  }

  return temp;
}
boost::numeric::ublas::compressed_matrix<double> ConvertMatrix(CSR& data) {



//
    unsigned int rows = data.num_rows;
    unsigned int cols = data.num_cols;
    unsigned int nonzeros = data.num_nonzero;
//
//
//    std::stringstream ss(ReadFileAsString(filename));
//    ss >> rows >> cols >> nonzeros;
//
    boost::numeric::ublas::compressed_matrix<double> mat(rows, cols,nonzeros);
//
//    for (int i = 0; i < rows; i++) {
//      int row = data[i];
//      for (int j = 0; j < row_nonzeros; j++) {
//        ss >> colval >> v;
//        mat (i, colval) = v;
//      }
//    }
    return mat;



}

void BoostTest::RunSPMV(boost::numeric::ublas::compressed_matrix<double>& D_T, boost::numeric::ublas::compressed_matrix<double>& M_invD, boost::numeric::ublas::vector<double>& gamma) {

  unsigned int num_rows = D_T.size1();
  unsigned int num_cols = D_T.size2();
  unsigned int num_nonzeros = D_T.nnz();

  boost::numeric::ublas::vector<double> temporary(num_rows);
  boost::numeric::ublas::vector<double> result(num_rows);

  for (size_t i = 0; i < RUNS; i++) {
    boost::numeric::ublas::axpy_prod(M_invD, gamma, temporary, true);
    boost::numeric::ublas::axpy_prod(D_T, temporary, result, true);
  }
}
