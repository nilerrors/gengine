//
// Created by nilerrors on 3/22/24.
//

#include "wireframezbuf.h"
#include "l_systems2d.h"

using namespace gengine;

ZBufferedWireframe::ZBufferedWireframe() = default;

ZBufferedWireframe::ZBufferedWireframe(const ini::Configuration &config, bool lighted) : Wireframe(config, lighted)
{
    zBuffer.set_lighted(lighted);
}

ZBufferedWireframe::~ZBufferedWireframe() = default;

const Lines2D &ZBufferedWireframe::doProjection()
{
    for (auto &figure: figures)
    {
        // for now, we assume that there are exactly 2 points in a face
        double d = 1;
        for (auto &face: figure->faces)
        {
            if (face.point_indexes.size() < 2)
            {
                throw std::runtime_error("Face has less than 2 points");
            }
            for (size_t i = 0; i < face.point_indexes.size(); i++)
            {
                size_t next = i + 1;
                if (next >= face.point_indexes.size())
                {
                    next = 0;
                }

                double z1, z2;

                Line2D line = Line2D(
                        doProjection(figure->points[face.point_indexes[i]], d, &z1),
                        doProjection(figure->points[face.point_indexes[next]], d, &z2),
                        figure->ambientReflection);

                line.z1 = z1;
                line.z2 = z2;

                lines.push_back(line);
            }
        }
    }

    return lines;
}

const img::EasyImage &ZBufferedWireframe::drawWireframe()
{
    doProjection();
    image = Draw2DLSystem::drawZBuf2DLines(zBuffer, lines, size, backgroundColor);
    return image;
}

Point2D ZBufferedWireframe::doProjection(
        const Vector3D &point, const double d, double *z)
{
    return doProjection(point, d, z, 0, 0);
}


Point2D ZBufferedWireframe::doProjection(
        const Vector3D &point, const double d, double *z, double dx, double dy)
{
    Point2D new_point;

    new_point.x = (d * point.x) / (-point.z) + dx;
    new_point.y = (d * point.y) / (-point.z) + dy;
    *z = point.z;

    return new_point;
}
