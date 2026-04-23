
/**
 * @file rt_communication_result/result.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compilation unit for the result
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#include "rt_communication_result/result.h"
#include <stdlib.h>

struct rt_communication_result_e {
  rt_communication_result_type_t result_type;
  int sub_type;
  void *result_value;
  rt_communication_result_callback_string_function_t
      *rt_communication_result_callback_string_function;
};

rt_communication_result_t *rt_communication_result_create(
    rt_communication_result_type_t result_type, int sub_type,
    void *result_value,
    rt_communication_result_callback_string_function_t
        rt_result_callback_funct) {
  rt_communication_result_t *rt_result =
      (rt_communication_result_t *)malloc(sizeof(rt_communication_result_t));

  if (!rt_result) {
    return NULL;
  }

  rt_result->result_type = result_type;
  rt_result->sub_type = sub_type;
  rt_result->result_value = result_value;
  rt_result->rt_communication_result_callback_string_function =
      rt_result_callback_funct;

  return rt_result;
}

void rt_communication_result_destroy(rt_communication_result_t *rt_result) {
  rt_result->result_value = NULL;
  rt_result->rt_communication_result_callback_string_function = NULL;
  free(rt_result);
}

rt_communication_result_type_t
rt_communication_result_get_type(rt_communication_result_t *rt_result) {
  return rt_result->result_type;
}

int rt_communication_result_get_subtype(rt_communication_result_t *rt_result) {
  return rt_result->sub_type;
}

void *rt_communication_result_get_value(rt_communication_result_t *rt_result) {
  return rt_result->result_value;
}

const char *
rt_communication_result_get_string(rt_communication_result_t *rt_result) {
  return rt_result->rt_communication_result_callback_string_function(rt_result);
}
