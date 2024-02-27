//
// Created by nilerrors on 2/21/24.
//

#ifndef ENGINE_UTILS_H
#define ENGINE_UTILS_H

#include <list>
#include <vector>
#include "easy_image.h"

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

	Color(COLOR color)
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

	img::Color to_img_color() const
	{
		// Convert the color from a value of in between 0 and 1 to a value between 0 and 255
		return img::Color(red * 255, green * 255, blue * 255);
	}
};

struct Point2D
{
	double x;
	double y;

	Point2D()
	{
		x = 0;
		y = 0;
	}

	Point2D(double x, double y)
	{
		Point2D::x = x;
		Point2D::y = y;
	}
};

struct Line2D
{
	Point2D p1;
	Point2D p2;
	Color color;

	Line2D(Point2D p1, Point2D p2)
	{
		Line2D::p1 = p1;
		Line2D::p2 = p2;
		Line2D::color = Color(1, 1, 1); // white
	}

	Line2D(Point2D p1, Point2D p2, Color color)
	{
		Line2D::p1 = p1;
		Line2D::p2 = p2;
		Line2D::color = color;
	}

	Line2D(Point2D p1, Point2D p2, Color::COLOR color)
	{
		Line2D::p1 = p1;
		Line2D::p2 = p2;
		Line2D::color = Color(color);
	}
};

using Lines2D = std::list<Line2D>;


}

#endif //ENGINE_UTILS_H
