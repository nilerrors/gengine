//
// Created by nilerrors on 5/7/24.
//

#ifndef ENGINE_FIGURE_H
#define ENGINE_FIGURE_H

#include <list>
#include <limits>
#include <vector>
#include "utils.h"
#include "color.h"
#include "../lib/easy_image.h"
#include "../lib/vector3d.h"

namespace gengine
{

class Face
{
public:
    Face() = default;

    explicit Face(const std::vector<int> &point_indexes) : point_indexes(point_indexes)
    {
    }

    Face(int p1, int p2)
    {
        point_indexes.push_back(p1);
        point_indexes.push_back(p2);
    }


    void triangulate(std::vector<Face> *faces) const
    {
        if (point_indexes.size() < 3)
        {
            faces->push_back(*this);
        }

        for (uint i = 1; i <= point_indexes.size() - 2; i++)
        {
            faces->push_back(Face({point_indexes[0], point_indexes[i], point_indexes[i + 1]}));
        }
    }

    std::vector<int> point_indexes;
};

class Figure
{
public:
    Figure()
    {
        ambientReflection = Color(0, 0, 0);
        diffuseReflection = Color(0, 0, 0);
        specularReflection = Color(0, 0, 0);
        reflectionCoefficient = 0;
    }

    void triangulate()
    {
        std::vector<Face> temp_faces;

        while (!faces.empty())
        {
            faces.back().triangulate(&temp_faces);
            faces.pop_back();
        }
        faces = temp_faces;
    }

    std::vector<Vector3D> points;
    std::vector<Face> faces;
    Color ambientReflection;
    Color diffuseReflection;
    Color specularReflection;
    double reflectionCoefficient;
};

using Figures3D = std::vector<Figure>;

}

#endif //ENGINE_FIGURE_H
