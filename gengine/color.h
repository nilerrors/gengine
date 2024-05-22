//
// Created by nilerrors on 5/7/24.
//

#ifndef ENGINE_COLOR_H
#define ENGINE_COLOR_H

#include <cstdint>
#include <vector>
#include "../lib/easy_image.h"

namespace gengine
{

struct Color
{
    // Values can be in between 0 and 1 (inclusive)
    double red;
    double green;
    double blue;

    enum COLOR
    {
        WHITE,
        BLACK,
        RED,
        GREEN,
        BLUE
    };

    Color()
    {
        red = 0;
        green = 0;
        blue = 0;
    }

    explicit Color(std::vector<double> color)
    {
        red = color[0];
        green = color[1];
        blue = color[2];
    }

    explicit Color(COLOR color)
    {
        switch (color)
        {
        case WHITE:
            red = 1;
            green = 1;
            blue = 1;
            break;
        case BLACK:
            red = 0;
            green = 0;
            blue = 0;
            break;
        case RED:
            red = 1;
            green = 0;
            blue = 0;
            break;
        case GREEN:
            red = 0;
            green = 1;
            blue = 0;
            break;
        case BLUE:
            red = 0;
            green = 0;
            blue = 1;
            break;
        }

    }

    Color(double r, double g, double b)
    {
        red = r;
        green = g;
        blue = b;
    }

    Color &operator=(const COLOR color)
    {
        *this = Color(color);
        return *this;
    }

    img::Color to_img_color() const
    {
        // Convert the color from a value of in between 0 and 1 to a value between 0 and 255
        return {static_cast<uint8_t>(red * 255),
                static_cast<uint8_t>(green * 255),
                static_cast<uint8_t>(blue * 255)};
    }
};

}

#endif //ENGINE_COLOR_H
