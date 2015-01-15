#include <vexcl/devlist.hpp>
#include <vexcl/spmat.hpp>
#include <vexcl/vector.hpp>

#include <blaze/math/CompressedMatrix.h>
#include <blaze/math/DynamicVector.h>
#include <blaze/math/DenseSubvector.h>

#include <blaze/util/serialization/Archive.h>
#include <blaze/math/serialization/MatrixSerializer.h>
#include <blaze/math/serialization/VectorSerializer.h>

//#include <Eigen/Sparse>

#include <vector>
#include <sstream>
#include <string>
#include <iostream>

/// Function takes a compressed blaze matrix and then generates a CSR representation from it, this is used for VexCL
template<typename T>
void ConvertSparse(blaze::CompressedMatrix<double>& mat,    // Input: The matrix to be converted
                   std::vector<T>& row,    // Output: Row entries for matrix
                   std::vector<T>& col,    // Output: Column entries for matrix
                   std::vector<double>& val    // Output: Values for the non zero entries
                   ) {
  row.clear();
  col.clear();
  val.clear();

  col.resize(mat.nonZeros());
  val.resize(mat.nonZeros());
  row.resize(mat.rows()+1);
  uint count = 0;
  for (int i = 0; i < mat.rows(); ++i) {
    row[i] = count;
    for (blaze::CompressedMatrix<double>::Iterator it = mat.begin(i); it != mat.end(i); ++it) {
      col[count] = it->index();
      val[count] =it->value();
      count++;
    }
  }
  row[mat.rows()] = count;
}

/// Function takes a compressed blaze matrix and then generates a CSR representation from it, this is used for Eigen
//void ConvertSparse(blaze::CompressedMatrix<double>& mat,    // Input: The matrix to be converted
//                   std::vector<Eigen::Triplet<double> >& triplet    // Output: Triplet of i,j, val entries for each non zero entry
//                   ) {
//  triplet.clear();
//  triplet.reserve(mat.nonZeros());
//  for (int i = 0; i < mat.rows(); ++i) {
//    for (blaze::CompressedMatrix<double>::Iterator it = mat.begin(i); it != mat.end(i); ++it) {
//      triplet.push_back(Eigen::Triplet<double>(i, it->index(), it->value()));
//    }
//  }
//}

/// Take a file and read its contents into a string
std::string ReadFileAsString(std::string fname) {
  std::ifstream file(fname.c_str());
  std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  return buffer;
}

/// Read matrix from a file and create a blaze matrix from it
void FillSparseMatrix(const std::string filename, blaze::CompressedMatrix<double>& mat) {
  std::stringstream ss(ReadFileAsString(filename));

  uint rows, cols, nonzeros;
  uint row_nonzeros, colval;
  ss >> rows >> cols >> nonzeros;
  mat.resize(rows, cols);
  mat.reserve(nonzeros);

  double v;
  for (int i = 0; i < rows; i++) {
    ss >> row_nonzeros;
    for (int j = 0; j < row_nonzeros; j++) {
      ss >> colval >> v;
      mat.append(i, colval, v);
    }
    mat.finalize(i);
  }
}
/// Read vector from a file and create a blaze vector from it

void FillVector(const std::string filename, blaze::DynamicVector<double>& vec) {
  std::stringstream ss(ReadFileAsString(filename));

  uint size;
  ss >> size;
  vec.resize(size);

  double v;
  for (int i = 0; i < size; i++) {
    ss >> v;
    vec[i] = v;
  }
}
