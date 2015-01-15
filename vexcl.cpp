
#include "vexcl.h"
#include "tests.h"
vex::Context* VexCLTest::CreateContext() {
  ctx = new vex::Context(vex::Filter::Env && vex::Filter::DoublePrecision);
  return ctx;
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
