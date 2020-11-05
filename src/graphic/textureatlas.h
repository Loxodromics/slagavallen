//
//  textureatlas.h
//  slagavallen
//
//  Created by philipp on 02.11.2020.
//  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
//
#pragma once

#include <QMap>
#include <QPointF>
#include <QVector>

#include "src/logic/tile.h"

namespace LFD {

namespace slagavallen {

class TextureAtlas {
public:
	enum class Side {
		Left,
		Top,
		Right,
		Bottom,
		NumSide
	};

	TextureAtlas();

	qreal textureCoordinates(std::shared_ptr<Tile> tile, Side side);

protected:
	qreal rotatedTextureCoordinates(QVector<qreal> textureCoordinates, Tile::Rotation rotation, Side side);

	QMap<Tile::TerrainType, QVector<qreal>> m_textureCoordinates;
};

}	// namespace slagavallen

}	// namespace LFD
