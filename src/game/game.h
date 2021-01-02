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
#include <QObject>

namespace LFD {

namespace slagavallen {

class Game : public QObject {
	Q_OBJECT

public:
	enum class TileMode {
		RectFlat,
		RectIso,
		HexFlat,
		HexIso,
		NumTileMode,
	};

	explicit Game(QObject* parent = nullptr);

	Q_INVOKABLE void newGame();
	void newGame(unsigned int seed);

	std::shared_ptr<Map> currentMap() const;
	void setCurrentMap(const std::shared_ptr<Map>& currentMap);

	unsigned int seed() const;
	void setSeed(unsigned int seed);

	TileMode tileMode() const;
	void setTileMode(const TileMode& tileMode);
	Q_INVOKABLE void setTileMode(const int tileMode);

signals:
	void starteNewGame();
	void tileModeChanged(Game::TileMode tileMode);

protected:
	std::shared_ptr<Map> m_currentMap;
	unsigned int m_seed;
	TileMode m_tileMode;
};

}	/// namespace slagavallen

}	/// namespace LFD
