/**
 *  hex.h
 *  Slagavallen
 *
 *  Created by philipp on 08.01.2021.
 *  Copyright (c) 2021 Philipp Engelhard. All rights reserved.
 *
 *  Based on and big thanks to Amit Patel (@redblobgames)
 *  http://www.redblobgames.com/grids/hexagons/
 */
#pragma once

#include <QPointF>
#include <vector>
#include <cmath>

namespace LFD {

namespace slagavallen {

struct Hex {
	const int q;
	const int r;
	const int s;
	Hex(int q_, int r_, int s_);
};

struct FractionalHex
{
	const double q;
	const double r;
	const double s;
	FractionalHex(double q_, double r_, double s_);
};

struct Orientation {
	const double f0;
	const double f1;
	const double f2;
	const double f3;
	const double b0;
	const double b1;
	const double b2;
	const double b3;
	const double start_angle;

	Orientation(double f0_, double f1_, double f2_, double f3_, double b0_, double b1_, double b2_, double b3_,
	  double start_angle_);
};

struct Layout {
	const Orientation orientation;
	const QPointF size;
	const QPointF origin;
	Layout(Orientation orientation_, QPointF size_, QPointF origin_);
};

Hex hexAdd(Hex a, Hex b)
{
	return Hex(a.q + b.q, a.r + b.r, a.s + b.s);
}

Hex hexSubtract(Hex a, Hex b)
{
	return Hex(a.q - b.q, a.r - b.r, a.s - b.s);
}

Hex hexScale(Hex a, int k)
{
	return Hex(a.q * k, a.r * k, a.s * k);
}

Hex hexRotateLeft(Hex a)
{
	return Hex(-a.s, -a.q, -a.r);
}

Hex hexRotateRight(Hex a)
{
	return Hex(-a.r, -a.s, -a.q);
}

const std::vector<Hex> hexDirections = {Hex(1, 0, -1), Hex(1, -1, 0), Hex(0, -1, 1), Hex(-1, 0, 1), Hex(-1, 1, 0), Hex(0, 1, -1)};
Hex hex_direction(int direction)
{
	return hexDirections[direction];
}

Hex hexNeighbor(Hex hex, int direction)
{
	return hexAdd(hex, hex_direction(direction));
}

const std::vector<Hex> hexDiagonals = {Hex(2, -1, -1), Hex(1, -2, 1), Hex(-1, -1, 2), Hex(-2, 1, 1), Hex(-1, 2, -1), Hex(1, 1, -2)};
Hex hex_diagonal_neighbor(Hex hex, int direction)
{
	return hexAdd(hex, hexDiagonals[direction]);
}

int hexLength(Hex hex)
{
	return int((abs(hex.q) + abs(hex.r) + abs(hex.s)) / 2);
}

int hexDistance(Hex a, Hex b)
{
	return hexLength(hexSubtract(a, b));
}

FractionalHex hexLerp(FractionalHex a, FractionalHex b, double t);
std::vector<Hex> hexLinedraw(Hex a, Hex b);

const Orientation layout_pointy
  = Orientation(sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0, sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0, 0.5);
const Orientation layout_flat
  = Orientation(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0), 2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0, 0.0);

QPointF hexToPixel(Layout layout, Hex h);
FractionalHex pixelToHex(Layout layout, QPointF p);
QPointF hexCornerOffset(Layout layout, int corner);
std::vector<QPointF> polygonCorners(Layout layout, Hex h);

}	/// namespace slagavallen

}	/// namespace LFD
