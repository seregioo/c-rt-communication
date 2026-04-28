/**
 * @file rt_daq/daq.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compilation unit for the RT DAQ
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#include "rt_daq/daq.h"
#include "rt_daq/driver.h"
#include "rt_daq/port.h"
#include "rt_daq/port/result.h"
#include "rt_daq/result.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct rt_daq_e {
  rt_daq_attrs_t daq_attrs;

  rt_daq_port_t **daq_ports;

  size_t n_analog_inputs;
  size_t n_analog_outputs;
  size_t n_digital_inputs;
  size_t n_digital_outputs;
  size_t n_digital_inputs_outputs;
  size_t n_total;
};

static size_t rt_daq_get_n_ports(rt_daq_t *rt_daq,
                                 rt_daq_port_type_t ports_type,
                                 rt_daq_port_direction_t ports_direction) {
  switch (ports_type) {
  case RT_DAQ_PORT_TYPE_ANALOG:
    switch (ports_direction) {
    case RT_DAQ_PORT_DIRECTION_INPUT:
      return rt_daq->n_analog_inputs;
      break;
    case RT_DAQ_PORT_DIRECTION_OUTPUT:
      return rt_daq->n_analog_outputs;
      break;
    default:
      break;
    }
    break;
  case RT_DAQ_PORT_TYPE_DIGITAL:
    switch (ports_direction) {
    case RT_DAQ_PORT_DIRECTION_INPUT:
      return rt_daq->n_digital_inputs;
      break;
    case RT_DAQ_PORT_DIRECTION_OUTPUT:
      return rt_daq->n_digital_outputs;
      break;
    case RT_DAQ_PORT_DIRECTION_INPUT_OUTPUT:
      return rt_daq->n_digital_inputs_outputs;
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
  return 0;
}

static size_t
rt_daq_get_position_by_type_and_dir(rt_daq_t *rt_daq,
                                    rt_daq_port_type_t ports_type,
                                    rt_daq_port_direction_t ports_direction) {
  switch (ports_type) {
  case RT_DAQ_PORT_TYPE_ANALOG:
    switch (ports_direction) {
    case RT_DAQ_PORT_DIRECTION_INPUT:
      return 0;
      break;
    case RT_DAQ_PORT_DIRECTION_OUTPUT:
      return rt_daq->n_analog_inputs;
      break;
    default:
      break;
    }
    break;
  case RT_DAQ_PORT_TYPE_DIGITAL:
    switch (ports_direction) {
    case RT_DAQ_PORT_DIRECTION_INPUT:
      return rt_daq->n_analog_inputs + rt_daq->n_analog_outputs;
      break;
    case RT_DAQ_PORT_DIRECTION_OUTPUT:
      return rt_daq->n_analog_inputs + rt_daq->n_analog_outputs +
             rt_daq->n_digital_inputs;
      break;
    case RT_DAQ_PORT_DIRECTION_INPUT_OUTPUT:
      return rt_daq->n_analog_inputs + rt_daq->n_analog_outputs +
             rt_daq->n_digital_inputs + rt_daq->n_digital_outputs;
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
  return 0;
}

static void
rt_daq_get_type_and_dir_from_index(rt_daq_t *rt_daq, size_t index,
                                   rt_daq_port_type_t *port_type,
                                   rt_daq_port_direction_t *port_direction) {

  if (index < rt_daq->n_analog_inputs) {
    *port_type = RT_DAQ_PORT_TYPE_ANALOG;
    *port_direction = RT_DAQ_PORT_DIRECTION_INPUT;
    return;
  } else if (index < rt_daq->n_analog_inputs + rt_daq->n_analog_outputs) {
    *port_type = RT_DAQ_PORT_TYPE_ANALOG;
    *port_direction = RT_DAQ_PORT_DIRECTION_OUTPUT;
    return;
  } else if (index < rt_daq->n_analog_inputs + rt_daq->n_analog_outputs +
                         rt_daq->n_digital_inputs) {
    *port_type = RT_DAQ_PORT_TYPE_DIGITAL;
    *port_direction = RT_DAQ_PORT_DIRECTION_INPUT;
    return;
  } else if (index < rt_daq->n_analog_inputs + rt_daq->n_analog_outputs +
                         rt_daq->n_digital_inputs + rt_daq->n_digital_outputs) {
    *port_type = RT_DAQ_PORT_TYPE_DIGITAL;
    *port_direction = RT_DAQ_PORT_DIRECTION_OUTPUT;
  } else if (index < rt_daq->n_analog_inputs + rt_daq->n_analog_outputs +
                         rt_daq->n_digital_inputs + rt_daq->n_digital_outputs +
                         rt_daq->n_digital_inputs_outputs) {
    *port_type = RT_DAQ_PORT_TYPE_DIGITAL;
    *port_direction = RT_DAQ_PORT_DIRECTION_INPUT_OUTPUT;
  }
}

rt_result_t rt_daq_create(char *rt_daq_name, float freq) {
  rt_daq_t *rt_daq = NULL;
  rt_daq_port_t **daq_ports;
  int analog_in = 0;
  int analog_out = 0;
  int digital_in = 0;
  int digital_out = 0;
  int digital_in_out = 0;
  int all_ports = 0;
  int port_counter = 0;
  size_t rt_daq_name_len = strlen(rt_daq_name);
  rt_result_t create_driver_result;
  rt_result_t create_failure = rt_daq_result_create(
      RT_RESULT_TYPE_FAILURE, RT_DAQ_RESULT_TYPE_CREATE, NULL);

  if (freq <= 0 || rt_daq_name == NULL || rt_daq_name_len <= 0) {
    return create_failure;
  }

  rt_daq = (rt_daq_t *)malloc(sizeof(rt_daq_t));

  if (!rt_daq) {
    return create_failure;
  }

  rt_daq->daq_attrs.freq = freq;

  size_t len_to_copy = RT_DAQ_DRIVER_MAX_NAME < rt_daq_name_len
                           ? RT_DAQ_DRIVER_MAX_NAME
                           : rt_daq_name_len;
  strncpy(rt_daq->daq_attrs.rt_daq_name, rt_daq_name, len_to_copy);

  create_driver_result = rt_daq_driver_create(rt_daq, &rt_daq->daq_attrs);

  if (!rt_result_succeded(&create_driver_result)) {
    free(rt_daq);
    return create_failure;
  }

  all_ports = rt_daq_driver_request_ports(rt_daq, &rt_daq->daq_attrs,
                                          &analog_in, &analog_out, &digital_in,
                                          &digital_out, &digital_in_out);

  if (!all_ports) {
    free(rt_daq);
    return create_failure;
  }

  daq_ports = (rt_daq_port_t **)malloc(sizeof(rt_daq_port_t *) * all_ports);

  if (!daq_ports) {
    free(rt_daq);
    return create_failure;
  }

  rt_daq->n_analog_inputs = (size_t)analog_in;
  rt_daq->n_analog_outputs = (size_t)analog_out;
  rt_daq->n_digital_inputs = (size_t)digital_in;
  rt_daq->n_digital_outputs = (size_t)digital_out;
  rt_daq->n_digital_inputs_outputs = (size_t)digital_in_out;
  rt_daq->n_total = (size_t)all_ports;
  // TODO: maybe make this the way to store all values
  size_t array_ports[5] = {analog_in, analog_out, digital_in, digital_out,
                           digital_in_out};
  int array_port_c = 0;
  int stop_at = 0;
  for (array_port_c = 0; array_port_c < 5; array_port_c++) {
    for (int channel = 0; channel < array_ports[array_port_c];
         port_counter++, channel++) {
      rt_daq_port_type_t port_type = RT_DAQ_PORT_TYPE_INVALID;
      rt_daq_port_direction_t port_direction = RT_DAQ_PORT_DIRECTION_INVALID;
      rt_daq_get_type_and_dir_from_index(rt_daq, port_counter, &port_type,
                                         &port_direction);
      // TODO: change range selection (harcoded now)
      rt_result_t rt_result =
          rt_daq_port_create(port_type, port_direction, (size_t)channel, 0);
      if (!rt_result_succeded(&rt_result)) {
        stop_at = port_counter;
        port_counter = channel;
        array_port_c = 5;
      } else {
        daq_ports[port_counter] =
            (rt_daq_port_t *)rt_result_get_value(&rt_result);
      }
    }
  }

  for (int i = 0; i < stop_at; i++) {
    rt_daq_port_destroy(daq_ports[i]);
  }

  if (stop_at > 0) {
    free(daq_ports);
    free(rt_daq);
    return create_failure;
  }

  rt_daq->daq_ports = daq_ports;

  return create_driver_result;
}

void rt_daq_destroy(rt_daq_t *rt_daq) {
  rt_daq_driver_destroy(&rt_daq->daq_attrs);
  free(rt_daq);
}

rt_result_t rt_daq_query_port(rt_daq_t *rt_daq, rt_daq_port_type_t port_type,
                              rt_daq_port_direction_t port_direction,
                              size_t channel_num) {
  size_t number_of_ports = 0;
  rt_result_t rt_result;
  rt_daq_port_t **rt_daq_ports = NULL;
  size_t position = 0;
  if (!rt_daq) {
    return rt_daq_result_create(RT_RESULT_TYPE_FAILURE, RT_DAQ_RESULT_TYPE_NULL,
                                NULL);
  }
  if (rt_daq->n_total <= 0 || !rt_daq->daq_ports) {
    return rt_daq_result_create(RT_RESULT_TYPE_FAILURE,
                                RT_DAQ_RESULT_TYPE_NO_PORTS, NULL);
  }

  if (port_type <= RT_DAQ_PORT_TYPE_INVALID ||
      port_type >= RT_DAQ_PORT_TYPE_MAX_VALUE) {
    return rt_daq_port_result_create(RT_RESULT_TYPE_FAILURE,
                                     RT_DAQ_PORT_RESULT_TYPE_BAD_TYPE, NULL);
  }
  if (port_direction <= RT_DAQ_PORT_DIRECTION_INVALID ||
      port_direction >= RT_DAQ_PORT_DIRECTION_MAX_VALUE) {
    return rt_daq_port_result_create(
        RT_RESULT_TYPE_FAILURE, RT_DAQ_PORT_RESULT_TYPE_BAD_DIRECTION, NULL);
  }

  position =
      rt_daq_get_position_by_type_and_dir(rt_daq, port_type, port_direction);

  number_of_ports = rt_daq_get_n_ports(rt_daq, port_type, port_direction);

  if (channel_num > (number_of_ports - 1)) {
    return rt_daq_result_create(RT_RESULT_TYPE_FAILURE,
                                RT_DAQ_RESULT_TYPE_NO_PORTS, NULL);
  }

  return rt_daq_port_result_create(RT_RESULT_TYPE_SUCCESS,
                                   RT_DAQ_PORT_RESULT_TYPE_QUERY,
                                   rt_daq->daq_ports[position + channel_num]);
}

rt_result_t rt_daq_open_port(rt_daq_t *rt_daq, rt_daq_port_t *rt_port) {
  return rt_daq_driver_open_port(rt_daq, &rt_daq->daq_attrs, rt_port);
}

rt_result_t rt_daq_close_port(rt_daq_t *rt_daq, rt_daq_port_t *rt_port) {
  return rt_daq_driver_close_port(rt_daq, &rt_daq->daq_attrs, rt_port);
}

rt_result_t rt_daq_read(rt_daq_t *rt_daq, rt_daq_port_t *rt_port,
                        double *readed_data) {
  return rt_daq_driver_read(rt_daq, &rt_daq->daq_attrs, rt_port, readed_data);
}

rt_result_t rt_daq_write(rt_daq_t *rt_daq, rt_daq_port_t *rt_port,
                         double voltage_v) {
  return rt_daq_driver_write(rt_daq, &rt_daq->daq_attrs, rt_port, voltage_v);
}
