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
#include "ring_buffer.hpp"
#include "picture.hpp"

#include <iostream>

using namespace rna1;

void* worker_impl::exec(void* args) {
  connection_handle* p_handle = static_cast<connection_handle*>(args);
  if (p_handle == NULL) {
    std::cout << "ERROR: (worker) corrupt connection_handle" << std::endl;
    return NULL;
  }
  socket sock(*p_handle);
  std::cout << "New worker spawned (Addr: " << std::hex << sock.get_addr()
            << ", Port: "                   << std::dec << sock.get_port()
            << ")" << std::endl;
  ring_buffer* buffer = ring_buffer::get_instance();
  size_t pos = buffer->get_current_pos();
  picture pic;
  while(m_running) {
    pic = buffer->get_picture(pos);
    int rc = 0;
    while (rc != sizeof(pic.m_data)) {
      int err = sock.send(pic.m_data + rc, sizeof(pic.m_data) - rc, MSG_NOSIGNAL);
      if (err == 0 || err == -1) {
        std::cerr << "Connection was closed!" << std::endl;
        m_running = false;
        break;
      }
      rc += err;
    }
    pos = (pos + 1) % 100;
  }
  delete p_handle; // delete allocated memory...
  delete this;
  return NULL;
}
