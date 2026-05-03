/**
 * @file rt_plugin/plugin.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for the RT plugin
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RT_PLUGIN_H
#define RT_PLUGIN_H

#include "rt_plugin/abi.h"
#include "rt_result/result.h"
#include <stdint.h>

#define RT_PLUGIN_MAX_STRING_VALUE 1024

typedef struct rt_plugin_e rt_plugin_t;

typedef struct rt_plugin_value_e {
  bool bool_balue;
  uint64_t unsigned_value;
  int int_value;
  double double_value;
  char string_value[RT_PLUGIN_MAX_STRING_VALUE];
} rt_plugin_value_t;

/**
 * @brief Creates the plugin
 *
 * @param plugin_abi The plugin ABI object wich contains the compiled rute
 * @param plugin_id Id of the plugin for the app
 * @return the plugin inside `rt_result_t` or NULL if fails
 */
rt_result_t rt_plugin_create(rt_plugin_abi_t *plugin_abi, uint64_t plugin_id);

/**
 * @brief Destroys the plugin
 *
 * @param plugin Plugin to be destroyed
 */
void rt_plugin_destroy(rt_plugin_t *plugin);

/**
 * @brief Executes one iteration of the plugin
 *
 * @param plugin Plugin to be executed
 * @return the plugin inside `rt_result_t`
 */
rt_result_t rt_plugin_execute(rt_plugin_t *plugin);

/**
 * @brief Sets config value for the plugin
 *
 * @param plugin The plugin
 * @param key Key to the config value
 * @param len Length of the key
 * @param plugin_value Plugin value that contains the value
 * @return the `rt_result_t` of the operation
 */
rt_result_t rt_plugin_set_config(rt_plugin_t *plugin, const char *key,
                                 size_t len, rt_plugin_value_t plugin_value);

/**
 * @brief Sets input value of the plugin
 *
 * @param plugin The plugin
 * @param key Key to the input value
 * @param len Length of the key
 * @param plugin_value Plugin value that contains the valu
 * @return the `rt_result_t` of the operation
 */
rt_result_t rt_plugin_set_input(rt_plugin_t *plugin, const char *key,
                                size_t len, rt_plugin_value_t plugin_value);

/**
 * @brief Sets input value of the plugin
 *
 * @param plugin The plugin
 * @param key Key to the output value
 * @param len Length of the key
 * @param plugin_value Plugin value that contains the value
 * @return the `rt_result_t` of the operation
 */
rt_result_t rt_plugin_set_output(rt_plugin_t *plugin, const char *key,
                                 size_t len, rt_plugin_value_t *plugin_value);
/**
 * @brief Gets config value of the plugin
 *
 * @param plugin The plugin
 * @param key Key to the config value
 * @param len Length of the key
 * @param plugin_value Plugin value pointer to contain the value
 * @return the `rt_result_t` of the operation and the value inside plugin value
 * pointer
 */
rt_result_t rt_plugin_get_config(rt_plugin_t *plugin, const char *key,
                                 size_t len, rt_plugin_value_t *plugin_value);

/**
 * @brief Gets input value of the plugin
 *
 * @param plugin The plugin
 * @param key Key to the input value
 * @param len Length of the key
 * @param plugin_value Plugin value pointer to contain the value
 * @return the `rt_result_t` of the operation and the value inside plugin value
 * pointer
 */
rt_result_t rt_plugin_get_input(rt_plugin_t *plugin, const char *key,
                                size_t len, rt_plugin_value_t *plugin_value);
/**
 * @brief Gets input value of the plugin
 *
 * @param plugin The plugin
 * @param key Key to the output value
 * @param len Length of the key
 * @param plugin_value Plugin value pointer to contain the value
 * @return the `rt_result_t` of the operation and the value inside plugin value
 * pointer
 */
rt_result_t rt_plugin_get_output(rt_plugin_t *plugin, const char *key,
                                 size_t len, rt_plugin_value_t *plugin_value);
/**
 * @brief Gets state variable value of the plugin
 *
 * @param plugin The plugin
 * @param key Key to the variable value
 * @param len Length of the key
 * @param plugin_value Plugin value pointer to contain the value
 * @return the `rt_result_t` of the operation and the value inside plugin value
 * pointer
 */
rt_result_t rt_plugin_get_state_variable(rt_plugin_t *plugin, const char *key,
                                         size_t len,
                                         rt_plugin_value_t *plugin_value);
#endif // RT_PLUGIN_H
