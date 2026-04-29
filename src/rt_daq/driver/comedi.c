/**
 * @file rt_daq/driver/comedi.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compilation unit for the comedi driver of the
 * RT DAQ
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#include "rt_daq/driver.h"
#include "rt_daq/port.h"
#include "rt_daq/port/result.h"
#include "rt_daq/result.h"
#include "rt_result/result.h"
#include <comedi.h>
#include <comedilib.h>
#include <stdlib.h>

typedef struct rt_daq_driver_subdev_aggregation_e {
  rt_daq_port_type_t port_type;
  rt_daq_port_direction_t port_direction;
  enum comedi_subdevice_type comedi_subdev;
  int index_subdev;
} rt_daq_driver_subdev_aggregation_t;

typedef struct rt_daq_driver_data_e {
  rt_daq_driver_subdev_aggregation_t *subdev_array;
  size_t n_subdev;
  comedi_calibration_t *cal;
} rt_daq_driver_data_t;

typedef struct rt_daq_driver_port_calibration_data_e {
  comedi_polynomial_t to_converter;
  comedi_polynomial_t from_converter;
} rt_daq_driver_port_calibration_data_t;

/**
 * @brief Translate the structure type and direction to the driver one
 *
 * @param data_attrs The daq attrs
 * @param port_type The port type
 * @param port_direction The direction
 * @return the driver data aggregation
 */
static rt_daq_driver_subdev_aggregation_t
rt_daq_driver_translate_to_subdev(rt_daq_attrs_t *daq_attrs,
                                  rt_daq_port_type_t port_type,
                                  rt_daq_port_direction_t port_direction) {

  rt_daq_driver_data_t *driver_data =
      (rt_daq_driver_data_t *)daq_attrs->driver_data_ptr;

  int i = 0;
  while (i < driver_data->n_subdev) {
    ;
    if (driver_data->subdev_array[i].port_type == port_type &&
        driver_data->subdev_array[i].port_direction == port_direction) {
      return driver_data->subdev_array[i];
    }
    i++;
  }

  return driver_data->subdev_array[0];
}

rt_result_t rt_daq_driver_create(rt_daq_t *rt_daq,
                                 rt_daq_attrs_t *rt_daq_attrs) {
  comedi_t *dev;
  rt_result_t error_result = rt_daq_result_create(
      RT_RESULT_TYPE_FAILURE, RT_DAQ_RESULT_TYPE_CREATE, NULL);
  rt_daq_driver_data_t *driver_data =
      (rt_daq_driver_data_t *)malloc(sizeof(rt_daq_driver_data_t));

  if (!driver_data) {
    return error_result;
  }

  dev = comedi_open(rt_daq_attrs->rt_daq_name);
  if (!dev) {
    free(driver_data);
    return error_result;
  }

  int n_subdev = comedi_get_n_subdevices(dev);
  driver_data->subdev_array = (rt_daq_driver_subdev_aggregation_t *)malloc(
      sizeof(rt_daq_driver_subdev_aggregation_t) * n_subdev);

  char *path = comedi_get_default_calibration_path(dev);
  driver_data->cal = comedi_parse_calibration_file(path);

  if (!driver_data->cal) {
    free(driver_data->subdev_array);
    free(driver_data);
    comedi_close(dev);
    return rt_daq_result_create(RT_RESULT_TYPE_FAILURE,
                                RT_DAQ_RESULT_TYPE_CALIBRATION_NOT_FOUND, NULL);
    ;
  }

  if (!driver_data->subdev_array) {
    comedi_cleanup_calibration(driver_data->cal);
    free(driver_data->subdev_array);
    free(driver_data);
    comedi_close(dev);
    return error_result;
  }
  rt_daq_attrs->driver_data_ptr = driver_data;
  rt_daq_attrs->daq_ptr = dev;

  rt_result_destroy(error_result);

  return rt_daq_result_create(RT_RESULT_TYPE_SUCCESS, RT_DAQ_RESULT_TYPE_CREATE,
                              rt_daq);
}

void rt_daq_driver_destroy(rt_daq_attrs_t *rt_daq_attrs) {
  rt_daq_driver_data_t *driver_data =
      (rt_daq_driver_data_t *)rt_daq_attrs->driver_data_ptr;

  if (!driver_data) {
    if (rt_daq_attrs->daq_ptr) {
      comedi_close((comedi_t *)rt_daq_attrs->daq_ptr);
    }
    return;
  }

  comedi_cleanup_calibration(driver_data->cal);
  free(driver_data->subdev_array);
  free(driver_data);
  comedi_close((comedi_t *)rt_daq_attrs->daq_ptr);
}

rt_result_t rt_daq_driver_open_port(rt_daq_t *rt_daq,
                                    rt_daq_attrs_t *rt_daq_attrs,
                                    rt_daq_port_t *rt_port) {
  comedi_t *dev = (comedi_t *)rt_daq_attrs->daq_ptr;
  rt_daq_driver_port_calibration_data_t *calibration = NULL;
  rt_daq_driver_subdev_aggregation_t agg = rt_daq_driver_translate_to_subdev(
      rt_daq_attrs, rt_daq_port_get_type(rt_port),
      rt_daq_port_get_direction(rt_port));
  rt_daq_driver_data_t *driver_data =
      (rt_daq_driver_data_t *)rt_daq_attrs->driver_data_ptr;
  int subdev = agg.index_subdev;
  int chan = rt_daq_port_get_channel(rt_port);
  int range = rt_daq_port_get_port_range(rt_port);

  calibration = (rt_daq_driver_port_calibration_data_t *)malloc(
      sizeof(rt_daq_driver_port_calibration_data_t));

  if (!calibration) {
    return rt_daq_port_result_create(RT_RESULT_TYPE_FAILURE,
                                     RT_DAQ_PORT_RESULT_TYPE_OPEN, NULL);
  }

  if (rt_daq_port_get_direction(rt_port) == RT_DAQ_PORT_DIRECTION_INPUT ||
      rt_daq_port_get_direction(rt_port) ==
          RT_DAQ_PORT_DIRECTION_INPUT_OUTPUT) {

    if (comedi_get_softcal_converter(subdev, chan, range, COMEDI_TO_PHYSICAL,
                                     driver_data->cal,
                                     &calibration->to_converter) < 0) {
      free(calibration);
      return rt_daq_port_result_create(RT_RESULT_TYPE_FAILURE,
                                       RT_DAQ_RESULT_TYPE_SOFTCALL_FAIL, NULL);
    }
  }

  if (rt_daq_port_get_direction(rt_port) == RT_DAQ_PORT_DIRECTION_OUTPUT ||
      rt_daq_port_get_direction(rt_port) ==
          RT_DAQ_PORT_DIRECTION_INPUT_OUTPUT) {
    if (comedi_get_softcal_converter(subdev, chan, range, COMEDI_FROM_PHYSICAL,
                                     driver_data->cal,
                                     &calibration->from_converter) < 0) {
      free(calibration);
      return rt_daq_port_result_create(RT_RESULT_TYPE_FAILURE,
                                       RT_DAQ_RESULT_TYPE_SOFTCALL_FAIL, NULL);
    }
  }

  return rt_daq_port_open(rt_port, calibration);
}

rt_result_t rt_daq_driver_close_port(rt_daq_t *rt_daq,
                                     rt_daq_attrs_t *rt_daq_attrs,
                                     rt_daq_port_t *rt_port) {
  comedi_t *dev = (comedi_t *)rt_daq_attrs->daq_ptr;
  rt_result_t calibration_result;
  rt_daq_driver_port_calibration_data_t *calibration = NULL;

  calibration_result = rt_daq_port_get_calibration_data(rt_port, calibration);
  if (rt_result_succeded(&calibration_result)) {
    calibration = rt_result_get_value(&calibration_result);
    free(calibration);
  }
  rt_result_t rt_result = rt_daq_port_close(rt_port);
  return rt_result;
}

rt_result_t rt_daq_driver_read(rt_daq_t *rt_daq, rt_daq_attrs_t *rt_daq_attrs,
                               rt_daq_port_t *rt_port, double *readed_value) {
  comedi_t *dev = (comedi_t *)rt_daq_attrs->daq_ptr;
  lsampl_t data = 0;
  rt_daq_driver_port_calibration_data_t *calibration = NULL;
  double phys_data = 0.0;
  rt_daq_driver_subdev_aggregation_t agg = rt_daq_driver_translate_to_subdev(
      rt_daq_attrs, rt_daq_port_get_type(rt_port),
      rt_daq_port_get_direction(rt_port));
  int subdev = agg.index_subdev;
  int chan = rt_daq_port_get_channel(rt_port);
  int aref = AREF_GROUND; // reference on struct
  int port_range = rt_daq_port_get_port_range(rt_port);

  if (!rt_daq_port_is_open(rt_port)) {
    return rt_daq_result_create(RT_RESULT_TYPE_ERROR, RT_DAQ_RESULT_TYPE_READ,
                                NULL);
  }

  if (comedi_data_read(dev, subdev, chan, port_range, aref, &data) < 0) {
    return rt_daq_result_create(RT_RESULT_TYPE_FAILURE, RT_DAQ_RESULT_TYPE_READ,
                                NULL);
  }

  rt_result_t rt_result_cal =
      rt_daq_port_get_calibration_data(rt_port, calibration);

  if (!rt_result_succeded(&rt_result_cal)) {
    return rt_result_cal;
  }

  calibration = (rt_daq_driver_port_calibration_data_t *)rt_result_get_value(
      &rt_result_cal);

  // phys_data = comedi_to_phys(data, calibration->range, calibration->maxdata);

  phys_data = comedi_to_physical(data, &calibration->to_converter);
  *readed_value = phys_data;
  return rt_daq_result_create(RT_RESULT_TYPE_SUCCESS, RT_DAQ_RESULT_TYPE_READ,
                              rt_daq);
}

rt_result_t rt_daq_driver_write(rt_daq_t *rt_daq, rt_daq_attrs_t *rt_daq_attrs,
                                rt_daq_port_t *rt_port, double voltage_v)

{

  comedi_t *dev = (comedi_t *)rt_daq_attrs->daq_ptr;
  rt_daq_driver_port_calibration_data_t *calibration = NULL;
  lsampl_t data = 0;

  double phys_data = 0.0;
  rt_daq_driver_subdev_aggregation_t agg = rt_daq_driver_translate_to_subdev(
      rt_daq_attrs, rt_daq_port_get_type(rt_port),
      rt_daq_port_get_direction(rt_port));
  int subdev = agg.index_subdev;
  int chan = rt_daq_port_get_channel(rt_port);
  int aref = AREF_GROUND; // reference on struct
  int port_range = rt_daq_port_get_port_range(rt_port);

  rt_result_t rt_result_cal =
      rt_daq_port_get_calibration_data(rt_port, calibration);

  if (!rt_result_succeded(&rt_result_cal)) {
    return rt_result_cal;
  }

  calibration = (rt_daq_driver_port_calibration_data_t *)rt_result_get_value(
      &rt_result_cal);

  // data = comedi_from_phys(voltage_v, calibration->range,
  // calibration->maxdata);
  data = comedi_from_physical(voltage_v, &calibration->from_converter);

  if (comedi_data_write(dev, subdev, chan, port_range, aref, data) < 0) {
    return rt_daq_result_create(RT_RESULT_TYPE_ERROR, RT_DAQ_RESULT_TYPE_WRITE,
                                rt_daq);
  }
  return rt_daq_result_create(RT_RESULT_TYPE_SUCCESS, RT_DAQ_RESULT_TYPE_WRITE,
                              rt_daq);
}

int rt_daq_driver_request_ports(rt_daq_t *rt_daq, rt_daq_attrs_t *rt_daq_attrs,
                                int *analog_in, int *analog_out,
                                int *digital_in, int *digital_out,
                                int *digital_in_out) {

  comedi_t *dev = (comedi_t *)rt_daq_attrs->daq_ptr;
  rt_daq_driver_data_t *driver_data =
      (rt_daq_driver_data_t *)rt_daq_attrs->driver_data_ptr;
  int subdev = comedi_get_n_subdevices(dev);
  driver_data->n_subdev = subdev;
  for (int s = 0, i = 0; s < subdev; s++) {
    int type = comedi_get_subdevice_type(dev, s);
    int chans = comedi_get_n_channels(dev, s);

    switch (type) {
    case COMEDI_SUBD_AI:
      driver_data->subdev_array[i].comedi_subdev = COMEDI_SUBD_AI;
      driver_data->subdev_array[i].index_subdev = s;
      driver_data->subdev_array[i].port_type = RT_DAQ_PORT_TYPE_ANALOG;
      driver_data->subdev_array[i].port_direction = RT_DAQ_PORT_DIRECTION_INPUT;
      i++;
      *analog_in += chans;
      break;

    case COMEDI_SUBD_AO:
      driver_data->subdev_array[i].comedi_subdev = COMEDI_SUBD_AO;
      driver_data->subdev_array[i].index_subdev = s;
      driver_data->subdev_array[i].port_type = RT_DAQ_PORT_TYPE_ANALOG;
      driver_data->subdev_array[i].port_direction =
          RT_DAQ_PORT_DIRECTION_OUTPUT;
      i++;
      *analog_out += chans;
      break;

    case COMEDI_SUBD_DI:
      driver_data->subdev_array[i].comedi_subdev = COMEDI_SUBD_DI;
      driver_data->subdev_array[i].index_subdev = s;
      driver_data->subdev_array[i].port_type = RT_DAQ_PORT_TYPE_DIGITAL;
      driver_data->subdev_array[i].port_direction = RT_DAQ_PORT_DIRECTION_INPUT;
      i++;
      *digital_in += chans;
      break;

    case COMEDI_SUBD_DO:
      driver_data->subdev_array[i].comedi_subdev = COMEDI_SUBD_DO;
      driver_data->subdev_array[i].index_subdev = s;
      driver_data->subdev_array[i].port_type = RT_DAQ_PORT_TYPE_DIGITAL;
      driver_data->subdev_array[i].port_direction =
          RT_DAQ_PORT_DIRECTION_OUTPUT;
      i++;
      *digital_out += chans;
      break;

    case COMEDI_SUBD_DIO:
      driver_data->subdev_array[i].comedi_subdev = COMEDI_SUBD_DIO;
      driver_data->subdev_array[i].index_subdev = s;
      driver_data->subdev_array[i].port_type = RT_DAQ_PORT_TYPE_DIGITAL;
      driver_data->subdev_array[i].port_direction =
          RT_DAQ_PORT_DIRECTION_INPUT_OUTPUT;
      i++;
      *digital_in_out += chans;
      break;
    default:
      break;
    }
  }
  return *analog_in + *analog_out + *digital_in + *digital_out +
         *digital_in_out;
}
