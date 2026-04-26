/**
 * @file rt_thread/thread/preempt_rt.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compilation unit for the RT thread implementation
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#include "rt_thread/core.h"
#include "rt_thread/result.h"
#include <asm-generic/errno-base.h>
#include <sys/mman.h>

static void rt_thread_timespec_from_ns(uint64_t time_ns, struct timespec *ts) {
  ts->tv_sec = time_ns / 1000000000ull;
  ts->tv_nsec = time_ns % 1000000000ull;
}

rt_result_t rt_thread_core_create(rt_thread_t *rt_thread,
                                  rt_thread_attrs_t *rt_attrs) {

  rt_result_t rt_thread_error = rt_thread_result_create(
      RT_RESULT_TYPE_FAILURE, RT_THREAD_RESULT_TYPE_CREATE, rt_thread);

  if (!rt_thread || !rt_attrs) {
    return rt_thread_error;
  }

  if (rt_attrs->priority < 0 || rt_attrs->priority > 99) {
    return rt_thread_error;
  }

  if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1) {
    return rt_thread_error;
  }

  if (pthread_attr_init(&rt_attrs->attr)) {
    return rt_thread_error;
  }

  if (pthread_attr_getstacksize(&rt_attrs->attr, &rt_attrs->stack_size)) {
    return rt_thread_error;
  }

  if (pthread_attr_setstacksize(&rt_attrs->attr, rt_attrs->stack_size)) {
    return rt_thread_error;
  }

  if (pthread_attr_setschedpolicy(&rt_attrs->attr, rt_attrs->policy)) {
    return rt_thread_error;
  }

  rt_attrs->param.sched_priority = rt_attrs->priority;

  if (pthread_attr_setschedparam(&rt_attrs->attr, &rt_attrs->param)) {
    return rt_thread_error;
  }

  if (pthread_attr_setinheritsched(&rt_attrs->attr, rt_attrs->inheritsched)) {
    return rt_thread_error;
  }

  rt_result_destroy(rt_thread_error);

  return rt_thread_result_create(RT_RESULT_TYPE_SUCCESS,
                                 RT_THREAD_RESULT_TYPE_CREATE, rt_thread);
}

void rt_thread_core_destroy(rt_thread_attrs_t *rt_attrs) {
  pthread_attr_destroy(&rt_attrs->attr);
}

rt_result_t rt_thread_core_run(rt_thread_t *rt_thread,
                               rt_thread_attrs_t *rt_attrs,
                               rt_thread_function_t *rt_function,
                               void *thread_function_arg) {
  if (pthread_create(&rt_attrs->thread, &rt_attrs->attr, rt_function,
                     thread_function_arg)) {
    return rt_thread_result_create(RT_RESULT_TYPE_FAILURE,
                                   RT_THREAD_RESULT_TYPE_RUN, rt_thread);
  }
  return rt_thread_result_create(RT_RESULT_TYPE_SUCCESS,
                                 RT_THREAD_RESULT_TYPE_RUN, rt_thread);
}

rt_result_t rt_thread_core_join(rt_thread_t *rt_thread,
                                rt_thread_attrs_t *rt_attrs,
                                void *return_value) {
  if (pthread_join(rt_attrs->thread, NULL)) {
    return rt_thread_result_create(RT_RESULT_TYPE_FAILURE,
                                   RT_THREAD_RESULT_TYPE_JOIN, rt_thread);
  }
  return rt_thread_result_create(RT_RESULT_TYPE_SUCCESS,
                                 RT_THREAD_RESULT_TYPE_JOIN, rt_thread);
}

uint64_t rt_thread_core_get_time() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  return (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;
}

void rt_thread_core_sleep(uint64_t nano_secs) {
  struct timespec req;
  rt_thread_timespec_from_ns(nano_secs, &req);
  clock_nanosleep(CLOCK_MONOTONIC, 0, &req, NULL);
}

void rt_thread_core_sleep_until(uint64_t abs_time_ns) {
  struct timespec req;
  int ret;
  rt_thread_timespec_from_ns(abs_time_ns, &req);
  do {
    ret = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &req, NULL);
  } while (ret == EINTR);
}
