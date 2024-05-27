//
// Created by nilerrors on 5/7/24.
//

#include <cmath>
#include "wireframeszbuflight.h"

using namespace gengine;

LightedZBuffering::LightedZBuffering() = default;

LightedZBuffering::LightedZBuffering(const ini::Configuration &config) : ZBuffering(config, true)
{
    for (int i = 0; i < config["General"]["nrLights"].as_int_or_die(); i++)
    {
        const ini::Section current_light_section = config["Light" + std::to_string(i)];

        addLightFromConfig(current_light_section);
    }
}

const Lines2D &LightedZBuffering::doProjection()
{
    return ZBuffering::doProjection();
}

const img::EasyImage &LightedZBuffering::drawWireframe()
{
    return ZBuffering::drawWireframe();
}

void LightedZBuffering::addLightFromConfig(const ini::Section &config)
{
    auto *light = new Light();
    light->ambientLight = Color(config["ambientLight"].as_double_tuple_or_die());

    std::vector<double> diffuse;
    std::vector<double> specular;
    bool diffuse_or_specular = false;
    if (config["diffuseLight"].as_double_tuple_if_exists(diffuse))
    {
        light->diffuseLight = Color(diffuse);
        diffuse_or_specular = true;
    }
    if (config["specularLight"].as_double_tuple_if_exists(specular))
    {
        light->specularLight = Color(specular);
        diffuse_or_specular = true;
    }

    if (diffuse_or_specular)
    {
        if (config["infinity"].as_bool_or_die())
        {
            light->ldVector = Vector3D::vector(config["direction"].as_double_tuple_or_die()[0],
                                               config["direction"].as_double_tuple_or_die()[1],
                                               config["direction"].as_double_tuple_or_die()[2]);
        }
        else
        {
            light->type = Light::Type::Point;
            light->location = Vector3D::point(config["location"].as_double_tuple_or_die()[0],
                                              config["location"].as_double_tuple_or_die()[1],
                                              config["location"].as_double_tuple_or_die()[2]);
            light->spotAngle = degToRad(config["spotAngle"].as_double_or_default(90));
        }
        light->eyePointMultiply(eyePoint);
    }

    lights.push_back(light);
}

void LightedZBuffering::applyLighting(
        Light *light,
        const Color &diffuse, const Color &specular,
        double reflectionCoeff, Color *pixel_color, const Vector3D &normal,
        double x, double y, double dx, double dy, double d, double inverted_z)
{
    int invz_d = -inverted_z / d;
    Vector3D p = Vector3D::point((x - dx) * invz_d, (y - dy) * invz_d, inverted_z);

    Vector3D l;
    double dot;
    switch (light->type)
    {
    case Light::Type::Inf:
    {
        l = -1 * light->ldVector;
        l.normalise();

        dot = std::max(0.0, Vector3D::dot(normal, l));
    }
        break;
    case Light::Type::Point:
    {
        l = light->location - p;
        l.normalise();

        dot = Vector3D::dot(normal, l);
        double angleSpot = std::cos(light->spotAngle);
        double intensity = 1.0 - ((1.0 - dot) / (1.0 - angleSpot));
        if (dot <= angleSpot)
        {
            intensity = 0.0;
        }
        pixel_color->red += light->diffuseLight.red * diffuse.red * intensity;
        pixel_color->green += light->diffuseLight.green * diffuse.green * intensity;
        pixel_color->blue += light->diffuseLight.blue * diffuse.blue * intensity;
    }
        break;
    }

    double specular_intensity =
            std::pow(std::max(0.0, Vector3D::dot(Vector3D::normalise((2 * dot * normal) - l),
                                                 Vector3D::normalise(Vector3D::point(0, 0, 0) - p))),
                     reflectionCoeff);

    pixel_color->red += light->specularLight.red * specular.red * specular_intensity;
    pixel_color->green += light->specularLight.green * specular.green * specular_intensity;
    pixel_color->blue += light->specularLight.blue * specular.blue * specular_intensity;


    if (pixel_color->red > 1.0)
    {
        pixel_color->red = 1.0;
    }
    if (pixel_color->green > 1.0)
    {
        pixel_color->green = 1.0;
    }
    if (pixel_color->blue > 1.0)
    {
        pixel_color->blue = 1.0;
    }
}
