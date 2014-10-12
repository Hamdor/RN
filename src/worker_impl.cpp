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

#include "ring_buffer.hpp"
#include "picture.hpp"

#include <iostream>

#include <unistd.h>

using namespace rna1;

void* worker_impl::exec(void* args) {
  worker_options* p_args = static_cast<worker_options*>(args);
  if (p_args == NULL) {
    std::cout << "ERROR: (worker) corrupt worker_options" << std::endl;
    this->detach();
    delete this;
    return NULL;
  }
  if (p_args->m_handle == NULL) {
    std::cout << "ERROR: (worker) corrupt connection_handle" << std::endl;
    this->detach();
    delete p_args;
    delete this;
    return NULL;
  }
  socket sock(*p_args->m_handle);
  int sleep = ((double) 1 / (double) p_args->m_fps) * 1000 * 1000;
  std::cout << "New worker spawned (Addr: " << std::hex << sock.get_addr()
            << ", Port: "                   << std::dec << sock.get_port()
            << ", fps: "                    << std::dec << p_args->m_fps
            << ")" << std::endl;
  ring_buffer* buffer = ring_buffer::get_instance();
  size_t pos = buffer->get_current_pos();
  picture pic;
  while (m_running && !sock.has_error()) {
    pic = buffer->get_picture(pos);
    int rc = 0;
    while (rc != sizeof(pic.m_data) && !sock.has_error() && m_running) {
      int err = sock.send(pic.m_data + rc, sizeof(pic.m_data) - rc, MSG_NOSIGNAL);
      if (err == 0 || err == -1) {
        m_running = false;
        break;
      }
      rc += err;
      ::usleep(sleep);
    }
    pos = (pos + 1) % buffer->size;
  }
  // cleanup
  this->detach();
  delete p_args->m_handle;
  delete p_args;
  delete this;
  return NULL;
}
