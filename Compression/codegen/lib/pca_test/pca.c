/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * pca.c
 *
 * Code generation for function 'pca'
 *
 */

/* Include files */
#include <math.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "pca_test.h"
#include "pca.h"
#include "sum.h"
#include "eps.h"
#include "xrot.h"
#include "xrotg.h"
#include "sqrt.h"
#include "xswap.h"
#include "xaxpy.h"
#include "xdotc.h"
#include "xnrm2.h"

/* Function Declarations */
static void localTSquared(const double score[9], const double latent[3], int DOF,
  double tsquared[3]);

/* Function Definitions */
static void localTSquared(const double score[9], const double latent[3], int DOF,
  double tsquared[3])
{
  int q;
  double sqrtlj;
  int j;
  double standScores_data[9];
  double tsquared_tmp;
  int standScores_data_tmp;
  if (DOF > 1) {
    if (DOF > 3) {
      q = DOF;
    } else {
      q = 3;
    }

    sqrtlj = (double)q * eps(latent[0]);
    q = 0;
    if (latent[0] > sqrtlj) {
      q = 1;
    }

    if (latent[1] > sqrtlj) {
      q++;
    }

    if (latent[2] > sqrtlj) {
      q++;
    }
  } else {
    q = 0;
  }

  tsquared[0] = 0.0;
  tsquared[1] = 0.0;
  tsquared[2] = 0.0;
  for (j = 0; j < q; j++) {
    sqrtlj = sqrt(latent[j]);
    standScores_data[3 * j] = score[3 * j] / sqrtlj;
    tsquared_tmp = standScores_data[3 * j];
    tsquared[0] += tsquared_tmp * tsquared_tmp;
    standScores_data_tmp = 1 + 3 * j;
    standScores_data[standScores_data_tmp] = score[standScores_data_tmp] /
      sqrtlj;
    tsquared[1] += standScores_data[standScores_data_tmp] *
      standScores_data[standScores_data_tmp];
    standScores_data_tmp = 2 + 3 * j;
    standScores_data[standScores_data_tmp] = score[standScores_data_tmp] /
      sqrtlj;
    tsquared[2] += standScores_data[standScores_data_tmp] *
      standScores_data[standScores_data_tmp];
  }
}

void b_localSVD(double x_data[], int x_size[2], int DOF, const double
                Weights_data[], const int Weights_size[1], double coeffOut_data[],
                int coeffOut_size[2], double scoreOut_data[], int scoreOut_size
                [2], double latentOut_data[], int latentOut_size[1], double
                tsquared_data[], int tsquared_size[2], double explained_data[],
                int explained_size[1])
{
  int OmegaSqrt_size[1];
  double OmegaSqrt_data[3];
  int nrows;
  int qjj;
  int m;
  int nmq;
  int nmqp1;
  double standScores_data[9];
  int n;
  double s_data[3];
  double e[3];
  double work_data[3];
  int score_size_idx_0;
  int loop_ub_tmp;
  double score_data[9];
  double Vf[9];
  int y;
  int i0;
  int q;
  int qp1;
  double coeff_data[9];
  int qq;
  boolean_T apply_transform;
  double nrm;
  int kase;
  int k;
  int ii;
  double rt;
  double snorm;
  boolean_T exitg1;
  double scale;
  double sm;
  double sqds;
  double b;
  OmegaSqrt_size[0] = Weights_size[0];
  if (0 <= Weights_size[0] - 1) {
    memcpy(&OmegaSqrt_data[0], &Weights_data[0], (unsigned int)(Weights_size[0] *
            (int)sizeof(double)));
  }

  c_sqrt(OmegaSqrt_data, OmegaSqrt_size);
  nrows = x_size[0] - 1;
  for (qjj = 0; qjj < 3; qjj++) {
    for (nmq = 0; nmq <= nrows; nmq++) {
      nmqp1 = nmq + x_size[0] * qjj;
      x_data[nmqp1] *= OmegaSqrt_data[nmq];
    }
  }

  m = x_size[0];
  nmqp1 = x_size[0] * x_size[1];
  if (0 <= nmqp1 - 1) {
    memcpy(&standScores_data[0], &x_data[0], (unsigned int)(nmqp1 * (int)sizeof
            (double)));
  }

  n = x_size[0];
  nmqp1 = x_size[0] + 1;
  if (nmqp1 >= 3) {
    nmqp1 = 3;
  }

  nmqp1 = (signed char)nmqp1;
  if (0 <= nmqp1 - 1) {
    memset(&s_data[0], 0, (unsigned int)(nmqp1 * (int)sizeof(double)));
  }

  e[0] = 0.0;
  e[1] = 0.0;
  e[2] = 0.0;
  nmqp1 = (signed char)x_size[0];
  if (0 <= nmqp1 - 1) {
    memset(&work_data[0], 0, (unsigned int)(nmqp1 * (int)sizeof(double)));
  }

  score_size_idx_0 = (signed char)x_size[0];
  loop_ub_tmp = (signed char)x_size[0] * (signed char)x_size[0];
  if (0 <= loop_ub_tmp - 1) {
    memset(&score_data[0], 0, (unsigned int)(loop_ub_tmp * (int)sizeof(double)));
  }

  memset(&Vf[0], 0, 9U * sizeof(double));
  if (x_size[0] == 0) {
    Vf[0] = 1.0;
    Vf[4] = 1.0;
    Vf[8] = 1.0;
  } else {
    if (x_size[0] > 1) {
      y = x_size[0] - 1;
    } else {
      y = 0;
    }

    if (y > 1) {
      i0 = y;
    } else {
      i0 = 1;
    }

    for (q = 0; q < i0; q++) {
      qp1 = q + 2;
      qq = (q + n * q) + 1;
      nmq = n - q;
      apply_transform = false;
      if (q + 1 <= y) {
        nrm = c_xnrm2(nmq, standScores_data, qq);
        if (nrm > 0.0) {
          apply_transform = true;
          if (standScores_data[qq - 1] < 0.0) {
            nrm = -nrm;
          }

          s_data[q] = nrm;
          if (fabs(nrm) >= 1.0020841800044864E-292) {
            nrm = 1.0 / nrm;
            kase = (qq + nmq) - 1;
            for (k = qq; k <= kase; k++) {
              standScores_data[k - 1] *= nrm;
            }
          } else {
            kase = (qq + nmq) - 1;
            for (k = qq; k <= kase; k++) {
              standScores_data[k - 1] /= s_data[q];
            }
          }

          standScores_data[qq - 1]++;
          s_data[q] = -s_data[q];
        } else {
          s_data[q] = 0.0;
        }
      }

      for (k = qp1; k < 4; k++) {
        qjj = q + n * (k - 1);
        if (apply_transform) {
          d_xaxpy(nmq, -(b_xdotc(nmq, standScores_data, qq, standScores_data,
                    qjj + 1) / standScores_data[q + m * q]), qq,
                  standScores_data, qjj + 1);
        }

        e[k - 1] = standScores_data[qjj];
      }

      if (q + 1 <= y) {
        for (ii = q + 1; ii <= n; ii++) {
          score_data[(ii + score_size_idx_0 * q) - 1] = standScores_data[(ii + m
            * q) - 1];
        }
      }

      if (q + 1 <= 1) {
        nrm = b_xnrm2(e, 2);
        if (nrm == 0.0) {
          e[0] = 0.0;
        } else {
          if (e[1] < 0.0) {
            e[0] = -nrm;
          } else {
            e[0] = nrm;
          }

          nrm = e[0];
          if (fabs(e[0]) >= 1.0020841800044864E-292) {
            nrm = 1.0 / e[0];
            for (k = qp1; k < 4; k++) {
              e[k - 1] *= nrm;
            }
          } else {
            for (k = qp1; k < 4; k++) {
              e[k - 1] /= nrm;
            }
          }

          e[1]++;
          e[0] = -e[0];
          if (2 <= n) {
            if (qp1 <= n) {
              memset(&work_data[qp1 + -1], 0, (unsigned int)(((n - qp1) + 1) *
                      (int)sizeof(double)));
            }

            for (k = qp1; k < 4; k++) {
              e_xaxpy(nmq - 1, e[k - 1], standScores_data, n * (k - 1) + 2,
                      work_data, 2);
            }

            for (k = qp1; k < 4; k++) {
              e_xaxpy(nmq - 1, -e[k - 1] / e[1], work_data, 2, standScores_data,
                      n * (k - 1) + 2);
            }
          }
        }

        for (ii = qp1; ii < 4; ii++) {
          Vf[ii - 1] = e[ii - 1];
        }
      }
    }

    if (3 < x_size[0] + 1) {
      m = 2;
    } else {
      m = x_size[0];
    }

    s_data[y] = standScores_data[y + x_size[0] * y];
    if (x_size[0] < m + 1) {
      s_data[m] = 0.0;
    }

    if (2 < m + 1) {
      e[1] = standScores_data[1 + x_size[0] * m];
    }

    e[m] = 0.0;
    if (y + 1 <= x_size[0]) {
      for (k = y + 1; k <= n; k++) {
        for (ii = 0; ii < n; ii++) {
          score_data[ii + score_size_idx_0 * (k - 1)] = 0.0;
        }

        score_data[(k + score_size_idx_0 * (k - 1)) - 1] = 1.0;
      }
    }

    for (q = y; q >= 1; q--) {
      qp1 = q + 1;
      kase = n - q;
      nmqp1 = kase + 1;
      qq = (q + n * (q - 1)) - 1;
      if (s_data[q - 1] != 0.0) {
        for (k = qp1; k <= n; k++) {
          qjj = q + n * (k - 1);
          d_xaxpy(kase + 1, -(b_xdotc(nmqp1, score_data, qq + 1, score_data, qjj)
                              / score_data[qq]), qq + 1, score_data, qjj);
        }

        for (ii = q; ii <= n; ii++) {
          kase = (ii + score_size_idx_0 * (q - 1)) - 1;
          score_data[kase] = -score_data[kase];
        }

        score_data[qq]++;
        if (0 <= q - 2) {
          score_data[score_size_idx_0 * (q - 1)] = 0.0;
        }
      } else {
        for (ii = 0; ii < n; ii++) {
          score_data[ii + score_size_idx_0 * (q - 1)] = 0.0;
        }

        score_data[qq] = 1.0;
      }
    }

    for (q = 2; q >= 0; q--) {
      if ((q + 1 <= 1) && (e[0] != 0.0)) {
        xaxpy(2, -(xdotc(2, Vf, 2, Vf, 5) / Vf[1]), 2, Vf, 5);
        xaxpy(2, -(xdotc(2, Vf, 2, Vf, 8) / Vf[1]), 2, Vf, 8);
      }

      Vf[3 * q] = 0.0;
      Vf[1 + 3 * q] = 0.0;
      Vf[2 + 3 * q] = 0.0;
      Vf[q + 3 * q] = 1.0;
    }

    for (q = 0; q <= m; q++) {
      if (s_data[q] != 0.0) {
        rt = fabs(s_data[q]);
        nrm = s_data[q] / rt;
        s_data[q] = rt;
        if (q + 1 < m + 1) {
          e[q] /= nrm;
        }

        if (q + 1 <= n) {
          nmqp1 = 1 + n * q;
          i0 = (nmqp1 + n) - 1;
          for (k = nmqp1; k <= i0; k++) {
            score_data[k - 1] *= nrm;
          }
        }
      }

      if ((q + 1 < m + 1) && (e[q] != 0.0)) {
        rt = fabs(e[q]);
        nrm = rt / e[q];
        e[q] = rt;
        s_data[q + 1] *= nrm;
        nmqp1 = 1 + 3 * (q + 1);
        i0 = nmqp1 + 2;
        for (k = nmqp1; k <= i0; k++) {
          Vf[k - 1] *= nrm;
        }
      }
    }

    qjj = m;
    nmq = 0;
    snorm = 0.0;
    for (ii = 0; ii <= m; ii++) {
      snorm = fmax(snorm, fmax(fabs(s_data[ii]), fabs(e[ii])));
    }

    while ((m + 1 > 0) && (nmq < 75)) {
      ii = m;
      exitg1 = false;
      while (!(exitg1 || (ii == 0))) {
        nrm = fabs(e[ii - 1]);
        if ((nrm <= 2.2204460492503131E-16 * (fabs(s_data[ii - 1]) + fabs
              (s_data[ii]))) || (nrm <= 1.0020841800044864E-292) || ((nmq > 20) &&
             (nrm <= 2.2204460492503131E-16 * snorm))) {
          e[ii - 1] = 0.0;
          exitg1 = true;
        } else {
          ii--;
        }
      }

      if (ii == m) {
        kase = 4;
      } else {
        nmqp1 = m + 1;
        kase = m + 1;
        exitg1 = false;
        while ((!exitg1) && (kase >= ii)) {
          nmqp1 = kase;
          if (kase == ii) {
            exitg1 = true;
          } else {
            nrm = 0.0;
            if (kase < m + 1) {
              nrm = fabs(e[kase - 1]);
            }

            if (kase > ii + 1) {
              nrm += fabs(e[kase - 2]);
            }

            rt = fabs(s_data[kase - 1]);
            if ((rt <= 2.2204460492503131E-16 * nrm) || (rt <=
                 1.0020841800044864E-292)) {
              s_data[kase - 1] = 0.0;
              exitg1 = true;
            } else {
              kase--;
            }
          }
        }

        if (nmqp1 == ii) {
          kase = 3;
        } else if (nmqp1 == m + 1) {
          kase = 1;
        } else {
          kase = 2;
          ii = nmqp1;
        }
      }

      switch (kase) {
       case 1:
        rt = e[m - 1];
        e[m - 1] = 0.0;
        nrm = e[0];
        for (k = m; k >= ii + 1; k--) {
          xrotg(&s_data[k - 1], &rt, &sqds, &sm);
          if (k > ii + 1) {
            rt = -sm * nrm;
            nrm *= sqds;
          }

          xrot(Vf, 1 + 3 * (k - 1), 1 + 3 * m, sqds, sm);
          e[0] = nrm;
        }
        break;

       case 2:
        rt = e[ii - 1];
        e[ii - 1] = 0.0;
        for (k = ii + 1; k <= m + 1; k++) {
          xrotg(&s_data[k - 1], &rt, &sqds, &sm);
          b = e[k - 1];
          rt = -sm * b;
          e[k - 1] = b * sqds;
          b_xrot(n, score_data, 1 + n * (k - 1), 1 + n * (ii - 1), sqds, sm);
        }
        break;

       case 3:
        nrm = s_data[m - 1];
        rt = e[m - 1];
        scale = fmax(fmax(fmax(fmax(fabs(s_data[m]), fabs(nrm)), fabs(rt)), fabs
                          (s_data[ii])), fabs(e[ii]));
        sm = s_data[m] / scale;
        nrm /= scale;
        rt /= scale;
        sqds = s_data[ii] / scale;
        b = ((nrm + sm) * (nrm - sm) + rt * rt) / 2.0;
        nrm = sm * rt;
        nrm *= nrm;
        if ((b != 0.0) || (nrm != 0.0)) {
          rt = b * b + nrm;
          b_sqrt(&rt);
          if (b < 0.0) {
            rt = -rt;
          }

          rt = nrm / (b + rt);
        } else {
          rt = 0.0;
        }

        rt += (sqds + sm) * (sqds - sm);
        nrm = sqds * (e[ii] / scale);
        for (k = ii + 1; k <= m; k++) {
          xrotg(&rt, &nrm, &sqds, &sm);
          if (k > ii + 1) {
            e[0] = rt;
          }

          b = e[k - 1];
          nrm = s_data[k - 1];
          e[k - 1] = sqds * b - sm * nrm;
          rt = sm * s_data[k];
          s_data[k] *= sqds;
          xrot(Vf, 1 + 3 * (k - 1), 1 + 3 * k, sqds, sm);
          s_data[k - 1] = sqds * nrm + sm * b;
          xrotg(&s_data[k - 1], &rt, &sqds, &sm);
          b = e[k - 1];
          rt = sqds * b + sm * s_data[k];
          s_data[k] = -sm * b + sqds * s_data[k];
          nrm = sm * e[k];
          e[k] *= sqds;
          if (k < n) {
            b_xrot(n, score_data, 1 + n * (k - 1), 1 + n * k, sqds, sm);
          }
        }

        e[m - 1] = rt;
        nmq++;
        break;

       default:
        if (s_data[ii] < 0.0) {
          s_data[ii] = -s_data[ii];
          nmqp1 = 1 + 3 * ii;
          i0 = nmqp1 + 2;
          for (k = nmqp1; k <= i0; k++) {
            Vf[k - 1] = -Vf[k - 1];
          }
        }

        qp1 = ii + 1;
        while ((ii + 1 < qjj + 1) && (s_data[ii] < s_data[qp1])) {
          rt = s_data[ii];
          s_data[ii] = s_data[qp1];
          s_data[qp1] = rt;
          xswap(Vf, 1 + 3 * ii, 1 + 3 * (ii + 1));
          if (ii + 1 < n) {
            b_xswap(n, score_data, 1 + n * ii, 1 + n * (ii + 1));
          }

          ii = qp1;
          qp1++;
        }

        nmq = 0;
        m--;
        break;
      }
    }
  }

  if (0 <= x_size[0] - 1) {
    memcpy(&work_data[0], &s_data[0], (unsigned int)(x_size[0] * (int)sizeof
            (double)));
  }

  for (qjj = 0; qjj < n; qjj++) {
    coeff_data[3 * qjj] = Vf[3 * qjj];
    nmqp1 = 1 + 3 * qjj;
    coeff_data[nmqp1] = Vf[nmqp1];
    nmqp1 = 2 + 3 * qjj;
    coeff_data[nmqp1] = Vf[nmqp1];
  }

  nmqp1 = (signed char)x_size[0];
  for (qjj = 0; qjj < nmqp1; qjj++) {
    for (nmq = 0; nmq <= nrows; nmq++) {
      kase = nmq + score_size_idx_0 * qjj;
      score_data[kase] = score_data[kase] / OmegaSqrt_data[nmq] * work_data[qjj];
    }
  }

  for (qjj = 0; qjj < nmqp1; qjj++) {
    work_data[qjj] = work_data[qjj] * work_data[qjj] / (double)DOF;
  }

  if ((signed char)x_size[0] == 0) {
    tsquared_size[0] = (signed char)x_size[0];
    tsquared_size[1] = (signed char)x_size[0];
    if (0 <= loop_ub_tmp - 1) {
      memcpy(&tsquared_data[0], &score_data[0], (unsigned int)(loop_ub_tmp *
              (int)sizeof(double)));
    }
  } else {
    if (DOF > 1) {
      if (DOF > 3) {
        y = DOF;
      } else {
        y = 3;
      }

      nrm = (double)y * eps(work_data[0]);
      q = 0;
      i0 = x_size[0];
      for (k = 0; k < i0; k++) {
        if (work_data[k] > nrm) {
          q++;
        }
      }
    } else {
      q = 0;
    }

    m = (signed char)x_size[0];
    kase = (signed char)x_size[0];
    tsquared_size[0] = (signed char)x_size[0];
    tsquared_size[1] = 1;
    nmqp1 = (signed char)x_size[0];
    if (0 <= nmqp1 - 1) {
      memset(&tsquared_data[0], 0, (unsigned int)(nmqp1 * (int)sizeof(double)));
    }

    for (qjj = 0; qjj < q; qjj++) {
      nrm = sqrt(work_data[qjj]);
      for (nmq = 0; nmq < m; nmq++) {
        standScores_data[nmq + kase * qjj] = score_data[nmq + score_size_idx_0 *
          qjj] / nrm;
        rt = standScores_data[nmq + kase * qjj];
        tsquared_data[nmq] += rt * rt;
      }
    }
  }

  if (DOF < 3) {
    kase = (signed char)x_size[0];
    if (DOF < kase) {
      kase = DOF;
    }

    scoreOut_size[0] = x_size[0];
    scoreOut_size[1] = kase;
    for (qjj = 0; qjj < kase; qjj++) {
      for (nmq = 0; nmq <= nrows; nmq++) {
        scoreOut_data[nmq + (nrows + 1) * qjj] = score_data[nmq +
          score_size_idx_0 * qjj];
      }
    }

    latentOut_size[0] = kase;
    if (0 <= kase - 1) {
      memcpy(&latentOut_data[0], &work_data[0], (unsigned int)(kase * (int)
              sizeof(double)));
    }

    coeffOut_size[0] = 3;
    coeffOut_size[1] = kase;
    for (qjj = 0; qjj < kase; qjj++) {
      coeffOut_data[3 * qjj] = coeff_data[3 * qjj];
      nmqp1 = 1 + 3 * qjj;
      coeffOut_data[nmqp1] = coeff_data[nmqp1];
      nmqp1 = 2 + 3 * qjj;
      coeffOut_data[nmqp1] = coeff_data[nmqp1];
    }
  } else {
    scoreOut_size[0] = (signed char)x_size[0];
    scoreOut_size[1] = (signed char)x_size[0];
    if (0 <= loop_ub_tmp - 1) {
      memcpy(&scoreOut_data[0], &score_data[0], (unsigned int)(loop_ub_tmp *
              (int)sizeof(double)));
    }

    latentOut_size[0] = x_size[0];
    if (0 <= x_size[0] - 1) {
      memcpy(&latentOut_data[0], &work_data[0], (unsigned int)(x_size[0] * (int)
              sizeof(double)));
    }

    coeffOut_size[0] = 3;
    coeffOut_size[1] = (signed char)x_size[0];
    nmqp1 = 3 * (signed char)x_size[0];
    if (0 <= nmqp1 - 1) {
      memcpy(&coeffOut_data[0], &coeff_data[0], (unsigned int)(nmqp1 * (int)
              sizeof(double)));
    }
  }

  nrm = sum(latentOut_data, latentOut_size);
  explained_size[0] = latentOut_size[0];
  nmqp1 = latentOut_size[0];
  for (i0 = 0; i0 < nmqp1; i0++) {
    explained_data[i0] = 100.0 * latentOut_data[i0] / nrm;
  }
}

void localSVD(const double x[9], int DOF, double coeffOut_data[], int
              coeffOut_size[2], double scoreOut_data[], int scoreOut_size[2],
              double latentOut_data[], int latentOut_size[1], double tsquared[3],
              double explained_data[], int explained_size[1])
{
  double coeff[9];
  double e[3];
  double work[3];
  double score[9];
  double Vf[9];
  boolean_T apply_transform;
  double nrm;
  double s[3];
  int kase;
  int qp1;
  int qjj;
  double r;
  int m;
  int q;
  int qq;
  double snorm;
  int exitg1;
  boolean_T exitg2;
  double scale;
  double sm;
  double sqds;
  double b;
  memcpy(&coeff[0], &x[0], 9U * sizeof(double));
  e[0] = 0.0;
  work[0] = 0.0;
  e[1] = 0.0;
  work[1] = 0.0;
  e[2] = 0.0;
  work[2] = 0.0;
  memset(&score[0], 0, 9U * sizeof(double));
  memset(&Vf[0], 0, 9U * sizeof(double));
  apply_transform = false;
  nrm = xnrm2(3, coeff, 1);
  if (nrm > 0.0) {
    apply_transform = true;
    if (coeff[0] < 0.0) {
      nrm = -nrm;
    }

    if (fabs(nrm) >= 1.0020841800044864E-292) {
      r = 1.0 / nrm;
      for (qp1 = 1; qp1 < 4; qp1++) {
        coeff[qp1 - 1] *= r;
      }
    } else {
      for (qp1 = 1; qp1 < 4; qp1++) {
        coeff[qp1 - 1] /= nrm;
      }
    }

    coeff[0]++;
    s[0] = -nrm;
  } else {
    s[0] = 0.0;
  }

  for (kase = 2; kase < 4; kase++) {
    qjj = 3 * (kase - 1);
    if (apply_transform) {
      xaxpy(3, -(xdotc(3, coeff, 1, coeff, qjj + 1) / coeff[0]), 1, coeff, qjj +
            1);
    }

    e[kase - 1] = coeff[qjj];
  }

  for (qp1 = 1; qp1 < 4; qp1++) {
    score[qp1 - 1] = coeff[qp1 - 1];
  }

  nrm = b_xnrm2(e, 2);
  if (nrm == 0.0) {
    e[0] = 0.0;
  } else {
    if (e[1] < 0.0) {
      e[0] = -nrm;
    } else {
      e[0] = nrm;
    }

    r = e[0];
    if (fabs(e[0]) >= 1.0020841800044864E-292) {
      r = 1.0 / e[0];
      for (qp1 = 2; qp1 < 4; qp1++) {
        e[qp1 - 1] *= r;
      }
    } else {
      for (qp1 = 2; qp1 < 4; qp1++) {
        e[qp1 - 1] /= r;
      }
    }

    e[1]++;
    e[0] = -e[0];
    for (qp1 = 2; qp1 < 4; qp1++) {
      work[qp1 - 1] = 0.0;
    }

    for (kase = 2; kase < 4; kase++) {
      b_xaxpy(2, e[kase - 1], coeff, 3 * (kase - 1) + 2, work, 2);
    }

    for (kase = 2; kase < 4; kase++) {
      c_xaxpy(2, -e[kase - 1] / e[1], work, 2, coeff, 3 * (kase - 1) + 2);
    }
  }

  for (qp1 = 2; qp1 < 4; qp1++) {
    Vf[qp1 - 1] = e[qp1 - 1];
  }

  apply_transform = false;
  nrm = xnrm2(2, coeff, 5);
  if (nrm > 0.0) {
    apply_transform = true;
    if (coeff[4] < 0.0) {
      nrm = -nrm;
    }

    if (fabs(nrm) >= 1.0020841800044864E-292) {
      r = 1.0 / nrm;
      for (qp1 = 5; qp1 < 7; qp1++) {
        coeff[qp1 - 1] *= r;
      }
    } else {
      for (qp1 = 5; qp1 < 7; qp1++) {
        coeff[qp1 - 1] /= nrm;
      }
    }

    coeff[4]++;
    s[1] = -nrm;
  } else {
    s[1] = 0.0;
  }

  for (kase = 3; kase < 4; kase++) {
    if (apply_transform) {
      xaxpy(2, -(xdotc(2, coeff, 5, coeff, 8) / coeff[4]), 5, coeff, 8);
    }
  }

  for (qp1 = 2; qp1 < 4; qp1++) {
    score[qp1 + 2] = coeff[qp1 + 2];
  }

  m = 1;
  s[2] = coeff[8];
  e[1] = coeff[7];
  e[2] = 0.0;
  score[6] = 0.0;
  score[7] = 0.0;
  score[8] = 1.0;
  for (q = 1; q >= 0; q--) {
    qp1 = q + 2;
    qq = q + 3 * q;
    if (s[q] != 0.0) {
      for (kase = qp1; kase < 4; kase++) {
        qjj = (q + 3 * (kase - 1)) + 1;
        xaxpy(3 - q, -(xdotc(3 - q, score, qq + 1, score, qjj) / score[qq]), qq
              + 1, score, qjj);
      }

      for (qp1 = q + 1; qp1 < 4; qp1++) {
        kase = (qp1 + 3 * q) - 1;
        score[kase] = -score[kase];
      }

      score[qq]++;
      if (0 <= q - 1) {
        score[3 * q] = 0.0;
      }
    } else {
      score[3 * q] = 0.0;
      score[1 + 3 * q] = 0.0;
      score[2 + 3 * q] = 0.0;
      score[qq] = 1.0;
    }
  }

  for (q = 2; q >= 0; q--) {
    if ((q + 1 <= 1) && (e[0] != 0.0)) {
      xaxpy(2, -(xdotc(2, Vf, 2, Vf, 5) / Vf[1]), 2, Vf, 5);
      xaxpy(2, -(xdotc(2, Vf, 2, Vf, 8) / Vf[1]), 2, Vf, 8);
    }

    Vf[3 * q] = 0.0;
    Vf[1 + 3 * q] = 0.0;
    Vf[2 + 3 * q] = 0.0;
    Vf[q + 3 * q] = 1.0;
  }

  qq = 0;
  snorm = 0.0;
  for (q = 0; q < 3; q++) {
    if (s[q] != 0.0) {
      nrm = fabs(s[q]);
      r = s[q] / nrm;
      s[q] = nrm;
      if (q + 1 < 3) {
        e[q] /= r;
      }

      kase = 3 * q;
      qjj = kase + 3;
      for (qp1 = kase + 1; qp1 <= qjj; qp1++) {
        score[qp1 - 1] *= r;
      }
    }

    if ((q + 1 < 3) && (e[q] != 0.0)) {
      nrm = fabs(e[q]);
      r = nrm / e[q];
      e[q] = nrm;
      s[q + 1] *= r;
      kase = 3 * (q + 1);
      qjj = kase + 3;
      for (qp1 = kase + 1; qp1 <= qjj; qp1++) {
        Vf[qp1 - 1] *= r;
      }
    }

    snorm = fmax(snorm, fmax(fabs(s[q]), fabs(e[q])));
  }

  while ((m + 2 > 0) && (qq < 75)) {
    qp1 = m;
    do {
      exitg1 = 0;
      q = qp1 + 1;
      if (qp1 + 1 == 0) {
        exitg1 = 1;
      } else {
        nrm = fabs(e[qp1]);
        if ((nrm <= 2.2204460492503131E-16 * (fabs(s[qp1]) + fabs(s[qp1 + 1]))) ||
            (nrm <= 1.0020841800044864E-292) || ((qq > 20) && (nrm <=
              2.2204460492503131E-16 * snorm))) {
          e[qp1] = 0.0;
          exitg1 = 1;
        } else {
          qp1--;
        }
      }
    } while (exitg1 == 0);

    if (qp1 + 1 == m + 1) {
      kase = 4;
    } else {
      qjj = m + 2;
      kase = m + 2;
      exitg2 = false;
      while ((!exitg2) && (kase >= qp1 + 1)) {
        qjj = kase;
        if (kase == qp1 + 1) {
          exitg2 = true;
        } else {
          nrm = 0.0;
          if (kase < m + 2) {
            nrm = fabs(e[kase - 1]);
          }

          if (kase > qp1 + 2) {
            nrm += fabs(e[kase - 2]);
          }

          r = fabs(s[kase - 1]);
          if ((r <= 2.2204460492503131E-16 * nrm) || (r <=
               1.0020841800044864E-292)) {
            s[kase - 1] = 0.0;
            exitg2 = true;
          } else {
            kase--;
          }
        }
      }

      if (qjj == qp1 + 1) {
        kase = 3;
      } else if (qjj == m + 2) {
        kase = 1;
      } else {
        kase = 2;
        q = qjj;
      }
    }

    switch (kase) {
     case 1:
      r = e[m];
      e[m] = 0.0;
      qjj = m + 1;
      nrm = e[0];
      for (qp1 = qjj; qp1 >= q + 1; qp1--) {
        xrotg(&s[qp1 - 1], &r, &sm, &sqds);
        if (qp1 > q + 1) {
          r = -sqds * nrm;
          nrm *= sm;
        }

        xrot(Vf, 1 + 3 * (qp1 - 1), 1 + 3 * (m + 1), sm, sqds);
        e[0] = nrm;
      }
      break;

     case 2:
      r = e[q - 1];
      e[q - 1] = 0.0;
      for (qp1 = q + 1; qp1 <= m + 2; qp1++) {
        xrotg(&s[qp1 - 1], &r, &sm, &sqds);
        b = e[qp1 - 1];
        r = -sqds * b;
        e[qp1 - 1] = b * sm;
        xrot(score, 1 + 3 * (qp1 - 1), 1 + 3 * (q - 1), sm, sqds);
      }
      break;

     case 3:
      qjj = m + 1;
      nrm = s[m + 1];
      scale = fmax(fmax(fmax(fmax(fabs(nrm), fabs(s[m])), fabs(e[m])), fabs(s[q])),
                   fabs(e[q]));
      sm = nrm / scale;
      nrm = s[m] / scale;
      r = e[m] / scale;
      sqds = s[q] / scale;
      b = ((nrm + sm) * (nrm - sm) + r * r) / 2.0;
      nrm = sm * r;
      nrm *= nrm;
      if ((b != 0.0) || (nrm != 0.0)) {
        r = b * b + nrm;
        b_sqrt(&r);
        if (b < 0.0) {
          r = -r;
        }

        r = nrm / (b + r);
      } else {
        r = 0.0;
      }

      r += (sqds + sm) * (sqds - sm);
      nrm = sqds * (e[q] / scale);
      for (qp1 = q + 1; qp1 <= qjj; qp1++) {
        xrotg(&r, &nrm, &sm, &sqds);
        if (qp1 > q + 1) {
          e[0] = r;
        }

        b = e[qp1 - 1];
        nrm = s[qp1 - 1];
        e[qp1 - 1] = sm * b - sqds * nrm;
        r = sqds * s[qp1];
        s[qp1] *= sm;
        xrot(Vf, 1 + 3 * (qp1 - 1), 1 + 3 * qp1, sm, sqds);
        s[qp1 - 1] = sm * nrm + sqds * b;
        xrotg(&s[qp1 - 1], &r, &sm, &sqds);
        b = e[qp1 - 1];
        r = sm * b + sqds * s[qp1];
        s[qp1] = -sqds * b + sm * s[qp1];
        nrm = sqds * e[qp1];
        e[qp1] *= sm;
        xrot(score, 1 + 3 * (qp1 - 1), 1 + 3 * qp1, sm, sqds);
      }

      e[m] = r;
      qq++;
      break;

     default:
      if (s[q] < 0.0) {
        s[q] = -s[q];
        kase = 3 * q;
        qjj = kase + 3;
        for (qp1 = kase + 1; qp1 <= qjj; qp1++) {
          Vf[qp1 - 1] = -Vf[qp1 - 1];
        }
      }

      qp1 = q + 1;
      while ((q + 1 < 3) && (s[q] < s[qp1])) {
        nrm = s[q];
        s[q] = s[qp1];
        s[qp1] = nrm;
        xswap(Vf, 1 + 3 * q, 1 + 3 * (q + 1));
        xswap(score, 1 + 3 * q, 1 + 3 * (q + 1));
        q = qp1;
        qp1++;
      }

      qq = 0;
      m--;
      break;
    }
  }

  for (qp1 = 0; qp1 < 3; qp1++) {
    e[qp1] = s[qp1];
    coeff[3 * qp1] = Vf[3 * qp1];
    kase = 1 + 3 * qp1;
    coeff[kase] = Vf[kase];
    kase = 2 + 3 * qp1;
    coeff[kase] = Vf[kase];
  }

  for (qjj = 0; qjj < 3; qjj++) {
    score[3 * qjj] *= e[qjj];
    kase = 1 + 3 * qjj;
    score[kase] *= e[qjj];
    kase = 2 + 3 * qjj;
    score[kase] *= e[qjj];
  }

  e[0] = e[0] * e[0] / (double)DOF;
  e[1] = e[1] * e[1] / (double)DOF;
  e[2] = e[2] * e[2] / (double)DOF;
  localTSquared(score, e, DOF, tsquared);
  if (DOF < 3) {
    scoreOut_size[0] = 3;
    scoreOut_size[1] = DOF;
    for (qjj = 0; qjj < DOF; qjj++) {
      scoreOut_data[3 * qjj] = score[3 * qjj];
      kase = 1 + 3 * qjj;
      scoreOut_data[kase] = score[kase];
      kase = 2 + 3 * qjj;
      scoreOut_data[kase] = score[kase];
    }

    latentOut_size[0] = DOF;
    if (0 <= DOF - 1) {
      memcpy(&latentOut_data[0], &e[0], (unsigned int)(DOF * (int)sizeof(double)));
    }

    coeffOut_size[0] = 3;
    coeffOut_size[1] = DOF;
    for (qjj = 0; qjj < DOF; qjj++) {
      coeffOut_data[3 * qjj] = coeff[3 * qjj];
      kase = 1 + 3 * qjj;
      coeffOut_data[kase] = coeff[kase];
      kase = 2 + 3 * qjj;
      coeffOut_data[kase] = coeff[kase];
    }
  } else {
    scoreOut_size[0] = 3;
    scoreOut_size[1] = 3;
    memcpy(&scoreOut_data[0], &score[0], 9U * sizeof(double));
    latentOut_size[0] = 3;
    latentOut_data[0] = e[0];
    latentOut_data[1] = e[1];
    latentOut_data[2] = e[2];
    coeffOut_size[0] = 3;
    coeffOut_size[1] = 3;
    memcpy(&coeffOut_data[0], &coeff[0], 9U * sizeof(double));
  }

  nrm = sum(latentOut_data, latentOut_size);
  explained_size[0] = latentOut_size[0];
  kase = latentOut_size[0];
  for (qjj = 0; qjj < kase; qjj++) {
    explained_data[qjj] = 100.0 * latentOut_data[qjj] / nrm;
  }
}

/* End of code generation (pca.c) */
