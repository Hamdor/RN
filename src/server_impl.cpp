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
  uint16_t port = *static_cast<uint16_t*>(args);
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
  while (m_running) {
    connection_handle* handle = sock.accept();
    if (handle == NULL) {
      // accept failed. We got already a error message
      return NULL;
    }
    worker_impl* new_worker = new worker_impl();
    // In C++ `new` won't return NULL, it will throw `std::bad_alloc`,
    // if you want NULL to be returned use `new (nothrow) ...`
    // https://stackoverflow.com/questions/3389420/will-new-operator-return-null
    new_worker->start(static_cast<void*>(handle));
  }
  return NULL;
}
