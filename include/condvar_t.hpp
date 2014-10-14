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

#ifndef RNA1_CONDVAR_HPP
#define RNA1_CONDVAR_HPP

#include "mutex_t.hpp"

#include <pthread.h>

namespace rna1 {

class condvar_t {
 public:
  condvar_t(mutex_t& mutex) : m_lock(mutex) {
    pthread_cond_init(&m_cond, NULL);
  }
  
  virtual ~condvar_t() {
    pthread_cond_destroy(&m_cond);
  }

  int wait() {
    return pthread_cond_wait(&m_cond, &m_lock.m_mutex);
  }

  int signal() {
    return pthread_cond_signal(&m_cond);
  }

  int broadcast() {
    return pthread_cond_broadcast(&m_cond);
  }

 private:
  condvar_t();
  condvar_t(const condvar_t&);

  mutex_t&       m_lock;
  pthread_cond_t m_cond;
};

} // namespace rna1

#endif // RNA1_CONDVAR_HPP
