/**
 * @file telemetry_msg.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for the telemetry messgae
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RT_SPSC_TELEMETRY_MSG_H
#define RT_SPSC_TELEMETRY_MSG_H

#include <stdbool.h>
#include <stdint.h>

#define RT_MSG_BATCH 10
#define RT_PLUGIN_MAX_PORTS 50
#define RT_PLUGIN_MAX_VARS 20

typedef enum rt_spsc_telemetry_msg_type_e {
  RT_SPSC_TELEMETRY_MSG_TYPE_NONE = 0,
  RT_SPSC_TELEMETRY_MSG_TYPE_PLUGIN_STATUS,
  RT_SPSC_TELEMETRY_MSG_TYPE_PLUGIN_PORTS_DATA,
  RT_SPSC_TELEMETRY_MSG_TYPE_PLUGIN_VARIABLES_DATA,
  RT_SPSC_TELEMETRY_MSG_TYPE_MAX,
} rt_spsc_telemetry_msg_type_t;

typedef struct rt_msg_plugin_status_e {
  uint32_t plugin_id;
  uint8_t plugin_state;
} rt_msg_plugin_status_t;

typedef struct rt_msg_plugin_ports_data_e {
  uint32_t plugin_id;
  int8_t output_port_count;
  uint8_t input_port_count;
  uint16_t port_ids[RT_PLUGIN_MAX_PORTS];
  uint8_t sample_counts[RT_PLUGIN_MAX_PORTS];
  double values[RT_MSG_BATCH * RT_PLUGIN_MAX_PORTS];
} rt_msg_plugin_ports_data_t;

typedef struct rt_msg_plugin_variables_data_e {
  uint32_t plugin_id;
  int8_t output_port_count;
  uint8_t input_port_count;
  uint16_t port_ids[RT_PLUGIN_MAX_PORTS];
  uint8_t sample_counts[RT_PLUGIN_MAX_PORTS];
  double values[RT_MSG_BATCH * RT_PLUGIN_MAX_VARS];
} rt_msg_plugin_variables_data_t;

typedef struct rt_spsc_telemetry_msg_e {
  uint64_t seq;
  uint64_t timestamp_ns;
  rt_spsc_telemetry_msg_type_t type;

  union {
    rt_msg_plugin_status_t plugin_status;
    rt_msg_plugin_ports_data_t plugin_port_data;
    rt_msg_plugin_variables_data_t plugin_variables_data;
  } data;
} rt_spsc_telemetry_msg_t;

#endif // RT_SPSC_TELEMETRY_MSG_H
