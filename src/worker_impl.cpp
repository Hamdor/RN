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

#include "server_impl.hpp"
#include "ring_buffer.hpp"
#include "picture.hpp"
#include "socket.hpp"

#include <iostream>

using namespace rna1;

void* worker_impl::exec(void* args) {
  connection_handle* p_handle = static_cast<connection_handle*>(args);
  socket sock(*p_handle);
  struct timeval tv;
  tv.tv_sec  = 5;
  tv.tv_usec = 0;
  if (sock.setsockopt(SO_RCVTIMEO, &tv, sizeof(struct timeval))) {
    std::cerr << "Error can't set SO_RCVTIMEO!" << std::endl;
    this->detach();
    delete p_handle;
    // decrease connection count
    server_impl::notify_death();
    delete this;
    return NULL;
  }
  //std::cout << "New worker spawned (Addr: " << std::hex << sock.get_addr()
  //          << ", Port: "                   << std::dec << sock.get_port()
  //          << ")" << std::endl;
  ring_buffer* buffer = ring_buffer::get_instance();
  size_t pos = buffer->get_current_pos();
  picture pic;
  while (m_running && !sock.has_error()) {
    pic = buffer->get_picture(pos);
    int rc = 0;
    while (rc != sizeof(pic.m_data) && !sock.has_error() && m_running) {
      int err = sock.send(pic.m_data + rc, sizeof(pic.m_data) - rc, MSG_NOSIGNAL);
      if (err == -1) {
        m_running = false;
        break;
      }
      rc += err;
    }
    buffer->wait_on_picture(pos);
    pos = (pos + 1) % buffer->size;
  }
  // cleanup
  this->detach();
  delete p_handle;
  // decrease connection count
  server_impl::notify_death();
  delete this;
  return NULL;
}
