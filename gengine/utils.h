//
// Created by nilerrors on 2/21/24.
//

#ifndef ENGINE_UTILS_H
#define ENGINE_UTILS_H

#include <list>
#include <limits>
#include <vector>
#include "../lib/easy_image.h"
#include "../lib/vector3d.h"

namespace gengine
{

#define PI M_PI

constexpr double INF = std::numeric_limits<double>::infinity();

double degToRad(double deg);

}

#endif //ENGINE_UTILS_H
