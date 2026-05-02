/**
 * @file main.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Main file for the RealTime communication program
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#include "hindmarsh_rose.h"
#define _GNU_SOURCE
#include "rt_daq/daq.h"
#include "rt_daq/port.h"
#include "rt_result/result.h"
#include "rt_spsc/spsc_command.h"
#include "rt_spsc/spsc_telemetry.h"
#include "rt_thread/thread.h"
#include <inttypes.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct rt_communication_args_e {
  rt_spsc_command_queue_t *command_queue;
  rt_spsc_telemetry_queue_t *telemetry_queue;
  atomic_bool *done;
} rt_communication_args_t;

#define MAX_PLUGINS 3

static void *main_loop(void *thread_args) {
  rt_communication_args_t *rt_communication_args =
      (rt_communication_args_t *)thread_args;

  rt_spsc_telemetry_msg_t telemetry_messages[3];
  rt_spsc_command_msg_t command_message;
  rt_spsc_command_queue_t *command_queue = rt_communication_args->command_queue;
  rt_spsc_telemetry_queue_t *telemetry_queue =
      rt_communication_args->telemetry_queue;
  atomic_bool *done = rt_communication_args->done;
  uint64_t time_start = 0;
  uint64_t after_exec = 0;
  uint64_t after_write = 0;
  uint64_t after_read = 0;
  uint64_t glob_seq = 0;

  uint64_t period = 100000; // ns (100 us)

  uint64_t goal = period * 100000 * 60; // time to goal
  uint64_t counter = 0;
  int64_t time_passed = 0;
  int64_t time_after_send = 0;
  int64_t time_to_sleep = 0;
  rt_daq_t *daq = NULL;
  rt_daq_port_t *i7 = NULL;
  rt_daq_port_t *o0 = NULL;
  rt_result_t result = rt_daq_create("/dev/comedi0", 100.0);
  double readed = 0.0;

  HRParams params = hr_default_params();
  HRState state = {.x = -1.6, .y = 0.0, .z = 0.0};
  // double dt = 0.01;
  double dt = 0.0015;

  if (!rt_result_succeded(&result)) {
    printf("%s\n", rt_result_get_string(&result));
    exit(1);
  }

  daq = (rt_daq_t *)rt_result_get_value(&result);

  result = rt_daq_query_port(daq, RT_DAQ_PORT_TYPE_ANALOG,
                             RT_DAQ_PORT_DIRECTION_INPUT, 7);
  if (!rt_result_succeded(&result)) {
    printf("%s\n", rt_result_get_string(&result));
    exit(1);
  }
  i7 = (rt_daq_port_t *)rt_result_get_value(&result);

  result = rt_daq_query_port(daq, RT_DAQ_PORT_TYPE_ANALOG,
                             RT_DAQ_PORT_DIRECTION_OUTPUT, 0);
  if (!rt_result_succeded(&result)) {
    printf("%s\n", rt_result_get_string(&result));
    exit(1);
  }
  o0 = (rt_daq_port_t *)rt_result_get_value(&result);

  result = rt_daq_open_port(daq, i7);
  if (!rt_result_succeded(&result)) {
    printf("%s\n", rt_result_get_string(&result));
    exit(1);
  }

  result = rt_daq_open_port(daq, o0);
  if (!rt_result_succeded(&result)) {
    printf("%s\n", rt_result_get_string(&result));
    exit(1);
  }

  while (true) {
    time_start = rt_thread_get_time();

    // 1. model execution
    state = hr_rk4_step(state, params, dt);

    after_exec = rt_thread_get_time();
    // 2. daq transmission

    result = rt_daq_write(daq, o0, state.x);
    if (!rt_result_succeded(&result)) {
      printf("%s\n", rt_result_get_string(&result));
      exit(1);
    }

    after_write = rt_thread_get_time();

    // 3. daq read
    result = rt_daq_read(daq, i7, &readed);
    if (!rt_result_succeded(&result)) {
      printf("%s\n", rt_result_get_string(&result));
      exit(1);
    }
    after_read = rt_thread_get_time();

    // 3. data transmission
    // rt_plugin_get_output_ports(plugin)

    telemetry_messages[0].type =
        RT_SPSC_TELEMETRY_MSG_TYPE_PLUGIN_VARIABLES_DATA;
    telemetry_messages[0].seq = glob_seq;
    telemetry_messages[0].timestamp_ns = after_read;
    telemetry_messages[0].data.plugin_variables_data.plugin_id = 1;
    telemetry_messages[0].data.plugin_variables_data.values[0] =
        state.x; // = value daq write
    telemetry_messages[0].data.plugin_variables_data.values[1] =
        readed; // = value daq read
    telemetry_messages[0].data.plugin_variables_data.values[2] =
        (double)time_start;
    telemetry_messages[0].data.plugin_variables_data.values[3] =
        (double)after_exec;
    telemetry_messages[0].data.plugin_variables_data.values[4] =
        (double)after_write;
    telemetry_messages[0].data.plugin_variables_data.values[5] =
        (double)after_read;
    telemetry_messages[0].data.plugin_variables_data.values[6] =
        (double)time_after_send;

    glob_seq++;

    rt_spsc_telemetry_try_push(telemetry_queue, &telemetry_messages[0]);

    // 3. order dispatch
    if (rt_spsc_command_try_pop(command_queue, &command_message)) {
      switch (command_message.type) {
      case RT_SPSC_COMMAND_MSG_TYPE_GLOBAL_COMMAND:
        rt_daq_destroy(daq);
        return NULL;
        break;
      default:
        break;
      }
    }

    counter += period;

    if (counter >= goal) {
      rt_daq_destroy(daq);
      atomic_store(done, true);
      return NULL;
    }

    time_after_send = rt_thread_get_time();
    time_passed = time_after_send - time_start;
    time_to_sleep = period - time_passed;
    if (time_passed > 0 && time_to_sleep > 0) {
      rt_thread_sleep_until(time_to_sleep);
    }
  }

  return NULL;
}

int main() {
  rt_result_t result;
  rt_thread_t *thread = NULL;
  cpu_set_t cpuset;
  rt_communication_args_t comm_args;
  rt_spsc_command_queue_t command_queue;
  rt_spsc_telemetry_queue_t telemetry_queue;
  rt_spsc_telemetry_msg_t telemetry_message;
  atomic_bool worker_done = false;
  FILE *fptr;
  fptr = fopen("results-c.csv", "w");
  fprintf(fptr,
          "x_trans,x_recv,time_at_begin_ns,time_after_op_ns,time_after_"
          "send_ns,time_after_receive_ns,time_after_send_to_main_thread_ns\n");
  fclose(fptr);
  fptr = fopen("results-c.csv", "a");

  rt_spsc_telemetry_init(&telemetry_queue);
  rt_spsc_command_init(&command_queue);

  comm_args.telemetry_queue = &telemetry_queue;
  comm_args.command_queue = &command_queue;
  comm_args.done = &worker_done;

  CPU_ZERO(&cpuset);
  result = rt_thread_create(94, PTHREAD_STACK_MIN, SCHED_FIFO,
                            PTHREAD_EXPLICIT_SCHED, cpuset, false);

  if (!rt_result_succeded(&result)) {
    printf("%s\n", rt_result_get_string(&result));
    exit(1);
  }
  thread = rt_result_get_value(&result);

  result = rt_thread_run(thread, main_loop, &comm_args);

  if (!rt_result_succeded(&result)) {
    printf("%s\n", rt_result_get_string(&result));
    exit(1);
  }

  while (true) {
    if (rt_spsc_telemetry_try_pop(&telemetry_queue, &telemetry_message)) {
      switch (telemetry_message.type) {
      case RT_SPSC_TELEMETRY_MSG_TYPE_PLUGIN_VARIABLES_DATA:
        if (telemetry_message.data.plugin_variables_data.plugin_id == 1) {
          fprintf(
              fptr,
              "%f,%f"
              ",%" PRIu64 ",%" PRIu64 ",%" PRIu64 ",%" PRIu64 ",%" PRIu64 "\n",
              telemetry_message.data.plugin_variables_data.values[0],
              telemetry_message.data.plugin_variables_data.values[1],
              (uint64_t)telemetry_message.data.plugin_variables_data.values[2],
              (uint64_t)telemetry_message.data.plugin_variables_data.values[3],
              (uint64_t)telemetry_message.data.plugin_variables_data.values[4],
              (uint64_t)telemetry_message.data.plugin_variables_data.values[5],
              (uint64_t)telemetry_message.data.plugin_variables_data.values[6]);
        }
        break;
      default:
        break;
      }
    } else if (atomic_load(&worker_done)) {
      rt_thread_join(thread, NULL);
      rt_thread_destroy(thread);
      fclose(fptr);
      exit(0);
    }
  }

  rt_thread_join(thread, NULL);
  rt_thread_destroy(thread);
  fclose(fptr);
}
