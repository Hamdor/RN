#include "fetch_impl.hpp"

#include "picture.hpp"
#include "socket.hpp"
#include "ring_buffer.hpp"

#include <unistd.h>

using namespace rna1;

void* fetch_impl::exec(void* args) {
  size_t port = 5000;
  socket sock(AF_INET,SOCK_STREAM, IPPROTO_TCP, port);
  sock.bind();
  sock.connect();
  picture pic;
  ring_buffer* instance = ring_buffer::get_instance();
  while(m_running) {
    sock.recv(&pic.m_data, sizeof(pic.m_data));
    instance->add_new_picture(pic);
    ::usleep(1000 * 33);
  }
  return NULL;
}
