//
// Created by nilerrors on 2/21/24.
//

#include <cmath>
#include "easy_image.h"
#include "ini_configuration.h"

img::EasyImage ColorRectangle(const uint height, const uint width)
{
    img::EasyImage image(width, height);

    for (uint i = 0; i < height; i++)
    {
        for (uint j = 0; j < width; j++)
        {
            image(i, j).red = i;
            image(i, j).green = j;
            image(i, j).blue = (i + j) % 256;
        }
    }

    return image;
}

img::EasyImage ColorRectangleBlocks(const uint imageWidth, const uint imageHeight, ini::Section blockProperties)
{
    const auto colorWhite = blockProperties["colorWhite"].as_double_tuple_or_die();
    const auto colorBlack = blockProperties["colorBlack"].as_double_tuple_or_die();
    const uint widthBlock = imageWidth / blockProperties["nrXBlocks"].as_int_or_die();
    const uint heightBlock = imageHeight / blockProperties["nrYBlocks"].as_int_or_die();
    const bool invert = blockProperties["invertColors"].as_bool_or_die();

    auto calculateColor = [](double color, bool invert) {
        return std::lround(invert ? 255 - color * 255 : color * 255);
    };

    img::EasyImage image(imageWidth, imageHeight);

    for (uint i = 0; i < imageHeight; i++)
    {
        for (uint j = 0; j < imageWidth; j++)
        {
            if (((i / heightBlock) + (j / widthBlock)) % 2 == 0)
            {
                image(i, j).red = calculateColor(colorWhite[0], invert);
                image(i, j).green = calculateColor(colorWhite[1], invert);
                image(i, j).blue = calculateColor(colorWhite[2], invert);
            }
            else
            {
                image(i, j).red = calculateColor(colorBlack[0], invert);
                image(i, j).green = calculateColor(colorBlack[1], invert);
                image(i, j).blue = calculateColor(colorBlack[2], invert);
            }
        }
    }

    return image;
}

img::EasyImage QuarterCircle(const uint imageWidth, const uint imageHeight, ini::Section lineProperties)
{
    const uint lines = lineProperties["nrLines"].as_int_or_die();
    const auto lineColor = lineProperties["lineColor"].as_double_tuple_or_die();
    img::EasyImage image(imageWidth, imageHeight);

    image.draw_line(0, 0, 499, 499, img::Color(lineColor[0] * 255, lineColor[1] * 255, lineColor[2] * 255));

    return image;
}

img::EasyImage ex00(const ini::Configuration &configuration)
{
    const std::string imageType = configuration["General"]["type"].as_string_or_die();
    const uint imageWidth = configuration["ImageProperties"]["width"].as_int_or_die();
    const uint imageHeight = configuration["ImageProperties"]["height"].as_int_or_die();

    img::EasyImage image(imageWidth, imageHeight);

    if (imageType == "IntroColorRectangle")
    {
        image = ColorRectangle(imageWidth, imageHeight);
    }
    else if (imageType == "IntroBlocks")
    {
        image = ColorRectangleBlocks(imageWidth, imageHeight, configuration["BlockProperties"]);
    }
    else if (imageType == "IntroLines")
    {
        image = QuarterCircle(imageWidth, imageHeight, configuration["LineProperties"]);
    }

    return image;
}
