/**
 * @file rt_plugin/result.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for program plugin results
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RT_PLUGIN_RESULT_H
#define RT_PLUGIN_RESULT_H

#include "rt_plugin/plugin.h"
#include "rt_result/result.h"

typedef enum rt_plugin_result_type_e {
  RT_PLUGIN_RESULT_TYPE_INVALID = 0,
  RT_PLUGIN_RESULT_TYPE_CREATE,
  RT_PLUGIN_RESULT_TYPE_RUN,
  RT_PLUGIN_RESULT_TYPE_SET_CONFIG,
  RT_PLUGIN_RESULT_TYPE_SET_INPUT,
  RT_PLUGIN_RESULT_TYPE_SET_OUPTUT,
  RT_PLUGIN_RESULT_TYPE_GET_CONFIG,
  RT_PLUGIN_RESULT_TYPE_GET_INPUT,
  RT_PLUGIN_RESULT_TYPE_GET_OUPTUT,
  RT_PLUGIN_RESULT_TYPE_GET_STATE_VARIABLE,
  RT_PLUGIN_RESULT_TYPE_MAX_VALUE
} rt_plugin_result_type_t;

/**
 * @brief Creates the RT communication plugin result
 *
 * @param result_type The type for the result
 * @param sub_type The specific plugin type for the result
 * @param result_value A pointer to the resulted value by the plugin
 * @return The created result
 */
rt_result_t rt_plugin_result_create(rt_result_type_t result_type,
                                    rt_plugin_result_type_t sub_type,
                                    rt_plugin_t *result_value);
#endif // RT_PLUGIN_RESULT_H
