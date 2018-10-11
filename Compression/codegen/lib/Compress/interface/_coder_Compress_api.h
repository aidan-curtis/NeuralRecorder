/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_Compress_api.h
 *
 * Code generation for function '_coder_Compress_api'
 *
 */

#ifndef _CODER_COMPRESS_API_H
#define _CODER_COMPRESS_API_H

/* Include files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_Compress_api.h"

/* Type Definitions */
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  real_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_real_T*/

#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T

typedef struct emxArray_real_T emxArray_real_T;

#endif                                 /*typedef_emxArray_real_T*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void Compress(emxArray_real_T *input_signal, emxArray_real_T *Phi,
                     emxArray_real_T *y);
extern void Compress_api(const mxArray * const prhs[2], int32_T nlhs, const
  mxArray *plhs[1]);
extern void Compress_atexit(void);
extern void Compress_initialize(void);
extern void Compress_terminate(void);
extern void Compress_xil_terminate(void);

#endif

/* End of code generation (_coder_Compress_api.h) */
