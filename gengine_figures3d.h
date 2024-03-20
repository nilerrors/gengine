//
// Created by nilerrors on 3/13/24.
//

#ifndef ENGINE_GENGINE_FIGURES3D_H
#define ENGINE_GENGINE_FIGURES3D_H

#include "ini_configuration.h"
#include "gengine_utils.h"
#include "gengine_figures3d.h"

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

#endif //ENGINE_GENGINE_FIGURES3D_H
