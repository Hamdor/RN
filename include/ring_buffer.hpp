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

#ifndef RNA1_RING_BUFFER_HPP
#define RNA1_RING_BUFFER_HPP

#include "mutex_t.hpp"
#include "condvar_t.hpp"
#include "picture.hpp"

namespace {

const int max_size = 100;

} // namepsace <anonymous>

namespace rna1 {

class ring_buffer {
 public:
  static ring_buffer* get_instance() {
    if (!instance) {
      instance = new ring_buffer();
    }
    return instance;
  }
 private:
  static ring_buffer* instance;
  ring_buffer() : m_current_pos(0), m_lock(), m_cond(m_lock) {
    // nop
  }

  ring_buffer(const ring_buffer&);

 public:
  picture get_picture(size_t pos);
  size_t get_current_pos();

  void add_new_picture(picture pic);
  
  void wait_on_picture(size_t num);

 private:
  size_t    m_current_pos;
  mutex_t   m_lock;
  condvar_t m_cond;
  picture   m_data[max_size];
};

} // namespace rna1

#endif // RNA1_RING_BUFFER_HPP

