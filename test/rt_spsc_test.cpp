/**
 * @file rt_spsc_test.cpp
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Test file for the SPSC queue
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

extern "C" {
#include "rt_spsc/spsc_command.h"
}

TEST(RtResultTest, MultiThreadedAccess) {

  std::thread thread;
  rt_spsc_command_queue_t *queue =
      (rt_spsc_command_queue_t *)malloc(sizeof(rt_spsc_command_queue_t));
  rt_spsc_command_msg_t msg;
  rt_spsc_command_init(queue);

  thread = std::thread([queue]() {
    rt_spsc_command_msg_t msg;

    msg.timestamp_ns = 1;
    msg.seq = 1;
    rt_spsc_command_try_push(queue, (const rt_spsc_command_msg_t *)&msg);
  });

  while (!rt_spsc_command_try_pop(queue, &msg)) {
    std::this_thread::sleep_for(std::chrono::microseconds(10));
  }
  thread.join();

  free(queue);

  EXPECT_EQ(msg.timestamp_ns, 1);
  EXPECT_EQ(msg.seq, 1);
}
