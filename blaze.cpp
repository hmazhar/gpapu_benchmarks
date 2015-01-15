
#include "blaze.h"
#include "tests.h"

blaze::DynamicVector<double> BlazeTest::ConvertVector(const std::string filename) {
  blaze::DynamicVector<double> vec;
  unsigned int size;
  double v;

  std::stringstream ss(ReadFileAsString(filename));

  ss >> size;
  vec.resize(size);

  for (int i = 0; i < size; i++) {
    ss >> v;
    vec[i] = v;
  }

  return vec;
}
blaze::CompressedMatrix<double> BlazeTest::ConvertMatrix(const std::string filename) {
  blaze::CompressedMatrix<double> mat;
  unsigned int rows, cols, nonzeros;
  unsigned int row_nonzeros, colval;
  double v;

  std::stringstream ss(ReadFileAsString(filename));
  ss >> rows >> cols >> nonzeros;
  mat.resize(rows, cols);
  mat.reserve(nonzeros);
  for (int i = 0; i < rows; i++) {
    ss >> row_nonzeros;
    for (int j = 0; j < row_nonzeros; j++) {
      ss >> colval >> v;
      mat.append(i, colval, v);
    }
    mat.finalize(i);
  }
  return mat;
}

CSR  BlazeTest::ConvertSparse(blaze::CompressedMatrix<double>& mat) {
  CSR data;
  std::vector<size_t>& row = data.row;
  std::vector<size_t>& col = data.col;
  std::vector<double>& val = data.val;

  row.clear();
  col.clear();
  val.clear();

  col.resize(mat.nonZeros());
  val.resize(mat.nonZeros());
  row.resize(mat.rows() + 1);
  unsigned int count = 0;
  for (int i = 0; i < mat.rows(); ++i) {
    row[i] = count;
    for (blaze::CompressedMatrix<double>::Iterator it = mat.begin(i); it != mat.end(i); ++it) {
      col[count] = it->index();
      val[count] = it->value();
      count++;
    }
  }
  row[mat.rows()] = count;

  data.num_rows = mat.rows();
  data.num_cols = mat.columns();
  data.num_nonzero = mat.nonZeros();

  return data;
}

void BlazeTest::RunSPMV(blaze::CompressedMatrix<double>& D_T, blaze::CompressedMatrix<double>& M_invD, blaze::DynamicVector<double>& gamma) {

  unsigned int num_rows = D_T.rows();
  unsigned int num_cols = D_T.columns();
  unsigned int num_nonzeros = D_T.nonZeros();

  blaze::DynamicVector<double> temporary(num_rows);
  blaze::DynamicVector<double> result(num_rows);

  SPMV(D_T, M_invD, gamma, temporary, result);
}
