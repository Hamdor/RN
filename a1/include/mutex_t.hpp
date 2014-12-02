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

#ifndef RNA1_MUTEX_HPP
#define RNA1_MUTEX_HPP

#include <pthread.h>

namespace rna1 {

class mutex_t {
 friend class condvar_t; // condvar_t needs access to m_mutex
 public:
  /**
   * Default Constructor
   **/
  mutex_t() {
    pthread_mutex_init(&m_mutex, NULL);
  }

  /**
   * Default Destructor
   **/
  ~mutex_t() {
    pthread_mutex_destroy(&m_mutex);
  }

  /**
   * The calling thread locks the mutex
   * This function block the calling thread until
   * the mutex is acquired
   **/
  void Acquire() {
    pthread_mutex_lock(&m_mutex);
  }

  /**
   * Returns the mutex
   **/
  void Release() {
    pthread_mutex_unlock(&m_mutex);
  }

 private:
  mutex_t(const mutex_t&);

  pthread_mutex_t m_mutex;
};

} // namespace rna1

#endif // RNA1_MUTEX_HPP
