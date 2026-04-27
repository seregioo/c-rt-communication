/**
 * @file rt_thread/core.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header for the RT thread core
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef THREAD_CORE_H
#define THREAD_CORE_H

#include "rt_result/result.h"
#include "rt_thread/thread.h"
#include <stdbool.h>
#include <stdint.h>

struct rt_thread_attrs_e {
  pthread_t thread;
  pthread_attr_t attr;
  struct sched_param param;

  int priority;
  size_t stack_size;
  int policy;
  int inheritsched; // PTHREAD_EXPLICIT_SCHED
  cpu_set_t cpuset; // for RT isolation/affinity
  bool use_affinity;
};

/**
 * @brief Creates the RT communication thread
 *
 * @param rt_function The RT function to be executed by the thread
 * @param rt_thread_attrs_t The thread attributes
 * @return a pointer to the result, which contains the thread if succeded
 */
rt_result_t rt_thread_core_create(rt_thread_t *rt_thread,
                                  rt_thread_attrs_t *rt_attrs);

/**
 * @brief Destroys the RT attributes (if needed any)
 *
 * @param rt_thread Pointer to the thread
 */
void rt_thread_core_destroy(rt_thread_attrs_t *rt_attrs);

/**
 * @brief Starts the thread
 *
 * @param rt_thread Pointer to the thread
 * @param rt_function Pointer to the function
 * @param thread_function_args Pointer to the function arguments
 * @return result of the launch
 */
rt_result_t rt_thread_core_run(rt_thread_t *rt_thread,
                               rt_thread_attrs_t *rt_attrs,
                               rt_thread_function_t *rt_function,
                               void *thread_function_args);

/**
 * @brief Waits the thread to finish
 *
 * @param rt_thread_core Pointer to the thread
 * @param rt_args Arguments to the thread
 * @param return_value Possible return value
 * @return result of the run
 */
rt_result_t rt_thread_core_join(rt_thread_t *rt_thread,
                                rt_thread_attrs_t *rt_attrs,
                                void *return_value);

/**
 * @brief Gets time with a given backend
 *
 * @return the absolute time
 */
uint64_t rt_thread_core_get_time();

/**
 * @brief Sleeps with a given backend
 *
 */
void rt_thread_core_sleep(uint64_t nano_secs);

/**
 * @brief Sleeps until an absolute monotonic timestamp
 *
 * @param abs_time_ns Absolute CLOCK_MONOTONIC time in nanoseconds
 */
void rt_thread_core_sleep_until(uint64_t abs_time_ns);

#endif // THREAD_CORE_H
