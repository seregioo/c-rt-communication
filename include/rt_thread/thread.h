/**
 * @file rt_thread/thread.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header for the RT communication thread
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RT_THREAD_H
#define RT_THREAD_H

#include "rt_result/result.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct rt_thread_e rt_thread_t;
typedef struct rt_thread_attrs_e rt_thread_attrs_t;
typedef void *(rt_thread_function_t)(void *);

/**
 * @brief Creates the RT communication thread
 *
 * @param rt_function The RT function to be executed by the thread
 * @param priority The priority of the thread
 * @return a pointer to the result, which contains the thread if succed
 */
rt_result_t rt_thread_create(int priority, size_t stack_size, int policy,
                             int inheritsched, cpu_set_t cpuset,
                             bool use_affinity);

/**
 * @brief Destroys the RT thread
 *
 * @param rt_thread Pointer to the thread
 */
void rt_thread_destroy(rt_thread_t *rt_thread);

/**
 * @brief Starts the thread
 *
 * @param rt_thread Pointer to the thread
 * @param rt_thread_args Arguments to the thread
 * @return result of the launch
 */
rt_result_t rt_thread_run(rt_thread_t *rt_thread,
                          rt_thread_function_t *rt_function,
                          void *thread_function_args);

/**
 * @brief Waits the thread to finish
 *
 * @param rt_thread Pointer to the thread
 * @param return_value Possible return value
 * @return result of the run
 */
rt_result_t rt_thread_join(rt_thread_t *rt_thread, void *return_value);

/**
 * @brief Gets time with a given backend
 *
 * @return the absolute time
 */
uint64_t rt_thread_get_time();

/**
 * @brief Sleeps with a given backend
 *
 */
void rt_thread_sleep(uint64_t nano_sec);

/**
 * @brief Sleeps until an absolute monotonic timestamp
 *
 * @param abs_time_ns Absolute CLOCK_MONOTONIC time in nanoseconds
 */
void rt_thread_sleep_until(uint64_t abs_time_ns);

#endif // RT_THREAD_H
