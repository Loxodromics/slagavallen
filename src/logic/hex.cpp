/**
 *  hex.cpp
 *  Slagavallen
 *
 *  Created by philipp on 08.01.2021.
 *  Copyright (c) 2021 Philipp Engelhard. All rights reserved.
 */
#include "hex.h"
#include <cmath>

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


}	/// namespace slagavallen

}	/// namespace LFD
