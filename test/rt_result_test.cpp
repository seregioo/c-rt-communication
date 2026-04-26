/**
 * @file rt_result_test.cpp
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Test file for the base rt_result
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#include <gtest/gtest.h>

extern "C" {
#include "rt_result/result.h"
}

#define RT_MOCK_RESULT_STRING "mock string"
#define RT_MOCK_RESULT_STRING_RESULT "[RT Result Success]: mock string"

char *rt_communication_mock_result(rt_result_t *result) {
  (void)result;
  return (char *)RT_MOCK_RESULT_STRING;
}

TEST(RtResultTest, Lifecycle) {
  int result_value = 1;
  rt_result_type_t result_type = RT_RESULT_TYPE_SUCCESS;
  int result_subtype = 1;

  rt_result_t rt_result = rt_result_create(
      result_type, result_subtype, 0, 2, &result_value,
      (rt_result_callback_string_function_t *)rt_communication_mock_result);

  EXPECT_EQ(result_value, *(int *)rt_result_get_value(&rt_result));
  EXPECT_EQ(result_type, rt_result_get_type(&rt_result));
  EXPECT_EQ(result_subtype, rt_result_get_subtype(&rt_result));
  EXPECT_STREQ(RT_MOCK_RESULT_STRING_RESULT, rt_result_get_string(&rt_result));

  rt_result_destroy(rt_result);
}
