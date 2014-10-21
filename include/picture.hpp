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

#ifndef RNA1_PICTURE_HPP
#define RNA1_PICTURE_HPP

#include <cstring>

namespace {

const int picture_width   = 320;
const int picture_height  = 240;
const int bytes_per_pixel = 3;

} // namespace <anonymous>

namespace rna1 {

struct picture {
  char m_data[picture_width * picture_height * bytes_per_pixel];

  /**
   * Copy Constructor
   **/
  picture(const picture& other) {
    std::memcpy(m_data, other.m_data, sizeof(m_data));
  }

  /**
   * Default Constructor
   **/
  picture() {
    // nop
  }
};

} // namespace rna1

#endif // RNA1_PICTURE HPP
