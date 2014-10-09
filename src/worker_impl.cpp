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

#include "worker_impl.hpp"

#include "socket.hpp"
#include <iostream>

using namespace rna1;

void* worker_impl::exec(void* args) {
  connection_handle* p_handle = static_cast<connection_handle*>(args);
  if (p_handle == NULL) {
    std::cout << "ERROR: (worker) corrupt connection_handle" << std::endl;
    return NULL;
  }
  while(m_running) {
    // TODO:
    // Send pictures to client...:)
  }
  delete p_handle; // delete allocated memory...
  return NULL;
}
