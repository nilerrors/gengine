//
// Created by nilerrors on 3/13/24.
//

#include <cmath>
#include <sstream>
#include <fstream>
#include <stack>
#include "../lib/l_parser.h"
#include "../lib/ini_configuration.h"
#include "figures3d.h"

using namespace gengine;

Figure gengine::Platonic::createCube()
{
	Figure cube;

	//  1 −1 1 −1  1 −1  1 −1
	// −1  1 1 −1  1 −1 −1  1
	// −1 −1 1  1 −1 −1  1  1
	Vector3D point1 = Vector3D::point(1, -1, -1);
	Vector3D point2 = Vector3D::point(-1, 1, -1);
	Vector3D point3 = Vector3D::point(1, 1, 1);
	Vector3D point4 = Vector3D::point(-1, -1, 1);
	Vector3D point5 = Vector3D::point(1, 1, -1);
	Vector3D point6 = Vector3D::point(-1, -1, -1);
	Vector3D point7 = Vector3D::point(1, -1, 1);
	Vector3D point8 = Vector3D::point(-1, 1, 1);

	cube.points.push_back(point1);
	cube.points.push_back(point2);
	cube.points.push_back(point3);
	cube.points.push_back(point4);
	cube.points.push_back(point5);
	cube.points.push_back(point6);
	cube.points.push_back(point7);
	cube.points.push_back(point8);

	// 1 5 2 6 7 1
	// 5 2 6 1 3 6
	// 3 8 4 7 8 2
	// 7 3 8 4 4 5
	// the indices in the course are 1-based, so we need to subtract 1
	Face rectangle1({0, 4, 2, 6});
	Face rectangle2({4, 1, 7, 2});
	Face rectangle3({1, 5, 3, 7});
	Face rectangle4({5, 0, 6, 3});
	Face rectangle5({6, 2, 7, 3});
	Face rectangle6({0, 5, 1, 4});

	cube.faces.push_back(rectangle1);
	cube.faces.push_back(rectangle2);
	cube.faces.push_back(rectangle3);
	cube.faces.push_back(rectangle4);
	cube.faces.push_back(rectangle5);
	cube.faces.push_back(rectangle6);

	return cube;
}

Figure gengine::Platonic::createTetrahedron()
{
	Figure tetrahedron;

	//  1 −1 1 −1
	// −1  1 1 −1
	// −1 −1 1  1
	Vector3D point1 = Vector3D::point(1, -1, -1);
	Vector3D point2 = Vector3D::point(-1, 1, -1);
	Vector3D point3 = Vector3D::point(1, 1, 1);
	Vector3D point4 = Vector3D::point(-1, -1, 1);

	tetrahedron.points.push_back(point1);
	tetrahedron.points.push_back(point2);
	tetrahedron.points.push_back(point3);
	tetrahedron.points.push_back(point4);

	// 1 2 1 1
	// 2 4 4 3
	// 3 3 2 4
	// the indices in the course are 1-based, so we need to subtract 1
	Face triangle1({0, 1, 2});
	Face triangle2({1, 3, 3});
	Face triangle3({0, 3, 1});
	Face triangle4({0, 2, 3});

	tetrahedron.faces.push_back(triangle1);
	tetrahedron.faces.push_back(triangle2);
	tetrahedron.faces.push_back(triangle3);
	tetrahedron.faces.push_back(triangle4);

	return tetrahedron;
}

Figure gengine::Platonic::createOctahedron()
{
	Figure octahedron;

	// 1 0 −1  0  0 0
	// 0 1  0 −1  0 0
	// 0 0  0  0 −1 1
	Vector3D point1 = Vector3D::point(1, 0, 0);
	Vector3D point2 = Vector3D::point(0, 1, 0);
	Vector3D point3 = Vector3D::point(-1, 0, 0);
	Vector3D point4 = Vector3D::point(0, -1, 0);
	Vector3D point5 = Vector3D::point(0, 0, -1);
	Vector3D point6 = Vector3D::point(0, 0, 1);

	octahedron.points.push_back(point1);
	octahedron.points.push_back(point2);
	octahedron.points.push_back(point3);
	octahedron.points.push_back(point4);
	octahedron.points.push_back(point5);
	octahedron.points.push_back(point6);

	// 1 2 3 4 2 3 4 1
	// 2 3 4 1 1 2 3 4
	// 6 6 6 6 5 5 5 5
	// the indices in the course are 1-based, so we need to subtract 1
	Face triangle1({0, 1, 5});
	Face triangle2({1, 2, 5});
	Face triangle3({2, 3, 5});
	Face triangle4({3, 0, 5});
	Face triangle5({1, 0, 4});
	Face triangle6({2, 1, 4});
	Face triangle7({3, 2, 4});
	Face triangle8({0, 3, 4});

	octahedron.faces.push_back(triangle1);
	octahedron.faces.push_back(triangle2);
	octahedron.faces.push_back(triangle3);
	octahedron.faces.push_back(triangle4);
	octahedron.faces.push_back(triangle5);
	octahedron.faces.push_back(triangle6);
	octahedron.faces.push_back(triangle7);
	octahedron.faces.push_back(triangle8);

	return octahedron;
}

Figure gengine::Platonic::createIcosahedron()
{
	Figure icosahedron;

	// punt i                   xi                  yi                  zi
	// 1                        0                   0                   √5/2
	// 2,3,4,5,6        cos((i − 2)2π/5)     sin((i − 2)2π/5)           0.5
	// 7,8,9,10,11   cos(π/5 + (i − 7)2π/5) sin(π/5 + (i − 7)2π/5)     −0.5
	// 12                       0                   0                  −√5/2
	Vector3D point1 = Vector3D::point(0, 0, std::sqrt(5) / 2);
	icosahedron.points.push_back(point1);
	for (int i = 2; i <= 6; i++)
	{
		Vector3D pointi = Vector3D::point(
								std::cos((i - 2) * 2 * PI / 5),
								std::sin((i - 2) * 2 * PI / 5),
								0.5);
		icosahedron.points.push_back(pointi);
	}
	for (int i = 7; i <= 11; i++)
	{
		Vector3D pointi = Vector3D::point(
								std::cos(PI / 5 + ((i - 7) * 2 * PI / 5)),
								std::sin(PI / 5 + ((i - 7) * 2 * PI / 5)),
								-0.5);
		icosahedron.points.push_back(pointi);
	}
	Vector3D point12 = Vector3D::point(0, 0, -std::sqrt(5) / 2);
	icosahedron.points.push_back(point12);

	// 1 1 1 1 1 2 3 3 4 4  5  5  6  6  2 12 12 12 12 12
	// 2 3 4 5 6 7 7 8 8 9  9 10 10 11 11  8  9 10 11  7
	// 3 4 5 6 2 3 8 4 9 5 10  6 11  2  7  7  8  9 10 11
	// the indices in the course are 1-based, so we need to subtract 1
	icosahedron.faces.push_back(Face({ 0,  1,  2}));
	icosahedron.faces.push_back(Face({ 0,  2,  3}));
	icosahedron.faces.push_back(Face({ 0,  3,  4}));
	icosahedron.faces.push_back(Face({ 0,  4,  5}));
	icosahedron.faces.push_back(Face({ 0,  5,  1}));
	icosahedron.faces.push_back(Face({ 1,  6,  2}));
	icosahedron.faces.push_back(Face({ 2,  6,  7}));
	icosahedron.faces.push_back(Face({ 2,  7,  3}));
	icosahedron.faces.push_back(Face({ 3,  7,  8}));
	icosahedron.faces.push_back(Face({ 3,  8,  4}));
	icosahedron.faces.push_back(Face({ 4,  8,  9}));
	icosahedron.faces.push_back(Face({ 4,  9,  5}));
	icosahedron.faces.push_back(Face({ 5,  9, 10}));
	icosahedron.faces.push_back(Face({ 5, 10,  1}));
	icosahedron.faces.push_back(Face({ 1, 10,  6}));
	icosahedron.faces.push_back(Face({11,  7,  6}));
	icosahedron.faces.push_back(Face({11,  8,  7}));
	icosahedron.faces.push_back(Face({11,  9,  8}));
	icosahedron.faces.push_back(Face({11, 10,  9}));
	icosahedron.faces.push_back(Face({11,  6, 10}));

	return icosahedron;
}




Figure gengine::Platonic::createDodecahedron()
{
	Figure dodecahedron;
	Figure icosahedron = createIcosahedron();

	for (size_t i = 0; i < icosahedron.faces.size(); i++)
	{
		double x = 0;
		double y = 0;
		double z = 0;
		for (size_t j = 0; j < icosahedron.faces[i].point_indexes.size(); j++)
		{
			x += icosahedron.points[icosahedron.faces[i].point_indexes[j]].x;
			y += icosahedron.points[icosahedron.faces[i].point_indexes[j]].y;
			z += icosahedron.points[icosahedron.faces[i].point_indexes[j]].z;
		}
		x /= 3;
		y /= 3;
		z /= 3;

		Vector3D center = Vector3D::point(x, y, z);
		dodecahedron.points.push_back(center);
	}


	//1 1  2  3  4  5 20 20 19 18 17 16
	//2 6  8 10 12 14 19 15 13 11  9  7
	//3 7  9 11 13 15 18 14 12 10  8  6
	//4 8 10 12 14  6 17 13 11  9  7 15
	//5 2  3  4  5  1 16 19 18 17 16 20
	dodecahedron.faces.push_back(Face({ 0,  1,  2,  3,  4}));
	dodecahedron.faces.push_back(Face({ 0,  5,  6,  7,  1}));
	dodecahedron.faces.push_back(Face({ 1,  7,  8,  9,  2}));
	dodecahedron.faces.push_back(Face({ 2,  9, 10, 11,  3}));
	dodecahedron.faces.push_back(Face({ 3, 11, 12, 13,  4}));
	dodecahedron.faces.push_back(Face({ 4, 13, 14,  5,  0}));
	dodecahedron.faces.push_back(Face({19, 18, 17, 16, 15}));
	dodecahedron.faces.push_back(Face({19, 14, 13, 12, 18}));
	dodecahedron.faces.push_back(Face({18, 12, 11, 10, 17}));
	dodecahedron.faces.push_back(Face({17, 10,  9,  8, 16}));
	dodecahedron.faces.push_back(Face({16,  8,  7,  6, 15}));
	dodecahedron.faces.push_back(Face({15,  6,  5, 14, 19}));

	return dodecahedron;
}

Figure gengine::Platonic::createCone(const int n, const double height)
{
	Figure cone;
	double r = 1;

	Vector3D top = Vector3D::point(0, 0, height);
	cone.points.push_back(top);

	for (int i = 0; i <= n; i++)
	{
		double angle = 2 * PI * i / n;
		Vector3D point = Vector3D::point(r * std::cos(angle), r * std::sin(angle), 0);
		cone.points.push_back(point);
	}
	for (int i = 0; i <= n; i++)
	{
		Face triangle({0, (i + 1) % n + 1, i + 1});
		cone.faces.push_back(triangle);
	}

	Face bottom;
	for (int i = 0; i < n; i++)
	{
		bottom.point_indexes.push_back(i + 1);
	}
	bottom.point_indexes.push_back(1);
	cone.faces.push_back(bottom);

	return cone;
}

Figure gengine::Platonic::createCylinder(const int n, const double height)
{
	Figure cylinder;

	for (int i = 0; i <= n; i++)
	{
		double angle = 2 * PI * i / n;
		Vector3D point1 = Vector3D::point(std::cos(angle), std::sin(angle), 0);
		Vector3D point2 = Vector3D::point(std::cos(angle), std::sin(angle), height);
		cylinder.points.push_back(point1);
		cylinder.points.push_back(point2);
	}

	for (int i = 0; i <= n; i++)
	{
		Face rectangle({2 * i,
						2 * i + 1,
						(2 * (i + 1)) % (2 * n) + 1,
						(2 * (i + 1)) % (2 * n)});
		cylinder.faces.push_back(rectangle);
	}

	Face bottom;
	for (int i = 0; i < 2 * n; i++)
	{
		if (i % 2 == 0)
			bottom.point_indexes.push_back(i);
	}
	cylinder.faces.push_back(bottom);

	Face top;
	for (int i = 0; i < 2 * n; i++)
	{
		if (i % 2 == 1)
			top.point_indexes.push_back(i);
	}
	cylinder.faces.push_back(top);

	return cylinder;
}

Figure gengine::Platonic::createSphere(const int n)
{
	Figure sphere = createIcosahedron();

	for (int i = 0; i < n; i++)
	{
		Figure new_sphere;
		for (size_t j = 0; j < sphere.faces.size(); j++)
		{
			Vector3D point1 = sphere.points[sphere.faces[j].point_indexes[0]];
			Vector3D point2 = sphere.points[sphere.faces[j].point_indexes[1]];
			Vector3D point3 = sphere.points[sphere.faces[j].point_indexes[2]];

			Vector3D point12 = (point1 + point2) / 2;
			Vector3D point23 = (point2 + point3) / 2;
			Vector3D point31 = (point3 + point1) / 2;

			new_sphere.points.push_back(point1);
			new_sphere.points.push_back(point12);
			new_sphere.points.push_back(point31);

			new_sphere.points.push_back(point12);
			new_sphere.points.push_back(point2);
			new_sphere.points.push_back(point23);

			new_sphere.points.push_back(point23);
			new_sphere.points.push_back(point3);
			new_sphere.points.push_back(point31);

			new_sphere.points.push_back(point12);
			new_sphere.points.push_back(point23);
			new_sphere.points.push_back(point31);

			Face new_face1({static_cast<int>(new_sphere.points.size()) - 12,
							static_cast<int>(new_sphere.points.size()) - 11,
							static_cast<int>(new_sphere.points.size()) - 10});
			Face new_face2({static_cast<int>(new_sphere.points.size()) - 9,
							static_cast<int>(new_sphere.points.size()) - 8,
							static_cast<int>(new_sphere.points.size()) - 7});
			Face new_face3({static_cast<int>(new_sphere.points.size()) - 6,
							static_cast<int>(new_sphere.points.size()) - 5,
							static_cast<int>(new_sphere.points.size()) - 4});
			Face new_face4({static_cast<int>(new_sphere.points.size()) - 3,
							static_cast<int>(new_sphere.points.size()) - 2,
							static_cast<int>(new_sphere.points.size()) - 1});

			new_sphere.faces.push_back(new_face1);
			new_sphere.faces.push_back(new_face2);
			new_sphere.faces.push_back(new_face3);
			new_sphere.faces.push_back(new_face4);
		}
		sphere = new_sphere;
	}

	for (auto &point : sphere.points)
	{
		point.normalise();
	}

	return sphere;
}

Figure gengine::Platonic::createTorus(const double r, const double R, const int n, const int m)
{
	Figure torus;

	for (int i = 0; i < n; i++)
	{
		double angle1 = 2 * PI * i / n;
		for (int j = 0; j < m; j++)
		{
			double angle2 = 2 * PI * j / m;
			Vector3D point = Vector3D::point(
								(R + r * std::cos(angle2)) * std::cos(angle1),
								(R + r * std::cos(angle2)) * std::sin(angle1),
								r * std::sin(angle2));
			torus.points.push_back(point);
		}
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			Face rectangle({i * m + j,
							i * m + (j + 1) % m,
							((i + 1) % n) * m + (j + 1) % m,
							((i + 1) % n) * m + j});
			torus.faces.push_back(rectangle);
		}
	}

	return torus;
}


Figure gengine::create3DLSystem(const ini::Section &LSystem3DConfig)
{
	Figure figure;
	LParser::LSystem3D l_system;

	std::stack<std::tuple<Vector3D, Vector3D, Vector3D, Vector3D>> stack;
	Vector3D H = Vector3D::vector(1, 0, 0);
	Vector3D L = Vector3D::vector(0, 1, 0);
	Vector3D U = Vector3D::vector(0, 0, 1);
	Vector3D current_point = Vector3D::point(0, 0, 0);

	std::ifstream lSystemFile(LSystem3DConfig["inputfile"].as_string_or_die());
	if (lSystemFile.fail())
		return figure;

	lSystemFile >> l_system;
	lSystemFile.close();

	const double delta = degToRad(l_system.get_angle());
	std::string full_replacement = l_system.get_initiator();

	for (unsigned int i = 0; i < l_system.get_nr_iterations(); i++)
	{
		std::string new_string;
		for (auto c : full_replacement)
		{
			if (c == '+' || c == '-' || c == '(' || c == ')'
				|| c == '^' || c == '&' || c == '\\' || c == '/' || c == '|')
			{
				new_string.push_back(c);
				continue;
			}
			const std::string& replacement = l_system.get_replacement(c);
			new_string.append(replacement);
		}
		full_replacement = new_string;
	}


	for (auto c : full_replacement)
	{
		auto Hnew = H;
		auto Lnew = L;
		auto Unew = U;
		switch (c)
		{
			case  '+':
			{
				// Hnew = (hx, hy, hz ) cos δ + (lx, ly, lz ) sin δ
				// Lnew = −(hx, hy, hz ) sin δ + (lx, ly, lz ) cos δ
				Hnew = H * std::cos(delta) + L * std::sin(delta);
				Lnew = L * std::cos(delta) - H * std::sin(delta);
				break;
			}
			case  '-':
			{
				// Hnew = (hx, hy, hz ) cos -δ + (lx, ly, lz ) sin -δ
				// Lnew = −(hx, hy, hz ) sin -δ + (lx, ly, lz ) cos -δ
				Hnew = H * std::cos(-delta) + L * std::sin(-delta);
				Lnew = L * std::cos(-delta) - H * std::sin(-delta);
				break;
			}
			case  '^':
			{
				// Hnew = (hx, hy, hz ) cos δ + (ux, uy, uz ) sin δ
				// Unew = −(hx, hy, hz ) sin δ + (ux, uy, uz ) cos δ
				Hnew = H * std::cos(delta) + U * std::sin(delta);
				Unew = U * std::cos(delta) - H * std::sin(delta);
				break;
			}
			case  '&':
			{
				// Hnew = (hx, hy, hz ) cos -δ + (ux, uy, uz ) sin -δ
				// Unew = −(hx, hy, hz ) sin -δ + (ux, uy, uz ) cos -δ
				Hnew = H * std::cos(-delta) + U * std::sin(-delta);
				Unew = U * std::cos(-delta) - H * std::sin(-delta);
				break;
			}
			case  '\\':
			{
				// Lnew = (lx, ly, lz ) cos δ − (ux, uy, uz ) sin δ
				// Unew = (lx, ly, lz ) sin δ + (ux, uy, uz ) cos δ
				Lnew = L * std::cos(delta) - U * std::sin(delta);
				Unew = U * std::cos(delta) + L * std::sin(delta);
				break;
			}
			case  '/':
			{
				// Lnew = (lx, ly, lz ) cos -δ − (ux, uy, uz ) sin -δ
				// Unew = (lx, ly, lz ) sin -δ + (ux, uy, uz ) cos -δ
				Lnew = L * std::cos(-delta) - U * std::sin(-delta);
				Unew = U * std::cos(-delta) + L * std::sin(-delta);
				break;
			}
			case  '|':
			{
				Hnew = -H;
				Lnew = -L;
				break;
			}
			case  '(':
			{
				stack.emplace(current_point, H, L, U);
				break;
			}
			case  ')':
			{
				auto top = stack.top();
				current_point = std::get<0>(top);
				Hnew = std::get<1>(top);
				Lnew = std::get<2>(top);
				Unew = std::get<3>(top);
				stack.pop();
				break;
			}
			default:
			{
				Vector3D new_point = current_point + H;
				if (l_system.draw(c))
				{
					figure.points.push_back(current_point);
					figure.points.push_back(new_point);
					Face face;
					face.point_indexes.push_back(static_cast<int>(figure.points.size()) - 2);
					face.point_indexes.push_back(static_cast<int>(figure.points.size()) - 1);
					figure.faces.push_back(face);
				}
				current_point = new_point;
				break;
			}
		}
		H = Hnew;
		L = Lnew;
		U = Unew;
	}

	return figure;
}
