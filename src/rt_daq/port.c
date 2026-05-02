/**
 * @file rt_daq/port.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compilation unit for the RT DAQ Port
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#include "rt_daq/port.h"
#include "rt_daq/port/result.h"
#include "rt_result/result.h"
#include <stdbool.h>
#include <stdlib.h>

struct rt_daq_port_e {
  rt_daq_port_type_t port_type;
  rt_daq_port_direction_t port_direction;
  size_t channel;
  void *calibration_data;
  int range;
  bool open;
};

rt_result_t rt_daq_port_create(rt_daq_port_type_t port_type,
                               rt_daq_port_direction_t port_direction,
                               size_t channel, int range) {
  rt_daq_port_t *port = (rt_daq_port_t *)malloc(sizeof(rt_daq_port_t));

  if (!port) {
    return rt_daq_port_result_create(RT_RESULT_TYPE_FAILURE,
                                     RT_DAQ_PORT_RESULT_TYPE_CREATE, NULL);
  }

  port->port_type = port_type;
  port->port_direction = port_direction;
  port->channel = channel;
  port->range = range;
  port->open = false;

  return rt_daq_port_result_create(RT_RESULT_TYPE_SUCCESS,
                                   RT_DAQ_PORT_RESULT_TYPE_CREATE, port);
}

void rt_daq_port_destroy(rt_daq_port_t *rt_daq_port) { free(rt_daq_port); }

rt_daq_port_type_t rt_daq_port_get_type(rt_daq_port_t *rt_daq_port) {
  if (!rt_daq_port) {
    return RT_DAQ_PORT_TYPE_INVALID;
  }
  return rt_daq_port->port_type;
}

rt_daq_port_direction_t rt_daq_port_get_direction(rt_daq_port_t *rt_daq_port) {
  if (!rt_daq_port) {
    return RT_DAQ_PORT_DIRECTION_INVALID;
  }
  return rt_daq_port->port_direction;
}

int rt_daq_port_get_channel(rt_daq_port_t *rt_daq_port) {
  if (!rt_daq_port) {
    return -1;
  }
  return rt_daq_port->channel;
}

int rt_daq_port_get_port_range(rt_daq_port_t *rt_daq_port) {
  if (!rt_daq_port) {
    return -1;
  }
  return rt_daq_port->range;
}

rt_result_t rt_daq_port_open(rt_daq_port_t *rt_daq_port,
                             void *calibration_data) {
  if (!rt_daq_port || !calibration_data) {
    return rt_daq_port_result_create(RT_RESULT_TYPE_FAILURE,
                                     RT_DAQ_PORT_RESULT_TYPE_OPEN, NULL);
  }

  rt_daq_port->open = true;
  rt_daq_port->calibration_data = calibration_data;

  return rt_daq_port_result_create(RT_RESULT_TYPE_SUCCESS,
                                   RT_DAQ_PORT_RESULT_TYPE_OPEN, rt_daq_port);
}

rt_result_t rt_daq_port_close(rt_daq_port_t *rt_daq_port) {
  if (!rt_daq_port || !rt_daq_port->calibration_data) {
    return rt_daq_port_result_create(RT_RESULT_TYPE_FAILURE,
                                     RT_DAQ_PORT_RESULT_TYPE_CLOSE, NULL);
  }

  rt_daq_port->open = false;
  rt_daq_port->calibration_data = NULL;

  return rt_daq_port_result_create(RT_RESULT_TYPE_SUCCESS,
                                   RT_DAQ_PORT_RESULT_TYPE_CLOSE, rt_daq_port);
}

rt_result_t rt_daq_port_get_calibration_data(rt_daq_port_t *rt_daq_port,
                                             void *calibration_data) {

  if (!rt_daq_port || !rt_daq_port->open || !rt_daq_port->calibration_data) {
    return rt_daq_port_result_create(RT_RESULT_TYPE_FAILURE,
                                     RT_DAQ_PORT_RESULT_TYPE_QUERY, NULL);
  }

  return rt_daq_port_result_create(
      RT_RESULT_TYPE_SUCCESS, RT_DAQ_PORT_RESULT_TYPE_QUERY,
      (rt_daq_port_t *)rt_daq_port->calibration_data);
}

bool rt_daq_port_is_open(rt_daq_port_t *rt_daq_port) {
  if (!rt_daq_port) {
    return false;
  }
  return rt_daq_port->open;
}

rt_result_t rt_daq_port_get_last_value(rt_daq_port_t *rt_daq_port);

rt_result_t rt_daq_port_get_last_n_values(rt_daq_port_t *rt_daq_port, size_t n,
                                          size_t *number_of_values);
