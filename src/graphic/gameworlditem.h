/**
 *  gameworlditem.h
 *  Slagavallen
 *
 *  Created by philipp on 27.09.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
 */
#pragma once

#include <QQuickItem>
#include <QtQuick/QSGGeometryNode>

#include "terrainnode.h"
#include "src/game/game.h"

namespace LFD {

namespace slagavallen {

class BackgroundNode;
class GameWorldItemNode;

class GameWorldItem : public QQuickItem {
	Q_OBJECT
	QML_ELEMENT

	Q_PROPERTY(Game* game READ game WRITE setGame NOTIFY gameChanged)

public:
	GameWorldItem();

	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	TerrainNode::TileMode tileMode() const;
	Q_INVOKABLE void setTileMode(int tileMode);

	Game* game() const;

public slots:
	void setGame(Game* game);

protected:
	QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*) override;
	void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) override;
	Game* m_game;

signals:
	void tileModeChanged(TerrainNode::TileMode tileMode);
	void gameChanged(Game* game);

protected:
	bool m_geometryChanged;
	QPointF m_mouseDownPos;
	GameWorldItemNode* m_gameWorldItemNode;
	TerrainNode::TileMode m_tileMode;
};

class GameWorldItemNode : public QSGNode {
public:
	TerrainNode* m_terrain;
	BackgroundNode* m_background;
};

}	/// namespace slagavallen

}	/// namespace LFD
