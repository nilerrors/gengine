//
// Created by nilerrors on 3/9/24.
//

#ifndef ENGINE_GENGINE_L3D_H
#define ENGINE_GENGINE_L3D_H

#include "ini_configuration.h"
#include "vector3d.h"
#include "gengine_utils.h"

namespace gengine
{

class Wireframe
{
public:
	Wireframe();
	Wireframe(const ini::Configuration &config);
	~Wireframe();

	Wireframe *allFiguresFromConfig(const ini::Configuration &configuration);
	void addFigureFromConfig(const ini::Section &section);

	const Lines2D &doProjection();

private:
	std::vector<Figure *> figures;
	Vector3D eyePoint;
	Lines2D lines;
};


// 3D transformations and projections

Matrix scaleFigure(const double scale);

Matrix rotateX(const double angle);

Matrix rotateY(const double angle);

Matrix rotateZ(const double angle);

Matrix translate(const Vector3D &vector);

void toPolar(const Vector3D &point, double &theta, double &phi, double &r);

Matrix eyePointTrans(const Vector3D &eyepoint);

void applyTransformation(Figure *fig, const Matrix &m);

Point2D doProjection(const Vector3D &point, const double d);

} // gengine

#endif //ENGINE_GENGINE_L3D_H
