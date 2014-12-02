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

#ifndef RNA1_FETCH_IMPL_HPP
#define RNA1_FETCH_IMPL_HPP

#include "thread_t.hpp"
#include <string>

namespace rna1 {

struct fetch_options {
  fetch_options(std::string ip, size_t port)
      : m_ip(ip), m_port(port) {
    // nop
  }
  std::string m_ip;
  size_t      m_port;
};

class fetch_impl : public thread_t {
 protected:
  /**
   * Main method of the thread
   * @param  arg is a pointer to an argument
   * @return a pointer to a return code or NULL
   **/
  void* exec(void* arg);
};

} // namespace rna1


#endif // RNA1_FETCH_IMPL_HPP
