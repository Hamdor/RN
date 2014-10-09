
#ifndef RNA1_PICTURE_HPP
#define RNA1_PICTURE_HPP

namespace {

const int picture_width   = 320;
const int picture_height  = 240;
const int bytes_per_pixel = 3;

};

struct picture {
  char m_data[picture_width * picture_height * bytes_per_pixel];
};

#endif // RNA1_PICTURE HPP



