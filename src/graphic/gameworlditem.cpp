//
//  gameworlditem.cpp
//  slagavallen
//
//  Created by philipp on 27.09.2020.
//  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
//

#include "gameworlditem.h"
#include "terrainnode.h"
#include "backgroundnode.h"

namespace LFD {

namespace slagavallen {

GameWorldItem::GameWorldItem()
	: m_geometryChanged(false)
{
	setFlag(ItemHasContents, true);
}

void GameWorldItem::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
	m_geometryChanged = true;
	update();
	QQuickItem::geometryChanged(newGeometry, oldGeometry);
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

		gameWorldItemNode->m_background = new BackgroundNode();
		gameWorldItemNode->m_terrain = new TerrainNode();

		gameWorldItemNode->appendChildNode(gameWorldItemNode->m_background);
		gameWorldItemNode->appendChildNode(gameWorldItemNode->m_terrain);
	}

	if (m_geometryChanged) {
		gameWorldItemNode->m_terrain->setRect(rect);
		gameWorldItemNode->m_background->setRect(rect);
	}

	m_geometryChanged = false;

	return gameWorldItemNode;
}

}

}
