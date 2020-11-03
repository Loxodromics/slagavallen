//
//  textureatlas.cpp
//  slagavallen
//
//  Created by philipp on 02.11.2020.
//  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
//

#include "textureatlas.h"

namespace LFD {

namespace slagavallen {

TextureAtlas::TextureAtlas()
{
	this->m_textureCoordinates.insert(Tile::TerrainType::Grass0, QVector<qreal> { 0.0f, 0.0f, 0.5f, 0.5f });
	this->m_textureCoordinates.insert(Tile::TerrainType::Grass1, QVector<qreal> { 0.5f, 0.0f, 1.0f, 0.5f });
	this->m_textureCoordinates.insert(Tile::TerrainType::Grass2, QVector<qreal> { 0.0f, 0.5f, 0.5f, 1.0f });
	this->m_textureCoordinates.insert(Tile::TerrainType::Grass3, QVector<qreal> { 0.5f, 0.5f, 1.0f, 1.0f });
}

qreal TextureAtlas::textureCoordinates(std::shared_ptr<Tile> tile, Side side)
{
	return TextureAtlas::rotatedTextureCoordinates(this->m_textureCoordinates[tile->terrainType], tile->rotation, side);
}

qreal TextureAtlas::rotatedTextureCoordinates(QVector<qreal> textureCoordinates, Tile::Rotation rotation, Side side)
{
	return textureCoordinates[static_cast<int>(side)];
}

}	// namespace slagavallen

}	// namespace LFD
