/**
 * @file rt_thread/thread/xenomai.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compilation unit for the RT thread implementation
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#include "rt_thread/core.h"
#include <pthread.h>

rt_result_t rt_thread_core_create(rt_thread_t *rt_thread,
                                  rt_thread_attrs_t *rt_attrs) {
  // TODO: Implement this function
}

rt_result_t rt_thread_core_run(rt_thread_t *rt_thread,
                               rt_thread_attrs_t *rt_attrs,
                               rt_thread_function_t *rt_function,
                               void *thread_function_args) {
  // TODO: Implement this function
}

rt_result_t rt_thread_core_join(rt_thread_t *rt_thread,
                                rt_thread_attrs_t *rt_attrs,
                                void *return_value) {
  // TODO: Implement this function
}

uint64_t rt_thread_core_get_time() {
  // TODO: Implement this function
}

void rt_thread_core_sleep(uint64_t nano_secs) {
  // TODO: Implement this function
}

void rt_thread_core_sleep_until(uint64_t abs_time_ns) {
  // TODO: Implement this function
}
