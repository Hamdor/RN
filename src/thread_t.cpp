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

#include <thread_t.hpp>

using namespace rna1;

thread_t::thread_t() {
  // nop
}

thread_t::~thread_t() {
  // nop
}

void thread_t::start(void* args) {
  set_args(args);
  pthread_create(&m_pthread, NULL, (thread_t::entry_point), this);
}

void thread_t::join() {
  pthread_join(m_pthread, NULL);
}

void thread_t::set_args(void* args) {
  m_args = args;
}

void* thread_t::entry_point(void* pthis) {
  thread_t* p = static_cast<thread_t*>(pthis);
  return p->exec(p->m_args);
}

