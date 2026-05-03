/**
 * @file spsc_telemetry.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for the SPSC Telemetry
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RT_TELEMETRY_SPSC_H
#define RT_TELEMETRY_SPSC_H

#define SPSC_TELEMETRY_CAPACITY 1024

#include "spsc_telemetry/telemetry_msg.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct rt_spsc_telemetry_queue_e {
  rt_spsc_telemetry_msg_t buffer[SPSC_TELEMETRY_CAPACITY];

  size_t head; // written by producer, read by consumer
  size_t tail; // written by consumer, read by producer
} rt_spsc_telemetry_queue_t;

void rt_spsc_telemetry_init(rt_spsc_telemetry_queue_t *q);

bool rt_spsc_telemetry_try_push(rt_spsc_telemetry_queue_t *q,
                                const rt_spsc_telemetry_msg_t *msg);

bool rt_spsc_telemetry_try_pop(rt_spsc_telemetry_queue_t *q,
                               rt_spsc_telemetry_msg_t *msg);

#endif // RT_TELEMETRY_SPSC_H
