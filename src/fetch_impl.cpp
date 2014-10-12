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
#include <arpa/inet.h>

using namespace rna1;

void* fetch_impl::exec(void* args) {
  if (args == NULL) {
    std::cerr << "ERROR in fetch_impl! args not valid" << std::endl;
    return NULL;
  }
  // get args from void ptr
  fetch_options* fopts = static_cast<fetch_options*>(args);
  struct in_addr inp;
  ::inet_aton(fopts->m_ip.c_str(), &inp);
  socket sock(AF_INET,SOCK_STREAM, IPPROTO_TCP, fopts->m_port, inp.s_addr);
  if (sock.connect()) {
    return NULL;
  }
  picture pic;
  ring_buffer* instance = ring_buffer::get_instance();
  while(m_running && !sock.has_error()) {
    int rc = 0;
    while (rc != sizeof(pic.m_data) && !sock.has_error() && m_running) {
      int err = sock.recv(pic.m_data + rc, sizeof(pic.m_data) - rc);
      if (err == 0 || err == -1) {
        m_running = false;
        return NULL;
      }
      rc += err;
    }
    instance->add_new_picture(pic);
  }
  return NULL;
}
