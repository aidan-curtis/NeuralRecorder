/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sum.c
 *
 * Code generation for function 'sum'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "pca_test.h"
#include "sum.h"

/* Function Definitions */
double sum(const double x_data[], const int x_size[1])
{
  double y;
  int vlen;
  int k;
  vlen = x_size[0];
  if (x_size[0] == 0) {
    y = 0.0;
  } else {
    y = x_data[0];
    for (k = 2; k <= vlen; k++) {
      y += x_data[k - 1];
    }
  }

  return y;
}

/* End of code generation (sum.c) */
