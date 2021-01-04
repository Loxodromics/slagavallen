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
	: m_geometryChanged(false)
	, m_newGame(false)
	, m_mouseDownPos(0.0f, 0.0f)
	, m_gameWorldItemNode(nullptr)
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
		gameWorldItemNode->m_terrain = new TerrainNode(window(), this->m_game);

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
}

void GameWorldItem::mouseMoveEvent(QMouseEvent* event)
{
	if (this->m_gameWorldItemNode != nullptr && (event->buttons() & Qt::RightButton)) {
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
	this->m_mouseDownPos = event->screenPos();
}

void GameWorldItem::mouseReleaseEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	this->m_mouseDownPos = QPointF();
}

void GameWorldItem::keyPressEvent(QKeyEvent* event)
{
//	qDebug() << "keyPressEvent" << event;
	QPointF offset{0.0f, 0.0f};
	float diff = 20.0f;
	/// this can only be one key at a time it seems...
	if (event->key() == Qt::Key_Up)
		offset.setY(offset.y() - diff);
	if (event->key() == Qt::Key_Down)
		offset.setY(offset.y() + diff);
	if (event->key() == Qt::Key_Left)
		offset.setX(offset.x() - diff);
	if (event->key() == Qt::Key_Right)
		offset.setX(offset.x() + diff);

	if (this->m_gameWorldItemNode != nullptr) {
		if (this->m_gameWorldItemNode->m_terrain != nullptr) {
			this->m_gameWorldItemNode->m_terrain->addOffset(offset);
			this->m_geometryChanged = true;
			this->update();
		}
	}
}

void GameWorldItem::setTileMode(Game::TileMode tileMode)
{
	Q_UNUSED(tileMode)

	if (this->m_gameWorldItemNode != nullptr) {
		if (this->m_gameWorldItemNode->m_terrain != nullptr) {
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
		QObject::connect(this->m_game, &Game::starteNewGame, this, &GameWorldItem::startedNewGame);

		QObject::connect(this->m_game, &Game::tileModeChanged, this, &GameWorldItem::setTileMode);
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

Game::TileMode GameWorldItem::tileMode() const { return this->m_game->tileMode(); }

}	/// namespace slagavallen

}	/// namespace LFD
