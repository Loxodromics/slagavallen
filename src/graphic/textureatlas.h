/**
 *  textureatlas.h
 *  Slagavallen
 *
 *  Created by philipp on 02.11.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
 */
#pragma once

#include <QMap>
#include <QPointF>
#include <QVector>

#include "src/logic/tile.h"

namespace LFD {

namespace slagavallen {

class TextureAtlas {
public:
	enum class Axis {
		Horizontal,
		Vertical
	};

	TextureAtlas();

	double textureCoordinates(std::shared_ptr<Tile> tile, Axis axis, unsigned int corner) const;

protected:
	double rotatedTextureCoordinates(
	  QVector<double> textureCoordinates, Tile::Rotation rotation, Axis axis, unsigned int corner) const;
	double coordinate(QVector<double> textureCoordinates, Axis axis, unsigned int corner) const;

	QMap<Tile::TerrainType, QVector<double>> m_textureCoordinates;
};

}	// namespace slagavallen

}	// namespace LFD
