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
#include <cstring>
#include <cstdio>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#include <arpa/inet.h>

using namespace rna1;

socket::socket(int socket_family, int socket_type, int protocol,
               size_t port, long addr) {
  m_handle.m_sockaddr.sin_family      = socket_family;
  m_handle.m_sockaddr.sin_port        = htons(port);
  m_handle.m_sockaddr.sin_addr.s_addr = 
      (addr != INADDR_ANY) ? addr : INADDR_ANY;
  m_handle.m_socket = ::socket(socket_family, socket_type, protocol);
  if (m_handle.m_socket < 0) {
    m_rc = -1;
  }
  int opt = 1;
  setsockopt(SO_REUSEADDR, &opt, sizeof(int));
}

socket::socket(const connection_handle& handle)
    : m_handle(handle),
      m_rc(0),
      m_bound(true),
      m_listen(false) {
  int opt = 1;
  setsockopt(SO_REUSEADDR, &opt, sizeof(int));
}

socket::~socket() {
  close();
}

int socket::bind() {
  if (m_rc != 0) {
    return m_rc;
  }
  m_rc = ::bind(m_handle.m_socket,
                reinterpret_cast<struct sockaddr*>(&m_handle.m_sockaddr),
                sizeof(struct sockaddr_in));
  if (m_rc == 0) {
    m_bound = true;
  } else {
    perror("ERROR: Socket bind() failed!");
  }
  return m_rc;
}

int socket::listen(size_t fifo_size) {
  if (m_rc != 0) {
    return m_rc;
  }
  if (!m_bound) {
    std::cerr << "ERROR: Socket is not bound!" << std::endl;
    return -2;
  }
  m_rc = ::listen(m_handle.m_socket, fifo_size);
  return m_rc;
}

int socket::close() {
  return m_handle.m_socket > 0 ? ::close(m_handle.m_socket) : 0;
}

connection_handle* socket::accept() {
  connection_handle* p_con = new connection_handle();
  // In C++ `new` won't return NULL, it will throw `std::bad_alloc`,
  // if you want NULL to be returned use `new (nothrow) ...`
  // https://stackoverflow.com/questions/3389420/will-new-operator-return-null
  int n = sizeof(struct sockaddr_in);
  p_con->m_socket = ::accept(m_handle.m_socket,
                             reinterpret_cast<
                               struct sockaddr*>(&p_con->m_sockaddr),
                             reinterpret_cast<socklen_t*>(&n));
  if (p_con->m_socket < 0) {
    // error free memory...
    perror("ERROR in accpet()!");
    delete p_con;
    return NULL;
  }
  return p_con;
}

int socket::connect() {
  m_rc = ::connect(m_handle.m_socket,
                   reinterpret_cast<struct sockaddr*>(&m_handle.m_sockaddr),
                   sizeof(struct sockaddr_in));
  if (m_rc != 0) {
    perror("ERROR in connect()!");
  }
  return m_rc;
}

int socket::recv(void* buffer, size_t len, int flags) {
  int rc = ::recv(m_handle.m_socket, buffer, len, flags);
  if (rc == 0) {
    perror("ERROR in recv()! Connection was closed!");
    m_rc = rc;
  } else if (rc == -1) {
    perror("ERROR in recv()! ");
    m_rc = rc;
  }
  return rc; // amount of bytes read
}

int socket::send(void* data, size_t len, int flags) {
  int rc = ::send(m_handle.m_socket, data, len, flags);
  if (rc == -1) {
    //perror("ERROR in send()!");
    m_rc = rc;
  }
  return rc; // amount of bytes send
}

long socket::get_addr() const {
  return ntohl(m_handle.m_sockaddr.sin_addr.s_addr);
}

uint16_t socket::get_port() const {
  return ntohs(m_handle.m_sockaddr.sin_port);
}

int socket::setsockopt(int optname, void* val, long unsigned int len) {
  m_rc = ::setsockopt(m_handle.m_socket, SOL_SOCKET, optname, val, len);
  if (m_rc) {
    perror("ERROR: setsockopt failed!");
  }
  return m_rc;
}

long socket::get_addr_from_str(std::string host) {
  struct addrinfo info;
  struct addrinfo* res = NULL;
  std::memset(&info, 0, sizeof(addrinfo));
  info.ai_family   = PF_UNSPEC;
  info.ai_socktype = SOCK_STREAM;
  int rc = getaddrinfo(host.c_str(), NULL, &info, &res);
  if (rc) {
    std::cerr << "getaddrinfo() failed!" << std::endl;
    return -1;
  }
  struct addrinfo* ptr = res;
  long ip = 0;
  while (ptr) {
    if (ptr->ai_family == AF_INET) {
      ip = ((struct sockaddr_in*)ptr->ai_addr)->sin_addr.s_addr;
      break;
    }
    ptr = ptr->ai_next;
  }
  freeaddrinfo(res);
  return ip; 
}

