//
// Created by nilerrors on 3/21/24.
//

#ifndef ENGINE_ZBUFFER_H
#define ENGINE_ZBUFFER_H

#include <vector>
#include <cassert>
#include <limits>
#include "utils.h"

namespace gengine
{

const double INF = std::numeric_limits<double>::infinity();

class ZBuffer: public std::vector<std::vector<double>>
{
public:
	ZBuffer() : std::vector<std::vector<double>>() {}

	ZBuffer(uint width, uint height)
		: std::vector<std::vector<double>>(height, std::vector<double>(width, INF)),
		  width(width), height(height) {}

	const double &ati(uint x, uint y) const
	{
		assert(y < height && x < width);
		return at(y).at(x);
	}

	double seti(uint x, uint y, double z)
	{
		assert(y < height && x < width);
		(*this)[y][x] = z;
		return at(y).at(x);
	}

	void draw_zbuf_line(img::EasyImage &image,
						long x0, long y0, double z0,
						long x1, long y1, double z1,
						const img::Color &color);

private:
	uint width = 0;
	uint height = 0;
};



} // gengine

#endif //ENGINE_ZBUFFER_H
