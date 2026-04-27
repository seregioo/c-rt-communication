/**
 * @file rt_daq/port.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for the RT DAQ port
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#ifndef RT_DAQ_PORT_H
#define RT_DAQ_PORT_H

#include "rt_result/result.h"
#include <stddef.h>

typedef struct rt_daq_port_e rt_daq_port_t;

typedef enum rt_daq_port_type_e {
  RT_DAQ_PORT_TYPE_INVALID = 0,
  RT_DAQ_PORT_TYPE_ANALOG,
  RT_DAQ_PORT_TYPE_DIGITAL,
  RT_DAQ_PORT_TYPE_MAX_VALUE,
} rt_daq_port_type_t;

typedef enum rt_daq_port_direction_e {
  RT_DAQ_PORT_DIRECTION_INVALID = 0,
  RT_DAQ_PORT_DIRECTION_INPUT,
  RT_DAQ_PORT_DIRECTION_OUTPUT,
  RT_DAQ_PORT_DIRECTION_INPUT_OUTPUT,
  RT_DAQ_PORT_DIRECTION_MAX_VALUE,
} rt_daq_port_direction_t;

/**
 * @brief Creates a RT DAQ port
 *
 * @param rt_daq_port_type The RT DAQ port type
 * @param rt_daq_port_direction The RT DAQ port direction
 * @param channel The RT DAQ port channel number
 * @param range The RT DAQ port range
 * @return The RT DAQ port
 */
rt_result_t rt_daq_port_create(rt_daq_port_type_t port_type,
                               rt_daq_port_direction_t port_direction,
                               size_t channel, int range);

/**
 * @brief Destroys a RT DAQ port
 *
 * @param rt_daq The RT DAQ port to be destroyed
 */
void rt_daq_port_destroy(rt_daq_port_t *rt_daq);

/**
 * @brief Returns the port type of the RT DAQ port
 *
 * @param rt_daq_port The RT DAQ port
 * @return The port type
 */
rt_daq_port_type_t rt_daq_port_get_type(rt_daq_port_t *rt_daq_port);

/**
 * @brief Returns the port direction of the RT DAQ port
 *
 * @param rt_daq_port The RT DAQ port
 * @return The port direction
 */
rt_daq_port_direction_t rt_daq_port_get_direction(rt_daq_port_t *rt_daq_port);

/**
 * @brief Returns the port channel number
 *
 * @param rt_daq_port The RT DAQ port
 * @return The port channel number
 */
int rt_daq_port_get_channel(rt_daq_port_t *rt_daq_port);

/**
 * @brief Returns the port range
 *
 * @param rt_daq_port The RT DAQ port
 * @return The port range id
 */
int rt_daq_port_get_port_range(rt_daq_port_t *rt_daq_port);

/**
 * @brief Opens a DAQ port (calibrates it)
 *
 * @param rt_daq_port The RT DAQ port
 * @param calibration_data Optional calibration data to be stored
 * @return Result of the operation
 */
rt_result_t rt_daq_port_open(rt_daq_port_t *rt_daq_port,
                             void *calibration_data);

/**
 * @brief Closes a DAQ port
 *
 * @param rt_daq_port The RT DAQ port
 * @return Result of the operation
 */
rt_result_t rt_daq_port_close(rt_daq_port_t *rt_daq_port);

rt_result_t rt_daq_port_get_calibration_data(rt_daq_port_t *rt_daq_port,
                                             void *calibration_data);

/**
 * @brief Checks if the port is opened
 *
 * @param rt_daq_port The RT DAQ port
 * @return true if open, false otherwise
 */
bool rt_daq_port_is_open(rt_daq_port_t *rt_daq_port);

/**
 * @brief Gets last value on port
 *
 * @param rt_daq_port The RT DAQ port
 * @return The RT DAQ value
 */
rt_result_t rt_daq_port_get_last_value(rt_daq_port_t *rt_daq_port);

/**
 * @brief Gets last value on port
 *
 * @param rt_daq_port The RT DAQ port
 * @param n The number of values to select
 * @param number_of_values The number that are actually on the port
 * @return An array of RT DAQ values
 */
rt_result_t rt_daq_port_get_last_n_values(rt_daq_port_t *rt_daq_port, size_t n,
                                          size_t *number_of_values);

#endif // RT_DAQ_PORT_H
