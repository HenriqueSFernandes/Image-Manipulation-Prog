#include "Image.hpp"

namespace prog
{
  Image::Image(int w, int h, const Color &fill)
  {
    this->img = new Color *[h];
    for (int i = 0; i < h; i++)
    {
      this->img[i] = new Color[w];
      for (int j = 0; j < w; j++)
      {
        this->img[i][j] = fill;
      }
    }
  }

  Image::~Image()
  {
    for (int i = 0; i < this->height_; i++)
    {
      delete[] this->img[i];
    }
    delete[] this->img;
  }

  int Image::width() const
  {
    return this->width_;
  }

  int Image::height() const
  {
    return this->height_;
  }

  // TODO: remove this DUMMY_color variable once you have appropriate fields for representing image pixels.
  Color DUMMY_color;

  Color &Image::at(int x, int y)
  {
    return this->img[x][y];
  }

  const Color &Image::at(int x, int y) const
  {
    return this->img[x][y];
  }
}
