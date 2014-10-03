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

#ifndef RNA1_LOCK_GUARD_HPP
#define RNA1_LOCK_GUARD_HPP

#include "mutex.hpp"

namespace rna1 {

class lock_guard {
  lock_guard(mutex_t& mutex) : m_ref(mutex) {
    m_ref.Acquire();
  }
  
  ~lock_guard() {
    m_ref.Release();
  }

 private:
  lock_guard(const lock_guard&);
  mutex_t& m_ref;
};

} // namespace rna1

#endif // RNA1_LOCK_GUARD_HPP
