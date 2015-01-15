

#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

//
// Define a structure that holds a sparse CSR representation
//
struct CSR {
  std::vector<size_t> row;    // Row entries for matrix
  std::vector<size_t> col;    // Column entries for matrix
  std::vector<double> val;    // Values for the non zero entries
  unsigned int num_rows;      // Number of Rows
  unsigned int num_cols;      // Number of Columns
};


//
// Read the contents of an ascii file as a string
// Here the file is read all at once for performance reasons
//
static std::string ReadFileAsString(std::string fname) {
  std::ifstream file(fname.c_str());
  std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  return buffer;
}

// The number of floating point operations is 2 * the number of non zeros
static double ComputeFlopSPMV(unsigned int num_nonzeros){
  return 2* num_nonzeros;

}

// The amount of memory moved is the number of nonzeros + 2 * number of rows
static double ComputeBandwidthSPMV(unsigned int num_nonzeros, unsigned int num_rows){
  return (num_nonzeros + 2 * num_rows) * sizeof(double);

}

#define RUNS 100

#endif
