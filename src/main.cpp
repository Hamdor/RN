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

#include "client_impl.hpp"
#include "server_impl.hpp"
#include "fetch_impl.hpp"
#include "ring_buffer.hpp"

#include <iostream>
#include <string>
#include <cstring>

using namespace rna1;

namespace {
  const char server_port_arg[]     = "--server-port=";
  const char cam_server_ip_arg[]   = "--cam-server=";
  const char cam_server_port_arg[] = "--cam-port=";
  const char with_client_arg[]     = "--with-client";
} // namespace <anonymous>

/*struct arguments {
  uint16_t    m_server_port = "1337";
  std::string m_cam_server  = "localhost";
  uint16_t    m_cam_port    = 5000;
  bool        m_clien   t   = false;
};*/

/*bool is_substr(const char* ptr, const char* end) {
  if(::strncmp(ptr, end, strlen(ptr) - 1)) {
    return true;
  }
  return false;
}

arguments parse_args(int argc, const char* argv[]) {
  arguments args;
  for (char* i = argv; i != argv + argc; ++i) {
    if (is_substr(server_port_arg, *i)) {
      int p = std::stoi(*i + ::strlen(cam_server_port_arg));
      args.m_cam_port = static_cast<uint16_t>(p);
    } else if (is_substr(cam_server_ip_arg, *i)) {
      args.m_cam_server.assign(*i + ::strlen(cam_server_ip_arg));
    } else if (is_substr(cam_server_port_arg, *i)) {
      int p = std::stoi(*i + ::strlen(cam_server_port_arg));
      args.m_cam_port = static_cast<uint16_t>(p);
    } else if (is_substr(with_client_arg, *i)) {
      args.m_client = true;
    }
  }
  return args;
}*/

void print_help() {
  std::cout << "Usage: rna1 [Options]"                        << std::endl
            << "Options:"                                     << std::endl
            << "  --server-port=<arg>  Port to listen on"     << std::endl
            << "  --cam-server=<arg>   IP of camera server"   << std::endl
            << "  --cam-port=<arg>     Port of camera server" << std::endl
            << "  --with-client        Open an client window" << std::endl;
}

int main(int argc, const char* argv[]) {
  //if (!parse_args(argc, argv)) {
  //  print_help();
  //  return 1;
  //}
  
  fetch_impl fetch;
  fetch.start();

  client_impl client;
  int sleep_client = 50;
  client.start(static_cast<void*>(&sleep_client));

  server_impl server;
  server.start();

  fetch.join();
  client.join();
  server.join();
  
  delete ring_buffer::get_instance(); // delete singleton
  return 0;
}
