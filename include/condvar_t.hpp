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
  /**
   * Creates a new condvar_t
   * @param referenze to a mutex_t
   **/
  condvar_t(mutex_t& mutex) : m_lock(mutex) {
    pthread_cond_init(&m_cond, NULL);
  }
  
  /**
   * Destructor
   **/
  virtual ~condvar_t() {
    pthread_cond_destroy(&m_cond);
  }

  /**
   * Calling thread blocks on this condition variable
   * @return EINVAL if the arguments were invalid
   *         EINVAL different mutexes were given
   *         EINVAL the mutex was not owned by the current
   *                thread at the time of the call
   **/
  int wait() {
    return pthread_cond_wait(&m_cond, &m_lock.m_mutex);
  }

  /**
   * Signal one waiting thread on this condition variable
   * @return EINVAL the internal `pthread_cond_t` was not
   *                initialized
   **/
  int signal() {
    return pthread_cond_signal(&m_cond);
  }

  /**
   * Signal more than one waiting thread on this condition variable
   * @return EINVAL the internal `pthread_cont_t` was not
   *                initialized
   **/
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
