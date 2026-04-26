
/**
 * @file rt_result/result.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compilation unit for the result
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#include "rt_result/result.h"
#include <stdio.h>
#include <stdlib.h>

static const char *rt_result_messages[RT_RESULT_TYPE_MAX_VALUE + 1] = {
    "RT Result Invalid",   // RT_RESULT_TYPE_Invalid
    "RT Result Success",   // RT_RESULT_TYPE_SUCCESS
    "RT Result Error",     // RT_RESULT_TYPE_ERROR
    "RT Result Failure",   // RT_RESULT_TYPE_FAILURE
    "RT Result Max Value", // RT_RESULT_TYPE_MAX_VALUE
};

rt_result_t rt_result_create(
    rt_result_type_t result_type, int sub_type, uint invalid_sub_type,
    uint max_sub_type, void *result_value,
    rt_result_callback_string_function_t rt_result_callback_funct) {
  rt_result_t rt_result;

  rt_result.result_type = result_type;
  rt_result.sub_type = sub_type;
  rt_result.invalid_sub_type = invalid_sub_type;
  rt_result.max_sub_type = max_sub_type;
  rt_result.result_value = result_value;
  rt_result.rt_result_callback_string_function = rt_result_callback_funct;

  return rt_result;
}

void rt_result_destroy(rt_result_t rt_result) {
  rt_result.result_value = NULL;
  rt_result.rt_result_callback_string_function = NULL;
}

rt_result_type_t rt_result_get_type(rt_result_t *rt_result) {
  return rt_result->result_type;
}

int rt_result_get_subtype(rt_result_t *rt_result) {
  if (rt_result->sub_type < rt_result->invalid_sub_type ||
      rt_result->sub_type > rt_result->max_sub_type) {
    return rt_result->invalid_sub_type;
  }
  return rt_result->sub_type;
}

void *rt_result_get_value(rt_result_t *rt_result) {
  return rt_result->result_value;
}

bool rt_result_succeded(rt_result_t *rt_result) {
  return rt_result->result_type == RT_RESULT_TYPE_SUCCESS;
}

const char *rt_result_get_string(rt_result_t *rt_result) {
  if (!rt_result)
    return NULL;

  const char *msg = "UNKNOWN";

  if (rt_result->result_type >= 0 &&
      rt_result->result_type <= RT_RESULT_TYPE_MAX_VALUE) {
    msg = rt_result_messages[rt_result->result_type];
  }

  const char *subtype_cb_str =
      rt_result->rt_result_callback_string_function
          ? rt_result->rt_result_callback_string_function(rt_result)
          : "";

  if (!subtype_cb_str) {
    subtype_cb_str = "";
  }
  snprintf(rt_result->result_string, RT_RESULT_MAX_BUFFER_SIZE, "[%s]: %s", msg,
           subtype_cb_str);
  return rt_result->result_string;
}
