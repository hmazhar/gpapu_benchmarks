
#include "viennacl.h"
#include "tests.h"
// vex::Context* ViennaCLTest::CreateContext() {
//  ctx = new vex::Context(vex::Filter::Env && vex::Filter::DoublePrecision);
//  return ctx;
//}

void ViennaCLTest::WarmUp() {

  const size_t N = 1024 * 1024;
  const size_t M = 1024;
  std::vector<double> A(N, 0);
  std::vector<double> B = random_vector<double>(N);
  std::vector<double> C = random_vector<double>(N);
  std::vector<double> D = random_vector<double>(N);
  viennacl::vector<double> a, b, c, d;
  viennacl::copy(A, a);
  viennacl::copy(B, b);
  viennacl::copy(C, c);
  viennacl::copy(D, d);
  a += b + viennacl::linalg::element_prod(c , d);
}

viennacl::vector<double> ViennaCLTest::ConvertVector(blaze::DynamicVector<double>& blaze_vec) {
  unsigned int num_rows = blaze_vec.size();

  std::vector<double> temp(num_rows);

  for (int i = 0; i < num_rows; i++) {
    temp[i] = blaze_vec[i];
  }

  viennacl::vector<double> vcl_vec(num_rows);
  viennacl::copy(temp, vcl_vec);

  return vcl_vec;
}

viennacl::compressed_matrix<double> ViennaCLTest::ConvertMatrix(CSR& data) {
  unsigned int& num_rows = data.num_rows;
  unsigned int& num_cols = data.num_cols;
  unsigned int& num_nonzero = data.num_nonzero;

  std::vector<size_t>& row = data.row;
  std::vector<size_t>& col = data.col;
  std::vector<double>& val = data.val;

  // vex::SpMat<double> mat(*ctx, num_rows, num_cols, row.data(), col.data(), val.data());
  viennacl::compressed_matrix<double> mat;
  mat.set(row.data(), col.data(), val.data(), num_rows, num_cols, num_nonzero);

  return mat;
}

void ViennaCLTest::RunSPMV(viennacl::compressed_matrix<double>& D_T, viennacl::compressed_matrix<double>& M_invD, viennacl::vector<double>& gamma) {
  unsigned int num_rows = gamma.size();

  viennacl::vector<double> temporary(num_rows);
  viennacl::vector<double> result(num_rows);

  for (size_t i = 0; i < RUNS; i++) {
    temporary = viennacl::linalg::prod(M_invD, gamma);
    result = viennacl::linalg::prod(D_T, temporary);
  }
}
