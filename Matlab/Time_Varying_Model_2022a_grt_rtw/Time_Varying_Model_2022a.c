/*
 * Time_Varying_Model_2022a.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "Time_Varying_Model_2022a".
 *
 * Model version              : 4.13
 * Simulink Coder version : 23.2 (R2023b) 01-Aug-2023
 * C source code generated on : Thu Mar  7 23:32:27 2024
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objective: Debugging
 * Validation result: Not run
 */

#include "Time_Varying_Model_2022a.h"
#include "rtwtypes.h"
#include <math.h>
#include "rt_nonfinite.h"
#include "Time_Varying_Model_2022a_private.h"
#include <string.h>
#include <emmintrin.h>
#include "rt_defines.h"

/* Block signals (default storage) */
B_Time_Varying_Model_2022a_T Time_Varying_Model_2022a_B;

/* Continuous states */
X_Time_Varying_Model_2022a_T Time_Varying_Model_2022a_X;

/* Disabled State Vector */
XDis_Time_Varying_Model_2022a_T Time_Varying_Model_2022a_XDis;

/* Block states (default storage) */
DW_Time_Varying_Model_2022a_T Time_Varying_Model_2022a_DW;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Time_Varying_Model_2022a_T Time_Varying_Model_2022a_Y;

/* Real-time model */
static RT_MODEL_Time_Varying_Model_2_T Time_Varying_Model_2022a_M_;
RT_MODEL_Time_Varying_Model_2_T *const Time_Varying_Model_2022a_M =
  &Time_Varying_Model_2022a_M_;

/* Forward declaration for local functions */
static real_T Time_Varying_Model_2022a_norm(const real_T x[3]);
static void rate_scheduler(void);
real_T look1_binlxpw(real_T u0, const real_T bp0[], const real_T table[],
                     uint32_T maxIndex)
{
  real_T frac;
  real_T yL_0d0;
  uint32_T iLeft;

  /* Column-major Lookup 1-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    iLeft = 0U;
    frac = (u0 - bp0[0U]) / (bp0[1U] - bp0[0U]);
  } else if (u0 < bp0[maxIndex]) {
    uint32_T bpIdx;
    uint32_T iRght;

    /* Binary Search */
    bpIdx = maxIndex >> 1U;
    iLeft = 0U;
    iRght = maxIndex;
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex - 1U;
    frac = (u0 - bp0[maxIndex - 1U]) / (bp0[maxIndex] - bp0[maxIndex - 1U]);
  }

  /* Column-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  yL_0d0 = table[iLeft];
  return (table[iLeft + 1U] - yL_0d0) * frac + yL_0d0;
}

uint32_T plook_bincpa(real_T u, const real_T bp[], uint32_T maxIndex, real_T
                      *fraction, uint32_T *prevIndex)
{
  uint32_T bpIndex;

  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'on'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u <= bp[0U]) {
    bpIndex = 0U;
    *fraction = 0.0;
  } else if (u < bp[maxIndex]) {
    bpIndex = binsearch_u32d_prevIdx(u, bp, *prevIndex, maxIndex);
    *fraction = (u - bp[bpIndex]) / (bp[bpIndex + 1U] - bp[bpIndex]);
  } else {
    bpIndex = maxIndex;
    *fraction = 0.0;
  }

  *prevIndex = bpIndex;
  return bpIndex;
}

real_T intrp2d_la_pw(const uint32_T bpIndex[], const real_T frac[], const real_T
                     table[], const uint32_T stride, const uint32_T maxIndex[])
{
  real_T y;
  real_T yL_0d0;
  uint32_T offset_1d;

  /* Column-major Interpolation 2-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'on'
     Overflow mode: 'portable wrapping'
   */
  offset_1d = bpIndex[1U] * stride + bpIndex[0U];
  if (bpIndex[0U] == maxIndex[0U]) {
    y = table[offset_1d];
  } else {
    yL_0d0 = table[offset_1d];
    y = (table[offset_1d + 1U] - yL_0d0) * frac[0U] + yL_0d0;
  }

  if (bpIndex[1U] == maxIndex[1U]) {
  } else {
    offset_1d += stride;
    if (bpIndex[0U] == maxIndex[0U]) {
      yL_0d0 = table[offset_1d];
    } else {
      yL_0d0 = table[offset_1d];
      yL_0d0 += (table[offset_1d + 1U] - yL_0d0) * frac[0U];
    }

    y += (yL_0d0 - y) * frac[1U];
  }

  return y;
}

uint32_T plook_u32d_binckan(real_T u, const real_T bp[], uint32_T maxIndex)
{
  uint32_T bpIndex;

  /* Prelookup - Index only
     Index Search method: 'binary'
     Interpolation method: 'Use nearest'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u <= bp[0U]) {
    bpIndex = 0U;
  } else if (u < bp[maxIndex]) {
    bpIndex = binsearch_u32d(u, bp, maxIndex >> 1U, maxIndex);
    if ((bpIndex < maxIndex) && (bp[bpIndex + 1U] - u <= u - bp[bpIndex])) {
      bpIndex++;
    }
  } else {
    bpIndex = maxIndex;
  }

  return bpIndex;
}

uint32_T binsearch_u32d_prevIdx(real_T u, const real_T bp[], uint32_T startIndex,
  uint32_T maxIndex)
{
  uint32_T bpIndex;
  uint32_T found;
  uint32_T iLeft;
  uint32_T iRght;

  /* Binary Search using Previous Index */
  bpIndex = startIndex;
  iLeft = 0U;
  iRght = maxIndex;
  found = 0U;
  while (found == 0U) {
    if (u < bp[bpIndex]) {
      iRght = bpIndex - 1U;
      bpIndex = ((bpIndex + iLeft) - 1U) >> 1U;
    } else if (u < bp[bpIndex + 1U]) {
      found = 1U;
    } else {
      iLeft = bpIndex + 1U;
      bpIndex = ((bpIndex + iRght) + 1U) >> 1U;
    }
  }

  return bpIndex;
}

uint32_T binsearch_u32d(real_T u, const real_T bp[], uint32_T startIndex,
  uint32_T maxIndex)
{
  uint32_T bpIdx;
  uint32_T bpIndex;
  uint32_T iRght;

  /* Binary Search */
  bpIdx = startIndex;
  bpIndex = 0U;
  iRght = maxIndex;
  while (iRght - bpIndex > 1U) {
    if (u < bp[bpIdx]) {
      iRght = bpIdx;
    } else {
      bpIndex = bpIdx;
    }

    bpIdx = (iRght + bpIndex) >> 1U;
  }

  return bpIndex;
}

/*
 *         This function updates active task flag for each subrate.
 *         The function is called at model base rate, hence the
 *         generated code self-manages all its subrates.
 */
static void rate_scheduler(void)
{
  /* Compute which subrates run during the next base time step.  Subrates
   * are an integer multiple of the base rate counter.  Therefore, the subtask
   * counter is reset when it reaches its limit (zero means run).
   */
  (Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[2])++;
  if ((Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[2]) > 49) {/* Sample time: [0.5s, 0.0s] */
    Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[2] = 0;
  }

  (Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[3])++;
  if ((Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[3]) > 599) {/* Sample time: [6.0s, 0.0s] */
    Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[3] = 0;
  }
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
  int_T nXc = 25;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  Time_Varying_Model_2022a_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  Time_Varying_Model_2022a_step();
  Time_Varying_Model_2022a_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  Time_Varying_Model_2022a_step();
  Time_Varying_Model_2022a_derivatives();

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

real_T rt_urand_Upu32_Yd_f_pw_snf(uint32_T *u)
{
  uint32_T hi;
  uint32_T lo;

  /* Uniform random number generator (random number between 0 and 1)

     #define IA      16807                      magic multiplier = 7^5
     #define IM      2147483647                 modulus = 2^31-1
     #define IQ      127773                     IM div IA
     #define IR      2836                       IM modulo IA
     #define S       4.656612875245797e-10      reciprocal of 2^31-1
     test = IA * (seed % IQ) - IR * (seed/IQ)
     seed = test < 0 ? (test + IM) : test
     return (seed*S)
   */
  lo = *u % 127773U * 16807U;
  hi = *u / 127773U * 2836U;
  if (lo < hi) {
    *u = 2147483647U - (hi - lo);
  } else {
    *u = lo - hi;
  }

  return (real_T)*u * 4.6566128752457969E-10;
}

real_T rt_nrand_Upu32_Yd_f_pw_snf(uint32_T *u)
{
  real_T si;
  real_T sr;
  real_T y;

  /* Normal (Gaussian) random number generator */
  do {
    sr = 2.0 * rt_urand_Upu32_Yd_f_pw_snf(u) - 1.0;
    si = 2.0 * rt_urand_Upu32_Yd_f_pw_snf(u) - 1.0;
    si = sr * sr + si * si;
  } while (si > 1.0);

  y = sqrt(-2.0 * log(si) / si) * sr;
  return y;
}

real_T rt_powd_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else {
    real_T tmp;
    real_T tmp_0;
    tmp = fabs(u0);
    tmp_0 = fabs(u1);
    if (rtIsInf(u1)) {
      if (tmp == 1.0) {
        y = 1.0;
      } else if (tmp > 1.0) {
        if (u1 > 0.0) {
          y = (rtInf);
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = (rtInf);
      }
    } else if (tmp_0 == 0.0) {
      y = 1.0;
    } else if (tmp_0 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = (rtNaN);
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    int32_T tmp;
    int32_T tmp_0;
    if (u1 > 0.0) {
      tmp = 1;
    } else {
      tmp = -1;
    }

    if (u0 > 0.0) {
      tmp_0 = 1;
    } else {
      tmp_0 = -1;
    }

    y = atan2(tmp_0, tmp);
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

/* Function for MATLAB Function: '<S1>/MATLAB Function5' */
static real_T Time_Varying_Model_2022a_norm(const real_T x[3])
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

  return scale * sqrt(y);
}

/* Model step function */
void Time_Varying_Model_2022a_step(void)
{
  __m128d tmp_0;
  __m128d tmp_1;
  real_T rotmZYX[9];
  real_T tmp[6];
  real_T frac[2];
  real_T ang_cos;
  real_T ang_sin;
  real_T ct_idx_0;
  real_T ct_idx_1;
  real_T ct_idx_2;
  real_T rotmZYX_tmp;
  real_T st;
  real_T st_idx_1;
  real_T *tmp_2;
  int32_T K_tmp;
  int32_T K_tmp_0;
  int32_T i;
  int32_T i_0;
  uint32_T bpIndex[2];
  uint32_T bpIdx;
  int8_T rtAction;
  boolean_T limitedCache_idx_0;
  boolean_T limitedCache_idx_1;
  boolean_T limitedCache_idx_2;
  boolean_T limitedCache_idx_3;
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M)) {
    /* set solver stop time */
    if (!(Time_Varying_Model_2022a_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&Time_Varying_Model_2022a_M->solverInfo,
                            ((Time_Varying_Model_2022a_M->Timing.clockTickH0 + 1)
        * Time_Varying_Model_2022a_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&Time_Varying_Model_2022a_M->solverInfo,
                            ((Time_Varying_Model_2022a_M->Timing.clockTick0 + 1)
        * Time_Varying_Model_2022a_M->Timing.stepSize0 +
        Time_Varying_Model_2022a_M->Timing.clockTickH0 *
        Time_Varying_Model_2022a_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(Time_Varying_Model_2022a_M)) {
    Time_Varying_Model_2022a_M->Timing.t[0] = rtsiGetT
      (&Time_Varying_Model_2022a_M->solverInfo);
  }

  /* Integrator: '<S16>/Integrator4' */
  Time_Varying_Model_2022a_B.Integrator4 =
    Time_Varying_Model_2022a_X.Integrator4_CSTATE;

  /* Gain: '<S21>/Gain' */
  Time_Varying_Model_2022a_B.Gain = Time_Varying_Model_2022a_P.Gain_Gain_h *
    Time_Varying_Model_2022a_B.Integrator4;

  /* Outport: '<Root>/thetaOut' */
  Time_Varying_Model_2022a_Y.pitchAngle = Time_Varying_Model_2022a_B.Gain;

  /* Clock: '<Root>/Clock' */
  Time_Varying_Model_2022a_B.Clock = Time_Varying_Model_2022a_M->Timing.t[0];

  /* Lookup_n-D: '<Root>/OpenRocket Velocity' incorporates:
   *  Clock: '<Root>/Clock'
   */
  Time_Varying_Model_2022a_B.OpenRocketVelocity = look1_binlxpw
    (Time_Varying_Model_2022a_B.Clock,
     Time_Varying_Model_2022a_P.OpenRocketVelocity_bp01Data,
     Time_Varying_Model_2022a_P.OpenRocketVelocity_tableData, 222U);

  /* Switch: '<S15>/Switch' */
  if (Time_Varying_Model_2022a_B.OpenRocketVelocity >=
      Time_Varying_Model_2022a_P.Switch_Threshold) {
    /* Switch: '<S15>/Switch' incorporates:
     *  Constant: '<S15>/Constant'
     */
    Time_Varying_Model_2022a_B.Switch =
      Time_Varying_Model_2022a_P.Constant_Value_j;
  } else {
    /* Switch: '<S15>/Switch' incorporates:
     *  Constant: '<S15>/Constant1'
     */
    Time_Varying_Model_2022a_B.Switch =
      Time_Varying_Model_2022a_P.Constant1_Value_p;
  }

  /* End of Switch: '<S15>/Switch' */

  /* Lookup_n-D: '<S9>/1-D Lookup Table' incorporates:
   *  Lookup_n-D: '<Root>/OpenRocket Velocity'
   */
  bpIdx = plook_u32d_binckan(Time_Varying_Model_2022a_B.OpenRocketVelocity,
    Time_Varying_Model_2022a_P.uDLookupTable_bp01Data, 49U);

  /* Lookup_n-D: '<S9>/1-D Lookup Table' */
  Time_Varying_Model_2022a_B.uDLookupTable =
    Time_Varying_Model_2022a_P.uDLookupTable_tableData[bpIdx];

  /* Selector: '<S9>/Selector' */
  i_0 = Time_Varying_Model_2022a_B.uDLookupTable;
  for (i = 0; i < 6; i++) {
    /* Selector: '<S9>/Selector' incorporates:
     *  Constant: '<S9>/Constant'
     */
    K_tmp = i << 2;
    K_tmp_0 = (i_0 - 1) * 24 + K_tmp;

    /* Selector: '<S9>/Selector' incorporates:
     *  Constant: '<S9>/Constant'
     */
    Time_Varying_Model_2022a_B.K[K_tmp] =
      Time_Varying_Model_2022a_P.Constant_Value_l[K_tmp_0];
    Time_Varying_Model_2022a_B.K[K_tmp + 1] =
      Time_Varying_Model_2022a_P.Constant_Value_l[K_tmp_0 + 1];
    Time_Varying_Model_2022a_B.K[K_tmp + 2] =
      Time_Varying_Model_2022a_P.Constant_Value_l[K_tmp_0 + 2];
    Time_Varying_Model_2022a_B.K[K_tmp + 3] =
      Time_Varying_Model_2022a_P.Constant_Value_l[K_tmp_0 + 3];
  }

  /* Integrator: '<S16>/Integrator5' */
  Time_Varying_Model_2022a_B.Integrator5 =
    Time_Varying_Model_2022a_X.Integrator5_CSTATE;

  /* Integrator: '<S16>/Integrator3' */
  Time_Varying_Model_2022a_B.Integrator3 =
    Time_Varying_Model_2022a_X.Integrator3_CSTATE;

  /* Integrator: '<S16>/Integrator2' */
  Time_Varying_Model_2022a_B.Integrator2 =
    Time_Varying_Model_2022a_X.Integrator2_CSTATE;

  /* Integrator: '<S16>/Integrator' */
  Time_Varying_Model_2022a_B.Integrator =
    Time_Varying_Model_2022a_X.Integrator_CSTATE;

  /* Integrator: '<S16>/Integrator1' */
  Time_Varying_Model_2022a_B.Integrator1 =
    Time_Varying_Model_2022a_X.Integrator1_CSTATE;

  /* Sum: '<Root>/Sum1' incorporates:
   *  Constant: '<Root>/p'
   *  Constant: '<Root>/phi'
   *  Constant: '<Root>/psi'
   *  Constant: '<Root>/q'
   *  Constant: '<Root>/r'
   *  Constant: '<Root>/theta'
   */
  Time_Varying_Model_2022a_B.Sum1[0] = Time_Varying_Model_2022a_P.phi_Value -
    Time_Varying_Model_2022a_B.Integrator5;
  Time_Varying_Model_2022a_B.Sum1[1] = Time_Varying_Model_2022a_P.theta_Value -
    Time_Varying_Model_2022a_B.Integrator4;
  Time_Varying_Model_2022a_B.Sum1[2] = Time_Varying_Model_2022a_P.psi_Value -
    Time_Varying_Model_2022a_B.Integrator3;
  Time_Varying_Model_2022a_B.Sum1[3] = Time_Varying_Model_2022a_P.p_Value -
    Time_Varying_Model_2022a_B.Integrator2;
  Time_Varying_Model_2022a_B.Sum1[4] = Time_Varying_Model_2022a_P.q_Value -
    Time_Varying_Model_2022a_B.Integrator;
  Time_Varying_Model_2022a_B.Sum1[5] = Time_Varying_Model_2022a_P.r_Value -
    Time_Varying_Model_2022a_B.Integrator1;

  /* Product: '<Root>/Matrix Multiply' incorporates:
   *  Selector: '<S9>/Selector'
   */
  tmp_2 = &Time_Varying_Model_2022a_B.K[0];
  for (i = 0; i < 6; i++) {
    tmp[i] = Time_Varying_Model_2022a_B.Sum1[i];
  }

  for (i = 0; i < 4; i++) {
    ct_idx_2 = 0.0;
    for (i_0 = 0; i_0 < 6; i_0++) {
      ct_idx_2 += tmp_2[(i_0 << 2) + i] * tmp[i_0];
    }

    /* Product: '<Root>/Matrix Multiply' */
    Time_Varying_Model_2022a_B.MatrixMultiply[i] = ct_idx_2;

    /* Product: '<Root>/Product' */
    Time_Varying_Model_2022a_B.Product[i] = Time_Varying_Model_2022a_B.Switch *
      ct_idx_2;
  }

  /* End of Product: '<Root>/Matrix Multiply' */

  /* RateLimiter: '<Root>/Motor Speed' */
  if (Time_Varying_Model_2022a_DW.LastMajorTime == (rtInf)) {
    /* RateLimiter: '<Root>/Motor Speed' */
    Time_Varying_Model_2022a_B.MotorSpeed[0] =
      Time_Varying_Model_2022a_B.Product[0];
    Time_Varying_Model_2022a_B.MotorSpeed[1] =
      Time_Varying_Model_2022a_B.Product[1];
    Time_Varying_Model_2022a_B.MotorSpeed[2] =
      Time_Varying_Model_2022a_B.Product[2];
    Time_Varying_Model_2022a_B.MotorSpeed[3] =
      Time_Varying_Model_2022a_B.Product[3];
  } else {
    ang_cos = Time_Varying_Model_2022a_M->Timing.t[0] -
      Time_Varying_Model_2022a_DW.LastMajorTime;
    if (Time_Varying_Model_2022a_DW.LastMajorTime ==
        Time_Varying_Model_2022a_M->Timing.t[0]) {
      if (Time_Varying_Model_2022a_DW.PrevLimited[0]) {
        Time_Varying_Model_2022a_B.MotorSpeed[0] =
          Time_Varying_Model_2022a_DW.PrevY[0];
      } else {
        Time_Varying_Model_2022a_B.MotorSpeed[0] =
          Time_Varying_Model_2022a_B.Product[0];
      }

      if (Time_Varying_Model_2022a_DW.PrevLimited[1]) {
        Time_Varying_Model_2022a_B.MotorSpeed[1] =
          Time_Varying_Model_2022a_DW.PrevY[1];
      } else {
        Time_Varying_Model_2022a_B.MotorSpeed[1] =
          Time_Varying_Model_2022a_B.Product[1];
      }

      if (Time_Varying_Model_2022a_DW.PrevLimited[2]) {
        Time_Varying_Model_2022a_B.MotorSpeed[2] =
          Time_Varying_Model_2022a_DW.PrevY[2];
      } else {
        Time_Varying_Model_2022a_B.MotorSpeed[2] =
          Time_Varying_Model_2022a_B.Product[2];
      }

      if (Time_Varying_Model_2022a_DW.PrevLimited[3]) {
        Time_Varying_Model_2022a_B.MotorSpeed[3] =
          Time_Varying_Model_2022a_DW.PrevY[3];
      } else {
        Time_Varying_Model_2022a_B.MotorSpeed[3] =
          Time_Varying_Model_2022a_B.Product[3];
      }
    } else {
      ct_idx_2 = ang_cos * Time_Varying_Model_2022a_P.MotorSpeed_RisingLim;
      ang_sin = Time_Varying_Model_2022a_B.Product[0];
      st_idx_1 = Time_Varying_Model_2022a_DW.PrevY[0];
      ct_idx_0 = ang_sin - st_idx_1;
      if (ct_idx_0 > ct_idx_2) {
        Time_Varying_Model_2022a_B.MotorSpeed[0] = st_idx_1 + ct_idx_2;
        limitedCache_idx_0 = true;
      } else {
        ct_idx_2 = ang_cos * Time_Varying_Model_2022a_P.MotorSpeed_FallingLim;
        if (ct_idx_0 < ct_idx_2) {
          Time_Varying_Model_2022a_B.MotorSpeed[0] = st_idx_1 + ct_idx_2;
          limitedCache_idx_0 = true;
        } else {
          Time_Varying_Model_2022a_B.MotorSpeed[0] = ang_sin;
          limitedCache_idx_0 = false;
        }
      }

      ct_idx_2 = ang_cos * Time_Varying_Model_2022a_P.MotorSpeed_RisingLim;
      ang_sin = Time_Varying_Model_2022a_B.Product[1];
      st_idx_1 = Time_Varying_Model_2022a_DW.PrevY[1];
      ct_idx_0 = ang_sin - st_idx_1;
      if (ct_idx_0 > ct_idx_2) {
        Time_Varying_Model_2022a_B.MotorSpeed[1] = st_idx_1 + ct_idx_2;
        limitedCache_idx_1 = true;
      } else {
        ct_idx_2 = ang_cos * Time_Varying_Model_2022a_P.MotorSpeed_FallingLim;
        if (ct_idx_0 < ct_idx_2) {
          Time_Varying_Model_2022a_B.MotorSpeed[1] = st_idx_1 + ct_idx_2;
          limitedCache_idx_1 = true;
        } else {
          Time_Varying_Model_2022a_B.MotorSpeed[1] = ang_sin;
          limitedCache_idx_1 = false;
        }
      }

      ct_idx_2 = ang_cos * Time_Varying_Model_2022a_P.MotorSpeed_RisingLim;
      ang_sin = Time_Varying_Model_2022a_B.Product[2];
      st_idx_1 = Time_Varying_Model_2022a_DW.PrevY[2];
      ct_idx_0 = ang_sin - st_idx_1;
      if (ct_idx_0 > ct_idx_2) {
        Time_Varying_Model_2022a_B.MotorSpeed[2] = st_idx_1 + ct_idx_2;
        limitedCache_idx_2 = true;
      } else {
        ct_idx_2 = ang_cos * Time_Varying_Model_2022a_P.MotorSpeed_FallingLim;
        if (ct_idx_0 < ct_idx_2) {
          Time_Varying_Model_2022a_B.MotorSpeed[2] = st_idx_1 + ct_idx_2;
          limitedCache_idx_2 = true;
        } else {
          Time_Varying_Model_2022a_B.MotorSpeed[2] = ang_sin;
          limitedCache_idx_2 = false;
        }
      }

      ct_idx_2 = ang_cos * Time_Varying_Model_2022a_P.MotorSpeed_RisingLim;
      ang_sin = Time_Varying_Model_2022a_B.Product[3];
      st_idx_1 = Time_Varying_Model_2022a_DW.PrevY[3];
      ct_idx_0 = ang_sin - st_idx_1;
      if (ct_idx_0 > ct_idx_2) {
        Time_Varying_Model_2022a_B.MotorSpeed[3] = st_idx_1 + ct_idx_2;
        limitedCache_idx_3 = true;
      } else {
        ct_idx_2 = ang_cos * Time_Varying_Model_2022a_P.MotorSpeed_FallingLim;
        if (ct_idx_0 < ct_idx_2) {
          Time_Varying_Model_2022a_B.MotorSpeed[3] = st_idx_1 + ct_idx_2;
          limitedCache_idx_3 = true;
        } else {
          Time_Varying_Model_2022a_B.MotorSpeed[3] = ang_sin;
          limitedCache_idx_3 = false;
        }
      }

      if (rtsiIsModeUpdateTimeStep(&Time_Varying_Model_2022a_M->solverInfo)) {
        Time_Varying_Model_2022a_DW.PrevLimited[0] = limitedCache_idx_0;
        Time_Varying_Model_2022a_DW.PrevLimited[1] = limitedCache_idx_1;
        Time_Varying_Model_2022a_DW.PrevLimited[2] = limitedCache_idx_2;
        Time_Varying_Model_2022a_DW.PrevLimited[3] = limitedCache_idx_3;
      }
    }
  }

  /* End of RateLimiter: '<Root>/Motor Speed' */

  /* Saturate: '<Root>/Max Deflection' */
  ang_cos = Time_Varying_Model_2022a_B.MotorSpeed[0];
  if (ang_cos > Time_Varying_Model_2022a_P.MaxDeflection_UpperSat) {
    ang_cos = Time_Varying_Model_2022a_P.MaxDeflection_UpperSat;
  } else if (ang_cos < Time_Varying_Model_2022a_P.MaxDeflection_LowerSat) {
    ang_cos = Time_Varying_Model_2022a_P.MaxDeflection_LowerSat;
  }

  /* Saturate: '<Root>/Max Deflection' */
  Time_Varying_Model_2022a_B.MaxDeflection[0] = ang_cos;

  /* Saturate: '<Root>/Max Deflection' */
  ang_cos = Time_Varying_Model_2022a_B.MotorSpeed[1];
  if (ang_cos > Time_Varying_Model_2022a_P.MaxDeflection_UpperSat) {
    ang_cos = Time_Varying_Model_2022a_P.MaxDeflection_UpperSat;
  } else if (ang_cos < Time_Varying_Model_2022a_P.MaxDeflection_LowerSat) {
    ang_cos = Time_Varying_Model_2022a_P.MaxDeflection_LowerSat;
  }

  /* Saturate: '<Root>/Max Deflection' */
  Time_Varying_Model_2022a_B.MaxDeflection[1] = ang_cos;

  /* Saturate: '<Root>/Max Deflection' */
  ang_cos = Time_Varying_Model_2022a_B.MotorSpeed[2];
  if (ang_cos > Time_Varying_Model_2022a_P.MaxDeflection_UpperSat) {
    ang_cos = Time_Varying_Model_2022a_P.MaxDeflection_UpperSat;
  } else if (ang_cos < Time_Varying_Model_2022a_P.MaxDeflection_LowerSat) {
    ang_cos = Time_Varying_Model_2022a_P.MaxDeflection_LowerSat;
  }

  /* Saturate: '<Root>/Max Deflection' */
  Time_Varying_Model_2022a_B.MaxDeflection[2] = ang_cos;

  /* Saturate: '<Root>/Max Deflection' */
  ang_cos = Time_Varying_Model_2022a_B.MotorSpeed[3];
  if (ang_cos > Time_Varying_Model_2022a_P.MaxDeflection_UpperSat) {
    ang_cos = Time_Varying_Model_2022a_P.MaxDeflection_UpperSat;
  } else if (ang_cos < Time_Varying_Model_2022a_P.MaxDeflection_LowerSat) {
    ang_cos = Time_Varying_Model_2022a_P.MaxDeflection_LowerSat;
  }

  /* Saturate: '<Root>/Max Deflection' */
  Time_Varying_Model_2022a_B.MaxDeflection[3] = ang_cos;

  /* Gain: '<S22>/Gain' */
  Time_Varying_Model_2022a_B.Gain_j = Time_Varying_Model_2022a_P.Gain_Gain_i *
    Time_Varying_Model_2022a_B.MaxDeflection[0];

  /* Outport: '<Root>/x1Out' */
  Time_Varying_Model_2022a_Y.x1 = Time_Varying_Model_2022a_B.Gain_j;

  /* Gain: '<S23>/Gain' */
  Time_Varying_Model_2022a_B.Gain_l = Time_Varying_Model_2022a_P.Gain_Gain_oo *
    Time_Varying_Model_2022a_B.MaxDeflection[1];

  /* Outport: '<Root>/x2Out' */
  Time_Varying_Model_2022a_Y.x2 = Time_Varying_Model_2022a_B.Gain_l;

  /* Gain: '<S24>/Gain' */
  Time_Varying_Model_2022a_B.Gain_g = Time_Varying_Model_2022a_P.Gain_Gain_p *
    Time_Varying_Model_2022a_B.MaxDeflection[2];

  /* Outport: '<Root>/y1Out' */
  Time_Varying_Model_2022a_Y.y1 = Time_Varying_Model_2022a_B.Gain_g;

  /* Gain: '<S25>/Gain' */
  Time_Varying_Model_2022a_B.Gain_f = Time_Varying_Model_2022a_P.Gain_Gain_f *
    Time_Varying_Model_2022a_B.MaxDeflection[3];

  /* Outport: '<Root>/y2Out' */
  Time_Varying_Model_2022a_Y.y2 = Time_Varying_Model_2022a_B.Gain_f;

  /* Outport: '<Root>/vOut' */
  Time_Varying_Model_2022a_Y.v = Time_Varying_Model_2022a_B.OpenRocketVelocity;

  /* Lookup_n-D: '<Root>/OpenRocket alt' incorporates:
   *  Clock: '<Root>/Clock'
   */
  Time_Varying_Model_2022a_B.OpenRocketalt = look1_binlxpw
    (Time_Varying_Model_2022a_B.Clock,
     Time_Varying_Model_2022a_P.OpenRocketalt_bp01Data,
     Time_Varying_Model_2022a_P.OpenRocketalt_tableData, 222U);

  /* UnitConversion: '<S118>/Unit Conversion' */
  /* Unit Conversion - from: m to: ft
     Expression: output = (3.28084*input) + (0) */
  Time_Varying_Model_2022a_B.UnitConversion = 3.280839895013123 *
    Time_Varying_Model_2022a_B.OpenRocketalt;

  /* UnitConversion: '<S124>/Unit Conversion' */
  /* Unit Conversion - from: m/s to: ft/s
     Expression: output = (3.28084*input) + (0) */
  Time_Varying_Model_2022a_B.UnitConversion_e = 3.280839895013123 *
    Time_Varying_Model_2022a_B.OpenRocketVelocity;

  /* Saturate: '<S151>/Limit Function 10ft to 1000ft' */
  if (Time_Varying_Model_2022a_B.UnitConversion >
      Time_Varying_Model_2022a_P.LimitFunction10ftto1000ft_Upper) {
    /* Saturate: '<S151>/Limit Function 10ft to 1000ft' */
    Time_Varying_Model_2022a_B.LimitFunction10ftto1000ft =
      Time_Varying_Model_2022a_P.LimitFunction10ftto1000ft_Upper;
  } else if (Time_Varying_Model_2022a_B.UnitConversion <
             Time_Varying_Model_2022a_P.LimitFunction10ftto1000ft_Lower) {
    /* Saturate: '<S151>/Limit Function 10ft to 1000ft' */
    Time_Varying_Model_2022a_B.LimitFunction10ftto1000ft =
      Time_Varying_Model_2022a_P.LimitFunction10ftto1000ft_Lower;
  } else {
    /* Saturate: '<S151>/Limit Function 10ft to 1000ft' */
    Time_Varying_Model_2022a_B.LimitFunction10ftto1000ft =
      Time_Varying_Model_2022a_B.UnitConversion;
  }

  /* End of Saturate: '<S151>/Limit Function 10ft to 1000ft' */

  /* Fcn: '<S151>/Low Altitude Scale Length' */
  ang_cos = 0.000823 * Time_Varying_Model_2022a_B.LimitFunction10ftto1000ft +
    0.177;
  if (ang_cos < 0.0) {
    ang_sin = -rt_powd_snf(-ang_cos, 1.2);
  } else {
    ang_sin = rt_powd_snf(ang_cos, 1.2);
  }

  /* Fcn: '<S151>/Low Altitude Scale Length' */
  Time_Varying_Model_2022a_B.LowAltitudeScaleLength =
    Time_Varying_Model_2022a_B.LimitFunction10ftto1000ft / ang_sin;
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
    /* UnitConversion: '<S153>/Unit Conversion' incorporates:
     *  Constant: '<S152>/Medium//High Altitude'
     */
    /* Unit Conversion - from: m to: ft
       Expression: output = (3.28084*input) + (0) */
    Time_Varying_Model_2022a_B.UnitConversion_f = 3.280839895013123 *
      Time_Varying_Model_2022a_P.DrydenWindTurbulenceModelContin;
  }

  /* Saturate: '<S134>/Limit Height h<1000ft' */
  if (Time_Varying_Model_2022a_B.UnitConversion >
      Time_Varying_Model_2022a_P.LimitHeighth1000ft_UpperSat) {
    /* Saturate: '<S134>/Limit Height h<1000ft' */
    Time_Varying_Model_2022a_B.LimitHeighth1000ft =
      Time_Varying_Model_2022a_P.LimitHeighth1000ft_UpperSat;
  } else if (Time_Varying_Model_2022a_B.UnitConversion <
             Time_Varying_Model_2022a_P.LimitHeighth1000ft_LowerSat) {
    /* Saturate: '<S134>/Limit Height h<1000ft' */
    Time_Varying_Model_2022a_B.LimitHeighth1000ft =
      Time_Varying_Model_2022a_P.LimitHeighth1000ft_LowerSat;
  } else {
    /* Saturate: '<S134>/Limit Height h<1000ft' */
    Time_Varying_Model_2022a_B.LimitHeighth1000ft =
      Time_Varying_Model_2022a_B.UnitConversion;
  }

  /* End of Saturate: '<S134>/Limit Height h<1000ft' */

  /* Fcn: '<S134>/Low Altitude Intensity' */
  ang_cos = 0.000823 * Time_Varying_Model_2022a_B.LimitHeighth1000ft + 0.177;
  if (ang_cos < 0.0) {
    ang_sin = -rt_powd_snf(-ang_cos, 0.4);
  } else {
    ang_sin = rt_powd_snf(ang_cos, 0.4);
  }

  /* Fcn: '<S134>/Low Altitude Intensity' */
  Time_Varying_Model_2022a_B.LowAltitudeIntensity = 1.0 / ang_sin;
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
    /* UnitConversion: '<S125>/Unit Conversion' incorporates:
     *  Constant: '<S113>/Windspeed at 20ft (6m)'
     */
    /* Unit Conversion - from: m/s to: ft/s
       Expression: output = (3.28084*input) + (0) */
    Time_Varying_Model_2022a_B.UnitConversion_g = 3.280839895013123 *
      Time_Varying_Model_2022a_P.DrydenWindTurbulenceModelCont_i;

    /* Gain: '<S134>/sigma_wg ' */
    Time_Varying_Model_2022a_B.sigma_wg =
      Time_Varying_Model_2022a_P.sigma_wg_Gain *
      Time_Varying_Model_2022a_B.UnitConversion_g;
  }

  /* Product: '<S134>/sigma_ug, sigma_vg' */
  Time_Varying_Model_2022a_B.sigma_ugsigma_vg =
    Time_Varying_Model_2022a_B.LowAltitudeIntensity *
    Time_Varying_Model_2022a_B.sigma_wg;

  /* PreLookup: '<S133>/PreLook-Up Index Search  (altitude)' */
  Time_Varying_Model_2022a_B.PreLookUpIndexSearchaltitude_o1 = plook_bincpa
    (Time_Varying_Model_2022a_B.UnitConversion,
     Time_Varying_Model_2022a_P.PreLookUpIndexSearchaltitude_Br, 11U,
     &Time_Varying_Model_2022a_B.PreLookUpIndexSearchaltitude_o2,
     &Time_Varying_Model_2022a_DW.PreLookUpIndexSearchaltitude_DW);
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
    /* PreLookup: '<S133>/PreLook-Up Index Search  (prob of exceed)' incorporates:
     *  Constant: '<S133>/Probability of  Exceedance'
     */
    Time_Varying_Model_2022a_B.PreLookUpIndexSearchprobofexc_m = plook_bincpa
      (Time_Varying_Model_2022a_P.DrydenWindTurbulenceModelCon_au,
       Time_Varying_Model_2022a_P.PreLookUpIndexSearchprobofexcee, 6U,
       &Time_Varying_Model_2022a_B.PreLookUpIndexSearchprobofexcee,
       &Time_Varying_Model_2022a_DW.PreLookUpIndexSearchprobofexcee);
  }

  /* Interpolation_n-D: '<S133>/Medium//High Altitude Intensity' */
  ang_cos = Time_Varying_Model_2022a_B.PreLookUpIndexSearchaltitude_o2;
  frac[0] = ang_cos;
  ang_cos = Time_Varying_Model_2022a_B.PreLookUpIndexSearchprobofexcee;
  frac[1] = ang_cos;
  bpIdx = Time_Varying_Model_2022a_B.PreLookUpIndexSearchaltitude_o1;
  bpIndex[0] = bpIdx;
  bpIdx = Time_Varying_Model_2022a_B.PreLookUpIndexSearchprobofexc_m;
  bpIndex[1] = bpIdx;

  /* Interpolation_n-D: '<S133>/Medium//High Altitude Intensity' */
  Time_Varying_Model_2022a_B.MediumHighAltitudeIntensity = intrp2d_la_pw(bpIndex,
    frac, Time_Varying_Model_2022a_P.MediumHighAltitudeIntensity_Tab, 12U,
    Time_Varying_Model_2022a_P.MediumHighAltitudeIntensity_max);
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[2] == 0) {
    /* Sqrt: '<S126>/Sqrt1' incorporates:
     *  Constant: '<S126>/Constant1'
     */
    Time_Varying_Model_2022a_B.Sqrt1 = sqrt
      (Time_Varying_Model_2022a_P.WhiteNoise_Ts);

    /* Sqrt: '<S126>/Sqrt' incorporates:
     *  Constant: '<S126>/Constant'
     */
    ang_cos = sqrt(Time_Varying_Model_2022a_P.WhiteNoise_pwr[0]);
    Time_Varying_Model_2022a_B.Sqrt[0] = ang_cos;

    /* Product: '<S126>/Divide' */
    ang_cos /= Time_Varying_Model_2022a_B.Sqrt1;
    Time_Varying_Model_2022a_B.Divide[0] = ang_cos;

    /* RandomNumber: '<S126>/White Noise' */
    ct_idx_2 = Time_Varying_Model_2022a_DW.NextOutput[0];

    /* RandomNumber: '<S126>/White Noise' */
    Time_Varying_Model_2022a_B.WhiteNoise[0] = ct_idx_2;

    /* Product: '<S126>/Product' */
    Time_Varying_Model_2022a_B.Product_p[0] = ang_cos * ct_idx_2;

    /* Sqrt: '<S126>/Sqrt' incorporates:
     *  Constant: '<S126>/Constant'
     */
    ang_cos = sqrt(Time_Varying_Model_2022a_P.WhiteNoise_pwr[1]);
    Time_Varying_Model_2022a_B.Sqrt[1] = ang_cos;

    /* Product: '<S126>/Divide' */
    ang_cos /= Time_Varying_Model_2022a_B.Sqrt1;
    Time_Varying_Model_2022a_B.Divide[1] = ang_cos;

    /* RandomNumber: '<S126>/White Noise' */
    ct_idx_2 = Time_Varying_Model_2022a_DW.NextOutput[1];

    /* RandomNumber: '<S126>/White Noise' */
    Time_Varying_Model_2022a_B.WhiteNoise[1] = ct_idx_2;

    /* Product: '<S126>/Product' */
    Time_Varying_Model_2022a_B.Product_p[1] = ang_cos * ct_idx_2;

    /* Sqrt: '<S126>/Sqrt' incorporates:
     *  Constant: '<S126>/Constant'
     */
    ang_cos = sqrt(Time_Varying_Model_2022a_P.WhiteNoise_pwr[2]);
    Time_Varying_Model_2022a_B.Sqrt[2] = ang_cos;

    /* Product: '<S126>/Divide' */
    ang_cos /= Time_Varying_Model_2022a_B.Sqrt1;
    Time_Varying_Model_2022a_B.Divide[2] = ang_cos;

    /* RandomNumber: '<S126>/White Noise' */
    ct_idx_2 = Time_Varying_Model_2022a_DW.NextOutput[2];

    /* RandomNumber: '<S126>/White Noise' */
    Time_Varying_Model_2022a_B.WhiteNoise[2] = ct_idx_2;

    /* Product: '<S126>/Product' */
    Time_Varying_Model_2022a_B.Product_p[2] = ang_cos * ct_idx_2;

    /* Sqrt: '<S126>/Sqrt' incorporates:
     *  Constant: '<S126>/Constant'
     */
    ang_cos = sqrt(Time_Varying_Model_2022a_P.WhiteNoise_pwr[3]);
    Time_Varying_Model_2022a_B.Sqrt[3] = ang_cos;

    /* Product: '<S126>/Divide' */
    ang_cos /= Time_Varying_Model_2022a_B.Sqrt1;
    Time_Varying_Model_2022a_B.Divide[3] = ang_cos;

    /* RandomNumber: '<S126>/White Noise' */
    ct_idx_2 = Time_Varying_Model_2022a_DW.NextOutput[3];

    /* RandomNumber: '<S126>/White Noise' */
    Time_Varying_Model_2022a_B.WhiteNoise[3] = ct_idx_2;

    /* Product: '<S126>/Product' */
    Time_Varying_Model_2022a_B.Product_p[3] = ang_cos * ct_idx_2;
  }

  /* Outputs for Enabled SubSystem: '<S117>/Hugw(s)' incorporates:
   *  EnablePort: '<S130>/Enable'
   */
  if ((rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
       Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) &&
      rtsiIsModeUpdateTimeStep(&Time_Varying_Model_2022a_M->solverInfo)) {
    /* Constant: '<S117>/Constant' */
    if (Time_Varying_Model_2022a_P.DrydenWindTurbulenceModelCont_c > 0.0) {
      if (!Time_Varying_Model_2022a_DW.Hugws_MODE) {
        (void) memset(&(Time_Varying_Model_2022a_XDis.ug_p_CSTATE), 0,
                      2*sizeof(boolean_T));

        /* InitializeConditions for Integrator: '<S130>/ug_p' */
        Time_Varying_Model_2022a_X.ug_p_CSTATE[0] =
          Time_Varying_Model_2022a_P.ug_p_IC;
        Time_Varying_Model_2022a_X.ug_p_CSTATE[1] =
          Time_Varying_Model_2022a_P.ug_p_IC;
        Time_Varying_Model_2022a_DW.Hugws_MODE = true;
      }
    } else {
      ang_cos = rtmGetTStart(Time_Varying_Model_2022a_M);
      if ((((Time_Varying_Model_2022a_M->Timing.clockTick1+
             Time_Varying_Model_2022a_M->Timing.clockTickH1* 4294967296.0)) *
           0.01) == ang_cos) {
        (void) memset(&(Time_Varying_Model_2022a_XDis.ug_p_CSTATE), 1,
                      2*sizeof(boolean_T));
      }

      if (Time_Varying_Model_2022a_DW.Hugws_MODE) {
        (void) memset(&(Time_Varying_Model_2022a_XDis.ug_p_CSTATE), 1,
                      2*sizeof(boolean_T));

        /* Disable for Product: '<S130>/w1' incorporates:
         *  Outport: '<S130>/ugw'
         */
        Time_Varying_Model_2022a_B.w1_a[0] = Time_Varying_Model_2022a_P.ugw_Y0;
        Time_Varying_Model_2022a_B.w1_a[1] = Time_Varying_Model_2022a_P.ugw_Y0;
        Time_Varying_Model_2022a_DW.Hugws_MODE = false;
      }
    }
  }

  if (Time_Varying_Model_2022a_DW.Hugws_MODE) {
    /* Product: '<S130>/Lug//V' */
    Time_Varying_Model_2022a_B.LugV[0] =
      Time_Varying_Model_2022a_B.LowAltitudeScaleLength /
      Time_Varying_Model_2022a_B.UnitConversion_e;
    Time_Varying_Model_2022a_B.LugV[1] =
      Time_Varying_Model_2022a_B.UnitConversion_f /
      Time_Varying_Model_2022a_B.UnitConversion_e;

    /* Gain: '<S130>/(2//pi)' */
    ang_cos = Time_Varying_Model_2022a_B.LugV[0];

    /* Gain: '<S130>/(2//pi)' */
    Time_Varying_Model_2022a_B.upi_l[0] = Time_Varying_Model_2022a_P.upi_Gain *
      ang_cos;

    /* Sqrt: '<S130>/sqrt' */
    ang_sin = sqrt(Time_Varying_Model_2022a_B.upi_l[0]);
    Time_Varying_Model_2022a_B.sqrt_a[0] = ang_sin;

    /* Product: '<S130>/Lug//V1' */
    ang_sin *= Time_Varying_Model_2022a_B.Product_p[0];
    Time_Varying_Model_2022a_B.LugV1_m[0] = ang_sin;

    /* Integrator: '<S130>/ug_p' */
    st_idx_1 = Time_Varying_Model_2022a_X.ug_p_CSTATE[0];
    Time_Varying_Model_2022a_B.ug_p[0] = st_idx_1;

    /* Sum: '<S130>/Sum' */
    ang_sin -= st_idx_1;
    Time_Varying_Model_2022a_B.Sum_i[0] = ang_sin;

    /* Product: '<S130>/w' */
    Time_Varying_Model_2022a_B.w_b[0] = ang_sin / ang_cos;

    /* Gain: '<S130>/(2//pi)' */
    ang_cos = Time_Varying_Model_2022a_B.LugV[1];

    /* Gain: '<S130>/(2//pi)' */
    Time_Varying_Model_2022a_B.upi_l[1] = Time_Varying_Model_2022a_P.upi_Gain *
      ang_cos;

    /* Sqrt: '<S130>/sqrt' */
    ang_sin = sqrt(Time_Varying_Model_2022a_B.upi_l[1]);
    Time_Varying_Model_2022a_B.sqrt_a[1] = ang_sin;

    /* Product: '<S130>/Lug//V1' */
    ang_sin *= Time_Varying_Model_2022a_B.Product_p[0];
    Time_Varying_Model_2022a_B.LugV1_m[1] = ang_sin;

    /* Integrator: '<S130>/ug_p' */
    st_idx_1 = Time_Varying_Model_2022a_X.ug_p_CSTATE[1];
    Time_Varying_Model_2022a_B.ug_p[1] = st_idx_1;

    /* Sum: '<S130>/Sum' */
    ang_sin -= st_idx_1;
    Time_Varying_Model_2022a_B.Sum_i[1] = ang_sin;

    /* Product: '<S130>/w' */
    Time_Varying_Model_2022a_B.w_b[1] = ang_sin / ang_cos;

    /* Product: '<S130>/w1' */
    Time_Varying_Model_2022a_B.w1_a[0] = Time_Varying_Model_2022a_B.ug_p[0] *
      Time_Varying_Model_2022a_B.sigma_ugsigma_vg;
    Time_Varying_Model_2022a_B.w1_a[1] = Time_Varying_Model_2022a_B.ug_p[1] *
      Time_Varying_Model_2022a_B.MediumHighAltitudeIntensity;
  }

  /* End of Outputs for SubSystem: '<S117>/Hugw(s)' */

  /* Gain: '<S123>/Lv' */
  Time_Varying_Model_2022a_B.Lv[0] = Time_Varying_Model_2022a_P.Lv_Gain *
    Time_Varying_Model_2022a_B.LowAltitudeScaleLength;
  Time_Varying_Model_2022a_B.Lv[1] = Time_Varying_Model_2022a_P.Lv_Gain *
    Time_Varying_Model_2022a_B.UnitConversion_f;

  /* Outputs for Enabled SubSystem: '<S117>/Hvgw(s)' incorporates:
   *  EnablePort: '<S131>/Enable'
   */
  if ((rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
       Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) &&
      rtsiIsModeUpdateTimeStep(&Time_Varying_Model_2022a_M->solverInfo)) {
    /* Constant: '<S117>/Constant' */
    if (Time_Varying_Model_2022a_P.DrydenWindTurbulenceModelCont_c > 0.0) {
      if (!Time_Varying_Model_2022a_DW.Hvgws_MODE) {
        (void) memset(&(Time_Varying_Model_2022a_XDis.vg_p1_CSTATE), 0,
                      4*sizeof(boolean_T));

        /* InitializeConditions for Integrator: '<S131>/vg_p1' */
        Time_Varying_Model_2022a_X.vg_p1_CSTATE[0] =
          Time_Varying_Model_2022a_P.vg_p1_IC;

        /* InitializeConditions for Integrator: '<S131>/vgw_p2' */
        Time_Varying_Model_2022a_X.vgw_p2_CSTATE[0] =
          Time_Varying_Model_2022a_P.vgw_p2_IC;

        /* InitializeConditions for Integrator: '<S131>/vg_p1' */
        Time_Varying_Model_2022a_X.vg_p1_CSTATE[1] =
          Time_Varying_Model_2022a_P.vg_p1_IC;

        /* InitializeConditions for Integrator: '<S131>/vgw_p2' */
        Time_Varying_Model_2022a_X.vgw_p2_CSTATE[1] =
          Time_Varying_Model_2022a_P.vgw_p2_IC;
        Time_Varying_Model_2022a_DW.Hvgws_MODE = true;
      }
    } else {
      /* Outputs for Enabled SubSystem: '<S117>/Hugw(s)' incorporates:
       *  EnablePort: '<S130>/Enable'
       */
      ang_cos = rtmGetTStart(Time_Varying_Model_2022a_M);

      /* End of Outputs for SubSystem: '<S117>/Hugw(s)' */
      if ((((Time_Varying_Model_2022a_M->Timing.clockTick1+
             Time_Varying_Model_2022a_M->Timing.clockTickH1* 4294967296.0)) *
           0.01) == ang_cos) {
        (void) memset(&(Time_Varying_Model_2022a_XDis.vg_p1_CSTATE), 1,
                      4*sizeof(boolean_T));
      }

      if (Time_Varying_Model_2022a_DW.Hvgws_MODE) {
        (void) memset(&(Time_Varying_Model_2022a_XDis.vg_p1_CSTATE), 1,
                      4*sizeof(boolean_T));

        /* Disable for Product: '<S131>/w 1' incorporates:
         *  Outport: '<S131>/vgw'
         */
        Time_Varying_Model_2022a_B.w1[0] = Time_Varying_Model_2022a_P.vgw_Y0;
        Time_Varying_Model_2022a_B.w1[1] = Time_Varying_Model_2022a_P.vgw_Y0;
        Time_Varying_Model_2022a_DW.Hvgws_MODE = false;
      }
    }
  }

  if (Time_Varying_Model_2022a_DW.Hvgws_MODE) {
    /* Product: '<S131>/Lvg//V' */
    ang_cos = Time_Varying_Model_2022a_B.Lv[0] /
      Time_Varying_Model_2022a_B.UnitConversion_e;
    Time_Varying_Model_2022a_B.LvgV[0] = ang_cos;

    /* Gain: '<S131>/(1//pi)' */
    Time_Varying_Model_2022a_B.upi_h[0] = Time_Varying_Model_2022a_P.upi_Gain_m *
      ang_cos;

    /* Sqrt: '<S131>/sqrt' */
    ang_sin = sqrt(Time_Varying_Model_2022a_B.upi_h[0]);
    Time_Varying_Model_2022a_B.sqrt_ew[0] = ang_sin;

    /* Product: '<S131>/Lug//V1' */
    st_idx_1 = ang_sin * Time_Varying_Model_2022a_B.Product_p[1];
    Time_Varying_Model_2022a_B.LugV1_c[0] = st_idx_1;

    /* Integrator: '<S131>/vg_p1' */
    ang_sin = Time_Varying_Model_2022a_X.vg_p1_CSTATE[0];
    Time_Varying_Model_2022a_B.vg_p1[0] = ang_sin;

    /* Sum: '<S131>/Sum' */
    st_idx_1 -= ang_sin;
    Time_Varying_Model_2022a_B.Sum_c[0] = st_idx_1;

    /* Product: '<S131>/w' */
    st_idx_1 /= ang_cos;
    Time_Varying_Model_2022a_B.w_l[0] = st_idx_1;

    /* Product: '<S131>/Lvg//V ' */
    ct_idx_0 = st_idx_1 * ang_cos;
    Time_Varying_Model_2022a_B.LvgV_n[0] = ct_idx_0;

    /* Integrator: '<S131>/vgw_p2' */
    st_idx_1 = Time_Varying_Model_2022a_X.vgw_p2_CSTATE[0];
    Time_Varying_Model_2022a_B.vgw_p2[0] = st_idx_1;

    /* Gain: '<S131>/sqrt(3)' */
    ct_idx_0 *= Time_Varying_Model_2022a_P.sqrt3_Gain;
    Time_Varying_Model_2022a_B.sqrt3[0] = ct_idx_0;

    /* Sum: '<S131>/Sum1' */
    ang_sin = (ang_sin - st_idx_1) + ct_idx_0;
    Time_Varying_Model_2022a_B.Sum1_l[0] = ang_sin;

    /* Product: '<S131>/w ' */
    Time_Varying_Model_2022a_B.w_i[0] = ang_sin / ang_cos;

    /* Product: '<S131>/Lvg//V' */
    ang_cos = Time_Varying_Model_2022a_B.Lv[1] /
      Time_Varying_Model_2022a_B.UnitConversion_e;
    Time_Varying_Model_2022a_B.LvgV[1] = ang_cos;

    /* Gain: '<S131>/(1//pi)' */
    Time_Varying_Model_2022a_B.upi_h[1] = Time_Varying_Model_2022a_P.upi_Gain_m *
      ang_cos;

    /* Sqrt: '<S131>/sqrt' */
    ang_sin = sqrt(Time_Varying_Model_2022a_B.upi_h[1]);
    Time_Varying_Model_2022a_B.sqrt_ew[1] = ang_sin;

    /* Product: '<S131>/Lug//V1' */
    st_idx_1 = ang_sin * Time_Varying_Model_2022a_B.Product_p[1];
    Time_Varying_Model_2022a_B.LugV1_c[1] = st_idx_1;

    /* Integrator: '<S131>/vg_p1' */
    ang_sin = Time_Varying_Model_2022a_X.vg_p1_CSTATE[1];
    Time_Varying_Model_2022a_B.vg_p1[1] = ang_sin;

    /* Sum: '<S131>/Sum' */
    st_idx_1 -= ang_sin;
    Time_Varying_Model_2022a_B.Sum_c[1] = st_idx_1;

    /* Product: '<S131>/w' */
    st_idx_1 /= ang_cos;
    Time_Varying_Model_2022a_B.w_l[1] = st_idx_1;

    /* Product: '<S131>/Lvg//V ' */
    ct_idx_0 = st_idx_1 * ang_cos;
    Time_Varying_Model_2022a_B.LvgV_n[1] = ct_idx_0;

    /* Integrator: '<S131>/vgw_p2' */
    st_idx_1 = Time_Varying_Model_2022a_X.vgw_p2_CSTATE[1];
    Time_Varying_Model_2022a_B.vgw_p2[1] = st_idx_1;

    /* Gain: '<S131>/sqrt(3)' */
    ct_idx_0 *= Time_Varying_Model_2022a_P.sqrt3_Gain;
    Time_Varying_Model_2022a_B.sqrt3[1] = ct_idx_0;

    /* Sum: '<S131>/Sum1' */
    ang_sin = (ang_sin - st_idx_1) + ct_idx_0;
    Time_Varying_Model_2022a_B.Sum1_l[1] = ang_sin;

    /* Product: '<S131>/w ' */
    Time_Varying_Model_2022a_B.w_i[1] = ang_sin / ang_cos;

    /* Product: '<S131>/w 1' */
    Time_Varying_Model_2022a_B.w1[0] =
      Time_Varying_Model_2022a_B.sigma_ugsigma_vg *
      Time_Varying_Model_2022a_B.vgw_p2[0];
    Time_Varying_Model_2022a_B.w1[1] =
      Time_Varying_Model_2022a_B.MediumHighAltitudeIntensity *
      Time_Varying_Model_2022a_B.vgw_p2[1];
  }

  /* End of Outputs for SubSystem: '<S117>/Hvgw(s)' */

  /* Gain: '<S123>/Lw' */
  Time_Varying_Model_2022a_B.Lw[0] = Time_Varying_Model_2022a_P.Lw_Gain *
    Time_Varying_Model_2022a_B.LimitFunction10ftto1000ft;
  Time_Varying_Model_2022a_B.Lw[1] = Time_Varying_Model_2022a_P.Lw_Gain *
    Time_Varying_Model_2022a_B.UnitConversion_f;

  /* Outputs for Enabled SubSystem: '<S117>/Hwgw(s)' incorporates:
   *  EnablePort: '<S132>/Enable'
   */
  if ((rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
       Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) &&
      rtsiIsModeUpdateTimeStep(&Time_Varying_Model_2022a_M->solverInfo)) {
    /* Constant: '<S117>/Constant' */
    if (Time_Varying_Model_2022a_P.DrydenWindTurbulenceModelCont_c > 0.0) {
      if (!Time_Varying_Model_2022a_DW.Hwgws_MODE) {
        (void) memset(&(Time_Varying_Model_2022a_XDis.wg_p1_CSTATE), 0,
                      4*sizeof(boolean_T));

        /* InitializeConditions for Integrator: '<S132>/wg_p1' */
        Time_Varying_Model_2022a_X.wg_p1_CSTATE[0] =
          Time_Varying_Model_2022a_P.wg_p1_IC;

        /* InitializeConditions for Integrator: '<S132>/wg_p2' */
        Time_Varying_Model_2022a_X.wg_p2_CSTATE[0] =
          Time_Varying_Model_2022a_P.wg_p2_IC;

        /* InitializeConditions for Integrator: '<S132>/wg_p1' */
        Time_Varying_Model_2022a_X.wg_p1_CSTATE[1] =
          Time_Varying_Model_2022a_P.wg_p1_IC;

        /* InitializeConditions for Integrator: '<S132>/wg_p2' */
        Time_Varying_Model_2022a_X.wg_p2_CSTATE[1] =
          Time_Varying_Model_2022a_P.wg_p2_IC;
        Time_Varying_Model_2022a_DW.Hwgws_MODE = true;
      }
    } else {
      /* Outputs for Enabled SubSystem: '<S117>/Hugw(s)' incorporates:
       *  EnablePort: '<S130>/Enable'
       */
      ang_cos = rtmGetTStart(Time_Varying_Model_2022a_M);

      /* End of Outputs for SubSystem: '<S117>/Hugw(s)' */
      if ((((Time_Varying_Model_2022a_M->Timing.clockTick1+
             Time_Varying_Model_2022a_M->Timing.clockTickH1* 4294967296.0)) *
           0.01) == ang_cos) {
        (void) memset(&(Time_Varying_Model_2022a_XDis.wg_p1_CSTATE), 1,
                      4*sizeof(boolean_T));
      }

      if (Time_Varying_Model_2022a_DW.Hwgws_MODE) {
        (void) memset(&(Time_Varying_Model_2022a_XDis.wg_p1_CSTATE), 1,
                      4*sizeof(boolean_T));

        /* Disable for Product: '<S132>/Lwg//V 1' incorporates:
         *  Outport: '<S132>/wgw'
         */
        Time_Varying_Model_2022a_B.LwgV1[0] = Time_Varying_Model_2022a_P.wgw_Y0;
        Time_Varying_Model_2022a_B.LwgV1[1] = Time_Varying_Model_2022a_P.wgw_Y0;
        Time_Varying_Model_2022a_DW.Hwgws_MODE = false;
      }
    }
  }

  if (Time_Varying_Model_2022a_DW.Hwgws_MODE) {
    /* Product: '<S132>/Lwg//V' */
    ang_cos = Time_Varying_Model_2022a_B.Lw[0] /
      Time_Varying_Model_2022a_B.UnitConversion_e;
    Time_Varying_Model_2022a_B.LwgV[0] = ang_cos;

    /* Gain: '<S132>/1//pi' */
    Time_Varying_Model_2022a_B.upi[0] = Time_Varying_Model_2022a_P.upi_Gain_l *
      ang_cos;

    /* Sqrt: '<S132>/sqrt1' */
    ang_sin = sqrt(Time_Varying_Model_2022a_B.upi[0]);
    Time_Varying_Model_2022a_B.sqrt1[0] = ang_sin;

    /* Product: '<S132>/Lug//V1' */
    ang_sin *= Time_Varying_Model_2022a_B.Product_p[2];
    Time_Varying_Model_2022a_B.LugV1[0] = ang_sin;

    /* Integrator: '<S132>/wg_p1' */
    st_idx_1 = Time_Varying_Model_2022a_X.wg_p1_CSTATE[0];
    Time_Varying_Model_2022a_B.wg_p1[0] = st_idx_1;

    /* Sum: '<S132>/Sum' */
    ang_sin -= st_idx_1;
    Time_Varying_Model_2022a_B.Sum_ht[0] = ang_sin;

    /* Product: '<S132>/w' */
    Time_Varying_Model_2022a_B.w[0] = ang_sin / ang_cos;

    /* Product: '<S132>/Lwg//V' */
    ang_cos = Time_Varying_Model_2022a_B.Lw[1] /
      Time_Varying_Model_2022a_B.UnitConversion_e;
    Time_Varying_Model_2022a_B.LwgV[1] = ang_cos;

    /* Gain: '<S132>/1//pi' */
    Time_Varying_Model_2022a_B.upi[1] = Time_Varying_Model_2022a_P.upi_Gain_l *
      ang_cos;

    /* Sqrt: '<S132>/sqrt1' */
    ang_sin = sqrt(Time_Varying_Model_2022a_B.upi[1]);
    Time_Varying_Model_2022a_B.sqrt1[1] = ang_sin;

    /* Product: '<S132>/Lug//V1' */
    ang_sin *= Time_Varying_Model_2022a_B.Product_p[2];
    Time_Varying_Model_2022a_B.LugV1[1] = ang_sin;

    /* Integrator: '<S132>/wg_p1' */
    st_idx_1 = Time_Varying_Model_2022a_X.wg_p1_CSTATE[1];
    Time_Varying_Model_2022a_B.wg_p1[1] = st_idx_1;

    /* Sum: '<S132>/Sum' */
    ang_sin -= st_idx_1;
    Time_Varying_Model_2022a_B.Sum_ht[1] = ang_sin;

    /* Product: '<S132>/w' */
    Time_Varying_Model_2022a_B.w[1] = ang_sin / ang_cos;
    if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
        Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
      /* Sqrt: '<S132>/sqrt' incorporates:
       *  Constant: '<S132>/Constant'
       */
      Time_Varying_Model_2022a_B.sqrt_d = sqrt
        (Time_Varying_Model_2022a_P.Constant_Value);
    }

    /* Product: '<S132>/Lwg//V ' */
    ang_cos = Time_Varying_Model_2022a_B.LwgV[0];

    /* Product: '<S132>/Lwg//V ' */
    ang_sin = Time_Varying_Model_2022a_B.w[0] *
      Time_Varying_Model_2022a_B.sqrt_d * ang_cos;
    Time_Varying_Model_2022a_B.LwgV_p[0] = ang_sin;

    /* Integrator: '<S132>/wg_p2' */
    st_idx_1 = Time_Varying_Model_2022a_X.wg_p2_CSTATE[0];
    Time_Varying_Model_2022a_B.wg_p2[0] = st_idx_1;

    /* Sum: '<S132>/Sum1' */
    ang_sin += Time_Varying_Model_2022a_B.wg_p1[0] - st_idx_1;
    Time_Varying_Model_2022a_B.Sum1_b[0] = ang_sin;

    /* Product: '<S132>/w ' */
    Time_Varying_Model_2022a_B.w_o[0] = ang_sin / ang_cos;

    /* Product: '<S132>/Lwg//V ' */
    ang_cos = Time_Varying_Model_2022a_B.LwgV[1];

    /* Product: '<S132>/Lwg//V ' */
    ang_sin = Time_Varying_Model_2022a_B.w[1] *
      Time_Varying_Model_2022a_B.sqrt_d * ang_cos;
    Time_Varying_Model_2022a_B.LwgV_p[1] = ang_sin;

    /* Integrator: '<S132>/wg_p2' */
    st_idx_1 = Time_Varying_Model_2022a_X.wg_p2_CSTATE[1];
    Time_Varying_Model_2022a_B.wg_p2[1] = st_idx_1;

    /* Sum: '<S132>/Sum1' */
    ang_sin += Time_Varying_Model_2022a_B.wg_p1[1] - st_idx_1;
    Time_Varying_Model_2022a_B.Sum1_b[1] = ang_sin;

    /* Product: '<S132>/w ' */
    Time_Varying_Model_2022a_B.w_o[1] = ang_sin / ang_cos;

    /* Product: '<S132>/Lwg//V 1' */
    Time_Varying_Model_2022a_B.LwgV1[0] = Time_Varying_Model_2022a_B.sigma_wg *
      Time_Varying_Model_2022a_B.wg_p2[0];
    Time_Varying_Model_2022a_B.LwgV1[1] =
      Time_Varying_Model_2022a_B.MediumHighAltitudeIntensity *
      Time_Varying_Model_2022a_B.wg_p2[1];
  }

  /* End of Outputs for SubSystem: '<S117>/Hwgw(s)' */

  /* MATLAB Function: '<S41>/RPY to DCM' */
  /* :  DCM=zeros(3); */
  /* :  eul = [yaw pitch roll]; */
  ang_sin = Time_Varying_Model_2022a_B.Integrator3;
  st_idx_1 = Time_Varying_Model_2022a_B.Integrator4;
  ang_cos = Time_Varying_Model_2022a_B.Integrator5;

  /* :  rotmZYX = eul2rotm(eul); */
  st = ang_sin;
  ct_idx_2 = st;
  ct_idx_2 = cos(ct_idx_2);
  ct_idx_0 = ct_idx_2;
  st = sin(st);
  ang_sin = st;
  st = st_idx_1;
  ct_idx_2 = st;
  ct_idx_2 = cos(ct_idx_2);
  ct_idx_1 = ct_idx_2;
  st = sin(st);
  st_idx_1 = st;
  st = ang_cos;
  ct_idx_2 = st;
  ct_idx_2 = cos(ct_idx_2);
  st = sin(st);
  ang_cos = st;
  rotmZYX[0] = ct_idx_0 * ct_idx_1;
  st = st_idx_1 * ang_cos;
  rotmZYX[3] = st * ct_idx_0 - ang_sin * ct_idx_2;
  rotmZYX_tmp = st_idx_1 * ct_idx_2;
  rotmZYX[6] = rotmZYX_tmp * ct_idx_0 + ang_sin * ang_cos;
  rotmZYX[1] = ang_sin * ct_idx_1;
  rotmZYX[4] = st * ang_sin + ct_idx_0 * ct_idx_2;
  rotmZYX[7] = rotmZYX_tmp * ang_sin - ct_idx_0 * ang_cos;
  rotmZYX[2] = -st_idx_1;
  rotmZYX[5] = ct_idx_1 * ang_cos;
  rotmZYX[8] = ct_idx_1 * ct_idx_2;

  /* :  DCM = rotmZYX.'; */
  for (i_0 = 0; i_0 < 3; i_0++) {
    Time_Varying_Model_2022a_B.DCM[3 * i_0] = rotmZYX[i_0];
    Time_Varying_Model_2022a_B.DCM[3 * i_0 + 1] = rotmZYX[i_0 + 3];
    Time_Varying_Model_2022a_B.DCM[3 * i_0 + 2] = rotmZYX[i_0 + 6];
  }

  /* End of MATLAB Function: '<S41>/RPY to DCM' */
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
    /* UnitConversion: '<S115>/Unit Conversion' incorporates:
     *  Constant: '<S113>/Wind direction'
     */
    /* Unit Conversion - from: deg to: rad
       Expression: output = (0.0174533*input) + (0) */
    Time_Varying_Model_2022a_B.UnitConversion_gl = 0.017453292519943295 *
      Time_Varying_Model_2022a_P.DrydenWindTurbulenceModelCont_m;
  }

  /* If: '<S122>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' incorporates:
   *  Merge: '<S146>/Merge'
   *  Product: '<S149>/Product'
   */
  if (rtsiIsModeUpdateTimeStep(&Time_Varying_Model_2022a_M->solverInfo)) {
    if (Time_Varying_Model_2022a_B.UnitConversion <= 1000.0) {
      rtAction = 0;
    } else if (Time_Varying_Model_2022a_B.UnitConversion >= 2000.0) {
      rtAction = 1;
    } else {
      rtAction = 2;
    }

    Time_Varying_Model_2022a_DW.ifHeightMaxlowaltitudeelseifHei = rtAction;
  } else {
    rtAction = Time_Varying_Model_2022a_DW.ifHeightMaxlowaltitudeelseifHei;
  }

  switch (rtAction) {
   case 0:
    /* Outputs for IfAction SubSystem: '<S122>/Low altitude  velocities' incorporates:
     *  ActionPort: '<S144>/Action Port'
     */
    /* SignalConversion generated from: '<S149>/Vector Concatenate' incorporates:
     *  Concatenate: '<S149>/Vector Concatenate'
     */
    Time_Varying_Model_2022a_B.VectorConcatenate_b[2] =
      Time_Varying_Model_2022a_B.LwgV1[0];

    /* Trigonometry: '<S150>/Trigonometric Function' */
    ang_sin = Time_Varying_Model_2022a_B.UnitConversion_gl;
    ang_cos = sin(ang_sin);
    ang_sin = cos(ang_sin);

    /* Trigonometry: '<S150>/Trigonometric Function' */
    Time_Varying_Model_2022a_B.TrigonometricFunction_o1_h = ang_cos;

    /* Trigonometry: '<S150>/Trigonometric Function' */
    Time_Varying_Model_2022a_B.TrigonometricFunction_o2_j = ang_sin;

    /* Product: '<S150>/Product2' */
    Time_Varying_Model_2022a_B.Product2_l[0] = Time_Varying_Model_2022a_B.w1_a[0]
      * Time_Varying_Model_2022a_B.TrigonometricFunction_o2_j;
    Time_Varying_Model_2022a_B.Product2_l[1] = Time_Varying_Model_2022a_B.w1[0] *
      Time_Varying_Model_2022a_B.TrigonometricFunction_o2_j;

    /* Product: '<S150>/Product1' */
    Time_Varying_Model_2022a_B.Product1_m[0] =
      Time_Varying_Model_2022a_B.TrigonometricFunction_o1_h *
      Time_Varying_Model_2022a_B.w1_a[0];
    Time_Varying_Model_2022a_B.Product1_m[1] =
      Time_Varying_Model_2022a_B.TrigonometricFunction_o1_h *
      Time_Varying_Model_2022a_B.w1[0];

    /* Sum: '<S150>/Sum' incorporates:
     *  Concatenate: '<S149>/Vector Concatenate'
     */
    Time_Varying_Model_2022a_B.VectorConcatenate_b[0] =
      Time_Varying_Model_2022a_B.Product2_l[0] -
      Time_Varying_Model_2022a_B.Product1_m[1];

    /* Sum: '<S150>/Sum1' incorporates:
     *  Concatenate: '<S149>/Vector Concatenate'
     */
    Time_Varying_Model_2022a_B.VectorConcatenate_b[1] =
      Time_Varying_Model_2022a_B.Product1_m[0] +
      Time_Varying_Model_2022a_B.Product2_l[1];

    /* Product: '<S149>/Product' incorporates:
     *  Concatenate: '<S149>/Vector Concatenate'
     */
    memcpy(&rotmZYX[0], &Time_Varying_Model_2022a_B.DCM[0], 9U * sizeof(real_T));
    ang_sin = Time_Varying_Model_2022a_B.VectorConcatenate_b[0];
    st_idx_1 = Time_Varying_Model_2022a_B.VectorConcatenate_b[1];
    ang_cos = Time_Varying_Model_2022a_B.VectorConcatenate_b[2];

    /* End of Outputs for SubSystem: '<S122>/Low altitude  velocities' */
    for (i = 0; i <= 0; i += 2) {
      /* Outputs for IfAction SubSystem: '<S122>/Low altitude  velocities' incorporates:
       *  ActionPort: '<S144>/Action Port'
       */
      tmp_0 = _mm_loadu_pd(&rotmZYX[i]);
      tmp_0 = _mm_mul_pd(tmp_0, _mm_set1_pd(ang_sin));
      tmp_1 = _mm_loadu_pd(&rotmZYX[i + 3]);
      tmp_1 = _mm_mul_pd(tmp_1, _mm_set1_pd(st_idx_1));

      /* End of Outputs for SubSystem: '<S122>/Low altitude  velocities' */
      tmp_0 = _mm_add_pd(tmp_1, tmp_0);

      /* Outputs for IfAction SubSystem: '<S122>/Low altitude  velocities' incorporates:
       *  ActionPort: '<S144>/Action Port'
       */
      tmp_1 = _mm_loadu_pd(&rotmZYX[i + 6]);
      tmp_1 = _mm_mul_pd(tmp_1, _mm_set1_pd(ang_cos));

      /* End of Outputs for SubSystem: '<S122>/Low altitude  velocities' */
      tmp_0 = _mm_add_pd(tmp_1, tmp_0);

      /* Outputs for IfAction SubSystem: '<S122>/Low altitude  velocities' incorporates:
       *  ActionPort: '<S144>/Action Port'
       */
      _mm_storeu_pd(&Time_Varying_Model_2022a_B.Product_i[i], tmp_0);
      _mm_storeu_pd(&Time_Varying_Model_2022a_B.Merge[i], tmp_0);

      /* End of Outputs for SubSystem: '<S122>/Low altitude  velocities' */
    }

    /* Outputs for IfAction SubSystem: '<S122>/Low altitude  velocities' incorporates:
     *  ActionPort: '<S144>/Action Port'
     */
    for (i = 2; i < 3; i++) {
      /* Product: '<S149>/Product' */
      ct_idx_0 = rotmZYX[i] * ang_sin;
      ct_idx_0 += rotmZYX[i + 3] * st_idx_1;
      ct_idx_0 += rotmZYX[i + 6] * ang_cos;
      Time_Varying_Model_2022a_B.Product_i[i] = ct_idx_0;

      /* Merge: '<S146>/Merge' incorporates:
       *  Product: '<S149>/Product'
       *  Reshape: '<S149>/Reshape1'
       */
      Time_Varying_Model_2022a_B.Merge[i] = ct_idx_0;
    }

    /* End of Outputs for SubSystem: '<S122>/Low altitude  velocities' */
    break;

   case 1:
    /* Outputs for IfAction SubSystem: '<S122>/Medium//High  altitude velocities' incorporates:
     *  ActionPort: '<S145>/Action Port'
     */
    /* Merge: '<S146>/Merge' incorporates:
     *  Gain: '<S145>/Gain'
     */
    Time_Varying_Model_2022a_B.Merge[0] = Time_Varying_Model_2022a_P.Gain_Gain_o
      * Time_Varying_Model_2022a_B.w1_a[1];
    Time_Varying_Model_2022a_B.Merge[1] = Time_Varying_Model_2022a_P.Gain_Gain_o
      * Time_Varying_Model_2022a_B.w1[1];
    Time_Varying_Model_2022a_B.Merge[2] = Time_Varying_Model_2022a_P.Gain_Gain_o
      * Time_Varying_Model_2022a_B.LwgV1[1];

    /* End of Outputs for SubSystem: '<S122>/Medium//High  altitude velocities' */
    break;

   default:
    /* Outputs for IfAction SubSystem: '<S122>/Interpolate  velocities' incorporates:
     *  ActionPort: '<S143>/Action Port'
     */
    /* Trigonometry: '<S148>/Trigonometric Function' */
    ang_sin = Time_Varying_Model_2022a_B.UnitConversion_gl;
    ang_cos = sin(ang_sin);
    ang_sin = cos(ang_sin);

    /* Trigonometry: '<S148>/Trigonometric Function' */
    Time_Varying_Model_2022a_B.TrigonometricFunction_o1 = ang_cos;

    /* Trigonometry: '<S148>/Trigonometric Function' */
    Time_Varying_Model_2022a_B.TrigonometricFunction_o2 = ang_sin;

    /* Product: '<S148>/Product2' */
    Time_Varying_Model_2022a_B.Product2_jq[0] = Time_Varying_Model_2022a_B.w1_a
      [0] * Time_Varying_Model_2022a_B.TrigonometricFunction_o2;
    Time_Varying_Model_2022a_B.Product2_jq[1] = Time_Varying_Model_2022a_B.w1[0]
      * Time_Varying_Model_2022a_B.TrigonometricFunction_o2;

    /* Product: '<S148>/Product1' */
    Time_Varying_Model_2022a_B.Product1_o[0] =
      Time_Varying_Model_2022a_B.TrigonometricFunction_o1 *
      Time_Varying_Model_2022a_B.w1_a[0];
    Time_Varying_Model_2022a_B.Product1_o[1] =
      Time_Varying_Model_2022a_B.TrigonometricFunction_o1 *
      Time_Varying_Model_2022a_B.w1[0];

    /* Sum: '<S148>/Sum' incorporates:
     *  Concatenate: '<S147>/Vector Concatenate'
     */
    Time_Varying_Model_2022a_B.VectorConcatenate[0] =
      Time_Varying_Model_2022a_B.Product2_jq[0] -
      Time_Varying_Model_2022a_B.Product1_o[1];

    /* Sum: '<S148>/Sum1' incorporates:
     *  Concatenate: '<S147>/Vector Concatenate'
     */
    Time_Varying_Model_2022a_B.VectorConcatenate[1] =
      Time_Varying_Model_2022a_B.Product1_o[0] +
      Time_Varying_Model_2022a_B.Product2_jq[1];

    /* SignalConversion generated from: '<S147>/Vector Concatenate' incorporates:
     *  Concatenate: '<S147>/Vector Concatenate'
     */
    Time_Varying_Model_2022a_B.VectorConcatenate[2] =
      Time_Varying_Model_2022a_B.LwgV1[0];

    /* Product: '<S147>/Product' incorporates:
     *  Concatenate: '<S147>/Vector Concatenate'
     */
    memcpy(&rotmZYX[0], &Time_Varying_Model_2022a_B.DCM[0], 9U * sizeof(real_T));
    ang_sin = Time_Varying_Model_2022a_B.VectorConcatenate[0];
    st_idx_1 = Time_Varying_Model_2022a_B.VectorConcatenate[1];
    ang_cos = Time_Varying_Model_2022a_B.VectorConcatenate[2];

    /* End of Outputs for SubSystem: '<S122>/Interpolate  velocities' */
    for (i_0 = 0; i_0 <= 0; i_0 += 2) {
      /* Outputs for IfAction SubSystem: '<S122>/Interpolate  velocities' incorporates:
       *  ActionPort: '<S143>/Action Port'
       */
      tmp_0 = _mm_loadu_pd(&rotmZYX[i_0]);
      tmp_0 = _mm_mul_pd(tmp_0, _mm_set1_pd(ang_sin));
      tmp_1 = _mm_loadu_pd(&rotmZYX[i_0 + 3]);
      tmp_1 = _mm_mul_pd(tmp_1, _mm_set1_pd(st_idx_1));

      /* End of Outputs for SubSystem: '<S122>/Interpolate  velocities' */
      tmp_0 = _mm_add_pd(tmp_1, tmp_0);

      /* Outputs for IfAction SubSystem: '<S122>/Interpolate  velocities' incorporates:
       *  ActionPort: '<S143>/Action Port'
       */
      tmp_1 = _mm_loadu_pd(&rotmZYX[i_0 + 6]);
      tmp_1 = _mm_mul_pd(tmp_1, _mm_set1_pd(ang_cos));

      /* End of Outputs for SubSystem: '<S122>/Interpolate  velocities' */
      tmp_0 = _mm_add_pd(tmp_1, tmp_0);

      /* Outputs for IfAction SubSystem: '<S122>/Interpolate  velocities' incorporates:
       *  ActionPort: '<S143>/Action Port'
       */
      _mm_storeu_pd(&Time_Varying_Model_2022a_B.Product_oc[i_0], tmp_0);

      /* End of Outputs for SubSystem: '<S122>/Interpolate  velocities' */
    }

    /* Outputs for IfAction SubSystem: '<S122>/Interpolate  velocities' incorporates:
     *  ActionPort: '<S143>/Action Port'
     */
    for (i_0 = 2; i_0 < 3; i_0++) {
      /* Product: '<S147>/Product' */
      ct_idx_0 = rotmZYX[i_0] * ang_sin;
      ct_idx_0 += rotmZYX[i_0 + 3] * st_idx_1;
      ct_idx_0 += rotmZYX[i_0 + 6] * ang_cos;
      Time_Varying_Model_2022a_B.Product_oc[i_0] = ct_idx_0;
    }

    /* Sum: '<S143>/Sum2' incorporates:
     *  Product: '<S147>/Product'
     */
    Time_Varying_Model_2022a_B.Sum2_np[0] = Time_Varying_Model_2022a_B.w1_a[1] -
      Time_Varying_Model_2022a_B.Product_oc[0];
    Time_Varying_Model_2022a_B.Sum2_np[1] = Time_Varying_Model_2022a_B.w1[1] -
      Time_Varying_Model_2022a_B.Product_oc[1];
    Time_Varying_Model_2022a_B.Sum2_np[2] = Time_Varying_Model_2022a_B.LwgV1[1]
      - Time_Varying_Model_2022a_B.Product_oc[2];

    /* Sum: '<S143>/Sum1' incorporates:
     *  Constant: '<S143>/max_height_low'
     */
    Time_Varying_Model_2022a_B.Sum1_kf =
      Time_Varying_Model_2022a_B.UnitConversion -
      Time_Varying_Model_2022a_P.max_height_low_Value_l;

    /* Sum: '<S143>/Sum' incorporates:
     *  Constant: '<S143>/max_height_low'
     *  Constant: '<S143>/min_height_high'
     */
    Time_Varying_Model_2022a_B.Sum_jj =
      Time_Varying_Model_2022a_P.min_height_high_Value_h -
      Time_Varying_Model_2022a_P.max_height_low_Value_l;

    /* Product: '<S143>/Product1' */
    ang_cos = Time_Varying_Model_2022a_B.Sum2_np[0] *
      Time_Varying_Model_2022a_B.Sum1_kf / Time_Varying_Model_2022a_B.Sum_jj;

    /* End of Outputs for SubSystem: '<S122>/Interpolate  velocities' */
    Time_Varying_Model_2022a_B.Product1_h[0] = ang_cos;

    /* Outputs for IfAction SubSystem: '<S122>/Interpolate  velocities' incorporates:
     *  ActionPort: '<S143>/Action Port'
     */
    /* Merge: '<S146>/Merge' incorporates:
     *  Sum: '<S143>/Sum3'
     */
    Time_Varying_Model_2022a_B.Merge[0] = Time_Varying_Model_2022a_B.Product_oc
      [0] + ang_cos;

    /* Product: '<S143>/Product1' */
    ang_cos = Time_Varying_Model_2022a_B.Sum2_np[1] *
      Time_Varying_Model_2022a_B.Sum1_kf / Time_Varying_Model_2022a_B.Sum_jj;

    /* End of Outputs for SubSystem: '<S122>/Interpolate  velocities' */
    Time_Varying_Model_2022a_B.Product1_h[1] = ang_cos;

    /* Outputs for IfAction SubSystem: '<S122>/Interpolate  velocities' incorporates:
     *  ActionPort: '<S143>/Action Port'
     */
    /* Merge: '<S146>/Merge' incorporates:
     *  Sum: '<S143>/Sum3'
     */
    Time_Varying_Model_2022a_B.Merge[1] = Time_Varying_Model_2022a_B.Product_oc
      [1] + ang_cos;

    /* Product: '<S143>/Product1' */
    ang_cos = Time_Varying_Model_2022a_B.Sum2_np[2] *
      Time_Varying_Model_2022a_B.Sum1_kf / Time_Varying_Model_2022a_B.Sum_jj;

    /* End of Outputs for SubSystem: '<S122>/Interpolate  velocities' */
    Time_Varying_Model_2022a_B.Product1_h[2] = ang_cos;

    /* Outputs for IfAction SubSystem: '<S122>/Interpolate  velocities' incorporates:
     *  ActionPort: '<S143>/Action Port'
     */
    /* Merge: '<S146>/Merge' incorporates:
     *  Sum: '<S143>/Sum3'
     */
    Time_Varying_Model_2022a_B.Merge[2] = Time_Varying_Model_2022a_B.Product_oc
      [2] + ang_cos;

    /* End of Outputs for SubSystem: '<S122>/Interpolate  velocities' */
    break;
  }

  /* End of If: '<S122>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */

  /* UnitConversion: '<S113>/Unit Conversion' */
  /* Unit Conversion - from: ft/s to: m/s
     Expression: output = (0.3048*input) + (0) */
  Time_Varying_Model_2022a_B.UnitConversion_j[0] = 0.3048 *
    Time_Varying_Model_2022a_B.Merge[0];

  /* Integrator: '<S41>/Integrator' */
  Time_Varying_Model_2022a_B.Integrator_o[0] =
    Time_Varying_Model_2022a_X.Integrator_CSTATE_m[0];

  /* UnitConversion: '<S113>/Unit Conversion' */
  Time_Varying_Model_2022a_B.UnitConversion_j[1] = 0.3048 *
    Time_Varying_Model_2022a_B.Merge[1];

  /* Integrator: '<S41>/Integrator' */
  Time_Varying_Model_2022a_B.Integrator_o[1] =
    Time_Varying_Model_2022a_X.Integrator_CSTATE_m[1];

  /* UnitConversion: '<S113>/Unit Conversion' */
  Time_Varying_Model_2022a_B.UnitConversion_j[2] = 0.3048 *
    Time_Varying_Model_2022a_B.Merge[2];

  /* Integrator: '<S41>/Integrator' */
  Time_Varying_Model_2022a_B.Integrator_o[2] =
    Time_Varying_Model_2022a_X.Integrator_CSTATE_m[2];

  /* Sum: '<Root>/Sum' incorporates:
   *  Constant: '<Root>/Windspeed'
   */
  Time_Varying_Model_2022a_B.Sum = Time_Varying_Model_2022a_B.UnitConversion_j[0]
    + Time_Varying_Model_2022a_P.Windspeed_Value;

  /* MATLAB Function: '<S1>/MATLAB Function4' */
  /* :  ang_cos = cos(angle); */
  ang_cos = cos(Time_Varying_Model_2022a_B.Integrator_o[0]);

  /* :  ang_sin = sin(angle); */
  ang_sin = sin(Time_Varying_Model_2022a_B.Integrator_o[0]);

  /* :  if abs(ang_cos) < 1e-4 */
  if (fabs(ang_cos) < 0.0001) {
    /* :  ang_cos = 0; */
    ang_cos = 0.0;
  }

  /* :  if abs(ang_sin) < 1e-4 */
  if (fabs(ang_sin) < 0.0001) {
    /* :  ang_sin = 0; */
    ang_sin = 0.0;
  }

  /* :  windVector = [0; speed*ang_sin; speed*ang_cos]; */
  Time_Varying_Model_2022a_B.windVector[0] = 0.0;
  Time_Varying_Model_2022a_B.windVector[1] = Time_Varying_Model_2022a_B.Sum *
    ang_sin;
  Time_Varying_Model_2022a_B.windVector[2] = Time_Varying_Model_2022a_B.Sum *
    ang_cos;

  /* End of MATLAB Function: '<S1>/MATLAB Function4' */

  /* Sum: '<S1>/Add' incorporates:
   *  Constant: '<S1>/Constant1'
   */
  Time_Varying_Model_2022a_B.airspeedvectoringlobalframe[0] =
    Time_Varying_Model_2022a_B.windVector[0] +
    Time_Varying_Model_2022a_B.OpenRocketVelocity;
  Time_Varying_Model_2022a_B.airspeedvectoringlobalframe[1] =
    Time_Varying_Model_2022a_B.windVector[1] +
    Time_Varying_Model_2022a_P.Constant1_Value_a;
  Time_Varying_Model_2022a_B.airspeedvectoringlobalframe[2] =
    Time_Varying_Model_2022a_B.windVector[2] +
    Time_Varying_Model_2022a_P.Constant1_Value_a;

  /* Gain: '<S1>/Gain' */
  Time_Varying_Model_2022a_B.Gain_n = Time_Varying_Model_2022a_P.Gain_Gain_e *
    Time_Varying_Model_2022a_B.Integrator3;

  /* Gain: '<S45>/1//2' */
  Time_Varying_Model_2022a_B.u2[0] = Time_Varying_Model_2022a_P.u2_Gain *
    Time_Varying_Model_2022a_B.Integrator5;
  Time_Varying_Model_2022a_B.u2[1] = Time_Varying_Model_2022a_P.u2_Gain *
    Time_Varying_Model_2022a_B.Integrator4;
  Time_Varying_Model_2022a_B.u2[2] = Time_Varying_Model_2022a_P.u2_Gain *
    Time_Varying_Model_2022a_B.Gain_n;

  /* Trigonometry: '<S45>/sincos' */
  ang_sin = Time_Varying_Model_2022a_B.u2[0];
  ang_cos = sin(ang_sin);
  ang_sin = cos(ang_sin);
  Time_Varying_Model_2022a_B.sincos_o1[0] = ang_cos;
  Time_Varying_Model_2022a_B.sincos_o2[0] = ang_sin;
  ang_sin = Time_Varying_Model_2022a_B.u2[1];
  ang_cos = sin(ang_sin);
  ang_sin = cos(ang_sin);
  Time_Varying_Model_2022a_B.sincos_o1[1] = ang_cos;
  Time_Varying_Model_2022a_B.sincos_o2[1] = ang_sin;
  ang_sin = Time_Varying_Model_2022a_B.u2[2];
  ang_cos = sin(ang_sin);
  ang_sin = cos(ang_sin);
  Time_Varying_Model_2022a_B.sincos_o1[2] = ang_cos;
  Time_Varying_Model_2022a_B.sincos_o2[2] = ang_sin;

  /* Fcn: '<S45>/q2' */
  Time_Varying_Model_2022a_B.q2 = Time_Varying_Model_2022a_B.sincos_o2[0] *
    Time_Varying_Model_2022a_B.sincos_o1[1] *
    Time_Varying_Model_2022a_B.sincos_o2[2] -
    Time_Varying_Model_2022a_B.sincos_o1[0] *
    Time_Varying_Model_2022a_B.sincos_o2[1] *
    Time_Varying_Model_2022a_B.sincos_o1[2];

  /* Fcn: '<S45>/q0' */
  Time_Varying_Model_2022a_B.q0 = Time_Varying_Model_2022a_B.sincos_o2[0] *
    Time_Varying_Model_2022a_B.sincos_o2[1] *
    Time_Varying_Model_2022a_B.sincos_o2[2] -
    Time_Varying_Model_2022a_B.sincos_o1[0] *
    Time_Varying_Model_2022a_B.sincos_o1[1] *
    Time_Varying_Model_2022a_B.sincos_o1[2];

  /* Product: '<S51>/Product' */
  Time_Varying_Model_2022a_B.Product_e = Time_Varying_Model_2022a_B.q0 *
    Time_Varying_Model_2022a_B.q0;

  /* Fcn: '<S45>/q1' */
  Time_Varying_Model_2022a_B.q1 = Time_Varying_Model_2022a_B.sincos_o2[0] *
    Time_Varying_Model_2022a_B.sincos_o1[1] *
    Time_Varying_Model_2022a_B.sincos_o1[2] +
    Time_Varying_Model_2022a_B.sincos_o1[0] *
    Time_Varying_Model_2022a_B.sincos_o2[1] *
    Time_Varying_Model_2022a_B.sincos_o2[2];

  /* Product: '<S51>/Product1' */
  Time_Varying_Model_2022a_B.Product1 = Time_Varying_Model_2022a_B.q1 *
    Time_Varying_Model_2022a_B.q1;

  /* Product: '<S51>/Product2' */
  Time_Varying_Model_2022a_B.Product2 = Time_Varying_Model_2022a_B.q2 *
    Time_Varying_Model_2022a_B.q2;

  /* Fcn: '<S45>/q3' */
  Time_Varying_Model_2022a_B.q3 = Time_Varying_Model_2022a_B.sincos_o2[0] *
    Time_Varying_Model_2022a_B.sincos_o2[1] *
    Time_Varying_Model_2022a_B.sincos_o1[2] +
    Time_Varying_Model_2022a_B.sincos_o1[0] *
    Time_Varying_Model_2022a_B.sincos_o1[1] *
    Time_Varying_Model_2022a_B.sincos_o2[2];

  /* Product: '<S51>/Product3' */
  Time_Varying_Model_2022a_B.Product3 = Time_Varying_Model_2022a_B.q3 *
    Time_Varying_Model_2022a_B.q3;

  /* Sum: '<S51>/Sum' */
  Time_Varying_Model_2022a_B.Sum_l = ((Time_Varying_Model_2022a_B.Product_e +
    Time_Varying_Model_2022a_B.Product1) + Time_Varying_Model_2022a_B.Product2)
    + Time_Varying_Model_2022a_B.Product3;

  /* Sqrt: '<S50>/sqrt' */
  Time_Varying_Model_2022a_B.sqrt_e = sqrt(Time_Varying_Model_2022a_B.Sum_l);

  /* Product: '<S46>/Product2' */
  Time_Varying_Model_2022a_B.Product2_m = Time_Varying_Model_2022a_B.q2 /
    Time_Varying_Model_2022a_B.sqrt_e;

  /* Product: '<S47>/Product6' */
  Time_Varying_Model_2022a_B.Product6 = Time_Varying_Model_2022a_B.Product2_m *
    Time_Varying_Model_2022a_B.Product2_m;

  /* Product: '<S46>/Product3' */
  Time_Varying_Model_2022a_B.Product3_a = Time_Varying_Model_2022a_B.q3 /
    Time_Varying_Model_2022a_B.sqrt_e;

  /* Product: '<S47>/Product7' */
  Time_Varying_Model_2022a_B.Product7 = Time_Varying_Model_2022a_B.Product3_a *
    Time_Varying_Model_2022a_B.Product3_a;

  /* Sum: '<S47>/Sum3' incorporates:
   *  Constant: '<S47>/Constant'
   */
  Time_Varying_Model_2022a_B.Sum3 =
    (Time_Varying_Model_2022a_P.Constant_Value_lw -
     Time_Varying_Model_2022a_B.Product6) - Time_Varying_Model_2022a_B.Product7;

  /* Gain: '<S47>/Gain2' */
  Time_Varying_Model_2022a_B.Gain2 = Time_Varying_Model_2022a_P.Gain2_Gain *
    Time_Varying_Model_2022a_B.Sum3;

  /* Product: '<S47>/Product8' */
  Time_Varying_Model_2022a_B.Product8 =
    Time_Varying_Model_2022a_B.airspeedvectoringlobalframe[0] *
    Time_Varying_Model_2022a_B.Gain2;

  /* Product: '<S46>/Product1' */
  Time_Varying_Model_2022a_B.Product1_i = Time_Varying_Model_2022a_B.q1 /
    Time_Varying_Model_2022a_B.sqrt_e;

  /* Product: '<S47>/Product' */
  Time_Varying_Model_2022a_B.Product_a = Time_Varying_Model_2022a_B.Product1_i *
    Time_Varying_Model_2022a_B.Product2_m;

  /* Product: '<S46>/Product' */
  Time_Varying_Model_2022a_B.Product_f = Time_Varying_Model_2022a_B.q0 /
    Time_Varying_Model_2022a_B.sqrt_e;

  /* Product: '<S47>/Product1' */
  Time_Varying_Model_2022a_B.Product1_d = Time_Varying_Model_2022a_B.Product_f *
    Time_Varying_Model_2022a_B.Product3_a;

  /* Sum: '<S47>/Sum1' */
  Time_Varying_Model_2022a_B.Sum1_k = Time_Varying_Model_2022a_B.Product_a +
    Time_Varying_Model_2022a_B.Product1_d;

  /* Gain: '<S47>/Gain' */
  Time_Varying_Model_2022a_B.Gain_nk = Time_Varying_Model_2022a_P.Gain_Gain_k *
    Time_Varying_Model_2022a_B.Sum1_k;

  /* Product: '<S47>/Product4' */
  Time_Varying_Model_2022a_B.Product4 = Time_Varying_Model_2022a_B.Gain_nk *
    Time_Varying_Model_2022a_B.airspeedvectoringlobalframe[1];

  /* Product: '<S47>/Product2' */
  Time_Varying_Model_2022a_B.Product2_c = Time_Varying_Model_2022a_B.Product_f *
    Time_Varying_Model_2022a_B.Product2_m;

  /* Product: '<S47>/Product3' */
  Time_Varying_Model_2022a_B.Product3_h = Time_Varying_Model_2022a_B.Product1_i *
    Time_Varying_Model_2022a_B.Product3_a;

  /* Sum: '<S47>/Sum2' */
  Time_Varying_Model_2022a_B.Sum2 = Time_Varying_Model_2022a_B.Product3_h -
    Time_Varying_Model_2022a_B.Product2_c;

  /* Gain: '<S47>/Gain1' */
  Time_Varying_Model_2022a_B.Gain1 = Time_Varying_Model_2022a_P.Gain1_Gain *
    Time_Varying_Model_2022a_B.Sum2;

  /* Product: '<S47>/Product5' */
  Time_Varying_Model_2022a_B.Product5 = Time_Varying_Model_2022a_B.Gain1 *
    Time_Varying_Model_2022a_B.airspeedvectoringlobalframe[2];

  /* Sum: '<S47>/Sum' */
  Time_Varying_Model_2022a_B.Sum_a = (Time_Varying_Model_2022a_B.Product8 +
    Time_Varying_Model_2022a_B.Product4) + Time_Varying_Model_2022a_B.Product5;

  /* Outport: '<Root>/airspeed_x' */
  Time_Varying_Model_2022a_Y.airspeed_x = Time_Varying_Model_2022a_B.Sum_a;

  /* Product: '<S48>/Product' */
  Time_Varying_Model_2022a_B.Product_ee = Time_Varying_Model_2022a_B.Product1_i *
    Time_Varying_Model_2022a_B.Product2_m;

  /* Product: '<S48>/Product1' */
  Time_Varying_Model_2022a_B.Product1_b = Time_Varying_Model_2022a_B.Product_f *
    Time_Varying_Model_2022a_B.Product3_a;

  /* Sum: '<S48>/Sum1' */
  Time_Varying_Model_2022a_B.Sum1_f = Time_Varying_Model_2022a_B.Product_ee -
    Time_Varying_Model_2022a_B.Product1_b;

  /* Gain: '<S48>/Gain' */
  Time_Varying_Model_2022a_B.Gain_p = Time_Varying_Model_2022a_P.Gain_Gain_ev *
    Time_Varying_Model_2022a_B.Sum1_f;

  /* Product: '<S48>/Product4' */
  Time_Varying_Model_2022a_B.Product4_j =
    Time_Varying_Model_2022a_B.airspeedvectoringlobalframe[0] *
    Time_Varying_Model_2022a_B.Gain_p;

  /* Product: '<S48>/Product6' */
  Time_Varying_Model_2022a_B.Product6_f = Time_Varying_Model_2022a_B.Product1_i *
    Time_Varying_Model_2022a_B.Product1_i;

  /* Product: '<S48>/Product7' */
  Time_Varying_Model_2022a_B.Product7_h = Time_Varying_Model_2022a_B.Product3_a *
    Time_Varying_Model_2022a_B.Product3_a;

  /* Sum: '<S48>/Sum3' incorporates:
   *  Constant: '<S48>/Constant'
   */
  Time_Varying_Model_2022a_B.Sum3_a =
    (Time_Varying_Model_2022a_P.Constant_Value_o -
     Time_Varying_Model_2022a_B.Product6_f) -
    Time_Varying_Model_2022a_B.Product7_h;

  /* Gain: '<S48>/Gain2' */
  Time_Varying_Model_2022a_B.Gain2_p = Time_Varying_Model_2022a_P.Gain2_Gain_o *
    Time_Varying_Model_2022a_B.Sum3_a;

  /* Product: '<S48>/Product8' */
  Time_Varying_Model_2022a_B.Product8_g = Time_Varying_Model_2022a_B.Gain2_p *
    Time_Varying_Model_2022a_B.airspeedvectoringlobalframe[1];

  /* Product: '<S48>/Product2' */
  Time_Varying_Model_2022a_B.Product2_b = Time_Varying_Model_2022a_B.Product_f *
    Time_Varying_Model_2022a_B.Product1_i;

  /* Product: '<S48>/Product3' */
  Time_Varying_Model_2022a_B.Product3_l = Time_Varying_Model_2022a_B.Product2_m *
    Time_Varying_Model_2022a_B.Product3_a;

  /* Sum: '<S48>/Sum2' */
  Time_Varying_Model_2022a_B.Sum2_k = Time_Varying_Model_2022a_B.Product2_b +
    Time_Varying_Model_2022a_B.Product3_l;

  /* Gain: '<S48>/Gain1' */
  Time_Varying_Model_2022a_B.Gain1_l = Time_Varying_Model_2022a_P.Gain1_Gain_d *
    Time_Varying_Model_2022a_B.Sum2_k;

  /* Product: '<S48>/Product5' */
  Time_Varying_Model_2022a_B.Product5_a = Time_Varying_Model_2022a_B.Gain1_l *
    Time_Varying_Model_2022a_B.airspeedvectoringlobalframe[2];

  /* Sum: '<S48>/Sum' */
  Time_Varying_Model_2022a_B.Sum_n = (Time_Varying_Model_2022a_B.Product4_j +
    Time_Varying_Model_2022a_B.Product8_g) +
    Time_Varying_Model_2022a_B.Product5_a;

  /* Outport: '<Root>/airspeed_y' */
  Time_Varying_Model_2022a_Y.airspeed_y = Time_Varying_Model_2022a_B.Sum_n;

  /* Product: '<S49>/Product' */
  Time_Varying_Model_2022a_B.Product_d = Time_Varying_Model_2022a_B.Product1_i *
    Time_Varying_Model_2022a_B.Product3_a;

  /* Product: '<S49>/Product1' */
  Time_Varying_Model_2022a_B.Product1_k = Time_Varying_Model_2022a_B.Product_f *
    Time_Varying_Model_2022a_B.Product2_m;

  /* Sum: '<S49>/Sum1' */
  Time_Varying_Model_2022a_B.Sum1_m = Time_Varying_Model_2022a_B.Product_d +
    Time_Varying_Model_2022a_B.Product1_k;

  /* Gain: '<S49>/Gain' */
  Time_Varying_Model_2022a_B.Gain_i = Time_Varying_Model_2022a_P.Gain_Gain_ob *
    Time_Varying_Model_2022a_B.Sum1_m;

  /* Product: '<S49>/Product4' */
  Time_Varying_Model_2022a_B.Product4_n =
    Time_Varying_Model_2022a_B.airspeedvectoringlobalframe[0] *
    Time_Varying_Model_2022a_B.Gain_i;

  /* Product: '<S49>/Product2' */
  Time_Varying_Model_2022a_B.Product2_a = Time_Varying_Model_2022a_B.Product_f *
    Time_Varying_Model_2022a_B.Product1_i;

  /* Product: '<S49>/Product3' */
  Time_Varying_Model_2022a_B.Product3_n = Time_Varying_Model_2022a_B.Product2_m *
    Time_Varying_Model_2022a_B.Product3_a;

  /* Sum: '<S49>/Sum2' */
  Time_Varying_Model_2022a_B.Sum2_n = Time_Varying_Model_2022a_B.Product3_n -
    Time_Varying_Model_2022a_B.Product2_a;

  /* Gain: '<S49>/Gain1' */
  Time_Varying_Model_2022a_B.Gain1_k = Time_Varying_Model_2022a_P.Gain1_Gain_k *
    Time_Varying_Model_2022a_B.Sum2_n;

  /* Product: '<S49>/Product5' */
  Time_Varying_Model_2022a_B.Product5_b = Time_Varying_Model_2022a_B.Gain1_k *
    Time_Varying_Model_2022a_B.airspeedvectoringlobalframe[1];

  /* Product: '<S49>/Product6' */
  Time_Varying_Model_2022a_B.Product6_a = Time_Varying_Model_2022a_B.Product1_i *
    Time_Varying_Model_2022a_B.Product1_i;

  /* Product: '<S49>/Product7' */
  Time_Varying_Model_2022a_B.Product7_l = Time_Varying_Model_2022a_B.Product2_m *
    Time_Varying_Model_2022a_B.Product2_m;

  /* Sum: '<S49>/Sum3' incorporates:
   *  Constant: '<S49>/Constant'
   */
  Time_Varying_Model_2022a_B.Sum3_p =
    (Time_Varying_Model_2022a_P.Constant_Value_f -
     Time_Varying_Model_2022a_B.Product6_a) -
    Time_Varying_Model_2022a_B.Product7_l;

  /* Gain: '<S49>/Gain2' */
  Time_Varying_Model_2022a_B.Gain2_b = Time_Varying_Model_2022a_P.Gain2_Gain_a *
    Time_Varying_Model_2022a_B.Sum3_p;

  /* Product: '<S49>/Product8' */
  Time_Varying_Model_2022a_B.Product8_b = Time_Varying_Model_2022a_B.Gain2_b *
    Time_Varying_Model_2022a_B.airspeedvectoringlobalframe[2];

  /* Sum: '<S49>/Sum' */
  Time_Varying_Model_2022a_B.Sum_ai = (Time_Varying_Model_2022a_B.Product4_n +
    Time_Varying_Model_2022a_B.Product5_b) +
    Time_Varying_Model_2022a_B.Product8_b;

  /* Outport: '<Root>/airspeed_z' */
  Time_Varying_Model_2022a_Y.airspeed_z = Time_Varying_Model_2022a_B.Sum_ai;

  /* SignalConversion generated from: '<S43>/ SFunction ' incorporates:
   *  MATLAB Function: '<S1>/MATLAB Function5'
   */
  Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctionI[0] =
    Time_Varying_Model_2022a_B.Sum_a;
  Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctionI[1] =
    Time_Varying_Model_2022a_B.Sum_n;
  Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctionI[2] =
    Time_Varying_Model_2022a_B.Sum_ai;

  /* SignalConversion generated from: '<S43>/ SFunction ' incorporates:
   *  Constant: '<S1>/Constant'
   *  Constant: '<S1>/Constant16'
   *  MATLAB Function: '<S1>/MATLAB Function5'
   */
  Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctio_d[0] =
    Time_Varying_Model_2022a_P.Constant_Value_ji;
  Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctio_d[1] =
    Time_Varying_Model_2022a_P.Constant16_Value;
  Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctio_d[2] =
    Time_Varying_Model_2022a_P.Constant16_Value;

  /* MATLAB Function: '<S1>/MATLAB Function5' incorporates:
   *  SignalConversion generated from: '<S43>/ SFunction '
   */
  /* :  airspeedVector_xy = [airspeedVector(1); airspeedVector(2); 0]; */
  ang_sin = Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctionI[0];
  st_idx_1 = Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctionI[1];

  /* :  airspeedVector_xz = [airspeedVector(1); 0; airspeedVector(3)]; */
  ct_idx_0 = Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctionI[0];
  ct_idx_2 = Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctionI[2];

  /* :  alpha = atan2(airspeedVector_xz(3), airspeedVector_xz(1)); */
  /* :  beta = atan2(airspeedVector_xy(2), airspeedVector_xy(1)); */
  /* :  alphaTotal = acos(dot(airspeedVector, rocketVector)/(norm(airspeedVector)*norm(rocketVector))); */
  ang_cos = Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctionI[0] *
    Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctio_d[0];
  ang_cos += Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctionI[1] *
    Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctio_d[1];
  ang_cos += Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctionI[2] *
    Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctio_d[2];

  /* :  windAngles = [alpha; beta; alphaTotal]; */
  ct_idx_0 = rt_atan2d_snf(ct_idx_2, ct_idx_0);
  ang_sin = rt_atan2d_snf(st_idx_1, ang_sin);
  st_idx_1 = Time_Varying_Model_2022a_norm
    (Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctionI);
  ct_idx_2 = Time_Varying_Model_2022a_norm
    (Time_Varying_Model_2022a_B.TmpSignalConversionAtSFunctio_d);
  st_idx_1 = acos(ang_cos / (st_idx_1 * ct_idx_2));
  Time_Varying_Model_2022a_B.windAngles[0] = ct_idx_0;
  Time_Varying_Model_2022a_B.windAngles[1] = ang_sin;
  Time_Varying_Model_2022a_B.windAngles[2] = st_idx_1;

  /* Gain: '<S27>/Gain' */
  Time_Varying_Model_2022a_B.Gain_gn = Time_Varying_Model_2022a_P.Gain_Gain_fn *
    Time_Varying_Model_2022a_B.windAngles[0];

  /* Outport: '<Root>/alphaOut' */
  Time_Varying_Model_2022a_Y.alpha = Time_Varying_Model_2022a_B.Gain_gn;

  /* Gain: '<S28>/Gain' */
  Time_Varying_Model_2022a_B.Gain_lo = Time_Varying_Model_2022a_P.Gain_Gain_kf *
    Time_Varying_Model_2022a_B.windAngles[1];

  /* Outport: '<Root>/betaOut' */
  Time_Varying_Model_2022a_Y.beta = Time_Varying_Model_2022a_B.Gain_lo;
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
  }

  /* Gain: '<S19>/Gain' */
  Time_Varying_Model_2022a_B.Gain_ly = Time_Varying_Model_2022a_P.Gain_Gain_ou *
    Time_Varying_Model_2022a_B.windAngles[0];

  /* Gain: '<S20>/Gain' */
  Time_Varying_Model_2022a_B.Gain_b = Time_Varying_Model_2022a_P.Gain_Gain_d *
    Time_Varying_Model_2022a_B.windAngles[1];
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
  }

  /* MATLAB Function: '<Root>/MATLAB Function' incorporates:
   *  Constant: '<Root>/Constant32'
   *  Constant: '<Root>/Constant33'
   *  Constant: '<Root>/Constant34'
   *  Constant: '<Root>/Constant6'
   */
  /* :  if alpha == 0 */
  if (Time_Varying_Model_2022a_B.windAngles[2] == 0.0) {
    /* :  a1 = 1; */
    ang_cos = 1.0;

    /* :  a2 = 0; */
    ang_sin = 0.0;
  } else {
    /* :  else */
    /* :  a1  = sin(alpha)/alpha; */
    ang_cos = sin(Time_Varying_Model_2022a_B.windAngles[2]) /
      Time_Varying_Model_2022a_B.windAngles[2];

    /* :  a2  = (sin(alpha)^2)/alpha; */
    ang_sin = sin(Time_Varying_Model_2022a_B.windAngles[2]);
    ang_sin = ang_sin * ang_sin / Time_Varying_Model_2022a_B.windAngles[2];
  }

  /* :  CNa_nose = (2/A_ref)*(A_l-A_0)*a1+K*(A_plan/A_ref)*a2; */
  Time_Varying_Model_2022a_B.CNa_nose = 2.0 /
    Time_Varying_Model_2022a_P.Constant6_Value *
    (Time_Varying_Model_2022a_P.Constant6_Value -
     Time_Varying_Model_2022a_P.Constant32_Value) * ang_cos +
    Time_Varying_Model_2022a_P.Constant33_Value /
    Time_Varying_Model_2022a_P.Constant6_Value *
    Time_Varying_Model_2022a_P.Constant34_Value * ang_sin;

  /* End of MATLAB Function: '<Root>/MATLAB Function' */

  /* Product: '<S5>/Product' incorporates:
   *  Constant: '<Root>/Constant22'
   */
  Time_Varying_Model_2022a_B.Product_k = Time_Varying_Model_2022a_B.CNa_nose *
    Time_Varying_Model_2022a_P.Constant22_Value;

  /* MATLAB Function: '<Root>/MATLAB Function2' incorporates:
   *  Constant: '<Root>/Constant35'
   *  Constant: '<Root>/Constant36'
   *  Constant: '<Root>/Constant6'
   */
  /* :  if alpha == 0 */
  if (Time_Varying_Model_2022a_B.windAngles[2] == 0.0) {
    /* :  a1 = 1; */
    ang_cos = 1.0;

    /* :  a2 = 0; */
    ang_sin = 0.0;
  } else {
    /* :  else */
    /* :  a1  = sin(alpha)/alpha; */
    ang_cos = sin(Time_Varying_Model_2022a_B.windAngles[2]) /
      Time_Varying_Model_2022a_B.windAngles[2];

    /* :  a2  = (sin(alpha)^2)/alpha; */
    ang_sin = sin(Time_Varying_Model_2022a_B.windAngles[2]);
    ang_sin = ang_sin * ang_sin / Time_Varying_Model_2022a_B.windAngles[2];
  }

  /* :  CNa_body = (2/A_ref)*(A_l-A_0)*a1+K*(A_plan/A_ref)*a2; */
  Time_Varying_Model_2022a_B.CNa_body = 2.0 /
    Time_Varying_Model_2022a_P.Constant6_Value *
    (Time_Varying_Model_2022a_P.Constant6_Value -
     Time_Varying_Model_2022a_P.Constant6_Value) * ang_cos +
    Time_Varying_Model_2022a_P.Constant35_Value /
    Time_Varying_Model_2022a_P.Constant6_Value *
    Time_Varying_Model_2022a_P.Constant36_Value * ang_sin;

  /* End of MATLAB Function: '<Root>/MATLAB Function2' */

  /* Product: '<S5>/Product1' incorporates:
   *  Constant: '<Root>/Constant23'
   */
  Time_Varying_Model_2022a_B.Product1_a = Time_Varying_Model_2022a_B.CNa_body *
    Time_Varying_Model_2022a_P.Constant23_Value;

  /* Lookup_n-D: '<Root>/OpenRocket Mach No' incorporates:
   *  Clock: '<Root>/Clock'
   */
  Time_Varying_Model_2022a_B.OpenRocketMachNo = look1_binlxpw
    (Time_Varying_Model_2022a_B.Clock,
     Time_Varying_Model_2022a_P.OpenRocketMachNo_bp01Data,
     Time_Varying_Model_2022a_P.OpenRocketMachNo_tableData, 222U);
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
    /* Gain: '<Root>/Gain2' incorporates:
     *  Constant: '<Root>/Constant11'
     */
    Time_Varying_Model_2022a_B.Gain2_j =
      Time_Varying_Model_2022a_P.Gain2_Gain_a5 *
      Time_Varying_Model_2022a_P.Constant11_Value;
  }

  /* MATLAB Function: '<Root>/MATLAB Function1' incorporates:
   *  Constant: '<Root>/Constant39'
   *  Constant: '<Root>/Constant40'
   *  Constant: '<Root>/Constant41'
   *  Constant: '<Root>/Constant6'
   */
  /* :  CNa1_num = 2*pi()*(s^2/A_ref); */
  /* :  CNa1_den = 1 + sqrt(1+(((s^2)*sqrt(1-M^2))/(A_fin*cos(G_c)))^2); */
  ang_sin = sqrt(1.0 - Time_Varying_Model_2022a_B.OpenRocketMachNo *
                 Time_Varying_Model_2022a_B.OpenRocketMachNo) *
    (Time_Varying_Model_2022a_P.Constant39_Value *
     Time_Varying_Model_2022a_P.Constant39_Value) /
    (Time_Varying_Model_2022a_P.Constant40_Value * cos
     (Time_Varying_Model_2022a_P.Constant41_Value));

  /* :  CNa1 = CNa1_num / CNa1_den; */
  /* :  CNa4 = 2*CNa1; */
  /* :  K_TB = 1 + r_t/(s+r_t); */
  /* :  CNa_fins = K_TB*CNa4; */
  Time_Varying_Model_2022a_B.CNa_fins =
    Time_Varying_Model_2022a_P.Constant39_Value *
    Time_Varying_Model_2022a_P.Constant39_Value /
    Time_Varying_Model_2022a_P.Constant6_Value * 6.2831853071795862 / (sqrt
    (ang_sin * ang_sin + 1.0) + 1.0) * 2.0 * (Time_Varying_Model_2022a_B.Gain2_j
    / (Time_Varying_Model_2022a_P.Constant39_Value +
       Time_Varying_Model_2022a_B.Gain2_j) + 1.0);

  /* Product: '<S5>/Product2' incorporates:
   *  Constant: '<Root>/Constant24'
   */
  Time_Varying_Model_2022a_B.Product2_j = Time_Varying_Model_2022a_B.CNa_fins *
    Time_Varying_Model_2022a_P.Constant24_Value;

  /* Sum: '<S5>/Sum of Elements' */
  Time_Varying_Model_2022a_B.SumofElements =
    (Time_Varying_Model_2022a_B.Product_k +
     Time_Varying_Model_2022a_B.Product1_a) +
    Time_Varying_Model_2022a_B.Product2_j;

  /* Sum: '<S5>/Sum of Elements1' */
  Time_Varying_Model_2022a_B.SumofElements1 =
    (Time_Varying_Model_2022a_B.CNa_nose + Time_Varying_Model_2022a_B.CNa_body)
    + Time_Varying_Model_2022a_B.CNa_fins;

  /* Product: '<S5>/Divide' */
  Time_Varying_Model_2022a_B.Divide_c = Time_Varying_Model_2022a_B.SumofElements
    / Time_Varying_Model_2022a_B.SumofElements1;
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
    /* RandomNumber: '<S54>/White Noise' */
    Time_Varying_Model_2022a_B.WhiteNoise_p =
      Time_Varying_Model_2022a_DW.NextOutput_p;

    /* Gain: '<S54>/Output' */
    ang_cos = Time_Varying_Model_2022a_P.BandLimitedWhiteNoise2_Cov;
    ang_cos = sqrt(ang_cos);
    ang_sin = ang_cos / 0.1;

    /* Gain: '<S54>/Output' */
    Time_Varying_Model_2022a_B.Output = ang_sin *
      Time_Varying_Model_2022a_B.WhiteNoise_p;
  }

  /* MATLAB Function: '<Root>/MATLAB Function4' */
  /* :  T0 = 288.15; */
  /* :  rho0 = 1.2256; */
  /* :  L = 0.0065; */
  /* :  g = 9.81; */
  /* :  R = 287.26; */
  /* :  rho = rho0*((T0-L*h)/T0)^((g/(L*R))-1); */
  Time_Varying_Model_2022a_B.rho = rt_powd_snf((288.15 - 0.0065 *
    Time_Varying_Model_2022a_B.OpenRocketalt) / 288.15, 4.2538841788998445) *
    1.2256;

  /* Sum: '<S8>/Sum4' */
  Time_Varying_Model_2022a_B.Sum4 = Time_Varying_Model_2022a_B.Output +
    Time_Varying_Model_2022a_B.rho;
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
    /* RandomNumber: '<S53>/White Noise' */
    Time_Varying_Model_2022a_B.WhiteNoise_c =
      Time_Varying_Model_2022a_DW.NextOutput_j;

    /* Gain: '<S53>/Output' */
    ang_cos = Time_Varying_Model_2022a_P.BandLimitedWhiteNoise1_Cov;
    ang_cos = sqrt(ang_cos);
    ang_sin = ang_cos / 0.1;

    /* Gain: '<S53>/Output' */
    Time_Varying_Model_2022a_B.Output_g = ang_sin *
      Time_Varying_Model_2022a_B.WhiteNoise_c;
  }

  /* Sum: '<S8>/Sum3' */
  Time_Varying_Model_2022a_B.Sum3_d =
    Time_Varying_Model_2022a_B.OpenRocketVelocity +
    Time_Varying_Model_2022a_B.Output_g;

  /* Math: '<S8>/Square' */
  Time_Varying_Model_2022a_B.Square = Time_Varying_Model_2022a_B.Sum3_d *
    Time_Varying_Model_2022a_B.Sum3_d;
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
  }

  /* Product: '<S8>/Product' */
  Time_Varying_Model_2022a_B.Product_ax = Time_Varying_Model_2022a_B.Sum4 *
    Time_Varying_Model_2022a_B.Square;
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
    /* RandomNumber: '<S52>/White Noise' */
    Time_Varying_Model_2022a_B.WhiteNoise_m =
      Time_Varying_Model_2022a_DW.NextOutput_d;

    /* Gain: '<S52>/Output' */
    ang_cos = Time_Varying_Model_2022a_P.BandLimitedWhiteNoise_Cov;
    ang_cos = sqrt(ang_cos);
    ang_sin = ang_cos / 0.1;

    /* Gain: '<S52>/Output' */
    Time_Varying_Model_2022a_B.Output_h = ang_sin *
      Time_Varying_Model_2022a_B.WhiteNoise_m;
  }

  /* Sum: '<S8>/Sum' */
  Time_Varying_Model_2022a_B.Sum_b = Time_Varying_Model_2022a_B.Output_h +
    Time_Varying_Model_2022a_B.Product_ax;

  /* RateTransition: '<S8>/Rate Transition1' */
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
    if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
        Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[3] == 0) {
      Time_Varying_Model_2022a_DW.RateTransition1_Buffer =
        Time_Varying_Model_2022a_B.Sum_b;
    }

    /* MATLAB Function: '<S105>/SqrtUsedFcn' incorporates:
     *  Constant: '<S105>/isSqrtUsed'
     *  Constant: '<S56>/CovarianceZ'
     */
    /* :  if isSqrtUsed */
    if (Time_Varying_Model_2022a_P.isSqrtUsed_Value) {
      /* :  P = u*u.'; */
      Time_Varying_Model_2022a_B.P =
        Time_Varying_Model_2022a_P.CovarianceZ_Value *
        Time_Varying_Model_2022a_P.CovarianceZ_Value;
    } else {
      /* :  else */
      /* :  P = u; */
      Time_Varying_Model_2022a_B.P =
        Time_Varying_Model_2022a_P.CovarianceZ_Value;
    }

    /* End of MATLAB Function: '<S105>/SqrtUsedFcn' */
  }

  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[3] == 0) {
    /* RateTransition: '<S8>/Rate Transition1' */
    Time_Varying_Model_2022a_B.RateTransition1 =
      Time_Varying_Model_2022a_DW.RateTransition1_Buffer;

    /* Delay: '<S55>/MemoryX' incorporates:
     *  Constant: '<S55>/X0'
     */
    if (Time_Varying_Model_2022a_DW.icLoad) {
      Time_Varying_Model_2022a_DW.MemoryX_DSTATE =
        Time_Varying_Model_2022a_P.X0_Value;
    }

    /* Delay: '<S55>/MemoryX' */
    Time_Varying_Model_2022a_B.MemoryX =
      Time_Varying_Model_2022a_DW.MemoryX_DSTATE;

    /* Outputs for Enabled SubSystem: '<S83>/Enabled Subsystem' incorporates:
     *  EnablePort: '<S109>/Enable'
     */
    if (rtsiIsModeUpdateTimeStep(&Time_Varying_Model_2022a_M->solverInfo)) {
      if (Time_Varying_Model_2022a_P.Enable_Value) {
        Time_Varying_Model_2022a_DW.EnabledSubsystem_MODE = true;
      } else if (Time_Varying_Model_2022a_DW.EnabledSubsystem_MODE) {
        /* Disable for Product: '<S109>/Product2' incorporates:
         *  Outport: '<S109>/deltax'
         */
        Time_Varying_Model_2022a_B.Product2_cw =
          Time_Varying_Model_2022a_P.deltax_Y0;
        Time_Varying_Model_2022a_DW.EnabledSubsystem_MODE = false;
      }
    }

    if (Time_Varying_Model_2022a_DW.EnabledSubsystem_MODE) {
      /* Product: '<S109>/Product' incorporates:
       *  Constant: '<S55>/C'
       */
      ang_sin = Time_Varying_Model_2022a_P.C_Value;
      ang_cos = Time_Varying_Model_2022a_B.MemoryX;

      /* Product: '<S109>/Product' */
      Time_Varying_Model_2022a_B.Product_n = ang_sin * ang_cos;

      /* Sum: '<S109>/Add1' */
      Time_Varying_Model_2022a_B.Add1 =
        Time_Varying_Model_2022a_B.RateTransition1 -
        Time_Varying_Model_2022a_B.Product_n;

      /* Product: '<S109>/Product2' incorporates:
       *  Constant: '<S56>/KalmanGainM'
       */
      ang_sin = Time_Varying_Model_2022a_P.KalmanGainM_Value;
      ang_cos = Time_Varying_Model_2022a_B.Add1;

      /* Product: '<S109>/Product2' */
      Time_Varying_Model_2022a_B.Product2_cw = ang_sin * ang_cos;
    }

    /* End of Outputs for SubSystem: '<S83>/Enabled Subsystem' */

    /* Sum: '<S83>/Add' incorporates:
     *  Constant: '<S55>/Enable'
     */
    Time_Varying_Model_2022a_B.Add = Time_Varying_Model_2022a_B.Product2_cw +
      Time_Varying_Model_2022a_B.MemoryX;

    /* Gain: '<S8>/Gain3' */
    Time_Varying_Model_2022a_B.Gain3 = Time_Varying_Model_2022a_P.Gain3_Gain *
      Time_Varying_Model_2022a_B.Add;
  }

  /* End of RateTransition: '<S8>/Rate Transition1' */

  /* RateTransition: '<S8>/Rate Transition' */
  if ((rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
       Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) &&
      (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
       Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[3] == 0)) {
    Time_Varying_Model_2022a_DW.RateTransition_Buffer =
      Time_Varying_Model_2022a_B.Square;
  }

  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[3] == 0) {
    /* Product: '<S76>/A[k]*xhat[k|k-1]' incorporates:
     *  Constant: '<S55>/A'
     */
    ang_sin = Time_Varying_Model_2022a_P.A_Value;
    ang_cos = Time_Varying_Model_2022a_B.MemoryX;

    /* Product: '<S76>/A[k]*xhat[k|k-1]' */
    Time_Varying_Model_2022a_B.Akxhatkk1 = ang_sin * ang_cos;

    /* RateTransition: '<S8>/Rate Transition' */
    Time_Varying_Model_2022a_B.RateTransition =
      Time_Varying_Model_2022a_DW.RateTransition_Buffer;

    /* Product: '<S76>/B[k]*u[k]' incorporates:
     *  Constant: '<S55>/B'
     */
    ang_sin = Time_Varying_Model_2022a_P.B_Value;
    ang_cos = Time_Varying_Model_2022a_B.RateTransition;

    /* Product: '<S76>/B[k]*u[k]' */
    Time_Varying_Model_2022a_B.Bkuk = ang_sin * ang_cos;

    /* Outputs for Enabled SubSystem: '<S76>/MeasurementUpdate' incorporates:
     *  EnablePort: '<S107>/Enable'
     */
    if (rtsiIsModeUpdateTimeStep(&Time_Varying_Model_2022a_M->solverInfo)) {
      if (Time_Varying_Model_2022a_P.Enable_Value) {
        Time_Varying_Model_2022a_DW.MeasurementUpdate_MODE = true;
      } else if (Time_Varying_Model_2022a_DW.MeasurementUpdate_MODE) {
        /* Disable for Product: '<S107>/Product3' incorporates:
         *  Outport: '<S107>/L*(y[k]-yhat[k|k-1])'
         */
        Time_Varying_Model_2022a_B.Product3_lw =
          Time_Varying_Model_2022a_P.Lykyhatkk1_Y0;
        Time_Varying_Model_2022a_DW.MeasurementUpdate_MODE = false;
      }
    }

    if (Time_Varying_Model_2022a_DW.MeasurementUpdate_MODE) {
      /* Product: '<S107>/C[k]*xhat[k|k-1]' incorporates:
       *  Constant: '<S55>/C'
       */
      ang_sin = Time_Varying_Model_2022a_P.C_Value;
      ang_cos = Time_Varying_Model_2022a_B.MemoryX;

      /* Product: '<S107>/C[k]*xhat[k|k-1]' */
      Time_Varying_Model_2022a_B.Ckxhatkk1 = ang_sin * ang_cos;

      /* Product: '<S107>/D[k]*u[k]' incorporates:
       *  Constant: '<S55>/D'
       */
      ang_sin = Time_Varying_Model_2022a_P.D_Value;
      ang_cos = Time_Varying_Model_2022a_B.RateTransition;

      /* Product: '<S107>/D[k]*u[k]' */
      Time_Varying_Model_2022a_B.Dkuk = ang_sin * ang_cos;

      /* Sum: '<S107>/Add1' */
      Time_Varying_Model_2022a_B.yhatkk1 = Time_Varying_Model_2022a_B.Ckxhatkk1
        + Time_Varying_Model_2022a_B.Dkuk;

      /* Sum: '<S107>/Sum' */
      Time_Varying_Model_2022a_B.Sum_e =
        Time_Varying_Model_2022a_B.RateTransition1 -
        Time_Varying_Model_2022a_B.yhatkk1;

      /* Product: '<S107>/Product3' incorporates:
       *  Constant: '<S56>/KalmanGainL'
       */
      ang_sin = Time_Varying_Model_2022a_P.KalmanGainL_Value;
      ang_cos = Time_Varying_Model_2022a_B.Sum_e;

      /* Product: '<S107>/Product3' */
      Time_Varying_Model_2022a_B.Product3_lw = ang_sin * ang_cos;
    }

    /* End of Outputs for SubSystem: '<S76>/MeasurementUpdate' */

    /* Sum: '<S76>/Add' incorporates:
     *  Constant: '<S55>/Enable'
     */
    Time_Varying_Model_2022a_B.Add_p = (Time_Varying_Model_2022a_B.Bkuk +
      Time_Varying_Model_2022a_B.Akxhatkk1) +
      Time_Varying_Model_2022a_B.Product3_lw;
  }

  /* End of RateTransition: '<S8>/Rate Transition' */
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
    /* Gain: '<Root>/Gain' incorporates:
     *  Constant: '<Root>/Constant11'
     */
    Time_Varying_Model_2022a_B.Gain_h = Time_Varying_Model_2022a_P.Gain_Gain_du *
      Time_Varying_Model_2022a_P.Constant11_Value;
  }

  /* MATLAB Function: '<Root>/MATLAB Function3' incorporates:
   *  Constant: '<Root>/Constant28'
   *  Constant: '<Root>/Constant37'
   *  Constant: '<Root>/Constant38'
   *  Constant: '<Root>/Constant6'
   */
  /* :  CNa1_num = 2*pi()*(s^2/A_ref); */
  /* :  CNa1_den = 1 + sqrt(1+(((s^2)*sqrt(1-M^2))/(A_fin*cos(G_c)))^2); */
  ang_sin = sqrt(1.0 - Time_Varying_Model_2022a_B.OpenRocketMachNo *
                 Time_Varying_Model_2022a_B.OpenRocketMachNo) *
    (Time_Varying_Model_2022a_P.Constant37_Value *
     Time_Varying_Model_2022a_P.Constant37_Value) /
    (Time_Varying_Model_2022a_P.Constant28_Value * cos
     (Time_Varying_Model_2022a_P.Constant38_Value));

  /* :  CNa1 = CNa1_num / CNa1_den; */
  /* :  CNa4 = 2*CNa1; */
  /* :  K_TB = 1 + r_t/(s+r_t); */
  /* :  CNa_can = K_TB*CNa4; */
  Time_Varying_Model_2022a_B.CNa_can =
    Time_Varying_Model_2022a_P.Constant37_Value *
    Time_Varying_Model_2022a_P.Constant37_Value /
    Time_Varying_Model_2022a_P.Constant6_Value * 6.2831853071795862 / (sqrt
    (ang_sin * ang_sin + 1.0) + 1.0) * 2.0 * (Time_Varying_Model_2022a_B.Gain_h /
    (Time_Varying_Model_2022a_P.Constant37_Value +
     Time_Varying_Model_2022a_B.Gain_h) + 1.0);

  /* Lookup_n-D: '<Root>/OpenRocket I_L' incorporates:
   *  Clock: '<Root>/Clock'
   */
  Time_Varying_Model_2022a_B.OpenRocketI_L = look1_binlxpw
    (Time_Varying_Model_2022a_B.Clock,
     Time_Varying_Model_2022a_P.OpenRocketI_L_bp01Data,
     Time_Varying_Model_2022a_P.OpenRocketI_L_tableData, 222U);

  /* Lookup_n-D: '<Root>/OpenRocket I_R' incorporates:
   *  Clock: '<Root>/Clock'
   */
  Time_Varying_Model_2022a_B.OpenRocketI_R = look1_binlxpw
    (Time_Varying_Model_2022a_B.Clock,
     Time_Varying_Model_2022a_P.OpenRocketI_R_bp01Data,
     Time_Varying_Model_2022a_P.OpenRocketI_R_tableData, 222U);

  /* Lookup_n-D: '<Root>/OpenRocket X_CG' incorporates:
   *  Clock: '<Root>/Clock'
   */
  Time_Varying_Model_2022a_B.OpenRocketX_CG = look1_binlxpw
    (Time_Varying_Model_2022a_B.Clock,
     Time_Varying_Model_2022a_P.OpenRocketX_CG_bp01Data,
     Time_Varying_Model_2022a_P.OpenRocketX_CG_tableData, 222U);

  /* Lookup_n-D: '<Root>/OpenRocket mDot' incorporates:
   *  Clock: '<Root>/Clock'
   */
  Time_Varying_Model_2022a_B.OpenRocketmDot = look1_binlxpw
    (Time_Varying_Model_2022a_B.Clock,
     Time_Varying_Model_2022a_P.OpenRocketmDot_bp01Data,
     Time_Varying_Model_2022a_P.OpenRocketmDot_tableData, 222U);

  /* Sum: '<Root>/Sum of Elements1' */
  Time_Varying_Model_2022a_B.SumofElements1_g =
    (Time_Varying_Model_2022a_B.CNa_fins + Time_Varying_Model_2022a_B.CNa_body)
    + Time_Varying_Model_2022a_B.CNa_nose;
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
    /* UnitConversion: '<S119>/Unit Conversion' incorporates:
     *  Constant: '<S113>/Wingspan'
     */
    /* Unit Conversion - from: m to: ft
       Expression: output = (3.28084*input) + (0) */
    Time_Varying_Model_2022a_B.UnitConversion_n = 3.280839895013123 *
      Time_Varying_Model_2022a_P.DrydenWindTurbulenceModelCont_j;

    /* Outputs for Enabled SubSystem: '<S116>/Hpgw' incorporates:
     *  EnablePort: '<S127>/Enable'
     */
    if (rtsiIsModeUpdateTimeStep(&Time_Varying_Model_2022a_M->solverInfo)) {
      /* Constant: '<S116>/Constant1' */
      if (Time_Varying_Model_2022a_P.DrydenWindTurbulenceModelCont_c > 0.0) {
        if (!Time_Varying_Model_2022a_DW.Hpgw_MODE) {
          (void) memset(&(Time_Varying_Model_2022a_XDis.pgw_p_CSTATE), 0,
                        2*sizeof(boolean_T));

          /* InitializeConditions for Integrator: '<S127>/pgw_p' */
          Time_Varying_Model_2022a_X.pgw_p_CSTATE[0] =
            Time_Varying_Model_2022a_P.pgw_p_IC;
          Time_Varying_Model_2022a_X.pgw_p_CSTATE[1] =
            Time_Varying_Model_2022a_P.pgw_p_IC;
          Time_Varying_Model_2022a_DW.Hpgw_MODE = true;
        }
      } else {
        /* Outputs for Enabled SubSystem: '<S117>/Hugw(s)' incorporates:
         *  EnablePort: '<S130>/Enable'
         */
        ang_cos = rtmGetTStart(Time_Varying_Model_2022a_M);

        /* End of Outputs for SubSystem: '<S117>/Hugw(s)' */
        if ((((Time_Varying_Model_2022a_M->Timing.clockTick1+
               Time_Varying_Model_2022a_M->Timing.clockTickH1* 4294967296.0)) *
             0.01) == ang_cos) {
          (void) memset(&(Time_Varying_Model_2022a_XDis.pgw_p_CSTATE), 1,
                        2*sizeof(boolean_T));
        }

        if (Time_Varying_Model_2022a_DW.Hpgw_MODE) {
          (void) memset(&(Time_Varying_Model_2022a_XDis.pgw_p_CSTATE), 1,
                        2*sizeof(boolean_T));

          /* Disable for Product: '<S127>/sigma_w' incorporates:
           *  Outport: '<S127>/pgw'
           */
          Time_Varying_Model_2022a_B.sigma_w[0] =
            Time_Varying_Model_2022a_P.pgw_Y0;
          Time_Varying_Model_2022a_B.sigma_w[1] =
            Time_Varying_Model_2022a_P.pgw_Y0;
          Time_Varying_Model_2022a_DW.Hpgw_MODE = false;
        }
      }

      /* End of Constant: '<S116>/Constant1' */
    }

    /* End of Outputs for SubSystem: '<S116>/Hpgw' */
  }

  /* Outputs for Enabled SubSystem: '<S116>/Hpgw' incorporates:
   *  EnablePort: '<S127>/Enable'
   */
  if (Time_Varying_Model_2022a_DW.Hpgw_MODE) {
    /* Math: '<S127>/L^1//3' incorporates:
     *  Constant: '<S127>/Constant1'
     */
    ang_cos = Time_Varying_Model_2022a_B.Lw[0];
    if ((ang_cos < 0.0) && (Time_Varying_Model_2022a_P.Constant1_Value > floor
                            (Time_Varying_Model_2022a_P.Constant1_Value))) {
      ang_cos = -rt_powd_snf(-ang_cos,
        Time_Varying_Model_2022a_P.Constant1_Value);
    } else {
      ang_cos = rt_powd_snf(ang_cos, Time_Varying_Model_2022a_P.Constant1_Value);
    }

    /* Math: '<S127>/L^1//3' */
    Time_Varying_Model_2022a_B.L13[0] = ang_cos;

    /* Math: '<S127>/L^1//3' incorporates:
     *  Constant: '<S127>/Constant1'
     */
    ang_cos = Time_Varying_Model_2022a_B.Lw[1];
    if ((ang_cos < 0.0) && (Time_Varying_Model_2022a_P.Constant1_Value > floor
                            (Time_Varying_Model_2022a_P.Constant1_Value))) {
      ang_cos = -rt_powd_snf(-ang_cos,
        Time_Varying_Model_2022a_P.Constant1_Value);
    } else {
      ang_cos = rt_powd_snf(ang_cos, Time_Varying_Model_2022a_P.Constant1_Value);
    }

    /* Math: '<S127>/L^1//3' */
    Time_Varying_Model_2022a_B.L13[1] = ang_cos;
    if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
        Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
      /* Product: '<S127>/w4' incorporates:
       *  Constant: '<S127>/Constant3'
       */
      Time_Varying_Model_2022a_B.w4 = Time_Varying_Model_2022a_P.Constant3_Value
        / Time_Varying_Model_2022a_B.UnitConversion_n;

      /* Math: '<S127>/u^1//6' incorporates:
       *  Constant: '<S127>/Constant2'
       */
      ang_sin = Time_Varying_Model_2022a_B.w4;
      ang_cos = Time_Varying_Model_2022a_P.Constant2_Value;
      if ((ang_sin < 0.0) && (ang_cos > floor(ang_cos))) {
        /* Math: '<S127>/u^1//6' */
        Time_Varying_Model_2022a_B.u16 = -rt_powd_snf(-ang_sin, ang_cos);
      } else {
        /* Math: '<S127>/u^1//6' */
        Time_Varying_Model_2022a_B.u16 = rt_powd_snf(ang_sin, ang_cos);
      }

      /* End of Math: '<S127>/u^1//6' */
    }

    /* Fcn: '<S127>/sqrt(0.8//V)' */
    ang_sin = 0.8 / Time_Varying_Model_2022a_B.UnitConversion_e;
    if (ang_sin < 0.0) {
      ang_sin = -sqrt(-ang_sin);
    } else {
      ang_sin = sqrt(ang_sin);
    }

    /* Fcn: '<S127>/sqrt(0.8//V)' */
    Time_Varying_Model_2022a_B.sqrt08V = ang_sin;

    /* Product: '<S127>/w3' */
    Time_Varying_Model_2022a_B.w3 = Time_Varying_Model_2022a_B.UnitConversion_e *
      Time_Varying_Model_2022a_B.w4;

    /* Product: '<S127>/w1' */
    ang_cos = Time_Varying_Model_2022a_B.sqrt08V /
      Time_Varying_Model_2022a_B.L13[0];
    Time_Varying_Model_2022a_B.w1_k[0] = ang_cos;

    /* Product: '<S127>/w2' */
    ang_cos *= Time_Varying_Model_2022a_B.u16;
    Time_Varying_Model_2022a_B.w2[0] = ang_cos;

    /* Product: '<S127>/Lug//V1' */
    ang_cos *= Time_Varying_Model_2022a_B.Product_p[3];
    Time_Varying_Model_2022a_B.LugV1_mq[0] = ang_cos;

    /* Integrator: '<S127>/pgw_p' */
    ang_sin = Time_Varying_Model_2022a_X.pgw_p_CSTATE[0];
    Time_Varying_Model_2022a_B.pgw_p[0] = ang_sin;

    /* Sum: '<S127>/Sum' */
    ang_cos -= ang_sin;
    Time_Varying_Model_2022a_B.Sum_b1[0] = ang_cos;

    /* Product: '<S127>/w' */
    Time_Varying_Model_2022a_B.w_m[0] = Time_Varying_Model_2022a_B.w3 * ang_cos;

    /* Product: '<S127>/w1' */
    ang_cos = Time_Varying_Model_2022a_B.sqrt08V /
      Time_Varying_Model_2022a_B.L13[1];
    Time_Varying_Model_2022a_B.w1_k[1] = ang_cos;

    /* Product: '<S127>/w2' */
    ang_cos *= Time_Varying_Model_2022a_B.u16;
    Time_Varying_Model_2022a_B.w2[1] = ang_cos;

    /* Product: '<S127>/Lug//V1' */
    ang_cos *= Time_Varying_Model_2022a_B.Product_p[3];
    Time_Varying_Model_2022a_B.LugV1_mq[1] = ang_cos;

    /* Integrator: '<S127>/pgw_p' */
    ang_sin = Time_Varying_Model_2022a_X.pgw_p_CSTATE[1];
    Time_Varying_Model_2022a_B.pgw_p[1] = ang_sin;

    /* Sum: '<S127>/Sum' */
    ang_cos -= ang_sin;
    Time_Varying_Model_2022a_B.Sum_b1[1] = ang_cos;

    /* Product: '<S127>/w' */
    Time_Varying_Model_2022a_B.w_m[1] = Time_Varying_Model_2022a_B.w3 * ang_cos;

    /* Product: '<S127>/sigma_w' */
    Time_Varying_Model_2022a_B.sigma_w[0] = Time_Varying_Model_2022a_B.sigma_wg *
      Time_Varying_Model_2022a_B.pgw_p[0];
    Time_Varying_Model_2022a_B.sigma_w[1] =
      Time_Varying_Model_2022a_B.MediumHighAltitudeIntensity *
      Time_Varying_Model_2022a_B.pgw_p[1];
  }

  /* End of Outputs for SubSystem: '<S116>/Hpgw' */

  /* Outputs for Enabled SubSystem: '<S116>/Hqgw' incorporates:
   *  EnablePort: '<S128>/Enable'
   */
  if ((rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
       Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) &&
      rtsiIsModeUpdateTimeStep(&Time_Varying_Model_2022a_M->solverInfo)) {
    /* Constant: '<S116>/Constant2' */
    if (Time_Varying_Model_2022a_P.DrydenWindTurbulenceModelCont_c > 0.0) {
      if (!Time_Varying_Model_2022a_DW.Hqgw_MODE) {
        (void) memset(&(Time_Varying_Model_2022a_XDis.qgw_p_CSTATE), 0,
                      2*sizeof(boolean_T));

        /* InitializeConditions for Integrator: '<S128>/qgw_p' */
        Time_Varying_Model_2022a_X.qgw_p_CSTATE[0] =
          Time_Varying_Model_2022a_P.qgw_p_IC;
        Time_Varying_Model_2022a_X.qgw_p_CSTATE[1] =
          Time_Varying_Model_2022a_P.qgw_p_IC;
        Time_Varying_Model_2022a_DW.Hqgw_MODE = true;
      }
    } else {
      /* Outputs for Enabled SubSystem: '<S117>/Hugw(s)' incorporates:
       *  EnablePort: '<S130>/Enable'
       */
      ang_cos = rtmGetTStart(Time_Varying_Model_2022a_M);

      /* End of Outputs for SubSystem: '<S117>/Hugw(s)' */
      if ((((Time_Varying_Model_2022a_M->Timing.clockTick1+
             Time_Varying_Model_2022a_M->Timing.clockTickH1* 4294967296.0)) *
           0.01) == ang_cos) {
        (void) memset(&(Time_Varying_Model_2022a_XDis.qgw_p_CSTATE), 1,
                      2*sizeof(boolean_T));
      }

      if (Time_Varying_Model_2022a_DW.Hqgw_MODE) {
        (void) memset(&(Time_Varying_Model_2022a_XDis.qgw_p_CSTATE), 1,
                      2*sizeof(boolean_T));

        /* Disable for Product: '<S128>/w' incorporates:
         *  Outport: '<S128>/qgw'
         */
        Time_Varying_Model_2022a_B.w_gw[0] = Time_Varying_Model_2022a_P.qgw_Y0;
        Time_Varying_Model_2022a_B.w_gw[1] = Time_Varying_Model_2022a_P.qgw_Y0;
        Time_Varying_Model_2022a_DW.Hqgw_MODE = false;
      }
    }

    /* End of Constant: '<S116>/Constant2' */
  }

  if (Time_Varying_Model_2022a_DW.Hqgw_MODE) {
    /* Gain: '<S128>/pi//4' */
    Time_Varying_Model_2022a_B.pi4 = Time_Varying_Model_2022a_P.pi4_Gain *
      Time_Varying_Model_2022a_B.UnitConversion_e;

    /* Product: '<S128>/wg//V' */
    ang_cos = Time_Varying_Model_2022a_B.LwgV1[0] /
      Time_Varying_Model_2022a_B.UnitConversion_e;
    Time_Varying_Model_2022a_B.wgV[0] = ang_cos;

    /* Integrator: '<S128>/qgw_p' */
    ang_sin = Time_Varying_Model_2022a_X.qgw_p_CSTATE[0];
    Time_Varying_Model_2022a_B.qgw_p[0] = ang_sin;

    /* Sum: '<S128>/Sum' */
    ang_cos -= ang_sin;
    Time_Varying_Model_2022a_B.Sum_m[0] = ang_cos;

    /* Product: '<S128>/w' */
    Time_Varying_Model_2022a_B.w_gw[0] = Time_Varying_Model_2022a_B.pi4 /
      Time_Varying_Model_2022a_B.UnitConversion_n * ang_cos;

    /* Product: '<S128>/wg//V' */
    ang_cos = Time_Varying_Model_2022a_B.LwgV1[1] /
      Time_Varying_Model_2022a_B.UnitConversion_e;
    Time_Varying_Model_2022a_B.wgV[1] = ang_cos;

    /* Integrator: '<S128>/qgw_p' */
    ang_sin = Time_Varying_Model_2022a_X.qgw_p_CSTATE[1];
    Time_Varying_Model_2022a_B.qgw_p[1] = ang_sin;

    /* Sum: '<S128>/Sum' */
    ang_cos -= ang_sin;
    Time_Varying_Model_2022a_B.Sum_m[1] = ang_cos;

    /* Product: '<S128>/w' */
    Time_Varying_Model_2022a_B.w_gw[1] = Time_Varying_Model_2022a_B.pi4 /
      Time_Varying_Model_2022a_B.UnitConversion_n * ang_cos;
  }

  /* End of Outputs for SubSystem: '<S116>/Hqgw' */

  /* Outputs for Enabled SubSystem: '<S116>/Hrgw' incorporates:
   *  EnablePort: '<S129>/Enable'
   */
  if ((rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
       Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) &&
      rtsiIsModeUpdateTimeStep(&Time_Varying_Model_2022a_M->solverInfo)) {
    /* Constant: '<S116>/Constant3' */
    if (Time_Varying_Model_2022a_P.DrydenWindTurbulenceModelCont_c > 0.0) {
      if (!Time_Varying_Model_2022a_DW.Hrgw_MODE) {
        (void) memset(&(Time_Varying_Model_2022a_XDis.rgw_p_CSTATE), 0,
                      2*sizeof(boolean_T));

        /* InitializeConditions for Integrator: '<S129>/rgw_p' */
        Time_Varying_Model_2022a_X.rgw_p_CSTATE[0] =
          Time_Varying_Model_2022a_P.rgw_p_IC;
        Time_Varying_Model_2022a_X.rgw_p_CSTATE[1] =
          Time_Varying_Model_2022a_P.rgw_p_IC;
        Time_Varying_Model_2022a_DW.Hrgw_MODE = true;
      }
    } else {
      /* Outputs for Enabled SubSystem: '<S117>/Hugw(s)' incorporates:
       *  EnablePort: '<S130>/Enable'
       */
      ang_cos = rtmGetTStart(Time_Varying_Model_2022a_M);

      /* End of Outputs for SubSystem: '<S117>/Hugw(s)' */
      if ((((Time_Varying_Model_2022a_M->Timing.clockTick1+
             Time_Varying_Model_2022a_M->Timing.clockTickH1* 4294967296.0)) *
           0.01) == ang_cos) {
        (void) memset(&(Time_Varying_Model_2022a_XDis.rgw_p_CSTATE), 1,
                      2*sizeof(boolean_T));
      }

      if (Time_Varying_Model_2022a_DW.Hrgw_MODE) {
        (void) memset(&(Time_Varying_Model_2022a_XDis.rgw_p_CSTATE), 1,
                      2*sizeof(boolean_T));

        /* Disable for Product: '<S129>/w' incorporates:
         *  Outport: '<S129>/rgw'
         */
        Time_Varying_Model_2022a_B.w_g[0] = Time_Varying_Model_2022a_P.rgw_Y0;
        Time_Varying_Model_2022a_B.w_g[1] = Time_Varying_Model_2022a_P.rgw_Y0;
        Time_Varying_Model_2022a_DW.Hrgw_MODE = false;
      }
    }

    /* End of Constant: '<S116>/Constant3' */
  }

  if (Time_Varying_Model_2022a_DW.Hrgw_MODE) {
    /* Gain: '<S129>/pi//3' */
    Time_Varying_Model_2022a_B.pi3 = Time_Varying_Model_2022a_P.pi3_Gain *
      Time_Varying_Model_2022a_B.UnitConversion_e;

    /* Product: '<S129>/vg//V' */
    ang_cos = Time_Varying_Model_2022a_B.w1[0] /
      Time_Varying_Model_2022a_B.UnitConversion_e;
    Time_Varying_Model_2022a_B.vgV[0] = ang_cos;

    /* Integrator: '<S129>/rgw_p' */
    ang_sin = Time_Varying_Model_2022a_X.rgw_p_CSTATE[0];
    Time_Varying_Model_2022a_B.rgw_p[0] = ang_sin;

    /* Sum: '<S129>/Sum' */
    ang_cos -= ang_sin;
    Time_Varying_Model_2022a_B.Sum_bn[0] = ang_cos;

    /* Product: '<S129>/w' */
    Time_Varying_Model_2022a_B.w_g[0] = Time_Varying_Model_2022a_B.pi3 /
      Time_Varying_Model_2022a_B.UnitConversion_n * ang_cos;

    /* Product: '<S129>/vg//V' */
    ang_cos = Time_Varying_Model_2022a_B.w1[1] /
      Time_Varying_Model_2022a_B.UnitConversion_e;
    Time_Varying_Model_2022a_B.vgV[1] = ang_cos;

    /* Integrator: '<S129>/rgw_p' */
    ang_sin = Time_Varying_Model_2022a_X.rgw_p_CSTATE[1];
    Time_Varying_Model_2022a_B.rgw_p[1] = ang_sin;

    /* Sum: '<S129>/Sum' */
    ang_cos -= ang_sin;
    Time_Varying_Model_2022a_B.Sum_bn[1] = ang_cos;

    /* Product: '<S129>/w' */
    Time_Varying_Model_2022a_B.w_g[1] = Time_Varying_Model_2022a_B.pi3 /
      Time_Varying_Model_2022a_B.UnitConversion_n * ang_cos;
  }

  /* End of Outputs for SubSystem: '<S116>/Hrgw' */

  /* If: '<S121>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' incorporates:
   *  Merge: '<S138>/Merge'
   *  Product: '<S141>/Product'
   */
  if (rtsiIsModeUpdateTimeStep(&Time_Varying_Model_2022a_M->solverInfo)) {
    if (Time_Varying_Model_2022a_B.UnitConversion <= 1000.0) {
      rtAction = 0;
    } else if (Time_Varying_Model_2022a_B.UnitConversion >= 2000.0) {
      rtAction = 1;
    } else {
      rtAction = 2;
    }

    Time_Varying_Model_2022a_DW.ifHeightMaxlowaltitudeelseifH_k = rtAction;
  } else {
    rtAction = Time_Varying_Model_2022a_DW.ifHeightMaxlowaltitudeelseifH_k;
  }

  switch (rtAction) {
   case 0:
    /* Outputs for IfAction SubSystem: '<S121>/Low altitude  rates' incorporates:
     *  ActionPort: '<S136>/Action Port'
     */
    /* SignalConversion generated from: '<S141>/Vector Concatenate' incorporates:
     *  Concatenate: '<S141>/Vector Concatenate'
     */
    Time_Varying_Model_2022a_B.VectorConcatenate_m[2] =
      Time_Varying_Model_2022a_B.w_g[0];

    /* Trigonometry: '<S142>/Trigonometric Function1' */
    ang_sin = Time_Varying_Model_2022a_B.UnitConversion_gl;
    ang_cos = sin(ang_sin);
    ang_sin = cos(ang_sin);

    /* Trigonometry: '<S142>/Trigonometric Function1' */
    Time_Varying_Model_2022a_B.TrigonometricFunction1_o1 = ang_cos;

    /* Trigonometry: '<S142>/Trigonometric Function1' */
    Time_Varying_Model_2022a_B.TrigonometricFunction1_o2 = ang_sin;

    /* Product: '<S142>/Product2' */
    Time_Varying_Model_2022a_B.Product2_dy[0] =
      Time_Varying_Model_2022a_B.sigma_w[0] *
      Time_Varying_Model_2022a_B.TrigonometricFunction1_o2;
    Time_Varying_Model_2022a_B.Product2_dy[1] = Time_Varying_Model_2022a_B.w_gw
      [0] * Time_Varying_Model_2022a_B.TrigonometricFunction1_o2;

    /* Product: '<S142>/Product1' */
    Time_Varying_Model_2022a_B.Product1_if[0] =
      Time_Varying_Model_2022a_B.TrigonometricFunction1_o1 *
      Time_Varying_Model_2022a_B.sigma_w[0];
    Time_Varying_Model_2022a_B.Product1_if[1] =
      Time_Varying_Model_2022a_B.TrigonometricFunction1_o1 *
      Time_Varying_Model_2022a_B.w_gw[0];

    /* Sum: '<S142>/Sum' incorporates:
     *  Concatenate: '<S141>/Vector Concatenate'
     */
    Time_Varying_Model_2022a_B.VectorConcatenate_m[0] =
      Time_Varying_Model_2022a_B.Product2_dy[0] -
      Time_Varying_Model_2022a_B.Product1_if[1];

    /* Sum: '<S142>/Sum1' incorporates:
     *  Concatenate: '<S141>/Vector Concatenate'
     */
    Time_Varying_Model_2022a_B.VectorConcatenate_m[1] =
      Time_Varying_Model_2022a_B.Product1_if[0] +
      Time_Varying_Model_2022a_B.Product2_dy[1];

    /* Product: '<S141>/Product' incorporates:
     *  Concatenate: '<S141>/Vector Concatenate'
     */
    memcpy(&rotmZYX[0], &Time_Varying_Model_2022a_B.DCM[0], 9U * sizeof(real_T));
    ang_sin = Time_Varying_Model_2022a_B.VectorConcatenate_m[0];
    st_idx_1 = Time_Varying_Model_2022a_B.VectorConcatenate_m[1];
    ang_cos = Time_Varying_Model_2022a_B.VectorConcatenate_m[2];

    /* End of Outputs for SubSystem: '<S121>/Low altitude  rates' */
    for (i = 0; i <= 0; i += 2) {
      /* Outputs for IfAction SubSystem: '<S121>/Low altitude  rates' incorporates:
       *  ActionPort: '<S136>/Action Port'
       */
      tmp_0 = _mm_loadu_pd(&rotmZYX[i]);
      tmp_0 = _mm_mul_pd(tmp_0, _mm_set1_pd(ang_sin));
      tmp_1 = _mm_loadu_pd(&rotmZYX[i + 3]);
      tmp_1 = _mm_mul_pd(tmp_1, _mm_set1_pd(st_idx_1));

      /* End of Outputs for SubSystem: '<S121>/Low altitude  rates' */
      tmp_0 = _mm_add_pd(tmp_1, tmp_0);

      /* Outputs for IfAction SubSystem: '<S121>/Low altitude  rates' incorporates:
       *  ActionPort: '<S136>/Action Port'
       */
      tmp_1 = _mm_loadu_pd(&rotmZYX[i + 6]);
      tmp_1 = _mm_mul_pd(tmp_1, _mm_set1_pd(ang_cos));

      /* End of Outputs for SubSystem: '<S121>/Low altitude  rates' */
      tmp_0 = _mm_add_pd(tmp_1, tmp_0);

      /* Outputs for IfAction SubSystem: '<S121>/Low altitude  rates' incorporates:
       *  ActionPort: '<S136>/Action Port'
       */
      _mm_storeu_pd(&Time_Varying_Model_2022a_B.Product_dt[i], tmp_0);
      _mm_storeu_pd(&Time_Varying_Model_2022a_B.Merge_n[i], tmp_0);

      /* End of Outputs for SubSystem: '<S121>/Low altitude  rates' */
    }

    /* Outputs for IfAction SubSystem: '<S121>/Low altitude  rates' incorporates:
     *  ActionPort: '<S136>/Action Port'
     */
    for (i = 2; i < 3; i++) {
      /* Product: '<S141>/Product' */
      ct_idx_0 = rotmZYX[i] * ang_sin;
      ct_idx_0 += rotmZYX[i + 3] * st_idx_1;
      ct_idx_0 += rotmZYX[i + 6] * ang_cos;
      Time_Varying_Model_2022a_B.Product_dt[i] = ct_idx_0;

      /* Merge: '<S138>/Merge' incorporates:
       *  Product: '<S141>/Product'
       *  Reshape: '<S141>/Reshape1'
       */
      Time_Varying_Model_2022a_B.Merge_n[i] = ct_idx_0;
    }

    /* End of Outputs for SubSystem: '<S121>/Low altitude  rates' */
    break;

   case 1:
    /* Outputs for IfAction SubSystem: '<S121>/Medium//High  altitude rates' incorporates:
     *  ActionPort: '<S137>/Action Port'
     */
    /* Merge: '<S138>/Merge' incorporates:
     *  Gain: '<S137>/Gain'
     */
    Time_Varying_Model_2022a_B.Merge_n[0] = Time_Varying_Model_2022a_P.Gain_Gain
      * Time_Varying_Model_2022a_B.sigma_w[1];
    Time_Varying_Model_2022a_B.Merge_n[1] = Time_Varying_Model_2022a_P.Gain_Gain
      * Time_Varying_Model_2022a_B.w_gw[1];
    Time_Varying_Model_2022a_B.Merge_n[2] = Time_Varying_Model_2022a_P.Gain_Gain
      * Time_Varying_Model_2022a_B.w_g[1];

    /* End of Outputs for SubSystem: '<S121>/Medium//High  altitude rates' */
    break;

   default:
    /* Outputs for IfAction SubSystem: '<S121>/Interpolate  rates' incorporates:
     *  ActionPort: '<S135>/Action Port'
     */
    /* Trigonometry: '<S140>/Trigonometric Function' */
    ang_sin = Time_Varying_Model_2022a_B.UnitConversion_gl;
    ang_cos = sin(ang_sin);
    ang_sin = cos(ang_sin);

    /* Trigonometry: '<S140>/Trigonometric Function' */
    Time_Varying_Model_2022a_B.TrigonometricFunction_o1_g = ang_cos;

    /* Trigonometry: '<S140>/Trigonometric Function' */
    Time_Varying_Model_2022a_B.TrigonometricFunction_o2_m = ang_sin;

    /* Product: '<S140>/Product2' */
    Time_Varying_Model_2022a_B.Product2_k[0] =
      Time_Varying_Model_2022a_B.sigma_w[0] *
      Time_Varying_Model_2022a_B.TrigonometricFunction_o2_m;
    Time_Varying_Model_2022a_B.Product2_k[1] = Time_Varying_Model_2022a_B.w_gw[0]
      * Time_Varying_Model_2022a_B.TrigonometricFunction_o2_m;

    /* Product: '<S140>/Product1' */
    Time_Varying_Model_2022a_B.Product1_dm[0] =
      Time_Varying_Model_2022a_B.TrigonometricFunction_o1_g *
      Time_Varying_Model_2022a_B.sigma_w[0];
    Time_Varying_Model_2022a_B.Product1_dm[1] =
      Time_Varying_Model_2022a_B.TrigonometricFunction_o1_g *
      Time_Varying_Model_2022a_B.w_gw[0];

    /* Sum: '<S140>/Sum' incorporates:
     *  Concatenate: '<S139>/Vector Concatenate'
     */
    Time_Varying_Model_2022a_B.VectorConcatenate_i[0] =
      Time_Varying_Model_2022a_B.Product2_k[0] -
      Time_Varying_Model_2022a_B.Product1_dm[1];

    /* Sum: '<S140>/Sum1' incorporates:
     *  Concatenate: '<S139>/Vector Concatenate'
     */
    Time_Varying_Model_2022a_B.VectorConcatenate_i[1] =
      Time_Varying_Model_2022a_B.Product1_dm[0] +
      Time_Varying_Model_2022a_B.Product2_k[1];

    /* SignalConversion generated from: '<S139>/Vector Concatenate' incorporates:
     *  Concatenate: '<S139>/Vector Concatenate'
     */
    Time_Varying_Model_2022a_B.VectorConcatenate_i[2] =
      Time_Varying_Model_2022a_B.w_g[0];

    /* Product: '<S139>/Product' incorporates:
     *  Concatenate: '<S139>/Vector Concatenate'
     */
    memcpy(&rotmZYX[0], &Time_Varying_Model_2022a_B.DCM[0], 9U * sizeof(real_T));
    ang_sin = Time_Varying_Model_2022a_B.VectorConcatenate_i[0];
    st_idx_1 = Time_Varying_Model_2022a_B.VectorConcatenate_i[1];
    ang_cos = Time_Varying_Model_2022a_B.VectorConcatenate_i[2];

    /* End of Outputs for SubSystem: '<S121>/Interpolate  rates' */
    for (i_0 = 0; i_0 <= 0; i_0 += 2) {
      /* Outputs for IfAction SubSystem: '<S121>/Interpolate  rates' incorporates:
       *  ActionPort: '<S135>/Action Port'
       */
      tmp_0 = _mm_loadu_pd(&rotmZYX[i_0]);
      tmp_0 = _mm_mul_pd(tmp_0, _mm_set1_pd(ang_sin));
      tmp_1 = _mm_loadu_pd(&rotmZYX[i_0 + 3]);
      tmp_1 = _mm_mul_pd(tmp_1, _mm_set1_pd(st_idx_1));

      /* End of Outputs for SubSystem: '<S121>/Interpolate  rates' */
      tmp_0 = _mm_add_pd(tmp_1, tmp_0);

      /* Outputs for IfAction SubSystem: '<S121>/Interpolate  rates' incorporates:
       *  ActionPort: '<S135>/Action Port'
       */
      tmp_1 = _mm_loadu_pd(&rotmZYX[i_0 + 6]);
      tmp_1 = _mm_mul_pd(tmp_1, _mm_set1_pd(ang_cos));

      /* End of Outputs for SubSystem: '<S121>/Interpolate  rates' */
      tmp_0 = _mm_add_pd(tmp_1, tmp_0);

      /* Outputs for IfAction SubSystem: '<S121>/Interpolate  rates' incorporates:
       *  ActionPort: '<S135>/Action Port'
       */
      _mm_storeu_pd(&Time_Varying_Model_2022a_B.Product_g[i_0], tmp_0);

      /* End of Outputs for SubSystem: '<S121>/Interpolate  rates' */
    }

    /* Outputs for IfAction SubSystem: '<S121>/Interpolate  rates' incorporates:
     *  ActionPort: '<S135>/Action Port'
     */
    for (i_0 = 2; i_0 < 3; i_0++) {
      /* Product: '<S139>/Product' */
      ct_idx_0 = rotmZYX[i_0] * ang_sin;
      ct_idx_0 += rotmZYX[i_0 + 3] * st_idx_1;
      ct_idx_0 += rotmZYX[i_0 + 6] * ang_cos;
      Time_Varying_Model_2022a_B.Product_g[i_0] = ct_idx_0;
    }

    /* Sum: '<S135>/Sum2' incorporates:
     *  Product: '<S139>/Product'
     */
    Time_Varying_Model_2022a_B.Sum2_f[0] = Time_Varying_Model_2022a_B.sigma_w[1]
      - Time_Varying_Model_2022a_B.Product_g[0];
    Time_Varying_Model_2022a_B.Sum2_f[1] = Time_Varying_Model_2022a_B.w_gw[1] -
      Time_Varying_Model_2022a_B.Product_g[1];
    Time_Varying_Model_2022a_B.Sum2_f[2] = Time_Varying_Model_2022a_B.w_g[1] -
      Time_Varying_Model_2022a_B.Product_g[2];

    /* Sum: '<S135>/Sum1' incorporates:
     *  Constant: '<S135>/max_height_low'
     */
    Time_Varying_Model_2022a_B.Sum1_p =
      Time_Varying_Model_2022a_B.UnitConversion -
      Time_Varying_Model_2022a_P.max_height_low_Value;

    /* Sum: '<S135>/Sum' incorporates:
     *  Constant: '<S135>/max_height_low'
     *  Constant: '<S135>/min_height_high'
     */
    Time_Varying_Model_2022a_B.Sum_d =
      Time_Varying_Model_2022a_P.min_height_high_Value -
      Time_Varying_Model_2022a_P.max_height_low_Value;

    /* Product: '<S135>/Product1' */
    ang_cos = Time_Varying_Model_2022a_B.Sum2_f[0] *
      Time_Varying_Model_2022a_B.Sum1_p / Time_Varying_Model_2022a_B.Sum_d;

    /* End of Outputs for SubSystem: '<S121>/Interpolate  rates' */
    Time_Varying_Model_2022a_B.Product1_he[0] = ang_cos;

    /* Outputs for IfAction SubSystem: '<S121>/Interpolate  rates' incorporates:
     *  ActionPort: '<S135>/Action Port'
     */
    /* Merge: '<S138>/Merge' incorporates:
     *  Sum: '<S135>/Sum3'
     */
    Time_Varying_Model_2022a_B.Merge_n[0] =
      Time_Varying_Model_2022a_B.Product_g[0] + ang_cos;

    /* Product: '<S135>/Product1' */
    ang_cos = Time_Varying_Model_2022a_B.Sum2_f[1] *
      Time_Varying_Model_2022a_B.Sum1_p / Time_Varying_Model_2022a_B.Sum_d;

    /* End of Outputs for SubSystem: '<S121>/Interpolate  rates' */
    Time_Varying_Model_2022a_B.Product1_he[1] = ang_cos;

    /* Outputs for IfAction SubSystem: '<S121>/Interpolate  rates' incorporates:
     *  ActionPort: '<S135>/Action Port'
     */
    /* Merge: '<S138>/Merge' incorporates:
     *  Sum: '<S135>/Sum3'
     */
    Time_Varying_Model_2022a_B.Merge_n[1] =
      Time_Varying_Model_2022a_B.Product_g[1] + ang_cos;

    /* Product: '<S135>/Product1' */
    ang_cos = Time_Varying_Model_2022a_B.Sum2_f[2] *
      Time_Varying_Model_2022a_B.Sum1_p / Time_Varying_Model_2022a_B.Sum_d;

    /* End of Outputs for SubSystem: '<S121>/Interpolate  rates' */
    Time_Varying_Model_2022a_B.Product1_he[2] = ang_cos;

    /* Outputs for IfAction SubSystem: '<S121>/Interpolate  rates' incorporates:
     *  ActionPort: '<S135>/Action Port'
     */
    /* Merge: '<S138>/Merge' incorporates:
     *  Sum: '<S135>/Sum3'
     */
    Time_Varying_Model_2022a_B.Merge_n[2] =
      Time_Varying_Model_2022a_B.Product_g[2] + ang_cos;

    /* End of Outputs for SubSystem: '<S121>/Interpolate  rates' */
    break;
  }

  /* End of If: '<S121>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */

  /* Gain: '<S26>/Gain' */
  Time_Varying_Model_2022a_B.Gain_jr = Time_Varying_Model_2022a_P.Gain_Gain_c *
    Time_Varying_Model_2022a_B.Integrator3;

  /* Outport: '<Root>/psiOut' */
  Time_Varying_Model_2022a_Y.yawAngle = Time_Varying_Model_2022a_B.Gain_jr;

  /* Gain: '<S30>/Gain' */
  Time_Varying_Model_2022a_B.Gain_nq = Time_Varying_Model_2022a_P.Gain_Gain_n *
    Time_Varying_Model_2022a_B.Integrator2;

  /* Outport: '<Root>/pOut' */
  Time_Varying_Model_2022a_Y.pOut = Time_Varying_Model_2022a_B.Gain_nq;

  /* Gain: '<S31>/Gain' */
  Time_Varying_Model_2022a_B.Gain_o = Time_Varying_Model_2022a_P.Gain_Gain_f5 *
    Time_Varying_Model_2022a_B.Integrator;

  /* Outport: '<Root>/qOut' */
  Time_Varying_Model_2022a_Y.qOut = Time_Varying_Model_2022a_B.Gain_o;

  /* Gain: '<S32>/Gain' */
  Time_Varying_Model_2022a_B.Gain_bo = Time_Varying_Model_2022a_P.Gain_Gain_m *
    Time_Varying_Model_2022a_B.Integrator1;

  /* Outport: '<Root>/rOut' */
  Time_Varying_Model_2022a_Y.rOut = Time_Varying_Model_2022a_B.Gain_bo;

  /* Gain: '<S33>/Gain' */
  Time_Varying_Model_2022a_B.Gain_om = Time_Varying_Model_2022a_P.Gain_Gain_l *
    Time_Varying_Model_2022a_B.Integrator5;

  /* Outport: '<Root>/phiOut' */
  Time_Varying_Model_2022a_Y.rollAngle = Time_Varying_Model_2022a_B.Gain_om;

  /* Sum: '<S2>/Sum' */
  Time_Varying_Model_2022a_B.Sum_j = Time_Varying_Model_2022a_B.Divide_c -
    Time_Varying_Model_2022a_B.OpenRocketX_CG;

  /* Product: '<S2>/Product' incorporates:
   *  Constant: '<Root>/Constant6'
   */
  Time_Varying_Model_2022a_B.Product_o = Time_Varying_Model_2022a_B.Gain3 *
    Time_Varying_Model_2022a_P.Constant6_Value *
    Time_Varying_Model_2022a_B.SumofElements1_g *
    Time_Varying_Model_2022a_B.Sum_j;

  /* Sum: '<S3>/Sum' incorporates:
   *  Constant: '<Root>/Constant22'
   */
  Time_Varying_Model_2022a_B.Sum_h = Time_Varying_Model_2022a_P.Constant22_Value
    - Time_Varying_Model_2022a_B.OpenRocketX_CG;

  /* Math: '<S3>/Square' */
  Time_Varying_Model_2022a_B.Square_g = Time_Varying_Model_2022a_B.Sum_h *
    Time_Varying_Model_2022a_B.Sum_h;

  /* Product: '<S3>/Product' */
  Time_Varying_Model_2022a_B.Product_m = Time_Varying_Model_2022a_B.CNa_nose *
    Time_Varying_Model_2022a_B.Square_g;

  /* Sum: '<S3>/Sum1' incorporates:
   *  Constant: '<Root>/Constant23'
   */
  Time_Varying_Model_2022a_B.Sum1_o =
    Time_Varying_Model_2022a_P.Constant23_Value -
    Time_Varying_Model_2022a_B.OpenRocketX_CG;

  /* Math: '<S3>/Square1' */
  Time_Varying_Model_2022a_B.Square1 = Time_Varying_Model_2022a_B.Sum1_o *
    Time_Varying_Model_2022a_B.Sum1_o;

  /* Product: '<S3>/Product1' */
  Time_Varying_Model_2022a_B.Product1_g = Time_Varying_Model_2022a_B.CNa_body *
    Time_Varying_Model_2022a_B.Square1;

  /* Sum: '<S3>/Sum2' incorporates:
   *  Constant: '<Root>/Constant25'
   */
  Time_Varying_Model_2022a_B.Sum2_j =
    Time_Varying_Model_2022a_P.Constant25_Value -
    Time_Varying_Model_2022a_B.OpenRocketX_CG;

  /* Math: '<S3>/Square2' */
  Time_Varying_Model_2022a_B.Square2 = Time_Varying_Model_2022a_B.Sum2_j *
    Time_Varying_Model_2022a_B.Sum2_j;

  /* Product: '<S3>/Product2' */
  Time_Varying_Model_2022a_B.Product2_d = Time_Varying_Model_2022a_B.CNa_can *
    Time_Varying_Model_2022a_B.Square2;

  /* Sum: '<S3>/Sum3' incorporates:
   *  Constant: '<Root>/Constant24'
   */
  Time_Varying_Model_2022a_B.Sum3_m =
    Time_Varying_Model_2022a_P.Constant24_Value -
    Time_Varying_Model_2022a_B.OpenRocketX_CG;

  /* Math: '<S3>/Square3' */
  Time_Varying_Model_2022a_B.Square3 = Time_Varying_Model_2022a_B.Sum3_m *
    Time_Varying_Model_2022a_B.Sum3_m;

  /* Product: '<S3>/Product3' */
  Time_Varying_Model_2022a_B.Product3_m = Time_Varying_Model_2022a_B.CNa_fins *
    Time_Varying_Model_2022a_B.Square3;

  /* Sum: '<S3>/Sum of Elements' */
  Time_Varying_Model_2022a_B.SumofElements_g =
    ((Time_Varying_Model_2022a_B.Product_m +
      Time_Varying_Model_2022a_B.Product1_g) +
     Time_Varying_Model_2022a_B.Product2_d) +
    Time_Varying_Model_2022a_B.Product3_m;

  /* Product: '<S3>/Product4' incorporates:
   *  Constant: '<Root>/Constant6'
   *  Constant: '<S3>/Constant'
   */
  Time_Varying_Model_2022a_B.Product4_a =
    Time_Varying_Model_2022a_P.Constant_Value_fq *
    Time_Varying_Model_2022a_B.rho *
    Time_Varying_Model_2022a_B.OpenRocketVelocity *
    Time_Varying_Model_2022a_P.Constant6_Value *
    Time_Varying_Model_2022a_B.SumofElements_g;

  /* Sum: '<S3>/Sum4' incorporates:
   *  Constant: '<S3>/Constant1'
   */
  Time_Varying_Model_2022a_B.Sum4_m =
    Time_Varying_Model_2022a_P.Constant1_Value_o -
    Time_Varying_Model_2022a_B.OpenRocketX_CG;

  /* Math: '<S3>/Square4' */
  Time_Varying_Model_2022a_B.Square4 = Time_Varying_Model_2022a_B.Sum4_m *
    Time_Varying_Model_2022a_B.Sum4_m;

  /* Product: '<S3>/Product5' */
  Time_Varying_Model_2022a_B.Product5_l = Time_Varying_Model_2022a_B.Square4 *
    Time_Varying_Model_2022a_B.OpenRocketmDot;

  /* Sum: '<S3>/Sum5' */
  Time_Varying_Model_2022a_B.Sum5 = Time_Varying_Model_2022a_B.Product4_a +
    Time_Varying_Model_2022a_B.Product5_l;
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[3] == 0) {
    /* Sum: '<S6>/Sum of Elements' incorporates:
     *  Constant: '<Root>/Constant12'
     *  Constant: '<Root>/Constant13'
     */
    Time_Varying_Model_2022a_B.SumofElements_h =
      Time_Varying_Model_2022a_P.Constant12_Value +
      Time_Varying_Model_2022a_P.Constant13_Value;

    /* Product: '<S6>/Product' incorporates:
     *  Constant: '<Root>/Constant6'
     *  Constant: '<Root>/Constant7'
     *  Constant: '<Root>/Constant8'
     */
    Time_Varying_Model_2022a_B.Product_p5 = Time_Varying_Model_2022a_B.Gain3 *
      Time_Varying_Model_2022a_P.Constant6_Value *
      Time_Varying_Model_2022a_P.Constant7_Value *
      Time_Varying_Model_2022a_B.SumofElements_h *
      Time_Varying_Model_2022a_P.Constant8_Value;
  }

  /* Product: '<S7>/Product' incorporates:
   *  Constant: '<Root>/Constant10'
   *  Constant: '<Root>/Constant11'
   *  Constant: '<Root>/Constant6'
   *  Constant: '<Root>/Constant7'
   *  Constant: '<Root>/Constant9'
   *  Constant: '<S7>/Constant'
   */
  Time_Varying_Model_2022a_B.Product_ap =
    Time_Varying_Model_2022a_P.Constant_Value_k * Time_Varying_Model_2022a_B.rho
    * Time_Varying_Model_2022a_P.Constant6_Value *
    Time_Varying_Model_2022a_B.OpenRocketVelocity *
    Time_Varying_Model_2022a_P.Constant11_Value *
    Time_Varying_Model_2022a_P.Constant7_Value *
    Time_Varying_Model_2022a_P.Constant9_Value *
    Time_Varying_Model_2022a_P.Constant10_Value;

  /* Gain: '<S17>/Gain' */
  Time_Varying_Model_2022a_B.Gain_c = Time_Varying_Model_2022a_P.Gain_Gain_cc *
    Time_Varying_Model_2022a_B.MaxDeflection[0];

  /* Gain: '<S18>/Gain' */
  Time_Varying_Model_2022a_B.Gain_m = Time_Varying_Model_2022a_P.Gain_Gain_eq *
    Time_Varying_Model_2022a_B.MaxDeflection[1];

  /* Gain: '<S29>/Gain' */
  Time_Varying_Model_2022a_B.Gain_ie = Time_Varying_Model_2022a_P.Gain_Gain_cj *
    Time_Varying_Model_2022a_B.MaxDeflection[2];

  /* Gain: '<S34>/Gain' */
  Time_Varying_Model_2022a_B.Gain_ps = Time_Varying_Model_2022a_P.Gain_Gain_ho *
    Time_Varying_Model_2022a_B.MaxDeflection[3];
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
  }

  /* Gain: '<S35>/Gain' */
  Time_Varying_Model_2022a_B.Gain_h3 = Time_Varying_Model_2022a_P.Gain_Gain_kh *
    Time_Varying_Model_2022a_B.Integrator5;

  /* Gain: '<S36>/Gain' */
  Time_Varying_Model_2022a_B.Gain_mg = Time_Varying_Model_2022a_P.Gain_Gain_d3 *
    Time_Varying_Model_2022a_B.Integrator4;

  /* Gain: '<S37>/Gain' */
  Time_Varying_Model_2022a_B.Gain_jm = Time_Varying_Model_2022a_P.Gain_Gain_mv *
    Time_Varying_Model_2022a_B.Integrator3;
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
  }

  /* Gain: '<S38>/Gain' */
  Time_Varying_Model_2022a_B.Gain_j5 = Time_Varying_Model_2022a_P.Gain_Gain_di *
    Time_Varying_Model_2022a_B.Integrator2;

  /* Gain: '<S39>/Gain' */
  Time_Varying_Model_2022a_B.Gain_bv = Time_Varying_Model_2022a_P.Gain_Gain_g *
    Time_Varying_Model_2022a_B.Integrator;

  /* Gain: '<S40>/Gain' */
  Time_Varying_Model_2022a_B.Gain_d = Time_Varying_Model_2022a_P.Gain_Gain_g4 *
    Time_Varying_Model_2022a_B.Integrator1;
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
  }

  /* MATLAB Function: '<S16>/Pitch' incorporates:
   *  Constant: '<Root>/Constant28'
   *  Constant: '<Root>/Constant30'
   *  Constant: '<Root>/Constant31'
   */
  /* :  Mroll = -((I_R-I_L)/I_L)*p*r; */
  ang_cos = -((Time_Varying_Model_2022a_B.OpenRocketI_R -
               Time_Varying_Model_2022a_B.OpenRocketI_L) /
              Time_Varying_Model_2022a_B.OpenRocketI_L) *
    Time_Varying_Model_2022a_B.Integrator2 *
    Time_Varying_Model_2022a_B.Integrator1;

  /* :  Mcorr = -(C1/I_L)*alpha; */
  ang_sin = -(Time_Varying_Model_2022a_B.Product_o /
              Time_Varying_Model_2022a_B.OpenRocketI_L) *
    Time_Varying_Model_2022a_B.windAngles[0];

  /* :  Mcan = ((qBar*S*CLa*Xbar_CP_can)/I_L)*((alpha - x1)*cos(x1) + (alpha + x2)*cos(x2)); */
  st_idx_1 = Time_Varying_Model_2022a_B.Gain3 *
    Time_Varying_Model_2022a_P.Constant28_Value *
    Time_Varying_Model_2022a_P.Constant31_Value *
    Time_Varying_Model_2022a_P.Constant30_Value /
    Time_Varying_Model_2022a_B.OpenRocketI_L *
    ((Time_Varying_Model_2022a_B.windAngles[0] -
      Time_Varying_Model_2022a_B.MaxDeflection[0]) * cos
     (Time_Varying_Model_2022a_B.MaxDeflection[0]) +
     (Time_Varying_Model_2022a_B.windAngles[0] +
      Time_Varying_Model_2022a_B.MaxDeflection[1]) * cos
     (Time_Varying_Model_2022a_B.MaxDeflection[1]));

  /* :  Mdamp = -(C2/I_L)*q; */
  ct_idx_0 = -(Time_Varying_Model_2022a_B.Sum5 /
               Time_Varying_Model_2022a_B.OpenRocketI_L) *
    Time_Varying_Model_2022a_B.Integrator;

  /* :  qDot = Mroll + Mcorr + Mcan + Mdamp; */
  Time_Varying_Model_2022a_B.qDot = ((ang_cos + ang_sin) + st_idx_1) + ct_idx_0;
  Time_Varying_Model_2022a_B.Mroll = ang_cos;
  Time_Varying_Model_2022a_B.Mcorr = ang_sin;
  Time_Varying_Model_2022a_B.Mcan = st_idx_1;
  Time_Varying_Model_2022a_B.Mdamp = ct_idx_0;

  /* Sum: '<S16>/Sum of Elements' */
  Time_Varying_Model_2022a_B.SumofElements_f =
    ((Time_Varying_Model_2022a_B.Mroll + Time_Varying_Model_2022a_B.Mcorr) +
     Time_Varying_Model_2022a_B.Mcan) + Time_Varying_Model_2022a_B.Mdamp;
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
  }

  /* MATLAB Function: '<S16>/Yaw' incorporates:
   *  Constant: '<Root>/Constant28'
   *  Constant: '<Root>/Constant30'
   *  Constant: '<Root>/Constant31'
   */
  /* :  Nroll = -((I_L-I_R)/I_L)*p*q; */
  ang_cos = -((Time_Varying_Model_2022a_B.OpenRocketI_L -
               Time_Varying_Model_2022a_B.OpenRocketI_R) /
              Time_Varying_Model_2022a_B.OpenRocketI_L) *
    Time_Varying_Model_2022a_B.Integrator2 *
    Time_Varying_Model_2022a_B.Integrator;

  /* :  Ncorr = -(C1/I_L)*beta; */
  ang_sin = -(Time_Varying_Model_2022a_B.Product_o /
              Time_Varying_Model_2022a_B.OpenRocketI_L) *
    Time_Varying_Model_2022a_B.windAngles[1];

  /* :  Ndamp = -(C2/I_L)*r; */
  st_idx_1 = -(Time_Varying_Model_2022a_B.Sum5 /
               Time_Varying_Model_2022a_B.OpenRocketI_L) *
    Time_Varying_Model_2022a_B.Integrator1;

  /* :  Ncan = ((qBar*S*CLa*Xbar_CP_can)/I_L)*((beta - y1)*cos(y1) + (beta + y2)*cos(y2)); */
  ct_idx_0 = Time_Varying_Model_2022a_B.Gain3 *
    Time_Varying_Model_2022a_P.Constant28_Value *
    Time_Varying_Model_2022a_P.Constant31_Value *
    Time_Varying_Model_2022a_P.Constant30_Value /
    Time_Varying_Model_2022a_B.OpenRocketI_L *
    ((Time_Varying_Model_2022a_B.windAngles[1] -
      Time_Varying_Model_2022a_B.MaxDeflection[2]) * cos
     (Time_Varying_Model_2022a_B.MaxDeflection[2]) +
     (Time_Varying_Model_2022a_B.windAngles[1] +
      Time_Varying_Model_2022a_B.MaxDeflection[3]) * cos
     (Time_Varying_Model_2022a_B.MaxDeflection[3]));

  /* :  rDot = Nroll + Ncorr + Ndamp + Ncan; */
  Time_Varying_Model_2022a_B.rDot = ((ang_cos + ang_sin) + st_idx_1) + ct_idx_0;
  Time_Varying_Model_2022a_B.Nroll = ang_cos;
  Time_Varying_Model_2022a_B.Ncorr = ang_sin;
  Time_Varying_Model_2022a_B.Ndamp = st_idx_1;
  Time_Varying_Model_2022a_B.Ncan = ct_idx_0;

  /* Sum: '<S16>/Sum of Elements1' */
  Time_Varying_Model_2022a_B.SumofElements1_d =
    ((Time_Varying_Model_2022a_B.Nroll + Time_Varying_Model_2022a_B.Ncorr) +
     Time_Varying_Model_2022a_B.Ndamp) + Time_Varying_Model_2022a_B.Ncan;
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
  }

  /* MATLAB Function: '<S16>/Roll' incorporates:
   *  Constant: '<Root>/Constant27'
   *  Constant: '<Root>/Constant28'
   *  Constant: '<Root>/Constant31'
   */
  /* :  pDot = - (Cdp/I_R)*p + ((qBar*S*CLa*Z_CP_can)/I_R)*((alpha - x1)*cos(x1) - (alpha + x2)*cos(x2) + (beta - y1)*cos(y1) - (beta + y2)*cos(y2)); */
  Time_Varying_Model_2022a_B.pDot = ((((Time_Varying_Model_2022a_B.windAngles[0]
    - Time_Varying_Model_2022a_B.MaxDeflection[0]) * cos
    (Time_Varying_Model_2022a_B.MaxDeflection[0]) -
    (Time_Varying_Model_2022a_B.windAngles[0] +
     Time_Varying_Model_2022a_B.MaxDeflection[1]) * cos
    (Time_Varying_Model_2022a_B.MaxDeflection[1])) +
    (Time_Varying_Model_2022a_B.windAngles[1] -
     Time_Varying_Model_2022a_B.MaxDeflection[2]) * cos
    (Time_Varying_Model_2022a_B.MaxDeflection[2])) -
    (Time_Varying_Model_2022a_B.windAngles[1] +
     Time_Varying_Model_2022a_B.MaxDeflection[3]) * cos
    (Time_Varying_Model_2022a_B.MaxDeflection[3])) *
    (Time_Varying_Model_2022a_B.Gain3 *
     Time_Varying_Model_2022a_P.Constant28_Value *
     Time_Varying_Model_2022a_P.Constant31_Value *
     Time_Varying_Model_2022a_P.Constant27_Value /
     Time_Varying_Model_2022a_B.OpenRocketI_R) +
    -(Time_Varying_Model_2022a_B.Product_ap /
      Time_Varying_Model_2022a_B.OpenRocketI_R) *
    Time_Varying_Model_2022a_B.Integrator2;
  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
      Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
  }

  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M)) {
    /* Matfile logging */
    rt_UpdateTXYLogVars(Time_Varying_Model_2022a_M->rtwLogInfo,
                        (Time_Varying_Model_2022a_M->Timing.t));
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M)) {
    /* Update for RateLimiter: '<Root>/Motor Speed' */
    Time_Varying_Model_2022a_DW.PrevY[0] =
      Time_Varying_Model_2022a_B.MotorSpeed[0];
    Time_Varying_Model_2022a_DW.PrevY[1] =
      Time_Varying_Model_2022a_B.MotorSpeed[1];
    Time_Varying_Model_2022a_DW.PrevY[2] =
      Time_Varying_Model_2022a_B.MotorSpeed[2];
    Time_Varying_Model_2022a_DW.PrevY[3] =
      Time_Varying_Model_2022a_B.MotorSpeed[3];
    Time_Varying_Model_2022a_DW.LastMajorTime =
      Time_Varying_Model_2022a_M->Timing.t[0];
    if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
        Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[2] == 0) {
      /* Update for RandomNumber: '<S126>/White Noise' */
      Time_Varying_Model_2022a_DW.NextOutput[0] = rt_nrand_Upu32_Yd_f_pw_snf
        (&Time_Varying_Model_2022a_DW.RandSeed[0]) *
        Time_Varying_Model_2022a_P.WhiteNoise_StdDev +
        Time_Varying_Model_2022a_P.WhiteNoise_Mean;
      Time_Varying_Model_2022a_DW.NextOutput[1] = rt_nrand_Upu32_Yd_f_pw_snf
        (&Time_Varying_Model_2022a_DW.RandSeed[1]) *
        Time_Varying_Model_2022a_P.WhiteNoise_StdDev +
        Time_Varying_Model_2022a_P.WhiteNoise_Mean;
      Time_Varying_Model_2022a_DW.NextOutput[2] = rt_nrand_Upu32_Yd_f_pw_snf
        (&Time_Varying_Model_2022a_DW.RandSeed[2]) *
        Time_Varying_Model_2022a_P.WhiteNoise_StdDev +
        Time_Varying_Model_2022a_P.WhiteNoise_Mean;
      Time_Varying_Model_2022a_DW.NextOutput[3] = rt_nrand_Upu32_Yd_f_pw_snf
        (&Time_Varying_Model_2022a_DW.RandSeed[3]) *
        Time_Varying_Model_2022a_P.WhiteNoise_StdDev +
        Time_Varying_Model_2022a_P.WhiteNoise_Mean;
    }

    if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
        Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[1] == 0) {
      /* Update for RandomNumber: '<S54>/White Noise' */
      Time_Varying_Model_2022a_DW.NextOutput_p = rt_nrand_Upu32_Yd_f_pw_snf
        (&Time_Varying_Model_2022a_DW.RandSeed_e) *
        Time_Varying_Model_2022a_P.WhiteNoise_StdDev_a +
        Time_Varying_Model_2022a_P.WhiteNoise_Mean_m;

      /* Update for RandomNumber: '<S53>/White Noise' */
      Time_Varying_Model_2022a_DW.NextOutput_j = rt_nrand_Upu32_Yd_f_pw_snf
        (&Time_Varying_Model_2022a_DW.RandSeed_h) *
        Time_Varying_Model_2022a_P.WhiteNoise_StdDev_o +
        Time_Varying_Model_2022a_P.WhiteNoise_Mean_l;

      /* Update for RandomNumber: '<S52>/White Noise' */
      Time_Varying_Model_2022a_DW.NextOutput_d = rt_nrand_Upu32_Yd_f_pw_snf
        (&Time_Varying_Model_2022a_DW.RandSeed_m) *
        Time_Varying_Model_2022a_P.WhiteNoise_StdDev_m +
        Time_Varying_Model_2022a_P.WhiteNoise_Mean_c;
    }

    if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M) &&
        Time_Varying_Model_2022a_M->Timing.TaskCounters.TID[3] == 0) {
      /* Update for Delay: '<S55>/MemoryX' */
      Time_Varying_Model_2022a_DW.icLoad = false;
      Time_Varying_Model_2022a_DW.MemoryX_DSTATE =
        Time_Varying_Model_2022a_B.Add_p;
    }
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(Time_Varying_Model_2022a_M)) {
    /* signal main to stop simulation */
    {                                  /* Sample time: [0.0s, 0.0s] */
      if ((rtmGetTFinal(Time_Varying_Model_2022a_M)!=-1) &&
          !((rtmGetTFinal(Time_Varying_Model_2022a_M)-
             (((Time_Varying_Model_2022a_M->Timing.clockTick1+
                Time_Varying_Model_2022a_M->Timing.clockTickH1* 4294967296.0)) *
              0.01)) > (((Time_Varying_Model_2022a_M->Timing.clockTick1+
                          Time_Varying_Model_2022a_M->Timing.clockTickH1*
                          4294967296.0)) * 0.01) * (DBL_EPSILON))) {
        rtmSetErrorStatus(Time_Varying_Model_2022a_M, "Simulation finished");
      }
    }

    rt_ertODEUpdateContinuousStates(&Time_Varying_Model_2022a_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick0 and the high bits
     * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++Time_Varying_Model_2022a_M->Timing.clockTick0)) {
      ++Time_Varying_Model_2022a_M->Timing.clockTickH0;
    }

    Time_Varying_Model_2022a_M->Timing.t[0] = rtsiGetSolverStopTime
      (&Time_Varying_Model_2022a_M->solverInfo);

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
      Time_Varying_Model_2022a_M->Timing.clockTick1++;
      if (!Time_Varying_Model_2022a_M->Timing.clockTick1) {
        Time_Varying_Model_2022a_M->Timing.clockTickH1++;
      }
    }

    rate_scheduler();
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void Time_Varying_Model_2022a_derivatives(void)
{
  XDot_Time_Varying_Model_2022a_T *_rtXdot;
  _rtXdot = ((XDot_Time_Varying_Model_2022a_T *)
             Time_Varying_Model_2022a_M->derivs);

  /* Derivatives for Integrator: '<S16>/Integrator4' */
  _rtXdot->Integrator4_CSTATE = Time_Varying_Model_2022a_B.Integrator;

  /* Derivatives for Integrator: '<S16>/Integrator5' */
  _rtXdot->Integrator5_CSTATE = Time_Varying_Model_2022a_B.Integrator2;

  /* Derivatives for Integrator: '<S16>/Integrator3' */
  _rtXdot->Integrator3_CSTATE = Time_Varying_Model_2022a_B.Integrator1;

  /* Derivatives for Integrator: '<S16>/Integrator2' */
  _rtXdot->Integrator2_CSTATE = Time_Varying_Model_2022a_B.pDot;

  /* Derivatives for Integrator: '<S16>/Integrator' */
  _rtXdot->Integrator_CSTATE = Time_Varying_Model_2022a_B.qDot;

  /* Derivatives for Integrator: '<S16>/Integrator1' */
  _rtXdot->Integrator1_CSTATE = Time_Varying_Model_2022a_B.rDot;

  /* Derivatives for Enabled SubSystem: '<S117>/Hugw(s)' */
  if (Time_Varying_Model_2022a_DW.Hugws_MODE) {
    /* Derivatives for Integrator: '<S130>/ug_p' */
    _rtXdot->ug_p_CSTATE[0] = Time_Varying_Model_2022a_B.w_b[0];
    _rtXdot->ug_p_CSTATE[1] = Time_Varying_Model_2022a_B.w_b[1];
  } else {
    {
      real_T *dx;
      int_T i;
      dx = &(((XDot_Time_Varying_Model_2022a_T *)
              Time_Varying_Model_2022a_M->derivs)->ug_p_CSTATE[0]);
      for (i=0; i < 2; i++) {
        dx[i] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S117>/Hugw(s)' */

  /* Derivatives for Enabled SubSystem: '<S117>/Hvgw(s)' */
  if (Time_Varying_Model_2022a_DW.Hvgws_MODE) {
    /* Derivatives for Integrator: '<S131>/vg_p1' */
    _rtXdot->vg_p1_CSTATE[0] = Time_Varying_Model_2022a_B.w_l[0];

    /* Derivatives for Integrator: '<S131>/vgw_p2' */
    _rtXdot->vgw_p2_CSTATE[0] = Time_Varying_Model_2022a_B.w_i[0];

    /* Derivatives for Integrator: '<S131>/vg_p1' */
    _rtXdot->vg_p1_CSTATE[1] = Time_Varying_Model_2022a_B.w_l[1];

    /* Derivatives for Integrator: '<S131>/vgw_p2' */
    _rtXdot->vgw_p2_CSTATE[1] = Time_Varying_Model_2022a_B.w_i[1];
  } else {
    {
      real_T *dx;
      int_T i;
      dx = &(((XDot_Time_Varying_Model_2022a_T *)
              Time_Varying_Model_2022a_M->derivs)->vg_p1_CSTATE[0]);
      for (i=0; i < 4; i++) {
        dx[i] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S117>/Hvgw(s)' */

  /* Derivatives for Enabled SubSystem: '<S117>/Hwgw(s)' */
  if (Time_Varying_Model_2022a_DW.Hwgws_MODE) {
    /* Derivatives for Integrator: '<S132>/wg_p1' */
    _rtXdot->wg_p1_CSTATE[0] = Time_Varying_Model_2022a_B.w[0];

    /* Derivatives for Integrator: '<S132>/wg_p2' */
    _rtXdot->wg_p2_CSTATE[0] = Time_Varying_Model_2022a_B.w_o[0];

    /* Derivatives for Integrator: '<S132>/wg_p1' */
    _rtXdot->wg_p1_CSTATE[1] = Time_Varying_Model_2022a_B.w[1];

    /* Derivatives for Integrator: '<S132>/wg_p2' */
    _rtXdot->wg_p2_CSTATE[1] = Time_Varying_Model_2022a_B.w_o[1];
  } else {
    {
      real_T *dx;
      int_T i;
      dx = &(((XDot_Time_Varying_Model_2022a_T *)
              Time_Varying_Model_2022a_M->derivs)->wg_p1_CSTATE[0]);
      for (i=0; i < 4; i++) {
        dx[i] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S117>/Hwgw(s)' */

  /* Derivatives for Integrator: '<S41>/Integrator' */
  _rtXdot->Integrator_CSTATE_m[0] = Time_Varying_Model_2022a_B.Merge_n[0];
  _rtXdot->Integrator_CSTATE_m[1] = Time_Varying_Model_2022a_B.Merge_n[1];
  _rtXdot->Integrator_CSTATE_m[2] = Time_Varying_Model_2022a_B.Merge_n[2];

  /* Derivatives for Enabled SubSystem: '<S116>/Hpgw' */
  if (Time_Varying_Model_2022a_DW.Hpgw_MODE) {
    /* Derivatives for Integrator: '<S127>/pgw_p' */
    _rtXdot->pgw_p_CSTATE[0] = Time_Varying_Model_2022a_B.w_m[0];
    _rtXdot->pgw_p_CSTATE[1] = Time_Varying_Model_2022a_B.w_m[1];
  } else {
    {
      real_T *dx;
      int_T i;
      dx = &(((XDot_Time_Varying_Model_2022a_T *)
              Time_Varying_Model_2022a_M->derivs)->pgw_p_CSTATE[0]);
      for (i=0; i < 2; i++) {
        dx[i] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S116>/Hpgw' */

  /* Derivatives for Enabled SubSystem: '<S116>/Hqgw' */
  if (Time_Varying_Model_2022a_DW.Hqgw_MODE) {
    /* Derivatives for Integrator: '<S128>/qgw_p' */
    _rtXdot->qgw_p_CSTATE[0] = Time_Varying_Model_2022a_B.w_gw[0];
    _rtXdot->qgw_p_CSTATE[1] = Time_Varying_Model_2022a_B.w_gw[1];
  } else {
    {
      real_T *dx;
      int_T i;
      dx = &(((XDot_Time_Varying_Model_2022a_T *)
              Time_Varying_Model_2022a_M->derivs)->qgw_p_CSTATE[0]);
      for (i=0; i < 2; i++) {
        dx[i] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S116>/Hqgw' */

  /* Derivatives for Enabled SubSystem: '<S116>/Hrgw' */
  if (Time_Varying_Model_2022a_DW.Hrgw_MODE) {
    /* Derivatives for Integrator: '<S129>/rgw_p' */
    _rtXdot->rgw_p_CSTATE[0] = Time_Varying_Model_2022a_B.w_g[0];
    _rtXdot->rgw_p_CSTATE[1] = Time_Varying_Model_2022a_B.w_g[1];
  } else {
    {
      real_T *dx;
      int_T i;
      dx = &(((XDot_Time_Varying_Model_2022a_T *)
              Time_Varying_Model_2022a_M->derivs)->rgw_p_CSTATE[0]);
      for (i=0; i < 2; i++) {
        dx[i] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S116>/Hrgw' */
}

/* Model initialize function */
void Time_Varying_Model_2022a_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)Time_Varying_Model_2022a_M, 0,
                sizeof(RT_MODEL_Time_Varying_Model_2_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&Time_Varying_Model_2022a_M->solverInfo,
                          &Time_Varying_Model_2022a_M->Timing.simTimeStep);
    rtsiSetTPtr(&Time_Varying_Model_2022a_M->solverInfo, &rtmGetTPtr
                (Time_Varying_Model_2022a_M));
    rtsiSetStepSizePtr(&Time_Varying_Model_2022a_M->solverInfo,
                       &Time_Varying_Model_2022a_M->Timing.stepSize0);
    rtsiSetdXPtr(&Time_Varying_Model_2022a_M->solverInfo,
                 &Time_Varying_Model_2022a_M->derivs);
    rtsiSetContStatesPtr(&Time_Varying_Model_2022a_M->solverInfo, (real_T **)
                         &Time_Varying_Model_2022a_M->contStates);
    rtsiSetNumContStatesPtr(&Time_Varying_Model_2022a_M->solverInfo,
      &Time_Varying_Model_2022a_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&Time_Varying_Model_2022a_M->solverInfo,
      &Time_Varying_Model_2022a_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&Time_Varying_Model_2022a_M->solverInfo,
      &Time_Varying_Model_2022a_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&Time_Varying_Model_2022a_M->solverInfo,
      &Time_Varying_Model_2022a_M->periodicContStateRanges);
    rtsiSetContStateDisabledPtr(&Time_Varying_Model_2022a_M->solverInfo,
      (boolean_T**) &Time_Varying_Model_2022a_M->contStateDisabled);
    rtsiSetErrorStatusPtr(&Time_Varying_Model_2022a_M->solverInfo,
                          (&rtmGetErrorStatus(Time_Varying_Model_2022a_M)));
    rtsiSetRTModelPtr(&Time_Varying_Model_2022a_M->solverInfo,
                      Time_Varying_Model_2022a_M);
  }

  rtsiSetSimTimeStep(&Time_Varying_Model_2022a_M->solverInfo, MAJOR_TIME_STEP);
  Time_Varying_Model_2022a_M->intgData.y = Time_Varying_Model_2022a_M->odeY;
  Time_Varying_Model_2022a_M->intgData.f[0] = Time_Varying_Model_2022a_M->odeF[0];
  Time_Varying_Model_2022a_M->intgData.f[1] = Time_Varying_Model_2022a_M->odeF[1];
  Time_Varying_Model_2022a_M->intgData.f[2] = Time_Varying_Model_2022a_M->odeF[2];
  Time_Varying_Model_2022a_M->contStates = ((X_Time_Varying_Model_2022a_T *)
    &Time_Varying_Model_2022a_X);
  Time_Varying_Model_2022a_M->contStateDisabled =
    ((XDis_Time_Varying_Model_2022a_T *) &Time_Varying_Model_2022a_XDis);
  Time_Varying_Model_2022a_M->Timing.tStart = (0.0);
  rtsiSetSolverData(&Time_Varying_Model_2022a_M->solverInfo, (void *)
                    &Time_Varying_Model_2022a_M->intgData);
  rtsiSetIsMinorTimeStepWithModeChange(&Time_Varying_Model_2022a_M->solverInfo,
    false);
  rtsiSetSolverName(&Time_Varying_Model_2022a_M->solverInfo,"ode3");
  rtmSetTPtr(Time_Varying_Model_2022a_M,
             &Time_Varying_Model_2022a_M->Timing.tArray[0]);
  rtmSetTFinal(Time_Varying_Model_2022a_M, 12.0);
  Time_Varying_Model_2022a_M->Timing.stepSize0 = 0.01;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (NULL);
    Time_Varying_Model_2022a_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(Time_Varying_Model_2022a_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(Time_Varying_Model_2022a_M->rtwLogInfo, (NULL));
    rtliSetLogT(Time_Varying_Model_2022a_M->rtwLogInfo, "tout");
    rtliSetLogX(Time_Varying_Model_2022a_M->rtwLogInfo, "");
    rtliSetLogXFinal(Time_Varying_Model_2022a_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(Time_Varying_Model_2022a_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(Time_Varying_Model_2022a_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(Time_Varying_Model_2022a_M->rtwLogInfo, 0);
    rtliSetLogDecimation(Time_Varying_Model_2022a_M->rtwLogInfo, 1);
    rtliSetLogY(Time_Varying_Model_2022a_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(Time_Varying_Model_2022a_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(Time_Varying_Model_2022a_M->rtwLogInfo, (NULL));
  }

  /* block I/O */
  (void) memset(((void *) &Time_Varying_Model_2022a_B), 0,
                sizeof(B_Time_Varying_Model_2022a_T));

  /* states (continuous) */
  {
    (void) memset((void *)&Time_Varying_Model_2022a_X, 0,
                  sizeof(X_Time_Varying_Model_2022a_T));
  }

  /* disabled states */
  {
    (void) memset((void *)&Time_Varying_Model_2022a_XDis, 0,
                  sizeof(XDis_Time_Varying_Model_2022a_T));
  }

  /* states (dwork) */
  (void) memset((void *)&Time_Varying_Model_2022a_DW, 0,
                sizeof(DW_Time_Varying_Model_2022a_T));

  /* external outputs */
  (void)memset(&Time_Varying_Model_2022a_Y, 0, sizeof
               (ExtY_Time_Varying_Model_2022a_T));

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(Time_Varying_Model_2022a_M->rtwLogInfo, 0.0,
    rtmGetTFinal(Time_Varying_Model_2022a_M),
    Time_Varying_Model_2022a_M->Timing.stepSize0, (&rtmGetErrorStatus
    (Time_Varying_Model_2022a_M)));

  /* Start for If: '<S122>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */
  Time_Varying_Model_2022a_DW.ifHeightMaxlowaltitudeelseifHei = -1;

  /* Start for If: '<S121>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */
  Time_Varying_Model_2022a_DW.ifHeightMaxlowaltitudeelseifH_k = -1;

  {
    real_T u0;
    int32_T r;
    int32_T t;
    uint32_T tseed;

    /* InitializeConditions for Integrator: '<S16>/Integrator4' */
    Time_Varying_Model_2022a_X.Integrator4_CSTATE =
      Time_Varying_Model_2022a_P.Integrator4_IC;

    /* InitializeConditions for Integrator: '<S16>/Integrator5' */
    Time_Varying_Model_2022a_X.Integrator5_CSTATE =
      Time_Varying_Model_2022a_P.Integrator5_IC;

    /* InitializeConditions for Integrator: '<S16>/Integrator3' */
    Time_Varying_Model_2022a_X.Integrator3_CSTATE =
      Time_Varying_Model_2022a_P.Integrator3_IC;

    /* InitializeConditions for Integrator: '<S16>/Integrator2' */
    Time_Varying_Model_2022a_X.Integrator2_CSTATE =
      Time_Varying_Model_2022a_P.Integrator2_IC;

    /* InitializeConditions for Integrator: '<S16>/Integrator' */
    Time_Varying_Model_2022a_X.Integrator_CSTATE =
      Time_Varying_Model_2022a_P.Integrator_IC;

    /* InitializeConditions for Integrator: '<S16>/Integrator1' */
    Time_Varying_Model_2022a_X.Integrator1_CSTATE =
      Time_Varying_Model_2022a_P.Integrator1_IC;

    /* InitializeConditions for RateLimiter: '<Root>/Motor Speed' */
    Time_Varying_Model_2022a_DW.LastMajorTime = (rtInf);

    /* InitializeConditions for RandomNumber: '<S126>/White Noise' */
    u0 = Time_Varying_Model_2022a_P.DrydenWindTurbulenceModelCont_a[0];
    u0 = floor(u0);
    if (rtIsNaN(u0) || rtIsInf(u0)) {
      u0 = 0.0;
    } else {
      u0 = fmod(u0, 4.294967296E+9);
    }

    if (u0 < 0.0) {
      tseed = (uint32_T)-(int32_T)(uint32_T)-u0;
    } else {
      tseed = (uint32_T)u0;
    }

    r = (int32_T)(tseed >> 16U);
    t = (int32_T)(tseed & 32768U);
    tseed = ((((tseed - ((uint32_T)r << 16U)) + (uint32_T)t) << 16U) + (uint32_T)
             t) + (uint32_T)r;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else if (tseed > 2147483646U) {
      tseed = 2147483646U;
    }

    u0 = rt_nrand_Upu32_Yd_f_pw_snf(&tseed) *
      Time_Varying_Model_2022a_P.WhiteNoise_StdDev +
      Time_Varying_Model_2022a_P.WhiteNoise_Mean;
    Time_Varying_Model_2022a_DW.NextOutput[0] = u0;
    Time_Varying_Model_2022a_DW.RandSeed[0] = tseed;
    u0 = Time_Varying_Model_2022a_P.DrydenWindTurbulenceModelCont_a[1];
    u0 = floor(u0);
    if (rtIsNaN(u0) || rtIsInf(u0)) {
      u0 = 0.0;
    } else {
      u0 = fmod(u0, 4.294967296E+9);
    }

    if (u0 < 0.0) {
      tseed = (uint32_T)-(int32_T)(uint32_T)-u0;
    } else {
      tseed = (uint32_T)u0;
    }

    r = (int32_T)(tseed >> 16U);
    t = (int32_T)(tseed & 32768U);
    tseed = ((((tseed - ((uint32_T)r << 16U)) + (uint32_T)t) << 16U) + (uint32_T)
             t) + (uint32_T)r;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else if (tseed > 2147483646U) {
      tseed = 2147483646U;
    }

    u0 = rt_nrand_Upu32_Yd_f_pw_snf(&tseed) *
      Time_Varying_Model_2022a_P.WhiteNoise_StdDev +
      Time_Varying_Model_2022a_P.WhiteNoise_Mean;
    Time_Varying_Model_2022a_DW.NextOutput[1] = u0;
    Time_Varying_Model_2022a_DW.RandSeed[1] = tseed;
    u0 = Time_Varying_Model_2022a_P.DrydenWindTurbulenceModelCont_a[2];
    u0 = floor(u0);
    if (rtIsNaN(u0) || rtIsInf(u0)) {
      u0 = 0.0;
    } else {
      u0 = fmod(u0, 4.294967296E+9);
    }

    if (u0 < 0.0) {
      tseed = (uint32_T)-(int32_T)(uint32_T)-u0;
    } else {
      tseed = (uint32_T)u0;
    }

    r = (int32_T)(tseed >> 16U);
    t = (int32_T)(tseed & 32768U);
    tseed = ((((tseed - ((uint32_T)r << 16U)) + (uint32_T)t) << 16U) + (uint32_T)
             t) + (uint32_T)r;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else if (tseed > 2147483646U) {
      tseed = 2147483646U;
    }

    u0 = rt_nrand_Upu32_Yd_f_pw_snf(&tseed) *
      Time_Varying_Model_2022a_P.WhiteNoise_StdDev +
      Time_Varying_Model_2022a_P.WhiteNoise_Mean;
    Time_Varying_Model_2022a_DW.NextOutput[2] = u0;
    Time_Varying_Model_2022a_DW.RandSeed[2] = tseed;
    u0 = Time_Varying_Model_2022a_P.DrydenWindTurbulenceModelCont_a[3];
    u0 = floor(u0);
    if (rtIsNaN(u0) || rtIsInf(u0)) {
      u0 = 0.0;
    } else {
      u0 = fmod(u0, 4.294967296E+9);
    }

    if (u0 < 0.0) {
      tseed = (uint32_T)-(int32_T)(uint32_T)-u0;
    } else {
      tseed = (uint32_T)u0;
    }

    r = (int32_T)(tseed >> 16U);
    t = (int32_T)(tseed & 32768U);
    tseed = ((((tseed - ((uint32_T)r << 16U)) + (uint32_T)t) << 16U) + (uint32_T)
             t) + (uint32_T)r;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else if (tseed > 2147483646U) {
      tseed = 2147483646U;
    }

    u0 = rt_nrand_Upu32_Yd_f_pw_snf(&tseed) *
      Time_Varying_Model_2022a_P.WhiteNoise_StdDev +
      Time_Varying_Model_2022a_P.WhiteNoise_Mean;
    Time_Varying_Model_2022a_DW.NextOutput[3] = u0;
    Time_Varying_Model_2022a_DW.RandSeed[3] = tseed;

    /* End of InitializeConditions for RandomNumber: '<S126>/White Noise' */

    /* InitializeConditions for Integrator: '<S41>/Integrator' */
    Time_Varying_Model_2022a_X.Integrator_CSTATE_m[0] =
      Time_Varying_Model_2022a_P.Integrator_IC_d;
    Time_Varying_Model_2022a_X.Integrator_CSTATE_m[1] =
      Time_Varying_Model_2022a_P.Integrator_IC_d;
    Time_Varying_Model_2022a_X.Integrator_CSTATE_m[2] =
      Time_Varying_Model_2022a_P.Integrator_IC_d;

    /* InitializeConditions for RandomNumber: '<S54>/White Noise' */
    u0 = floor(Time_Varying_Model_2022a_P.BandLimitedWhiteNoise2_seed);
    if (rtIsNaN(u0) || rtIsInf(u0)) {
      u0 = 0.0;
    } else {
      u0 = fmod(u0, 4.294967296E+9);
    }

    if (u0 < 0.0) {
      tseed = (uint32_T)-(int32_T)(uint32_T)-u0;
    } else {
      tseed = (uint32_T)u0;
    }

    r = (int32_T)(tseed >> 16U);
    t = (int32_T)(tseed & 32768U);
    tseed = ((((tseed - ((uint32_T)r << 16U)) + (uint32_T)t) << 16U) + (uint32_T)
             t) + (uint32_T)r;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else if (tseed > 2147483646U) {
      tseed = 2147483646U;
    }

    Time_Varying_Model_2022a_DW.RandSeed_e = tseed;
    Time_Varying_Model_2022a_DW.NextOutput_p = rt_nrand_Upu32_Yd_f_pw_snf
      (&Time_Varying_Model_2022a_DW.RandSeed_e) *
      Time_Varying_Model_2022a_P.WhiteNoise_StdDev_a +
      Time_Varying_Model_2022a_P.WhiteNoise_Mean_m;

    /* End of InitializeConditions for RandomNumber: '<S54>/White Noise' */

    /* InitializeConditions for RandomNumber: '<S53>/White Noise' */
    u0 = floor(Time_Varying_Model_2022a_P.BandLimitedWhiteNoise1_seed);
    if (rtIsNaN(u0) || rtIsInf(u0)) {
      u0 = 0.0;
    } else {
      u0 = fmod(u0, 4.294967296E+9);
    }

    if (u0 < 0.0) {
      tseed = (uint32_T)-(int32_T)(uint32_T)-u0;
    } else {
      tseed = (uint32_T)u0;
    }

    r = (int32_T)(tseed >> 16U);
    t = (int32_T)(tseed & 32768U);
    tseed = ((((tseed - ((uint32_T)r << 16U)) + (uint32_T)t) << 16U) + (uint32_T)
             t) + (uint32_T)r;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else if (tseed > 2147483646U) {
      tseed = 2147483646U;
    }

    Time_Varying_Model_2022a_DW.RandSeed_h = tseed;
    Time_Varying_Model_2022a_DW.NextOutput_j = rt_nrand_Upu32_Yd_f_pw_snf
      (&Time_Varying_Model_2022a_DW.RandSeed_h) *
      Time_Varying_Model_2022a_P.WhiteNoise_StdDev_o +
      Time_Varying_Model_2022a_P.WhiteNoise_Mean_l;

    /* End of InitializeConditions for RandomNumber: '<S53>/White Noise' */

    /* InitializeConditions for RandomNumber: '<S52>/White Noise' */
    u0 = floor(Time_Varying_Model_2022a_P.BandLimitedWhiteNoise_seed);
    if (rtIsNaN(u0) || rtIsInf(u0)) {
      u0 = 0.0;
    } else {
      u0 = fmod(u0, 4.294967296E+9);
    }

    if (u0 < 0.0) {
      tseed = (uint32_T)-(int32_T)(uint32_T)-u0;
    } else {
      tseed = (uint32_T)u0;
    }

    r = (int32_T)(tseed >> 16U);
    t = (int32_T)(tseed & 32768U);
    tseed = ((((tseed - ((uint32_T)r << 16U)) + (uint32_T)t) << 16U) + (uint32_T)
             t) + (uint32_T)r;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else if (tseed > 2147483646U) {
      tseed = 2147483646U;
    }

    Time_Varying_Model_2022a_DW.RandSeed_m = tseed;
    Time_Varying_Model_2022a_DW.NextOutput_d = rt_nrand_Upu32_Yd_f_pw_snf
      (&Time_Varying_Model_2022a_DW.RandSeed_m) *
      Time_Varying_Model_2022a_P.WhiteNoise_StdDev_m +
      Time_Varying_Model_2022a_P.WhiteNoise_Mean_c;

    /* End of InitializeConditions for RandomNumber: '<S52>/White Noise' */

    /* InitializeConditions for Delay: '<S55>/MemoryX' */
    Time_Varying_Model_2022a_DW.icLoad = true;

    /* SystemInitialize for Enabled SubSystem: '<S83>/Enabled Subsystem' */
    /* SystemInitialize for Product: '<S109>/Product2' incorporates:
     *  Outport: '<S109>/deltax'
     */
    Time_Varying_Model_2022a_B.Product2_cw =
      Time_Varying_Model_2022a_P.deltax_Y0;

    /* End of SystemInitialize for SubSystem: '<S83>/Enabled Subsystem' */

    /* SystemInitialize for Enabled SubSystem: '<S76>/MeasurementUpdate' */
    /* SystemInitialize for Product: '<S107>/Product3' incorporates:
     *  Outport: '<S107>/L*(y[k]-yhat[k|k-1])'
     */
    Time_Varying_Model_2022a_B.Product3_lw =
      Time_Varying_Model_2022a_P.Lykyhatkk1_Y0;

    /* End of SystemInitialize for SubSystem: '<S76>/MeasurementUpdate' */

    /* SystemInitialize for Enabled SubSystem: '<S117>/Hugw(s)' */
    /* InitializeConditions for Integrator: '<S130>/ug_p' */
    Time_Varying_Model_2022a_X.ug_p_CSTATE[0] =
      Time_Varying_Model_2022a_P.ug_p_IC;

    /* SystemInitialize for Product: '<S130>/w1' incorporates:
     *  Outport: '<S130>/ugw'
     */
    Time_Varying_Model_2022a_B.w1_a[0] = Time_Varying_Model_2022a_P.ugw_Y0;

    /* End of SystemInitialize for SubSystem: '<S117>/Hugw(s)' */

    /* SystemInitialize for Enabled SubSystem: '<S117>/Hvgw(s)' */
    /* InitializeConditions for Integrator: '<S131>/vg_p1' */
    Time_Varying_Model_2022a_X.vg_p1_CSTATE[0] =
      Time_Varying_Model_2022a_P.vg_p1_IC;

    /* InitializeConditions for Integrator: '<S131>/vgw_p2' */
    Time_Varying_Model_2022a_X.vgw_p2_CSTATE[0] =
      Time_Varying_Model_2022a_P.vgw_p2_IC;

    /* SystemInitialize for Product: '<S131>/w 1' incorporates:
     *  Outport: '<S131>/vgw'
     */
    Time_Varying_Model_2022a_B.w1[0] = Time_Varying_Model_2022a_P.vgw_Y0;

    /* End of SystemInitialize for SubSystem: '<S117>/Hvgw(s)' */

    /* SystemInitialize for Enabled SubSystem: '<S117>/Hwgw(s)' */
    /* InitializeConditions for Integrator: '<S132>/wg_p1' */
    Time_Varying_Model_2022a_X.wg_p1_CSTATE[0] =
      Time_Varying_Model_2022a_P.wg_p1_IC;

    /* InitializeConditions for Integrator: '<S132>/wg_p2' */
    Time_Varying_Model_2022a_X.wg_p2_CSTATE[0] =
      Time_Varying_Model_2022a_P.wg_p2_IC;

    /* SystemInitialize for Product: '<S132>/Lwg//V 1' incorporates:
     *  Outport: '<S132>/wgw'
     */
    Time_Varying_Model_2022a_B.LwgV1[0] = Time_Varying_Model_2022a_P.wgw_Y0;

    /* End of SystemInitialize for SubSystem: '<S117>/Hwgw(s)' */

    /* SystemInitialize for Enabled SubSystem: '<S116>/Hpgw' */
    /* InitializeConditions for Integrator: '<S127>/pgw_p' */
    Time_Varying_Model_2022a_X.pgw_p_CSTATE[0] =
      Time_Varying_Model_2022a_P.pgw_p_IC;

    /* SystemInitialize for Product: '<S127>/sigma_w' incorporates:
     *  Outport: '<S127>/pgw'
     */
    Time_Varying_Model_2022a_B.sigma_w[0] = Time_Varying_Model_2022a_P.pgw_Y0;

    /* End of SystemInitialize for SubSystem: '<S116>/Hpgw' */

    /* SystemInitialize for Enabled SubSystem: '<S116>/Hqgw' */
    /* InitializeConditions for Integrator: '<S128>/qgw_p' */
    Time_Varying_Model_2022a_X.qgw_p_CSTATE[0] =
      Time_Varying_Model_2022a_P.qgw_p_IC;

    /* SystemInitialize for Product: '<S128>/w' incorporates:
     *  Outport: '<S128>/qgw'
     */
    Time_Varying_Model_2022a_B.w_gw[0] = Time_Varying_Model_2022a_P.qgw_Y0;

    /* End of SystemInitialize for SubSystem: '<S116>/Hqgw' */

    /* SystemInitialize for Enabled SubSystem: '<S116>/Hrgw' */
    /* InitializeConditions for Integrator: '<S129>/rgw_p' */
    Time_Varying_Model_2022a_X.rgw_p_CSTATE[0] =
      Time_Varying_Model_2022a_P.rgw_p_IC;

    /* SystemInitialize for Product: '<S129>/w' incorporates:
     *  Outport: '<S129>/rgw'
     */
    Time_Varying_Model_2022a_B.w_g[0] = Time_Varying_Model_2022a_P.rgw_Y0;

    /* End of SystemInitialize for SubSystem: '<S116>/Hrgw' */

    /* SystemInitialize for Enabled SubSystem: '<S117>/Hugw(s)' */
    /* InitializeConditions for Integrator: '<S130>/ug_p' */
    Time_Varying_Model_2022a_X.ug_p_CSTATE[1] =
      Time_Varying_Model_2022a_P.ug_p_IC;

    /* SystemInitialize for Product: '<S130>/w1' incorporates:
     *  Outport: '<S130>/ugw'
     */
    Time_Varying_Model_2022a_B.w1_a[1] = Time_Varying_Model_2022a_P.ugw_Y0;

    /* End of SystemInitialize for SubSystem: '<S117>/Hugw(s)' */

    /* SystemInitialize for Enabled SubSystem: '<S117>/Hvgw(s)' */
    /* InitializeConditions for Integrator: '<S131>/vg_p1' */
    Time_Varying_Model_2022a_X.vg_p1_CSTATE[1] =
      Time_Varying_Model_2022a_P.vg_p1_IC;

    /* InitializeConditions for Integrator: '<S131>/vgw_p2' */
    Time_Varying_Model_2022a_X.vgw_p2_CSTATE[1] =
      Time_Varying_Model_2022a_P.vgw_p2_IC;

    /* SystemInitialize for Product: '<S131>/w 1' incorporates:
     *  Outport: '<S131>/vgw'
     */
    Time_Varying_Model_2022a_B.w1[1] = Time_Varying_Model_2022a_P.vgw_Y0;

    /* End of SystemInitialize for SubSystem: '<S117>/Hvgw(s)' */

    /* SystemInitialize for Enabled SubSystem: '<S117>/Hwgw(s)' */
    /* InitializeConditions for Integrator: '<S132>/wg_p1' */
    Time_Varying_Model_2022a_X.wg_p1_CSTATE[1] =
      Time_Varying_Model_2022a_P.wg_p1_IC;

    /* InitializeConditions for Integrator: '<S132>/wg_p2' */
    Time_Varying_Model_2022a_X.wg_p2_CSTATE[1] =
      Time_Varying_Model_2022a_P.wg_p2_IC;

    /* SystemInitialize for Product: '<S132>/Lwg//V 1' incorporates:
     *  Outport: '<S132>/wgw'
     */
    Time_Varying_Model_2022a_B.LwgV1[1] = Time_Varying_Model_2022a_P.wgw_Y0;

    /* End of SystemInitialize for SubSystem: '<S117>/Hwgw(s)' */

    /* SystemInitialize for Enabled SubSystem: '<S116>/Hpgw' */
    /* InitializeConditions for Integrator: '<S127>/pgw_p' */
    Time_Varying_Model_2022a_X.pgw_p_CSTATE[1] =
      Time_Varying_Model_2022a_P.pgw_p_IC;

    /* SystemInitialize for Product: '<S127>/sigma_w' incorporates:
     *  Outport: '<S127>/pgw'
     */
    Time_Varying_Model_2022a_B.sigma_w[1] = Time_Varying_Model_2022a_P.pgw_Y0;

    /* End of SystemInitialize for SubSystem: '<S116>/Hpgw' */

    /* SystemInitialize for Enabled SubSystem: '<S116>/Hqgw' */
    /* InitializeConditions for Integrator: '<S128>/qgw_p' */
    Time_Varying_Model_2022a_X.qgw_p_CSTATE[1] =
      Time_Varying_Model_2022a_P.qgw_p_IC;

    /* SystemInitialize for Product: '<S128>/w' incorporates:
     *  Outport: '<S128>/qgw'
     */
    Time_Varying_Model_2022a_B.w_gw[1] = Time_Varying_Model_2022a_P.qgw_Y0;

    /* End of SystemInitialize for SubSystem: '<S116>/Hqgw' */

    /* SystemInitialize for Enabled SubSystem: '<S116>/Hrgw' */
    /* InitializeConditions for Integrator: '<S129>/rgw_p' */
    Time_Varying_Model_2022a_X.rgw_p_CSTATE[1] =
      Time_Varying_Model_2022a_P.rgw_p_IC;

    /* SystemInitialize for Product: '<S129>/w' incorporates:
     *  Outport: '<S129>/rgw'
     */
    Time_Varying_Model_2022a_B.w_g[1] = Time_Varying_Model_2022a_P.rgw_Y0;

    /* End of SystemInitialize for SubSystem: '<S116>/Hrgw' */
  }
}

/* Model terminate function */
void Time_Varying_Model_2022a_terminate(void)
{
  /* (no terminate code required) */
}
