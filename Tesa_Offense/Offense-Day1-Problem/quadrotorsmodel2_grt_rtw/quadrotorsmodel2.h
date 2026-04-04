/*
 * quadrotorsmodel2.h
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

#ifndef quadrotorsmodel2_h_
#define quadrotorsmodel2_h_
#ifndef quadrotorsmodel2_COMMON_INCLUDES_
#define quadrotorsmodel2_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#include "rt_nonfinite.h"
#include "math.h"
#endif                                 /* quadrotorsmodel2_COMMON_INCLUDES_ */

#include "quadrotorsmodel2_types.h"
#include "rtGetInf.h"
#include "rtGetNaN.h"
#include <float.h>
#include <string.h>
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
  real_T Integrator[18];               /* '<Root>/Integrator' */
  real_T Derivative1[18];              /* '<Root>/Derivative1' */
  real_T xdot[18];                     /* '<Root>/MATLAB Function1' */
  real_T Phee_value;                   /* '<Root>/MATLAB Function' */
} B_quadrotorsmodel2_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T TimeStampA;                   /* '<Root>/Derivative1' */
  real_T LastUAtTimeA[18];             /* '<Root>/Derivative1' */
  real_T TimeStampB;                   /* '<Root>/Derivative1' */
  real_T LastUAtTimeB[18];             /* '<Root>/Derivative1' */
  real_T TimeStampA_g;                 /* '<Root>/Derivative' */
  real_T LastUAtTimeA_d[18];           /* '<Root>/Derivative' */
  real_T TimeStampB_g;                 /* '<Root>/Derivative' */
  real_T LastUAtTimeB_k[18];           /* '<Root>/Derivative' */
} DW_quadrotorsmodel2_T;

/* Continuous states (default storage) */
typedef struct {
  real_T Integrator_CSTATE[18];        /* '<Root>/Integrator' */
} X_quadrotorsmodel2_T;

/* State derivatives (default storage) */
typedef struct {
  real_T Integrator_CSTATE[18];        /* '<Root>/Integrator' */
} XDot_quadrotorsmodel2_T;

/* State disabled  */
typedef struct {
  boolean_T Integrator_CSTATE[18];     /* '<Root>/Integrator' */
} XDis_quadrotorsmodel2_T;

#ifndef ODE3_INTG
#define ODE3_INTG

/* ODE3 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[3];                        /* derivatives */
} ODE3_IntgData;

#endif

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T Inport;                       /* '<Root>/Inport' */
  real_T Inport1;                      /* '<Root>/Inport1' */
  real_T Inport2;                      /* '<Root>/Inport2' */
  real_T Inport3;                      /* '<Root>/Inport3' */
  real_T Inport4;                      /* '<Root>/Inport4' */
  real_T Inport5;                      /* '<Root>/Inport5' */
  real_T Inport6;                      /* '<Root>/Inport6' */
  real_T Inport7;                      /* '<Root>/Inport7' */
  real_T Inport8;                      /* '<Root>/Inport8' */
  real_T Inport9;                      /* '<Root>/Inport9' */
  real_T Inport10;                     /* '<Root>/Inport10' */
  real_T Inport11;                     /* '<Root>/Inport11' */
  real_T Inport12;                     /* '<Root>/Inport12' */
  real_T Inport13;                     /* '<Root>/Inport13' */
  real_T Inport14;                     /* '<Root>/Inport14' */
  real_T Inport15;                     /* '<Root>/Inport15' */
  real_T Inport16;                     /* '<Root>/Inport16' */
  real_T Inport17;                     /* '<Root>/Inport17' */
} ExtU_quadrotorsmodel2_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T Out2[18];                     /* '<Root>/Out2' */
  real_T Out3;                         /* '<Root>/Out3' */
  real_T Out4;                         /* '<Root>/Out4' */
  real_T Out5;                         /* '<Root>/Out5' */
  real_T Out6;                         /* '<Root>/Out6' */
} ExtY_quadrotorsmodel2_T;

/* Parameters (default storage) */
struct P_quadrotorsmodel2_T_ {
  real_T I[9];                         /* Variable: I
                                        * Referenced by: '<Root>/Constant4'
                                        */
  real_T K[16];                        /* Variable: K
                                        * Referenced by:
                                        *   '<Root>/Constant3'
                                        *   '<S3>/Gain'
                                        */
  real_T Kp[9];                        /* Variable: Kp
                                        * Referenced by: '<Root>/Constant5'
                                        */
  real_T Kr[9];                        /* Variable: Kr
                                        * Referenced by: '<Root>/Constant'
                                        */
  real_T Kv[9];                        /* Variable: Kv
                                        * Referenced by: '<Root>/Constant6'
                                        */
  real_T Kw[9];                        /* Variable: Kw
                                        * Referenced by: '<Root>/Constant7'
                                        */
  real_T g;                            /* Variable: g
                                        * Referenced by: '<Root>/Constant2'
                                        */
  real_T m;                            /* Variable: m
                                        * Referenced by: '<Root>/Mass'
                                        */
  real_T Integrator_IC[18]; /* Expression: [0;0;0;0;0;0;1;0;0;0;1;0;0;0;1;0;0;0]
                             * Referenced by: '<Root>/Integrator'
                             */
  real_T Fx_Value;                     /* Expression: 0
                                        * Referenced by: '<S3>/Fx'
                                        */
  real_T Fy_Value;                     /* Expression: 0
                                        * Referenced by: '<S3>/Fy'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_quadrotorsmodel2_T {
  const char_T *errorStatus;
  RTWLogInfo *rtwLogInfo;
  RTWSolverInfo solverInfo;
  X_quadrotorsmodel2_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  XDis_quadrotorsmodel2_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[18];
  real_T odeF[3][18];
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
    time_T tStart;
    time_T tFinal;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Block parameters (default storage) */
extern P_quadrotorsmodel2_T quadrotorsmodel2_P;

/* Block signals (default storage) */
extern B_quadrotorsmodel2_T quadrotorsmodel2_B;

/* Continuous states (default storage) */
extern X_quadrotorsmodel2_T quadrotorsmodel2_X;

/* Disabled states (default storage) */
extern XDis_quadrotorsmodel2_T quadrotorsmodel2_XDis;

/* Block states (default storage) */
extern DW_quadrotorsmodel2_T quadrotorsmodel2_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_quadrotorsmodel2_T quadrotorsmodel2_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_quadrotorsmodel2_T quadrotorsmodel2_Y;

/* Model entry point functions */
extern void quadrotorsmodel2_initialize(void);
extern void quadrotorsmodel2_step(void);
extern void quadrotorsmodel2_terminate(void);

/* Real-time Model object */
extern RT_MODEL_quadrotorsmodel2_T *const quadrotorsmodel2_M;

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
 * '<Root>' : 'quadrotorsmodel2'
 * '<S1>'   : 'quadrotorsmodel2/MATLAB Function'
 * '<S2>'   : 'quadrotorsmodel2/MATLAB Function1'
 * '<S3>'   : 'quadrotorsmodel2/Rotors'
 */
#endif                                 /* quadrotorsmodel2_h_ */
