/**
 * @file rt_plugin/result.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compilation unit for the RT plugin result
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#include "rt_plugin/result.h"

static const char *rt_plugin_result_messages[RT_PLUGIN_RESULT_TYPE_MAX_VALUE +
                                             1] = {
    "RT Plugin Invalid",            // RT_PLUGIN_RESULT_TYPE_INVALID
    "RT Plugin Create",             // RT_PLUGIN_RESULT_TYPE_CREATE
    "RT Plugin Run",                // RT_PLUGIN_RESULT_TYPE_RUN
    "RT Plugin Set Config",         // RT_PLUGIN_RESULT_TYPE_SET_CONFIG
    "RT Plugin Set Input",          // RT_PLUGIN_RESULT_TYPE_SET_INPUT
    "RT Plugin Set Output",         // RT_PLUGIN_RESULT_TYPE_SET_OUTPUT
    "RT Plugin Get Config",         // RT_PLUGIN_RESULT_TYPE_GET_CONFIG
    "RT Plugin Get Input",          // RT_PLUGIN_RESULT_TYPE_GET_INPUT
    "RT Plugin Get Output",         // RT_PLUGIN_RESULT_TYPE_GET_OUTPUT
    "RT Plugin Get State Variable", // RT_PLUGIN_RESULT_TYPE_GET_STATE_VARIABLE
    "RT Plugin Max Value",          // RT_PLUGIN_RESULT_TYPE_MAX_VALUE
};

/**
 * @brief Returns the string related with the thread result
 *
 * @param rt_result The RT result
 * @return the string of the sub type
 */
static const char *rt_plugin_result_callback(rt_result_t *rt_result) {
  return rt_plugin_result_messages[rt_result_get_subtype(rt_result)];
}

rt_result_t rt_plugin_result_create(rt_result_type_t result_type,
                                    rt_plugin_result_type_t sub_type,
                                    rt_plugin_t *result_value) {
  return rt_result_create(result_type, sub_type, RT_PLUGIN_RESULT_TYPE_INVALID,
                          RT_PLUGIN_RESULT_TYPE_MAX_VALUE, result_value,
                          rt_plugin_result_callback);
}
