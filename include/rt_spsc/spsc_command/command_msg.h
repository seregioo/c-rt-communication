/**
 * @file command_msg.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for the command messgae
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RT_SPSC_COMMAND_MSG_H
#define RT_SPSC_COMMAND_MSG_H

#include <stdbool.h>
#include <stdint.h>

typedef enum rt_spsc_command_msg_type_e {
  RT_SPSC_COMMAND_MSG_TYPE_NONE = 0,
  RT_SPSC_COMMAND_MSG_TYPE_PLUGIN_UPDATE,
  RT_SPSC_COMMAND_MSG_TYPE_REQUEST_PORT_VALUES,
  RT_SPSC_COMMAND_MSG_TYPE_REQUEST_PORT_VARIABLES,
  RT_SPSC_COMMAND_MSG_TYPE_GLOBAL_COMMAND,
  RT_SPSC_COMMAND_MSG_TYPE_MAX,
} rt_spsc_command_msg_type_t;

typedef struct rt_msg_plugin_update_e {
  uint32_t plugin_id;
  uint8_t plugin_state;
} rt_msg_plugin_update_t;

typedef struct rt_msg_plugin_request_port_values_e {
  uint32_t plugin_id;
  bool send;
  uint64_t port_mask;
} rt_msg_plugin_request_port_values_t;

typedef struct rt_msg_plugin_request_variables_e {
  uint32_t plugin_id;
  bool send;
  uint64_t variable_mask;
} rt_msg_plugin_request_variables_t;

typedef struct rt_msg_global_command_update_e {
  uint8_t command;
} rt_msg_global_command_update_t;

typedef struct rt_command_msg_e {
  uint64_t seq;
  uint64_t timestamp_ns;
  rt_spsc_command_msg_type_t type;

  union {
    rt_msg_plugin_update_t plugin_update;
    rt_msg_plugin_request_port_values_t plugin_request_ports;
    rt_msg_plugin_request_variables_t plugin_request_variables;
    rt_msg_global_command_update_t global_command;
  } data;
} rt_spsc_command_msg_t;

#endif // RT_SPSC_COMMAND_MSG_H
