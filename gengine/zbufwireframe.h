//
// Created by nilerrors on 3/22/24.
//

#ifndef ENGINE_ZBUFWIREFRAME_H
#define ENGINE_ZBUFWIREFRAME_H

#include "../lib/ini_configuration.h"
#include "../lib/vector3d.h"
#include "line3d.h"
#include "utils.h"
#include "zbuffer.h"

namespace gengine
{

class ZBufferedWireframe : public Wireframe
{
public:
    ZBufferedWireframe();

    explicit ZBufferedWireframe(const ini::Configuration &config);

    ~ZBufferedWireframe();

    const Lines2D &doProjection() override;

    static Point2D doProjection(const Vector3D &point, double d, double *z);

    static Point2D doProjection(const Vector3D &point, double d, double *z, double dx, double dy);

    const img::EasyImage &drawWireframe() override;

protected:
    ZBuffer zBuffer;
};

}

#endif //ENGINE_ZBUFWIREFRAME_H
