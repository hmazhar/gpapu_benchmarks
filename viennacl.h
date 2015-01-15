

#ifndef VIENNACL_H
#define VIENNACL_H

#ifndef VIENNACL_WITH_OPENCL
#define VIENNACL_WITH_OPENCL
#endif

#include <viennacl/scalar.hpp>
#include <viennacl/vector.hpp>
#include <viennacl/compressed_matrix.hpp>
#include <viennacl/linalg/prod.hpp>

#include "blaze.h"

class ViennaCLTest {
 public:
  ViennaCLTest() {}

  //  vex::Context * CreateContext();

  //
  // This function will effectively warm up OpenCL by running a few calls
  // This is require dto get better timeing results as the first OpenCL call is
  // usually very slow.
  //
  void WarmUp();

  // The way that I wrote this class, by using a pointer to the context,
  // the convert functions cannot be static because the context pointer is used
  viennacl::vector<double> ConvertVector(blaze::DynamicVector<double>& vec);
  viennacl::compressed_matrix<double> ConvertMatrix(CSR& data);

  void RunSPMV(viennacl::compressed_matrix<double>& D_T, viennacl::compressed_matrix<double>& M_invD, viennacl::vector<double>& gamma);

  //  vex::Context * ctx;
};
#endif
