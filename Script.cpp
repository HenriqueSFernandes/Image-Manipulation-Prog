#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"

using namespace std;

namespace prog
{
    // Use to read color values from a script file.
    istream &operator>>(istream &input, Color &c)
    {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    // Use to compare colors.
    bool operator==(const Color &color1, const Color &color2)
    {
        return (color1.red() == color2.red() && color1.green() == color2.green() && color1.blue() == color2.blue());
    }

    bool operator!=(const Color &color1, const Color &color2)
    {
        return (color1.red() != color2.red() || color1.green() != color2.green() || color1.blue() != color2.blue());
    }

    Script::Script(const string &filename) : image(nullptr), input(filename)
    {
    }
    void Script::clear_image_if_any()
    {
        if (image != nullptr)
        {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script()
    {
        clear_image_if_any();
    }

    void Script::run()
    {
        string command;
        while (input >> command)
        {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open")
            {
                open();
                continue;
            }
            if (command == "blank")
            {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save")
            {
                save();
                continue;
            }
            if (command == "invert")
            {
                invert();
                continue;
            }
            if (command == "to_gray_scale")
            {
                to_gray_scale();
                continue;
            }
            if (command == "replace")
            {
                replace();
                continue;
            }
            if (command == "h_mirror")
            {
                h_mirror();
                continue;
            }
            if (command == "v_mirror")
            {
                v_mirror();
                continue;
            }
            if (command == "fill")
            {
                fill();
                continue;
            }
            if (command == "add")
            {
                add();
                continue;
            }
            if (command == "crop")
            {
                crop();
                continue;
            }
            // TODO ...
        }
    }
    void Script::open()
    {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank()
    {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save()
    {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }

    void Script::invert()
    {
        // Iterate over each pixel and invert it.
        for (int i = 0; i < this->image->height(); i++)
        {
            for (int j = 0; j < this->image->width(); j++)
            {
                Color *curr_pixel = &this->image->at(j, i);
                curr_pixel->red() = 255 - curr_pixel->red();
                curr_pixel->green() = 255 - curr_pixel->green();
                curr_pixel->blue() = 255 - curr_pixel->blue();
            }
        }
    }

    void Script::to_gray_scale()
    {
        // Iterate over each pixel and convert it to gray scale using (r + g + b) / 3
        for (int i = 0; i < this->image->height(); i++)
        {
            for (int j = 0; j < this->image->width(); j++)
            {
                Color *curr_pixel = &this->image->at(j, i);
                rgb_value final_color = (curr_pixel->red() + curr_pixel->green() + curr_pixel->blue()) / 3;
                curr_pixel->red() = final_color;
                curr_pixel->green() = final_color;
                curr_pixel->blue() = final_color;
            }
        }
    }

    void Script::replace()
    {
        Color color1, color2;
        input >> color1 >> color2;
        for (int i = 0; i < this->image->height(); i++)
        {
            for (int j = 0; j < this->image->width(); j++)
            {
                Color *curr_pixel = &this->image->at(j, i);
                cout << curr_pixel->red() << " " << curr_pixel->green() << " " << curr_pixel->blue() << "\n";
                if (*curr_pixel == color1)
                {
                    *curr_pixel = color2;
                }
            }
        }
    }

    void Script::h_mirror()
    {
        // Iterate over width / 2 and swap it with the corresponding pixel
        for (int i = 0; i < this->image->height(); i++)
        {
            for (int j = 0; j < (this->image->width() / 2); j++)
            {
                Color *pixel1 = &this->image->at(j, i);
                Color *pixel2 = &this->image->at(this->image->width() - 1 - j, i);
                Color temp = *pixel2;
                *pixel2 = *pixel1;
                *pixel1 = temp;
            }
        }
    }
    void Script::v_mirror()
    {
        // Iterate over height / 2 and swap it with the corresponding pixel
        for (int i = 0; i < this->image->height() / 2; i++)
        {
            for (int j = 0; j < this->image->width(); j++)
            {
                Color *pixel1 = &this->image->at(j, i);
                Color *pixel2 = &this->image->at(j, this->image->height() - 1 - i);
                Color safe = *pixel2;
                *pixel2 = *pixel1;
                *pixel1 = safe;
            }
        }
    }

    void Script::fill()
    {
        // Iterate over each pixel ranging from y until y + h and from x until x + w and replace it with the fill color.
        int x, y, w, h;
        Color c;
        input >> x >> y >> w >> h >> c;
        for (int i = y; i < (y + h); i++)
        {
            for (int j = x; j < (w + x); j++)
            {
                Color *curr_pixel = &this->image->at(j, i);
                curr_pixel->show();
                curr_pixel->red() = c.red();
                curr_pixel->green() = c.green();
                curr_pixel->blue() = c.blue();
            }
        }
    }

    void Script::add()
    {
        // Iterate over each pixel of the new image, if it's different from the neutral color add it to the original color
        string filename;
        Color c;
        int x, y;
        input >> filename;
        input >> c;
        input >> x >> y;
        Image *loaded_img = loadFromPNG(filename);
        int w = loaded_img->width();
        int h = loaded_img->height();
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                if (c != loaded_img->at(j, i))
                {
                    this->image->at(j + x, i + y) = loaded_img->at(j, i);
                }
            }
        }
        delete loaded_img;
    }

    void Script::crop()
    {
        // Create new image to hold the cropped image, then replace the original image with the cropped image
        int x, y, w, h;
        input >> x >> y >> w >> h;
        Image *new_img = new Image(w, h);
        // Iterate over each pixel from the new image and change it to match the original image
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                new_img->at(j, i) = this->image->at(j + x, i + y);
            }
        }
        Image *temporary_addr = this->image;
        this->image = new_img;
        delete temporary_addr;
    }
}
