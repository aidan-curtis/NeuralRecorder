/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * sqrt.c
 *
 * Code generation for function 'sqrt'
 *
 */

/* Include files */
#include <math.h>
#include "rt_nonfinite.h"
#include "pca_test.h"
#include "sqrt.h"

/* Function Definitions */
void b_sqrt(double *x)
{
  *x = sqrt(*x);
}

void c_sqrt(double x_data[], int x_size[1])
{
  int nx;
  int k;
  nx = x_size[0];
  for (k = 0; k < nx; k++) {
    x_data[k] = sqrt(x_data[k]);
  }
}

/* End of code generation (sqrt.c) */
