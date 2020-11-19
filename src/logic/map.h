/**
 *  map.h
 *  Slagavallen
 *
 *  Created by philipp on 15.11.2020.
 *  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
*/
#pragma once

#include <QObject>
#include <vector>

#include "tile.h"

namespace LFD {

namespace slagavallen {

class Map
{
public:
	explicit Map(unsigned int width, unsigned int height);

	void setTile(std::shared_ptr<Tile> tile, unsigned int id);
	std::shared_ptr<Tile> getTile(unsigned int id);
	std::shared_ptr<Tile> getTile(unsigned int i_v, unsigned int i_h);

	unsigned int width() const;
	unsigned int height() const;
	unsigned int size() const;

protected:
	unsigned int m_width;
	unsigned int m_height;
	std::vector<std::shared_ptr<Tile>> m_tiles;

};

}	// namespace slagavallen

}	// namespace LFD
