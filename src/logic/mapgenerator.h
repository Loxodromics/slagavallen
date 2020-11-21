/**
 *  mapgenerator.h
 *  Slagavallen
 *
 *  Created by philipp on 15.11.2020.
 *  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
*/
#pragma once

#include "map.h"
#include "src/thirdparty/simplexnoise.h"

namespace LFD {

namespace slagavallen {

class MapGenerator
{
public:
	explicit MapGenerator(unsigned int seed, unsigned int octaves);

	std::shared_ptr<Map> generateMap(unsigned int width, unsigned int height);
	virtual double elevationAt(const double x, const double y, const double z);

protected:
	double avoidZero(const double value);
	SimplexNoise m_simplexElevation;
	unsigned int m_octaves;

	/// Debug
	double m_min = 99999.9;
	double m_max = 0.0;

};

}	// namespace slagavallen

}	// namespace LFD
