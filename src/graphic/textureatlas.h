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

	qreal textureCoordinates(std::shared_ptr<Tile> tile, Axis axis, unsigned int corner);

protected:
	qreal rotatedTextureCoordinates(QVector<qreal> textureCoordinates, Tile::Rotation rotation, Axis axis, unsigned int corner);
	qreal coordinate(QVector<qreal> textureCoordinates, Axis axis, unsigned int corner);

	QMap<Tile::TerrainType, QVector<qreal>> m_textureCoordinates;
};

}	// namespace slagavallen

}	// namespace LFD
