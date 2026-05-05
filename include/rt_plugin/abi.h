/**
 * @file abi.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for the plugin ABI
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RT_PLUGIN_ABI_H
#define RT_PLUGIN_ABI_H

#include "rt_result/result.h"

typedef struct rt_plugin_abi_e rt_plugin_abi_t;

typedef enum rt_plugin_value_type_e {
  RT_PLUGIN_VALUE_F64,
  RT_PLUGIN_VALUE_D64,
  RT_PLUGIN_VALUE_U64,
  RT_PLUGIN_VALUE_I64,
  RT_PLUGIN_VALUE_BOOL,
  RT_PLUGIN_VALUE_STRING,
} rt_plugin_value_type_t;

typedef struct rt_plugin_port_desc_e {
  const char *name;
  rt_plugin_value_type_t type;
} rt_plugin_port_desc_t;

/**
 * @brief Creates the plugin ABI
 *
 * @param abi_path The path for the ABI folder
 * @return rt_result_t that contains the plugin ABI if successfull, NULL
 * othrewise
 */
rt_result_t rt_plugin_abi_create(char *abi_path);

/**
 * @brief Destroys the plugin ABI
 *
 * @param plugin_abi The ABI structure to be destroyed
 */
void rt_plugin_abi_destroy(rt_plugin_abi_t *plugin_abi);

/**
 * @brief Creates the plugin ABI
 *
 * If already built but there are any changes, it cleans and rebuild again
 *
 * @param plugin_abi Theplugin ABI to be built
 * @return rt_result_t that contains the plugin ABI
 */
rt_result_t rt_plugin_abi_build(rt_plugin_abi_t *plugin_abi);

/**
 * @brief Checks if the plugin ABI is built
 *
 * If already built and there is no changes, returns true
 *
 * @param plugin_abi The plugin ABI to be built
 * @param is_built The result
 * @return rt_result_t that contains the plugin ABI, and the result inside
 * is_built pointer
 */
rt_result_t rt_plugin_abi_is_built(rt_plugin_abi_t *plugin_abi, bool *is_built);

#endif // RT_PLUGIN_ABI_H
