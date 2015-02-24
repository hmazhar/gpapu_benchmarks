

#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>

struct COO {
  std::vector<size_t> row;     // Row entries for matrix
  std::vector<size_t> col;     // Column entries for matrix
  std::vector<double> val;     // Values for the non zero entries
  unsigned int num_rows;       // Number of Rows
  unsigned int num_cols;       // Number of Columns
  unsigned int num_nonzero;    // Number of non zeros
  // Once the data has been read in, compute the number of rows, columns, and nonzeros
  void Update() {
    num_rows = row.back() + 1;
    num_cols = *std::max_element(col.begin(), col.end()) + 1;
    num_nonzero = val.size();
    std::cout << "COO Updated: [Rows, Columns, Non Zeros] [" << num_rows << ", " << num_cols << ", " << num_nonzero << "] " << std::endl;
  }
  void Print(){
    for (int i = 0; i < num_nonzero; i++) {
      std::cout<<row[i]<<" "<<col[i]<<" "<<val[i]<<std::endl;
    }
  }
};

//
// Define a structure that holds a sparse CSR representation
//
struct CSR {
  std::vector<size_t> row;    // Row pointer for matrix
  std::vector<size_t> col;    // Column entries for matrix
  std::vector<double> val;    // Values for the non zero entries
  unsigned int num_rows;      // Number of Rows
  unsigned int num_cols;      // Number of Columns
  unsigned int num_nonzero;   // Number of non zeros

  void Convert(const COO & data){
    //Initialize the CSR structure
    num_rows = data.num_rows;
    num_cols = data.num_cols;
    num_nonzero = data.num_nonzero;

    //The column data and value data will be the same
    col = data.col;
    val = data.val;
    //row stores row pointer, the extra last item is total number of entries
    row.reserve(num_rows + 1);

    unsigned int count = 0;
    for (int i = 0; i < data.num_nonzero; i++) {
      // If we are at the first entry write a zero
      if (i == 0) {
        row[data.row[i]] = count;
      } else if(data.row[i]!=data.row[i+1]){
        row[data.row[i]] = count + 1;
      }
      count++;
    }
    row[num_rows] = data.num_nonzero;
  }
};

//
// Read the contents of an ascii file as a string
// Here the file is read all at once for performance reasons
static std::string ReadFileAsString(std::string fname) {
  std::ifstream in(fname.c_str(), std::ios::in);
  std::string contents;
  in.seekg(0, std::ios::end);
  contents.resize(in.tellg());
  in.seekg(0, std::ios::beg);
  in.read(&contents[0], contents.size());
  in.close();
  return (contents);
}

// The number of floating point operations is 2 * the number of non zeros
static double ComputeFlopSPMV(unsigned int num_nonzeros) { return 2 * num_nonzeros; }

// The amount of memory moved is the number of nonzeros + 2 * number of rows
static double ComputeBandwidthSPMV(unsigned int num_nonzeros, unsigned int num_rows) { return (num_nonzeros + 2 * num_rows) * sizeof(double); }



// Generates a vector with random values
template <typename real>
std::vector<real> random_vector(size_t n) {
  std::default_random_engine rng(std::rand());
  std::uniform_real_distribution<real> rnd(0.0, 1.0);
  std::vector<real> x(n);
  for (size_t i = 0; i < n; ++i)
    x[i] = rnd(rng);
  return x;
}

#define RUNS 100
#define GFLOP 1000000000

#endif
