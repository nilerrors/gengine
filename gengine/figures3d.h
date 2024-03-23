//
// Created by nilerrors on 3/13/24.
//

#ifndef ENGINE_FIGURES3D_H
#define ENGINE_FIGURES3D_H

#include "../lib/ini_configuration.h"
#include "utils.h"
#include "figures3d.h"

namespace gengine
{

namespace Platonic
{

Figure createCube();

Figure createTetrahedron();

Figure createOctahedron();

Figure createIcosahedron();

Figure createDodecahedron();

Figure createCylinder(int n, double height);

Figure createCone(int n, double height);

Figure createSphere(int n);

Figure createTorus(double r, double R, int n, int m);

}

Figure create3DLSystem(const ini::Section &LSystem3DConfig);

}

#endif //ENGINE_FIGURES3D_H
