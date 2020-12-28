/**
 *  gameworlditem.cpp
 *  Slagavallen
 *
 *  Created by philipp on 27.09.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
 */

#include "gameworlditem.h"
#include "backgroundnode.h"

#include <QDebug>

namespace LFD {

namespace slagavallen {

GameWorldItem::GameWorldItem()
	: m_geometryChanged(false),
	  m_newGame(false),
	  m_mouseDownPos(0.0f, 0.0f),
	  m_gameWorldItemNode(nullptr),
	  m_tileMode(TerrainNode::TileMode::RectFlat)
{
	this->setFlag(ItemHasContents, true);
}

void GameWorldItem::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
	this->m_geometryChanged = true;
	this->update();
	QQuickItem::geometryChanged(newGeometry, oldGeometry);
	this->setAcceptedMouseButtons(Qt::AllButtons);
}

QSGNode* GameWorldItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
{
	GameWorldItemNode* gameWorldItemNode = static_cast<GameWorldItemNode*>(oldNode);

	QRectF rect = boundingRect();

	if (rect.isEmpty()) {
		delete gameWorldItemNode->m_background;
		delete gameWorldItemNode->m_terrain;
		delete gameWorldItemNode;
		return nullptr;
	}

	if (!gameWorldItemNode || m_newGame) {
		if (m_newGame) {
			delete gameWorldItemNode->m_background;
			delete gameWorldItemNode->m_terrain;
			delete gameWorldItemNode;
		}
		gameWorldItemNode = new GameWorldItemNode();

		gameWorldItemNode->m_background = new BackgroundNode(window());
		gameWorldItemNode->m_terrain = new TerrainNode(window(), this->m_game->currentMap());

		gameWorldItemNode->appendChildNode(gameWorldItemNode->m_background);
		gameWorldItemNode->appendChildNode(gameWorldItemNode->m_terrain);
		this->m_newGame = false;
	}

	if (m_geometryChanged) {
		gameWorldItemNode->m_terrain->setRect(rect);
		gameWorldItemNode->m_background->setRect(rect);
	}

	m_geometryChanged = false;

	this->m_gameWorldItemNode = gameWorldItemNode;
	return gameWorldItemNode;
}

void GameWorldItem::mouseDoubleClickEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
//	qDebug() << event;
//	update();	// changing an attribute of the qquickitem and updating the scenegraph
}

void GameWorldItem::mouseMoveEvent(QMouseEvent* event)
{
//	qDebug() << event;
	if (this->m_gameWorldItemNode != nullptr && (event->buttons() & Qt::RightButton) ) {
		if (this->m_gameWorldItemNode->m_terrain != nullptr) {
			this->m_gameWorldItemNode->m_terrain->addOffset(event->screenPos() - this->m_mouseDownPos);
			this->m_mouseDownPos = event->screenPos();
			this->m_geometryChanged = true;
			this->update();	// changing an attribute of the qquickitem and updating the scenegraph
		}
	}
}

void GameWorldItem::mousePressEvent(QMouseEvent* event)
{
//	qDebug() << event;
	this->m_mouseDownPos = event->screenPos();
//	update();	// changing an attribute of the qquickitem and updating the scenegraph
}

void GameWorldItem::mouseReleaseEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
//	qDebug() << event;
	this->m_mouseDownPos = QPointF();
//	update();	// changing an attribute of the qquickitem and updating the scenegraph
}

void GameWorldItem::setTileMode(int tileMode)
{
	TerrainNode::TileMode newTileMode = static_cast<TerrainNode::TileMode>(tileMode);
	if (this->m_tileMode == newTileMode)
		return;

	this->m_tileMode = newTileMode;
	emit tileModeChanged(this->m_tileMode);

	if (this->m_gameWorldItemNode != nullptr) {
		if (this->m_gameWorldItemNode->m_terrain != nullptr) {
			this->m_gameWorldItemNode->m_terrain->setTileMode(this->m_tileMode);
			this->m_geometryChanged = true;
			this->update();
		}
	}
}

Game* GameWorldItem::game() const
{
	return this->m_game;
}

void GameWorldItem::setGame(Game* game)
{
	if (this->m_game == game)
		return;

	auto oldGame = this->m_game;

	this->m_game = game;
	emit gameChanged(this->m_game);

	if (this->game() != nullptr) {
		QObject::connect(this->m_game, &Game::starteNewGame,
						 this, &GameWorldItem::startedNewGame);
	}
	if (oldGame != nullptr) {
		QObject::disconnect(oldGame, nullptr, this, nullptr);
	}
}

void GameWorldItem::startedNewGame()
{
	this->m_newGame = true;
	this->m_geometryChanged = true;
	this->update();
}

TerrainNode::TileMode GameWorldItem::tileMode() const
{
	return this->m_tileMode;
}

}	/// namespace slagavallen

}	/// namespace LFD
