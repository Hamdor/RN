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
#include "stdint.h"

namespace rna1 {

struct server_options {
  server_options(uint16_t port, uint8_t fps)
      : m_port(port), m_fps(fps) {
    // nop
  }
  uint16_t m_port;
  uint8_t  m_fps;
 private:
  server_options();
};

class server_impl : public thread_t {
 protected:
  void* exec(void*);
};

} // namespace rna1

#endif // RNA1_SERVER_IMPL_HPP
