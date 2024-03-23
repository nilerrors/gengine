//
// Created by nilerrors on 2/21/24.
//

#ifndef ENGINE_UTILS_H
#define ENGINE_UTILS_H

#include <list>
#include <vector>
#include "../lib/easy_image.h"
#include "../lib/vector3d.h"

namespace gengine
{

#define PI M_PI

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
		*this = color;
	}

	Color(double r, double g, double b)
	{
		red = r;
		green = g;
		blue = b;
	}

	Color &operator=(const COLOR color)
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
		return *this;
	}

	img::Color to_img_color() const
	{
		// Convert the color from a value of in between 0 and 1 to a value between 0 and 255
		return { static_cast<uint8_t>(red * 255),
				 static_cast<uint8_t>(green * 255),
				 static_cast<uint8_t>(blue * 255) };
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

	double z1;
	double z2;

	Line2D()
	{
		Line2D::p1 = Point2D(0, 0);
		Line2D::p2 = Point2D(0, 0);
		Line2D::color = Color(0, 0, 0);
		z1 = 0;
		z2 = 0;
	}

	Line2D(Point2D p1, Point2D p2)
	{
		Line2D::p1 = p1;
		Line2D::p2 = p2;
		Line2D::color = Color(1, 1, 1); // white
		z1 = 0;
		z2 = 0;
	}

	Line2D(Point2D p1, Point2D p2, Color color)
	{
		Line2D::p1 = p1;
		Line2D::p2 = p2;
		Line2D::color = color;
		z1 = 0;
		z2 = 0;
	}

	Line2D(Point2D p1, Point2D p2, Color::COLOR color)
	{
		Line2D::p1 = p1;
		Line2D::p2 = p2;
		Line2D::color = Color(color);
		z1 = 0;
		z2 = 0;
	}
};

using Lines2D = std::vector<Line2D>;

double degToRad(double deg);


//////////////////////////////////////
// 3D L-System

class Face
{
public:
	//De indexen refereren naar
	// punten in de ‘points’ vector
	// van de Figure-klasse

	Face() = default;

	explicit Face(const std::vector<int> &point_indexes) : point_indexes(point_indexes) { }

	Face(int p1, int p2)
	{
		point_indexes.push_back(p1);
		point_indexes.push_back(p2);
	}

	std::vector<int> point_indexes;
};

class Figure
{
public:
	Figure() : color(Color::WHITE) {}

	std::vector<Vector3D> points;
	std::vector<Face> faces;
	Color color;
};

using Figures3D = std::vector<Figure>;

}

#endif //ENGINE_UTILS_H
