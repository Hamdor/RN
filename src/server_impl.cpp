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

#include "server_impl.hpp"

#include "worker_impl.hpp"
#include "socket.hpp"
#include <iostream>

using namespace rna1;

void* server_impl::exec(void* args) {
  size_t port = 5001;
  size_t fps  = 20;
  socket sock(AF_INET,SOCK_STREAM, IPPROTO_TCP, port);
  if (sock.has_error()) {
    std::cout << "ERROR ::socket() failed! Port: " << port << std::endl;
    return NULL;
  }
  if (sock.bind()) {
    std::cout << "ERROR ::bind() failed! Port: " << port << std::endl;
    return NULL;
  }
  if (sock.listen()) {
    std::cout << "ERROR ::listen() failed! Port: " << port << std::endl;
    return NULL;
  }
  std::cout << "Server listen on port " << sock.get_port() << std::endl;
  while(m_running) {
    worker_options* opts = new worker_options(sock.accept(), fps);
    worker_impl* new_worker = new worker_impl();
    new_worker->detach();
    new_worker->start(static_cast<void*>(opts));
  }
  return NULL;
}
