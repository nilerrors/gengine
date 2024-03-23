//
// Created by nilerrors on 3/9/24.
//

#ifndef ENGINE_LINE3D_H
#define ENGINE_LINE3D_H

#include "../lib/ini_configuration.h"
#include "../lib/vector3d.h"
#include "utils.h"

namespace gengine
{

class Wireframe
{
public:
	Wireframe();
	explicit Wireframe(const ini::Configuration &config);
	~Wireframe();

	void allFiguresFromConfig(const ini::Configuration &configuration);
	void addFigureFromConfig(const ini::Section &section);

	virtual const Lines2D &doProjection();
	static Point2D doProjection(const Vector3D &point, double d);

	virtual const img::EasyImage &drawWireframe();

protected:
	img::EasyImage image;
	std::vector<Figure *> figures;
	int size;
	Color backgroundColor;
	Vector3D eyePoint;
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

#endif //ENGINE_LINE3D_H
