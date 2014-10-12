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

#include "ring_buffer.hpp"
#include "lock_guard.hpp"
#include <iostream>

using namespace std;
using namespace rna1;

ring_buffer* ring_buffer::instance = NULL;

picture ring_buffer::get_picture(size_t pos) {
  lock_guard guard(m_lock);
  return m_data[pos];
}

size_t ring_buffer::get_current_pos() {
  lock_guard guard(m_lock);
  return m_current_pos;
}

void ring_buffer::add_new_picture(picture pic) {
  lock_guard guard(m_lock);
  m_current_pos = (m_current_pos + 1) % 100;
  m_data[m_current_pos] = pic;
  m_cond.broadcast();
}

void ring_buffer::wait_on_picture(size_t num) {
  lock_guard guard(m_lock);
  while (m_current_pos == num) {
    // no newer picture
    m_cond.wait();
  }
}
