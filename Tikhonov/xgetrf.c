/*
 * File: xgetrf.c
 *
 * MATLAB Coder version            : 3.0
 * C/C++ source code generated on  : 10-Jul-2017 15:21:32
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "Tikhonov.h"
#include "xgetrf.h"
#include "xswap.h"

/* Function Definitions */

/*
 * Arguments    : double A[160000]
 *                int ipiv[400]
 * Return Type  : void
 */
void xgetrf(double A[160000], int ipiv[400])
{
  int i1;
  int j;
  int c;
  int jA;
  int ix;
  double smax;
  int jy;
  double s;
  int b_j;
  int ijA;
  for (i1 = 0; i1 < 400; i1++) {
    ipiv[i1] = 1 + i1;
  }

  for (j = 0; j < 399; j++) {
    c = j * 401;
    jA = 1;
    ix = c;
    smax = fabs(A[c]);
    for (jy = 2; jy <= 400 - j; jy++) {
      ix++;
      s = fabs(A[ix]);
      if (s > smax) {
        jA = jy;
        smax = s;
      }
    }

    if (A[(c + jA) - 1] != 0.0) {
      if (jA - 1 != 0) {
        ipiv[j] = j + jA;
        xswap(A, j + 1, j + jA);
      }

      i1 = (c - j) + 400;
      for (jA = c + 1; jA + 1 <= i1; jA++) {
        A[jA] /= A[c];
      }
    }

    jA = c;
    jy = c + 400;
    for (b_j = 1; b_j <= 399 - j; b_j++) {
      smax = A[jy];
      if (A[jy] != 0.0) {
        ix = c + 1;
        i1 = (jA - j) + 800;
        for (ijA = 401 + jA; ijA + 1 <= i1; ijA++) {
          A[ijA] += A[ix] * -smax;
          ix++;
        }
      }

      jy += 400;
      jA += 400;
    }
  }
}

/*
 * File trailer for xgetrf.c
 *
 * [EOF]
 */