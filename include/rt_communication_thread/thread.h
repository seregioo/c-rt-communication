/**
 * @file rt_communication_thread/thread.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header for the RealTime communication thread
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RT_COMMUNICATION_THREAD_H
#define RT_COMMUNICATION_THREAD_H

#include "rt_communication_result/result.h"
#include <stdint.h>

typedef struct rt_communication_thread_e rt_communication_thread_t;

typedef void *(rt_communication_thread_function_t)(void *);

/**
 * @brief Creates the RealTime communication thread
 *
 * @param rt_function The realtime function to be executed by the thread
 * @param priority The priority of the thread
 * @return a pointer to the result, which contains the thread if succed
 */
rt_communication_result_t *
rt_communication_thread_create(rt_communication_thread_function_t *rt_function,
                               uint8_t priority);

/**
 * @brief Destroys the RealTime thread
 *
 * @param rt_thread Pointer to the thread
 */
void rt_communication_thread_destroy(rt_communication_thread_t *rt_thread);

/**
 * @brief Starts the thread
 *
 * @param rt_thread Pointer to the thread
 * @param rt_thread_args Arguments to the thread
 * @return result of the launch
 */
rt_communication_result_t *
rt_communication_thread_run(rt_communication_thread_t *rt_thread,
                            void *rt_thread_args);

/**
 * @brief Waits the thread to finish
 *
 * @param rt_thread Pointer to the thread
 * @return result of the run
 */
rt_communication_result_t *
rt_communication_thread_join(rt_communication_thread_t *rt_thread);

#endif // RT_COMMUNICATION_THREAD_H
