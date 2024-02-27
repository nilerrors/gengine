//
// Created by nilerrors on 2/21/24.
//

#include <cmath>
#include "gengine_line.h"

using namespace gengine;

static OuterMostPixels gengine::calculate_outer_most_pixels(const Lines2D& lines)
{
	OuterMostPixels pixels;

	for (const Line2D& line : lines)
	{
		// max
		if (line.p1.x > pixels.max.x)
			pixels.max.x = line.p1.x;
		if (line.p2.x > pixels.max.x)
			pixels.max.x = line.p2.x;
		if (line.p1.y > pixels.max.y)
			pixels.max.y = line.p1.y;
		if (line.p2.y > pixels.max.y)
			pixels.max.y = line.p2.y;
		// min
		if (line.p1.x < pixels.min.x)
			pixels.min.x = line.p1.x;
		if (line.p2.x < pixels.min.x)
			pixels.min.x = line.p2.x;
		if (line.p1.y < pixels.min.y)
			pixels.min.y = line.p1.y;
		if (line.p2.y < pixels.min.y)
			pixels.min.y = line.p2.y;
	}

	return pixels;
}

img::EasyImage gengine::draw2DLines(Lines2D& lines, int size, const Color &bgColor)
{
	OuterMostPixels outer_most_pixels = calculate_outer_most_pixels(lines);
	double x_range = outer_most_pixels.max.x - outer_most_pixels.min.x;
	double y_range = outer_most_pixels.max.y - outer_most_pixels.min.y;
	double image_x = size * (x_range/fmax(x_range, y_range));
	double image_y = size * (y_range/fmax(x_range, y_range));
	double scale_factor = 0.95 * (image_x / x_range);

	std::cout << "image height " << image_x << std::endl;
	std::cout << "image width " << image_y << std::endl;

	img::EasyImage image(lround(image_x), lround(image_y));

	double DC_x = scale_factor * (outer_most_pixels.min.x + outer_most_pixels.max.x) / 2;
	double DC_y = scale_factor * (outer_most_pixels.min.y + outer_most_pixels.max.y) / 2;
	double d_x = (image_x / 2) - DC_x;
	double d_y = (image_y / 2) - DC_y;

	for (int i = 0; i < image.get_width(); i++)
	{
		for (int j = 0; j < image.get_height(); j++)
		{
			image(i, j).red = bgColor.to_img_color().red;
			image(i, j).green = bgColor.to_img_color().green;
			image(i, j).blue = bgColor.to_img_color().blue;
		}
	}

	for (auto &line : lines)
	{
		// mulitply all coordinates by scale_factor
		line.p1.x = line.p1.x * scale_factor + d_x;
		line.p1.y = line.p1.y * scale_factor + d_y;
		line.p2.x = line.p2.x * scale_factor + d_x;
		line.p2.y = line.p2.y * scale_factor + d_y;
	}

	for (const Line2D& line : lines)
	{
		image.draw_line(lround(line.p1.x), lround(line.p1.y), lround(line.p2.x), lround(line.p2.y), line.color.to_img_color());
	}

	return image;
}

void gengine::handle2DLSystem(img::EasyImage &image, const ini::Section &generalConfig, const ini::Section &LSystem2DConfig)
{
	const int size = generalConfig["size"].as_int_or_die();
	const Color backgroundColor = Color(generalConfig["backgroundcolor"].as_double_tuple_or_die());

	Lines2D lines;
	lines.push_back(Line2D(Point2D(0, 0), Point2D(90, 90)));
	lines.push_back(Line2D(Point2D(0, 90), Point2D(90, 0)));
	lines.push_back(Line2D(Point2D(0, 180), Point2D(20, 43), Color::RED));
	lines.push_back(Line2D(Point2D(0, 180), Point2D(20, 130), Color::GREEN));

	draw2DLines(lines, size, backgroundColor);
}

Lines2D gengine::drawLSystem(const LParser::LSystem2D &lSystem)
{
	Lines2D lines;


	return lines;
}
