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


class Draw2DLSystem
{
public:
	Draw2DLSystem(const ini::Section &generalConfig, const ini::Section &LSystem2DConfig);

	void draw2DLines();
	const img::EasyImage &drawLSystem();
	void applyReplacement();

private:
	img::EasyImage image;
	Lines2D lines;
	std::string full_replacement;
	LParser::LSystem2D l_system;

	int size;
	Color backgroundColor;
	Color linesColor;
};

}

#endif //ENGINE_LINE_H
