/**
 *  game.cpp
 *  Slagavallen
 *
 *  Created by philipp on 21.12.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
*/
#include <QtCore/QRandomGenerator>

#include "game.h"
#include "src/logic/mapgenerator.h"

namespace LFD {

namespace slagavallen {

Game::Game(QObject* parent)
	: QObject(parent)
	, m_seed(12345)
	, m_tileMode(TileMode::RectFlat)
{
	this->newGame(12345);
}

void Game::newGame()
{
	this->newGame(QRandomGenerator::global()->bounded(99999));
}

void Game::newGame(unsigned int seed)
{
	this->setSeed(seed);
	MapGenerator mapGenerator(this->m_seed, 5);
	auto map = mapGenerator.generateMap(32, 32);
	this->setCurrentMap(map);

	emit starteNewGame();
}

std::shared_ptr<Map> Game::currentMap() const
{
	return m_currentMap;
}

void Game::setCurrentMap(const std::shared_ptr<Map>& currentMap)
{
	m_currentMap = currentMap;
}

unsigned int Game::seed() const
{
	return m_seed;
}

void Game::setSeed(unsigned int seed)
{
	m_seed = seed;
}

Game::TileMode Game::tileMode() const
{
	return this->m_tileMode;
}

void Game::setTileMode(const Game::TileMode& tileMode)
{
	this->m_tileMode = tileMode;

	emit tileModeChanged(this->m_tileMode);
}

void Game::setTileMode(const int tileMode)
{
	Game::TileMode newTileMode = static_cast<Game::TileMode>(tileMode);
	this->setTileMode(newTileMode);
}

}	/// namespace slagavallen

}	/// namespace LFD
