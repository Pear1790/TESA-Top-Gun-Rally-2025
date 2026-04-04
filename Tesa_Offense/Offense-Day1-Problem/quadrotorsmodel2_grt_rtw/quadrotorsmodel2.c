/*
 * quadrotorsmodel2.c
 *
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 *
 * Code generation for model "quadrotorsmodel2".
 *
 * Model version              : 3.108
 * Simulink Coder version : 25.2 (R2025b) 28-Jul-2025
 * C source code generated on : Wed Nov 12 14:43:03 2025
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "quadrotorsmodel2.h"
#include "rtwtypes.h"
#include <string.h>
#include <math.h>
#include <emmintrin.h>
#include "quadrotorsmodel2_private.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"

/* Block signals (default storage) */
B_quadrotorsmodel2_T quadrotorsmodel2_B;

/* Continuous states */
X_quadrotorsmodel2_T quadrotorsmodel2_X;

/* Disabled State Vector */
XDis_quadrotorsmodel2_T quadrotorsmodel2_XDis;

/* Block states (default storage) */
DW_quadrotorsmodel2_T quadrotorsmodel2_DW;

/* External inputs (root inport signals with default storage) */
ExtU_quadrotorsmodel2_T quadrotorsmodel2_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_quadrotorsmodel2_T quadrotorsmodel2_Y;

/* Real-time model */
static RT_MODEL_quadrotorsmodel2_T quadrotorsmodel2_M_;
RT_MODEL_quadrotorsmodel2_T *const quadrotorsmodel2_M = &quadrotorsmodel2_M_;

/* Forward declaration for local functions */
static real_T quadrotorsmodel2_norm(const real_T x[3]);
static void quadrotorsmodel2_xzlascl(real_T cfrom, real_T cto, real_T A[9]);
static real_T quadrotorsmodel2_xnrm2(int32_T n, const real_T x[9], int32_T ix0);
static real_T quadrotorsmodel2_xzlarfg(int32_T n, real_T *alpha1, real_T x[9],
  int32_T ix0);
static void quadrotorsmodel2_xzgehrd(real_T a[9], int32_T ilo, int32_T ihi);
static real_T quadrotorsmodel2_xnrm2_n(int32_T n, const real_T x[3]);
static void quadrotorsmodel2_xdlanv2(real_T *a, real_T *b, real_T *c, real_T *d,
  real_T *rt1r, real_T *rt1i, real_T *rt2r, real_T *rt2i, real_T *cs, real_T *sn);
static void quadrotorsmodel2_xdlahqr(int32_T ilo, int32_T ihi, real_T h[9],
  real_T *z, int32_T *info, real_T wr[3], real_T wi[3]);
static void quadrotorsmodel2_xzlascl_d(real_T cfrom, real_T cto, int32_T m,
  real_T A[3], int32_T iA0);
static void quadrotorsmodel2_eigStandard(const real_T A[9], creal_T V[3]);
static void quadrotorsmodel2_xzlascl_dx(real_T cfrom, real_T cto, int32_T m,
  real_T A[2], int32_T iA0);
static void quadrotorsmodel2_xdlaev2(real_T a, real_T b, real_T c, real_T *rt1,
  real_T *rt2);
static void quadrotorsmodel2_insertionsort(real_T x[3], int32_T xstart, int32_T
  xend);
static int32_T quadrotorsmodel2_xdsterf(real_T d[3], real_T e[2]);
static void quadrotorsmodel2_eig(const real_T A[9], creal_T V[3]);
int32_T div_nde_s32_floor(int32_T numerator, int32_T denominator)
{
  return (((numerator < 0) != (denominator < 0)) && (numerator % denominator !=
           0) ? -1 : 0) + numerator / denominator;
}

/*
 * This function updates continuous states using the ODE3 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  /* Solver Matrices */
  static const real_T rt_ODE3_A[3] = {
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3] = {
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE3_IntgData *id = (ODE3_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 18;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  quadrotorsmodel2_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  quadrotorsmodel2_step();
  quadrotorsmodel2_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  quadrotorsmodel2_step();
  quadrotorsmodel2_derivatives();

  /* tnew = t + hA(3);
     ynew = y + f*hB(:,3); */
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Function for MATLAB Function: '<Root>/MATLAB Function' */
static real_T quadrotorsmodel2_norm(const real_T x[3])
{
  real_T absxk;
  real_T scale;
  real_T t;
  real_T y;
  scale = 3.3121686421112381E-170;
  absxk = fabs(x[0]);
  if (absxk > 3.3121686421112381E-170) {
    y = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    y = t * t;
  }

  absxk = fabs(x[1]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  absxk = fabs(x[2]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  y = scale * sqrt(y);
  if (rtIsNaN(y)) {
    int32_T k;
    k = 0;
    int32_T exitg1;
    do {
      exitg1 = 0;
      if (k < 3) {
        if (rtIsNaN(x[k])) {
          exitg1 = 1;
        } else {
          k++;
        }
      } else {
        y = (rtInf);
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }

  return y;
}

/* Function for MATLAB Function: '<Root>/MATLAB Function' */
static void quadrotorsmodel2_xzlascl(real_T cfrom, real_T cto, real_T A[9])
{
  real_T cfromc;
  real_T ctoc;
  int32_T j;
  boolean_T notdone;
  cfromc = cfrom;
  ctoc = cto;
  notdone = true;
  while (notdone) {
    real_T cfrom1;
    real_T cto1;
    real_T mul;
    cfrom1 = cfromc * 2.0041683600089728E-292;
    cto1 = ctoc / 4.9896007738368E+291;
    if ((fabs(cfrom1) > fabs(ctoc)) && (ctoc != 0.0)) {
      mul = 2.0041683600089728E-292;
      cfromc = cfrom1;
    } else if (fabs(cto1) > fabs(cfromc)) {
      mul = 4.9896007738368E+291;
      ctoc = cto1;
    } else {
      mul = ctoc / cfromc;
      notdone = false;
    }

    for (j = 0; j < 3; j++) {
      int32_T tmp;
      A[j * 3] *= mul;
      tmp = j * 3 + 1;
      A[tmp] *= mul;
      tmp = j * 3 + 2;
      A[tmp] *= mul;
    }
  }
}

/* Function for MATLAB Function: '<Root>/MATLAB Function' */
static real_T quadrotorsmodel2_xnrm2(int32_T n, const real_T x[9], int32_T ix0)
{
  real_T y;
  int32_T k;
  y = 0.0;
  if (n >= 1) {
    if (n == 1) {
      y = fabs(x[ix0 - 1]);
    } else {
      real_T scale;
      int32_T kend;
      scale = 3.3121686421112381E-170;
      kend = (ix0 + n) - 1;
      for (k = ix0; k <= kend; k++) {
        real_T absxk;
        absxk = fabs(x[k - 1]);
        if (absxk > scale) {
          real_T t;
          t = scale / absxk;
          y = y * t * t + 1.0;
          scale = absxk;
        } else {
          real_T t;
          t = absxk / scale;
          y += t * t;
        }
      }

      y = scale * sqrt(y);
      if (rtIsNaN(y)) {
        k = ix0;
        int32_T exitg1;
        do {
          exitg1 = 0;
          if (k <= kend) {
            if (rtIsNaN(x[k - 1])) {
              exitg1 = 1;
            } else {
              k++;
            }
          } else {
            y = (rtInf);
            exitg1 = 1;
          }
        } while (exitg1 == 0);
      }
    }
  }

  return y;
}

real_T rt_hypotd_snf(real_T u0, real_T u1)
{
  real_T a;
  real_T b;
  real_T y;
  a = fabs(u0);
  b = fabs(u1);
  if (a < b) {
    a /= b;
    y = sqrt(a * a + 1.0) * b;
  } else if (a > b) {
    b /= a;
    y = sqrt(b * b + 1.0) * a;
  } else if (rtIsNaN(b)) {
    y = (rtNaN);
  } else {
    y = a * 1.4142135623730951;
  }

  return y;
}

/* Function for MATLAB Function: '<Root>/MATLAB Function' */
static real_T quadrotorsmodel2_xzlarfg(int32_T n, real_T *alpha1, real_T x[9],
  int32_T ix0)
{
  __m128d tmp;
  real_T a;
  real_T tau;
  real_T xnorm;
  int32_T c;
  int32_T d;
  int32_T knt;
  int32_T scalarLB;
  int32_T vectorUB;
  tau = 0.0;
  if (n > 0) {
    xnorm = quadrotorsmodel2_xnrm2(n - 1, x, ix0);
    if (xnorm != 0.0) {
      xnorm = rt_hypotd_snf(*alpha1, xnorm);
      if (*alpha1 >= 0.0) {
        xnorm = -xnorm;
      }

      if (fabs(xnorm) < 1.0020841800044864E-292) {
        knt = 0;
        do {
          knt++;
          d = (ix0 + n) - 2;
          scalarLB = ((((d - ix0) + 1) / 2) << 1) + ix0;
          vectorUB = scalarLB - 2;
          for (c = ix0; c <= vectorUB; c += 2) {
            tmp = _mm_loadu_pd(&x[c - 1]);
            _mm_storeu_pd(&x[c - 1], _mm_mul_pd(tmp, _mm_set1_pd
              (9.9792015476736E+291)));
          }

          for (c = scalarLB; c <= d; c++) {
            x[c - 1] *= 9.9792015476736E+291;
          }

          xnorm *= 9.9792015476736E+291;
          *alpha1 *= 9.9792015476736E+291;
        } while ((fabs(xnorm) < 1.0020841800044864E-292) && (knt < 20));

        xnorm = rt_hypotd_snf(*alpha1, quadrotorsmodel2_xnrm2(n - 1, x, ix0));
        if (*alpha1 >= 0.0) {
          xnorm = -xnorm;
        }

        tau = (xnorm - *alpha1) / xnorm;
        a = 1.0 / (*alpha1 - xnorm);
        vectorUB = scalarLB - 2;
        for (c = ix0; c <= vectorUB; c += 2) {
          tmp = _mm_loadu_pd(&x[c - 1]);
          _mm_storeu_pd(&x[c - 1], _mm_mul_pd(tmp, _mm_set1_pd(a)));
        }

        for (c = scalarLB; c <= d; c++) {
          x[c - 1] *= a;
        }

        for (c = 0; c < knt; c++) {
          xnorm *= 1.0020841800044864E-292;
        }

        *alpha1 = xnorm;
      } else {
        tau = (xnorm - *alpha1) / xnorm;
        a = 1.0 / (*alpha1 - xnorm);
        c = (ix0 + n) - 2;
        scalarLB = ((((c - ix0) + 1) / 2) << 1) + ix0;
        vectorUB = scalarLB - 2;
        for (knt = ix0; knt <= vectorUB; knt += 2) {
          tmp = _mm_loadu_pd(&x[knt - 1]);
          _mm_storeu_pd(&x[knt - 1], _mm_mul_pd(tmp, _mm_set1_pd(a)));
        }

        for (knt = scalarLB; knt <= c; knt++) {
          x[knt - 1] *= a;
        }

        *alpha1 = xnorm;
      }
    }
  }

  return tau;
}

/* Function for MATLAB Function: '<Root>/MATLAB Function' */
static void quadrotorsmodel2_xzgehrd(real_T a[9], int32_T ilo, int32_T ihi)
{
  __m128d tmp;
  __m128d tmp_0;
  real_T work[3];
  real_T tau[2];
  real_T alpha1;
  real_T tau_0;
  real_T temp;
  int32_T alpha1_tmp;
  int32_T c_ix;
  int32_T exitg1;
  int32_T i;
  int32_T im1n;
  int32_T in;
  int32_T ix;
  int32_T jy;
  int32_T lastc;
  int32_T lastv;
  int32_T rowleft;
  int32_T tau_tmp;
  int32_T work_tmp;
  boolean_T exitg2;
  if ((ihi - ilo) + 1 > 1) {
    if ((uint8_T)(ilo - 1) - 1 >= 0) {
      memset(&tau[0], 0, (uint8_T)(ilo - 1) * sizeof(real_T));
    }

    for (i = ihi; i < 3; i++) {
      tau[i - 1] = 0.0;
    }

    work[0] = 0.0;
    work[1] = 0.0;
    work[2] = 0.0;
    for (i = ilo; i < ihi; i++) {
      jy = (i - 1) * 3;
      in = i * 3;
      alpha1_tmp = jy + i;
      alpha1 = a[alpha1_tmp];
      tau_tmp = ihi - i;
      tau_0 = quadrotorsmodel2_xzlarfg(tau_tmp, &alpha1, a, jy + 3);
      tau[i - 1] = tau_0;
      a[alpha1_tmp] = 1.0;
      lastv = tau_tmp;
      if (tau_0 != 0.0) {
        lastc = alpha1_tmp + tau_tmp;
        while ((lastv > 0) && (a[lastc - 1] == 0.0)) {
          lastv--;
          lastc--;
        }

        lastc = ihi;
        exitg2 = false;
        while ((!exitg2) && (lastc > 0)) {
          rowleft = in + lastc;
          jy = rowleft;
          do {
            exitg1 = 0;
            if (jy <= (lastv - 1) * 3 + rowleft) {
              if (a[jy - 1] != 0.0) {
                exitg1 = 1;
              } else {
                jy += 3;
              }
            } else {
              lastc--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);

          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        lastv = 0;
        lastc = 0;
      }

      if (lastv > 0) {
        if (lastc != 0) {
          memset(&work[0], 0, (uint8_T)lastc * sizeof(real_T));
          ix = alpha1_tmp;
          c_ix = ((lastv - 1) * 3 + in) + 1;
          for (rowleft = in + 1; rowleft <= c_ix; rowleft += 3) {
            im1n = rowleft + lastc;
            for (jy = rowleft; jy < im1n; jy++) {
              work_tmp = jy - rowleft;
              work[work_tmp] += a[jy - 1] * a[ix];
            }

            ix++;
          }
        }

        tau_0 = -tau[i - 1];
        if (!(tau_0 == 0.0)) {
          rowleft = in + 1;
          ix = (uint8_T)lastv;
          for (lastv = 0; lastv < ix; lastv++) {
            temp = a[alpha1_tmp + lastv];
            if (temp != 0.0) {
              temp *= tau_0;
              c_ix = lastc + rowleft;
              work_tmp = (((c_ix - rowleft) / 2) << 1) + rowleft;
              im1n = work_tmp - 2;
              for (jy = rowleft; jy <= im1n; jy += 2) {
                tmp = _mm_loadu_pd(&work[jy - rowleft]);
                tmp_0 = _mm_loadu_pd(&a[jy - 1]);
                _mm_storeu_pd(&a[jy - 1], _mm_add_pd(_mm_mul_pd(tmp, _mm_set1_pd
                  (temp)), tmp_0));
              }

              for (jy = work_tmp; jy < c_ix; jy++) {
                a[jy - 1] += work[jy - rowleft] * temp;
              }
            }

            rowleft += 3;
          }
        }
      }

      lastc = tau_tmp;
      in = (i + in) + 1;
      tau_0 = tau[i - 1];
      if (tau_0 != 0.0) {
        lastv = alpha1_tmp + tau_tmp;
        while ((lastc > 0) && (a[lastv - 1] == 0.0)) {
          lastc--;
          lastv--;
        }

        lastv = 3 - i;
        exitg2 = false;
        while ((!exitg2) && (lastv > 0)) {
          jy = (lastv - 1) * 3 + in;
          rowleft = jy;
          do {
            exitg1 = 0;
            if (rowleft <= (jy + lastc) - 1) {
              if (a[rowleft - 1] != 0.0) {
                exitg1 = 1;
              } else {
                rowleft++;
              }
            } else {
              lastv--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);

          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        lastc = 0;
        lastv = 0;
      }

      if (lastc > 0) {
        if (lastv != 0) {
          memset(&work[0], 0, (uint8_T)lastv * sizeof(real_T));
          ix = (lastv - 1) * 3 + in;
          for (jy = in; jy <= ix; jy += 3) {
            temp = 0.0;
            c_ix = jy + lastc;
            for (rowleft = jy; rowleft < c_ix; rowleft++) {
              temp += a[(alpha1_tmp + rowleft) - jy] * a[rowleft - 1];
            }

            work_tmp = div_nde_s32_floor(jy - in, 3);
            work[work_tmp] += temp;
          }
        }

        if (!(-tau_0 == 0.0)) {
          jy = (uint8_T)lastv;
          for (lastv = 0; lastv < jy; lastv++) {
            temp = work[lastv];
            if (temp != 0.0) {
              temp *= -tau_0;
              c_ix = lastc + in;
              for (rowleft = in; rowleft < c_ix; rowleft++) {
                a[rowleft - 1] += a[(alpha1_tmp + rowleft) - in] * temp;
              }
            }

            in += 3;
          }
        }
      }

      a[alpha1_tmp] = alpha1;
    }
  }
}

/* Function for MATLAB Function: '<Root>/MATLAB Function' */
static real_T quadrotorsmodel2_xnrm2_n(int32_T n, const real_T x[3])
{
  real_T y;
  y = 0.0;
  if (n >= 1) {
    if (n == 1) {
      y = fabs(x[1]);
    } else {
      real_T absxk;
      real_T scale;
      real_T t;
      int32_T k;
      scale = 3.3121686421112381E-170;
      absxk = fabs(x[1]);
      if (absxk > 3.3121686421112381E-170) {
        y = 1.0;
        scale = absxk;
      } else {
        t = absxk / 3.3121686421112381E-170;
        y = t * t;
      }

      absxk = fabs(x[2]);
      if (absxk > scale) {
        t = scale / absxk;
        y = y * t * t + 1.0;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }

      k = 2;
      y = scale * sqrt(y);
      if (rtIsNaN(y)) {
        int32_T exitg1;
        do {
          exitg1 = 0;
          if (k < 4) {
            if (rtIsNaN(x[k - 1])) {
              exitg1 = 1;
            } else {
              k++;
            }
          } else {
            y = (rtInf);
            exitg1 = 1;
          }
        } while (exitg1 == 0);
      }
    }
  }

  return y;
}

/* Function for MATLAB Function: '<Root>/MATLAB Function' */
static void quadrotorsmodel2_xdlanv2(real_T *a, real_T *b, real_T *c, real_T *d,
  real_T *rt1r, real_T *rt1i, real_T *rt2r, real_T *rt2i, real_T *cs, real_T *sn)
{
  real_T bcmax;
  real_T bcmis;
  real_T p;
  real_T scale;
  real_T temp;
  real_T z;
  int32_T count;
  int32_T tmp;
  if (*c == 0.0) {
    *cs = 1.0;
    *sn = 0.0;
  } else if (*b == 0.0) {
    *cs = 0.0;
    *sn = 1.0;
    temp = *d;
    *d = *a;
    *a = temp;
    *b = -*c;
    *c = 0.0;
  } else if ((*a - *d == 0.0) && ((*b < 0.0) != (*c < 0.0))) {
    *cs = 1.0;
    *sn = 0.0;
  } else {
    temp = *a - *d;
    p = 0.5 * temp;
    bcmax = fmax(fabs(*b), fabs(*c));
    if (!(*b < 0.0)) {
      count = 1;
    } else {
      count = -1;
    }

    if (!(*c < 0.0)) {
      tmp = 1;
    } else {
      tmp = -1;
    }

    bcmis = fmin(fabs(*b), fabs(*c)) * (real_T)count * (real_T)tmp;
    scale = fmax(fabs(p), bcmax);
    z = p / scale * p + bcmax / scale * bcmis;
    if (z >= 8.8817841970012523E-16) {
      if (!(p < 0.0)) {
        temp = sqrt(scale) * sqrt(z);
      } else {
        temp = -(sqrt(scale) * sqrt(z));
      }

      z = p + temp;
      *a = *d + z;
      *d -= bcmax / z * bcmis;
      bcmax = rt_hypotd_snf(*c, z);
      *cs = z / bcmax;
      *sn = *c / bcmax;
      *b -= *c;
      *c = 0.0;
    } else {
      p = *b + *c;
      scale = fmax(fabs(temp), fabs(p));
      count = 0;
      while ((scale >= 7.4428285367870146E+137) && (count <= 20)) {
        p *= 1.3435752215134178E-138;
        temp *= 1.3435752215134178E-138;
        scale = fmax(fabs(temp), fabs(p));
        count++;
      }

      while ((scale <= 1.3435752215134178E-138) && (count <= 20)) {
        p *= 7.4428285367870146E+137;
        temp *= 7.4428285367870146E+137;
        scale = fmax(fabs(temp), fabs(p));
        count++;
      }

      bcmax = rt_hypotd_snf(p, temp);
      *cs = sqrt((fabs(p) / bcmax + 1.0) * 0.5);
      if (!(p < 0.0)) {
        count = 1;
      } else {
        count = -1;
      }

      *sn = -(0.5 * temp / (bcmax * *cs)) * (real_T)count;
      temp = *a * *cs + *b * *sn;
      p = -*a * *sn + *b * *cs;
      bcmax = *c * *cs + *d * *sn;
      bcmis = -*c * *sn + *d * *cs;
      *b = p * *cs + bcmis * *sn;
      *c = -temp * *sn + bcmax * *cs;
      temp = ((temp * *cs + bcmax * *sn) + (-p * *sn + bcmis * *cs)) * 0.5;
      *a = temp;
      *d = temp;
      if (*c != 0.0) {
        if (*b != 0.0) {
          if ((*b < 0.0) == (*c < 0.0)) {
            scale = sqrt(fabs(*b));
            bcmis = sqrt(fabs(*c));
            p = scale * bcmis;
            if (*c < 0.0) {
              p = -p;
            }

            bcmax = 1.0 / sqrt(fabs(*b + *c));
            *a = temp + p;
            *d = temp - p;
            *b -= *c;
            *c = 0.0;
            p = scale * bcmax;
            bcmax *= bcmis;
            temp = *cs * p - *sn * bcmax;
            *sn = *cs * bcmax + *sn * p;
            *cs = temp;
          }
        } else {
          *b = -*c;
          *c = 0.0;
          temp = *cs;
          *cs = -*sn;
          *sn = temp;
        }
      }
    }
  }

  *rt1r = *a;
  *rt2r = *d;
  if (*c == 0.0) {
    *rt1i = 0.0;
    *rt2i = 0.0;
  } else {
    *rt1i = sqrt(fabs(*b)) * sqrt(fabs(*c));
    *rt2i = -*rt1i;
  }
}

/* Function for MATLAB Function: '<Root>/MATLAB Function' */
static void quadrotorsmodel2_xdlahqr(int32_T ilo, int32_T ihi, real_T h[9],
  real_T *z, int32_T *info, real_T wr[3], real_T wi[3])
{
  __m128d tmp;
  real_T v[3];
  real_T aa;
  real_T bb;
  real_T h11;
  real_T h11_tmp_0;
  real_T h12;
  real_T h21;
  real_T smlnum;
  real_T tst;
  int32_T h11_tmp;
  int32_T i;
  int32_T its;
  int32_T k;
  int32_T kdefl;
  int32_T knt;
  int32_T l;
  int32_T n;
  int32_T nr;
  int32_T vectorUB;
  boolean_T converged;
  boolean_T exitg1;
  boolean_T exitg2;
  boolean_T exitg3;
  *z = 1.0;
  *info = 0;
  kdefl = (uint8_T)(ilo - 1);
  for (k = 0; k < kdefl; k++) {
    wr[k] = h[3 * k + k];
    wi[k] = 0.0;
  }

  for (k = ihi + 1; k < 4; k++) {
    wr[k - 1] = h[((k - 1) * 3 + k) - 1];
    wi[k - 1] = 0.0;
  }

  if (ilo == ihi) {
    wr[ilo - 1] = h[((ilo - 1) * 3 + ilo) - 1];
    wi[ilo - 1] = 0.0;
  } else {
    if (ilo <= ihi - 2) {
      h[ihi - 1] = 0.0;
    }

    smlnum = (real_T)((ihi - ilo) + 1) / 2.2204460492503131E-16 *
      2.2250738585072014E-308;
    kdefl = 0;
    i = ihi - 1;
    exitg1 = false;
    while ((!exitg1) && (i + 1 >= ilo)) {
      l = ilo;
      converged = false;
      its = 0;
      exitg2 = false;
      while ((!exitg2) && (its < 301)) {
        k = i;
        exitg3 = false;
        while ((!exitg3) && (k + 1 > l)) {
          knt = (k - 1) * 3 + k;
          if (fabs(h[knt]) <= smlnum) {
            exitg3 = true;
          } else {
            nr = 3 * k + k;
            tst = fabs(h[knt - 1]) + fabs(h[nr]);
            if (tst == 0.0) {
              if (k - 1 >= ilo) {
                tst = fabs(h[k - 1]);
              }

              if (k + 2 <= ihi) {
                tst += fabs(h[3 * k + 2]);
              }
            }

            if (fabs(h[knt]) <= 2.2204460492503131E-16 * tst) {
              tst = fabs(h[knt - 1] - h[nr]);
              aa = fmax(fabs(h[nr]), tst);
              bb = fmin(fabs(h[nr]), tst);
              tst = aa + bb;
              if (fmax(fabs(h[knt]), fabs(h[nr - 1])) / tst * fmin(fabs(h[knt]),
                   fabs(h[nr - 1])) <= fmax(smlnum, aa / tst * bb *
                   2.2204460492503131E-16)) {
                exitg3 = true;
              } else {
                k--;
              }
            } else {
              k--;
            }
          }
        }

        l = k + 1;
        if (k + 1 > ilo) {
          h[k + 3 * (k - 1)] = 0.0;
        }

        if (k + 1 >= i) {
          converged = true;
          exitg2 = true;
        } else {
          kdefl++;
          if (kdefl - kdefl / 20 * 20 == 0) {
            tst = fabs(h[(i - 1) * 3 + i]) + fabs(h[i - 1]);
            h11 = h[3 * i + i] + 0.75 * tst;
            h12 = -0.4375 * tst;
            h21 = tst;
            aa = h11;
          } else if (kdefl - kdefl / 10 * 10 == 0) {
            tst = fabs(h[1]) + fabs(h[5]);
            h11 = 0.75 * tst + h[0];
            h12 = -0.4375 * tst;
            h21 = tst;
            aa = h11;
          } else {
            h11_tmp = (i - 1) * 3 + i;
            h11 = h[h11_tmp - 1];
            h21 = h[h11_tmp];
            k = 3 * i + i;
            h12 = h[k - 1];
            aa = h[k];
          }

          tst = ((fabs(h11) + fabs(h12)) + fabs(h21)) + fabs(aa);
          if (tst == 0.0) {
            h11 = 0.0;
            aa = 0.0;
            bb = 0.0;
            h21 = 0.0;
          } else {
            h11 /= tst;
            aa /= tst;
            bb = (h11 + aa) / 2.0;
            h11 = (h11 - bb) * (aa - bb) - h12 / tst * (h21 / tst);
            h21 = sqrt(fabs(h11));
            if (h11 >= 0.0) {
              h11 = bb * tst;
              bb = h11;
              aa = h21 * tst;
              h21 = -aa;
            } else {
              h11 = bb + h21;
              bb -= h21;
              if (fabs(h11 - aa) <= fabs(bb - aa)) {
                h11 *= tst;
                bb = h11;
              } else {
                bb *= tst;
                h11 = bb;
              }

              aa = 0.0;
              h21 = 0.0;
            }
          }

          if (i - 1 >= 1) {
            tst = (fabs(h[0] - bb) + fabs(h21)) + fabs(h[1]);
            h12 = h[1] / tst;
            v[0] = ((h[0] - bb) / tst * (h[0] - h11) + h12 * h[3]) - h21 / tst *
              aa;
            v[1] = (((h[0] + h[4]) - h11) - bb) * h12;
            v[2] = h12 * h[5];
            tst = (fabs(v[0]) + fabs(v[1])) + fabs(v[2]);
            tmp = _mm_div_pd(_mm_loadu_pd(&v[0]), _mm_set1_pd(tst));
            _mm_storeu_pd(&v[0], tmp);
            v[2] /= tst;
          }

          for (k = i - 1; k <= i; k++) {
            nr = (i - k) + 2;
            if (nr >= 3) {
              nr = 3;
            }

            if (k > i - 1) {
              h11_tmp = ((k - 2) * 3 + k) - 1;
              n = (uint8_T)nr;
              for (knt = 0; knt < n; knt++) {
                v[knt] = h[h11_tmp + knt];
              }
            }

            bb = v[0];
            tst = 0.0;
            if (nr > 0) {
              aa = quadrotorsmodel2_xnrm2_n(nr - 1, v);
              if (aa != 0.0) {
                aa = rt_hypotd_snf(v[0], aa);
                if (v[0] >= 0.0) {
                  aa = -aa;
                }

                if (fabs(aa) < 1.0020841800044864E-292) {
                  knt = 0;
                  do {
                    knt++;
                    n = (((nr - 1) / 2) << 1) + 2;
                    vectorUB = n - 2;
                    for (h11_tmp = 2; h11_tmp <= vectorUB; h11_tmp += 2) {
                      tmp = _mm_loadu_pd(&v[h11_tmp - 1]);
                      _mm_storeu_pd(&v[h11_tmp - 1], _mm_mul_pd(tmp, _mm_set1_pd
                        (9.9792015476736E+291)));
                    }

                    for (h11_tmp = n; h11_tmp <= nr; h11_tmp++) {
                      v[h11_tmp - 1] *= 9.9792015476736E+291;
                    }

                    aa *= 9.9792015476736E+291;
                    bb *= 9.9792015476736E+291;
                  } while ((fabs(aa) < 1.0020841800044864E-292) && (knt < 20));

                  aa = rt_hypotd_snf(bb, quadrotorsmodel2_xnrm2_n(nr - 1, v));
                  if (bb >= 0.0) {
                    aa = -aa;
                  }

                  tst = (aa - bb) / aa;
                  bb = 1.0 / (bb - aa);
                  vectorUB = n - 2;
                  for (h11_tmp = 2; h11_tmp <= vectorUB; h11_tmp += 2) {
                    tmp = _mm_loadu_pd(&v[h11_tmp - 1]);
                    _mm_storeu_pd(&v[h11_tmp - 1], _mm_mul_pd(tmp, _mm_set1_pd
                      (bb)));
                  }

                  for (h11_tmp = n; h11_tmp <= nr; h11_tmp++) {
                    v[h11_tmp - 1] *= bb;
                  }

                  for (h11_tmp = 0; h11_tmp < knt; h11_tmp++) {
                    aa *= 1.0020841800044864E-292;
                  }

                  bb = aa;
                } else {
                  tst = (aa - v[0]) / aa;
                  bb = 1.0 / (v[0] - aa);
                  n = (((nr - 1) / 2) << 1) + 2;
                  vectorUB = n - 2;
                  for (knt = 2; knt <= vectorUB; knt += 2) {
                    tmp = _mm_loadu_pd(&v[knt - 1]);
                    _mm_storeu_pd(&v[knt - 1], _mm_mul_pd(tmp, _mm_set1_pd(bb)));
                  }

                  for (knt = n; knt <= nr; knt++) {
                    v[knt - 1] *= bb;
                  }

                  bb = aa;
                }
              }
            }

            if (k > i - 1) {
              h[k - 1] = bb;
              h[k] = 0.0;
              if (k < i) {
                /* Check node always fails. would cause program termination and was eliminated */
              }
            }

            aa = v[1];
            bb = tst * v[1];
            if (nr == 3) {
              h21 = v[2];
              h12 = tst * v[2];
              for (nr = k; nr <= i + 1; nr++) {
                n = (nr - 1) * 3;
                h11_tmp = n + k;
                h11_tmp_0 = h[h11_tmp - 1];
                h11 = (h[h11_tmp] * aa + h11_tmp_0) + h[n + 2] * h21;
                h[h11_tmp - 1] = h11_tmp_0 - h11 * tst;
                h[h11_tmp] -= h11 * bb;
                h[n + 2] -= h11 * h12;
              }

              if (k + 3 <= i + 1) {
                knt = k + 3;
              } else {
                knt = i + 1;
              }

              for (nr = 1; nr <= knt; nr++) {
                n = ((k - 1) * 3 + nr) - 1;
                h11_tmp_0 = h[n];
                h11_tmp = (3 * k + nr) - 1;
                h11 = (h[h11_tmp] * aa + h11_tmp_0) + h[nr + 5] * h21;
                h[n] = h11_tmp_0 - h11 * tst;
                h[h11_tmp] -= h11 * bb;
                h[nr + 5] -= h11 * h12;
              }
            } else if (nr == 2) {
              for (nr = k; nr <= i + 1; nr++) {
                knt = (nr - 1) * 3 + k;
                h12 = h[knt - 1];
                h21 = h[knt];
                h11 = aa * h21 + h12;
                h[knt - 1] = h12 - h11 * tst;
                h[knt] = h21 - h11 * bb;
              }

              for (nr = 1; nr <= i + 1; nr++) {
                knt = ((k - 1) * 3 + nr) - 1;
                h12 = h[knt];
                h11_tmp = (3 * k + nr) - 1;
                h21 = h[h11_tmp];
                h11 = aa * h21 + h12;
                h[knt] = h12 - h11 * tst;
                h[h11_tmp] = h21 - h11 * bb;
              }
            }
          }

          its++;
        }
      }

      if (!converged) {
        *info = i + 1;
        exitg1 = true;
      } else {
        if (i + 1 == l) {
          wr[i] = h[3 * i + i];
          wi[i] = 0.0;
        } else if (l == i) {
          nr = 3 * i + i;
          tst = h[nr - 1];
          k = (i - 1) * 3 + i;
          aa = h[k];
          bb = h[nr];
          quadrotorsmodel2_xdlanv2(&h[k - 1], &tst, &aa, &bb, &wr[i - 1], &wi[i
            - 1], &h11, &h21, &h12, &h11_tmp_0);
          wr[i] = h11;
          wi[i] = h21;
          h[nr - 1] = tst;
          h[k] = aa;
          h[nr] = bb;
        }

        kdefl = 0;
        i = l - 2;
      }
    }

    if (*info != 0) {
      h[2] = 0.0;
    }
  }
}

/* Function for MATLAB Function: '<Root>/MATLAB Function' */
static void quadrotorsmodel2_xzlascl_d(real_T cfrom, real_T cto, int32_T m,
  real_T A[3], int32_T iA0)
{
  real_T cfromc;
  real_T ctoc;
  int32_T b_i;
  boolean_T notdone;
  cfromc = cfrom;
  ctoc = cto;
  notdone = true;
  while (notdone) {
    real_T cfrom1;
    real_T cto1;
    real_T mul;
    int32_T scalarLB;
    int32_T tmp_0;
    int32_T vectorUB;
    cfrom1 = cfromc * 2.0041683600089728E-292;
    cto1 = ctoc / 4.9896007738368E+291;
    if ((fabs(cfrom1) > fabs(ctoc)) && (ctoc != 0.0)) {
      mul = 2.0041683600089728E-292;
      cfromc = cfrom1;
    } else if (fabs(cto1) > fabs(cfromc)) {
      mul = 4.9896007738368E+291;
      ctoc = cto1;
    } else {
      mul = ctoc / cfromc;
      notdone = false;
    }

    scalarLB = (m / 2) << 1;
    vectorUB = scalarLB - 2;
    for (b_i = 0; b_i <= vectorUB; b_i += 2) {
      __m128d tmp;
      tmp_0 = (b_i + iA0) - 1;
      tmp = _mm_loadu_pd(&A[tmp_0]);
      _mm_storeu_pd(&A[tmp_0], _mm_mul_pd(tmp, _mm_set1_pd(mul)));
    }

    for (b_i = scalarLB; b_i < m; b_i++) {
      tmp_0 = (b_i + iA0) - 1;
      A[tmp_0] *= mul;
    }
  }
}

/* Function for MATLAB Function: '<Root>/MATLAB Function' */
static void quadrotorsmodel2_eigStandard(const real_T A[9], creal_T V[3])
{
  __m128d tmp;
  real_T b_A[9];
  real_T wi[3];
  real_T wr[3];
  real_T absxk;
  real_T anrm;
  real_T b_absxk;
  real_T c;
  real_T g;
  real_T r;
  real_T s;
  real_T scale;
  real_T t;
  int32_T b_k;
  int32_T c_ix;
  int32_T c_tmp;
  int32_T exitg2;
  int32_T exitg3;
  int32_T exitg4;
  int32_T exitg5;
  int32_T exitg8;
  int32_T f_ix;
  int32_T ica;
  int32_T ica_tmp;
  int32_T kend;
  int32_T l;
  boolean_T exitg1;
  boolean_T exitg6;
  boolean_T exitg7;
  boolean_T notdone;
  boolean_T scalea;
  boolean_T skipThisColumn;
  memcpy(&b_A[0], &A[0], 9U * sizeof(real_T));
  anrm = 0.0;
  b_k = 0;
  exitg1 = false;
  while ((!exitg1) && (b_k < 9)) {
    absxk = fabs(A[b_k]);
    if (rtIsNaN(absxk)) {
      anrm = (rtNaN);
      exitg1 = true;
    } else {
      if (absxk > anrm) {
        anrm = absxk;
      }

      b_k++;
    }
  }

  if (rtIsInf(anrm) || rtIsNaN(anrm)) {
    V[0].re = (rtNaN);
    V[0].im = 0.0;
    V[1].re = (rtNaN);
    V[1].im = 0.0;
    V[2].re = (rtNaN);
    V[2].im = 0.0;
  } else {
    absxk = anrm;
    scalea = false;
    if ((anrm > 0.0) && (anrm < 6.7178761075670888E-139)) {
      scalea = true;
      absxk = 6.7178761075670888E-139;
      quadrotorsmodel2_xzlascl(anrm, absxk, b_A);
    } else if (anrm > 1.4885657073574029E+138) {
      scalea = true;
      absxk = 1.4885657073574029E+138;
      quadrotorsmodel2_xzlascl(anrm, absxk, b_A);
    }

    wr[0] = 1.0;
    wr[1] = 1.0;
    wr[2] = 1.0;
    b_k = 1;
    l = 3;
    notdone = true;
    do {
      exitg5 = 0;
      if (notdone) {
        notdone = false;
        f_ix = l;
        do {
          exitg4 = 0;
          if (f_ix > 0) {
            skipThisColumn = false;
            ica = 0;
            exitg6 = false;
            while ((!exitg6) && (ica <= l - 1)) {
              if ((ica + 1 == f_ix) || (!(b_A[(3 * ica + f_ix) - 1] != 0.0))) {
                ica++;
              } else {
                skipThisColumn = true;
                exitg6 = true;
              }
            }

            if (skipThisColumn) {
              f_ix--;
            } else {
              wr[l - 1] = f_ix;
              if (f_ix != l) {
                ica = (f_ix - 1) * 3;
                kend = (l - 1) * 3;
                for (c_ix = 0; c_ix < l; c_ix++) {
                  c_tmp = ica + c_ix;
                  c = b_A[c_tmp];
                  ica_tmp = kend + c_ix;
                  b_A[c_tmp] = b_A[ica_tmp];
                  b_A[ica_tmp] = c;
                }

                c = b_A[f_ix - 1];
                b_A[f_ix - 1] = b_A[l - 1];
                b_A[l - 1] = c;
                c = b_A[f_ix + 2];
                b_A[f_ix + 2] = b_A[l + 2];
                b_A[l + 2] = c;
                c = b_A[f_ix + 5];
                b_A[f_ix + 5] = b_A[l + 5];
                b_A[l + 5] = c;
              }

              exitg4 = 1;
            }
          } else {
            exitg4 = 2;
          }
        } while (exitg4 == 0);

        if (exitg4 == 1) {
          if (l == 1) {
            exitg5 = 1;
          } else {
            l--;
            notdone = true;
          }
        }
      } else {
        notdone = true;
        while (notdone) {
          notdone = false;
          f_ix = b_k;
          exitg6 = false;
          while ((!exitg6) && (f_ix <= l)) {
            skipThisColumn = false;
            ica = b_k;
            exitg7 = false;
            while ((!exitg7) && (ica <= l)) {
              if ((ica == f_ix) || (!(b_A[((f_ix - 1) * 3 + ica) - 1] != 0.0)))
              {
                ica++;
              } else {
                skipThisColumn = true;
                exitg7 = true;
              }
            }

            if (skipThisColumn) {
              f_ix++;
            } else {
              wr[b_k - 1] = f_ix;
              if (f_ix != b_k) {
                ica = (f_ix - 1) * 3;
                kend = (b_k - 1) * 3;
                for (c_ix = 0; c_ix < l; c_ix++) {
                  c_tmp = ica + c_ix;
                  c = b_A[c_tmp];
                  ica_tmp = kend + c_ix;
                  b_A[c_tmp] = b_A[ica_tmp];
                  b_A[ica_tmp] = c;
                }

                f_ix = (kend + f_ix) - 1;
                ica = (kend + b_k) - 1;
                kend = (uint8_T)(4 - b_k);
                for (c_ix = 0; c_ix < kend; c_ix++) {
                  c_tmp = c_ix * 3 + f_ix;
                  c = b_A[c_tmp];
                  ica_tmp = c_ix * 3 + ica;
                  b_A[c_tmp] = b_A[ica_tmp];
                  b_A[ica_tmp] = c;
                }
              }

              b_k++;
              notdone = true;
              exitg6 = true;
            }
          }
        }

        exitg5 = 2;
      }
    } while (exitg5 == 0);

    if (exitg5 == 1) {
    } else {
      exitg1 = false;
      while ((!exitg1) && (!notdone)) {
        notdone = true;
        f_ix = b_k - 1;
        do {
          exitg3 = 0;
          if (f_ix + 1 <= l) {
            c_tmp = (l - b_k) + 1;
            c = quadrotorsmodel2_xnrm2(c_tmp, b_A, f_ix * 3 + b_k);
            ica_tmp = (b_k - 1) * 3 + f_ix;
            ica = ica_tmp;
            r = 0.0;
            if (c_tmp >= 1) {
              if (c_tmp == 1) {
                r = fabs(b_A[ica_tmp]);
              } else {
                scale = 3.3121686421112381E-170;
                kend = ((c_tmp - 1) * 3 + ica_tmp) + 1;
                for (c_ix = ica_tmp + 1; c_ix <= kend; c_ix += 3) {
                  b_absxk = fabs(b_A[c_ix - 1]);
                  if (b_absxk > scale) {
                    t = scale / b_absxk;
                    r = r * t * t + 1.0;
                    scale = b_absxk;
                  } else {
                    t = b_absxk / scale;
                    r += t * t;
                  }
                }

                r = scale * sqrt(r);
                if (rtIsNaN(r)) {
                  do {
                    exitg8 = 0;
                    if (ica + 1 <= kend) {
                      if (rtIsNaN(b_A[ica])) {
                        exitg8 = 1;
                      } else {
                        ica += 3;
                      }
                    } else {
                      r = (rtInf);
                      exitg8 = 1;
                    }
                  } while (exitg8 == 0);
                }
              }
            }

            kend = f_ix * 3;
            ica = 1;
            if (l > 1) {
              scale = fabs(b_A[kend]);
              for (c_ix = 2; c_ix <= l; c_ix++) {
                b_absxk = fabs(b_A[(kend + c_ix) - 1]);
                if (b_absxk > scale) {
                  ica = c_ix;
                  scale = b_absxk;
                }
              }
            }

            scale = fabs(b_A[(3 * f_ix + ica) - 1]);
            ica = 4 - b_k;
            if (4 - b_k < 1) {
              kend = 0;
            } else {
              kend = 1;
              if (4 - b_k > 1) {
                b_absxk = fabs(b_A[ica_tmp]);
                for (c_ix = 2; c_ix <= ica; c_ix++) {
                  t = fabs(b_A[(c_ix - 1) * 3 + ica_tmp]);
                  if (t > b_absxk) {
                    kend = c_ix;
                    b_absxk = t;
                  }
                }
              }
            }

            t = fabs(b_A[((kend + b_k) - 2) * 3 + f_ix]);
            if ((c == 0.0) || (r == 0.0)) {
              f_ix++;
            } else {
              g = r / 2.0;
              b_absxk = 1.0;
              s = c + r;
              do {
                exitg2 = 0;
                if ((c < g) && (fmax(b_absxk, fmax(c, scale)) <
                                4.9896007738368E+291) && (fmin(r, fmin(g, t)) >
                     2.0041683600089728E-292)) {
                  if (rtIsNaN(((((c + b_absxk) + scale) + r) + g) + t)) {
                    exitg2 = 1;
                  } else {
                    b_absxk *= 2.0;
                    c *= 2.0;
                    scale *= 2.0;
                    r /= 2.0;
                    g /= 2.0;
                    t /= 2.0;
                  }
                } else {
                  g = c / 2.0;
                  while ((g >= r) && (fmax(r, t) < 4.9896007738368E+291) &&
                         (fmin(fmin(b_absxk, c), fmin(g, scale)) >
                          2.0041683600089728E-292)) {
                    b_absxk /= 2.0;
                    c /= 2.0;
                    g /= 2.0;
                    scale /= 2.0;
                    r *= 2.0;
                    t *= 2.0;
                  }

                  if ((c + r >= 0.95 * s) || ((b_absxk < 1.0) && (wr[f_ix] < 1.0)
                       && (b_absxk * wr[f_ix] <= 1.0020841800044864E-292)) ||
                      ((b_absxk > 1.0) && (wr[f_ix] > 1.0) && (wr[f_ix] >=
                        9.9792015476736E+291 / b_absxk))) {
                  } else {
                    c = 1.0 / b_absxk;
                    wr[f_ix] *= b_absxk;
                    ica = ica_tmp + 1;
                    kend = ((3 - b_k) * 3 + ica_tmp) + 1;
                    for (c_ix = ica; c_ix <= kend; c_ix += 3) {
                      b_A[c_ix - 1] *= c;
                    }

                    ica = f_ix * 3;
                    kend = ica + l;
                    c_tmp = ((((kend - ica) / 2) << 1) + ica) + 1;
                    ica_tmp = c_tmp - 2;
                    for (c_ix = ica + 1; c_ix <= ica_tmp; c_ix += 2) {
                      tmp = _mm_loadu_pd(&b_A[c_ix - 1]);
                      _mm_storeu_pd(&b_A[c_ix - 1], _mm_mul_pd(tmp, _mm_set1_pd
                        (b_absxk)));
                    }

                    for (c_ix = c_tmp; c_ix <= kend; c_ix++) {
                      b_A[c_ix - 1] *= b_absxk;
                    }

                    notdone = false;
                  }

                  exitg2 = 2;
                }
              } while (exitg2 == 0);

              if (exitg2 == 1) {
                exitg3 = 2;
              } else {
                f_ix++;
              }
            }
          } else {
            exitg3 = 1;
          }
        } while (exitg3 == 0);

        if (exitg3 == 1) {
        } else {
          exitg1 = true;
        }
      }
    }

    quadrotorsmodel2_xzgehrd(b_A, b_k, l);
    quadrotorsmodel2_xdlahqr(b_k, l, b_A, &c, &f_ix, wr, wi);
    if (scalea) {
      quadrotorsmodel2_xzlascl_d(absxk, anrm, 3 - f_ix, wr, f_ix + 1);
      quadrotorsmodel2_xzlascl_d(absxk, anrm, 3 - f_ix, wi, f_ix + 1);
      if (f_ix != 0) {
        quadrotorsmodel2_xzlascl_d(absxk, anrm, b_k - 1, wr, 1);
        quadrotorsmodel2_xzlascl_d(absxk, anrm, b_k - 1, wi, 1);
      }
    }

    if (f_ix != 0) {
      for (l = b_k; l <= f_ix; l++) {
        wr[l - 1] = (rtNaN);
        wi[l - 1] = 0.0;
      }
    }

    V[0].re = wr[0];
    V[0].im = wi[0];
    V[1].re = wr[1];
    V[1].im = wi[1];
    V[2].re = wr[2];
    V[2].im = wi[2];
  }
}

/* Function for MATLAB Function: '<Root>/MATLAB Function' */
static void quadrotorsmodel2_xzlascl_dx(real_T cfrom, real_T cto, int32_T m,
  real_T A[2], int32_T iA0)
{
  real_T cfromc;
  real_T ctoc;
  int32_T b_i;
  boolean_T notdone;
  cfromc = cfrom;
  ctoc = cto;
  notdone = true;
  while (notdone) {
    real_T cfrom1;
    real_T cto1;
    real_T mul;
    int32_T scalarLB;
    int32_T tmp_0;
    int32_T vectorUB;
    cfrom1 = cfromc * 2.0041683600089728E-292;
    cto1 = ctoc / 4.9896007738368E+291;
    if ((fabs(cfrom1) > fabs(ctoc)) && (ctoc != 0.0)) {
      mul = 2.0041683600089728E-292;
      cfromc = cfrom1;
    } else if (fabs(cto1) > fabs(cfromc)) {
      mul = 4.9896007738368E+291;
      ctoc = cto1;
    } else {
      mul = ctoc / cfromc;
      notdone = false;
    }

    scalarLB = (m / 2) << 1;
    vectorUB = scalarLB - 2;
    for (b_i = 0; b_i <= vectorUB; b_i += 2) {
      __m128d tmp;
      tmp_0 = (b_i + iA0) - 1;
      tmp = _mm_loadu_pd(&A[tmp_0]);
      _mm_storeu_pd(&A[tmp_0], _mm_mul_pd(tmp, _mm_set1_pd(mul)));
    }

    for (b_i = scalarLB; b_i < m; b_i++) {
      tmp_0 = (b_i + iA0) - 1;
      A[tmp_0] *= mul;
    }
  }
}

/* Function for MATLAB Function: '<Root>/MATLAB Function' */
static void quadrotorsmodel2_xdlaev2(real_T a, real_T b, real_T c, real_T *rt1,
  real_T *rt2)
{
  real_T ab;
  real_T acmn;
  real_T acmx;
  real_T adf;
  real_T sm;
  sm = a + c;
  adf = fabs(a - c);
  ab = fabs(b + b);
  if (fabs(a) > fabs(c)) {
    acmx = a;
    acmn = c;
  } else {
    acmx = c;
    acmn = a;
  }

  if (adf > ab) {
    ab /= adf;
    adf *= sqrt(ab * ab + 1.0);
  } else if (adf < ab) {
    adf /= ab;
    adf = sqrt(adf * adf + 1.0) * ab;
  } else {
    adf = ab * 1.4142135623730951;
  }

  if (sm < 0.0) {
    *rt1 = (sm - adf) * 0.5;
    *rt2 = acmx / *rt1 * acmn - b / *rt1 * b;
  } else if (sm > 0.0) {
    *rt1 = (sm + adf) * 0.5;
    *rt2 = acmx / *rt1 * acmn - b / *rt1 * b;
  } else {
    *rt1 = 0.5 * adf;
    *rt2 = -0.5 * adf;
  }
}

/* Function for MATLAB Function: '<Root>/MATLAB Function' */
static void quadrotorsmodel2_insertionsort(real_T x[3], int32_T xstart, int32_T
  xend)
{
  int32_T k;
  for (k = xstart + 1; k <= xend; k++) {
    real_T xc;
    int32_T idx;
    boolean_T exitg1;
    xc = x[k - 1];
    idx = k - 1;
    exitg1 = false;
    while ((!exitg1) && (idx >= xstart)) {
      real_T tmp;
      tmp = x[idx - 1];
      if (xc < tmp) {
        x[idx] = tmp;
        idx--;
      } else {
        exitg1 = true;
      }
    }

    x[idx] = xc;
  }
}

/* Function for MATLAB Function: '<Root>/MATLAB Function' */
static int32_T quadrotorsmodel2_xdsterf(real_T d[3], real_T e[2])
{
  __m128d tmp;
  real_T anorm;
  real_T b_anorm;
  real_T b_gamma;
  real_T e_0;
  real_T oldc;
  real_T p;
  real_T r;
  real_T rte;
  real_T s;
  int32_T e_i;
  int32_T exitg1;
  int32_T exitg3;
  int32_T exitg4;
  int32_T info;
  int32_T iscale;
  int32_T iscale_tmp;
  int32_T jtot;
  int32_T l;
  int32_T l1;
  int32_T lend;
  int32_T lendsv;
  int32_T lsv;
  int32_T m;
  int32_T vectorUB;
  boolean_T exitg2;
  info = 0;
  jtot = 0;
  l1 = 1;
  do {
    exitg1 = 0;
    if (l1 > 3) {
      quadrotorsmodel2_insertionsort(d, 1, 3);
      exitg1 = 1;
    } else {
      if (l1 > 1) {
        e[l1 - 2] = 0.0;
      }

      m = l1;
      exitg2 = false;
      while ((!exitg2) && (m < 3)) {
        if (fabs(e[m - 1]) <= sqrt(fabs(d[m - 1])) * sqrt(fabs(d[m])) *
            2.2204460492503131E-16) {
          e[m - 1] = 0.0;
          exitg2 = true;
        } else {
          m++;
        }
      }

      l = l1;
      lsv = l1;
      lend = m;
      lendsv = m + 1;
      l1 = m + 1;
      if (m == l) {
      } else {
        iscale_tmp = m - l;
        if (iscale_tmp + 1 <= 0) {
          anorm = 0.0;
        } else {
          anorm = fabs(d[(l + iscale_tmp) - 1]);
          e_i = -1;
          exitg2 = false;
          while ((!exitg2) && (e_i + 1 <= iscale_tmp - 1)) {
            iscale = l + e_i;
            b_anorm = fabs(d[iscale]);
            if (rtIsNaN(b_anorm)) {
              anorm = (rtNaN);
              exitg2 = true;
            } else {
              if (b_anorm > anorm) {
                anorm = b_anorm;
              }

              b_anorm = fabs(e[iscale]);
              if (rtIsNaN(b_anorm)) {
                anorm = (rtNaN);
                exitg2 = true;
              } else {
                if (b_anorm > anorm) {
                  anorm = b_anorm;
                }

                e_i++;
              }
            }
          }
        }

        if (anorm == 0.0) {
        } else {
          iscale = 0;
          if (anorm > 2.2346346549904327E+153) {
            iscale = 1;
            quadrotorsmodel2_xzlascl_d(anorm, 2.2346346549904327E+153,
              iscale_tmp + 1, d, l);
            quadrotorsmodel2_xzlascl_dx(anorm, 2.2346346549904327E+153,
              iscale_tmp, e, l);
          } else if (anorm < 3.02546243347603E-123) {
            iscale = 2;
            quadrotorsmodel2_xzlascl_d(anorm, 3.02546243347603E-123, iscale_tmp
              + 1, d, l);
            quadrotorsmodel2_xzlascl_dx(anorm, 3.02546243347603E-123, iscale_tmp,
              e, l);
          }

          iscale_tmp = ((iscale_tmp / 2) << 1) + l;
          vectorUB = iscale_tmp - 2;
          for (e_i = l; e_i <= vectorUB; e_i += 2) {
            tmp = _mm_loadu_pd(&e[e_i - 1]);
            _mm_storeu_pd(&e[e_i - 1], _mm_mul_pd(tmp, tmp));
          }

          for (e_i = iscale_tmp; e_i < m; e_i++) {
            e_0 = e[e_i - 1];
            e[e_i - 1] = e_0 * e_0;
          }

          if (fabs(d[m - 1]) < fabs(d[l - 1])) {
            lend = lsv;
            l = m;
          }

          if (lend >= l) {
            do {
              exitg4 = 0;
              if (l != lend) {
                m = l;
                while ((m < lend) && (!(fabs(e[m - 1]) <= fabs(d[m - 1]) *
                                        4.9303806576313238E-32 * fabs(d[m])))) {
                  m++;
                }
              } else {
                m = lend;
              }

              if (m < lend) {
                e[m - 1] = 0.0;
              }

              if (m == l) {
                l++;
                if (l > lend) {
                  exitg4 = 1;
                }
              } else if (l + 1 == m) {
                quadrotorsmodel2_xdlaev2(d[l - 1], sqrt(e[l - 1]), d[l], &d[l -
                  1], &b_anorm);
                d[l] = b_anorm;
                e[l - 1] = 0.0;
                l += 2;
                if (l > lend) {
                  exitg4 = 1;
                }
              } else if (jtot == 90) {
                exitg4 = 1;
              } else {
                jtot++;
                rte = sqrt(e[l - 1]);
                e_0 = d[l - 1];
                b_anorm = (d[l] - e_0) / (2.0 * rte);
                s = rt_hypotd_snf(b_anorm, 1.0);
                if (!(b_anorm >= 0.0)) {
                  s = -s;
                }

                b_anorm = e_0 - rte / (b_anorm + s);
                rte = 1.0;
                s = 0.0;
                b_gamma = d[m - 1] - b_anorm;
                p = b_gamma * b_gamma;
                for (e_i = m - 1; e_i >= l; e_i--) {
                  e_0 = e[e_i - 1];
                  r = p + e_0;
                  if (m - 1 != e_i) {
                    e[1] = s * r;
                  }

                  oldc = rte;
                  rte = p / r;
                  s = e_0 / r;
                  p = b_gamma;
                  r = d[e_i - 1];
                  b_gamma = (r - b_anorm) * rte - s * b_gamma;
                  d[e_i] = (r - b_gamma) + p;
                  if (rte != 0.0) {
                    p = b_gamma * b_gamma / rte;
                  } else {
                    p = oldc * e_0;
                  }
                }

                e[l - 1] = s * p;
                d[l - 1] = b_anorm + b_gamma;
              }
            } while (exitg4 == 0);
          } else {
            do {
              exitg3 = 0;
              m = l;
              while ((m > lend) && (!(fabs(e[m - 2]) <= fabs(d[m - 1]) *
                                      4.9303806576313238E-32 * fabs(d[m - 2]))))
              {
                m--;
              }

              if (m > lend) {
                e[m - 2] = 0.0;
              }

              if (m == l) {
                l--;
                if (l < lend) {
                  exitg3 = 1;
                }
              } else if (l - 1 == m) {
                quadrotorsmodel2_xdlaev2(d[l - 1], sqrt(e[l - 2]), d[l - 2],
                  &d[l - 1], &b_anorm);
                d[l - 2] = b_anorm;
                e[l - 2] = 0.0;
                l -= 2;
                if (l < lend) {
                  exitg3 = 1;
                }
              } else if (jtot == 90) {
                exitg3 = 1;
              } else {
                jtot++;
                rte = sqrt(e[l - 2]);
                e_0 = d[l - 1];
                b_anorm = (d[l - 2] - e_0) / (2.0 * rte);
                s = rt_hypotd_snf(b_anorm, 1.0);
                if (!(b_anorm >= 0.0)) {
                  s = -s;
                }

                b_anorm = e_0 - rte / (b_anorm + s);
                rte = 1.0;
                s = 0.0;
                b_gamma = d[m - 1] - b_anorm;
                p = b_gamma * b_gamma;
                for (e_i = m; e_i < l; e_i++) {
                  e_0 = e[e_i - 1];
                  r = p + e_0;
                  if (e_i != m) {
                    e[0] = s * r;
                  }

                  oldc = rte;
                  rte = p / r;
                  s = e_0 / r;
                  p = b_gamma;
                  b_gamma = (d[e_i] - b_anorm) * rte - s * b_gamma;
                  d[e_i - 1] = (d[e_i] - b_gamma) + p;
                  if (rte != 0.0) {
                    p = b_gamma * b_gamma / rte;
                  } else {
                    p = oldc * e_0;
                  }
                }

                e[l - 2] = s * p;
                d[l - 1] = b_anorm + b_gamma;
              }
            } while (exitg3 == 0);
          }

          if (iscale == 1) {
            quadrotorsmodel2_xzlascl_d(2.2346346549904327E+153, anorm, lendsv -
              lsv, d, lsv);
          } else if (iscale == 2) {
            quadrotorsmodel2_xzlascl_d(3.02546243347603E-123, anorm, lendsv -
              lsv, d, lsv);
          }

          if (jtot >= 90) {
            if (e[0] != 0.0) {
              info = 1;
            }

            if (e[1] != 0.0) {
              info++;
            }

            exitg1 = 1;
          }
        }
      }
    }
  } while (exitg1 == 0);

  return info;
}

/* Function for MATLAB Function: '<Root>/MATLAB Function' */
static void quadrotorsmodel2_eig(const real_T A[9], creal_T V[3])
{
  __m128d tmp;
  real_T b_A[9];
  real_T a__3[3];
  real_T wi[3];
  real_T e[2];
  real_T tau[2];
  real_T tmp_0[2];
  real_T absx;
  real_T anrm;
  real_T b_A_tmp;
  real_T temp1;
  real_T temp2;
  int32_T b_ix;
  int32_T b_iy;
  int32_T e_tmp_tmp;
  int32_T exitg1;
  int32_T ii;
  int32_T iv;
  int32_T k;
  int32_T tau_tmp;
  int32_T temp1_tmp;
  boolean_T exitg2;
  boolean_T iscale;
  iscale = true;
  for (k = 0; k < 9; k++) {
    if (iscale) {
      anrm = A[k];
      if ((!rtIsInf(anrm)) && (!rtIsNaN(anrm))) {
      } else {
        iscale = false;
      }
    } else {
      iscale = false;
    }
  }

  if (!iscale) {
    V[0].re = (rtNaN);
    V[0].im = 0.0;
    V[1].re = (rtNaN);
    V[1].im = 0.0;
    V[2].re = (rtNaN);
    V[2].im = 0.0;
  } else {
    k = 0;
    exitg2 = false;
    while ((!exitg2) && (k < 3)) {
      iv = 0;
      do {
        exitg1 = 0;
        if (iv <= k) {
          if (!(A[3 * k + iv] == A[3 * iv + k])) {
            iscale = false;
            exitg1 = 1;
          } else {
            iv++;
          }
        } else {
          k++;
          exitg1 = 2;
        }
      } while (exitg1 == 0);

      if (exitg1 == 1) {
        exitg2 = true;
      }
    }

    if (iscale) {
      memcpy(&b_A[0], &A[0], 9U * sizeof(real_T));
      anrm = 0.0;
      k = 0;
      exitg2 = false;
      while ((!exitg2) && (k < 3)) {
        iv = 0;
        do {
          exitg1 = 0;
          if (iv <= k) {
            absx = fabs(A[3 * k + iv]);
            if (rtIsNaN(absx)) {
              anrm = (rtNaN);
              exitg1 = 1;
            } else {
              if (absx > anrm) {
                anrm = absx;
              }

              iv++;
            }
          } else {
            k++;
            exitg1 = 2;
          }
        } while (exitg1 == 0);

        if (exitg1 == 1) {
          exitg2 = true;
        }
      }

      if (rtIsInf(anrm) || rtIsNaN(anrm)) {
        a__3[0] = (rtNaN);
        a__3[1] = (rtNaN);
        a__3[2] = (rtNaN);
      } else {
        iscale = false;
        if ((anrm > 0.0) && (anrm < 1.0010415475915505E-146)) {
          iscale = true;
          anrm = 1.0010415475915505E-146 / anrm;
          quadrotorsmodel2_xzlascl(1.0, anrm, b_A);
        } else if (anrm > 9.9895953610111751E+145) {
          iscale = true;
          anrm = 9.9895953610111751E+145 / anrm;
          quadrotorsmodel2_xzlascl(1.0, anrm, b_A);
        }

        for (k = 0; k < 2; k++) {
          e_tmp_tmp = 3 * k + k;
          e[k] = b_A[e_tmp_tmp + 1];
          absx = quadrotorsmodel2_xzlarfg(2 - k, &e[k], b_A, k * 3 + 3);
          if (absx != 0.0) {
            b_A[e_tmp_tmp + 1] = 1.0;
            for (iv = k + 1; iv < 3; iv++) {
              tau[iv - 1] = 0.0;
            }

            iv = 1 - k;
            b_iy = 3 - k;
            for (b_ix = 0; b_ix <= iv; b_ix++) {
              temp1_tmp = k + b_ix;
              temp1 = b_A[(3 * k + temp1_tmp) + 1] * absx;
              temp2 = 0.0;
              tau_tmp = (temp1_tmp + 1) * 3;
              tau[temp1_tmp] += b_A[((tau_tmp + k) + b_ix) + 1] * temp1;
              for (ii = b_ix + 2; ii < b_iy; ii++) {
                _mm_storeu_pd(&tmp_0[0], _mm_add_pd(_mm_mul_pd(_mm_set1_pd
                  (b_A[tau_tmp + 2]), _mm_set_pd(b_A[3 * k + 2], temp1)),
                  _mm_set_pd(temp2, tau[1])));
                tau[1] = tmp_0[0];
                temp2 = tmp_0[1];
              }

              tau[temp1_tmp] += absx * temp2;
            }

            iv = e_tmp_tmp + 1;
            b_iy = e_tmp_tmp + 1;
            temp1 = 0.0;
            ii = 1 - k;
            for (temp1_tmp = 0; temp1_tmp <= ii; temp1_tmp++) {
              temp1 += tau[k + temp1_tmp] * b_A[b_iy];
              b_iy++;
            }

            temp1 *= -0.5 * absx;
            if (!(temp1 == 0.0)) {
              b_iy = 2 - k;
              for (ii = 0; ii < b_iy; ii++) {
                tau_tmp = k + ii;
                tau[tau_tmp] += temp1 * b_A[iv];
                iv++;
              }
            }

            iv = 1 - k;
            ii = 3 - k;
            for (b_ix = 0; b_ix <= iv; b_ix++) {
              temp1_tmp = k + b_ix;
              temp1 = b_A[(3 * k + temp1_tmp) + 1];
              temp2 = tau[temp1_tmp];
              b_A_tmp = temp2 * temp1;
              b_iy = (temp1_tmp + 1) * 3;
              b_A[(temp1_tmp + b_iy) + 1] = (b_A[((b_iy + k) + b_ix) + 1] -
                b_A_tmp) - b_A_tmp;
              for (temp1_tmp = b_ix + 2; temp1_tmp < ii; temp1_tmp++) {
                b_A[b_iy + 2] = (b_A[b_iy + 2] - tau[1] * temp1) - b_A[3 * k + 2]
                  * temp2;
              }
            }
          }

          b_A[e_tmp_tmp + 1] = e[k];
          a__3[k] = b_A[e_tmp_tmp];
          tau[k] = absx;
        }

        a__3[2] = b_A[8];
        k = quadrotorsmodel2_xdsterf(a__3, e);
        if (k != 0) {
          a__3[0] = (rtNaN);
          a__3[1] = (rtNaN);
          a__3[2] = (rtNaN);
        } else if (iscale) {
          anrm = 1.0 / anrm;
          tmp = _mm_mul_pd(_mm_set1_pd(anrm), _mm_loadu_pd(&a__3[0]));
          _mm_storeu_pd(&a__3[0], tmp);
          a__3[2] *= anrm;
        }
      }

      V[0].re = a__3[0];
      V[0].im = 0.0;
      V[1].re = a__3[1];
      V[1].im = 0.0;
      V[2].re = a__3[2];
      V[2].im = 0.0;
    } else {
      iscale = true;
      k = 0;
      exitg2 = false;
      while ((!exitg2) && (k < 3)) {
        iv = 0;
        do {
          exitg1 = 0;
          if (iv <= k) {
            if (!(A[3 * k + iv] == -A[3 * iv + k])) {
              iscale = false;
              exitg1 = 1;
            } else {
              iv++;
            }
          } else {
            k++;
            exitg1 = 2;
          }
        } while (exitg1 == 0);

        if (exitg1 == 1) {
          exitg2 = true;
        }
      }

      if (iscale) {
        memcpy(&b_A[0], &A[0], 9U * sizeof(real_T));
        quadrotorsmodel2_xzgehrd(b_A, 1, 3);
        quadrotorsmodel2_xdlahqr(1, 3, b_A, &anrm, &k, a__3, wi);
        iv = (uint8_T)k;
        for (b_ix = 0; b_ix < iv; b_ix++) {
          V[b_ix].re = (rtNaN);
          V[b_ix].im = 0.0;
        }

        for (iv = k + 1; iv < 4; iv++) {
          V[iv - 1].re = 0.0;
          V[iv - 1].im = wi[iv - 1];
        }
      } else {
        quadrotorsmodel2_eigStandard(A, V);
      }
    }
  }
}

real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    int32_T tmp;
    int32_T tmp_0;
    if (u0 > 0.0) {
      tmp = 1;
    } else {
      tmp = -1;
    }

    if (u1 > 0.0) {
      tmp_0 = 1;
    } else {
      tmp_0 = -1;
    }

    y = atan2(tmp, tmp_0);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

/* Model step function */
void quadrotorsmodel2_step(void)
{
  if (rtmIsMajorTimeStep(quadrotorsmodel2_M)) {
    /* set solver stop time */
    if (!(quadrotorsmodel2_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&quadrotorsmodel2_M->solverInfo,
                            ((quadrotorsmodel2_M->Timing.clockTickH0 + 1) *
        quadrotorsmodel2_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&quadrotorsmodel2_M->solverInfo,
                            ((quadrotorsmodel2_M->Timing.clockTick0 + 1) *
        quadrotorsmodel2_M->Timing.stepSize0 +
        quadrotorsmodel2_M->Timing.clockTickH0 *
        quadrotorsmodel2_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(quadrotorsmodel2_M)) {
    quadrotorsmodel2_M->Timing.t[0] = rtsiGetT(&quadrotorsmodel2_M->solverInfo);
  }

  {
    __m128d tmp_1;
    __m128d tmp_2;
    __m128d tmp_3;
    __m128d tmp_4;
    __m128d tmp_6;
    __m128d tmp_7;
    creal_T unusedExpr[3];
    real_T (*lastU)[18];
    real_T A[16];
    real_T M_tmp[9];
    real_T R[9];
    real_T Rd[9];
    real_T Rd_0[9];
    real_T B[4];
    real_T rtb_f[4];
    real_T F_des_vec[3];
    real_T F_des_vec_tmp[3];
    real_T F_des_vec_tmp_0[3];
    real_T M[3];
    real_T b[3];
    real_T temp_vec[3];
    real_T tmp[3];
    real_T tmp_5[2];
    real_T R_tmp;
    real_T b_0;
    real_T b_1;
    real_T b_2;
    real_T c_phi;
    real_T c_psi;
    real_T c_th;
    real_T s_phi;
    real_T s_psi;
    real_T s_th;
    real_T tmp_0;
    int32_T b_ix;
    int32_T e;
    int32_T i;
    int32_T iy;
    int32_T jj;
    int32_T kAcol;
    int32_T vectorUB;
    int8_T ipiv[4];
    for (i = 0; i < 18; i++) {
      /* Integrator: '<Root>/Integrator' */
      c_phi = quadrotorsmodel2_X.Integrator_CSTATE[i];
      quadrotorsmodel2_B.Integrator[i] = c_phi;

      /* Outport: '<Root>/Out2' */
      quadrotorsmodel2_Y.Out2[i] = c_phi;
    }

    /* Derivative: '<Root>/Derivative1' */
    c_phi = quadrotorsmodel2_M->Timing.t[0];
    if ((quadrotorsmodel2_DW.TimeStampA >= c_phi) &&
        (quadrotorsmodel2_DW.TimeStampB >= c_phi)) {
      /* Derivative: '<Root>/Derivative1' */
      memset(&quadrotorsmodel2_B.Derivative1[0], 0, 18U * sizeof(real_T));
    } else {
      s_phi = quadrotorsmodel2_DW.TimeStampA;
      lastU = &quadrotorsmodel2_DW.LastUAtTimeA;
      if (quadrotorsmodel2_DW.TimeStampA < quadrotorsmodel2_DW.TimeStampB) {
        if (quadrotorsmodel2_DW.TimeStampB < c_phi) {
          s_phi = quadrotorsmodel2_DW.TimeStampB;
          lastU = &quadrotorsmodel2_DW.LastUAtTimeB;
        }
      } else if (quadrotorsmodel2_DW.TimeStampA >= c_phi) {
        s_phi = quadrotorsmodel2_DW.TimeStampB;
        lastU = &quadrotorsmodel2_DW.LastUAtTimeB;
      }

      c_phi -= s_phi;

      /* Derivative: '<Root>/Derivative1' */
      for (i = 0; i < 18; i++) {
        quadrotorsmodel2_B.Derivative1[i] = (quadrotorsmodel2_B.Integrator[i] -
          (*lastU)[i]) / c_phi;
      }
    }

    /* End of Derivative: '<Root>/Derivative1' */

    /* MATLAB Function: '<Root>/MATLAB Function' incorporates:
     *  Constant: '<Root>/Constant'
     *  Constant: '<Root>/Constant2'
     *  Constant: '<Root>/Constant3'
     *  Constant: '<Root>/Constant4'
     *  Constant: '<Root>/Constant5'
     *  Constant: '<Root>/Constant6'
     *  Constant: '<Root>/Constant7'
     *  Constant: '<Root>/Mass'
     *  Inport: '<Root>/Inport'
     *  Inport: '<Root>/Inport1'
     *  Inport: '<Root>/Inport15'
     *  Inport: '<Root>/Inport16'
     *  Inport: '<Root>/Inport2'
     *  Inport: '<Root>/Inport3'
     *  Inport: '<Root>/Inport4'
     *  Inport: '<Root>/Inport5'
     *  Inport: '<Root>/Inport6'
     *  Inport: '<Root>/Inport7'
     *  Inport: '<Root>/Inport8'
     *  MATLAB Function: '<Root>/MATLAB Function1'
     */
    memcpy(&A[0], &quadrotorsmodel2_P.K[0], sizeof(real_T) << 4U);
    c_phi = cos(quadrotorsmodel2_B.Integrator[3]);
    s_phi = sin(quadrotorsmodel2_B.Integrator[3]);
    c_th = cos(quadrotorsmodel2_B.Integrator[4]);
    s_th = sin(quadrotorsmodel2_B.Integrator[4]);
    c_psi = cos(quadrotorsmodel2_B.Integrator[5]);
    s_psi = sin(quadrotorsmodel2_B.Integrator[5]);
    R[0] = c_psi * c_th;
    R_tmp = c_psi * s_th;
    R[3] = R_tmp * s_phi - s_psi * c_phi;
    R[6] = R_tmp * c_phi + s_psi * s_phi;
    R[1] = s_psi * c_th;
    R_tmp = s_psi * s_th;
    R[4] = R_tmp * s_phi + c_psi * c_phi;
    R[7] = R_tmp * c_phi - c_psi * s_phi;
    R[2] = -s_th;
    R[5] = c_th * s_phi;
    R[8] = c_th * c_phi;
    F_des_vec_tmp[0] = 0.0;
    c_phi = quadrotorsmodel2_P.g * 0.0;
    F_des_vec_tmp_0[0] = c_phi;
    F_des_vec_tmp[1] = 0.0;
    F_des_vec_tmp_0[1] = c_phi;
    F_des_vec_tmp[2] = 1.0;
    F_des_vec_tmp_0[2] = quadrotorsmodel2_P.g;
    for (jj = 0; jj <= 6; jj += 2) {
      _mm_storeu_pd(&Rd_0[jj], _mm_mul_pd(_mm_loadu_pd(&quadrotorsmodel2_P.Kp[jj]),
        _mm_set1_pd(-1.0)));
    }

    for (jj = 8; jj < 9; jj++) {
      Rd_0[jj] = -quadrotorsmodel2_P.Kp[jj];
    }

    tmp_6 = _mm_sub_pd(_mm_loadu_pd(&quadrotorsmodel2_B.Integrator[0]),
                       _mm_set_pd(quadrotorsmodel2_U.Inport1,
      quadrotorsmodel2_U.Inport));
    _mm_storeu_pd(&F_des_vec[0], tmp_6);
    F_des_vec[2] = quadrotorsmodel2_B.Integrator[2] - quadrotorsmodel2_U.Inport2;
    tmp_6 = _mm_sub_pd(_mm_loadu_pd(&quadrotorsmodel2_B.Integrator[6]),
                       _mm_set_pd(quadrotorsmodel2_U.Inport4,
      quadrotorsmodel2_U.Inport3));
    _mm_storeu_pd(&tmp[0], tmp_6);
    tmp[2] = quadrotorsmodel2_B.Integrator[8] - quadrotorsmodel2_U.Inport5;
    c_psi = 0.0;
    s_phi = 0.0;
    s_th = 0.0;
    c_phi = 0.0;
    c_th = 0.0;
    s_psi = 0.0;
    for (jj = 0; jj < 3; jj++) {
      tmp_6 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&Rd_0[3 * jj]), _mm_set1_pd
        (F_des_vec[jj])), _mm_set_pd(s_phi, c_psi));
      _mm_storeu_pd(&tmp_5[0], tmp_6);
      c_psi = tmp_5[0];
      s_phi = tmp_5[1];
      _mm_storeu_pd(&tmp_5[0], _mm_add_pd(_mm_mul_pd(_mm_set_pd
        (quadrotorsmodel2_P.Kv[3 * jj], Rd_0[3 * jj + 2]), _mm_set_pd(tmp[jj],
        F_des_vec[jj])), _mm_set_pd(c_phi, s_th)));
      s_th = tmp_5[0];
      c_phi = tmp_5[1];
      _mm_storeu_pd(&tmp_5[0], _mm_add_pd(_mm_mul_pd(_mm_loadu_pd
        (&quadrotorsmodel2_P.Kv[3 * jj + 1]), _mm_set1_pd(tmp[jj])), _mm_set_pd
        (s_psi, c_th)));
      c_th = tmp_5[0];
      s_psi = tmp_5[1];
    }

    tmp_6 = _mm_add_pd(_mm_mul_pd(_mm_add_pd(_mm_loadu_pd(&F_des_vec_tmp_0[0]),
      _mm_set_pd(quadrotorsmodel2_U.Inport7, quadrotorsmodel2_U.Inport6)),
      _mm_set1_pd(quadrotorsmodel2_P.m)), _mm_sub_pd(_mm_set_pd(s_phi, c_psi),
      _mm_set_pd(c_th, c_phi)));
    _mm_storeu_pd(&F_des_vec[0], tmp_6);
    F_des_vec[2] = (quadrotorsmodel2_P.g + quadrotorsmodel2_U.Inport8) *
      quadrotorsmodel2_P.m + (s_th - s_psi);
    c_phi = quadrotorsmodel2_norm(F_des_vec);
    if (c_phi < 1.0E-6) {
      s_phi = 0.0;
      s_th = 0.0;
      c_th = 1.0;
    } else {
      tmp_6 = _mm_div_pd(_mm_loadu_pd(&F_des_vec[0]), _mm_set1_pd(c_phi));
      _mm_storeu_pd(&tmp_5[0], tmp_6);
      s_phi = tmp_5[0];
      s_th = tmp_5[1];
      c_th = F_des_vec[2] / c_phi;
    }

    b[0] = cos(quadrotorsmodel2_U.Inport15);
    b[1] = sin(quadrotorsmodel2_U.Inport15);
    _mm_storeu_pd(&temp_vec[0], _mm_sub_pd(_mm_mul_pd(_mm_set_pd(b[0], s_th),
      _mm_set_pd(c_th, 0.0)), _mm_mul_pd(_mm_set_pd(s_phi, b[1]), _mm_set_pd(0.0,
      c_th))));
    temp_vec[2] = s_phi * b[1] - b[0] * s_th;
    c_phi = quadrotorsmodel2_norm(temp_vec);
    if (c_phi < 1.0E-6) {
      c_phi = 0.0 * c_th;
      temp_vec[0] = c_phi - s_th;
      temp_vec[1] = s_phi - c_phi;
      temp_vec[2] = 0.0 * s_th - 0.0 * s_phi;
      c_phi = quadrotorsmodel2_norm(temp_vec);
      if (c_phi < 1.0E-6) {
        temp_vec[0] = 0.0;
        temp_vec[1] = 1.0;
        temp_vec[2] = 0.0;
      } else {
        tmp_6 = _mm_div_pd(_mm_loadu_pd(&temp_vec[0]), _mm_set1_pd(c_phi));
        _mm_storeu_pd(&temp_vec[0], tmp_6);
        temp_vec[2] /= c_phi;
      }
    } else {
      tmp_6 = _mm_div_pd(_mm_loadu_pd(&temp_vec[0]), _mm_set1_pd(c_phi));
      _mm_storeu_pd(&temp_vec[0], tmp_6);
      temp_vec[2] /= c_phi;
    }

    _mm_storeu_pd(&Rd[0], _mm_sub_pd(_mm_mul_pd(_mm_set_pd(s_phi, temp_vec[1]),
      _mm_set_pd(temp_vec[2], c_th)), _mm_mul_pd(_mm_set_pd(temp_vec[0], s_th),
      _mm_set_pd(c_th, temp_vec[2]))));
    Rd[2] = temp_vec[0] * s_th - s_phi * temp_vec[1];
    Rd[3] = temp_vec[0];
    Rd[6] = s_phi;
    Rd[4] = temp_vec[1];
    Rd[7] = s_th;
    Rd[5] = temp_vec[2];
    Rd[8] = c_th;
    b_0 = 0.0;
    b_1 = 0.0;
    b_2 = 0.0;
    for (jj = 0; jj < 3; jj++) {
      s_psi = (int32_T)F_des_vec_tmp[jj];
      tmp_6 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&R[3 * jj]), _mm_set1_pd(s_psi)),
                         _mm_set_pd(b_1, b_0));
      _mm_storeu_pd(&tmp_5[0], tmp_6);
      b_0 = tmp_5[0];
      b_1 = tmp_5[1];
      b_2 += R[3 * jj + 2] * s_psi;
    }

    b[2] = b_2;
    b[1] = b_1;
    b[0] = b_0;
    c_phi = 0.0;
    for (i = 0; i < 3; i++) {
      c_phi += F_des_vec[i] * b[i];
      s_phi = Rd[3 * i + 1];
      s_th = Rd[3 * i];
      c_psi = Rd[3 * i + 2];
      for (jj = 0; jj < 3; jj++) {
        M_tmp[jj + 3 * i] = (R[3 * jj + 1] * s_phi + R[3 * jj] * s_th) + R[3 *
          jj + 2] * c_psi;
      }
    }

    for (jj = 0; jj < 3; jj++) {
      s_phi = Rd[3 * jj + 1];
      s_th = Rd[3 * jj];
      c_psi = Rd[3 * jj + 2];
      for (i = 0; i < 3; i++) {
        iy = 3 * i + jj;
        Rd_0[iy] = ((R[3 * i + 1] * s_phi + R[3 * i] * s_th) + R[3 * i + 2] *
                    c_psi) - M_tmp[iy];
      }
    }

    for (jj = 0; jj <= 6; jj += 2) {
      tmp_6 = _mm_loadu_pd(&Rd_0[jj]);
      _mm_storeu_pd(&R[jj], _mm_mul_pd(_mm_set1_pd(0.5), tmp_6));
    }

    for (jj = 8; jj < 9; jj++) {
      R[jj] = 0.5 * Rd_0[jj];
    }

    quadrotorsmodel2_eig(quadrotorsmodel2_P.I, unusedExpr);
    quadrotorsmodel2_eig(quadrotorsmodel2_P.Kr, unusedExpr);
    b_0 = 0.0;
    b_1 = 0.0;
    b_2 = 0.0;
    for (jj = 0; jj < 3; jj++) {
      s_psi = quadrotorsmodel2_B.Integrator[jj + 9];
      _mm_storeu_pd(&tmp_5[0], _mm_add_pd(_mm_mul_pd(_mm_loadu_pd
        (&quadrotorsmodel2_P.I[3 * jj]), _mm_set1_pd(s_psi)), _mm_set_pd(b_1,
        b_0)));
      b_0 = tmp_5[0];
      b_1 = tmp_5[1];
      b_2 += quadrotorsmodel2_P.I[3 * jj + 2] * s_psi;
    }

    b[2] = b_2;
    b[1] = b_1;
    b[0] = b_0;
    for (jj = 0; jj <= 6; jj += 2) {
      _mm_storeu_pd(&Rd_0[jj], _mm_mul_pd(_mm_loadu_pd(&quadrotorsmodel2_P.Kr[jj]),
        _mm_set1_pd(-1.0)));
    }

    for (jj = 8; jj < 9; jj++) {
      Rd_0[jj] = -quadrotorsmodel2_P.Kr[jj];
    }

    M[0] = R[5];
    M[1] = R[6];
    M[2] = R[1];
    for (jj = 0; jj <= 0; jj += 2) {
      tmp_6 = _mm_loadu_pd(&M_tmp[jj + 3]);
      tmp_7 = _mm_set1_pd(0.0);
      tmp_2 = _mm_loadu_pd(&M_tmp[jj]);
      tmp_3 = _mm_loadu_pd(&M_tmp[jj + 6]);
      tmp_4 = _mm_loadu_pd(&quadrotorsmodel2_B.Integrator[jj + 9]);
      _mm_storeu_pd(&F_des_vec[jj], _mm_sub_pd(tmp_4, _mm_add_pd(_mm_add_pd
        (_mm_mul_pd(tmp_6, tmp_7), _mm_mul_pd(tmp_2, tmp_7)), _mm_mul_pd(tmp_3,
        _mm_set1_pd(quadrotorsmodel2_U.Inport16)))));
      _mm_storeu_pd(&tmp[jj], tmp_7);
    }

    for (jj = 2; jj < 3; jj++) {
      F_des_vec[jj] = quadrotorsmodel2_B.Integrator[jj + 9] - ((M_tmp[jj + 3] *
        0.0 + M_tmp[jj] * 0.0) + M_tmp[jj + 6] * quadrotorsmodel2_U.Inport16);
    }

    c_psi = 0.0;
    s_phi = 0.0;
    s_th = 0.0;
    s_psi = 0.0;
    R_tmp = 0.0;
    tmp_0 = 0.0;
    for (jj = 0; jj < 3; jj++) {
      tmp_6 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&Rd_0[3 * jj]), _mm_set1_pd
        (M[jj])), _mm_set_pd(R_tmp, s_psi));
      _mm_storeu_pd(&tmp_5[0], tmp_6);
      s_psi = tmp_5[0];
      R_tmp = tmp_5[1];
      _mm_storeu_pd(&tmp_5[0], _mm_add_pd(_mm_mul_pd(_mm_set_pd
        (quadrotorsmodel2_P.Kw[3 * jj], Rd_0[3 * jj + 2]), _mm_set_pd
        (F_des_vec[jj], M[jj])), _mm_set_pd(c_psi, tmp_0)));
      tmp_0 = tmp_5[0];
      c_psi = tmp_5[1];
      _mm_storeu_pd(&tmp_5[0], _mm_add_pd(_mm_mul_pd(_mm_loadu_pd
        (&quadrotorsmodel2_P.Kw[3 * jj + 1]), _mm_set1_pd(F_des_vec[jj])),
        _mm_set_pd(s_th, s_phi)));
      s_phi = tmp_5[0];
      s_th = tmp_5[1];
    }

    tmp[2] = tmp_0;
    tmp[1] = R_tmp;
    tmp[0] = s_psi;
    B[0] = c_phi;
    tmp_6 = _mm_add_pd(_mm_sub_pd(_mm_mul_pd(_mm_set_pd(b_0, b_2), _mm_loadu_pd(
      &quadrotorsmodel2_B.Integrator[10])), _mm_mul_pd(_mm_loadu_pd(&b[1]),
      _mm_set_pd(quadrotorsmodel2_B.Integrator[9],
                 quadrotorsmodel2_B.Integrator[11]))), _mm_sub_pd(_mm_loadu_pd
      (&tmp[0]), _mm_set_pd(s_phi, c_psi)));
    _mm_storeu_pd(&B[1], tmp_6);
    B[3] = (b_1 * quadrotorsmodel2_B.Integrator[9] - b_0 *
            quadrotorsmodel2_B.Integrator[10]) + (tmp_0 - s_th);
    ipiv[0] = 1;
    ipiv[1] = 2;
    ipiv[2] = 3;
    for (i = 0; i < 3; i++) {
      jj = i * 5;
      iy = 5 - i;
      b_ix = 0;
      c_phi = fabs(A[jj]);
      for (kAcol = 2; kAcol < iy; kAcol++) {
        s_phi = fabs(A[(jj + kAcol) - 1]);
        if (s_phi > c_phi) {
          b_ix = kAcol - 1;
          c_phi = s_phi;
        }
      }

      if (A[jj + b_ix] != 0.0) {
        if (b_ix != 0) {
          iy = i + b_ix;
          ipiv[i] = (int8_T)(iy + 1);
          c_phi = A[i];
          A[i] = A[iy];
          A[iy] = c_phi;
          c_phi = A[i + 4];
          A[i + 4] = A[iy + 4];
          A[iy + 4] = c_phi;
          c_phi = A[i + 8];
          A[i + 8] = A[iy + 8];
          A[iy + 8] = c_phi;
          c_phi = A[i + 12];
          A[i + 12] = A[iy + 12];
          A[iy + 12] = c_phi;
        }

        iy = (jj - i) + 4;
        b_ix = (((((iy - jj) - 1) / 2) << 1) + jj) + 2;
        vectorUB = b_ix - 2;
        for (kAcol = jj + 2; kAcol <= vectorUB; kAcol += 2) {
          tmp_6 = _mm_loadu_pd(&A[kAcol - 1]);
          _mm_storeu_pd(&A[kAcol - 1], _mm_div_pd(tmp_6, _mm_set1_pd(A[jj])));
        }

        for (kAcol = b_ix; kAcol <= iy; kAcol++) {
          A[kAcol - 1] /= A[jj];
        }
      }

      b_ix = jj + 6;
      vectorUB = 2 - i;
      for (kAcol = 0; kAcol <= vectorUB; kAcol++) {
        c_phi = A[((kAcol << 2) + jj) + 4];
        if (c_phi != 0.0) {
          e = (b_ix - i) + 2;
          for (iy = b_ix; iy <= e; iy++) {
            A[iy - 1] += A[((jj + iy) - b_ix) + 1] * -c_phi;
          }
        }

        b_ix += 4;
      }
    }

    if (ipiv[0] != 1) {
      c_phi = B[0];
      B[0] = B[ipiv[0] - 1];
      B[ipiv[0] - 1] = c_phi;
    }

    if (ipiv[1] != 2) {
      c_phi = B[1];
      B[1] = B[ipiv[1] - 1];
      B[ipiv[1] - 1] = c_phi;
    }

    if (ipiv[2] != 3) {
      c_phi = B[2];
      B[2] = B[ipiv[2] - 1];
      B[ipiv[2] - 1] = c_phi;
    }

    for (i = 0; i < 4; i++) {
      kAcol = i << 2;
      if (B[i] != 0.0) {
        for (jj = i + 2; jj < 5; jj++) {
          B[jj - 1] -= A[(jj + kAcol) - 1] * B[i];
        }
      }
    }

    for (i = 3; i >= 0; i--) {
      kAcol = i << 2;
      c_phi = B[i];
      if (c_phi != 0.0) {
        B[i] = c_phi / A[i + kAcol];
        for (jj = 0; jj < i; jj++) {
          B[jj] -= A[jj + kAcol] * B[i];
        }
      }
    }

    rtb_f[0] = fmin(fmax(B[0], 0.0), 100.0);
    rtb_f[1] = fmin(fmax(B[1], 0.0), 100.0);
    rtb_f[2] = fmin(fmax(B[2], 0.0), 100.0);
    c_phi = fmin(fmax(B[3], 0.0), 100.0);
    rtb_f[3] = c_phi;
    quadrotorsmodel2_B.Phee_value = rt_atan2d_snf(temp_vec[2], c_th);

    /* Outport: '<Root>/Out3' */
    quadrotorsmodel2_Y.Out3 = rtb_f[0];

    /* Outport: '<Root>/Out4' */
    quadrotorsmodel2_Y.Out4 = rtb_f[1];

    /* Outport: '<Root>/Out5' */
    quadrotorsmodel2_Y.Out5 = rtb_f[2];

    /* Outport: '<Root>/Out6' incorporates:
     *  MATLAB Function: '<Root>/MATLAB Function'
     */
    quadrotorsmodel2_Y.Out6 = c_phi;
    if (rtmIsMajorTimeStep(quadrotorsmodel2_M)) {
    }

    /* Gain: '<S3>/Gain' */
    c_psi = 0.0;
    s_phi = 0.0;
    s_th = 0.0;
    c_th = 0.0;
    for (jj = 0; jj < 4; jj++) {
      i = jj << 2;
      tmp_6 = _mm_set1_pd(rtb_f[jj]);
      _mm_storeu_pd(&tmp_5[0], _mm_add_pd(_mm_mul_pd(_mm_loadu_pd
        (&quadrotorsmodel2_P.K[i]), tmp_6), _mm_set_pd(s_phi, c_psi)));
      c_psi = tmp_5[0];
      s_phi = tmp_5[1];
      _mm_storeu_pd(&tmp_5[0], _mm_add_pd(_mm_mul_pd(_mm_loadu_pd
        (&quadrotorsmodel2_P.K[i + 2]), tmp_6), _mm_set_pd(c_th, s_th)));
      s_th = tmp_5[0];
      c_th = tmp_5[1];
    }

    /* End of Gain: '<S3>/Gain' */

    /* MATLAB Function: '<Root>/MATLAB Function1' incorporates:
     *  Constant: '<Root>/Constant4'
     *  Constant: '<Root>/Mass'
     *  SignalConversion generated from: '<S2>/ SFunction '
     */
    memset(&quadrotorsmodel2_B.xdot[0], 0, 18U * sizeof(real_T));
    quadrotorsmodel2_B.xdot[0] = quadrotorsmodel2_B.Integrator[3];
    quadrotorsmodel2_B.xdot[1] = quadrotorsmodel2_B.Integrator[4];
    quadrotorsmodel2_B.xdot[2] = quadrotorsmodel2_B.Integrator[5];
    c_phi = 1.0 / quadrotorsmodel2_P.m * c_psi;
    memcpy(&Rd_0[0], &quadrotorsmodel2_B.Integrator[6], 9U * sizeof(real_T));
    for (jj = 0; jj <= 0; jj += 2) {
      tmp_6 = _mm_loadu_pd(&Rd_0[jj + 3]);
      tmp_7 = _mm_set1_pd(c_phi);
      tmp_2 = _mm_set1_pd(0.0);
      tmp_3 = _mm_loadu_pd(&Rd_0[jj]);
      tmp_4 = _mm_loadu_pd(&Rd_0[jj + 6]);
      tmp_1 = _mm_loadu_pd(&F_des_vec_tmp_0[jj]);
      _mm_storeu_pd(&quadrotorsmodel2_B.xdot[jj + 3], _mm_sub_pd(tmp_1,
        _mm_add_pd(_mm_add_pd(_mm_mul_pd(_mm_mul_pd(tmp_6, tmp_7), tmp_2),
        _mm_mul_pd(_mm_mul_pd(tmp_7, tmp_3), tmp_2)), _mm_mul_pd(tmp_4, tmp_7))));
    }

    for (jj = 2; jj < 3; jj++) {
      quadrotorsmodel2_B.xdot[jj + 3] = F_des_vec_tmp_0[jj] - ((Rd_0[jj + 3] *
        c_phi * 0.0 + c_phi * Rd_0[jj] * 0.0) + Rd_0[jj + 6] * c_phi);
    }

    memcpy(&Rd_0[0], &quadrotorsmodel2_B.Integrator[6], 9U * sizeof(real_T));
    M_tmp[0] = 0.0;
    M_tmp[3] = -quadrotorsmodel2_B.Integrator[17];
    M_tmp[6] = quadrotorsmodel2_B.Integrator[16];
    M_tmp[1] = quadrotorsmodel2_B.Integrator[17];
    M_tmp[4] = 0.0;
    M_tmp[7] = -quadrotorsmodel2_B.Integrator[15];
    M_tmp[2] = -quadrotorsmodel2_B.Integrator[16];
    M_tmp[5] = quadrotorsmodel2_B.Integrator[15];
    M_tmp[8] = 0.0;
    for (jj = 0; jj < 3; jj++) {
      s_psi = 0.0;
      R_tmp = 0.0;
      tmp_0 = 0.0;
      for (i = 0; i < 3; i++) {
        c_phi = M_tmp[3 * jj + i];
        tmp_6 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&Rd_0[3 * i]), _mm_set1_pd
          (c_phi)), _mm_set_pd(R_tmp, s_psi));
        _mm_storeu_pd(&tmp_5[0], tmp_6);
        s_psi = tmp_5[0];
        R_tmp = tmp_5[1];
        tmp_0 += Rd_0[3 * i + 2] * c_phi;
      }

      R[3 * jj + 2] = tmp_0;
      R[3 * jj + 1] = R_tmp;
      R[3 * jj] = s_psi;
    }

    memcpy(&quadrotorsmodel2_B.xdot[6], &R[0], 9U * sizeof(real_T));
    b_0 = 0.0;
    b_1 = 0.0;
    b_2 = 0.0;
    for (jj = 0; jj < 3; jj++) {
      s_psi = quadrotorsmodel2_B.Integrator[jj + 15];
      _mm_storeu_pd(&tmp_5[0], _mm_add_pd(_mm_mul_pd(_mm_loadu_pd
        (&quadrotorsmodel2_P.I[3 * jj]), _mm_set1_pd(s_psi)), _mm_set_pd(b_1,
        b_0)));
      b_0 = tmp_5[0];
      b_1 = tmp_5[1];
      b_2 += quadrotorsmodel2_P.I[3 * jj + 2] * s_psi;
    }

    b[2] = b_2;
    b[1] = b_1;
    b[0] = b_0;
    memcpy(&R[0], &quadrotorsmodel2_P.I[0], 9U * sizeof(real_T));

    /* SignalConversion generated from: '<S2>/ SFunction ' incorporates:
     *  MATLAB Function: '<Root>/MATLAB Function1'
     */
    tmp_6 = _mm_sub_pd(_mm_set_pd(s_th, s_phi), _mm_sub_pd(_mm_mul_pd(_mm_set_pd
      (b_0, b_2), _mm_loadu_pd(&quadrotorsmodel2_B.Integrator[16])), _mm_mul_pd
      (_mm_loadu_pd(&b[1]), _mm_set_pd(quadrotorsmodel2_B.Integrator[15],
      quadrotorsmodel2_B.Integrator[17]))));

    /* MATLAB Function: '<Root>/MATLAB Function1' incorporates:
     *  Constant: '<Root>/Constant4'
     *  SignalConversion generated from: '<S2>/ SFunction '
     */
    _mm_storeu_pd(&F_des_vec_tmp[0], tmp_6);
    F_des_vec_tmp[2] = c_th - (b_1 * quadrotorsmodel2_B.Integrator[15] - b_0 *
      quadrotorsmodel2_B.Integrator[16]);
    i = 0;
    jj = 1;
    kAcol = 2;
    c_phi = fabs(quadrotorsmodel2_P.I[0]);
    s_phi = fabs(quadrotorsmodel2_P.I[1]);
    if (s_phi > c_phi) {
      c_phi = s_phi;
      i = 1;
      jj = 0;
    }

    if (fabs(quadrotorsmodel2_P.I[2]) > c_phi) {
      i = 2;
      jj = 1;
      kAcol = 0;
    }

    R[jj] = quadrotorsmodel2_P.I[jj] / quadrotorsmodel2_P.I[i];
    R[kAcol] /= R[i];
    R[jj + 3] -= R[i + 3] * R[jj];
    R[kAcol + 3] -= R[i + 3] * R[kAcol];
    R[jj + 6] -= R[i + 6] * R[jj];
    R[kAcol + 6] -= R[i + 6] * R[kAcol];
    if (fabs(R[kAcol + 3]) > fabs(R[jj + 3])) {
      iy = jj;
      jj = kAcol;
      kAcol = iy;
    }

    R[kAcol + 3] /= R[jj + 3];
    R[kAcol + 6] -= R[kAcol + 3] * R[jj + 6];
    b[1] = F_des_vec_tmp[jj] - F_des_vec_tmp[i] * R[jj];
    b[2] = (F_des_vec_tmp[kAcol] - F_des_vec_tmp[i] * R[kAcol]) - R[kAcol + 3] *
      b[1];
    b[2] /= R[kAcol + 6];
    _mm_storeu_pd(&b[0], _mm_sub_pd(_mm_set_pd(b[1], F_des_vec_tmp[i]),
      _mm_mul_pd(_mm_set_pd(R[jj + 6], R[i + 6]), _mm_set1_pd(b[2]))));
    b[1] /= R[jj + 3];
    b[0] -= R[i + 3] * b[1];
    b[0] /= R[i];
    quadrotorsmodel2_B.xdot[15] = b[0];
    quadrotorsmodel2_B.xdot[16] = b[1];
    quadrotorsmodel2_B.xdot[17] = b[2];
  }

  if (rtmIsMajorTimeStep(quadrotorsmodel2_M)) {
    /* Matfile logging */
    rt_UpdateTXYLogVars(quadrotorsmodel2_M->rtwLogInfo,
                        (quadrotorsmodel2_M->Timing.t));
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(quadrotorsmodel2_M)) {
    real_T (*lastU)[18];
    int32_T i;

    /* Update for Derivative: '<Root>/Derivative1' */
    if (quadrotorsmodel2_DW.TimeStampA == (rtInf)) {
      quadrotorsmodel2_DW.TimeStampA = quadrotorsmodel2_M->Timing.t[0];
      lastU = &quadrotorsmodel2_DW.LastUAtTimeA;
    } else if (quadrotorsmodel2_DW.TimeStampB == (rtInf)) {
      quadrotorsmodel2_DW.TimeStampB = quadrotorsmodel2_M->Timing.t[0];
      lastU = &quadrotorsmodel2_DW.LastUAtTimeB;
    } else if (quadrotorsmodel2_DW.TimeStampA < quadrotorsmodel2_DW.TimeStampB)
    {
      quadrotorsmodel2_DW.TimeStampA = quadrotorsmodel2_M->Timing.t[0];
      lastU = &quadrotorsmodel2_DW.LastUAtTimeA;
    } else {
      quadrotorsmodel2_DW.TimeStampB = quadrotorsmodel2_M->Timing.t[0];
      lastU = &quadrotorsmodel2_DW.LastUAtTimeB;
    }

    for (i = 0; i < 18; i++) {
      (*lastU)[i] = quadrotorsmodel2_B.Integrator[i];
    }

    /* End of Update for Derivative: '<Root>/Derivative1' */

    /* Update for Derivative: '<Root>/Derivative' */
    if (quadrotorsmodel2_DW.TimeStampA_g == (rtInf)) {
      quadrotorsmodel2_DW.TimeStampA_g = quadrotorsmodel2_M->Timing.t[0];
      lastU = &quadrotorsmodel2_DW.LastUAtTimeA_d;
    } else if (quadrotorsmodel2_DW.TimeStampB_g == (rtInf)) {
      quadrotorsmodel2_DW.TimeStampB_g = quadrotorsmodel2_M->Timing.t[0];
      lastU = &quadrotorsmodel2_DW.LastUAtTimeB_k;
    } else if (quadrotorsmodel2_DW.TimeStampA_g <
               quadrotorsmodel2_DW.TimeStampB_g) {
      quadrotorsmodel2_DW.TimeStampA_g = quadrotorsmodel2_M->Timing.t[0];
      lastU = &quadrotorsmodel2_DW.LastUAtTimeA_d;
    } else {
      quadrotorsmodel2_DW.TimeStampB_g = quadrotorsmodel2_M->Timing.t[0];
      lastU = &quadrotorsmodel2_DW.LastUAtTimeB_k;
    }

    for (i = 0; i < 18; i++) {
      (*lastU)[i] = quadrotorsmodel2_B.Derivative1[i];
    }

    /* End of Update for Derivative: '<Root>/Derivative' */
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(quadrotorsmodel2_M)) {
    /* signal main to stop simulation */
    {                                  /* Sample time: [0.0s, 0.0s] */
      if ((rtmGetTFinal(quadrotorsmodel2_M)!=-1) &&
          !((rtmGetTFinal(quadrotorsmodel2_M)-
             (((quadrotorsmodel2_M->Timing.clockTick1+
                quadrotorsmodel2_M->Timing.clockTickH1* 4294967296.0)) * 0.01)) >
            (((quadrotorsmodel2_M->Timing.clockTick1+
               quadrotorsmodel2_M->Timing.clockTickH1* 4294967296.0)) * 0.01) *
            (DBL_EPSILON))) {
        rtmSetErrorStatus(quadrotorsmodel2_M, "Simulation finished");
      }
    }

    rt_ertODEUpdateContinuousStates(&quadrotorsmodel2_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick0 and the high bits
     * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++quadrotorsmodel2_M->Timing.clockTick0)) {
      ++quadrotorsmodel2_M->Timing.clockTickH0;
    }

    quadrotorsmodel2_M->Timing.t[0] = rtsiGetSolverStopTime
      (&quadrotorsmodel2_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.01s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.01, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       * Timer of this task consists of two 32 bit unsigned integers.
       * The two integers represent the low bits Timing.clockTick1 and the high bits
       * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
       */
      quadrotorsmodel2_M->Timing.clockTick1++;
      if (!quadrotorsmodel2_M->Timing.clockTick1) {
        quadrotorsmodel2_M->Timing.clockTickH1++;
      }
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void quadrotorsmodel2_derivatives(void)
{
  XDot_quadrotorsmodel2_T *_rtXdot;
  _rtXdot = ((XDot_quadrotorsmodel2_T *) quadrotorsmodel2_M->derivs);

  /* Derivatives for Integrator: '<Root>/Integrator' */
  memcpy(&_rtXdot->Integrator_CSTATE[0], &quadrotorsmodel2_B.xdot[0], 18U *
         sizeof(real_T));
}

/* Model initialize function */
void quadrotorsmodel2_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)quadrotorsmodel2_M, 0,
                sizeof(RT_MODEL_quadrotorsmodel2_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&quadrotorsmodel2_M->solverInfo,
                          &quadrotorsmodel2_M->Timing.simTimeStep);
    rtsiSetTPtr(&quadrotorsmodel2_M->solverInfo, &rtmGetTPtr(quadrotorsmodel2_M));
    rtsiSetStepSizePtr(&quadrotorsmodel2_M->solverInfo,
                       &quadrotorsmodel2_M->Timing.stepSize0);
    rtsiSetdXPtr(&quadrotorsmodel2_M->solverInfo, &quadrotorsmodel2_M->derivs);
    rtsiSetContStatesPtr(&quadrotorsmodel2_M->solverInfo, (real_T **)
                         &quadrotorsmodel2_M->contStates);
    rtsiSetNumContStatesPtr(&quadrotorsmodel2_M->solverInfo,
      &quadrotorsmodel2_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&quadrotorsmodel2_M->solverInfo,
      &quadrotorsmodel2_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&quadrotorsmodel2_M->solverInfo,
      &quadrotorsmodel2_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&quadrotorsmodel2_M->solverInfo,
      &quadrotorsmodel2_M->periodicContStateRanges);
    rtsiSetContStateDisabledPtr(&quadrotorsmodel2_M->solverInfo, (boolean_T**)
      &quadrotorsmodel2_M->contStateDisabled);
    rtsiSetErrorStatusPtr(&quadrotorsmodel2_M->solverInfo, (&rtmGetErrorStatus
      (quadrotorsmodel2_M)));
    rtsiSetRTModelPtr(&quadrotorsmodel2_M->solverInfo, quadrotorsmodel2_M);
  }

  rtsiSetSimTimeStep(&quadrotorsmodel2_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetIsMinorTimeStepWithModeChange(&quadrotorsmodel2_M->solverInfo, false);
  rtsiSetIsContModeFrozen(&quadrotorsmodel2_M->solverInfo, false);
  quadrotorsmodel2_M->intgData.y = quadrotorsmodel2_M->odeY;
  quadrotorsmodel2_M->intgData.f[0] = quadrotorsmodel2_M->odeF[0];
  quadrotorsmodel2_M->intgData.f[1] = quadrotorsmodel2_M->odeF[1];
  quadrotorsmodel2_M->intgData.f[2] = quadrotorsmodel2_M->odeF[2];
  quadrotorsmodel2_M->contStates = ((X_quadrotorsmodel2_T *) &quadrotorsmodel2_X);
  quadrotorsmodel2_M->contStateDisabled = ((XDis_quadrotorsmodel2_T *)
    &quadrotorsmodel2_XDis);
  quadrotorsmodel2_M->Timing.tStart = (0.0);
  rtsiSetSolverData(&quadrotorsmodel2_M->solverInfo, (void *)
                    &quadrotorsmodel2_M->intgData);
  rtsiSetSolverName(&quadrotorsmodel2_M->solverInfo,"ode3");
  rtmSetTPtr(quadrotorsmodel2_M, &quadrotorsmodel2_M->Timing.tArray[0]);
  rtmSetTFinal(quadrotorsmodel2_M, 20.0);
  quadrotorsmodel2_M->Timing.stepSize0 = 0.01;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (NULL);
    quadrotorsmodel2_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(quadrotorsmodel2_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(quadrotorsmodel2_M->rtwLogInfo, (NULL));
    rtliSetLogT(quadrotorsmodel2_M->rtwLogInfo, "tout");
    rtliSetLogX(quadrotorsmodel2_M->rtwLogInfo, "");
    rtliSetLogXFinal(quadrotorsmodel2_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(quadrotorsmodel2_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(quadrotorsmodel2_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(quadrotorsmodel2_M->rtwLogInfo, 0);
    rtliSetLogDecimation(quadrotorsmodel2_M->rtwLogInfo, 1);
    rtliSetLogY(quadrotorsmodel2_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(quadrotorsmodel2_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(quadrotorsmodel2_M->rtwLogInfo, (NULL));
  }

  /* block I/O */
  (void) memset(((void *) &quadrotorsmodel2_B), 0,
                sizeof(B_quadrotorsmodel2_T));

  /* states (continuous) */
  {
    (void) memset((void *)&quadrotorsmodel2_X, 0,
                  sizeof(X_quadrotorsmodel2_T));
  }

  /* disabled states */
  {
    (void) memset((void *)&quadrotorsmodel2_XDis, 0,
                  sizeof(XDis_quadrotorsmodel2_T));
  }

  /* states (dwork) */
  (void) memset((void *)&quadrotorsmodel2_DW, 0,
                sizeof(DW_quadrotorsmodel2_T));

  /* external inputs */
  (void)memset(&quadrotorsmodel2_U, 0, sizeof(ExtU_quadrotorsmodel2_T));

  /* external outputs */
  (void)memset(&quadrotorsmodel2_Y, 0, sizeof(ExtY_quadrotorsmodel2_T));

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(quadrotorsmodel2_M->rtwLogInfo, 0.0,
    rtmGetTFinal(quadrotorsmodel2_M), quadrotorsmodel2_M->Timing.stepSize0,
    (&rtmGetErrorStatus(quadrotorsmodel2_M)));

  /* InitializeConditions for Integrator: '<Root>/Integrator' */
  memcpy(&quadrotorsmodel2_X.Integrator_CSTATE[0],
         &quadrotorsmodel2_P.Integrator_IC[0], 18U * sizeof(real_T));

  /* InitializeConditions for Derivative: '<Root>/Derivative1' */
  quadrotorsmodel2_DW.TimeStampA = (rtInf);
  quadrotorsmodel2_DW.TimeStampB = (rtInf);

  /* InitializeConditions for Derivative: '<Root>/Derivative' */
  quadrotorsmodel2_DW.TimeStampA_g = (rtInf);
  quadrotorsmodel2_DW.TimeStampB_g = (rtInf);
}

/* Model terminate function */
void quadrotorsmodel2_terminate(void)
{
  /* (no terminate code required) */
}
