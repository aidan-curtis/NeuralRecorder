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
#include "rt_nonfinite.h"
#include "pca_test.h"
#include "main.h"
#include "pca_test_terminate.h"
#include "pca_test_initialize.h"

/* Function Declarations */
static void argInit_3x3_real_T(double result[9]);
static double argInit_real_T(void);
static void main_pca_test(void);

/* Function Definitions */
static void argInit_3x3_real_T(double result[9])
{
  int idx0;
  double result_tmp;

  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 3; idx0++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result_tmp = argInit_real_T();
    result[idx0] = result_tmp;

    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result[idx0 + 3] = result_tmp;

    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result[idx0 + 6] = argInit_real_T();
  }
}

static double argInit_real_T(void)
{
  return 0.0;
}

static void main_pca_test(void)
{
  double dv0[9];
  double coeff_data[9];
  int coeff_size[2];
  double eig_data[9];
  int eig_size[2];

  /* Initialize function 'pca_test' input arguments. */
  /* Initialize function input argument 'X'. */
  /* Call the entry-point 'pca_test'. */
  argInit_3x3_real_T(dv0);
  pca_test(dv0, coeff_data, coeff_size, eig_data, eig_size);
}

int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;

  /* Initialize the application.
     You do not need to do this more than one time. */
  pca_test_initialize();

  /* Invoke the entry-point functions.
     You can call entry-point functions multiple times. */
  main_pca_test();

  /* Terminate the application.
     You do not need to do this more than one time. */
  pca_test_terminate();
  return 0;
}

/* End of code generation (main.c) */
