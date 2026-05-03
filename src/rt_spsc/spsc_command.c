/**
 * @file spsc_command.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compilation unit for the SPSC Command
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */

#include "rt_spsc/spsc_command.h"

void rt_spsc_command_init(rt_spsc_command_queue_t *q) {
  __atomic_store_n(&q->head, 0, __ATOMIC_RELAXED);
  __atomic_store_n(&q->tail, 0, __ATOMIC_RELAXED);
}

bool rt_spsc_command_try_push(rt_spsc_command_queue_t *q,
                              const rt_spsc_command_msg_t *msg) {
  size_t head = __atomic_load_n(&q->head, __ATOMIC_RELAXED);
  size_t tail = __atomic_load_n(&q->tail, __ATOMIC_ACQUIRE);

  if ((head - tail) >= SPSC_COMMAND_CAPACITY) {
    return false; // queue full
  }

  q->buffer[head & (SPSC_COMMAND_CAPACITY - 1)] = *msg;

  __atomic_store_n(&q->head, head + 1, __ATOMIC_RELEASE);
  return true;
}

bool rt_spsc_command_try_pop(rt_spsc_command_queue_t *q,
                             rt_spsc_command_msg_t *msg) {
  size_t tail = __atomic_load_n(&q->tail, __ATOMIC_RELAXED);
  size_t head = __atomic_load_n(&q->head, __ATOMIC_ACQUIRE);

  if (tail == head) {
    return false; // queue empty
  }

  *msg = q->buffer[tail & (SPSC_COMMAND_CAPACITY - 1)];

  __atomic_store_n(&q->tail, tail + 1, __ATOMIC_RELEASE);
  return true;
}
