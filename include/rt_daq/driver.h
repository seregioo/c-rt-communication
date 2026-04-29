/**
 * @file rt_daq/driver.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for the RT DAQ driver
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#ifndef RT_DAQ_DRIVER_H
#define RT_DAQ_DRIVER_H

#include "rt_daq/daq.h"
#include "rt_daq/port.h"
#include "rt_result/result.h"
#include <comedilib.h>
#include <stddef.h>

#define RT_DAQ_DRIVER_MAX_NAME 1024

typedef struct rt_daq_attrs_e {
  char rt_daq_name[RT_DAQ_DRIVER_MAX_NAME];
  float freq;
  void *daq_ptr;
  void *driver_data_ptr;
} rt_daq_attrs_t;

/**
 * @brief Creates a RT DAQ driver
 *
 * @return The RT DAQ driver atributes
 */
rt_result_t rt_daq_driver_create(rt_daq_t *rt_daq,
                                 rt_daq_attrs_t *rt_daq_attrs);

/**
 * @brief Destroys a RT DAQ driver attributes
 *
 * @param rt_daq The RT DAQ driver to be destroyed
 */
void rt_daq_driver_destroy(rt_daq_attrs_t *rt_daq_attrs);

/**
 * @brief Requests the ports of the daq
 *
 * @param rt_daq The RT DAQ
 * @param rt_daq_attrs The RT DAQ attributes
 * @return the sum of all ports and all individual quantity of channels
 */
int rt_daq_driver_request_ports(rt_daq_t *rt_daq, rt_daq_attrs_t *rt_daq_attrs,
                                int *analog_in, int *analog_out,
                                int *digital_in, int *digital_out,
                                int *digital_in_out);
/**
 * @brief Opens a daq port
 *
 * @param rt_daq The DAQ
 * @param rt_port The DAQ port
 * @return a `rt_result_t`, that will be containing a
 * `rt_daq_port_t` or NULL if error
 */
rt_result_t rt_daq_driver_open_port(rt_daq_t *rt_daq,
                                    rt_daq_attrs_t *rt_daq_attrs,
                                    rt_daq_port_t *rt_port);

/**
 * @brief Closes a daq port
 *
 * @param rt_daq The DAQ
 * @param rt_port The DAQ port
 * @return a `rt_result_t`, that will be containing a `rt_daq_port_t` or NULL
 * if error
 */
rt_result_t rt_daq_driver_close_port(rt_daq_t *rt_daq,
                                     rt_daq_attrs_t *rt_daq_attrs,
                                     rt_daq_port_t *rt_port);

/**
 * @brief Reads a value inside a port
 *
 * @param rt_daq The DAQ
 * @param rt_port The DAQ port
 * @param readed_data The readed data pointer to be filled
 * @return a `rt_result_t`, that will be containing a `rt_daq_t` or NULL
 * if error
 */
rt_result_t rt_daq_driver_read(rt_daq_t *rt_daq, rt_daq_attrs_t *rt_daq_attrs,
                               rt_daq_port_t *rt_port, double *readed_data);

/**
 * @brief Reads a value inside a port
 *
 * @param rt_daq The DAQ
 * @param rt_port The DAQ port
 * @param voltage_v The voltage in volts
 * @return a `rt_result_t`, that will be containing a `rt_daq_value_t` or NULL
 * if error
 */
rt_result_t rt_daq_driver_write(rt_daq_t *rt_daq, rt_daq_attrs_t *rt_daq_attrs,
                                rt_daq_port_t *rt_port, double voltage_v);

#endif // RT_DAQ_DRIVER_H
