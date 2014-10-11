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
#include <cstdlib>
#include <stdint.h>
#include <getopt.h>

using namespace rna1;

namespace {
  enum short_options {
    noserver = (char)1,
    noclient,
    serverport,
    camserver,
    camport,
    fps,
    help
  };

  const struct option options[] = {
    { "no-server",   no_argument,       0, static_cast<char>(noserver)   },
    { "no-client",   no_argument,       0, static_cast<char>(noclient)   },
    { "server-port", required_argument, 0, static_cast<char>(serverport) },
    { "cam-server",  required_argument, 0, static_cast<char>(camserver)  },
    { "cam-port",    required_argument, 0, static_cast<char>(camport)    },
    { "fps",         required_argument, 0, static_cast<char>(fps)        },
    { "help",        no_argument,       0, static_cast<char>(help)       }
  };

  struct arguments {
    arguments(uint16_t server_port, std::string cam_server,
              uint16_t cam_port, bool no_client, bool no_server, uint8_t fps)
        : m_server_port(server_port),
          m_cam_server(cam_server),
          m_cam_port(cam_port),
          m_no_client(no_client),
          m_no_server(no_server),
          m_fps(fps) {
      // nop
    }

    uint16_t    m_server_port;
    std::string m_cam_server;
    uint16_t    m_cam_port;
    bool        m_no_client;
    bool        m_no_server;
    uint8_t     m_fps;
   private:
    arguments();
  };
} // namespace <anonymous>

void print_help() {
  std::cout << "Usage: rna1 [Options]"                          << std::endl
            << "Options:"                                       << std::endl
            << "  --no-server          Start without server"    << std::endl
            << "  --no-client          Start without client"    << std::endl
            << "  --server-port=<arg>  Port to listen on"       << std::endl
            << "  --cam-server=<arg>   IP of camera server"     << std::endl
            << "  --cam-port=<arg>     Port of camera server"   << std::endl
            << "  --fps=<arg>          fps of server"           << std::endl
            << "  --help               Print this help message" << std::endl;
}

arguments parse_args(int argc, char* argv[]) {
  arguments args(5001, "127.0.0.1", 5000, false, false, 20);
  int opt = 0;
  int idx = 0;
  while ((opt = getopt_long_only(argc, argv, "", options,
                                &idx)) != -1 ) {
    switch (opt) {
      case noserver: {
        args.m_no_server = true;
      } break;
      case noclient: {
        args.m_no_client = true;
      } break;
      case serverport: {
        args.m_server_port = ::atoi(optarg);
      } break;
      case camserver: {
        args.m_cam_server = optarg;
      } break;
      case camport: {
        args.m_cam_port = ::atoi(optarg);
      } break;
      case fps: {
        args.m_fps = ::atoi(optarg);
      } break;
      case help: {
        print_help();
        exit(0);
      } break;
      default: {
        // unkown argument
        std::cerr << "Unkown argument (" << opt << ")" << std::endl;
        print_help();
        exit(0);
      } break;
    }
  }
  return args;
}

int main(int argc, char* argv[]) {
  // parse arguments
  arguments args = parse_args(argc, argv);
  // prepare fetcher
  fetch_options fopts(args.m_cam_server, args.m_cam_port);
  fetch_impl fetch;
  fetch.start(&fopts);
  // prepare client
  client_impl client;
  if (!args.m_no_client) {
    client.start();
  }
  // prepare server
  server_options sopts(args.m_server_port, args.m_fps);
  server_impl server;
  if (!args.m_no_server) {
    server.start(&sopts);
  }
  // join threads
  fetch.join();
  if (!args.m_no_client) {
    client.join();
  }
  if (!args.m_no_server) {
    server.join();
  }
  delete ring_buffer::get_instance(); // delete singleton
  return 0;
}
