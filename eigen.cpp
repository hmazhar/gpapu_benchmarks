
#include "eigen.h"
#include "tests.h"

Eigen::VectorXd EigenTest::ConvertVector(const std::vector<double> & data) {
  Eigen::VectorXd vec(data.size());
  for (int i = 0; i < data.size(); i++) {
    vec[i] = data[i];
  }

  return vec;
}
void  EigenTest::ConvertMatrix(const std::vector<Eigen::Triplet<double> >& data, const COO& coo_data, Eigen::SparseMatrix<double> & output) {
  std::cout<<"Creating an Eigen Sparse matrix "<<coo_data.num_rows<<" "<<coo_data.num_cols<<" "<<data.size()<<std::endl;
  output.setFromTriplets(data.begin(), data.end());
}

void  EigenTest::ConvertMatrixAlt(const std::vector<Eigen::Triplet<double> >& data, const COO& coo_data, Eigen::SparseMatrix<double> & output) {
  std::cout<<"Creating an Eigen Sparse matrix "<<coo_data.num_rows<<" "<<coo_data.num_cols<<" "<<data.size()<<std::endl;
  for (int i = 0; i < coo_data.num_nonzero; i++) {
    output.insert(coo_data.row[i],coo_data.col[i]) = coo_data.val[i];
  }
}


std::vector<Eigen::Triplet<double> > EigenTest::ConvertCOO(const COO& data) {
  std::vector<Eigen::Triplet<double> > triplet_data;

  triplet_data.resize(data.num_nonzero);

  for (int i = 0; i < data.num_nonzero; i++) {
    triplet_data[i] = Eigen::Triplet<double>(data.row[i],data.col[i],data.val[i]);
  }
  std::cout<<"Created an Eigen Triplet "<<triplet_data.size()<<std::endl;
  return triplet_data;
}


void EigenTest::RunSPMV(Eigen::SparseMatrix<double>& D_T, Eigen::SparseMatrix<double>& M_invD, Eigen::VectorXd& gamma) {

  unsigned int num_rows = D_T.rows();
  unsigned int num_cols = D_T.cols();
  unsigned int num_nonzeros = D_T.nonZeros();

  Eigen::VectorXd temporary(num_rows);
  Eigen::VectorXd result(num_rows);

  SPMV(D_T, M_invD, gamma, temporary, result);
}
