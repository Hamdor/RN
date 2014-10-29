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

namespace rna1 {

namespace {

  const int socket_fifo_size = 100;

} // namespace <anonymous>

struct connection_handle {
  int                m_socket;   // socket descriptor
  struct sockaddr_in m_sockaddr; // sockaddr struct
};

/**
 * This class is a wrapper for the socket API
 **/
class socket {
 public:
  /**
   * Default Constructor
   * Creates a new socket for the given options.
   * @param socket_family
   * @param socket_type
   * @param protocol
   * @param port port the socket listen on
   * @param addr address the socket listen on (Default: INADDR_ANY)
   **/
  socket(int socket_family, int socket_type, int protocol,
         size_t port, long addr = INADDR_ANY);

  /**
   * Creates a new socket wrapper from an exsisting connection_handle
   * @param handle is the initialized connection_handle
   **/
  socket(const connection_handle& handle);

  ~socket();
  
  /**
   * Check if the socket is bound
   **/
  bool is_bound() const { return m_bound; }

  /**
   * Check if the socket is listening
   **/
  bool is_listen() const { return m_listen; }

  /**
   * Check if the socket had an error
   **/
  bool has_error() const { return m_rc != 0; }

  /**
   * Bind a name to a socket
   **/
  int bind();

  /**
   * Marks the socket as a passive socket.
   * @param fifo_size set the maximum fifo size for
   *        incomming connections (Default: see socket_fifo_size)
   **/
  int listen(size_t fifo_size = socket_fifo_size);
  
  /**
   * Close the socket
   **/
  int close();

  /**
   * Acceopt an incomming connection
   * this function is blocking
   * @return a pointer to a connection_handle
   *         this object has to be deleted by the caller
   **/
  connection_handle* accept();

  /**
   * This socket connects to another host
   **/
  int connect();

  /**
   * Receive data on this socket
   * @param buffer a pointer to the buffer to fill
   * @param len is the length of the buffer
   * @flags optional flags
   **/
  int recv(void* buffer, size_t len, int flags = 0);

  /**
   * Send data on this socket
   * @param pointer to data
   * @param len the length of the data
   * @param optional flags
   **/
  int send(void* data, size_t len, int flags = 0);

  /**
   * Return the addr this socket is listen on
   * @return addr is returned as long
   **/
  long get_addr() const;

  /**
   * Return the port this socket is listen on
   * @return the port
   **/
  uint16_t get_port() const;

  /**
   * Set socket options to this socket
   **/
  int setsockopt(int optname, void* val, long unsigned int len);

 private:
  socket(const socket&);

  connection_handle  m_handle;  
  int                m_rc;
  bool               m_bound;
  bool               m_listen;
};

} // namespace rna1

#endif // RNA1_SOCKET_HPP
