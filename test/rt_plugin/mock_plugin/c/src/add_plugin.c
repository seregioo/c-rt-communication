/**
 * @file add_plugin.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compilation unit for the add plugin file
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#include "add_plugin.h"

struct add_plugin_e {
  float a;
  float b;
  float result;
};

static const rt_plugin_port_desc_t INPUTS[] = {
    {"a", RT_PLUGIN_VALUE_F64},
    {"b", RT_PLUGIN_VALUE_F64},
};

static const rt_plugin_port_desc_t OUTPUTS[] = {
    {"result", RT_PLUGIN_VALUE_F64},
};

static const rt_plugin_port_desc_t STATE_VARIABLES[] = {
    {"result", RT_PLUGIN_VALUE_F64},
};
