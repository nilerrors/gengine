//
// Created by nilerrors on 3/27/24.
//

#ifndef ZBUFFERING_H
#define ZBUFFERING_H

#include "../lib/ini_configuration.h"
#include "../lib/vector3d.h"
#include "wireframes.h"
#include "utils.h"
#include "wireframezbuf.h"
#include "zbuffer.h"

namespace gengine
{

class ZBuffering : public ZBufferedWireframe
{
public:
    ZBuffering();

    explicit ZBuffering(const ini::Configuration &config, bool lighted = false);

    ~ZBuffering();

    const Lines2D &doProjection() override;

    const img::EasyImage &drawWireframe() override;

protected:
    Lights3D lights = {};
};


std::vector<Face> triangulate(const Face &face);

void calculate_x_range(long y, Point2D a, Point2D b, Point2D c, long *x_min, long *x_max);

} // gengine

#endif //ZBUFFERING_H
