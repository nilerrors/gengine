//
// Created by nilerrors on 5/7/24.
//

#ifndef ENGINE_LIGHTEDZBUFFERING_H
#define ENGINE_LIGHTEDZBUFFERING_H

#include "wireframeszbuftri.h"
#include "light.h"
#include "utils.h"

namespace gengine
{

class LightedZBuffering : public ZBuffering
{
public:
    LightedZBuffering();

    explicit LightedZBuffering(const ini::Configuration &config);

    ~LightedZBuffering() = default;

    const Lines2D &doProjection() override;

    const img::EasyImage &drawWireframe() override;

    void addLightFromConfig(const ini::Section &config);

    static void
    applyLighting(Light *light, const Color &diffuse, const Color &specular, double reflectionCoeff,
                  Color *pixel_color, const Vector3D &normal, double x, double y, double dx, double dy, double d,
                  double inverted_z);
};


}

#endif //ENGINE_LIGHTEDZBUFFERING_H
