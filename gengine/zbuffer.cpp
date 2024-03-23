//
// Created by nilerrors on 3/21/24.
//

#include <cmath>
#include <functional>
#include "zbuffer.h"
#include "utils.h"

namespace gengine
{


void ZBuffer::draw_zbuf_line(
		img::EasyImage &image,
		long x0, long int y0, double z0,
		long x1, long int y1, double z1,
		const img::Color &color)
{
	// inspired by img::EasyImage::draw_line of ../lib/easy_image.cc

	assert(x0 < image.get_width() && y0 < image.get_height());
	assert(x1 < image.get_width() && y1 < image.get_height());

	const long y_min = std::min(y0, y1);
	const long y_max = std::max(y0, y1);
	const long x_min = std::min(x0, x1);
	const long x_max = std::max(x0, x1);

	if (x0 == x1)
	{
		//special case for x0 == x1
		long a = y_max - y_min;
		long i = a;
		for (uint j = y_min; j <= y_max; j++)
		{
			double z = (((double)i / (double)a) / z0) + ((1 - ((double)i / (double)a)) / z1);

			if (z < ati(x0,j))
			{
				seti(x0,j,z);
				image(x0, j) = color;
			}
			i--;
		}
	}
	else if (y0 == y1)
	{
		//special case for y0 == y1
		long a = x_max - x_min;
		long i = a;
		for (uint j = x_min; j <= x_max; j++)
		{
			double z = (((double)i / (double)a) / z0) + ((1.0 - ((double)i / (double)a)) / z1);

			if (z < ati(j,y0))
			{
				seti(j,y0,z);
				image(j, y0) = color;
			}
			i--;
		}
	}
	else
	{
		if (x0 > x1)
		{
			//flip points if x1>x0: we want x0 to have the lowest value
			std::swap(x0, x1);
			std::swap(y0, y1);
			std::swap(z0, z1);
		}

		double m = ((double)y1 - (double)y0) / ((double)x1 - (double)x0);
		if (-1.0 <= m && m <= 1.0)
		{
			long a = (x1 - x0);
			long i = a;
			for (uint j = 0; j <= (x1 - x0); j++)
			{
				double z = (((double)i / (double)a) / z0) + ((1 - ((double)i / (double)a)) / z1);

				if (z < ati(x0 + j,(uint)std::round(y0 + m * j)))
				{
					seti(x0 + j,(uint)std::round(y0 + m * j),z);
					image(x0 + j, (uint)std::round(y0 + m * j)) = color;
				}
				i--;
			}
		}
		else if (m > 1.0)
		{
			long a = (y1 - y0);
			long i = a;
			for (uint j = 0; j <= (y1 - y0); j++)
			{
				double z = (((double)i / (double)a) / z0) + ((1 - ((double)i / (double)a)) / z1);

				if (z < ati((uint)std::round(x0 + (j / m)), y0 + j))
				{
					seti((uint)std::round(x0 + (j / m)), y0 + j, z);
					image((uint)std::round(x0 + (j / m)), y0 + j) = color;
				}
				i--;
			}
		}
		else if (m < -1.0)
		{
			long a = (y0 - y1);
			long i = a;
			for (uint j = 0; j <= (y0 - y1); j++)
			{
				double z = (((double)i / (double)a) / z0) + ((1 - ((double)i / (double)a)) / z1);

				if (z < ati((uint)std::round(x0 - (j / m)),y0 - j))
				{
					seti((uint)std::round(x0 - (j / m)),y0 - j, z);
					image((uint)std::round(x0 - (j / m)), y0 - j) = color;
				}
				i--;
			}
		}
	}
}

} // gengine