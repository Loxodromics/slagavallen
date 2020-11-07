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
#include "terrainnode.h"

#include <QDebug>

namespace LFD {

namespace slagavallen {

GameWorldItem::GameWorldItem()
	: m_geometryChanged(false),
	  m_gameWorldItemNode(nullptr)
{
	setFlag(ItemHasContents, true);
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
		delete gameWorldItemNode;
		return nullptr;
	}

	if (!gameWorldItemNode) {
		gameWorldItemNode = new GameWorldItemNode();

		gameWorldItemNode->m_background = new BackgroundNode(window());
		gameWorldItemNode->m_terrain = new TerrainNode(window());

		gameWorldItemNode->appendChildNode(gameWorldItemNode->m_background);
		gameWorldItemNode->appendChildNode(gameWorldItemNode->m_terrain);
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
	qDebug() << event;
//	update();	// changing an attribute of the qquickitem and updating the scenegraph
}

void GameWorldItem::mouseMoveEvent(QMouseEvent* event)
{
	qDebug() << event;
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
	qDebug() << event;
	this->m_mouseDownPos = event->screenPos();
//	update();	// changing an attribute of the qquickitem and updating the scenegraph
}

void GameWorldItem::mouseReleaseEvent(QMouseEvent* event)
{
	qDebug() << event;
	this->m_mouseDownPos = QPointF();
	//	update();	// changing an attribute of the qquickitem and updating the scenegraph
}

}

}
