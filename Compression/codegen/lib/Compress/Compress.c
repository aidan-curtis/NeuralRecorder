/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Compress.c
 *
 * Code generation for function 'Compress'
 *
 */

/* Include files */
#include "Compress.h"
#include "Compress_emxutil.h"

/* Function Definitions */
void Compress(const emxArray_real_T *input_signal, const emxArray_real_T *Phi,
              emxArray_real_T *y)
{
  emxArray_real_T *b;
  int loop_ub;
  int i0;
  emxArray_real_T *C;
  int m;
  int inner;
  int k;
  int aoffset;
  emxInit_real_T(&b, 1);

  /* COMPRESS Summary of this function goes here */
  loop_ub = input_signal->size[1];
  i0 = b->size[0];
  b->size[0L] = loop_ub;
  emxEnsureCapacity_real_T(b, i0);
  for (i0 = 0; i0 < loop_ub; i0++) {
    b->data[i0] = input_signal->data[i0];
  }

  emxInit_real_T(&C, 1);
  if ((Phi->size[1] == 1) || (b->size[0] == 1)) {
    i0 = y->size[0];
    y->size[0L] = Phi->size[0L];
    emxEnsureCapacity_real_T(y, i0);
    loop_ub = Phi->size[0L];
    for (i0 = 0; i0 < loop_ub; i0++) {
      y->data[i0] = 0.0;
      k = Phi->size[1L];
      for (aoffset = 0; aoffset < k; aoffset++) {
        y->data[i0] += Phi->data[i0 + Phi->size[0] * aoffset] * b->data[aoffset];
      }
    }
  } else {
    m = Phi->size[0];
    inner = Phi->size[1];
    i0 = C->size[0];
    C->size[0] = Phi->size[0];
    emxEnsureCapacity_real_T(C, i0);
    for (loop_ub = 0; loop_ub < m; loop_ub++) {
      C->data[loop_ub] = 0.0;
    }

    for (k = 0; k < inner; k++) {
      aoffset = k * m;
      for (loop_ub = 0; loop_ub < m; loop_ub++) {
        C->data[loop_ub] += b->data[k] * Phi->data[aoffset + loop_ub];
      }
    }

    i0 = y->size[0];
    y->size[0L] = C->size[0L];
    emxEnsureCapacity_real_T(y, i0);
    loop_ub = C->size[0L];
    for (i0 = 0; i0 < loop_ub; i0++) {
      y->data[i0] = C->data[i0];
    }
  }

  emxFree_real_T(&C);
  emxFree_real_T(&b);
}

/* End of code generation (Compress.c) */
