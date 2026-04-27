/**
 * @file rt_daq/port/result.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compilation unit for the RT DAQ port result
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#include "rt_result/result.h"
#include "rt_daq/port/result.h"

static const char
    *rt_daq_port_result_messages[RT_DAQ_PORT_RESULT_TYPE_MAX_VALUE + 1] = {
        "RT DAQ Port Invalid",       // RT_DAQ_PORT_TYPE_INVALID
        "RT DAQ Port Bad Type",      // RT_DAQ_PORT_TYPE_BAD_TYPE
        "RT DAQ Port Bad Direction", // RT_DAQ_PORT_TYPE_BAD_DIRECTION
        "RT DAQ Port Create",        // RT_DAQ_PORT_TYPE_CREATE
        "RT DAQ Port Read",          // RT_DAQ_PORT_TYPE_READ
        "RT DAQ Port Write",         // RT_DAQ_PORT_TYPE_WRITE
        "RT DAQ Port Query",         // RT_DAQ_PORT_TYPE_QUERY
        "RT DAQ Port Open",          // RT_DAQ_PORT_TYPE_OPEN
        "RT DAQ Port Close",         // RT_DAQ_PORT_TYPE_OPEN
        "RT DAQ Port Softcall",      // RT_DAQ_RESULT_TYPE_SOFTCALL_FAIL
        "RT DAQ Port Max Value",     // RT_DAQ_PORT_TYPE_MAX_VALUE
};

/**
 * @brief Returns the string related with the DAQ port result
 *
 * @param rt_result The RT result
 * @return the string of the sub type
 */
static const char *rt_daq_port_result_callback(rt_result_t *rt_result) {
  return rt_daq_port_result_messages[rt_result_get_subtype(rt_result)];
}

rt_result_t rt_daq_port_result_create(rt_result_type_t result_type,
                                      rt_daq_port_result_type_t sub_type,
                                      rt_daq_port_t *result_value) {
  return rt_result_create(result_type, sub_type,
                          RT_DAQ_PORT_RESULT_TYPE_INVALID,
                          RT_DAQ_PORT_RESULT_TYPE_MAX_VALUE, result_value,
                          rt_daq_port_result_callback);
}
