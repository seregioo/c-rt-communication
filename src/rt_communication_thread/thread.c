/**
 * @file rt_communication_thread/thread.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compilation unit for the RealTime thread implementation
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#include "rt_communication_thread/thread.h"
#include "rt_communication_thread/result.h"
#include <pthread.h>
#include <stdlib.h>

struct rt_communication_thread_e {
  struct sched_param param;
  pthread_attr_t attr;
  pthread_t thread;
  rt_communication_thread_function_t *rt_function;
};

rt_communication_result_t *
rt_communication_thread_create(rt_communication_thread_function_t *rt_function,
                               uint8_t priority) {
  rt_communication_thread_t *rt_thread =
      (rt_communication_thread_t *)malloc(sizeof(rt_communication_thread_t));

  if (priority < 0 || priority > 99) {
    return rt_communication_thread_result_create(
        RT_COMMUNICATION_RESULT_TYPE_FAILURE,
        RT_COMMUNICATION_RESULT_THREAD_TYPE_BAD_PRIORITY_FAILURE, NULL,
        rt_communication_thread_result_get_string);
  }

  if (!rt_function) {
    return rt_communication_thread_result_create(
        RT_COMMUNICATION_RESULT_TYPE_FAILURE,
        RT_COMMUNICATION_RESULT_THREAD_TYPE_BAD_RT_FUNCTION_FAILURE, NULL,
        rt_communication_thread_result_get_string);
  }

  if (!rt_thread) {
    return rt_communication_thread_result_create(
        RT_COMMUNICATION_RESULT_TYPE_FAILURE,
        RT_COMMUNICATION_RESULT_THREAD_TYPE_NO_MEMORY_FAILURE, NULL,
        rt_communication_thread_result_get_string);
  }

  if (pthread_attr_init(&rt_thread->attr)) {
    free(rt_thread);
    return rt_communication_thread_result_create(
        RT_COMMUNICATION_RESULT_TYPE_FAILURE,
        RT_COMMUNICATION_RESULT_THREAD_TYPE_ATTR_FAILURE, NULL,
        rt_communication_thread_result_get_string);
  }

  if (pthread_attr_setstacksize(&rt_thread->attr, PTHREAD_STACK_MIN)) {
    free(rt_thread);
    return rt_communication_thread_result_create(
        RT_COMMUNICATION_RESULT_TYPE_FAILURE,
        RT_COMMUNICATION_RESULT_THREAD_TYPE_STACK_SIZE_FAILURE, NULL,
        rt_communication_thread_result_get_string);
  }

  if (pthread_attr_setschedpolicy(&rt_thread->attr, SCHED_FIFO)) {
    free(rt_thread);
    return rt_communication_thread_result_create(
        RT_COMMUNICATION_RESULT_TYPE_FAILURE,
        RT_COMMUNICATION_RESULT_THREAD_TYPE_SET_SCHED_POLICY_FAILURE, NULL,
        rt_communication_thread_result_get_string);
  }

  rt_thread->param.sched_priority = (int)priority;

  if (pthread_attr_setschedparam(&rt_thread->attr, &rt_thread->param)) {
    free(rt_thread);
    return rt_communication_thread_result_create(
        RT_COMMUNICATION_RESULT_TYPE_FAILURE,
        RT_COMMUNICATION_RESULT_THREAD_TYPE_SET_SCHED_PARAM_FAILURE, NULL,
        rt_communication_thread_result_get_string);
  }

  if (pthread_attr_setinheritsched(&rt_thread->attr, PTHREAD_EXPLICIT_SCHED)) {
    free(rt_thread);
    return rt_communication_thread_result_create(
        RT_COMMUNICATION_RESULT_TYPE_FAILURE,
        RT_COMMUNICATION_RESULT_THREAD_TYPE_INHERIT_SCHED_FAILURE, NULL,
        rt_communication_thread_result_get_string);
  }

  // Assign here to prevent dangling pointer if fails
  rt_thread->rt_function = rt_function;

  return rt_communication_thread_result_create(
      RT_COMMUNICATION_RESULT_TYPE_FAILURE,
      RT_COMMUNICATION_RESULT_THREAD_TYPE_SUCCESS, rt_thread,
      rt_communication_thread_result_get_string);
}

void rt_communication_thread_destroy(rt_communication_thread_t *rt_thread) {
  rt_thread->rt_function = NULL;
  free(rt_thread);
}

rt_communication_result_t *
rt_communication_thread_run(rt_communication_thread_t *rt_thread,
                            void *rt_thread_args) {
  if (pthread_create(&rt_thread->thread, &rt_thread->attr,
                     rt_thread->rt_function, rt_thread_args)) {
    return rt_communication_thread_result_create(
        RT_COMMUNICATION_RESULT_TYPE_FAILURE,
        RT_COMMUNICATION_RESULT_THREAD_TYPE_CREATE_FAILURE, NULL,
        rt_communication_thread_result_get_string);
  }
  return rt_communication_thread_result_create(
      RT_COMMUNICATION_RESULT_TYPE_FAILURE,
      RT_COMMUNICATION_RESULT_THREAD_TYPE_SUCCESS, rt_thread,
      rt_communication_thread_result_get_string);
}

rt_communication_result_t *
rt_communication_thread_join(rt_communication_thread_t *rt_thread) {
  if (pthread_join(rt_thread->thread, NULL)) {
    return rt_communication_thread_result_create(
        RT_COMMUNICATION_RESULT_TYPE_FAILURE,
        RT_COMMUNICATION_RESULT_THREAD_TYPE_JOIN_FAILURE, NULL,
        rt_communication_thread_result_get_string);
  }
  return rt_communication_thread_result_create(
      RT_COMMUNICATION_RESULT_TYPE_FAILURE,
      RT_COMMUNICATION_RESULT_THREAD_TYPE_SUCCESS, rt_thread,
      rt_communication_thread_result_get_string);
}
