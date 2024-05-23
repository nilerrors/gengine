//
// Created by nilerrors on 3/9/24.
//

#include "wireframes.h"
#include "l_systems2d.h"

#include <iostream>
#include <cmath>
#include "figures3d.h"

using namespace gengine;

Wireframe::Wireframe() : size(0), backgroundColor(Color::WHITE), eyePoint(eyePointTrans(Vector3D::point(0, 0, 0)))
{
}

Wireframe::Wireframe(const ini::Configuration &config, bool lighted)
{
    size = config["General"]["size"].as_int_or_die();
    backgroundColor = Color(config["General"]["backgroundcolor"].as_double_tuple_or_die());
    const std::vector<double> eyePointConfig = config["General"]["eye"].as_double_tuple_or_die();
    eyePoint = eyePointTrans(Vector3D::point(eyePointConfig[0], eyePointConfig[1], eyePointConfig[2]));
    this->lighted = lighted;

    allFiguresFromConfig(config);
}

Wireframe::~Wireframe()
{
    for (auto &figure: figures)
    {
        delete figure;
        figure = nullptr;
    }
}

void Wireframe::addFigureFromConfig(const ini::Section &section)
{
    std::string type = section["type"].as_string_or_die();
    const double rotX = degToRad(section["rotateX"].as_double_or_die());
    const double rotY = degToRad(section["rotateY"].as_double_or_die());
    const double rotZ = degToRad(section["rotateZ"].as_double_or_die());
    const double scale = section["scale"].as_double_or_die();
    const Vector3D center = Vector3D::point(
            section["center"].as_double_tuple_or_die()[0],
            section["center"].as_double_tuple_or_die()[1],
            section["center"].as_double_tuple_or_die()[2]);
    Color color;
    if (lighted)
    {
        color = Color(section["ambientReflection"].as_double_tuple_or_die());
    }
    else
    {
        color = Color(section["color"].as_double_tuple_or_die());
    }
    auto *figure = new Figure();
    bool fractal = false;

    if (type.rfind("Fractal", 0) == 0)
    {
        fractal = true;
        type = type.substr(std::string("Fractal").size());
    }

    if (type == "LineDrawing")
    {
        const int nrPoints = section["nrPoints"].as_int_or_die();
        const int nrLines = section["nrLines"].as_int_or_die();

        for (int i = 0; i < nrPoints; i++)
        {
            const std::string point = "point" + std::to_string(i);
            figure->points.push_back(Vector3D::point(
                    section[point].as_double_tuple_or_die()[0],
                    section[point].as_double_tuple_or_die()[1],
                    section[point].as_double_tuple_or_die()[2]));
        }

        for (int i = 0; i < nrLines; i++)
        {
            const std::string line = "line" + std::to_string(i);
            figure->faces.emplace_back(
                    section[line].as_int_tuple_or_die()[0],
                    section[line].as_int_tuple_or_die()[1]);
        }
    }
    else if (type == "3DLSystem")
    {
        *figure = create3DLSystem(section);
    }
    else if (type == "Cube")
    {
        *figure = Platonic::createCube();
    }
    else if (type == "Tetrahedron")
    {
        *figure = Platonic::createTetrahedron();
    }
    else if (type == "Octahedron")
    {
        *figure = Platonic::createOctahedron();
    }
    else if (type == "Icosahedron")
    {
        *figure = Platonic::createIcosahedron();
    }
    else if (type == "Dodecahedron")
    {
        *figure = Platonic::createDodecahedron();
    }
    else if (type == "Cylinder")
    {
        *figure = Platonic::createCylinder(section["n"].as_int_or_die(), section["height"].as_double_or_die());
    }
    else if (type == "Cone")
    {
        *figure = Platonic::createCone(section["n"].as_int_or_die(), section["height"].as_double_or_die());
    }
    else if (type == "Sphere")
    {
        *figure = Platonic::createSphere(section["n"].as_int_or_die());
    }
    else if (type == "Torus")
    {
        *figure = Platonic::createTorus(
                section["r"].as_double_or_die(),
                section["R"].as_double_or_die(),
                section["n"].as_int_or_die(),
                section["m"].as_int_or_die());
    }
    else if (type == "BuckyBall")
    {
        static bool buckyBallWarned = false;
        if (!buckyBallWarned)
        {
            std::cerr << "BuckyBall is not implemented, an icosahedron is generated instead." << std::endl;
            buckyBallWarned = true;
        }
        *figure = createBuckyBall();
    }
    else if (type == "MengerSponge")
    {
        static bool mengerSpongeWarned = false;
        if (!mengerSpongeWarned)
        {
            std::cerr << "MengerSponge is not implemented, a cube is generated instead." << std::endl;
            mengerSpongeWarned = true;
        }
        *figure = createMengerSponge();
    }
    else
    {
        std::cerr << "Unknown figure type: " << type << std::endl;
        exit(1);
    }

    if (fractal && section["nrIterations"].as_int_or_die())
    {
        int iterations = section["nrIterations"].as_int_or_die();
        double scale_fractal = section["fractalScale"].as_double_or_die();
        std::vector<Figure *> fractal_figures = {};

        generateFractal(figure, fractal_figures, iterations, scale_fractal);

        for (Figure *fig: fractal_figures)
        {
            fig->ambientReflection = color;
            if (lighted)
            {
                fig->diffuseReflection = Color(section["diffuseReflection"].as_double_tuple_or_default({0, 0, 0}));
                fig->specularReflection = Color(section["specularReflection"].as_double_tuple_or_default({0, 0, 0}));
            }
            Matrix transformation =
                    scaleFigure(scale)
                    * rotateX(rotX) * rotateY(rotY) * rotateZ(rotZ)
                    * translate(center)
                    * eyePoint;

            applyTransformation(fig, transformation);
        }
        figures.insert(figures.end(), fractal_figures.begin(), fractal_figures.end());
        return;
    }

    figure->ambientReflection = color;
    if (lighted)
    {
        figure->diffuseReflection = Color(section["diffuseReflection"].as_double_tuple_or_default({0, 0, 0}));
        figure->specularReflection = Color(section["specularReflection"].as_double_tuple_or_default({0, 0, 0}));
    }

    Matrix transformation =
            scaleFigure(scale)
            * rotateX(rotX) * rotateY(rotY) * rotateZ(rotZ)
            * translate(center)
            * eyePoint;

    applyTransformation(figure, transformation);

    figures.push_back(figure);
}

void Wireframe::allFiguresFromConfig(const ini::Configuration &configuration)
{
    for (int i = 0; i < configuration["General"]["nrFigures"].as_int_or_die(); i++)
    {
        const ini::Section current_figure_section = configuration["Figure" + std::to_string(i)];

        addFigureFromConfig(current_figure_section);
    }
}

const Lines2D &Wireframe::doProjection()
{
    for (auto &figure: figures)
    {
        double d = 1;
        for (auto &face: figure->faces)
        {
            if (face.point_indexes.size() < 2)
            {
                throw std::runtime_error("Face has less than 2 points");
            }
            for (size_t i = 0; i < face.point_indexes.size(); i++)
            {
                size_t next = i + 1;
                if (next >= face.point_indexes.size())
                {
                    next = 0;
                }

                Line2D line = Line2D(
                        doProjection(figure->points[face.point_indexes[i]], d),
                        doProjection(figure->points[face.point_indexes[next]], d),
                        figure->ambientReflection);

                lines.push_back(line);
            }
        }
    }

    return lines;
}

Point2D Wireframe::doProjection(const Vector3D &point, const double d)
{
    Point2D new_point;

    new_point.x = d * point.x / -point.z;
    new_point.y = d * point.y / -point.z;

    return new_point;
}

const img::EasyImage &Wireframe::drawWireframe()
{
    doProjection();
    image = Draw2DLSystem::draw2DLines(lines, size, backgroundColor);
    return image;
}

void Wireframe::generateFractal(
        Figure *fig, std::vector<Figure *> &fractal, const int nr_iterations, const double scale)
{
    if (nr_iterations == 0)
    {
        fractal.push_back(fig);
        return;
    }

    Matrix scale_fractal = scaleFigure(1 / scale);
    for (uint i = 0; i < fig->points.size(); i++)
    {
        auto *copied_frac = new Figure();
        *copied_frac = *fig;
        applyTransformation(copied_frac, scale_fractal);

        Matrix translation = translate(fig->points[i] - copied_frac->points[i]);
        applyTransformation(copied_frac, translation);

        std::vector<Figure *> fractal_fracals = {};
        generateFractal(copied_frac, fractal_fracals, nr_iterations - 1, scale);
        fractal.insert(fractal.end(), fractal_fracals.begin(), fractal_fracals.end());
    }
}

void Wireframe::setLighted(bool light)
{
    lighted = light;
}

bool Wireframe::isLighted() const
{
    return lighted;
}

Matrix gengine::scaleFigure(const double scale)
{
    Matrix scaleMatrix;
    scaleMatrix(1, 1) = scale;
    scaleMatrix(2, 2) = scale;
    scaleMatrix(3, 3) = scale;
    scaleMatrix(4, 4) = 1;
    return scaleMatrix;
}

Matrix gengine::rotateX(const double angle)
{
    Matrix rotateMatrix;
    rotateMatrix(1, 1) = 1;
    rotateMatrix(2, 2) = std::cos(angle);
    rotateMatrix(2, 3) = std::sin(angle);
    rotateMatrix(3, 2) = -std::sin(angle);
    rotateMatrix(3, 3) = std::cos(angle);
    rotateMatrix(4, 4) = 1;
    return rotateMatrix;
}

Matrix gengine::rotateY(const double angle)
{
    Matrix rotateMatrix;
    rotateMatrix(1, 1) = std::cos(angle);
    rotateMatrix(1, 3) = -std::sin(angle);
    rotateMatrix(2, 2) = 1;
    rotateMatrix(3, 1) = std::sin(angle);
    rotateMatrix(3, 3) = std::cos(angle);
    rotateMatrix(4, 4) = 1;
    return rotateMatrix;
}

Matrix gengine::rotateZ(const double angle)
{
    Matrix rotateMatrix;
    rotateMatrix(1, 1) = std::cos(angle);
    rotateMatrix(1, 2) = std::sin(angle);
    rotateMatrix(2, 1) = -std::sin(angle);
    rotateMatrix(2, 2) = std::cos(angle);
    rotateMatrix(3, 3) = 1;
    rotateMatrix(4, 4) = 1;
    return rotateMatrix;
}

Matrix gengine::translate(const Vector3D &vector)
{
    Matrix translateMatrix;
    translateMatrix(1, 1) = 1;
    translateMatrix(2, 2) = 1;
    translateMatrix(3, 3) = 1;
    translateMatrix(4, 4) = 1;
    translateMatrix(4, 1) = vector.x;
    translateMatrix(4, 2) = vector.y;
    translateMatrix(4, 3) = vector.z;
    return translateMatrix;
}

void gengine::toPolar(const Vector3D &point, double &theta, double &phi, double &r)
{
    r = std::sqrt(std::pow(point.x, 2) + std::pow(point.y, 2) + std::pow(point.z, 2));
    theta = std::atan2(point.y, point.x);
    phi = std::acos(point.z / r);
}

Matrix gengine::eyePointTrans(const Vector3D &eyepoint)
{
    Matrix m;
    double r;
    double theta;
    double phi;

    toPolar(eyepoint, theta, phi, r);

    m(1, 1) = -std::sin(theta);
    m(1, 2) = -std::cos(theta) * std::cos(phi);
    m(1, 3) = std::cos(theta) * std::sin(phi);
    m(2, 1) = std::cos(theta);
    m(2, 2) = -std::sin(theta) * std::cos(phi);
    m(2, 3) = std::sin(theta) * std::sin(phi);
    m(3, 2) = std::sin(phi);
    m(3, 3) = std::cos(phi);
    m(4, 3) = -r;
    m(4, 4) = 1;

    return m;
}

void gengine::applyTransformation(Figure *fig, const Matrix &m)
{
    for (auto &point: fig->points)
    {
        point *= m;
    }
}
