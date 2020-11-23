/**
 *  mapgenerator.cpp
 *  Slagavallen
 *
 *  Created by philipp on 15.11.2020.
 *  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
 */
#include "mapgenerator.h"

#include "qmath.h"
#include <QDebug>
#include <QtCore/QRandomGenerator>
#include <vector>

namespace LFD {

namespace slagavallen {

MapGenerator::MapGenerator(unsigned int seed, unsigned int octaves)
	: m_simplexElevation(seed)
	, m_simplexTerrainType(seed + 1)
	, m_octaves(octaves)
{
}

double MapGenerator::elevationAt(const double x, const double y, const double z)
{
	double newX = this->avoidZero(x) * 0.05;
	double newY = this->avoidZero(y) * 0.05;
	double newZ = this->avoidZero(z) * 0.05;

	/// magic numbers to get the a range that is _roughly_ [0, 1], that's good enough
	double elevation = this->m_simplexElevation.fractal(this->m_octaves, newX, newY, newZ) / 1.3 + 0.45;

	//	elevation = pow(elevation, Values::getInstance().terrainExp());

	if (elevation > m_max) {
		m_max = elevation;
		qDebug() << "min: " << m_min << "max: " << m_max;
	}

	if (elevation < m_min) {
		m_min = elevation;
		qDebug() << "min: " << m_min << "max: " << m_max;
	}

	return elevation;
}

Tile::TerrainType MapGenerator::terrainTypeAt(const double x, const double y, const double z)
{

	double newX = this->avoidZero(x) * 0.05;
	double newY = this->avoidZero(y) * 0.05;
	double newZ = this->avoidZero(z) * 0.05;

	/// magic numbers to get the a range that is _roughly_ [0, 1], that's good enough
	double fract = this->m_simplexTerrainType.fractal(2, newX, newY, newZ) / 1.3 + 0.45;

	Tile::TerrainType terrainType = static_cast<Tile::TerrainType>(fract * 4 + 1);

	return terrainType;
}

double MapGenerator::avoidZero(const double value)
{
	if ((value > -0.00001) && (value < 0.00001))
		return 0.0002;
	else
		return value;
}

std::shared_ptr<Map> MapGenerator::generateMap(unsigned int width, unsigned int height)
{
	auto newMap = std::make_shared<Map>(width, height);

	/// randomly fill the world with tiles
	for (unsigned int i_v = 0; i_v <= width; ++i_v) {
		for (unsigned int i_h = 0; i_h <= height; ++i_h) {
			unsigned int i = i_v * width + i_h;
			auto tile = std::make_shared<Tile>(i, terrainTypeAt(i_h, i_v, 0.0),
			  static_cast<Tile::Rotation>(
				QRandomGenerator::global()->bounded(static_cast<int>(Tile::Rotation::NumRotation))),
			  std::vector<double>({ elevationAt(i_h, i_v, 0.0), elevationAt(1.0 + i_h, i_v, 0.0),
				elevationAt(1.0 + i_h, 1.0 + i_v, 0.0), elevationAt(i_h, 1.0 + i_v, 0.0) }));
			newMap->setTile(tile, tile->id);
		}
	}

	return newMap;
}

}	// namespace slagavallen

}	// namespace LFD
