#include "Image.hpp"
namespace prog
{
  Image::Image(int w, int h, const Color &fill)
  {
    this->height_ = h;
    this->width_ = w;
    this->img = new Color *[h]; // allocate memory for array
    for (int i = 0; i < h; i++)
    {
      this->img[i] = new Color[w]; // for each line create new array
      for (int j = 0; j < w; j++)
      {
        this->img[i][j] = fill; // change color to fill color
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

  Color &Image::at(int x, int y)
  {
    return this->img[y][x];
  }

  const Color &Image::at(int x, int y) const
  {
    return this->img[y][x];
  }
}
