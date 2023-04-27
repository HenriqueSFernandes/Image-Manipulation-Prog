#include "Color.hpp"
#include <iostream>
namespace prog
{
    Color::Color()
    {
        this->red_ = 0;
        this->green_ = 0;
        this->blue_ = 0;
    }

    Color::Color(const Color &other)
    {
        this->red_ = other.red_;
        this->green_ = other.green_;
        this->blue_ = other.blue_;
    }

    Color::Color(rgb_value red, rgb_value green, rgb_value blue)
    {
        this->red_ = red;
        this->green_ = green;
        this->blue_ = blue;
    }

    rgb_value Color::red() const
    {
        return this->red_;
    }

    rgb_value Color::green() const
    {
        return this->green_;
    }

    rgb_value Color::blue() const
    {
        return this->blue_;
    }

    rgb_value &Color::red()
    {
        return this->red_;
    }

    rgb_value &Color::green()
    {
        return this->green_;
    }

    rgb_value &Color::blue()
    {
        return this->blue_;
    }
}
