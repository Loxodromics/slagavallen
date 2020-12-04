/**
 *  mapgenerator.h
 *  Slagavallen
 *
 *  Created by philipp on 15.11.2020.
 *  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
*/
#pragma once

#include <QVector3D>

#include "map.h"
#include "src/thirdparty/simplexnoise.h"

namespace LFD {

namespace slagavallen {

class MapGenerator
{
public:
	explicit MapGenerator(unsigned int seed, unsigned int octaves);

	std::shared_ptr<Map> generateMap(unsigned int width, unsigned int height);
	double elevationAt(const double x, const double y, const double z) const;
	Tile::TerrainType terrainTypeAt(const double x, const double y, const double z);

protected:
	double avoidZero(const double value) const;
	SimplexNoise m_simplexElevation;
	SimplexNoise m_simplexTerrainType;
	unsigned int m_octaves;

	/// Debug
	double m_min = 99999.9;
	double m_max = 0.0;

	QVector3D* normalAt(const double x, const double y, const double z) const;
};

}	// namespace slagavallen

}	// namespace LFD
