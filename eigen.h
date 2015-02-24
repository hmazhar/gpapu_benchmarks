

#ifndef EIGEN_H
#define EIGEN_H

#include <Eigen/Eigen>
#include <Eigen/Sparse>
#include "utils.h"

class EigenTest {
 public:
  EigenTest() {}

  static Eigen::VectorXd ConvertVector(const std::vector<double>& data);
  static void ConvertMatrix(const std::vector<Eigen::Triplet<double> >& data, const COO& coo_data, Eigen::SparseMatrix<double> & output);
  static void ConvertMatrixAlt(const std::vector<Eigen::Triplet<double> >& data, const COO& coo_data, Eigen::SparseMatrix<double> & output);
  static std::vector<Eigen::Triplet<double> > ConvertCOO(const COO& data);

  static void RunSPMV(Eigen::SparseMatrix<double>& D_T, Eigen::SparseMatrix<double>& M_invD, Eigen::VectorXd& gamma);
};
#endif
