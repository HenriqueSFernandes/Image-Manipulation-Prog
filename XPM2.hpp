#ifndef __prog_XPM2_hpp__
#define __prog_XPM2_hpp__

#include <string>
#include "Image.hpp"

namespace prog
{
    Image *loadFromXPM2(const std::string &filename);

    void saveToXPM2(const std::string &filename, const Image *img);

    Color hexa_to_rgb(const std::string &hexa_color);

    std::string rgb_to_hexa(const Color &color);
}
#endif
