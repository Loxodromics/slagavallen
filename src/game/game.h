/**
 *  game.h
 *  Slagavallen
 *
 *  Created by philipp on 21.12.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
 */
#pragma once

#include "src/logic/map.h"

namespace LFD {

namespace slagavallen {

class Game: public QObject
{
	Q_OBJECT
public:
	explicit Game(QObject* parent = nullptr);

	std::shared_ptr<Map> currentMap() const;
	void setCurrentMap(const std::shared_ptr<Map>& currentMap);

protected:
	std::shared_ptr<Map> m_currentMap;
};

}	/// namespace slagavallen

}	/// namespace LFD
