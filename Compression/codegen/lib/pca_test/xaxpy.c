/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * xaxpy.c
 *
 * Code generation for function 'xaxpy'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "pca_test.h"
#include "xaxpy.h"

/* Function Definitions */
void b_xaxpy(int n, double a, const double x[9], int ix0, double y[3], int iy0)
{
  int ix;
  int iy;
  int i2;
  int k;
  if (!(a == 0.0)) {
    ix = ix0 - 1;
    iy = iy0 - 1;
    i2 = n - 1;
    for (k = 0; k <= i2; k++) {
      y[iy] += a * x[ix];
      ix++;
      iy++;
    }
  }
}

void c_xaxpy(int n, double a, const double x[3], int ix0, double y[9], int iy0)
{
  int ix;
  int iy;
  int i3;
  int k;
  if (!(a == 0.0)) {
    ix = ix0 - 1;
    iy = iy0 - 1;
    i3 = n - 1;
    for (k = 0; k <= i3; k++) {
      y[iy] += a * x[ix];
      ix++;
      iy++;
    }
  }
}

void d_xaxpy(int n, double a, int ix0, double y_data[], int iy0)
{
  int ix;
  int iy;
  int i4;
  int k;
  if ((n < 1) || (a == 0.0)) {
  } else {
    ix = ix0 - 1;
    iy = iy0 - 1;
    i4 = n - 1;
    for (k = 0; k <= i4; k++) {
      y_data[iy] += a * y_data[ix];
      ix++;
      iy++;
    }
  }
}

void e_xaxpy(int n, double a, const double x_data[], int ix0, double y_data[],
             int iy0)
{
  int ix;
  int iy;
  int i5;
  int k;
  if ((n < 1) || (a == 0.0)) {
  } else {
    ix = ix0 - 1;
    iy = iy0 - 1;
    i5 = n - 1;
    for (k = 0; k <= i5; k++) {
      y_data[iy] += a * x_data[ix];
      ix++;
      iy++;
    }
  }
}

void xaxpy(int n, double a, int ix0, double y[9], int iy0)
{
  int ix;
  int iy;
  int i1;
  int k;
  if (!(a == 0.0)) {
    ix = ix0 - 1;
    iy = iy0 - 1;
    i1 = n - 1;
    for (k = 0; k <= i1; k++) {
      y[iy] += a * y[ix];
      ix++;
      iy++;
    }
  }
}

/* End of code generation (xaxpy.c) */
