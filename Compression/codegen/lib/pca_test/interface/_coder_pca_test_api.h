/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_pca_test_api.h
 *
 * Code generation for function '_coder_pca_test_api'
 *
 */

#ifndef _CODER_PCA_TEST_API_H
#define _CODER_PCA_TEST_API_H

/* Include files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_pca_test_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void pca_test(real_T X[9], real_T coeff_data[], int32_T coeff_size[2],
                     real_T eig_data[], int32_T eig_size[2]);
extern void pca_test_api(const mxArray * const prhs[1], int32_T nlhs, const
  mxArray *plhs[2]);
extern void pca_test_atexit(void);
extern void pca_test_initialize(void);
extern void pca_test_terminate(void);
extern void pca_test_xil_terminate(void);

#endif

/* End of code generation (_coder_pca_test_api.h) */
