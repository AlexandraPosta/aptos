/*
 * Time_Varying_Model_2022a.h
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

#ifndef RTW_HEADER_Time_Varying_Model_2022a_h_
#define RTW_HEADER_Time_Varying_Model_2022a_h_
#ifndef Time_Varying_Model_2022a_COMMON_INCLUDES_
#define Time_Varying_Model_2022a_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#endif                           /* Time_Varying_Model_2022a_COMMON_INCLUDES_ */

#include "Time_Varying_Model_2022a_types.h"
#include "rt_nonfinite.h"
#include <string.h>
#include "rtGetInf.h"
#include "rtGetNaN.h"
#include <float.h>
#include <stddef.h>

/* Macros for accessing real-time model data structure */
#ifndef rtmGetContStateDisabled
#define rtmGetContStateDisabled(rtm)   ((rtm)->contStateDisabled)
#endif

#ifndef rtmSetContStateDisabled
#define rtmSetContStateDisabled(rtm, val) ((rtm)->contStateDisabled = (val))
#endif

#ifndef rtmGetContStates
#define rtmGetContStates(rtm)          ((rtm)->contStates)
#endif

#ifndef rtmSetContStates
#define rtmSetContStates(rtm, val)     ((rtm)->contStates = (val))
#endif

#ifndef rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm) ((rtm)->CTOutputIncnstWithState)
#endif

#ifndef rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm, val) ((rtm)->CTOutputIncnstWithState = (val))
#endif

#ifndef rtmGetDerivCacheNeedsReset
#define rtmGetDerivCacheNeedsReset(rtm) ((rtm)->derivCacheNeedsReset)
#endif

#ifndef rtmSetDerivCacheNeedsReset
#define rtmSetDerivCacheNeedsReset(rtm, val) ((rtm)->derivCacheNeedsReset = (val))
#endif

#ifndef rtmGetFinalTime
#define rtmGetFinalTime(rtm)           ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetIntgData
#define rtmGetIntgData(rtm)            ((rtm)->intgData)
#endif

#ifndef rtmSetIntgData
#define rtmSetIntgData(rtm, val)       ((rtm)->intgData = (val))
#endif

#ifndef rtmGetOdeF
#define rtmGetOdeF(rtm)                ((rtm)->odeF)
#endif

#ifndef rtmSetOdeF
#define rtmSetOdeF(rtm, val)           ((rtm)->odeF = (val))
#endif

#ifndef rtmGetOdeY
#define rtmGetOdeY(rtm)                ((rtm)->odeY)
#endif

#ifndef rtmSetOdeY
#define rtmSetOdeY(rtm, val)           ((rtm)->odeY = (val))
#endif

#ifndef rtmGetPeriodicContStateIndices
#define rtmGetPeriodicContStateIndices(rtm) ((rtm)->periodicContStateIndices)
#endif

#ifndef rtmSetPeriodicContStateIndices
#define rtmSetPeriodicContStateIndices(rtm, val) ((rtm)->periodicContStateIndices = (val))
#endif

#ifndef rtmGetPeriodicContStateRanges
#define rtmGetPeriodicContStateRanges(rtm) ((rtm)->periodicContStateRanges)
#endif

#ifndef rtmSetPeriodicContStateRanges
#define rtmSetPeriodicContStateRanges(rtm, val) ((rtm)->periodicContStateRanges = (val))
#endif

#ifndef rtmGetRTWLogInfo
#define rtmGetRTWLogInfo(rtm)          ((rtm)->rtwLogInfo)
#endif

#ifndef rtmGetZCCacheNeedsReset
#define rtmGetZCCacheNeedsReset(rtm)   ((rtm)->zCCacheNeedsReset)
#endif

#ifndef rtmSetZCCacheNeedsReset
#define rtmSetZCCacheNeedsReset(rtm, val) ((rtm)->zCCacheNeedsReset = (val))
#endif

#ifndef rtmGetdX
#define rtmGetdX(rtm)                  ((rtm)->derivs)
#endif

#ifndef rtmSetdX
#define rtmSetdX(rtm, val)             ((rtm)->derivs = (val))
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTFinal
#define rtmGetTFinal(rtm)              ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

#ifndef rtmGetTStart
#define rtmGetTStart(rtm)              ((rtm)->Timing.tStart)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T Integrator4;                  /* '<S16>/Integrator4' */
  real_T Gain;                         /* '<S21>/Gain' */
  real_T Clock;                        /* '<Root>/Clock' */
  real_T OpenRocketVelocity;           /* '<Root>/OpenRocket Velocity' */
  real_T Switch;                       /* '<S15>/Switch' */
  real_T K[24];                        /* '<S9>/Selector' */
  real_T Integrator5;                  /* '<S16>/Integrator5' */
  real_T Integrator3;                  /* '<S16>/Integrator3' */
  real_T Integrator2;                  /* '<S16>/Integrator2' */
  real_T Integrator;                   /* '<S16>/Integrator' */
  real_T Integrator1;                  /* '<S16>/Integrator1' */
  real_T Sum1[6];                      /* '<Root>/Sum1' */
  real_T MatrixMultiply[4];            /* '<Root>/Matrix Multiply' */
  real_T Product[4];                   /* '<Root>/Product' */
  real_T MotorSpeed[4];                /* '<Root>/Motor Speed' */
  real_T MaxDeflection[4];             /* '<Root>/Max Deflection' */
  real_T Gain_j;                       /* '<S22>/Gain' */
  real_T Gain_l;                       /* '<S23>/Gain' */
  real_T Gain_g;                       /* '<S24>/Gain' */
  real_T Gain_f;                       /* '<S25>/Gain' */
  real_T OpenRocketalt;                /* '<Root>/OpenRocket alt' */
  real_T UnitConversion;               /* '<S118>/Unit Conversion' */
  real_T UnitConversion_e;             /* '<S124>/Unit Conversion' */
  real_T LimitFunction10ftto1000ft; /* '<S151>/Limit Function 10ft to 1000ft' */
  real_T LowAltitudeScaleLength;       /* '<S151>/Low Altitude Scale Length' */
  real_T UnitConversion_f;             /* '<S153>/Unit Conversion' */
  real_T LimitHeighth1000ft;           /* '<S134>/Limit Height h<1000ft' */
  real_T LowAltitudeIntensity;         /* '<S134>/Low Altitude Intensity' */
  real_T UnitConversion_g;             /* '<S125>/Unit Conversion' */
  real_T sigma_wg;                     /* '<S134>/sigma_wg ' */
  real_T sigma_ugsigma_vg;             /* '<S134>/sigma_ug, sigma_vg' */
  real_T PreLookUpIndexSearchaltitude_o2;
                              /* '<S133>/PreLook-Up Index Search  (altitude)' */
  real_T PreLookUpIndexSearchprobofexcee;
                        /* '<S133>/PreLook-Up Index Search  (prob of exceed)' */
  real_T MediumHighAltitudeIntensity;
                                  /* '<S133>/Medium//High Altitude Intensity' */
  real_T Sqrt[4];                      /* '<S126>/Sqrt' */
  real_T Sqrt1;                        /* '<S126>/Sqrt1' */
  real_T Divide[4];                    /* '<S126>/Divide' */
  real_T WhiteNoise[4];                /* '<S126>/White Noise' */
  real_T Product_p[4];                 /* '<S126>/Product' */
  real_T Lv[2];                        /* '<S123>/Lv' */
  real_T Lw[2];                        /* '<S123>/Lw' */
  real_T UnitConversion_gl;            /* '<S115>/Unit Conversion' */
  real_T Merge[3];                     /* '<S146>/Merge' */
  real_T UnitConversion_j[3];          /* '<S113>/Unit Conversion' */
  real_T Sum;                          /* '<Root>/Sum' */
  real_T Integrator_o[3];              /* '<S41>/Integrator' */
  real_T airspeedvectoringlobalframe[3];/* '<S1>/Add' */
  real_T Gain_n;                       /* '<S1>/Gain' */
  real_T u2[3];                        /* '<S45>/1//2' */
  real_T sincos_o1[3];                 /* '<S45>/sincos' */
  real_T sincos_o2[3];                 /* '<S45>/sincos' */
  real_T q2;                           /* '<S45>/q2' */
  real_T q0;                           /* '<S45>/q0' */
  real_T Product_e;                    /* '<S51>/Product' */
  real_T q1;                           /* '<S45>/q1' */
  real_T Product1;                     /* '<S51>/Product1' */
  real_T Product2;                     /* '<S51>/Product2' */
  real_T q3;                           /* '<S45>/q3' */
  real_T Product3;                     /* '<S51>/Product3' */
  real_T Sum_l;                        /* '<S51>/Sum' */
  real_T sqrt_e;                       /* '<S50>/sqrt' */
  real_T Product2_m;                   /* '<S46>/Product2' */
  real_T Product6;                     /* '<S47>/Product6' */
  real_T Product3_a;                   /* '<S46>/Product3' */
  real_T Product7;                     /* '<S47>/Product7' */
  real_T Sum3;                         /* '<S47>/Sum3' */
  real_T Gain2;                        /* '<S47>/Gain2' */
  real_T Product8;                     /* '<S47>/Product8' */
  real_T Product1_i;                   /* '<S46>/Product1' */
  real_T Product_a;                    /* '<S47>/Product' */
  real_T Product_f;                    /* '<S46>/Product' */
  real_T Product1_d;                   /* '<S47>/Product1' */
  real_T Sum1_k;                       /* '<S47>/Sum1' */
  real_T Gain_nk;                      /* '<S47>/Gain' */
  real_T Product4;                     /* '<S47>/Product4' */
  real_T Product2_c;                   /* '<S47>/Product2' */
  real_T Product3_h;                   /* '<S47>/Product3' */
  real_T Sum2;                         /* '<S47>/Sum2' */
  real_T Gain1;                        /* '<S47>/Gain1' */
  real_T Product5;                     /* '<S47>/Product5' */
  real_T Sum_a;                        /* '<S47>/Sum' */
  real_T Product_ee;                   /* '<S48>/Product' */
  real_T Product1_b;                   /* '<S48>/Product1' */
  real_T Sum1_f;                       /* '<S48>/Sum1' */
  real_T Gain_p;                       /* '<S48>/Gain' */
  real_T Product4_j;                   /* '<S48>/Product4' */
  real_T Product6_f;                   /* '<S48>/Product6' */
  real_T Product7_h;                   /* '<S48>/Product7' */
  real_T Sum3_a;                       /* '<S48>/Sum3' */
  real_T Gain2_p;                      /* '<S48>/Gain2' */
  real_T Product8_g;                   /* '<S48>/Product8' */
  real_T Product2_b;                   /* '<S48>/Product2' */
  real_T Product3_l;                   /* '<S48>/Product3' */
  real_T Sum2_k;                       /* '<S48>/Sum2' */
  real_T Gain1_l;                      /* '<S48>/Gain1' */
  real_T Product5_a;                   /* '<S48>/Product5' */
  real_T Sum_n;                        /* '<S48>/Sum' */
  real_T Product_d;                    /* '<S49>/Product' */
  real_T Product1_k;                   /* '<S49>/Product1' */
  real_T Sum1_m;                       /* '<S49>/Sum1' */
  real_T Gain_i;                       /* '<S49>/Gain' */
  real_T Product4_n;                   /* '<S49>/Product4' */
  real_T Product2_a;                   /* '<S49>/Product2' */
  real_T Product3_n;                   /* '<S49>/Product3' */
  real_T Sum2_n;                       /* '<S49>/Sum2' */
  real_T Gain1_k;                      /* '<S49>/Gain1' */
  real_T Product5_b;                   /* '<S49>/Product5' */
  real_T Product6_a;                   /* '<S49>/Product6' */
  real_T Product7_l;                   /* '<S49>/Product7' */
  real_T Sum3_p;                       /* '<S49>/Sum3' */
  real_T Gain2_b;                      /* '<S49>/Gain2' */
  real_T Product8_b;                   /* '<S49>/Product8' */
  real_T Sum_ai;                       /* '<S49>/Sum' */
  real_T Gain_gn;                      /* '<S27>/Gain' */
  real_T Gain_lo;                      /* '<S28>/Gain' */
  real_T Gain_ly;                      /* '<S19>/Gain' */
  real_T Gain_b;                       /* '<S20>/Gain' */
  real_T Product_k;                    /* '<S5>/Product' */
  real_T Product1_a;                   /* '<S5>/Product1' */
  real_T OpenRocketMachNo;             /* '<Root>/OpenRocket Mach No' */
  real_T Gain2_j;                      /* '<Root>/Gain2' */
  real_T Product2_j;                   /* '<S5>/Product2' */
  real_T SumofElements;                /* '<S5>/Sum of Elements' */
  real_T SumofElements1;               /* '<S5>/Sum of Elements1' */
  real_T Divide_c;                     /* '<S5>/Divide' */
  real_T WhiteNoise_p;                 /* '<S54>/White Noise' */
  real_T Output;                       /* '<S54>/Output' */
  real_T Sum4;                         /* '<S8>/Sum4' */
  real_T WhiteNoise_c;                 /* '<S53>/White Noise' */
  real_T Output_g;                     /* '<S53>/Output' */
  real_T Sum3_d;                       /* '<S8>/Sum3' */
  real_T Square;                       /* '<S8>/Square' */
  real_T Product_ax;                   /* '<S8>/Product' */
  real_T WhiteNoise_m;                 /* '<S52>/White Noise' */
  real_T Output_h;                     /* '<S52>/Output' */
  real_T Sum_b;                        /* '<S8>/Sum' */
  real_T RateTransition1;              /* '<S8>/Rate Transition1' */
  real_T MemoryX;                      /* '<S55>/MemoryX' */
  real_T Add;                          /* '<S83>/Add' */
  real_T Gain3;                        /* '<S8>/Gain3' */
  real_T Akxhatkk1;                    /* '<S76>/A[k]*xhat[k|k-1]' */
  real_T RateTransition;               /* '<S8>/Rate Transition' */
  real_T Bkuk;                         /* '<S76>/B[k]*u[k]' */
  real_T Add_p;                        /* '<S76>/Add' */
  real_T Gain_h;                       /* '<Root>/Gain' */
  real_T OpenRocketI_L;                /* '<Root>/OpenRocket I_L' */
  real_T OpenRocketI_R;                /* '<Root>/OpenRocket I_R' */
  real_T OpenRocketX_CG;               /* '<Root>/OpenRocket X_CG' */
  real_T OpenRocketmDot;               /* '<Root>/OpenRocket mDot' */
  real_T SumofElements1_g;             /* '<Root>/Sum of Elements1' */
  real_T UnitConversion_n;             /* '<S119>/Unit Conversion' */
  real_T Merge_n[3];                   /* '<S138>/Merge' */
  real_T Gain_jr;                      /* '<S26>/Gain' */
  real_T Gain_nq;                      /* '<S30>/Gain' */
  real_T Gain_o;                       /* '<S31>/Gain' */
  real_T Gain_bo;                      /* '<S32>/Gain' */
  real_T Gain_om;                      /* '<S33>/Gain' */
  real_T Sum_j;                        /* '<S2>/Sum' */
  real_T Product_o;                    /* '<S2>/Product' */
  real_T Sum_h;                        /* '<S3>/Sum' */
  real_T Square_g;                     /* '<S3>/Square' */
  real_T Product_m;                    /* '<S3>/Product' */
  real_T Sum1_o;                       /* '<S3>/Sum1' */
  real_T Square1;                      /* '<S3>/Square1' */
  real_T Product1_g;                   /* '<S3>/Product1' */
  real_T Sum2_j;                       /* '<S3>/Sum2' */
  real_T Square2;                      /* '<S3>/Square2' */
  real_T Product2_d;                   /* '<S3>/Product2' */
  real_T Sum3_m;                       /* '<S3>/Sum3' */
  real_T Square3;                      /* '<S3>/Square3' */
  real_T Product3_m;                   /* '<S3>/Product3' */
  real_T SumofElements_g;              /* '<S3>/Sum of Elements' */
  real_T Product4_a;                   /* '<S3>/Product4' */
  real_T Sum4_m;                       /* '<S3>/Sum4' */
  real_T Square4;                      /* '<S3>/Square4' */
  real_T Product5_l;                   /* '<S3>/Product5' */
  real_T Sum5;                         /* '<S3>/Sum5' */
  real_T SumofElements_h;              /* '<S6>/Sum of Elements' */
  real_T Product_p5;                   /* '<S6>/Product' */
  real_T Product_ap;                   /* '<S7>/Product' */
  real_T Gain_c;                       /* '<S17>/Gain' */
  real_T Gain_m;                       /* '<S18>/Gain' */
  real_T Gain_ie;                      /* '<S29>/Gain' */
  real_T Gain_ps;                      /* '<S34>/Gain' */
  real_T Gain_h3;                      /* '<S35>/Gain' */
  real_T Gain_mg;                      /* '<S36>/Gain' */
  real_T Gain_jm;                      /* '<S37>/Gain' */
  real_T Gain_j5;                      /* '<S38>/Gain' */
  real_T Gain_bv;                      /* '<S39>/Gain' */
  real_T Gain_d;                       /* '<S40>/Gain' */
  real_T SumofElements_f;              /* '<S16>/Sum of Elements' */
  real_T SumofElements1_d;             /* '<S16>/Sum of Elements1' */
  real_T DCM[9];                       /* '<S41>/RPY to DCM' */
  real_T TrigonometricFunction_o1;     /* '<S148>/Trigonometric Function' */
  real_T TrigonometricFunction_o2;     /* '<S148>/Trigonometric Function' */
  real_T Product2_jq[2];               /* '<S148>/Product2' */
  real_T Product1_o[2];                /* '<S148>/Product1' */
  real_T VectorConcatenate[3];         /* '<S147>/Vector Concatenate' */
  real_T Product_oc[3];                /* '<S147>/Product' */
  real_T Sum2_np[3];                   /* '<S143>/Sum2' */
  real_T Sum1_kf;                      /* '<S143>/Sum1' */
  real_T Sum_jj;                       /* '<S143>/Sum' */
  real_T Product1_h[3];                /* '<S143>/Product1' */
  real_T TrigonometricFunction_o1_h;   /* '<S150>/Trigonometric Function' */
  real_T TrigonometricFunction_o2_j;   /* '<S150>/Trigonometric Function' */
  real_T Product2_l[2];                /* '<S150>/Product2' */
  real_T Product1_m[2];                /* '<S150>/Product1' */
  real_T VectorConcatenate_b[3];       /* '<S149>/Vector Concatenate' */
  real_T Product_i[3];                 /* '<S149>/Product' */
  real_T TrigonometricFunction_o1_g;   /* '<S140>/Trigonometric Function' */
  real_T TrigonometricFunction_o2_m;   /* '<S140>/Trigonometric Function' */
  real_T Product2_k[2];                /* '<S140>/Product2' */
  real_T Product1_dm[2];               /* '<S140>/Product1' */
  real_T VectorConcatenate_i[3];       /* '<S139>/Vector Concatenate' */
  real_T Product_g[3];                 /* '<S139>/Product' */
  real_T Sum2_f[3];                    /* '<S135>/Sum2' */
  real_T Sum1_p;                       /* '<S135>/Sum1' */
  real_T Sum_d;                        /* '<S135>/Sum' */
  real_T Product1_he[3];               /* '<S135>/Product1' */
  real_T TrigonometricFunction1_o1;    /* '<S142>/Trigonometric Function1' */
  real_T TrigonometricFunction1_o2;    /* '<S142>/Trigonometric Function1' */
  real_T Product2_dy[2];               /* '<S142>/Product2' */
  real_T Product1_if[2];               /* '<S142>/Product1' */
  real_T VectorConcatenate_m[3];       /* '<S141>/Vector Concatenate' */
  real_T Product_dt[3];                /* '<S141>/Product' */
  real_T LwgV[2];                      /* '<S132>/Lwg//V' */
  real_T upi[2];                       /* '<S132>/1//pi' */
  real_T sqrt1[2];                     /* '<S132>/sqrt1' */
  real_T LugV1[2];                     /* '<S132>/Lug//V1' */
  real_T wg_p1[2];                     /* '<S132>/wg_p1' */
  real_T Sum_ht[2];                    /* '<S132>/Sum' */
  real_T w[2];                         /* '<S132>/w' */
  real_T sqrt_d;                       /* '<S132>/sqrt' */
  real_T LwgV_p[2];                    /* '<S132>/Lwg//V ' */
  real_T wg_p2[2];                     /* '<S132>/wg_p2' */
  real_T LwgV1[2];                     /* '<S132>/Lwg//V 1' */
  real_T Sum1_b[2];                    /* '<S132>/Sum1' */
  real_T w_o[2];                       /* '<S132>/w ' */
  real_T LvgV[2];                      /* '<S131>/Lvg//V' */
  real_T upi_h[2];                     /* '<S131>/(1//pi)' */
  real_T sqrt_ew[2];                   /* '<S131>/sqrt' */
  real_T LugV1_c[2];                   /* '<S131>/Lug//V1' */
  real_T vg_p1[2];                     /* '<S131>/vg_p1' */
  real_T Sum_c[2];                     /* '<S131>/Sum' */
  real_T w_l[2];                       /* '<S131>/w' */
  real_T LvgV_n[2];                    /* '<S131>/Lvg//V ' */
  real_T vgw_p2[2];                    /* '<S131>/vgw_p2' */
  real_T sqrt3[2];                     /* '<S131>/sqrt(3)' */
  real_T Sum1_l[2];                    /* '<S131>/Sum1' */
  real_T w_i[2];                       /* '<S131>/w ' */
  real_T w1[2];                        /* '<S131>/w 1' */
  real_T LugV[2];                      /* '<S130>/Lug//V' */
  real_T upi_l[2];                     /* '<S130>/(2//pi)' */
  real_T sqrt_a[2];                    /* '<S130>/sqrt' */
  real_T LugV1_m[2];                   /* '<S130>/Lug//V1' */
  real_T ug_p[2];                      /* '<S130>/ug_p' */
  real_T Sum_i[2];                     /* '<S130>/Sum' */
  real_T w_b[2];                       /* '<S130>/w' */
  real_T w1_a[2];                      /* '<S130>/w1' */
  real_T vgV[2];                       /* '<S129>/vg//V' */
  real_T rgw_p[2];                     /* '<S129>/rgw_p' */
  real_T Sum_bn[2];                    /* '<S129>/Sum' */
  real_T pi3;                          /* '<S129>/pi//3' */
  real_T w_g[2];                       /* '<S129>/w' */
  real_T wgV[2];                       /* '<S128>/wg//V' */
  real_T qgw_p[2];                     /* '<S128>/qgw_p' */
  real_T Sum_m[2];                     /* '<S128>/Sum' */
  real_T pi4;                          /* '<S128>/pi//4' */
  real_T w_gw[2];                      /* '<S128>/w' */
  real_T L13[2];                       /* '<S127>/L^1//3' */
  real_T w4;                           /* '<S127>/w4' */
  real_T u16;                          /* '<S127>/u^1//6' */
  real_T sqrt08V;                      /* '<S127>/sqrt(0.8//V)' */
  real_T w1_k[2];                      /* '<S127>/w1' */
  real_T w2[2];                        /* '<S127>/w2' */
  real_T LugV1_mq[2];                  /* '<S127>/Lug//V1' */
  real_T pgw_p[2];                     /* '<S127>/pgw_p' */
  real_T Sum_b1[2];                    /* '<S127>/Sum' */
  real_T sigma_w[2];                   /* '<S127>/sigma_w' */
  real_T w3;                           /* '<S127>/w3' */
  real_T w_m[2];                       /* '<S127>/w' */
  real_T rDot;                         /* '<S16>/Yaw' */
  real_T Nroll;                        /* '<S16>/Yaw' */
  real_T Ncorr;                        /* '<S16>/Yaw' */
  real_T Ndamp;                        /* '<S16>/Yaw' */
  real_T Ncan;                         /* '<S16>/Yaw' */
  real_T pDot;                         /* '<S16>/Roll' */
  real_T qDot;                         /* '<S16>/Pitch' */
  real_T Mroll;                        /* '<S16>/Pitch' */
  real_T Mcorr;                        /* '<S16>/Pitch' */
  real_T Mcan;                         /* '<S16>/Pitch' */
  real_T Mdamp;                        /* '<S16>/Pitch' */
  real_T rho;                          /* '<Root>/MATLAB Function4' */
  real_T CNa_can;                      /* '<Root>/MATLAB Function3' */
  real_T CNa_body;                     /* '<Root>/MATLAB Function2' */
  real_T CNa_fins;                     /* '<Root>/MATLAB Function1' */
  real_T CNa_nose;                     /* '<Root>/MATLAB Function' */
  real_T Product_n;                    /* '<S109>/Product' */
  real_T Add1;                         /* '<S109>/Add1' */
  real_T Product2_cw;                  /* '<S109>/Product2' */
  real_T Ckxhatkk1;                    /* '<S107>/C[k]*xhat[k|k-1]' */
  real_T Dkuk;                         /* '<S107>/D[k]*u[k]' */
  real_T yhatkk1;                      /* '<S107>/Add1' */
  real_T Sum_e;                        /* '<S107>/Sum' */
  real_T Product3_lw;                  /* '<S107>/Product3' */
  real_T P;                            /* '<S105>/SqrtUsedFcn' */
  real_T TmpSignalConversionAtSFunctionI[3];/* '<S1>/MATLAB Function5' */
  real_T TmpSignalConversionAtSFunctio_d[3];/* '<S1>/MATLAB Function5' */
  real_T windAngles[3];                /* '<S1>/MATLAB Function5' */
  real_T windVector[3];                /* '<S1>/MATLAB Function4' */
  uint32_T PreLookUpIndexSearchaltitude_o1;
                              /* '<S133>/PreLook-Up Index Search  (altitude)' */
  uint32_T PreLookUpIndexSearchprobofexc_m;
                        /* '<S133>/PreLook-Up Index Search  (prob of exceed)' */
  int8_T uDLookupTable;                /* '<S9>/1-D Lookup Table' */
} B_Time_Varying_Model_2022a_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T MemoryX_DSTATE;               /* '<S55>/MemoryX' */
  real_T PrevY[4];                     /* '<Root>/Motor Speed' */
  real_T LastMajorTime;                /* '<Root>/Motor Speed' */
  real_T NextOutput[4];                /* '<S126>/White Noise' */
  real_T NextOutput_p;                 /* '<S54>/White Noise' */
  real_T NextOutput_j;                 /* '<S53>/White Noise' */
  real_T NextOutput_d;                 /* '<S52>/White Noise' */
  real_T RateTransition1_Buffer;       /* '<S8>/Rate Transition1' */
  real_T RateTransition_Buffer;        /* '<S8>/Rate Transition' */
  uint32_T PreLookUpIndexSearchaltitude_DW;
                              /* '<S133>/PreLook-Up Index Search  (altitude)' */
  uint32_T PreLookUpIndexSearchprobofexcee;
                        /* '<S133>/PreLook-Up Index Search  (prob of exceed)' */
  uint32_T RandSeed[4];                /* '<S126>/White Noise' */
  uint32_T RandSeed_e;                 /* '<S54>/White Noise' */
  uint32_T RandSeed_h;                 /* '<S53>/White Noise' */
  uint32_T RandSeed_m;                 /* '<S52>/White Noise' */
  int8_T ifHeightMaxlowaltitudeelseifHei;
  /* '<S122>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */
  int8_T ifHeightMaxlowaltitudeelseifH_k;
  /* '<S121>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */
  boolean_T PrevLimited[4];            /* '<Root>/Motor Speed' */
  boolean_T icLoad;                    /* '<S55>/MemoryX' */
  boolean_T Hwgws_MODE;                /* '<S117>/Hwgw(s)' */
  boolean_T Hvgws_MODE;                /* '<S117>/Hvgw(s)' */
  boolean_T Hugws_MODE;                /* '<S117>/Hugw(s)' */
  boolean_T Hrgw_MODE;                 /* '<S116>/Hrgw' */
  boolean_T Hqgw_MODE;                 /* '<S116>/Hqgw' */
  boolean_T Hpgw_MODE;                 /* '<S116>/Hpgw' */
  boolean_T EnabledSubsystem_MODE;     /* '<S83>/Enabled Subsystem' */
  boolean_T MeasurementUpdate_MODE;    /* '<S76>/MeasurementUpdate' */
} DW_Time_Varying_Model_2022a_T;

/* Continuous states (default storage) */
typedef struct {
  real_T Integrator4_CSTATE;           /* '<S16>/Integrator4' */
  real_T Integrator5_CSTATE;           /* '<S16>/Integrator5' */
  real_T Integrator3_CSTATE;           /* '<S16>/Integrator3' */
  real_T Integrator2_CSTATE;           /* '<S16>/Integrator2' */
  real_T Integrator_CSTATE;            /* '<S16>/Integrator' */
  real_T Integrator1_CSTATE;           /* '<S16>/Integrator1' */
  real_T Integrator_CSTATE_m[3];       /* '<S41>/Integrator' */
  real_T wg_p1_CSTATE[2];              /* '<S132>/wg_p1' */
  real_T wg_p2_CSTATE[2];              /* '<S132>/wg_p2' */
  real_T vg_p1_CSTATE[2];              /* '<S131>/vg_p1' */
  real_T vgw_p2_CSTATE[2];             /* '<S131>/vgw_p2' */
  real_T ug_p_CSTATE[2];               /* '<S130>/ug_p' */
  real_T rgw_p_CSTATE[2];              /* '<S129>/rgw_p' */
  real_T qgw_p_CSTATE[2];              /* '<S128>/qgw_p' */
  real_T pgw_p_CSTATE[2];              /* '<S127>/pgw_p' */
} X_Time_Varying_Model_2022a_T;

/* State derivatives (default storage) */
typedef struct {
  real_T Integrator4_CSTATE;           /* '<S16>/Integrator4' */
  real_T Integrator5_CSTATE;           /* '<S16>/Integrator5' */
  real_T Integrator3_CSTATE;           /* '<S16>/Integrator3' */
  real_T Integrator2_CSTATE;           /* '<S16>/Integrator2' */
  real_T Integrator_CSTATE;            /* '<S16>/Integrator' */
  real_T Integrator1_CSTATE;           /* '<S16>/Integrator1' */
  real_T Integrator_CSTATE_m[3];       /* '<S41>/Integrator' */
  real_T wg_p1_CSTATE[2];              /* '<S132>/wg_p1' */
  real_T wg_p2_CSTATE[2];              /* '<S132>/wg_p2' */
  real_T vg_p1_CSTATE[2];              /* '<S131>/vg_p1' */
  real_T vgw_p2_CSTATE[2];             /* '<S131>/vgw_p2' */
  real_T ug_p_CSTATE[2];               /* '<S130>/ug_p' */
  real_T rgw_p_CSTATE[2];              /* '<S129>/rgw_p' */
  real_T qgw_p_CSTATE[2];              /* '<S128>/qgw_p' */
  real_T pgw_p_CSTATE[2];              /* '<S127>/pgw_p' */
} XDot_Time_Varying_Model_2022a_T;

/* State disabled  */
typedef struct {
  boolean_T Integrator4_CSTATE;        /* '<S16>/Integrator4' */
  boolean_T Integrator5_CSTATE;        /* '<S16>/Integrator5' */
  boolean_T Integrator3_CSTATE;        /* '<S16>/Integrator3' */
  boolean_T Integrator2_CSTATE;        /* '<S16>/Integrator2' */
  boolean_T Integrator_CSTATE;         /* '<S16>/Integrator' */
  boolean_T Integrator1_CSTATE;        /* '<S16>/Integrator1' */
  boolean_T Integrator_CSTATE_m[3];    /* '<S41>/Integrator' */
  boolean_T wg_p1_CSTATE[2];           /* '<S132>/wg_p1' */
  boolean_T wg_p2_CSTATE[2];           /* '<S132>/wg_p2' */
  boolean_T vg_p1_CSTATE[2];           /* '<S131>/vg_p1' */
  boolean_T vgw_p2_CSTATE[2];          /* '<S131>/vgw_p2' */
  boolean_T ug_p_CSTATE[2];            /* '<S130>/ug_p' */
  boolean_T rgw_p_CSTATE[2];           /* '<S129>/rgw_p' */
  boolean_T qgw_p_CSTATE[2];           /* '<S128>/qgw_p' */
  boolean_T pgw_p_CSTATE[2];           /* '<S127>/pgw_p' */
} XDis_Time_Varying_Model_2022a_T;

#ifndef ODE3_INTG
#define ODE3_INTG

/* ODE3 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[3];                        /* derivatives */
} ODE3_IntgData;

#endif

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T pitchAngle;                   /* '<Root>/thetaOut' */
  real_T x1;                           /* '<Root>/x1Out' */
  real_T x2;                           /* '<Root>/x2Out' */
  real_T y1;                           /* '<Root>/y1Out' */
  real_T y2;                           /* '<Root>/y2Out' */
  real_T v;                            /* '<Root>/vOut' */
  real_T yawAngle;                     /* '<Root>/psiOut' */
  real_T airspeed_x;                   /* '<Root>/airspeed_x' */
  real_T airspeed_y;                   /* '<Root>/airspeed_y' */
  real_T airspeed_z;                   /* '<Root>/airspeed_z' */
  real_T alpha;                        /* '<Root>/alphaOut' */
  real_T beta;                         /* '<Root>/betaOut' */
  real_T pOut;                         /* '<Root>/pOut' */
  real_T qOut;                         /* '<Root>/qOut' */
  real_T rOut;                         /* '<Root>/rOut' */
  real_T rollAngle;                    /* '<Root>/phiOut' */
} ExtY_Time_Varying_Model_2022a_T;

/* Parameters (default storage) */
struct P_Time_Varying_Model_2022a_T_ {
  real_T BandLimitedWhiteNoise2_Cov;
                                   /* Mask Parameter: BandLimitedWhiteNoise2_Cov
                                    * Referenced by: '<S54>/Output'
                                    */
  real_T BandLimitedWhiteNoise1_Cov;
                                   /* Mask Parameter: BandLimitedWhiteNoise1_Cov
                                    * Referenced by: '<S53>/Output'
                                    */
  real_T BandLimitedWhiteNoise_Cov; /* Mask Parameter: BandLimitedWhiteNoise_Cov
                                     * Referenced by: '<S52>/Output'
                                     */
  real_T DrydenWindTurbulenceModelContin;
                              /* Mask Parameter: DrydenWindTurbulenceModelContin
                               * Referenced by: '<S152>/Medium//High Altitude'
                               */
  real_T DrydenWindTurbulenceModelCont_a[4];
                              /* Mask Parameter: DrydenWindTurbulenceModelCont_a
                               * Referenced by: '<S126>/White Noise'
                               */
  real_T DrydenWindTurbulenceModelCont_c;
                              /* Mask Parameter: DrydenWindTurbulenceModelCont_c
                               * Referenced by:
                               *   '<S116>/Constant1'
                               *   '<S116>/Constant2'
                               *   '<S116>/Constant3'
                               *   '<S117>/Constant'
                               */
  real_T WhiteNoise_Ts;                /* Mask Parameter: WhiteNoise_Ts
                                        * Referenced by: '<S126>/Constant1'
                                        */
  real_T DrydenWindTurbulenceModelCon_au;
                              /* Mask Parameter: DrydenWindTurbulenceModelCon_au
                               * Referenced by: '<S133>/Probability of  Exceedance'
                               */
  real_T DrydenWindTurbulenceModelCont_i;
                              /* Mask Parameter: DrydenWindTurbulenceModelCont_i
                               * Referenced by: '<S113>/Windspeed at 20ft (6m)'
                               */
  real_T DrydenWindTurbulenceModelCont_m;
                              /* Mask Parameter: DrydenWindTurbulenceModelCont_m
                               * Referenced by: '<S113>/Wind direction'
                               */
  real_T DrydenWindTurbulenceModelCont_j;
                              /* Mask Parameter: DrydenWindTurbulenceModelCont_j
                               * Referenced by: '<S113>/Wingspan'
                               */
  real_T WhiteNoise_pwr[4];            /* Mask Parameter: WhiteNoise_pwr
                                        * Referenced by: '<S126>/Constant'
                                        */
  real_T BandLimitedWhiteNoise2_seed;
                                  /* Mask Parameter: BandLimitedWhiteNoise2_seed
                                   * Referenced by: '<S54>/White Noise'
                                   */
  real_T BandLimitedWhiteNoise1_seed;
                                  /* Mask Parameter: BandLimitedWhiteNoise1_seed
                                   * Referenced by: '<S53>/White Noise'
                                   */
  real_T BandLimitedWhiteNoise_seed;
                                   /* Mask Parameter: BandLimitedWhiteNoise_seed
                                    * Referenced by: '<S52>/White Noise'
                                    */
  real_T Lykyhatkk1_Y0;                /* Expression: 0
                                        * Referenced by: '<S107>/L*(y[k]-yhat[k|k-1])'
                                        */
  real_T deltax_Y0;                    /* Expression: 0
                                        * Referenced by: '<S109>/deltax'
                                        */
  real_T pgw_Y0;                       /* Expression: 0
                                        * Referenced by: '<S127>/pgw'
                                        */
  real_T Constant1_Value;              /* Expression: 1/3
                                        * Referenced by: '<S127>/Constant1'
                                        */
  real_T Constant2_Value;              /* Expression: 1/6
                                        * Referenced by: '<S127>/Constant2'
                                        */
  real_T Constant3_Value;              /* Expression: pi/4
                                        * Referenced by: '<S127>/Constant3'
                                        */
  real_T pgw_p_IC;                     /* Expression: 0
                                        * Referenced by: '<S127>/pgw_p'
                                        */
  real_T qgw_Y0;                       /* Expression: 0
                                        * Referenced by: '<S128>/qgw'
                                        */
  real_T qgw_p_IC;                     /* Expression: 0
                                        * Referenced by: '<S128>/qgw_p'
                                        */
  real_T pi4_Gain;                     /* Expression: pi/4
                                        * Referenced by: '<S128>/pi//4'
                                        */
  real_T rgw_Y0;                       /* Expression: 0
                                        * Referenced by: '<S129>/rgw'
                                        */
  real_T rgw_p_IC;                     /* Expression: 0
                                        * Referenced by: '<S129>/rgw_p'
                                        */
  real_T pi3_Gain;                     /* Expression: pi/3
                                        * Referenced by: '<S129>/pi//3'
                                        */
  real_T ugw_Y0;                       /* Expression: 0
                                        * Referenced by: '<S130>/ugw'
                                        */
  real_T upi_Gain;                     /* Expression: 2/pi
                                        * Referenced by: '<S130>/(2//pi)'
                                        */
  real_T ug_p_IC;                      /* Expression: 0
                                        * Referenced by: '<S130>/ug_p'
                                        */
  real_T vgw_Y0;                       /* Expression: 0
                                        * Referenced by: '<S131>/vgw'
                                        */
  real_T upi_Gain_m;                   /* Expression: 1/pi
                                        * Referenced by: '<S131>/(1//pi)'
                                        */
  real_T vg_p1_IC;                     /* Expression: 0
                                        * Referenced by: '<S131>/vg_p1'
                                        */
  real_T vgw_p2_IC;                    /* Expression: 0
                                        * Referenced by: '<S131>/vgw_p2'
                                        */
  real_T sqrt3_Gain;                   /* Expression: sqrt(3)
                                        * Referenced by: '<S131>/sqrt(3)'
                                        */
  real_T wgw_Y0;                       /* Expression: 0
                                        * Referenced by: '<S132>/wgw'
                                        */
  real_T upi_Gain_l;                   /* Expression: 1/pi
                                        * Referenced by: '<S132>/1//pi'
                                        */
  real_T Constant_Value;               /* Expression: 3
                                        * Referenced by: '<S132>/Constant'
                                        */
  real_T wg_p1_IC;                     /* Expression: 0
                                        * Referenced by: '<S132>/wg_p1'
                                        */
  real_T wg_p2_IC;                     /* Expression: 0
                                        * Referenced by: '<S132>/wg_p2'
                                        */
  real_T Gain_Gain;                    /* Expression: 1
                                        * Referenced by: '<S137>/Gain'
                                        */
  real_T max_height_low_Value;         /* Expression: max_height_low
                                        * Referenced by: '<S135>/max_height_low'
                                        */
  real_T min_height_high_Value;        /* Expression: min_height_high
                                        * Referenced by: '<S135>/min_height_high'
                                        */
  real_T Gain_Gain_o;                  /* Expression: 1
                                        * Referenced by: '<S145>/Gain'
                                        */
  real_T max_height_low_Value_l;       /* Expression: max_height_low
                                        * Referenced by: '<S143>/max_height_low'
                                        */
  real_T min_height_high_Value_h;      /* Expression: min_height_high
                                        * Referenced by: '<S143>/min_height_high'
                                        */
  real_T Integrator4_IC;               /* Expression: 0
                                        * Referenced by: '<S16>/Integrator4'
                                        */
  real_T Gain_Gain_h;                  /* Expression: 180/pi
                                        * Referenced by: '<S21>/Gain'
                                        */
  real_T Constant_Value_j;             /* Expression: 1
                                        * Referenced by: '<S15>/Constant'
                                        */
  real_T OpenRocketVelocity_tableData[223];/* Expression: OR.v
                                            * Referenced by: '<Root>/OpenRocket Velocity'
                                            */
  real_T OpenRocketVelocity_bp01Data[223];/* Expression: OR.t
                                           * Referenced by: '<Root>/OpenRocket Velocity'
                                           */
  real_T Constant1_Value_p;            /* Expression: 0
                                        * Referenced by: '<S15>/Constant1'
                                        */
  real_T Switch_Threshold;             /* Expression: controller.minControlSpeed
                                        * Referenced by: '<S15>/Switch'
                                        */
  real_T Constant_Value_l[1200];       /* Expression: controller.K
                                        * Referenced by: '<S9>/Constant'
                                        */
  real_T uDLookupTable_bp01Data[50];   /* Expression: controller.velocities
                                        * Referenced by: '<S9>/1-D Lookup Table'
                                        */
  real_T phi_Value;                    /* Expression: 0
                                        * Referenced by: '<Root>/phi'
                                        */
  real_T theta_Value;                  /* Expression: 0
                                        * Referenced by: '<Root>/theta'
                                        */
  real_T psi_Value;                    /* Expression: 0
                                        * Referenced by: '<Root>/psi'
                                        */
  real_T p_Value;                      /* Expression: 0
                                        * Referenced by: '<Root>/p'
                                        */
  real_T q_Value;                      /* Expression: 0
                                        * Referenced by: '<Root>/q'
                                        */
  real_T r_Value;                      /* Expression: 0
                                        * Referenced by: '<Root>/r'
                                        */
  real_T Integrator5_IC;               /* Expression: 0
                                        * Referenced by: '<S16>/Integrator5'
                                        */
  real_T Integrator3_IC;               /* Expression: 0
                                        * Referenced by: '<S16>/Integrator3'
                                        */
  real_T Integrator2_IC;               /* Expression: 0
                                        * Referenced by: '<S16>/Integrator2'
                                        */
  real_T Integrator_IC;                /* Expression: 0
                                        * Referenced by: '<S16>/Integrator'
                                        */
  real_T Integrator1_IC;               /* Expression: 0
                                        * Referenced by: '<S16>/Integrator1'
                                        */
  real_T MotorSpeed_RisingLim;         /* Expression: 3.27
                                        * Referenced by: '<Root>/Motor Speed'
                                        */
  real_T MotorSpeed_FallingLim;        /* Expression: -3.27
                                        * Referenced by: '<Root>/Motor Speed'
                                        */
  real_T MaxDeflection_UpperSat;       /* Expression: 0.23
                                        * Referenced by: '<Root>/Max Deflection'
                                        */
  real_T MaxDeflection_LowerSat;       /* Expression: -0.23
                                        * Referenced by: '<Root>/Max Deflection'
                                        */
  real_T Gain_Gain_i;                  /* Expression: 180/pi
                                        * Referenced by: '<S22>/Gain'
                                        */
  real_T Gain_Gain_oo;                 /* Expression: 180/pi
                                        * Referenced by: '<S23>/Gain'
                                        */
  real_T Gain_Gain_p;                  /* Expression: 180/pi
                                        * Referenced by: '<S24>/Gain'
                                        */
  real_T Gain_Gain_f;                  /* Expression: 180/pi
                                        * Referenced by: '<S25>/Gain'
                                        */
  real_T OpenRocketalt_tableData[223]; /* Expression: OR.h
                                        * Referenced by: '<Root>/OpenRocket alt'
                                        */
  real_T OpenRocketalt_bp01Data[223];  /* Expression: OR.t
                                        * Referenced by: '<Root>/OpenRocket alt'
                                        */
  real_T LimitFunction10ftto1000ft_Upper;/* Expression: max_height_low
                                          * Referenced by: '<S151>/Limit Function 10ft to 1000ft'
                                          */
  real_T LimitFunction10ftto1000ft_Lower;/* Expression: 10
                                          * Referenced by: '<S151>/Limit Function 10ft to 1000ft'
                                          */
  real_T LimitHeighth1000ft_UpperSat;  /* Expression: max_height_low
                                        * Referenced by: '<S134>/Limit Height h<1000ft'
                                        */
  real_T LimitHeighth1000ft_LowerSat;  /* Expression: 0
                                        * Referenced by: '<S134>/Limit Height h<1000ft'
                                        */
  real_T sigma_wg_Gain;                /* Expression: 0.1
                                        * Referenced by: '<S134>/sigma_wg '
                                        */
  real_T PreLookUpIndexSearchaltitude_Br[12];/* Expression: h_vec
                                              * Referenced by: '<S133>/PreLook-Up Index Search  (altitude)'
                                              */
  real_T PreLookUpIndexSearchprobofexcee[7];/* Expression: [1:7]
                                             * Referenced by: '<S133>/PreLook-Up Index Search  (prob of exceed)'
                                             */
  real_T MediumHighAltitudeIntensity_Tab[84];/* Expression: sigma_vec'
                                              * Referenced by: '<S133>/Medium//High Altitude Intensity'
                                              */
  real_T WhiteNoise_Mean;              /* Expression: 0
                                        * Referenced by: '<S126>/White Noise'
                                        */
  real_T WhiteNoise_StdDev;            /* Computed Parameter: WhiteNoise_StdDev
                                        * Referenced by: '<S126>/White Noise'
                                        */
  real_T Lv_Gain;                      /* Expression: 1
                                        * Referenced by: '<S123>/Lv'
                                        */
  real_T Lw_Gain;                      /* Expression: 1
                                        * Referenced by: '<S123>/Lw'
                                        */
  real_T Windspeed_Value;              /* Expression: 5.00
                                        * Referenced by: '<Root>/Windspeed'
                                        */
  real_T Integrator_IC_d;              /* Expression: 0
                                        * Referenced by: '<S41>/Integrator'
                                        */
  real_T Constant1_Value_a;            /* Expression: 0
                                        * Referenced by: '<S1>/Constant1'
                                        */
  real_T Gain_Gain_e;                  /* Expression: -1
                                        * Referenced by: '<S1>/Gain'
                                        */
  real_T u2_Gain;                      /* Expression: 0.5
                                        * Referenced by: '<S45>/1//2'
                                        */
  real_T Constant_Value_lw;            /* Expression: 0.5
                                        * Referenced by: '<S47>/Constant'
                                        */
  real_T Gain2_Gain;                   /* Expression: 2
                                        * Referenced by: '<S47>/Gain2'
                                        */
  real_T Gain_Gain_k;                  /* Expression: 2
                                        * Referenced by: '<S47>/Gain'
                                        */
  real_T Gain1_Gain;                   /* Expression: 2
                                        * Referenced by: '<S47>/Gain1'
                                        */
  real_T Gain_Gain_ev;                 /* Expression: 2
                                        * Referenced by: '<S48>/Gain'
                                        */
  real_T Constant_Value_o;             /* Expression: 0.5
                                        * Referenced by: '<S48>/Constant'
                                        */
  real_T Gain2_Gain_o;                 /* Expression: 2
                                        * Referenced by: '<S48>/Gain2'
                                        */
  real_T Gain1_Gain_d;                 /* Expression: 2
                                        * Referenced by: '<S48>/Gain1'
                                        */
  real_T Gain_Gain_ob;                 /* Expression: 2
                                        * Referenced by: '<S49>/Gain'
                                        */
  real_T Gain1_Gain_k;                 /* Expression: 2
                                        * Referenced by: '<S49>/Gain1'
                                        */
  real_T Constant_Value_f;             /* Expression: 0.5
                                        * Referenced by: '<S49>/Constant'
                                        */
  real_T Gain2_Gain_a;                 /* Expression: 2
                                        * Referenced by: '<S49>/Gain2'
                                        */
  real_T Constant_Value_ji;            /* Expression: 1
                                        * Referenced by: '<S1>/Constant'
                                        */
  real_T Constant16_Value;             /* Expression: 0
                                        * Referenced by: '<S1>/Constant16'
                                        */
  real_T Gain_Gain_fn;                 /* Expression: 180/pi
                                        * Referenced by: '<S27>/Gain'
                                        */
  real_T Gain_Gain_kf;                 /* Expression: 180/pi
                                        * Referenced by: '<S28>/Gain'
                                        */
  real_T Gain_Gain_ou;                 /* Expression: 180/pi
                                        * Referenced by: '<S19>/Gain'
                                        */
  real_T Gain_Gain_d;                  /* Expression: 180/pi
                                        * Referenced by: '<S20>/Gain'
                                        */
  real_T Constant6_Value;              /* Expression: aptos.A_ref
                                        * Referenced by: '<Root>/Constant6'
                                        */
  real_T Constant32_Value;             /* Expression: 0
                                        * Referenced by: '<Root>/Constant32'
                                        */
  real_T Constant33_Value;             /* Expression: aptos.A_plan_nose
                                        * Referenced by: '<Root>/Constant33'
                                        */
  real_T Constant34_Value;             /* Expression: aptos.K_const
                                        * Referenced by: '<Root>/Constant34'
                                        */
  real_T Constant22_Value;             /* Expression: aptos.CP_nose
                                        * Referenced by: '<Root>/Constant22'
                                        */
  real_T Constant35_Value;             /* Expression: aptos.A_plan_body
                                        * Referenced by: '<Root>/Constant35'
                                        */
  real_T Constant36_Value;             /* Expression: aptos.K_const
                                        * Referenced by: '<Root>/Constant36'
                                        */
  real_T Constant23_Value;             /* Expression: aptos.CP_body
                                        * Referenced by: '<Root>/Constant23'
                                        */
  real_T Constant39_Value;             /* Expression: aptos.ss_fin
                                        * Referenced by: '<Root>/Constant39'
                                        */
  real_T OpenRocketMachNo_tableData[223];/* Expression: OR.M
                                          * Referenced by: '<Root>/OpenRocket Mach No'
                                          */
  real_T OpenRocketMachNo_bp01Data[223];/* Expression: OR.t
                                         * Referenced by: '<Root>/OpenRocket Mach No'
                                         */
  real_T Constant40_Value;             /* Expression: aptos.S_fin
                                        * Referenced by: '<Root>/Constant40'
                                        */
  real_T Constant41_Value;             /* Expression: aptos.G_c_fin
                                        * Referenced by: '<Root>/Constant41'
                                        */
  real_T Constant11_Value;             /* Expression: aptos.d_body
                                        * Referenced by: '<Root>/Constant11'
                                        */
  real_T Gain2_Gain_a5;                /* Expression: 0.5
                                        * Referenced by: '<Root>/Gain2'
                                        */
  real_T Constant24_Value;             /* Expression: aptos.CP_fins
                                        * Referenced by: '<Root>/Constant24'
                                        */
  real_T WhiteNoise_Mean_m;            /* Expression: 0
                                        * Referenced by: '<S54>/White Noise'
                                        */
  real_T WhiteNoise_StdDev_a;         /* Computed Parameter: WhiteNoise_StdDev_a
                                       * Referenced by: '<S54>/White Noise'
                                       */
  real_T WhiteNoise_Mean_l;            /* Expression: 0
                                        * Referenced by: '<S53>/White Noise'
                                        */
  real_T WhiteNoise_StdDev_o;         /* Computed Parameter: WhiteNoise_StdDev_o
                                       * Referenced by: '<S53>/White Noise'
                                       */
  real_T WhiteNoise_Mean_c;            /* Expression: 0
                                        * Referenced by: '<S52>/White Noise'
                                        */
  real_T WhiteNoise_StdDev_m;         /* Computed Parameter: WhiteNoise_StdDev_m
                                       * Referenced by: '<S52>/White Noise'
                                       */
  real_T KalmanGainM_Value;            /* Expression: pInitialization.M
                                        * Referenced by: '<S56>/KalmanGainM'
                                        */
  real_T C_Value;                      /* Expression: pInitialization.C
                                        * Referenced by: '<S55>/C'
                                        */
  real_T X0_Value;                     /* Expression: pInitialization.X0
                                        * Referenced by: '<S55>/X0'
                                        */
  real_T Gain3_Gain;                   /* Expression: 0.5
                                        * Referenced by: '<S8>/Gain3'
                                        */
  real_T A_Value;                      /* Expression: pInitialization.A
                                        * Referenced by: '<S55>/A'
                                        */
  real_T B_Value;                      /* Expression: pInitialization.B
                                        * Referenced by: '<S55>/B'
                                        */
  real_T CovarianceZ_Value;            /* Expression: pInitialization.Z
                                        * Referenced by: '<S56>/CovarianceZ'
                                        */
  real_T KalmanGainL_Value;            /* Expression: pInitialization.L
                                        * Referenced by: '<S56>/KalmanGainL'
                                        */
  real_T D_Value;                      /* Expression: pInitialization.D
                                        * Referenced by: '<S55>/D'
                                        */
  real_T Constant25_Value;             /* Expression: aptos.X_CP_can
                                        * Referenced by: '<Root>/Constant25'
                                        */
  real_T Constant28_Value;             /* Expression: aptos.S_can
                                        * Referenced by: '<Root>/Constant28'
                                        */
  real_T Constant37_Value;             /* Expression: aptos.ss_can
                                        * Referenced by: '<Root>/Constant37'
                                        */
  real_T Constant38_Value;             /* Expression: aptos.G_c_can
                                        * Referenced by: '<Root>/Constant38'
                                        */
  real_T Gain_Gain_du;                 /* Expression: 0.5
                                        * Referenced by: '<Root>/Gain'
                                        */
  real_T OpenRocketI_L_tableData[223]; /* Expression: OR.I_L
                                        * Referenced by: '<Root>/OpenRocket I_L'
                                        */
  real_T OpenRocketI_L_bp01Data[223];  /* Expression: OR.t
                                        * Referenced by: '<Root>/OpenRocket I_L'
                                        */
  real_T OpenRocketI_R_tableData[223]; /* Expression: OR.I_R
                                        * Referenced by: '<Root>/OpenRocket I_R'
                                        */
  real_T OpenRocketI_R_bp01Data[223];  /* Expression: OR.t
                                        * Referenced by: '<Root>/OpenRocket I_R'
                                        */
  real_T OpenRocketX_CG_tableData[223];/* Expression: OR.X_CG
                                        * Referenced by: '<Root>/OpenRocket X_CG'
                                        */
  real_T OpenRocketX_CG_bp01Data[223]; /* Expression: OR.t
                                        * Referenced by: '<Root>/OpenRocket X_CG'
                                        */
  real_T OpenRocketmDot_tableData[223];/* Expression: OR.mDot
                                        * Referenced by: '<Root>/OpenRocket mDot'
                                        */
  real_T OpenRocketmDot_bp01Data[223]; /* Expression: OR.t
                                        * Referenced by: '<Root>/OpenRocket mDot'
                                        */
  real_T Gain_Gain_c;                  /* Expression: 180/pi
                                        * Referenced by: '<S26>/Gain'
                                        */
  real_T Gain_Gain_n;                  /* Expression: 180/pi
                                        * Referenced by: '<S30>/Gain'
                                        */
  real_T Gain_Gain_f5;                 /* Expression: 180/pi
                                        * Referenced by: '<S31>/Gain'
                                        */
  real_T Gain_Gain_m;                  /* Expression: 180/pi
                                        * Referenced by: '<S32>/Gain'
                                        */
  real_T Gain_Gain_l;                  /* Expression: 180/pi
                                        * Referenced by: '<S33>/Gain'
                                        */
  real_T Constant_Value_fq;            /* Expression: 0.5
                                        * Referenced by: '<S3>/Constant'
                                        */
  real_T Constant1_Value_o;            /* Expression: aptos.X_T
                                        * Referenced by: '<S3>/Constant1'
                                        */
  real_T Constant7_Value;              /* Expression: aptos.N
                                        * Referenced by: '<Root>/Constant7'
                                        */
  real_T Constant12_Value;             /* Expression: aptos.yMAC
                                        * Referenced by: '<Root>/Constant12'
                                        */
  real_T Constant13_Value;             /* Expression: aptos.rt
                                        * Referenced by: '<Root>/Constant13'
                                        */
  real_T Constant8_Value;              /* Expression: aptos.CNa_1
                                        * Referenced by: '<Root>/Constant8'
                                        */
  real_T Constant_Value_k;             /* Expression: 0.5
                                        * Referenced by: '<S7>/Constant'
                                        */
  real_T Constant9_Value;              /* Expression: aptos.CNa0
                                        * Referenced by: '<Root>/Constant9'
                                        */
  real_T Constant10_Value;             /* Expression: aptos.sumTerm
                                        * Referenced by: '<Root>/Constant10'
                                        */
  real_T Gain_Gain_cc;                 /* Expression: 180/pi
                                        * Referenced by: '<S17>/Gain'
                                        */
  real_T Gain_Gain_eq;                 /* Expression: 180/pi
                                        * Referenced by: '<S18>/Gain'
                                        */
  real_T Gain_Gain_cj;                 /* Expression: 180/pi
                                        * Referenced by: '<S29>/Gain'
                                        */
  real_T Gain_Gain_ho;                 /* Expression: 180/pi
                                        * Referenced by: '<S34>/Gain'
                                        */
  real_T Constant26_Value;             /* Expression: aptos.Y_CP_can
                                        * Referenced by: '<Root>/Constant26'
                                        */
  real_T Constant27_Value;             /* Expression: aptos.Z_CP_can
                                        * Referenced by: '<Root>/Constant27'
                                        */
  real_T Constant30_Value;             /* Expression: aptos.Xbar_CP_can
                                        * Referenced by: '<Root>/Constant30'
                                        */
  real_T Constant31_Value;             /* Expression: aptos.CLa
                                        * Referenced by: '<Root>/Constant31'
                                        */
  real_T Gain_Gain_kh;                 /* Expression: 180/pi
                                        * Referenced by: '<S35>/Gain'
                                        */
  real_T Gain_Gain_d3;                 /* Expression: 180/pi
                                        * Referenced by: '<S36>/Gain'
                                        */
  real_T Gain_Gain_mv;                 /* Expression: 180/pi
                                        * Referenced by: '<S37>/Gain'
                                        */
  real_T Gain_Gain_di;                 /* Expression: 180/pi
                                        * Referenced by: '<S38>/Gain'
                                        */
  real_T Gain_Gain_g;                  /* Expression: 180/pi
                                        * Referenced by: '<S39>/Gain'
                                        */
  real_T Gain_Gain_g4;                 /* Expression: 180/pi
                                        * Referenced by: '<S40>/Gain'
                                        */
  real_T Rearfincant_Value;            /* Expression: 0
                                        * Referenced by: '<S16>/Rear fin cant'
                                        */
  uint32_T MediumHighAltitudeIntensity_max[2];
                          /* Computed Parameter: MediumHighAltitudeIntensity_max
                           * Referenced by: '<S133>/Medium//High Altitude Intensity'
                           */
  boolean_T Enable_Value;              /* Expression: true()
                                        * Referenced by: '<S55>/Enable'
                                        */
  boolean_T isSqrtUsed_Value;          /* Expression: pInitialization.isSqrtUsed
                                        * Referenced by: '<S105>/isSqrtUsed'
                                        */
  int8_T uDLookupTable_tableData[50];
                                  /* Computed Parameter: uDLookupTable_tableData
                                   * Referenced by: '<S9>/1-D Lookup Table'
                                   */
};

/* Real-time Model Data Structure */
struct tag_RTM_Time_Varying_Model_20_T {
  const char_T *errorStatus;
  RTWLogInfo *rtwLogInfo;
  RTWSolverInfo solverInfo;
  X_Time_Varying_Model_2022a_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  XDis_Time_Varying_Model_2022a_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[25];
  real_T odeF[3][25];
  ODE3_IntgData intgData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    uint32_T clockTick1;
    uint32_T clockTickH1;
    struct {
      uint16_T TID[4];
    } TaskCounters;

    time_T tStart;
    time_T tFinal;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[4];
  } Timing;
};

/* Block parameters (default storage) */
extern P_Time_Varying_Model_2022a_T Time_Varying_Model_2022a_P;

/* Block signals (default storage) */
extern B_Time_Varying_Model_2022a_T Time_Varying_Model_2022a_B;

/* Continuous states (default storage) */
extern X_Time_Varying_Model_2022a_T Time_Varying_Model_2022a_X;

/* Disabled states (default storage) */
extern XDis_Time_Varying_Model_2022a_T Time_Varying_Model_2022a_XDis;

/* Block states (default storage) */
extern DW_Time_Varying_Model_2022a_T Time_Varying_Model_2022a_DW;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Time_Varying_Model_2022a_T Time_Varying_Model_2022a_Y;

/* Model entry point functions */
extern void Time_Varying_Model_2022a_initialize(void);
extern void Time_Varying_Model_2022a_step(void);
extern void Time_Varying_Model_2022a_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Time_Varying_Model_2_T *const Time_Varying_Model_2022a_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S1>/Display' : Unused code path elimination
 * Block '<S1>/Display1' : Unused code path elimination
 * Block '<S1>/Display2' : Unused code path elimination
 * Block '<S1>/Display3' : Unused code path elimination
 * Block '<S1>/Display4' : Unused code path elimination
 * Block '<S4>/Divide' : Unused code path elimination
 * Block '<S4>/Product' : Unused code path elimination
 * Block '<S4>/Product1' : Unused code path elimination
 * Block '<S4>/Product2' : Unused code path elimination
 * Block '<S4>/Product3' : Unused code path elimination
 * Block '<S4>/Sum of Elements' : Unused code path elimination
 * Block '<S4>/Sum of Elements1' : Unused code path elimination
 * Block '<S99>/Data Type Duplicate' : Unused code path elimination
 * Block '<S100>/Data Type Duplicate' : Unused code path elimination
 * Block '<S101>/Conversion' : Unused code path elimination
 * Block '<S101>/Data Type Duplicate' : Unused code path elimination
 * Block '<S102>/Data Type Duplicate' : Unused code path elimination
 * Block '<S59>/Data Type Duplicate' : Unused code path elimination
 * Block '<S60>/Data Type Duplicate' : Unused code path elimination
 * Block '<S61>/Data Type Duplicate' : Unused code path elimination
 * Block '<S62>/Data Type Duplicate' : Unused code path elimination
 * Block '<S64>/Data Type Duplicate' : Unused code path elimination
 * Block '<S65>/Data Type Duplicate' : Unused code path elimination
 * Block '<S66>/Data Type Duplicate' : Unused code path elimination
 * Block '<S67>/Conversion' : Unused code path elimination
 * Block '<S67>/Data Type Duplicate' : Unused code path elimination
 * Block '<S68>/Data Type Duplicate' : Unused code path elimination
 * Block '<S69>/Data Type Duplicate' : Unused code path elimination
 * Block '<S70>/Data Type Duplicate' : Unused code path elimination
 * Block '<S72>/Data Type Duplicate' : Unused code path elimination
 * Block '<S73>/Data Type Duplicate' : Unused code path elimination
 * Block '<S55>/G' : Unused code path elimination
 * Block '<S55>/H' : Unused code path elimination
 * Block '<S55>/N' : Unused code path elimination
 * Block '<S55>/P0' : Unused code path elimination
 * Block '<S55>/Q' : Unused code path elimination
 * Block '<S55>/R' : Unused code path elimination
 * Block '<S97>/CheckSignalProperties' : Unused code path elimination
 * Block '<S98>/CheckSignalProperties' : Unused code path elimination
 * Block '<Root>/Display' : Unused code path elimination
 * Block '<Root>/Display1' : Unused code path elimination
 * Block '<Root>/Display2' : Unused code path elimination
 * Block '<Root>/Display3' : Unused code path elimination
 * Block '<Root>/Sum of Elements' : Unused code path elimination
 * Block '<Root>/Windspeed1' : Unused code path elimination
 * Block '<Root>/Windspeed2' : Unused code path elimination
 * Block '<S99>/Conversion' : Eliminate redundant data type conversion
 * Block '<S100>/Conversion' : Eliminate redundant data type conversion
 * Block '<S102>/Conversion' : Eliminate redundant data type conversion
 * Block '<S72>/Conversion' : Eliminate redundant data type conversion
 * Block '<S76>/Reshape' : Reshape block reduction
 * Block '<S55>/ReshapeX0' : Reshape block reduction
 * Block '<S55>/Reshapeu' : Reshape block reduction
 * Block '<S55>/Reshapexhat' : Reshape block reduction
 * Block '<S55>/Reshapey' : Reshape block reduction
 * Block '<S113>/Cast' : Eliminate redundant data type conversion
 * Block '<S113>/Cast To Double' : Eliminate redundant data type conversion
 * Block '<S113>/Cast To Double1' : Eliminate redundant data type conversion
 * Block '<S113>/Cast To Double2' : Eliminate redundant data type conversion
 * Block '<S113>/Cast To Double3' : Eliminate redundant data type conversion
 * Block '<S113>/Cast To Double4' : Eliminate redundant data type conversion
 * Block '<S139>/Reshape' : Reshape block reduction
 * Block '<S139>/Reshape1' : Reshape block reduction
 * Block '<S141>/Reshape' : Reshape block reduction
 * Block '<S147>/Reshape' : Reshape block reduction
 * Block '<S147>/Reshape1' : Reshape block reduction
 * Block '<S149>/Reshape' : Reshape block reduction
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Time_Varying_Model_2022a'
 * '<S1>'   : 'Time_Varying_Model_2022a/Calculate Angle of Attack and Sideslip'
 * '<S2>'   : 'Time_Varying_Model_2022a/Calculate C1'
 * '<S3>'   : 'Time_Varying_Model_2022a/Calculate C2'
 * '<S4>'   : 'Time_Varying_Model_2022a/Calculate CP with canards'
 * '<S5>'   : 'Time_Varying_Model_2022a/Calculate CP without canards'
 * '<S6>'   : 'Time_Varying_Model_2022a/Calculate Ccd'
 * '<S7>'   : 'Time_Varying_Model_2022a/Calculate Cdp'
 * '<S8>'   : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure'
 * '<S9>'   : 'Time_Varying_Model_2022a/Gain Scheduling'
 * '<S10>'  : 'Time_Varying_Model_2022a/MATLAB Function'
 * '<S11>'  : 'Time_Varying_Model_2022a/MATLAB Function1'
 * '<S12>'  : 'Time_Varying_Model_2022a/MATLAB Function2'
 * '<S13>'  : 'Time_Varying_Model_2022a/MATLAB Function3'
 * '<S14>'  : 'Time_Varying_Model_2022a/MATLAB Function4'
 * '<S15>'  : 'Time_Varying_Model_2022a/Minimum controllability switch'
 * '<S16>'  : 'Time_Varying_Model_2022a/Nonlinear Dynamics'
 * '<S17>'  : 'Time_Varying_Model_2022a/Radians to Degrees'
 * '<S18>'  : 'Time_Varying_Model_2022a/Radians to Degrees1'
 * '<S19>'  : 'Time_Varying_Model_2022a/Radians to Degrees10'
 * '<S20>'  : 'Time_Varying_Model_2022a/Radians to Degrees11'
 * '<S21>'  : 'Time_Varying_Model_2022a/Radians to Degrees12'
 * '<S22>'  : 'Time_Varying_Model_2022a/Radians to Degrees13'
 * '<S23>'  : 'Time_Varying_Model_2022a/Radians to Degrees14'
 * '<S24>'  : 'Time_Varying_Model_2022a/Radians to Degrees15'
 * '<S25>'  : 'Time_Varying_Model_2022a/Radians to Degrees16'
 * '<S26>'  : 'Time_Varying_Model_2022a/Radians to Degrees17'
 * '<S27>'  : 'Time_Varying_Model_2022a/Radians to Degrees18'
 * '<S28>'  : 'Time_Varying_Model_2022a/Radians to Degrees19'
 * '<S29>'  : 'Time_Varying_Model_2022a/Radians to Degrees2'
 * '<S30>'  : 'Time_Varying_Model_2022a/Radians to Degrees20'
 * '<S31>'  : 'Time_Varying_Model_2022a/Radians to Degrees21'
 * '<S32>'  : 'Time_Varying_Model_2022a/Radians to Degrees22'
 * '<S33>'  : 'Time_Varying_Model_2022a/Radians to Degrees23'
 * '<S34>'  : 'Time_Varying_Model_2022a/Radians to Degrees3'
 * '<S35>'  : 'Time_Varying_Model_2022a/Radians to Degrees4'
 * '<S36>'  : 'Time_Varying_Model_2022a/Radians to Degrees5'
 * '<S37>'  : 'Time_Varying_Model_2022a/Radians to Degrees6'
 * '<S38>'  : 'Time_Varying_Model_2022a/Radians to Degrees7'
 * '<S39>'  : 'Time_Varying_Model_2022a/Radians to Degrees8'
 * '<S40>'  : 'Time_Varying_Model_2022a/Radians to Degrees9'
 * '<S41>'  : 'Time_Varying_Model_2022a/Windspeed//Wind Angle'
 * '<S42>'  : 'Time_Varying_Model_2022a/Calculate Angle of Attack and Sideslip/MATLAB Function4'
 * '<S43>'  : 'Time_Varying_Model_2022a/Calculate Angle of Attack and Sideslip/MATLAB Function5'
 * '<S44>'  : 'Time_Varying_Model_2022a/Calculate Angle of Attack and Sideslip/Quaternion Rotation'
 * '<S45>'  : 'Time_Varying_Model_2022a/Calculate Angle of Attack and Sideslip/Rotation Angles to Quaternions'
 * '<S46>'  : 'Time_Varying_Model_2022a/Calculate Angle of Attack and Sideslip/Quaternion Rotation/Quaternion Normalize'
 * '<S47>'  : 'Time_Varying_Model_2022a/Calculate Angle of Attack and Sideslip/Quaternion Rotation/V1'
 * '<S48>'  : 'Time_Varying_Model_2022a/Calculate Angle of Attack and Sideslip/Quaternion Rotation/V2'
 * '<S49>'  : 'Time_Varying_Model_2022a/Calculate Angle of Attack and Sideslip/Quaternion Rotation/V3'
 * '<S50>'  : 'Time_Varying_Model_2022a/Calculate Angle of Attack and Sideslip/Quaternion Rotation/Quaternion Normalize/Quaternion Modulus'
 * '<S51>'  : 'Time_Varying_Model_2022a/Calculate Angle of Attack and Sideslip/Quaternion Rotation/Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
 * '<S52>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Band-Limited White Noise'
 * '<S53>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Band-Limited White Noise1'
 * '<S54>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Band-Limited White Noise2'
 * '<S55>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter'
 * '<S56>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/CalculatePL'
 * '<S57>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/CalculateYhat'
 * '<S58>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/CovarianceOutputConfigurator'
 * '<S59>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/DataTypeConversionA'
 * '<S60>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/DataTypeConversionB'
 * '<S61>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/DataTypeConversionC'
 * '<S62>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/DataTypeConversionD'
 * '<S63>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/DataTypeConversionEnable'
 * '<S64>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/DataTypeConversionG'
 * '<S65>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/DataTypeConversionH'
 * '<S66>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/DataTypeConversionN'
 * '<S67>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/DataTypeConversionP'
 * '<S68>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/DataTypeConversionP0'
 * '<S69>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/DataTypeConversionQ'
 * '<S70>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/DataTypeConversionR'
 * '<S71>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/DataTypeConversionReset'
 * '<S72>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/DataTypeConversionX'
 * '<S73>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/DataTypeConversionX0'
 * '<S74>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/DataTypeConversionu'
 * '<S75>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/MemoryP'
 * '<S76>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/Observer'
 * '<S77>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/ReducedQRN'
 * '<S78>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/Reset'
 * '<S79>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/Reshapeyhat'
 * '<S80>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/ScalarExpansionP0'
 * '<S81>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/ScalarExpansionQ'
 * '<S82>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/ScalarExpansionR'
 * '<S83>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/UseCurrentEstimator'
 * '<S84>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/checkA'
 * '<S85>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/checkB'
 * '<S86>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/checkC'
 * '<S87>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/checkD'
 * '<S88>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/checkEnable'
 * '<S89>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/checkG'
 * '<S90>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/checkH'
 * '<S91>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/checkN'
 * '<S92>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/checkP0'
 * '<S93>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/checkQ'
 * '<S94>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/checkR'
 * '<S95>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/checkReset'
 * '<S96>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/checkX0'
 * '<S97>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/checku'
 * '<S98>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/checky'
 * '<S99>'  : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/CalculatePL/DataTypeConversionL'
 * '<S100>' : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/CalculatePL/DataTypeConversionM'
 * '<S101>' : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/CalculatePL/DataTypeConversionP'
 * '<S102>' : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/CalculatePL/DataTypeConversionZ'
 * '<S103>' : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/CalculatePL/Ground'
 * '<S104>' : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/CalculateYhat/Ground'
 * '<S105>' : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/CovarianceOutputConfigurator/decideOutput'
 * '<S106>' : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/CovarianceOutputConfigurator/decideOutput/SqrtUsedFcn'
 * '<S107>' : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/Observer/MeasurementUpdate'
 * '<S108>' : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/ReducedQRN/Ground'
 * '<S109>' : 'Time_Varying_Model_2022a/Calculate Dynamic Pressure/Kalman Filter/UseCurrentEstimator/Enabled Subsystem'
 * '<S110>' : 'Time_Varying_Model_2022a/Nonlinear Dynamics/Pitch'
 * '<S111>' : 'Time_Varying_Model_2022a/Nonlinear Dynamics/Roll'
 * '<S112>' : 'Time_Varying_Model_2022a/Nonlinear Dynamics/Yaw'
 * '<S113>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))'
 * '<S114>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/RPY to DCM'
 * '<S115>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Angle Conversion'
 * '<S116>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Filters on angular rates'
 * '<S117>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Filters on velocities'
 * '<S118>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Length Conversion'
 * '<S119>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Length Conversion1'
 * '<S120>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/RMS turbulence  intensities'
 * '<S121>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select angular rates'
 * '<S122>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select velocities'
 * '<S123>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Turbulence scale lengths'
 * '<S124>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Velocity Conversion'
 * '<S125>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Velocity Conversion2'
 * '<S126>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/White Noise'
 * '<S127>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Filters on angular rates/Hpgw'
 * '<S128>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Filters on angular rates/Hqgw'
 * '<S129>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Filters on angular rates/Hrgw'
 * '<S130>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Filters on velocities/Hugw(s)'
 * '<S131>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Filters on velocities/Hvgw(s)'
 * '<S132>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Filters on velocities/Hwgw(s)'
 * '<S133>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/RMS turbulence  intensities/High Altitude Intensity'
 * '<S134>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/RMS turbulence  intensities/Low Altitude Intensity'
 * '<S135>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select angular rates/Interpolate  rates'
 * '<S136>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select angular rates/Low altitude  rates'
 * '<S137>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select angular rates/Medium//High  altitude rates'
 * '<S138>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select angular rates/Merge Subsystems'
 * '<S139>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select angular rates/Interpolate  rates/wind to body transformation'
 * '<S140>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select angular rates/Interpolate  rates/wind to body transformation/convert to earth coords'
 * '<S141>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select angular rates/Low altitude  rates/wind to body transformation'
 * '<S142>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select angular rates/Low altitude  rates/wind to body transformation/convert to earth coords'
 * '<S143>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select velocities/Interpolate  velocities'
 * '<S144>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select velocities/Low altitude  velocities'
 * '<S145>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select velocities/Medium//High  altitude velocities'
 * '<S146>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select velocities/Merge Subsystems'
 * '<S147>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select velocities/Interpolate  velocities/wind to body transformation'
 * '<S148>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select velocities/Interpolate  velocities/wind to body transformation/convert to earth coords'
 * '<S149>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select velocities/Low altitude  velocities/wind to body transformation'
 * '<S150>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Select velocities/Low altitude  velocities/wind to body transformation/convert to earth coords'
 * '<S151>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Turbulence scale lengths/Low altitude scale length'
 * '<S152>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Turbulence scale lengths/Medium//High altitude scale length'
 * '<S153>' : 'Time_Varying_Model_2022a/Windspeed//Wind Angle/Dryden Wind Turbulence Model  (Continuous (+q +r))/Turbulence scale lengths/Medium//High altitude scale length/Length Conversion'
 */
#endif                              /* RTW_HEADER_Time_Varying_Model_2022a_h_ */
