//
// Created by nilerrors on 3/9/24.
//

#ifndef ENGINE_WIREFRAMES_H
#define ENGINE_WIREFRAMES_H

#include "../lib/ini_configuration.h"
#include "../lib/vector3d.h"
#include "utils.h"
#include "figure.h"
#include "color.h"
#include "line2d.h"

namespace gengine
{

class Wireframe
{
public:
    Wireframe();

    explicit Wireframe(const ini::Configuration &config, bool lighted = false);

    ~Wireframe();

    void allFiguresFromConfig(const ini::Configuration &configuration);

    void addFigureFromConfig(const ini::Section &section);

    virtual const Lines2D &doProjection();

    static Point2D doProjection(const Vector3D &point, double d);

    static void
    generateFractal(Figure *fig, std::vector<Figure *> &fractal, int nr_iterations, double scale);

    virtual const img::EasyImage &drawWireframe();

    bool isLighted() const;

    void setLighted(bool lighted);

protected:
    img::EasyImage image;
    std::vector<Figure *> figures;
    int size;
    bool lighted = false;
    Color backgroundColor;
    Matrix eyePoint;
    Lines2D lines;
};


// 3D transformations and projections

Matrix scaleFigure(double scale);

Matrix rotateX(double angle);

Matrix rotateY(double angle);

Matrix rotateZ(double angle);

Matrix translate(const Vector3D &vector);

void toPolar(const Vector3D &point, double &theta, double &phi, double &r);

Matrix eyePointTrans(const Vector3D &eyepoint);

void applyTransformation(Figure *fig, const Matrix &m);

} // gengine

#endif //ENGINE_WIREFRAMES_H
