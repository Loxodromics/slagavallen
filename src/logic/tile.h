/**
 *  tile.h
 *  Slagavallen
 *
 *  Created by philipp on 02.11.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
 */
#pragma once

namespace LFD {

namespace slagavallen {

class Tile {
public:
	enum class TerrainType {
		None,
		Grass0,
		Grass1,
		Grass2,
		Grass3,
		NumTerrainType
	};

	enum class Rotation {
		Rot_0,
		Rot_90,
		Rot_180,
		Rot_270,
		NumRotation
	};

	explicit Tile();

	unsigned int id;
	TerrainType terrainType;
	Rotation rotation;
};

}	// namespace slagavallen

}	// namespace LFD
