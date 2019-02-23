/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pca_test.c
 *
 * Code generation for function 'pca_test'
 *
 */

/* Include files */
#include <math.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "pca_test.h"
#include "pca.h"

/* Function Definitions */
void pca_test(const double X[9], double coeff_data[], int coeff_size[2], double
              eig_data[], int eig_size[2])
{
  double x[9];
  int p;
  int naninfo_nRowsWithNaNs;
  int naninfo_nNaNsInRow[3];
  int wcol;
  int j;
  boolean_T naninfo_isNaN[9];
  boolean_T noNaNs;
  int DOF;
  double mu[3];
  double xcol;
  double sgn;
  int xNoNaNs_size[2];
  double b_coeff_data[9];
  int b_coeff_size[2];
  double score_data[9];
  int score_size[2];
  double latent_data[3];
  int latent_size[1];
  double explained_data[3];
  int explained_size[1];
  int WNoNaNs_size[1];
  int i;
  double b_score_data[9];
  int b_score_size[2];
  int tsquared_size[2];
  double absc_tmp;
  double absc;
  memcpy(&x[0], &X[0], 9U * sizeof(double));
  p = 0;
  naninfo_nRowsWithNaNs = 0;
  naninfo_nNaNsInRow[0] = 0;
  naninfo_nNaNsInRow[1] = 0;
  naninfo_nNaNsInRow[2] = 0;
  for (wcol = 0; wcol < 9; wcol++) {
    naninfo_isNaN[wcol] = rtIsNaN(X[wcol]);
  }

  for (j = 0; j < 3; j++) {
    if (naninfo_isNaN[3 * j]) {
      naninfo_nNaNsInRow[0]++;
      p++;
    }

    if (naninfo_isNaN[1 + 3 * j]) {
      naninfo_nNaNsInRow[1]++;
      p++;
    }

    if (naninfo_isNaN[2 + 3 * j]) {
      naninfo_nNaNsInRow[2]++;
      p++;
    }
  }

  if (naninfo_nNaNsInRow[0] > 0) {
    naninfo_nRowsWithNaNs = 1;
  }

  if (naninfo_nNaNsInRow[1] > 0) {
    naninfo_nRowsWithNaNs++;
  }

  if (naninfo_nNaNsInRow[2] > 0) {
    naninfo_nRowsWithNaNs++;
  }

  noNaNs = (p <= 0);
  DOF = 3 - naninfo_nRowsWithNaNs;
  if (3 - naninfo_nRowsWithNaNs >= 1) {
    DOF = 2 - naninfo_nRowsWithNaNs;
  }

  if (!noNaNs) {
    for (j = 0; j < 3; j++) {
      wcol = 0;
      xcol = 0.0;
      sgn = X[3 * j];
      if (!rtIsNaN(sgn)) {
        wcol = 1;
        xcol = sgn;
      }

      sgn = X[1 + 3 * j];
      if (!rtIsNaN(sgn)) {
        wcol++;
        xcol += sgn;
      }

      sgn = X[2 + 3 * j];
      if (!rtIsNaN(sgn)) {
        wcol++;
        xcol += sgn;
      }

      mu[j] = xcol / (double)wcol;
    }
  } else {
    for (j = 0; j < 3; j++) {
      mu[j] = ((X[3 * j] + X[1 + 3 * j]) + X[2 + 3 * j]) / 3.0;
    }
  }

  for (j = 0; j < 3; j++) {
    x[3 * j] -= mu[j];
    p = 1 + 3 * j;
    x[p] -= mu[j];
    p = 2 + 3 * j;
    x[p] -= mu[j];
  }

  if (noNaNs) {
    localSVD(x, DOF, b_coeff_data, b_coeff_size, score_data, score_size,
             latent_data, latent_size, mu, explained_data, explained_size);
  } else {
    xNoNaNs_size[0] = 3 - naninfo_nRowsWithNaNs;
    xNoNaNs_size[1] = 3;
    WNoNaNs_size[0] = 3 - naninfo_nRowsWithNaNs;
    wcol = -1;
    for (i = 0; i < 3; i++) {
      if (naninfo_nNaNsInRow[i] == 0) {
        wcol++;
        score_data[wcol] = x[i];
        score_data[(wcol - naninfo_nRowsWithNaNs) + 3] = x[i + 3];
        score_data[wcol + ((3 - naninfo_nRowsWithNaNs) << 1)] = x[i + 6];
        mu[wcol] = 1.0;
      }
    }

    b_localSVD(score_data, xNoNaNs_size, DOF, mu, WNoNaNs_size, b_coeff_data,
               b_coeff_size, b_score_data, b_score_size, latent_data,
               latent_size, x, tsquared_size, explained_data, explained_size);
    p = b_score_size[1] - 1;
    score_size[0] = 3;
    score_size[1] = b_score_size[1];
    wcol = -1;
    for (i = 0; i < 3; i++) {
      if (naninfo_nNaNsInRow[i] > 0) {
        for (j = 0; j <= p; j++) {
          score_data[i + 3 * j] = rtNaN;
        }
      } else {
        wcol++;
        for (j = 0; j <= p; j++) {
          score_data[i + 3 * j] = b_score_data[wcol + b_score_size[0] * j];
        }
      }
    }
  }

  coeff_size[0] = 3;
  coeff_size[1] = b_coeff_size[1];
  wcol = b_coeff_size[0] * b_coeff_size[1];
  if (0 <= wcol - 1) {
    memcpy(&coeff_data[0], &b_coeff_data[0], (unsigned int)(wcol * (int)sizeof
            (double)));
  }

  eig_size[0] = 3;
  eig_size[1] = score_size[1];
  wcol = score_size[0] * score_size[1];
  if (0 <= wcol - 1) {
    memcpy(&eig_data[0], &score_data[0], (unsigned int)(wcol * (int)sizeof
            (double)));
  }

  wcol = b_coeff_size[1];
  for (j = 0; j < wcol; j++) {
    xcol = 0.0;
    sgn = 1.0;
    absc_tmp = coeff_data[3 * j];
    absc = fabs(absc_tmp);
    if (absc > 0.0) {
      xcol = absc;
      sgn = absc_tmp;
      if (absc_tmp < 0.0) {
        sgn = -1.0;
      } else if (absc_tmp > 0.0) {
        sgn = 1.0;
      } else {
        if (absc_tmp == 0.0) {
          sgn = 0.0;
        }
      }
    }

    p = 1 + 3 * j;
    absc = fabs(coeff_data[p]);
    if (absc > xcol) {
      xcol = absc;
      sgn = coeff_data[p];
      if (coeff_data[p] < 0.0) {
        sgn = -1.0;
      } else if (coeff_data[p] > 0.0) {
        sgn = 1.0;
      } else {
        if (coeff_data[p] == 0.0) {
          sgn = 0.0;
        }
      }
    }

    i = 2 + 3 * j;
    if (fabs(coeff_data[i]) > xcol) {
      sgn = coeff_data[i];
      if (coeff_data[i] < 0.0) {
        sgn = -1.0;
      } else if (coeff_data[i] > 0.0) {
        sgn = 1.0;
      } else {
        if (coeff_data[i] == 0.0) {
          sgn = 0.0;
        }
      }
    }

    if (sgn < 0.0) {
      coeff_data[3 * j] = -absc_tmp;
      coeff_data[p] = -coeff_data[p];
      coeff_data[i] = -coeff_data[i];
      eig_data[3 * j] = -eig_data[3 * j];
      eig_data[p] = -eig_data[p];
      eig_data[i] = -eig_data[i];
    }
  }
}

/* End of code generation (pca_test.c) */
