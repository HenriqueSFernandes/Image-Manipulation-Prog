#include "XPM2.hpp"
#include <string>
#include <algorithm>
#include <fstream>
#include <map>

using namespace std;
namespace prog
{
    Color hexa_to_rgb(const string &hexa_color)
    {
        // Read 2 characters at a time from the string and interpret them as base16.
        rgb_value red = stoi(hexa_color.substr(1, 2), nullptr, 16);
        rgb_value green = stoi(hexa_color.substr(3, 2), nullptr, 16);
        rgb_value blue = stoi(hexa_color.substr(5, 2), nullptr, 16);
        return {red, green, blue};
    }
    Image *loadFromXPM2(const std::string &filename)
    {
        ifstream file(filename);
        string format;
        getline(file, format);
        if (format != "! XPM2")
        {
            return nullptr;
        }
        // Read image attributes.
        int width, height, n_colors, char_per_color;
        file >> width >> height >> n_colors >> char_per_color;
        // Create a map that matches each character with a color.
        map<char, Color> map_colors;
        for (int i = 0; i < n_colors; i++)
        {
            char c;
            string color;
            file >> c;
            file.ignore(2);
            file >> color;
            map_colors[c] = hexa_to_rgb(color);
        }
        Image *img = new Image(width, height);
        // Iterate over each character and add the corresponding color to the image.
        for (int i = 0; i < height; i++)
        {
            string line;
            file >> line;
            for (int j = 0; j < width; j++)
            {
                img->at(j, i) = map_colors[line[j]];
            }
        }
        file.close();
        return img;
    }

    void saveToXPM2(const std::string &file, const Image *image)
    {
    }
}
