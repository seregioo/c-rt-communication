/**
 * @file rt_communication_thread/thread.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compilation unit for the RealTime thread implementation
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#include "rt_communication_thread/thread.h"
#include <pthread.h>

struct rt_communication_thread_e {
  struct sched_param param;
  pthread_attr_t attr;
  pthread_t thread;
};

rt_communication_result_t *
rt_communication_thread_create(rt_communication_thread_function_t *rt_function,
                               uint8_t priority) {
  // TODO: Implement this function
}

void rt_communication_thread_destroy(rt_communication_thread_t *rt_thread) {

  // TODO: Implement this function
}

rt_communication_result_t *
rt_communication_thread_run(rt_communication_thread_t *rt_thread,
                            void *rt_thread_args) {
  // TODO: Implement this function
}

rt_communication_result_t *
rt_communication_thread_join(rt_communication_thread_t *rt_thread) {
  // TODO: Implement this function
}
