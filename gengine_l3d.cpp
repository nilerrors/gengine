//
// Created by nilerrors on 3/9/24.
//

#include "gengine_l3d.h"
#include "ini_configuration.h"
#include "vector3d.h"
#include "gengine_line.h"

#include <iostream>
#include <cmath>
#include "gengine_figures3d.h"

namespace gengine
{

Wireframe::Wireframe() : eyePoint(Vector3D::point(0, 0, 0)) {}

Wireframe::Wireframe(const ini::Configuration &config)
{
	allFiguresFromConfig(config);
}

Wireframe::~Wireframe()
{
	for (auto &figure : figures)
	{
		delete figure;
		figure = nullptr;
	}
}

void Wireframe::addFigureFromConfig(const ini::Section &section)
{
	const std::string type = section["type"].as_string_or_die();
	const double rotX = degToRad(section["rotateX"].as_double_or_die());
	const double rotY = degToRad(section["rotateY"].as_double_or_die());
	const double rotZ = degToRad(section["rotateZ"].as_double_or_die());
	const double scale = section["scale"].as_double_or_die();
	const Vector3D center = Vector3D::point(
			section["center"].as_double_tuple_or_die()[0],
			section["center"].as_double_tuple_or_die()[1],
			section["center"].as_double_tuple_or_die()[2]);
	const Color color = Color(section["color"].as_double_tuple_or_die());
	auto *figure = new Figure();

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
			figure->faces.push_back(Face(
					section[line].as_int_tuple_or_die()[0],
					section[line].as_int_tuple_or_die()[1]));
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
	else
	{
		std::cerr << "Unknown figure type: " << type << std::endl;
		exit(1);
	}

	figure->color = color;
	Matrix transformation =
			scaleFigure(scale)
			* rotateX(rotX) * rotateY(rotY) * rotateZ(rotZ)
			* translate(center)
			* eyePointTrans(eyePoint);

	applyTransformation(figure, transformation);

	figures.push_back(figure);
}

Wireframe *Wireframe::allFiguresFromConfig(const ini::Configuration &configuration)
{
	const std::vector<double> eyePointConfig = configuration["General"]["eye"].as_double_tuple_or_die();
	eyePoint = Vector3D::point(eyePointConfig[0], eyePointConfig[1], eyePointConfig[2]);

	for (int i = 0; i < configuration["General"]["nrFigures"].as_int_or_die(); i++)
	{
		const ini::Section current_figure_section = configuration["Figure" + std::to_string(i)];

		addFigureFromConfig(current_figure_section);
	}

	return this;
}

const Lines2D &Wireframe::doProjection()
{
	for (auto &figure : figures)
	{
		// for now, we assume that there are exactly 2 points in a face
		double d = 1;
		for (auto &face : figure->faces)
		{
			if (face.point_indexes.size() < 2)
				throw std::runtime_error("Face has less than 2 points");
			lines.push_back(Line2D(
					gengine::doProjection(figure->points[face.point_indexes[0]], d),
					gengine::doProjection(figure->points[face.point_indexes[face.point_indexes.size() - 1]], d),
					figure->color));
			for (size_t i = 0; i < face.point_indexes.size() - 1; i++)
			{
				lines.push_back(Line2D(
						gengine::doProjection(figure->points[face.point_indexes[i]], d),
						gengine::doProjection(figure->points[face.point_indexes[i + 1]], d),
						figure->color));
			}
		}
	}

	return lines;
}


Matrix scaleFigure(const double scale)
{
	Matrix scaleMatrix;
	scaleMatrix(1, 1) = scale;
	scaleMatrix(2, 2) = scale;
	scaleMatrix(3, 3) = scale;
	scaleMatrix(4, 4) = 1;
	return scaleMatrix;
}

Matrix rotateX(const double angle)
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

Matrix rotateY(const double angle)
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

Matrix rotateZ(const double angle)
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

Matrix translate(const Vector3D &vector)
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

void toPolar(const Vector3D &point, double &theta, double &phi, double &r)
{
	r = std::sqrt(std::pow(point.x, 2) + std::pow(point.y, 2) + std::pow(point.z, 2));
	theta = std::atan2(point.y, point.x);
	phi = std::acos(point.z / r);
}

Matrix eyePointTrans(const Vector3D &eyepoint)
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

Point2D doProjection(const Vector3D &point, const double d)
{
	Point2D new_point;

	new_point.x = d * point.x / -point.z;
	new_point.y = d * point.y / -point.z;

	return new_point;
}

void applyTransformation(Figure *fig, const Matrix &m)
{
	for (auto &point : fig->points)
	{
		point *= m;
	}
}

} // gengine