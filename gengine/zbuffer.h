//
// Created by nilerrors on 3/21/24.
//

#ifndef ENGINE_ZBUFFER_H
#define ENGINE_ZBUFFER_H

#include <vector>
#include <cassert>
#include "utils.h"
#include "light.h"

namespace gengine
{


class ZBuffer
{
public:
    ZBuffer() = default;

    ZBuffer(uint width, uint height) : width(width), height(height)
    {
        zbuf = std::vector<double>(width * height, INF);
    }

    const double &ati(uint x, uint y) const
    {
        assert(y < height && x < width);
        return zbuf[y * width + x];
    }

    double seti(uint x, uint y, double z)
    {
        assert(y < height && x < width);
        zbuf[y * width + x] = z;
        return z;
    }

    void draw_zbuf_line(img::EasyImage &image,
                        long x0, long y0, double z0,
                        long x1, long y1, double z1,
                        const img::Color &color);

    void draw_zbuf_triag(img::EasyImage &image,
                         Vector3D const &A, Vector3D const &B, Vector3D const &C,
                         double d, double dx, double dy,
                         const Color &ambient, const Color &diffuse = Color(0, 0, 0),
                         const Color &specular = Color(0, 0, 0),
                         double reflectionCoeff = 0.0, Lights3D const &lights = {});

    void set_lighted(bool light)
    {
        lighted = light;
    }

    bool is_lighted() const
    {
        return lighted;
    }

private:
    std::vector<double> zbuf;

    bool lighted = false;
    uint width = 0;
    uint height = 0;
};


} // gengine

#endif //ENGINE_ZBUFFER_H
