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

#include "fetch_impl.hpp"

#include "picture.hpp"
#include "socket.hpp"
#include "ring_buffer.hpp"

#include <iostream>

using namespace rna1;

void* fetch_impl::exec(void* args) {
  size_t port = 5000;
  socket sock(AF_INET,SOCK_STREAM, IPPROTO_TCP, port);
  sock.bind();
  sock.connect();
  picture pic;
  ring_buffer* instance = ring_buffer::get_instance();
  while(m_running) {
    int rc = 0;
    while (rc != sizeof(pic.m_data)) {
      rc += sock.recv(pic.m_data + rc, sizeof(pic.m_data) - rc);
    }
    instance->add_new_picture(pic);
  }
  return NULL;
}
