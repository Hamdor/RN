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

#include "socket.hpp"

#include <iostream>

using namespace rna1;

socket::socket(int socket_family, int socket_type, int protocol,
               size_t port) {
  m_handle.m_sockaddr.sin_family      = socket_family;
  m_handle.m_sockaddr.sin_port        = ::htons(port);
  m_handle.m_sockaddr.sin_addr.s_addr = INADDR_ANY;
  m_handle.m_socket = ::socket(socket_family, socket_type, protocol);
  if (m_handle.m_socket == -1) {
    m_rc = -1;
  }
  int option = 1;
  m_rc = ::setsockopt(m_handle.m_socket, SOL_SOCKET,
                      SO_REUSEADDR, &option, sizeof(option));
  if (m_rc != 0) {
    std::cout << "ERROR: setsockopt failed with "
              << m_rc << std::endl;
  }
}

socket::~socket() {
  if (m_handle.m_socket != 0) {
    ::close(m_handle.m_socket);
  }
}

int socket::bind() {
  if (m_rc != 0) {
    return m_rc;
  }
  if (m_bound) {
    std::cout << "ERROR: Socket already bound!" << std::endl;
    return -1;
  }
  m_rc = ::bind(m_handle.m_socket,
                reinterpret_cast<struct sockaddr*>(&m_handle.m_sockaddr),
                sizeof(struct sockaddr_in));
  if (m_rc == 0) {
    m_bound = true;
  }
  return m_rc;
}

int socket::listen(size_t fifo_size) {
  if (m_rc != 0) {
    return m_rc;
  }
  if (m_listen) {
    std::cout << "ERROR: Socket already listen!" << std::endl;
    return -1;
  }
  if (!m_bound) {
    std::cout << "ERROR: Socket is not bound!" << std::endl;
    return -2;
  }
  m_rc = ::listen(m_handle.m_socket, fifo_size);
  return m_rc;
}

int socket::close() {
  return m_handle.m_socket != 0 ? ::close(m_handle.m_socket) : 0;
}

connection_handle* socket::accept() {
  connection_handle* p_con = new connection_handle();
  int n = sizeof(struct sockaddr_in);
  p_con->m_socket = ::accept(m_handle.m_socket,
                             reinterpret_cast<
                               struct sockaddr*>(&p_con->m_sockaddr),
                             reinterpret_cast<socklen_t*>(&n));
  if (p_con->m_socket == -1) {
    // error free memory...
    std::cout << "ERROR in accept()! p_con->m_socket = "
              << p_con->m_socket << std::endl;
    delete p_con;
    return NULL;
  }
  return p_con;
}
