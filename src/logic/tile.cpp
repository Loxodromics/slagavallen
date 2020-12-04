/**
 *  tile.cpp
 *  Slagavallen
 *
 *  Created by philipp on 02.11.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
 */
#include "tile.h"

namespace LFD {

namespace slagavallen {

Tile::Tile(unsigned int id, Tile::TerrainType terrainType, Tile::Rotation rotation, std::vector<double> height, std::vector<QVector3D*> normal)
	: id(id)
	, terrainType(terrainType)
	, rotation(rotation)
	, height(height)
	, normal(normal)
{

}

}	// namespace slagavallen

}	// namespace LFD
