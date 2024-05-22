//
// Created by nilerrors on 5/7/24.
//

#ifndef ENGINE_LINE2D_H
#define ENGINE_LINE2D_H

#include "color.h"

namespace gengine
{

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

}

#endif //ENGINE_LINE2D_H
