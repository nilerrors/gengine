//
// Created by nilerrors on 3/21/24.
//

#include <cmath>
#include <functional>
#include "zbuffer.h"
#include "wireframeszbuftri.h"
#include "wireframezbuf.h"
#include "utils.h"
#include "light.h"
#include "wireframeszbuflight.h"

namespace gengine
{


void ZBuffer::draw_zbuf_line(
        img::EasyImage &image,
        long x0, long int y0, double z0,
        long x1, long int y1, double z1,
        const img::Color &color)
{
    // inspired by img::EasyImage::draw_line of ../lib/easy_image.cc

    assert(x0 < image.get_width() && y0 < image.get_height());
    assert(x1 < image.get_width() && y1 < image.get_height());

    const long y_min = std::min(y0, y1);
    const long y_max = std::max(y0, y1);
    const long x_min = std::min(x0, x1);
    const long x_max = std::max(x0, x1);

    if (x0 == x1)
    {
        //special case for x0 == x1
        long a = y_max - y_min;
        long i = a;
        for (uint j = y_min; j <= y_max; j++)
        {
            double ia = (double) i / (double) a;
            double z = (ia / z0) + ((1 - ia) / z1);

            if (z < ati(x0, j))
            {
                seti(x0, j, z);
                image(x0, j) = color;
            }
            i--;
        }
    }
    else if (y0 == y1)
    {
        //special case for y0 == y1
        long a = x_max - x_min;
        long i = a;
        for (uint j = x_min; j <= x_max; j++)
        {
            double ia = (double) i / (double) a;
            double z = (ia / z0) + ((1.0 - ia) / z1);

            if (z < ati(j, y0))
            {
                seti(j, y0, z);
                image(j, y0) = color;
            }
            i--;
        }
    }
    else
    {
        if (x0 > x1)
        {
            //flip points if x1>x0: we want x0 to have the lowest value
            std::swap(x0, x1);
            std::swap(y0, y1);
            std::swap(z0, z1);
        }

        double m = ((double) y1 - (double) y0) / ((double) x1 - (double) x0);
        if (-1.0 <= m && m <= 1.0)
        {
            long a = (x1 - x0);
            long i = a;
            for (uint j = 0; j <= (x1 - x0); j++)
            {
                double ia = (double) i / (double) a;
                double z = (ia / z0) + ((1 - ia) / z1);

                if (z < ati(x0 + j, (uint) std::round(y0 + m * j)))
                {
                    seti(x0 + j, (uint) std::round(y0 + m * j), z);
                    image(x0 + j, (uint) std::round(y0 + m * j)) = color;
                }
                i--;
            }
        }
        else if (m > 1.0)
        {
            long a = (y1 - y0);
            long i = a;
            for (uint j = 0; j <= (y1 - y0); j++)
            {
                double ia = (double) i / (double) a;
                double z = (ia / z0) + ((1 - ia) / z1);

                if (z < ati((uint) std::round(x0 + (j / m)), y0 + j))
                {
                    seti((uint) std::round(x0 + (j / m)), y0 + j, z);
                    image((uint) std::round(x0 + (j / m)), y0 + j) = color;
                }
                i--;
            }
        }
        else if (m < -1.0)
        {
            long a = (y0 - y1);
            long i = a;
            for (uint j = 0; j <= (y0 - y1); j++)
            {
                double ia = (double) i / (double) a;
                double z = (ia / z0) + ((1 - ia) / z1);

                if (z < ati((uint) std::round(x0 - (j / m)), y0 - j))
                {
                    seti((uint) std::round(x0 - (j / m)), y0 - j, z);
                    image((uint) std::round(x0 - (j / m)), y0 - j) = color;
                }
                i--;
            }
        }
    }
}

void ZBuffer::draw_zbuf_triag(
        img::EasyImage &image,
        Vector3D const &A, Vector3D const &B, Vector3D const &C,
        double d, double dx, double dy,
        const Color &ambient, const Color &diffuse, const Color &specular,
        double reflectionCoeff, Lights3D const &lights)
{
    double az = 0.0;
    double bz = 0.0;
    double cz = 0.0;
    Point2D a = ZBufferedWireframe::doProjection(A, d, &az, dx, dy);
    Point2D b = ZBufferedWireframe::doProjection(B, d, &bz, dx, dy);
    Point2D c = ZBufferedWireframe::doProjection(C, d, &cz, dx, dy);

    Vector3D g = Vector3D::point((a.x + b.x + c.x) / 3.0, (a.y + b.y + c.y) / 3.0,
                                 (1.0 / (3.0 * A.z)) + (1.0 / (3.0 * B.z)) + (1.0 / (3.0 * C.z)));


    Vector3D normal = Vector3D::cross(B - A, C - A);
    normal.normalise();

    double w1 = normal.x;
    double w2 = normal.y;
    double w3 = normal.z;
    double k = w1 * A.x + w2 * A.y + w3 * A.z;

    double dzdx = w1 / (-d * k);
    double dzdy = w2 / (-d * k);

    long ymin = std::lround(std::min(std::min(a.y, b.y), c.y) + 0.5);
    long ymax = std::lround(std::max(std::max(a.y, b.y), c.y) - 0.5);

    Color triangle_color = Color();

    if (lighted)
    {
        for (Light *light: lights)
        {
            triangle_color.red += light->ambientLight.red * ambient.red;
            triangle_color.green += light->ambientLight.green * ambient.green;
            triangle_color.blue += light->ambientLight.blue * ambient.blue;

            if (light->type == Light::Type::Inf)
            {
                Vector3D l = -1 * light->ldVector;
                l.normalise();

                double dot = Vector3D::dot(normal, l);
                if (dot < 0)
                {
                    dot = 0;
                }
                triangle_color.red += light->diffuseLight.red * diffuse.red * dot;
                triangle_color.green += light->diffuseLight.green * diffuse.green * dot;
                triangle_color.blue += light->diffuseLight.blue * diffuse.blue * dot;
            }
        }
    }

    for (long y = ymin; y <= ymax; y++)
    {
        long xmin;
        long xmax;

        calculate_x_range(y, a, b, c, &xmin, &xmax);

        for (long x = xmin; x <= xmax; x++)
        {
            double z = (1.000001 * g.z) + (x - g.x) * dzdx + (y - g.y) * dzdy;

            if (z < ati(x, y))
            {
                seti(x, y, z);
                if (!lighted)
                {
                    image(x, y) = ambient.to_img_color();
                    continue;
                }

                Color pixel_color = triangle_color;
                for (Light *light: lights)
                {
                    LightedZBuffering::applyLight(
                            light, diffuse, specular, reflectionCoeff, &pixel_color, normal, x, y, dx, dy, d, 1.0 / z);
                }
                image(x, y) = pixel_color.to_img_color();
            }
        }
    }
}

} // gengine