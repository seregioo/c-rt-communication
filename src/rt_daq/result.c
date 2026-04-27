/**
 * @file rt_daq/result.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compilation unit for the RT daq result
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#include "rt_result/result.h"
#include "rt_daq/result.h"

static const char *rt_daq_result_messages[RT_DAQ_RESULT_TYPE_MAX_VALUE + 1] = {
    "RT DAQ Invalid",               // RT_DAQ_RESULT_TYPE_INVALID
    "RT DAQ Create",                // RT_DAQ_RESULT_TYPE_CREATE
    "RT DAQ Open",                  // RT_DAQ_RESULT_TYPE_OPEN
    "RT DAQ Read",                  // RT_DAQ_RESULT_TYPE_READ
    "RT DAQ Write",                 // RT_DAQ_RESULT_TYPE_WRITE
    "RT DAQ No Ports",              // RT_DAQ_RESULT_TYPE_NO_PORTS
    "RT DAQ NULL",                  // RT_DAQ_RESULT_TYPE_NULL
    "RT DAQ Calibration not found", // RT_DAQ_RESULT_TYPE_CALIBRATION_NOT_FOUND
    "RT DAQ Max Value",             // RT_DAQ_RESULT_TYPE_MAX_VALUE
};

/**
 * @brief Returns the string related with the thread result
 *
 * @param rt_result The RT result
 * @return the string of the sub type
 */
static const char *rt_daq_result_callback(rt_result_t *rt_result) {
  return rt_daq_result_messages[rt_result_get_subtype(rt_result)];
}

rt_result_t rt_daq_result_create(rt_result_type_t result_type,
                                 rt_daq_result_type_t sub_type,
                                 rt_daq_t *result_value) {
  return rt_result_create(result_type, sub_type, RT_DAQ_RESULT_TYPE_INVALID,
                          RT_DAQ_RESULT_TYPE_MAX_VALUE, result_value,
                          rt_daq_result_callback);
}
