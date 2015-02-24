#ifndef TESTS_H
#define TESTS_H

template <typename Matrix, typename Vector>
void SPMV(Matrix& D_T, Matrix& M_invD, Vector& gamma, Vector& temporary, Vector& result) {
  for (size_t i = 0; i < RUNS; i++) {
    temporary = M_invD * gamma;
    result = D_T * temporary;
  }
}


#endif

