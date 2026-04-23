/**
 * @file rt_communication_thread/result.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compilation unit for the RealTime thread result
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#include "rt_communication_result/result.h"
#include "rt_communication_thread/result.h"

static const char *rt_communication_thread_result_messages[] = {
    "RT Communication Thread Success", // RT_COMMUNICATION_RESULT_THREAD_TYPE_SUCCESS
    "RT Communication Thread Priority Failure", // RT_COMMUNICATION_RESULT_THREAD_TYPE_BAD_PRIORITY_FAILURE
    "RT Communication Thread Bad RT function", // RT_COMMUNICATION_RESULT_THREAD_TYPE_BAD_RT_FUNCTION_FAILURE
    "RT Communication Thread Memory Alloc Failure", // RT_COMMUNICATION_RESULT_THREAD_TYPE_BAD_PRIORITY_FAILURE
    "RT Communication Thread Mem Lock Failure", // RT_COMMUNICATION_RESULT_THREAD_TYPE_MEM_LOCK_FAILURE
    "RT Communication Thread Attr Failure ", // RT_COMMUNICATION_RESULT_THREAD_TYPE_ATTR_FAILURE
    "RT Communication Thread Stack Size Failure ", // RT_COMMUNICATION_RESULT_THREAD_TYPE_STACK_SIZE_FAILURE
    "RT Communication Thread Set Sched Policy Failure ", // RT_COMMUNICATION_RESULT_THREAD_TYPE_SET_SCHED_POLICY_FAILURE
    "RT Communication Thread Set Sched Param Failure ", // RT_COMMUNICATION_RESULT_THREAD_TYPE_SET_SCHED_PARAM_FAILURE
    "RT Communication Thread Inherit Sched Failure ", // RT_COMMUNICATION_RESULT_THREAD_TYPE_INHERIT_SCHED_FAILURE
    "RT Communication Thread Create Failure ", // RT_COMMUNICATION_RESULT_THREAD_TYPE_CREATE_FAILURE
    "RT Communication Thread Join Failure ", // RT_COMMUNICATION_RESULT_THREAD_TYPE_JOIN_FAILURE
    "RT Communication Thread Max Value ", // RT_COMMUNICATION_RESULT_THREAD_TYPE_MAX_VALUE
};

rt_communication_result_t *rt_communication_thread_result_create(
    rt_communication_result_type_t result_type,
    rt_communication_thread_result_type_t sub_type,
    rt_communication_thread_t *result_value,
    rt_communication_result_callback_string_function_t
        rt_result_callback_funct) {
  return rt_communication_result_create(result_type, sub_type, result_value,
                                        rt_result_callback_funct);
}

void rt_communication_thread_result_destroy(
    rt_communication_result_t *rt_result) {
  rt_communication_result_destroy(rt_result);
}

rt_communication_result_type_t
rt_communication_thread_get_type(rt_communication_result_t *rt_result) {
  return rt_communication_result_get_type(rt_result);
}

rt_communication_thread_result_type_t
rt_communication_thread_get_subtype(rt_communication_result_t *rt_result) {
  return (rt_communication_thread_result_type_t)
      rt_communication_result_get_subtype(rt_result);
}

rt_communication_thread_t *
rt_communication_thread_get_value(rt_communication_result_t *rt_result) {
  return (rt_communication_thread_t *)rt_communication_result_get_value(
      rt_result);
}

const char *rt_communication_thread_result_get_string(
    rt_communication_result_t *rt_result) {
  return rt_communication_thread_result_messages
      [rt_communication_thread_get_subtype(rt_result)];
}
