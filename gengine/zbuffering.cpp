//
// Created by nilerrors on 3/27/24.
//

#include <cmath>
#include "zbuffering.h"
#include "line.h"

using namespace gengine;


ZBuffering::ZBuffering() = default;

ZBuffering::ZBuffering(const ini::Configuration &config) : ZBufferedWireframe(config)
{
    for (auto &figure: figures)
    {
        figure->triangulate();
    }
}

ZBuffering::~ZBuffering() = default;


const Lines2D &ZBuffering::doProjection()
{
    return ZBufferedWireframe::doProjection();
}

const img::EasyImage &ZBuffering::drawWireframe()
{
    doProjection();
    OuterMostPixels outer_most_pixels;
    calculate_outer_most_pixels(lines, &outer_most_pixels);
    double x_range = outer_most_pixels.max.x - outer_most_pixels.min.x;
    double y_range = outer_most_pixels.max.y - outer_most_pixels.min.y;
    double image_x = size * (x_range / std::fmax(x_range, y_range));
    double image_y = size * (y_range / std::fmax(x_range, y_range));

    image = img::EasyImage(std::lround(image_x), std::lround(image_y), backgroundColor.to_img_color());
    zBuffer = ZBuffer(image.get_width(), image.get_height());

    double scale_factor = 0.95 * (image_x / x_range);
    double DC_x = scale_factor * (outer_most_pixels.min.x + outer_most_pixels.max.x) / 2;
    double DC_y = scale_factor * (outer_most_pixels.min.y + outer_most_pixels.max.y) / 2;
    double d_x = (image_x / 2) - DC_x;
    double d_y = (image_y / 2) - DC_y;

    for (auto &figure: figures)
    {
        for (auto &face: figure->faces)
        {
            Vector3D A = figure->points[face.point_indexes[0]];
            Vector3D B = figure->points[face.point_indexes[1]];
            Vector3D C = figure->points[face.point_indexes[2]];

            zBuffer.draw_zbuf_triag(image, A, B, C, scale_factor, d_x, d_y, figure->color.to_img_color());
        }
    }

    return image;
}


void gengine::calculate_x_range(
        long y,
        gengine::Point2D a, gengine::Point2D b, gengine::Point2D c,
        long *x_min, long *x_max)
{
    double x_min_ab = INF;
    double x_min_ac = INF;
    double x_min_bc = INF;

    double x_max_ab = -INF;
    double x_max_ac = -INF;
    double x_max_bc = -INF;

    auto crosses = [&y](Point2D p, Point2D q) -> bool {
        return (p.y != q.y
                && (((double) y - p.y) * ((double) y - q.y) <= 0));
    };

    auto Xofi = [&y](Point2D p, Point2D q) -> double {
        return q.x + (p.x - q.x) * ((double) y - q.y) / (p.y - q.y);
    };

    if (crosses(a, b))
    {
        double xi = Xofi(a, b);
        if (xi < x_min_ab)
        {
            x_min_ab = xi;
        }
        if (xi > x_max_ab)
        {
            x_max_ab = xi;
        }
    }
    if (crosses(a, c))
    {
        double xi = Xofi(a, c);
        if (xi < x_min_ac)
        {
            x_min_ac = xi;
        }
        if (xi > x_max_ac)
        {
            x_max_ac = xi;
        }
    }
    if (crosses(b, c))
    {
        double xi = Xofi(b, c);
        if (xi < x_min_bc)
        {
            x_min_bc = xi;
        }
        if (xi > x_max_bc)
        {
            x_max_bc = xi;
        }
    }

    *x_min = std::lround(std::min(std::min(x_min_ab, x_min_ac), x_min_bc) + 0.5);
    *x_max = std::lround(std::max(std::max(x_max_ab, x_max_ac), x_max_bc) - 0.5);
}
