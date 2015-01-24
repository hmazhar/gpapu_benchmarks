#include "main.h"
#include <chrono>

int num_rows;
int num_cols;
int num_nonzeros;

std::chrono::duration<double> blaze_time;
std::chrono::duration<double> vcl_time;
std::chrono::duration<double> vexcl_time;

std::chrono::time_point<std::chrono::system_clock> start, end;

int main(int argc, char* argv[]) {

  blaze::CompressedMatrix<double> D_T_blaze = BlazeTest::ConvertMatrix("D_T_" + std::string(argv[1]) + ".dat");
  blaze::CompressedMatrix<double> M_invD_blaze = BlazeTest::ConvertMatrix("M_invD_" + std::string(argv[1]) + ".dat");

  blaze::DynamicVector<double> gamma_blaze = BlazeTest::ConvertVector("gamma_" + std::string(argv[1]) + ".dat");
  blaze::DynamicVector<double> rhs_blaze = BlazeTest::ConvertVector("b_" + std::string(argv[1]) + ".dat");

  CSR D_T_csr = BlazeTest::ConvertSparse(D_T_blaze);
  CSR M_invD_csr = BlazeTest::ConvertSparse(M_invD_blaze);

  // Compute some metrics and print them out
  num_rows = D_T_blaze.rows();
  num_cols = D_T_blaze.columns();
  num_nonzeros = D_T_blaze.nonZeros();
  printf("D_T: rows:, columns:, non zeros:,M_invD: rows:, columns:, non zeros:, gamma: size:, b: size: Memory\n");
  printf("%d, %d, %d, %d, %d, %d, %d, %d %f\n", num_rows, num_cols, num_nonzeros, M_invD_blaze.rows(), M_invD_blaze.columns(), M_invD_blaze.nonZeros(), gamma_blaze.size(), rhs_blaze.size(),
         ((num_nonzeros + M_invD_blaze.nonZeros() + gamma_blaze.size() + rhs_blaze.size()) * sizeof(double)) / GFLOP);

  //  printf("D_T: rows: %d, columns: %d, non zeros: %d\n", D_T_blaze.rows(), D_T_blaze.columns(), D_T_blaze.nonZeros());
  //  printf("M_invD: rows: %d, columns: %d, non zeros: %d\n", M_invD_blaze.rows(), M_invD_blaze.columns(), M_invD_blaze.nonZeros());
  //  printf("gamma: size: %d\n", gamma_blaze.size());
  //  printf("b: size: %d\n", rhs_blaze.size());
  {
    BlazeTest blaze_test;
    start = std::chrono::system_clock::now();
    blaze_test.RunSPMV(D_T_blaze, M_invD_blaze, gamma_blaze);
    end = std::chrono::system_clock::now();
    blaze_time = end - start;
  }

  {
    ViennaCLTest viennacl_test;
    viennacl_test.WarmUp();

    viennacl::compressed_matrix<double> D_T_vcl = viennacl_test.ConvertMatrix(D_T_csr);
    viennacl::compressed_matrix<double> M_invD_vcl = viennacl_test.ConvertMatrix(M_invD_csr);
    viennacl::vector<double> gamma_vcl = viennacl_test.ConvertVector(gamma_blaze);

    start = std::chrono::system_clock::now();
    viennacl_test.RunSPMV(D_T_vcl, M_invD_vcl, gamma_vcl);
    end = std::chrono::system_clock::now();
    vcl_time = end - start;
  }

  {
    VexCLTest vexcl_test;
    vexcl_test.CreateContext();
    vexcl_test.WarmUp();

    vex::SpMat<double> D_T_vex = vexcl_test.ConvertMatrix(D_T_csr);
    vex::SpMat<double> M_invD_vex = vexcl_test.ConvertMatrix(M_invD_csr);
    vex::vector<double> gamma_vex = vexcl_test.ConvertVector(gamma_blaze);

    start = std::chrono::system_clock::now();
    vexcl_test.RunSPMV(D_T_vex, M_invD_vex, gamma_vex);
    end = std::chrono::system_clock::now();
    vexcl_time = end - start;
  }

  // Two Spmv each with 2*nnz operations
  uint operations = 2 * 2 * num_nonzeros;
  uint moved = 2 * (num_nonzeros + 2 * num_rows) * sizeof(double);

  double blaze_single = blaze_time.count() / RUNS;
  double vex_single = vexcl_time.count() / RUNS;
  double vcl_single = vcl_time.count() / RUNS;
  // double eig_single = eigen_time.count() / RUNS;

  printf("Blaze: %f %f %f", blaze_single, operations / blaze_single / GFLOP, moved / blaze_single / GFLOP);
  printf("VexCL: %f %f %f", vex_single, operations / vex_single / GFLOP, moved / vex_single / GFLOP);
  printf("VCL: %f %f %f", vcl_single, operations / vcl_single / GFLOP, moved / vcl_single / GFLOP);

  //  printf("Blaze sec, VexCL sec, Speedup, Blaze Flops, VexCL Flops, Bandwidth Blaze, Bandwidth VexCL \n");
  //  printf("%f, %f, %f, %f, %f, %f, %f\n", blaze_single, vex_single, blaze_single / vex_single, operations / blaze_single / GFLOP, operations / vex_single / GFLOP, moved / blaze_single / GFLOP,
  //         moved / vex_single / GFLOP);
  //  //
  //  printf("Blaze %f sec. Eigen %f sec. VexCL %f sec.\n", blaze_single, eig_single, vex_single);
  //  printf("Speedup: Blaze vs Eigen %f\n", blaze_single / eig_single);
  //  printf("Speedup: Blaze vs VexCL %f\n", blaze_single / vex_single);
  //  printf("Speedup: Eigen vs VexCL %f\n", eig_single / vex_single);
  //
  //  printf("Flops: Blaze %f Eigen %f VexCL %f\n", operations / blaze_single / GFLOP, operations / eig_single / GFLOP, operations / vex_single / GFLOP);
  //  printf("Bandwidth: Blaze %f Eigen %f VexCL %f\n", moved / blaze_single / GFLOP, moved / eig_single / GFLOP, moved / vex_single / GFLOP);

  return 0;
}
