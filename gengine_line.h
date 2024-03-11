//
// Created by nilerrors on 2/21/24.
//

#ifndef ENGINE_LINE_H
#define ENGINE_LINE_H

#include <cmath>
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

void calculate_outer_most_pixels(const Lines2D &lines, OuterMostPixels *pixels);


class Draw2DLSystem
{
public:
	Draw2DLSystem(const ini::Section &generalConfig, const ini::Section &LSystem2DConfig);
	virtual ~Draw2DLSystem();

	static img::EasyImage draw2DLines(const Lines2D &lines, int size, Color backgroundColor)
	{
		OuterMostPixels outer_most_pixels;
		calculate_outer_most_pixels(lines, &outer_most_pixels);
		double x_range = outer_most_pixels.max.x - outer_most_pixels.min.x;
		double y_range = outer_most_pixels.max.y - outer_most_pixels.min.y;
		double image_x = size * (x_range / std::fmax(x_range, y_range));
		double image_y = size * (y_range / std::fmax(x_range, y_range));

		auto* image = new img::EasyImage(lround(image_x), lround(image_y), backgroundColor.to_img_color());

		double scale_factor = 0.95 * (image_x / x_range);
		double DC_x = scale_factor * (outer_most_pixels.min.x + outer_most_pixels.max.x) / 2;
		double DC_y = scale_factor * (outer_most_pixels.min.y + outer_most_pixels.max.y) / 2;
		double d_x = (image_x / 2) - DC_x;
		double d_y = (image_y / 2) - DC_y;

		for (auto &line : lines)
		{
			image->draw_line(lround(line.p1.x * scale_factor + d_x), lround(line.p1.y * scale_factor + d_y),
			                 lround(line.p2.x * scale_factor + d_x), lround(line.p2.y * scale_factor + d_y),
							 line.color.to_img_color());
		}
		return *image;
	}
	void draw2DLines();
	const img::EasyImage &drawLSystem();
	void applyReplacement();

private:
	img::EasyImage* image = nullptr;
	Lines2D lines;
	std::string full_replacement;
	LParser::LSystem2D l_system;

	int size = 0;
	Color backgroundColor;
	Color linesColor;
};

}

#endif //ENGINE_LINE_H
