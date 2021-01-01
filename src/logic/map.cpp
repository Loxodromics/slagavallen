/**
 *  map.cpp
 *  Slagavallen
 *
 *  Created by philipp on 15.11.2020.
 *  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
*/
#include "map.h"

namespace LFD {

namespace slagavallen {

Map::Map(unsigned int width, unsigned int height, unsigned int tileSize)
	: m_width(width)
	, m_height(height)
	, m_tileSize(tileSize)
	, m_tiles()
{
	this->m_tiles.resize(width * height);
}

void Map::setTile(std::shared_ptr<Tile> tile, unsigned int id)
{
	if (id < this->m_tiles.size()) {
		this->m_tiles[id] = tile;
	}
}

std::shared_ptr<Tile> Map::getTile(unsigned int id)
{
	return this->m_tiles[id];
}

std::shared_ptr<Tile> Map::getTile(unsigned int i_v, unsigned int i_h)
{
	return this->m_tiles[i_v * this->m_width + i_h];
}

unsigned int Map::width() const
{
	return this->m_width;
}

unsigned int Map::height() const
{
	return this->m_height;
}

unsigned int Map::size() const
{
	return this->m_width * this->m_height;
}

unsigned int Map::tileSize() const
{
	return this->m_tileSize;
}

void Map::setTileSize(unsigned int tileSize)
{
	this->m_tileSize = tileSize;
}

}	// namespace slagavallen

}	// namespace LFD
