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

using namespace std;
using namespace rna1;

ring_buffer* ring_buffer::instance = NULL;

picture* ring_buffer::get_picture(size_t pos) {
  lock_guard guard (m_lock);
  return pos < sizeof(m_data) ? &m_data[pos] : NULL;
}

size_t ring_buffer::get_current_pos() {
  return m_current_pos;
}

void ring_buffer::add_new_picture(picture pic) {
  lock_guard guard (m_lock);
  m_data[++m_current_pos % sizeof(m_data)] = pic;
}
