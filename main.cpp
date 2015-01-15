#include "main.h"

blaze::CompressedMatrix<double> D_T_blaze, M_invD_blaze;
blaze::DynamicVector<double> gamma_blaze, rhs_blaze;

int num_rows;
int num_cols;
int num_nonzeros;
int RUNS = 100;

vex::profiler<> prof;

double blaze_time;
double eigen_time;
double vexcl_time;
double GFLOP = 1000000000;
template <typename Matrix, typename Vector>
void TEST(Matrix& D_T, Matrix& M_invD, Vector& gamma, Vector& temporary, Vector& result) {
  for (size_t i = 0; i < RUNS; i++) {
    temporary = M_invD * gamma;
    result = D_T * temporary;
  }
}

void Blaze_TEST() {
  blaze::DynamicVector<double> temporary(num_rows);
  blaze::DynamicVector<double> result(num_rows);

  prof.tic_cpu("Blaze");
  TEST(D_T_blaze, M_invD_blaze, gamma_blaze, temporary, result);
  blaze_time = prof.toc("Blaze");
  std::cout << blaze_time << std::endl;
}

void VexCL_TEST() {

  vex::Context ctx(vex::Filter::Env && vex::Filter::DoublePrecision);
  std::cout << ctx;

  std::vector<size_t> row, col;
  std::vector<double> val;

  ConvertSparse(D_T_blaze, row, col, val);
  vex::SpMat<double> D_T_vex(ctx, num_rows, num_cols, row.data(), col.data(), val.data());

  ConvertSparse(M_invD_blaze, row, col, val);
  vex::SpMat<double> M_invD_vex(ctx, num_rows, num_cols, row.data(), col.data(), val.data());

  std::vector<double> gamma_temp(num_rows);

  for (int i = 0; i < num_rows; i++) {
    gamma_temp[i] = gamma_blaze[i];
  }

  vex::vector<double> gamma_vex(ctx, num_rows);
  vex::vector<double> temporary(ctx, num_rows);
  vex::vector<double> result(ctx, num_rows);
  vex::copy(gamma_temp, gamma_vex);


  temporary = M_invD_vex * gamma_vex;
  result = D_T_vex * temporary;

  prof.tic_cpu("VexCL");
  TEST(D_T_vex, M_invD_vex, gamma_vex, temporary, result);
  ctx.finish();
  vexcl_time = prof.toc("VexCL");
  std::cout << vexcl_time << std::endl;
}
//
// void Eigen_TEST() {
//
//  Eigen::SparseMatrix<double> M_invD_eigen(num_cols, num_rows);
//  Eigen::SparseMatrix<double> D_T_eigen(num_rows, num_cols);
//
//  std::vector<Eigen::Triplet<double> > triplet;
//  ConvertSparse(D_T_blaze, triplet);
//  D_T_eigen.setFromTriplets(triplet.begin(), triplet.end());
//
//  ConvertSparse(M_invD_blaze, triplet);
//  M_invD_eigen.setFromTriplets(triplet.begin(), triplet.end());
//
//  Eigen::AMDOrdering<int> ordering;
//  Eigen::PermutationMatrix<Eigen::Dynamic, Eigen::Dynamic, int> perm;
//
//  ordering(D_T_eigen, perm);
//  ordering(M_invD_eigen, perm);
//
//  Eigen::VectorXd gamma_eigen(num_rows);
//  Eigen::VectorXd temporary(num_rows);
//  Eigen::VectorXd result(num_rows);
//
//  for (int i = 0; i < num_rows; i++) {
//    gamma_eigen[i] = gamma_blaze[i];
//  }
//  prof.tic_cpu("EIGEN");
//  TEST(D_T_eigen, M_invD_eigen, gamma_eigen, temporary, result);
//  eigen_time = prof.toc("EIGEN");
//}

int main(int argc, char* argv[]) {
  if (argc > 2) {
    RUNS = atoi(argv[2]);
  }

  FillSparseMatrix("D_T_" + std::string(argv[1]) + ".dat", D_T_blaze);
  FillSparseMatrix("M_invD_" + std::string(argv[1]) + ".dat", M_invD_blaze);
  FillVector("gamma_" + std::string(argv[1]) + ".dat", gamma_blaze);
  FillVector("b_" + std::string(argv[1]) + ".dat", rhs_blaze);

  num_rows = D_T_blaze.rows();
  num_cols = D_T_blaze.columns();
  num_nonzeros = D_T_blaze.nonZeros();
  printf("D_T: rows:, columns:, non zeros:,M_invD: rows:, columns:, non zeros:, gamma: size:, b: size: Memory\n");
  printf("%d, %d, %d, %d, %d, %d, %d, %d %f\n",
		  num_rows,
		  num_cols,
		  num_nonzeros,
         M_invD_blaze.rows(),
         M_invD_blaze.columns(),
         M_invD_blaze.nonZeros(),
         gamma_blaze.size(),
         rhs_blaze.size(),
		 ((num_nonzeros + M_invD_blaze.nonZeros() + gamma_blaze.size() + rhs_blaze.size()) * sizeof(double))/GFLOP );
  //  printf("D_T: rows: %d, columns: %d, non zeros: %d\n", D_T_blaze.rows(), D_T_blaze.columns(), D_T_blaze.nonZeros());
  //  printf("M_invD: rows: %d, columns: %d, non zeros: %d\n", M_invD_blaze.rows(), M_invD_blaze.columns(), M_invD_blaze.nonZeros());
  //  printf("gamma: size: %d\n", gamma_blaze.size());
  //  printf("b: size: %d\n", rhs_blaze.size());

  Blaze_TEST();
  // Eigen_TEST();
  VexCL_TEST();

  // Two Spmv each with 2*nnz operations
  uint operations = 2 * 2 * num_nonzeros;
  uint moved = 2 * (num_nonzeros + 2 * num_rows) * sizeof(double);

  double blaze_single = blaze_time / RUNS;
  double vex_single = vexcl_time / RUNS;
  double eig_single = eigen_time / RUNS;

  printf("Blaze sec, VexCL sec, Speedup, Blaze Flops, VexCL Flops, Bandwidth Blaze, Bandwidth VexCL \n");
  printf("%f, %f, %f, %f, %f, %f, %f\n",
         blaze_single,
         vex_single,
         blaze_single / vex_single,
         operations / blaze_single / GFLOP,
         operations / vex_single / GFLOP,
         moved / blaze_single / GFLOP,
         moved / vex_single / GFLOP);

  //  printf("Blaze %f sec. Eigen %f sec. VexCL %f sec.\n", blaze_single, eig_single, vex_single);
  //  printf("Speedup: Blaze vs Eigen %f\n", blaze_single / eig_single);
  //  printf("Speedup: Blaze vs VexCL %f\n", blaze_single / vex_single);
  //  printf("Speedup: Eigen vs VexCL %f\n", eig_single / vex_single);
  //
  //  printf("Flops: Blaze %f Eigen %f VexCL %f\n", operations / blaze_single / GFLOP, operations / eig_single / GFLOP, operations / vex_single / GFLOP);
  //  printf("Bandwidth: Blaze %f Eigen %f VexCL %f\n", moved / blaze_single / GFLOP, moved / eig_single / GFLOP, moved / vex_single / GFLOP);

  return 0;
}
