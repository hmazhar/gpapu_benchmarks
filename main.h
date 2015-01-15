
#include "utils.h"
#include "blaze.h"
#include "vexcl.h"


/// Function takes a compressed blaze matrix and then generates a CSR representation from it, this is used for VexCL


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



/// Read matrix from a file and create a blaze matrix from it

