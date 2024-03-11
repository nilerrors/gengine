//
// Created by nilerrors on 2/21/24.
//

#include <cmath>
#include <stack>
#include <fstream>
#include <sstream>
#include "gengine_line.h"

using namespace gengine;


Draw2DLSystem::~Draw2DLSystem()
{
	delete image;
	image = nullptr;
}

void gengine::calculate_outer_most_pixels(const Lines2D &lines, OuterMostPixels *pixels)
{
	for (const Line2D &line : lines)
	{
		// max
		if (line.p1.x > pixels->max.x)
			pixels->max.x = line.p1.x;
		if (line.p2.x > pixels->max.x)
			pixels->max.x = line.p2.x;
		if (line.p1.y > pixels->max.y)
			pixels->max.y = line.p1.y;
		if (line.p2.y > pixels->max.y)
			pixels->max.y = line.p2.y;
		// min
		if (line.p1.x < pixels->min.x)
			pixels->min.x = line.p1.x;
		if (line.p2.x < pixels->min.x)
			pixels->min.x = line.p2.x;
		if (line.p1.y < pixels->min.y)
			pixels->min.y = line.p1.y;
		if (line.p2.y < pixels->min.y)
			pixels->min.y = line.p2.y;
	}
}

Draw2DLSystem::Draw2DLSystem(const ini::Section &generalConfig, const ini::Section &LSystem2DConfig)
{
	size = generalConfig["size"].as_int_or_die();
	backgroundColor = Color(generalConfig["backgroundcolor"].as_double_tuple_or_die());
	linesColor = Color(LSystem2DConfig["color"].as_double_tuple_or_die());

	std::ifstream lSystemFile(LSystem2DConfig["inputfile"].as_string_or_die());
	if (lSystemFile.fail())
		return;

	lSystemFile >> l_system;
	lSystemFile.close();
}


void gengine::Draw2DLSystem::draw2DLines()
{
	OuterMostPixels outer_most_pixels;
	calculate_outer_most_pixels(lines, &outer_most_pixels);
	double x_range = outer_most_pixels.max.x - outer_most_pixels.min.x;
	double y_range = outer_most_pixels.max.y - outer_most_pixels.min.y;
	double image_x = size * (x_range / fmax(x_range, y_range));
	double image_y = size * (y_range / fmax(x_range, y_range));

	delete image;
	image = nullptr;
	image = new img::EasyImage(lround(image_x), lround(image_y), backgroundColor.to_img_color());

	double scale_factor = 0.95 * (image_x / x_range);
	double DC_x = scale_factor * (outer_most_pixels.min.x + outer_most_pixels.max.x) / 2;
	double DC_y = scale_factor * (outer_most_pixels.min.y + outer_most_pixels.max.y) / 2;
	double d_x = (image_x / 2) - DC_x;
	double d_y = (image_y / 2) - DC_y;

	for (auto &line : lines)
	{
		line.p1.x = line.p1.x * scale_factor + d_x;
		line.p1.y = line.p1.y * scale_factor + d_y;
		line.p2.x = line.p2.x * scale_factor + d_x;
		line.p2.y = line.p2.y * scale_factor + d_y;

		image->draw_line(lround(line.p1.x), lround(line.p1.y),
		                lround(line.p2.x), lround(line.p2.y), line.color.to_img_color());
	}
}

void gengine::Draw2DLSystem::applyReplacement()
{
	for (unsigned int i = 0; i < l_system.get_nr_iterations(); i++)
	{
		std::string full_string;

		for (auto c : full_replacement)
		{
			switch (c)
			{
				case '-':
				case '+':
				case '(':
				case ')':
				{
					full_string.push_back(c);
					break;
				}
				default:
				{
					full_string += l_system.get_replacement(c);
					break;
				}
			}
		}

		full_replacement = full_string;
	}
}

const img::EasyImage &gengine::Draw2DLSystem::drawLSystem()
{
	Point2D currentPosition(0, 0);
	double currentAngle = l_system.get_starting_angle();
	std::stack<std::pair<Point2D, double>> stackedPositions;

	full_replacement = l_system.get_initiator();
	applyReplacement();

	for (auto c : full_replacement)
	{
		switch (c)
		{
			case '+':
			{
				currentAngle += l_system.get_angle();
				break;
			}
			case '-':
			{
				currentAngle -= l_system.get_angle();
				break;
			}
			case '(':
			{
				stackedPositions.emplace(currentPosition, currentAngle);
				break;
			}
			case ')':
			{
				auto position = stackedPositions.top();
				stackedPositions.pop();
				currentPosition = position.first;
				currentAngle = position.second;
				break;
			}
			default:
			{
				Point2D next_position = currentPosition;
				next_position.x += std::cos(degToRad(currentAngle));
				next_position.y += std::sin(degToRad(currentAngle));

				if (l_system.draw(c))
				{
					Line2D line;
					line.p1 = currentPosition;
					line.p2 = next_position;
					line.color = linesColor;
					lines.push_back(line);
				}

				currentPosition = next_position;
				break;
			}
		}
	}

	draw2DLines();
	return *image;
}
