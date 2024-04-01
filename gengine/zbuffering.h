//
// Created by nilerrors on 3/27/24.
//

#ifndef ZBUFFERING_H
#define ZBUFFERING_H

#include "../lib/ini_configuration.h"
#include "../lib/vector3d.h"
#include "line3d.h"
#include "utils.h"
#include "zbufwireframe.h"
#include "zbuffer.h"

namespace gengine
{

class ZBuffering : public ZBufferedWireframe
{
public:
	ZBuffering();

	explicit ZBuffering(const ini::Configuration &config);

	~ZBuffering();

	const Lines2D &doProjection() override;

	const img::EasyImage &drawWireframe() override;
};


std::vector<Face> triangulate(const Face&face);

void calculate_x_range(long y, Point2D a, Point2D b, Point2D c, long *x_min, long *x_max);

} // gengine

#endif //ZBUFFERING_H
