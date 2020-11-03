//
//  tile.h
//  slagavallen
//
//  Created by philipp on 02.11.2020.
//  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
//
#pragma once

namespace LFD {

namespace slagavallen {

class Tile
{
public:

	enum class TerrainType {
		None,
		Grass0,
		Grass1,
		Grass2,
		Grass3,
	};

	enum class Rotation {
		Rot_0,
		Rot_90,
		Rot_180,
		Rot_270,
	};

	explicit Tile();

	unsigned int id;
	TerrainType terrainType;
	Rotation rotation;

};

}	// namespace slagavallen

}	// namespace LFD

