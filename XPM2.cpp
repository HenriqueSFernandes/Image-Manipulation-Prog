#include "XPM2.hpp"
#include <string>
#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>
#include <iomanip>

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

    string rgb_to_hexa(const Color &c)
    {
        // Convert a color to hexadecimal.
        stringstream ss;
        ss << setfill('0') << setw(2) << hex << static_cast<int>(c.red()) << setfill('0') << setw(2) << static_cast<int>(c.green()) << setfill('0') << setw(2) << static_cast<int>(c.blue());
        string s = ss.str();
        for (char &i : s)
        {
            i = toupper(i);
        }
        return "#" + s;
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

    void saveToXPM2(const std::string &filename, const Image *image)
    {
        ofstream file(filename);
        // Map that associates a color with a character.
        map<string, char> map_colors;
        int width = image->width();
        int height = image->height();
        // Stream to store characters.
        stringstream ss;
        int n_colors = 0;
        string curr_pixel;
        // Iterate over each pixel.
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                // Get the current color, if it isn't on the map add it to the map.
                curr_pixel = rgb_to_hexa(image->at(x, y));
                if (map_colors.find(curr_pixel) == map_colors.end())
                {
                    n_colors++;
                    map_colors[curr_pixel] = 'a' - 1 + n_colors;
                }
                // Add the characters to the stream.
                ss << map_colors[curr_pixel];
            }
            ss << '\n';
        }
        // Output to the attributes, color map and the characters to the file.
        file << "! XPM2\n";
        file << width << " " << height << " " << n_colors << " 1\n";
        for (auto i : map_colors)
        {
            file << i.second << " c " << i.first << '\n';
        }
        file << ss.str();
        file.close();
    }
}
