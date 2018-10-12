/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Compress_emxutil.h
 *
 * Code generation for function 'Compress_emxutil'
 *
 */

#ifndef COMPRESS_EMXUTIL_H
#define COMPRESS_EMXUTIL_H

/* Include files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "Compress_types.h"

/* Function Declarations */
extern void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int oldNumel);
extern void emxFree_real_T(emxArray_real_T **pEmxArray);
extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#endif

/* End of code generation (Compress_emxutil.h) */
