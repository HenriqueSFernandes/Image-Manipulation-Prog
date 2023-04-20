#ifndef __prog_PNG_hpp__
#define __prog_PNG_hpp__

#include <string>
#include <iostream>
#include "Image.hpp"

namespace prog {
    Image* loadFromPNG(const std::string &file);

    void saveToPNG(const std::string& file, const Image* img);

    bool comparePNG(const std::string& file1, const std::string& file2, std::ostream& out);
}
#endif
