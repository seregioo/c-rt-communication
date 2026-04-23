
/**
 * @file rt_communication_thread/result.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for the RT thread results
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#ifndef RT_COMMUNICATION_THREAD_RESULT_H
#define RT_COMMUNICATION_THREAD_RESULT_H

#include "rt_communication_result/result.h"
#include "rt_communication_thread/thread.h"

typedef enum rt_communication_thread_result_type_e {
  RT_COMMUNICATION_RESULT_THREAD_TYPE_SUCCESS = 0,
  RT_COMMUNICATION_RESULT_THREAD_TYPE_BAD_PRIORITY_FAILURE,
  RT_COMMUNICATION_RESULT_THREAD_TYPE_BAD_RT_FUNCTION_FAILURE,
  RT_COMMUNICATION_RESULT_THREAD_TYPE_NO_MEMORY_FAILURE,
  RT_COMMUNICATION_RESULT_THREAD_TYPE_MEM_LOCK_FAILURE,
  RT_COMMUNICATION_RESULT_THREAD_TYPE_ATTR_FAILURE,
  RT_COMMUNICATION_RESULT_THREAD_TYPE_STACK_SIZE_FAILURE,
  RT_COMMUNICATION_RESULT_THREAD_TYPE_SET_SCHED_FAILURE,
  RT_COMMUNICATION_RESULT_THREAD_TYPE_SET_SCHED_POLICY_FAILURE,
  RT_COMMUNICATION_RESULT_THREAD_TYPE_SET_SCHED_PARAM_FAILURE,
  RT_COMMUNICATION_RESULT_THREAD_TYPE_INHERIT_SCHED_FAILURE,
  RT_COMMUNICATION_RESULT_THREAD_TYPE_CREATE_FAILURE,
  RT_COMMUNICATION_RESULT_THREAD_TYPE_JOIN_FAILURE,
  RT_COMMUNICATION_RESULT_THREAD_TYPE_MAX_VALUE,
} rt_communication_thread_result_type_t;

/**
 * @brief Creates the RealTime communication thread result
 *
 * @param result_type The type for the result
 * @param sub_type The specific thread type for the result
 * @param result_value A pointer to the resulted value by the thread
 * @return The created result
 */
rt_communication_result_t *rt_communication_thread_result_create(
    rt_communication_result_type_t result_type,
    rt_communication_thread_result_type_t sub_type,
    rt_communication_thread_t *result_value,
    rt_communication_result_callback_string_function_t
        rt_result_callback_funct);

/**
 * @brief Destroys the result, but not its data content
 *
 * @param rt_result pointer to the result
 */
void rt_communication_thread_result_destroy(
    rt_communication_result_t *rt_result);

/**
 * @brief Returns the type of the thread RealTime result
 *
 * @param rt_result The result
 * @return the type
 */
rt_communication_result_type_t
rt_communication_thread_get_type(rt_communication_result_t *rt_result);

/**
 * @brief Returns the subtype of the RealTime result
 *
 * @param rt_result The result
 * @return the numeric value of the subtype
 */
rt_communication_thread_result_type_t
rt_communication_thread_get_subtype(rt_communication_result_t *rt_result);

/**
 * @brief Gets the value asociated with that result, if there is one
 *
 * @param rt_result The result
 * @return a pointer to the thread, or NULL
 */
rt_communication_thread_t *
rt_communication_thread_get_value(rt_communication_result_t *rt_result);

/**
 * @brief Gets the string asociated with that result
 *
 * @param rt_result The result
 * @return a string with the message
 */
const char *
rt_communication_thread_result_get_string(rt_communication_result_t *rt_result);

#endif // RT_COMMUNICATION_THREAD_RESULT_H
