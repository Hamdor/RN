/******************************************************************************
 *                     ____    __  __      ______     _                       *
 *                    /\  _`\ /\ \/\ \    /\  _  \  /' \                      *
 *                    \ \ \L\ \ \ `\\ \   \ \ \L\ \/\_, \                     *
 *                     \ \ ,  /\ \ , ` \   \ \  __ \/_/\ \                    *
 *                      \ \ \\ \\ \ \`\ \   \ \ \/\ \ \ \ \                   *
 *                       \ \_\ \_\ \_\ \_\   \ \_\ \_\ \ \_\                  *
 *                        \/_/\/ /\/_/\/_/    \/_/\/_/  \/_/                  *
 *                                                                            *
 * Copyright (C) 2014                                                         *
 * Marian Triebe  <marian.triebe  (at) haw-hamburg.de>                        *
 * Katja Kirstein <katja.kirstein (at) haw-hamburg.de>                        *
 *                                                                            *
 ******************************************************************************/

#ifndef RNA1_SERVER_IMPL_HPP
#define RNA1_SERVER_IMPL_HPP

#include "thread_t.hpp"
#include "mutex_t.hpp"
#include "stdint.h"

namespace rna1 {

struct server_options {
  server_options(uint16_t port, uint16_t max_clients)
      : m_port(port), m_clients(max_clients) {
    // nop
  }
  uint16_t m_port;
  uint16_t m_clients;
};

class server_impl : public thread_t {
  static uint16_t s_curr_clients;
  static mutex_t  s_lock;
 public:
  /**
   * Called from an `worker_impl` which is about to exit.
   * this will decrease the s_curr_clients count.
   **/
  static void notify_death();
 protected:
  /**
   * Main method of the thread
   * @param  arg is a pointer to an argument
   * @return a pointer to a return code or NULL
   **/
  void* exec(void* arg);
};

} // namespace rna1

#endif // RNA1_SERVER_IMPL_HPP
