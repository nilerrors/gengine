//
// Created by nilerrors on 3/22/24.
//

#ifndef ENGINE_WIREFRAMEZBUF_H
#define ENGINE_WIREFRAMEZBUF_H

#include "../lib/ini_configuration.h"
#include "../lib/vector3d.h"
#include "wireframes.h"
#include "utils.h"
#include "zbuffer.h"

namespace gengine
{

class ZBufferedWireframe : public Wireframe
{
public:
    ZBufferedWireframe();

    explicit ZBufferedWireframe(const ini::Configuration &config, bool lighted = false);

    ~ZBufferedWireframe();

    const Lines2D &doProjection() override;

    static Point2D doProjection(const Vector3D &point, double d, double *z);

    static Point2D doProjection(const Vector3D &point, double d, double *z, double dx, double dy);

    const img::EasyImage &drawWireframe() override;

protected:
    ZBuffer zBuffer;
};

}

#endif //ENGINE_WIREFRAMEZBUF_H
