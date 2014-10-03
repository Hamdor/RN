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

#ifndef RNA1_THREAD_HPP
#define RNA1_THREAD_HPP

#include <pthread.h>

namespace rna1 {

class thread_t {
 public:
  thread_t();
  virtual ~thread_t();

  void start();
  void join();

 protected:
  virtual void* exec() = 0;
  static  void* entry_point(void*);

 private:
  thread_t(const thread_t&);

  pthread_t m_pthread;
};

} // namespace rna1

#endif // RNA1_THREAD_HPP
