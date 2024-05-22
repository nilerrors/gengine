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
    if (config["diffuseLight"].as_double_tuple_if_exists(diffuse))
    {
        light->diffuseLight = Color(diffuse);
        if (config["infinity"].as_bool_or_die())
        {
            light->ldVector = Vector3D::vector(config["direction"].as_double_tuple_or_die()[0],
                                               config["direction"].as_double_tuple_or_die()[1],
                                               config["direction"].as_double_tuple_or_die()[2]);
        }
        else
        {
            light->type = Light::Type::Point;
            light->location = Vector3D::vector(config["location"].as_double_tuple_or_die()[0],
                                               config["location"].as_double_tuple_or_die()[1],
                                               config["location"].as_double_tuple_or_die()[2]);
            light->spotAngle = degToRad(config["spotAngle"].as_double_or_die());
        }
        light->eyePointMultiply(eyePoint);
    }

    std::vector<double> specular;
    if (config["specularLight"].as_double_tuple_if_exists(diffuse))
    {
        light->specularLight = Color(diffuse);
        if (config["infinity"].as_bool_or_die())
        {
            light->ldVector = Vector3D::vector(config["direction"].as_double_tuple_or_die()[0],
                                               config["direction"].as_double_tuple_or_die()[1],
                                               config["direction"].as_double_tuple_or_die()[2]);
        }
        else
        {
            light->type = Light::Type::Point;
            light->location = Vector3D::vector(config["location"].as_double_tuple_or_die()[0],
                                               config["location"].as_double_tuple_or_die()[1],
                                               config["location"].as_double_tuple_or_die()[2]);
            light->spotAngle = degToRad(config["spotAngle"].as_double_or_die());
        }
        light->eyePointMultiply(eyePoint);
    }

    lights.push_back(light);
}

void LightedZBuffering::applyLight(
        Light *light,
        const Color &diffuse, const Color &specular,
        double reflectionCoeff, Color *pixel_color, Vector3D normal,
        long y, long x, double dx, double dy, double d, double inverted_z)
{
    double px = (x - dx) * (-inverted_z) / d;
    double py = (y - dy) * (-inverted_z) / d;
    double pz = inverted_z;
    Vector3D p = Vector3D::point(px, py, pz);

    Vector3D l;
    double dot = 0;
    if (light->type == Light::Type::Inf)
    {
        l = light->ldVector * (-1);
        l.normalise();

        dot = Vector3D::dot(normal, l);
        if (dot < 0)
        {
            dot = 0;
        }
    }
    else
    {
        l = light->location - p;
        l.normalise();

        dot = Vector3D::dot(normal, l);
        double angleSpot = std::cos(light->spotAngle);
        double intensity = 1.0 - ((1.0 - dot) / (1.0 - angleSpot));
        if (dot <= angleSpot)
        {
            intensity = 0;
        }
        pixel_color->red += light->diffuseLight.red * diffuse.red * intensity;
        pixel_color->green += light->diffuseLight.green * diffuse.green * intensity;
        pixel_color->blue += light->diffuseLight.blue * diffuse.blue * intensity;
    }

    Vector3D r = (2 * dot * normal) - l;
    r.normalise();

    Vector3D camera = Vector3D::point(0, 0, 0) - p;
    camera.normalise();

    double specular_dot = Vector3D::dot(r, camera);
    if (specular_dot < 0)
    {
        specular_dot = 0;
    }
    double specular_intensity = std::pow(specular_dot, reflectionCoeff);

    pixel_color->red += light->specularLight.red * specular.red * specular_intensity;
    pixel_color->green += light->specularLight.green * specular.green * specular_intensity;
    pixel_color->blue += light->specularLight.blue * specular.blue * specular_intensity;
}
