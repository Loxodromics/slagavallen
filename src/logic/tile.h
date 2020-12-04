/**
 *  tile.h
 *  Slagavallen
 *
 *  Created by philipp on 02.11.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
 */
#pragma once

#include <vector>
#include <QVector3D>

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

	explicit Tile(unsigned int id, TerrainType terrainType, Rotation rotation, std::vector<double> height, std::vector<QVector3D*> normal);

	unsigned int id;
	TerrainType terrainType;
	Rotation rotation;
	std::vector<double> height;
	std::vector<QVector3D*> normal;

	/// Ideas:
	/// layer
	/// visibility [0..1]
	/// discovered
};

}	// namespace slagavallen

}	// namespace LFD
