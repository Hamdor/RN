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

class worker_impl : public thread_t {
  /**
   * The worker calls `destroy()` on error or
   * if it is about to leave its exec() scope
   * @param p_handle a pointer to the connection_handle to delete
   **/
  void destroy(connection_handle* p_handle);
 protected:
  /**
   * Main method of the thread
   * @param  arg is a pointer to an argument
   * @return a pointer to a return code or NULL
   **/
  void* exec(void*);
};

} // namespace rna1


#endif // RNA1_WORKER_IMPL_HPP
