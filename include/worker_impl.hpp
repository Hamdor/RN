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

#ifndef RNA1_WORKER_IMPL_HPP
#define RNA1_WORKER_IMPL_HPP

#include "thread_t.hpp"
#include "socket.hpp"

namespace rna1 {

struct worker_options {
  worker_options(connection_handle* handle, size_t fps)
      : m_handle(handle), m_fps(fps) {
    // nop
  }
  connection_handle* m_handle;
  size_t            m_fps;
};

class worker_impl : public thread_t {
 protected:
  void* exec(void*);
};

} // namespace rna1


#endif // RNA1_WORKER_IMPL_HPP
