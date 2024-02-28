//
// Created by nilerrors on 2/21/24.
//

#ifndef ENGINE_LINE_H
#define ENGINE_LINE_H

#include "l_parser.h"
#include "ini_configuration.h"
#include "easy_image.h"
#include "gengine_utils.h"
#include "gengine_utils.h"

namespace gengine
{

struct OuterMostPixels
{
	Point2D min;
	Point2D max;
};

static OuterMostPixels calculate_outer_most_pixels(const Lines2D &lines);

img::EasyImage draw2DLines(Lines2D &lines, int size, const Color &bgColor);
Lines2D drawLSystem(const LParser::LSystem2D &lSystem, const Color &color);
void handle2DLSystem(img::EasyImage &image, const ini::Section &generalConfig, const ini::Section &LSystem2DConfig);

}

#endif //ENGINE_LINE_H
