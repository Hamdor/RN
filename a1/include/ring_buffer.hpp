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
#include "lock_guard.hpp"
#include "condvar_t.hpp"
#include "picture.hpp"

namespace {

const int max_size = 30;

} // namepsace <anonymous>

namespace rna1 {

class ring_buffer {
 public:
  /**
   * Get the instance of `ring_buffer`
   * @return a pointer to the initialized `ring_buffer`
   **/
  static ring_buffer* get_instance() {
    if (!instance) {
      lock_guard gurad(s_lock);
      if (!instance) {
        instance = new ring_buffer();
      }
    }
    return instance;
  }
 private:
  static mutex_t      s_lock;
  static ring_buffer* instance;

  /**
   * Default Constructor
   **/
  ring_buffer() : m_current_pos(0), m_lock(), m_cond(m_lock) {
    // nop
  }

  ring_buffer(const ring_buffer&);

 public:
  static const int size = max_size;
  /**
   * Return a `picture` from the `ring_buffer`
   * @param  pos determines the position of the picture in the ring buffer
   * @return the requested `picture` object
   **/
  picture get_picture(size_t pos);

  /**
   * Returns the current internal index of the `ring_buffer`
   * @return the current index
   **/
  size_t get_current_pos();

  /**
   * Adds a new picture to the ring_buffer
   **/
  void add_new_picture(picture pic);
  
  /**
   * This call block until a new picture is in the buffer
   * @param num is the current picture, if the internal index
   *        is equal to num this function is blocking
   **/
  void wait_on_picture(size_t num);

 private:
  size_t    m_current_pos;
  mutex_t   m_lock;
  condvar_t m_cond;
  picture   m_data[max_size];
};

} // namespace rna1

#endif // RNA1_RING_BUFFER_HPP
