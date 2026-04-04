/*
 * quadrotorsmodel2_data.c
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

/* Block parameters (default storage) */
P_quadrotorsmodel2_T quadrotorsmodel2_P = {
  /* Variable: I
   * Referenced by: '<Root>/Constant4'
   */
  { 0.082, 0.0, 0.0, 0.0, 0.0845, 0.0, 0.0, 0.0, 0.1377 },

  /* Variable: K
   * Referenced by:
   *   '<Root>/Constant3'
   *   '<S3>/Gain'
   */
  { 1.0, 0.0, 0.315, -0.0008004, 1.0, -0.315, 0.0, 0.0008004, 1.0, 0.0, -0.315,
    -0.0008004, 1.0, 0.315, 0.0, 0.0008004 },

  /* Variable: Kp
   * Referenced by: '<Root>/Constant5'
   */
  { 4.0, 0.0, 0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 4.0 },

  /* Variable: Kr
   * Referenced by: '<Root>/Constant'
   */
  { 3.0, 0.0, 0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 3.0 },

  /* Variable: Kv
   * Referenced by: '<Root>/Constant6'
   */
  { 2.0, 0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 2.0 },

  /* Variable: Kw
   * Referenced by: '<Root>/Constant7'
   */
  { 1.5, 0.0, 0.0, 0.0, 1.5, 0.0, 0.0, 0.0, 1.5 },

  /* Variable: g
   * Referenced by: '<Root>/Constant2'
   */
  9.81,

  /* Variable: m
   * Referenced by: '<Root>/Mass'
   */
  4.34,

  /* Expression: [0;0;0;0;0;0;1;0;0;0;1;0;0;0;1;0;0;0]
   * Referenced by: '<Root>/Integrator'
   */
  { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 0.0 },

  /* Expression: 0
   * Referenced by: '<S3>/Fx'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S3>/Fy'
   */
  0.0
};
