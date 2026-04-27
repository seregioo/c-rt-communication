/**
 * @file rt_daq/result.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for the RT DAQ results
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#ifndef RT_DAQ_RESULT_H
#define RT_DAQ_RESULT_H

#include "rt_daq/daq.h"
#include "rt_result/result.h"

typedef enum rt_daq_result_type_e {
  RT_DAQ_RESULT_TYPE_INVALID = 0,
  RT_DAQ_RESULT_TYPE_CREATE,
  RT_DAQ_RESULT_TYPE_OPEN,
  RT_DAQ_RESULT_TYPE_READ,
  RT_DAQ_RESULT_TYPE_WRITE,
  RT_DAQ_RESULT_TYPE_NO_PORTS,
  RT_DAQ_RESULT_TYPE_NULL,
  RT_DAQ_RESULT_TYPE_CALIBRATION_NOT_FOUND,
  RT_DAQ_RESULT_TYPE_MAX_VALUE,
} rt_daq_result_type_t;

/**
 * @brief Creates the RT communication DAQ result
 *
 * @param result_type The type for the result
 * @param sub_type The specific DAQ type for the result
 * @param result_value A pointer to the resulted value by the DAQ
 * @return The created result
 */
rt_result_t rt_daq_result_create(rt_result_type_t result_type,
                                 rt_daq_result_type_t sub_type,
                                 rt_daq_t *result_value);

#endif // RT_DAQ_RESULT_H
