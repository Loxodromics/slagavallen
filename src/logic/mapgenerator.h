/**
 *  mapgenerator.h
 *  Slagavallen
 *
 *  Created by philipp on 15.11.2020.
 *  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
*/
#pragma once

#include "map.h"

namespace LFD {

namespace slagavallen {

class MapGenerator
{
public:
	explicit MapGenerator();

	std::shared_ptr<Map> generateMap(unsigned int width, unsigned int height);
};

}	// namespace slagavallen

}	// namespace LFD
