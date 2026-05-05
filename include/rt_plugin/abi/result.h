/**
 * @file rt_plugin/abi/result.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for program plugin ABI results
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RT_PLUGIN_ABI_RESULT_H
#define RT_PLUGIN_ABI_RESULT_H

#include "rt_plugin/abi.h"
#include "rt_result/result.h"

typedef enum rt_plugin_abi_result_type_e {
  RT_PLUGIN_ABI_RESULT_TYPE_INVALID = 0,
  RT_PLUGIN_ABI_RESULT_TYPE_CREATE,
  RT_PLUGIN_ABI_RESULT_TYPE_BUILD,
  RT_PLUGIN_ABI_RESULT_TYPE_IS_BUILT,
  RT_PLUGIN_ABI_RESULT_TYPE_MAX_VALUE
} rt_plugin_abi_result_type_t;

/**
 * @brief Creates the RT communication plugin ABI result
 *
 * @param result_type The type for the result
 * @param sub_type The specific plugin_abi type for the result
 * @param result_value A pointer to the resulted value by the plugin ABI
 * @return The created result
 */
rt_result_t rt_plugin_abi_result_create(rt_result_type_t result_type,
                                        rt_plugin_abi_result_type_t sub_type,
                                        rt_plugin_abi_t *result_value);
#endif // RT_PLUGIN_ABI_RESULT_H
