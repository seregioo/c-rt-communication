/**
 * @file hindmarsh_rose.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file with the code for executing hindmarsh_rose
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef HINDMARSH_ROSE_H
#define HINDMARSH_ROSE_H

/*
 * hindmarsh_rose.h
 *
 * Header-only implementation of the Hindmarsh-Rose neuron model integrated
 * with fourth-order Runge-Kutta (RK4).
 *
 * Model:
 *   dx/dt = y - a*x^3 + b*x^2 - z + I
 *   dy/dt = c - d*x^2 - y
 *   dz/dt = r * (s * (x - x_R) - z)
 *
 * Typical bursting parameters:
 *   a = 1.0, b = 3.0, c = 1.0, d = 5.0,
 *   r = 0.006, s = 4.0, x_R = -1.6, I = 3.25
 *
 * This file is C and C++ compatible.
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct HRState {
  double x; /* membrane potential-like variable */
  double y; /* fast recovery variable */
  double z; /* slow adaptation current */
} HRState;

typedef struct HRParams {
  double a;
  double b;
  double c;
  double d;
  double r;
  double s;
  double x_R;
  double I;
} HRParams;

/* Return a commonly used parameter set that produces bursting dynamics. */
static inline HRParams hr_default_params(void) {
  HRParams p;
  p.a = 1.0;
  p.b = 3.0;
  p.c = 1.0;
  p.d = 5.0;
  p.r = 0.006;
  p.s = 4.0;
  p.x_R = -1.6;
  p.I = 3.25;
  return p;
}

/* Return a standard initial condition. */
static inline HRState hr_default_state(void) {
  HRState state;
  state.x = -1.0;
  state.y = 1.0;
  state.z = 1.0;
  return state;
}

/* Compute the right-hand side of the Hindmarsh-Rose system. */
static inline HRState hr_derivatives(HRState state, HRParams params) {
  HRState deriv;

  const double x2 = state.x * state.x;
  const double x3 = x2 * state.x;

  deriv.x = state.y - params.a * x3 + params.b * x2 - state.z + params.I;
  deriv.y = params.c - params.d * x2 - state.y;
  deriv.z = params.r * (params.s * (state.x - params.x_R) - state.z);

  return deriv;
}

/* Utility: state + scale * increment. */
static inline HRState hr_state_add_scaled(HRState state, HRState increment,
                                          double scale) {
  HRState result;
  result.x = state.x + scale * increment.x;
  result.y = state.y + scale * increment.y;
  result.z = state.z + scale * increment.z;
  return result;
}

/* Advance the Hindmarsh-Rose model by one RK4 step of size dt. */
static inline HRState hr_rk4_step(HRState state, HRParams params, double dt) {
  const HRState k1 = hr_derivatives(state, params);
  const HRState k2 =
      hr_derivatives(hr_state_add_scaled(state, k1, 0.5 * dt), params);
  const HRState k3 =
      hr_derivatives(hr_state_add_scaled(state, k2, 0.5 * dt), params);
  const HRState k4 = hr_derivatives(hr_state_add_scaled(state, k3, dt), params);

  HRState next;
  next.x = state.x + (dt / 6.0) * (k1.x + 2.0 * k2.x + 2.0 * k3.x + k4.x);
  next.y = state.y + (dt / 6.0) * (k1.y + 2.0 * k2.y + 2.0 * k3.y + k4.y);
  next.z = state.z + (dt / 6.0) * (k1.z + 2.0 * k2.z + 2.0 * k3.z + k4.z);

  return next;
}

/*
 * Fill an output trajectory array with num_steps + 1 states.
 *
 * output[0] receives initial_state.
 * output[i] receives the state after i integration steps.
 *
 * The caller must allocate output with at least num_steps + 1 elements.
 */
static inline void hr_simulate(HRState initial_state, HRParams params,
                               double dt, unsigned long num_steps,
                               HRState *output) {
  unsigned long i;

  if (output == 0) {
    return;
  }

  output[0] = initial_state;

  for (i = 0; i < num_steps; ++i) {
    output[i + 1] = hr_rk4_step(output[i], params, dt);
  }
}

#ifdef __cplusplus
}
#endif

#endif /* HINDMARSH_ROSE_H */
