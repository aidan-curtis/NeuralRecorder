/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * main.c
 *
 * Code generation for function 'main'
 *
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/
/* Include files */
#include "Compress.h"
#include "main.h"
#include "Compress_terminate.h"
#include "Compress_emxAPI.h"
#include "Compress_initialize.h"

/* Function Declarations */
static emxArray_real_T *argInit_1xUnbounded_real_T(void);
static double argInit_real_T(void);
static emxArray_real_T *c_argInit_UnboundedxUnbounded_r(void);
static void main_Compress(void);

/* Function Definitions */
static emxArray_real_T *argInit_1xUnbounded_real_T(void)
{
  emxArray_real_T *result;
  int idx1;

  /* Set the size of the array.
     Change this size to the value that the application requires. */
  result = emxCreate_real_T(1, 2);

  /* Loop over the array to initialize each element. */
  for (idx1 = 0; idx1 < result->size[1UL]; idx1++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result->data[idx1] = argInit_real_T();
  }

  return result;
}

static double argInit_real_T(void)
{
  return 0.0;
}

static emxArray_real_T *c_argInit_UnboundedxUnbounded_r(void)
{
  emxArray_real_T *result;
  int idx0;
  int idx1;

  /* Set the size of the array.
     Change this size to the value that the application requires. */
  result = emxCreate_real_T(2, 2);

  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < result->size[0UL]; idx0++) {
    for (idx1 = 0; idx1 < result->size[1UL]; idx1++) {
      /* Set the value of the array element.
         Change this value to the value that the application requires. */
      result->data[idx0 + result->size[0] * idx1] = argInit_real_T();
    }
  }

  return result;
}

static void main_Compress(void)
{
  emxArray_real_T *y;
  emxArray_real_T *input_signal;
  emxArray_real_T *Phi;
  emxInitArray_real_T(&y, 1);

  /* Initialize function 'Compress' input arguments. */
  /* Initialize function input argument 'input_signal'. */
  input_signal = argInit_1xUnbounded_real_T();

  /* Initialize function input argument 'Phi'. */
  Phi = c_argInit_UnboundedxUnbounded_r();

  /* Call the entry-point 'Compress'. */
  Compress(input_signal, Phi, y);
  emxDestroyArray_real_T(y);
  emxDestroyArray_real_T(Phi);
  emxDestroyArray_real_T(input_signal);
}

int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;

  /* Initialize the application.
     You do not need to do this more than one time. */
  Compress_initialize();

  /* Invoke the entry-point functions.
     You can call entry-point functions multiple times. */
  main_Compress();

  /* Terminate the application.
     You do not need to do this more than one time. */
  Compress_terminate();
  return 0;
}

/* End of code generation (main.c) */
