/**
 * @file rt_thread/result.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compilation unit for the RT thread result
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#include "rt_result/result.h"
#include "rt_thread/result.h"

static const char *rt_thread_result_messages[RT_THREAD_RESULT_TYPE_MAX + 1] = {
    "RT Thread Invalid",    // RT_THREAD_RESULT_TYPE_INVALID
    "RT Thread Create ",    // RT_THREAD_RESULT_TYPE_CREATE
    "RT Thread Run ",       // RT_THREAD_RESULT_TYPE_RUN
    "RT Thread Join ",      // RT_THREAD_RESULT_TYPE_JOIN
    "RT Thread Max Value ", // RT_THREAD_RESULT_TYPE_MAX_VALUE
};

/**
 * @brief Returns the string related with the thread result
 *
 * @param rt_result The RT result
 * @return the string of the sub type
 */
static const char *rt_thread_result_callback(rt_result_t *rt_result) {
  return rt_thread_result_messages[rt_result_get_subtype(rt_result)];
}

rt_result_t rt_thread_result_create(rt_result_type_t result_type,
                                    rt_thread_result_type_t sub_type,
                                    rt_thread_t *result_value) {
  return rt_result_create(result_type, sub_type, RT_THREAD_RESULT_TYPE_INVALID,
                          RT_THREAD_RESULT_TYPE_MAX, result_value,
                          rt_thread_result_callback);
}
