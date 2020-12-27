/**
 *  game.cpp
 *  Slagavallen
 *
 *  Created by philipp on 21.12.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
*/
#include "game.h"

namespace LFD {

namespace slagavallen {

Game::Game(QObject* parent)
	:QObject(parent)
{

}

std::shared_ptr<Map> Game::currentMap() const
{
	return m_currentMap;
}

void Game::setCurrentMap(const std::shared_ptr<Map>& currentMap)
{
	m_currentMap = currentMap;
}

}	/// namespace slagavallen

}	/// namespace LFD
