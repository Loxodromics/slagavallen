/**
 *  hex.cpp
 *  Slagavallen
 *
 *  Created by philipp on 08.01.2021.
 *  Copyright (c) 2021 Philipp Engelhard. All rights reserved.
 */
#include "hex.h"

namespace LFD {

namespace slagavallen {

Hex::Hex(int q_, int r_, int s_)
	: q(q_)
	, r(r_)
	, s(s_)
{
	if (q + r + s != 0)
		throw "q + r + s must be 0";
}

Orientation::Orientation(double f0_, double f1_, double f2_, double f3_, double b0_, double b1_, double b2_, double b3_, double start_angle_)
	: f0(f0_)
	, f1(f1_)
	, f2(f2_)
	, f3(f3_)
	, b0(b0_)
	, b1(b1_)
	, b2(b2_)
	, b3(b3_)
	, start_angle(start_angle_)
{
}

Layout::Layout(Orientation orientation_, QPointF size_, QPointF origin_)
	: orientation(orientation_)
	, size(size_)
	, origin(origin_)
{
}

FractionalHex::FractionalHex(double q_, double r_, double s_): q(q_), r(r_), s(s_) {
	if (round(q + r + s) != 0) throw "q + r + s must be 0";
}

Hex hex_round(FractionalHex h)
{
	int qi = int(round(h.q));
	int ri = int(round(h.r));
	int si = int(round(h.s));
	double q_diff = abs(qi - h.q);
	double r_diff = abs(ri - h.r);
	double s_diff = abs(si - h.s);
	if (q_diff > r_diff && q_diff > s_diff) {
		qi = -ri - si;
	}
	else if (r_diff > s_diff) {
		ri = -qi - si;
	}
	else {
		si = -qi - ri;
	}
	return Hex(qi, ri, si);
}


FractionalHex hexLerp(FractionalHex a, FractionalHex b, double t)
{
	return FractionalHex(a.q * (1.0 - t) + b.q * t, a.r * (1.0 - t) + b.r * t, a.s * (1.0 - t) + b.s * t);
}

std::vector<Hex> hexLinedraw(Hex a, Hex b)
{
	int N = hexDistance(a, b);
	FractionalHex aNudge = FractionalHex(a.q + 1e-06, a.r + 1e-06, a.s - 2e-06);
	FractionalHex bNudge = FractionalHex(b.q + 1e-06, b.r + 1e-06, b.s - 2e-06);
	std::vector<Hex> results = {};
	double step = 1.0 / std::max(N, 1);
	for (int i = 0; i <= N; i++) {
		results.push_back(hex_round(hexLerp(aNudge, bNudge, step * i)));
	}
	return results;
}

QPointF hexToPixel(Layout layout, Hex h)
{
	Orientation M = layout.orientation;
	QPointF size = layout.size;
	QPointF origin = layout.origin;
	double x = (M.f0 * h.q + M.f1 * h.r) * size.x();
	double y = (M.f2 * h.q + M.f3 * h.r) * size.y();
	return QPointF(x + origin.x(), y + origin.y());
}

FractionalHex pixelToHex(Layout layout, QPointF p)
{
	Orientation M = layout.orientation;
	QPointF size = layout.size;
	QPointF origin = layout.origin;
	QPointF pt = QPointF((p.x() - origin.x()) / size.x(), (p.y() - origin.y()) / size.y());
	double q = M.b0 * pt.x() + M.b1 * pt.y();
	double r = M.b2 * pt.x() + M.b3 * pt.y();
	return FractionalHex(q, r, -q - r);
}

QPointF hexCornerOffset(Layout layout, int corner)
{
	Orientation M = layout.orientation;
	QPointF size = layout.size;
	double angle = 2.0 * M_PI * (M.start_angle - corner) / 6.0;
	return QPointF(size.x() * cos(angle), size.y() * sin(angle));
}

std::vector<QPointF> polygonCorners(Layout layout, Hex h)
{
	std::vector<QPointF> corners = {};
	QPointF center = hexToPixel(layout, h);
	for (int i = 0; i < 6; i++)
	{
		QPointF offset = hexCornerOffset(layout, i);
		corners.push_back(QPointF(center.x() + offset.x(), center.y() + offset.y()));
	}
	return corners;
}

Hex hexNeighbor(Hex hex, int direction)
{
	return hexAdd(hex, hexDirection(direction));
}

Hex hexDiagonalNeighbor(Hex hex, int direction)
{
	return hexAdd(hex, hexDiagonals[direction]);
}

Hex hexDirection(int direction)
{
	return hexDirections[direction];
}

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

int hexLength(Hex hex)
{
	return int((abs(hex.q) + abs(hex.r) + abs(hex.s)) / 2);
}

int hexDistance(Hex a, Hex b)
{
	return hexLength(hexSubtract(a, b));
}


}	/// namespace slagavallen

}	/// namespace LFD
