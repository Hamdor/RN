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

#ifndef RNA1_SOCKET_HPP
#define RNA1_SOCKET_HPP

#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace rna1 {

struct connection_handle {
  int                m_socket;
  struct sockaddr_in m_sockaddr;
};

class socket {
 public:
  socket(int socket_family, int socket_type, int protocol,
         size_t port, long addr = INADDR_ANY);

  socket(const connection_handle& handle);

  ~socket();
  
  bool is_bound() const { return m_bound; }
  bool is_listen() const { return m_listen; }
  bool has_error() const { return m_rc != 0; }

  int bind();

  int listen(size_t fifo_size=100);
  
  int close();

  connection_handle* accept();

  int connect();

  int recv(void* buffer, size_t len, int flags = 0);

  int send(void* data, size_t len, int flags = 0);

 private:
  socket(const socket&);

  connection_handle  m_handle;  
  int                m_rc;
  bool               m_bound;
  bool               m_listen;
};

} // namespace rna1

#endif // RNA1_SOCKET_HPP
