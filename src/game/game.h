/**
 *  game.h
 *  Slagavallen
 *
 *  Created by philipp on 21.12.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
 */
#pragma once

#include <QObject>
#include "src/logic/map.h"

namespace LFD {

namespace slagavallen {

class Game: public QObject
{
	Q_OBJECT
public:
	explicit Game(QObject* parent = nullptr);


	Q_INVOKABLE void newGame();
	void newGame(unsigned int seed);

	std::shared_ptr<Map> currentMap() const;
	void setCurrentMap(const std::shared_ptr<Map>& currentMap);

	unsigned int seed() const;
	void setSeed(unsigned int seed);

signals:
	void starteNewGame();

public slots:
	void changeChanges();

protected:
	std::shared_ptr<Map> m_currentMap;
	unsigned int m_seed;
};

}	/// namespace slagavallen

}	/// namespace LFD
