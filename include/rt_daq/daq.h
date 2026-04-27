/**
 * @file rt_daq/daq.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for the RT DAQ
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#ifndef RT_DAQ_H
#define RT_DAQ_H

#include "rt_daq/port.h"
#include "rt_result/result.h"
#include <stddef.h>

typedef struct rt_daq_e rt_daq_t;

/**
 * @brief Creates a RT DAQ
 *
 * @param rt_daq_name The RT DAQ name
 * @param freq The frequency of the DAQ to sample data
 * @return The RT DAQ
 */
rt_result_t rt_daq_create(char *rt_daq_name, float freq);

/**
 * @brief Destroys a RT DAQ
 *
 * @param rt_daq The RT DAQ
 */
void rt_daq_destroy(rt_daq_t *rt_daq);

/**
 * @brief Gets ports of the DAQ
 *
 * @param rt_daq The DAQ from which we need to retrieve the digital ports
 * @param port_type The port type
 * @param port_direction The port direction
 * @param channel_num
 * @return a `rt_result_t`, that will be containing a `rt_port_t` or
 * NULL if error
 */
rt_result_t rt_daq_query_port(rt_daq_t *rt_daq, rt_daq_port_type_t port_type,
                              rt_daq_port_direction_t port_direction,
                              size_t channel_num);
/**
 * @brief Opens a daq port
 *
 * @param rt_daq The DAQ
 * @param rt_port The DAQ port
 * @return a `rt_result_t`, that will be containing a
 * `rt_daq_port_t` or NULL if error
 */
rt_result_t rt_daq_open_port(rt_daq_t *rt_daq, rt_daq_port_t *rt_port);

/**
 * @brief Closes a daq port
 *
 * @param rt_daq The DAQ
 * @param rt_port The DAQ port
 * @return a `rt_result_t`, that will be containing a `rt_daq_port_t` or NULL
 * if error
 */
rt_result_t rt_daq_close_port(rt_daq_t *rt_daq, rt_daq_port_t *rt_port);

/**
 * @brief Reads a value inside a port
 *
 * @param rt_daq The DAQ
 * @param rt_port The DAQ port
 * @param readed_data The readed data pointer to be filled
 * @return a `rt_result_t`, that will be containing a `rt_daq_t` or NULL
 * if error
 */
rt_result_t rt_daq_read(rt_daq_t *rt_daq, rt_daq_port_t *rt_port,
                        double *readed_data);

/**
 * @brief Reads a value inside a port
 *
 * @param rt_daq The DAQ
 * @param rt_port The DAQ port
 * @param voltage_v The voltage in volts
 * @return a `rt_result_t`, that will be containing a `rt_daq_value_t` or NULL
 * if error
 */
rt_result_t rt_daq_write(rt_daq_t *rt_daq, rt_daq_port_t *rt_port,
                         double voltage_v);

#endif // RT_DAQ_H
