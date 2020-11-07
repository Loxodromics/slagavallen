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

namespace LFD {

namespace slagavallen {

class TerrainNode;
class BackgroundNode;
class GameWorldItemNode;

class GameWorldItem : public QQuickItem {
	Q_OBJECT
	QML_ELEMENT
public:
	GameWorldItem();

	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

protected:
	QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*) override;
	void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) override;

public slots:

protected:
	bool m_geometryChanged;
	QPointF m_mouseDownPos;
	GameWorldItemNode* m_gameWorldItemNode;
};

class GameWorldItemNode : public QSGNode {
public:
	TerrainNode* m_terrain;
	BackgroundNode* m_background;
};

}

}
