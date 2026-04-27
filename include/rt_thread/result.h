/**
 * @file rt_thread/result.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for the RT thread results
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#ifndef RT_THREAD_RESULT_H
#define RT_THREAD_RESULT_H

#include "rt_result/result.h"
#include "rt_thread/thread.h"

typedef enum rt_thread_result_type_e {
  RT_THREAD_RESULT_TYPE_INVALID = 0,
  RT_THREAD_RESULT_TYPE_CREATE,
  RT_THREAD_RESULT_TYPE_RUN,
  RT_THREAD_RESULT_TYPE_JOIN,
  RT_THREAD_RESULT_TYPE_MAX,
} rt_thread_result_type_t;

/**
 * @brief Creates the RT communication thread result
 *
 * @param result_type The type for the result
 * @param sub_type The specific thread type for the result
 * @param result_value A pointer to the resulted value by the thread
 * @return The created result
 */
rt_result_t rt_thread_result_create(rt_result_type_t result_type,
                                    rt_thread_result_type_t sub_type,
                                    rt_thread_t *result_value);
#endif // RT_THREAD_RESULT_H
