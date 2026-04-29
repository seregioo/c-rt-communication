/**
 * @file rt_thread/thread.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compilation unit for the RT thread implementation
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#include "rt_thread/thread.h"
#include "rt_thread/core.h"
#include "rt_thread/result.h"
#include <stdlib.h>
#include <string.h>

struct rt_thread_e {
  rt_thread_attrs_t attrs;
};

rt_result_t rt_thread_create(int priority, size_t stack_size, int policy,
                             int inheritsched, cpu_set_t cpuset,
                             bool use_affinity) {
  rt_thread_t *rt_thread = (rt_thread_t *)malloc(sizeof(rt_thread_t));
  rt_result_t rt_thread_core_result;
  rt_result_t rt_thread_error_result = rt_thread_result_create(
      RT_RESULT_TYPE_FAILURE, RT_THREAD_RESULT_TYPE_CREATE, NULL);

  if (!rt_thread) {
    return rt_thread_error_result;
  }

  memset(&rt_thread->attrs, 0, sizeof(rt_thread_attrs_t));

  rt_thread->attrs.priority = priority;
  rt_thread->attrs.stack_size = stack_size;
  rt_thread->attrs.policy = policy;
  rt_thread->attrs.inheritsched = inheritsched;
  rt_thread->attrs.cpuset = cpuset;
  rt_thread->attrs.use_affinity = use_affinity;

  rt_thread_core_result = rt_thread_core_create(rt_thread, &rt_thread->attrs);

  if (!rt_result_succeded(&rt_thread_core_result)) {
    free(rt_thread);
    rt_result_destroy(rt_thread_core_result);
    return rt_thread_error_result;
  }

  rt_result_destroy(rt_thread_error_result);

  return rt_thread_core_result;
}

void rt_thread_destroy(rt_thread_t *rt_thread) {
  rt_thread_core_destroy(&rt_thread->attrs);
  free(rt_thread);
}

rt_result_t rt_thread_run(rt_thread_t *rt_thread,
                          rt_thread_function_t *rt_function,
                          void *rt_thread_args) {
  return rt_thread_core_run(rt_thread, &rt_thread->attrs, rt_function,
                            rt_thread_args);
}

rt_result_t rt_thread_join(rt_thread_t *rt_thread, void *return_value) {
  return rt_thread_core_join(rt_thread, &rt_thread->attrs, return_value);
}

uint64_t rt_thread_get_time() { return rt_thread_core_get_time(); }

void rt_thread_sleep(uint64_t nano_secs) { rt_thread_core_sleep(nano_secs); }

void rt_thread_sleep_until(uint64_t abs_time_ns) {
  rt_thread_core_sleep_until(abs_time_ns);
}
