#include "PNG.hpp"

// STB library includes & constraints
#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

using namespace std;

namespace prog {
    Image* loadFromPNG(const string& file) {
        int w, h, dummy;
        rgb_value *buffer = stbi_load(file.c_str(), &w, &h, &dummy, 3);
        if (buffer == nullptr) {
            return nullptr; // Could not load image!
        }
        rgb_value* p = buffer;
        Image* image = new Image(w, h);
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                image -> at(x, y) = { p[0], p[1], p[2] };
                p += 3;
            }
        }
        stbi_image_free(buffer);
        return image;
    }
    void saveToPNG(const string& file, const Image* image) {
        int h = image->height();
        int w = image->width();
        rgb_value* buffer = new rgb_value[h * w * 3];
        rgb_value* p = buffer;
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                Color c = image -> at(x, y);
                p[0] = c.red();
                p[1] = c.green();
                p[2] = c.blue();
                p += 3;
            }
        }
        stbi_write_png(file.c_str(),
                       w,
                       h,
                       3,
                       buffer,
                       w * 3);
        delete [] buffer;
    }

    bool comparePNG(const string& file1, const string& file2, ostream& out) {
        int w1, h1, w2, h2, dummy;
        rgb_value *buffer1 = stbi_load(file1.c_str(), &w1, &h1, &dummy, 3);
        if (buffer1 == nullptr) {
            out << "Could not load image from " << file1 << std::endl;
            return false;
        }            
        rgb_value *buffer2 = stbi_load(file2.c_str(), &w2, &h2, &dummy, 3);
        if (buffer2 == nullptr) {
            out << "Could not load image from " << file2 << std::endl;
            stbi_image_free(buffer1);
            return false;
        }  
        bool success = true;
        if (w1 != w2 || h1 != h2) {
            out << "Images have different dimensions: "  
                << w1 << "x" << h1 << " != "
                << w2 << "x" << h2 <<  endl;
            success = false;
        } else {
            rgb_value* p1 = buffer1;
            rgb_value* p2 = buffer2;
            for (int y = 0; y < h1 && success; y++) {
                for (int x = 0; x < w1; x++) {
                    if (p1[0] != p2[0] || p1[1] != p2[1] || p1[2] != p2[2]) {
                        out << "Images differ at pixel (" << x << ", " << y << ")" << endl 
                            << (int) p1[0] << ' ' << (int) p1[1] << ' ' << (int) p1[2] << " vs "
                            << (int) p2[0] << ' ' << (int) p2[1] << ' ' << (int) p2[2] << endl;

                        success = false;
                        break;
                    }
                    p1 += 3;
                    p2 += 3;
                }
            }
        }
        stbi_image_free(buffer1);
        stbi_image_free(buffer2);
        return success;
    }
}
