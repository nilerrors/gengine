//
// Created by nilerrors on 5/7/24.
//

#ifndef ENGINE_LIGHT_H
#define ENGINE_LIGHT_H

#include <vector>
#include "color.h"
#include "utils.h"

namespace gengine
{

class Light
{
public:
    enum class Type
    {
        Inf,
        Point
    };

    Light() = default;

    ~Light() = default;

    void eyePointMultiply(const Matrix &eyePoint)
    {
        switch (type)
        {
        case Type::Inf:
            ldVector *= eyePoint;
            break;
        case Type::Point:
            location *= eyePoint;
            break;
        }
    }

    Color ambientLight = Color(1, 1, 1);
    Color diffuseLight = Color(0, 0, 0);
    Color specularLight = Color(0, 0, 0);

    Type type = Type::Inf;
    Vector3D ldVector = Vector3D::vector(1, 1, 1);
    Vector3D location;
    double spotAngle = 0;
};

using Lights3D = std::vector<Light *>;

}

#endif //ENGINE_LIGHT_H
