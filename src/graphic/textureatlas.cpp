/**
 *  textureatlas.cpp
 *  Slagavallen
 *
 *  Created by philipp on 02.11.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
 */

#include "textureatlas.h"

namespace LFD {

namespace slagavallen {

TextureAtlas::TextureAtlas()
{
	this->m_textureCoordinates.insert(Tile::TerrainType::None,   QVector<qreal> { 0.0f, 0.0f, 0.0001f, 0.0001f });
	this->m_textureCoordinates.insert(Tile::TerrainType::Grass0, QVector<qreal> { 0.0f, 0.0f, 0.5f, 0.5f });
	this->m_textureCoordinates.insert(Tile::TerrainType::Grass1, QVector<qreal> { 0.5f, 0.0f, 1.0f, 0.5f });
	this->m_textureCoordinates.insert(Tile::TerrainType::Grass2, QVector<qreal> { 0.0f, 0.5f, 0.5f, 1.0f });
	this->m_textureCoordinates.insert(Tile::TerrainType::Grass3, QVector<qreal> { 0.5f, 0.5f, 1.0f, 1.0f });
}

qreal TextureAtlas::textureCoordinates(std::shared_ptr<Tile> tile, Axis axis, unsigned int corner)
{
	return TextureAtlas::rotatedTextureCoordinates(this->m_textureCoordinates[tile->terrainType], tile->rotation, axis, corner);
}

qreal TextureAtlas::rotatedTextureCoordinates(QVector<qreal> textureCoordinates, Tile::Rotation rotation, Axis axis, unsigned int corner)
{
	/// % 4 corners
	int rotatedCorner = (corner + static_cast<int>(rotation)) % 4;
	return this->coordinate(textureCoordinates, axis, rotatedCorner);
}

qreal TextureAtlas::coordinate(QVector<qreal> textureCoordinates, Axis axis, unsigned int corner)
{
	switch (corner) {
	case 0:
		/// Top Left
		if (axis == Axis::Horizontal)
			return textureCoordinates[0];
		else
			return textureCoordinates[1];
	case 1:
		/// Top Right
		if (axis == Axis::Horizontal)
			return textureCoordinates[2];
		else
			return textureCoordinates[1];
	case 2:
		/// Bottom Right
		if (axis == Axis::Horizontal)
			return textureCoordinates[2];
		else
			return textureCoordinates[3];
	case 3:
		/// Bottom Left
		if (axis == Axis::Horizontal)
			return textureCoordinates[0];
		else
			return textureCoordinates[3];

	default:
		return -1.0f;
	}

}

}	// namespace slagavallen

}	// namespace LFD
