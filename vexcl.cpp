
#include "vexcl.h"
#include "tests.h"
vex::Context* VexCLTest::CreateContext() {
  ctx = new vex::Context(vex::Filter::Env && vex::Filter::DoublePrecision);
  return ctx;
}

void VexCLTest::WarmUp(){

 // This code is taken from the VexCL demo
 const size_t N = 1024 * 1024;
 const size_t M = 1024;
 std::vector<double> A(N, 0);
 std::vector<double> B = random_vector<double>(N);
 std::vector<double> C = random_vector<double>(N);
 std::vector<double> D = random_vector<double>(N);
 vex::vector<double> a(*ctx, A);
 vex::vector<double> b(*ctx, B);
 vex::vector<double> c(*ctx, C);
 vex::vector<double> d(*ctx, D);
 a += b + c * d;

}

 vex::vector<double> VexCLTest::ConvertVector(blaze::DynamicVector<double>& blaze_vec) {
   unsigned int num_rows = blaze_vec.size();

  std::vector<double> temp(num_rows);

  for (int i = 0; i < num_rows; i++) {
    temp[i] = blaze_vec[i];
  }

  vex::vector<double> vex_vec(*ctx, num_rows);
  vex::copy(temp, vex_vec);

  return vex_vec;
}

 vex::SpMat<double> VexCLTest::ConvertMatrix(CSR& data) {
   unsigned int & num_rows = data.num_rows;
   unsigned int & num_cols = data.num_cols;

  std::vector<size_t>& row = data.row;
  std::vector<size_t>& col = data.col;
  std::vector<double>& val = data.val;

  vex::SpMat<double> mat(*ctx, num_rows, num_cols, row.data(), col.data(), val.data());

  return mat;
}

void VexCLTest::RunSPMV(vex::SpMat<double>& D_T, vex::SpMat<double>& M_invD, vex::vector<double>& gamma) {
  unsigned int num_rows = gamma.size();

  vex::vector<double> temporary(*ctx, num_rows);
  vex::vector<double> result(*ctx, num_rows);
  SPMV(D_T, M_invD, gamma, temporary, result);
  ctx->finish();
}
