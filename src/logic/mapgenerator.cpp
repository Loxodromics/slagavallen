/**
 *  mapgenerator.cpp
 *  Slagavallen
 *
 *  Created by philipp on 15.11.2020.
 *  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
 */
#include "mapgenerator.h"

#include "qmath.h"
#include <QtCore/QRandomGenerator>
#include <vector>

namespace LFD {

namespace slagavallen {

MapGenerator::MapGenerator() {}

std::shared_ptr<Map> LFD::slagavallen::MapGenerator::generateMap(unsigned int width, unsigned int height)
{
	qreal sinFactor = 0.6;

	auto newMap = std::make_shared<Map>(width, height);

	/// randomly fill the world with tiles
	for (unsigned int i_v = 0; i_v <= width; ++i_v) {
		for (unsigned int i_h = 0; i_h <= height; ++i_h) {
			unsigned int i = i_v * width + i_h;
			auto tile = std::make_shared<Tile>(i,
			  static_cast<Tile::TerrainType>(
				QRandomGenerator::global()->bounded(static_cast<int>(Tile::TerrainType::NumTerrainType) - 1) + 1),
			  static_cast<Tile::Rotation>(
				QRandomGenerator::global()->bounded(static_cast<int>(Tile::Rotation::NumRotation))),
			  std::vector<double>({ qSin(sinFactor * i_h) + qCos(sinFactor * i_v),
				qSin(sinFactor * (1.0 + i_h)) + qCos(sinFactor * (i_v)),
				qSin(sinFactor * (1.0 + i_h)) + qCos(sinFactor * (1.0 + i_v)),
				qSin(sinFactor * (i_h)) + qCos(sinFactor * (1.0 + i_v)) }));
			newMap->setTile(tile, tile->id);
		}
	}

	return newMap;
}

}	// namespace slagavallen

}	// namespace LFD
