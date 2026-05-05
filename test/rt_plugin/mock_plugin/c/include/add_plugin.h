/**
 * @file add_plugin.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Add plugin header
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef ADD_PLUGIN_H
#define ADD_PLUGIN_H

#include "abi.h"
#include <stddef.h>
#include <stdint.h>

/*
 * Replace this include with the real header where rt_plugin_value_t is defined.
 *
 * Example:
 *   #include "rt_plugin/value.h"
 */

#define RT_PLUGIN_EXPORT __attribute__((visibility("default")))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct add_plugin_e add_plugin_t;

/**
 * @brief Creates the plugin
 *
 * @return The plugin
 */
RT_PLUGIN_EXPORT add_plugin_t rt_abi_plugin_create();

/**
 * @brief Destroys the plugin
 *
 * @param plugin Plugin to be destroyed
 */
RT_PLUGIN_EXPORT void rt_abi_plugin_destroy(add_plugin_t *plugin);

/**
 * @brief Executes one iteration of the plugin
 *
 * Reads inputs "a" and "b", computes "result = a + b", stores it as output and
 * state.
 *
 * @param plugin Plugin to be executed
 */
RT_PLUGIN_EXPORT void rt_plugin_execute(add_plugin_t *plugin);

/**
 * @brief Gets a pointer to the config values for the plugin
 */
RT_PLUGIN_EXPORT rt_plugin_port_desc_t *
rt_plugin_get_config(add_plugin_t *plugin);

/**
 * @brief Gets a pointer to the input values of the plugin
 */
RT_PLUGIN_EXPORT rt_plugin_port_desc_t *
rt_plugin_get_inputs(add_plugin_t *plugin);

/**
 * @brief Gets a pointer to the output values of the plugin
 */
RT_PLUGIN_EXPORT bool rt_plugin_get_outputs(add_plugin_t *plugin);

/**
 * @brief Gets a pointer to the state variables of the plugin
 */
RT_PLUGIN_EXPORT bool rt_plugin_get_state_variables(add_plugin_t *plugin);

#ifdef __cplusplus
}
#endif

#endif /* ADD_PLUGIN_H */
