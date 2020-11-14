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

namespace LFD {

namespace slagavallen {

class BackgroundNode;
class GameWorldItemNode;

class GameWorldItem : public QQuickItem {
	Q_OBJECT
	QML_ELEMENT

public:
	Q_PROPERTY(TerrainNode::TileMode tileMode READ tileMode WRITE setTileMode NOTIFY tileModeChanged)

	GameWorldItem();

	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	TerrainNode::TileMode tileMode() const;

protected:
	QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*) override;
	void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) override;

public slots:
	void setTileMode(TerrainNode::TileMode tileMode);

signals:
	void tileModeChanged(TerrainNode::TileMode tileMode);

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
