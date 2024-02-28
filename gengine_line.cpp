//
// Created by nilerrors on 2/21/24.
//

#include <cmath>
#include <stack>
#include <fstream>
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

	img::EasyImage image(lround(image_x), lround(image_y));

	double DC_x = scale_factor * (outer_most_pixels.min.x + outer_most_pixels.max.x) / 2;
	double DC_y = scale_factor * (outer_most_pixels.min.y + outer_most_pixels.max.y) / 2;
	double d_x = (image_x / 2) - DC_x;
	double d_y = (image_y / 2) - DC_y;

	image.clear(bgColor.to_img_color());

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
		image.draw_line(lround(line.p1.x), lround(line.p1.y),
						lround(line.p2.x), lround(line.p2.y), line.color.to_img_color());
	}

	return image;
}

void gengine::handle2DLSystem(img::EasyImage &image,
							  const ini::Section &generalConfig, const ini::Section &LSystem2DConfig)
{
	const int size = generalConfig["size"].as_int_or_die();
	const Color backgroundColor = Color(generalConfig["backgroundcolor"].as_double_tuple_or_die());
	const auto lSystemInputFile = LSystem2DConfig["inputfile"].as_string_or_die();
	const Color linesColor = Color(LSystem2DConfig["color"].as_double_tuple_or_die());

	Lines2D lines;
	LParser::LSystem2D l_system;
	std::ifstream lSystemFile(lSystemInputFile);

	if (lSystemFile.fail())
		return;

	lSystemFile >> l_system;
	lSystemFile.close();

	lines = drawLSystem(l_system, linesColor);
	image = draw2DLines(lines, size, backgroundColor);
}

std::string applyReplacement(const std::string& string, const LParser::LSystem2D &lSystem)
{
	std::string full_string;

	for (auto c : string)
	{
		if (c == '-' || c == '+' || c == '(' || c == ')')
			full_string += c;
		else
			full_string += lSystem.get_replacement(c);
	}

	return full_string;
}

Lines2D gengine::drawLSystem(const LParser::LSystem2D &lSystem, const Color &color)
{
	Lines2D lines;
	Point2D currentPosition(0, 0);
	double currentAngle = lSystem.get_starting_angle();
	std::stack<std::pair<Point2D, double>> stackedPositions;

	std::string full_string = lSystem.get_initiator();
	for (unsigned int i = 0; i < lSystem.get_nr_iterations(); i++)
	{
		full_string = applyReplacement(full_string, lSystem);
	}

	for (auto c : full_string)
	{
		if (c == '+')
		{
			currentAngle += lSystem.get_angle();
			continue;
		}
		else if (c == '-')
		{
			currentAngle -= lSystem.get_angle();
			continue;
		}
		else if (c == '(')
		{
			stackedPositions.push(std::make_pair(currentPosition, currentAngle));
			continue;
		}
		else if (c == ')')
		{
			auto position = stackedPositions.top();
			stackedPositions.pop();
			currentPosition = position.first;
			currentAngle = position.second;
			continue;
		}

		Point2D next_position;
		next_position.x = currentPosition.x + std::cos(degToRad(currentAngle));
		next_position.y = currentPosition.y + std::sin(degToRad(currentAngle));

		if (lSystem.draw(c))
		{
			Line2D line;

			line.p1 = currentPosition;
			line.p2 = next_position;
			line.color = color;

			lines.push_back(line);
		}

		currentPosition = next_position;
	}

	return lines;
}
