

#ifndef VEXCL_H
#define VEXCL_H

#include <vexcl/devlist.hpp>
#include <vexcl/spmat.hpp>
#include <vexcl/vector.hpp>

#include "blaze.h"

class VexCLTest {
public:

  VexCLTest(){}

  vex::Context * CreateContext();

  //
  // This function will effectively warm up OpenCL by running a few calls
  // This is require dto get better timeing results as the first OpenCL call is
  // usually very slow.
  //
  void WarmUp();

  // The way that I wrote this class, by using a pointer to the context,
  // the convert functions cannot be static because the context pointer is used
   vex::vector<double> ConvertVector(blaze::DynamicVector <double>& vec);
   vex::SpMat<double> ConvertMatrix(CSR & data);

  void RunSPMV(vex::SpMat<double>& D_T, vex::SpMat<double>& M_invD, vex::vector<double>& gamma);


  vex::Context * ctx;

};
#endif
