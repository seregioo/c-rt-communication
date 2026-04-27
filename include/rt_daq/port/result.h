/**
 * @file rt_daq/port/result.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for the RT DAQ port results
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#ifndef RT_DAQ_PORT_RESULT_H
#define RT_DAQ_PORT_RESULT_H

#include "rt_daq/port.h"
#include "rt_result/result.h"

typedef enum rt_daq_port_result_type_e {
  RT_DAQ_PORT_RESULT_TYPE_INVALID = 0,
  RT_DAQ_PORT_RESULT_TYPE_BAD_TYPE,
  RT_DAQ_PORT_RESULT_TYPE_BAD_DIRECTION,
  RT_DAQ_PORT_RESULT_TYPE_CREATE,
  RT_DAQ_PORT_RESULT_TYPE_READ,
  RT_DAQ_PORT_RESULT_TYPE_WRITE,
  RT_DAQ_PORT_RESULT_TYPE_QUERY,
  RT_DAQ_PORT_RESULT_TYPE_OPEN,
  RT_DAQ_PORT_RESULT_TYPE_CLOSE,
  RT_DAQ_RESULT_TYPE_SOFTCALL_FAIL,
  RT_DAQ_PORT_RESULT_TYPE_MAX_VALUE,
} rt_daq_port_result_type_t;

/**
 * @brief Creates the RT communication DAQ port result
 *
 * @param result_type The type for the result
 * @param sub_type The specific thread type for the result
 * @param result_value A pointer to the resulted value by the port
 * @return The created result
 */
rt_result_t rt_daq_port_result_create(rt_result_type_t result_type,
                                      rt_daq_port_result_type_t sub_type,
                                      rt_daq_port_t *result_value);

#endif // RT_DAQ_PORT_RESULT_H
