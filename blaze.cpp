
#include "blaze.h"
#include "tests.h"

blaze::DynamicVector<double> BlazeTest::ConvertVector(const std::vector<double> & data) {
  blaze::DynamicVector<double> vec(data.size());
  for (int i = 0; i < data.size(); i++) {
    vec[i] = data[i];
  }

  return vec;
}
blaze::CompressedMatrix<double> BlazeTest::ConvertMatrix(const COO& data) {
  blaze::CompressedMatrix<double> mat;

  mat.resize(data.num_rows, data.num_cols);
  mat.reserve(data.num_nonzero);
  for (int i = 0; i < data.num_nonzero; i++) {

    mat.append(data.row[i], data.col[i], data.val[i]);
    // If we are at the last entry, finalize the row
    if (i == data.num_nonzero - 1) {
      mat.finalize(data.row[i]);
    } else if(data.row[i]!=data.row[i+1]){
      mat.finalize(data.row[i]);
    }
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
