//
//  gameworlditem.h
//  slagavallen
//
//  Created by philipp on 27.09.2020.
//  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
//
#pragma once

#include <QQuickItem>
#include <QtQuick/QSGGeometryNode>

namespace LFD {

namespace slagavallen {

class TerrainNode;
class BackgroundNode;

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

private:
	bool m_geometryChanged;
};

class GameWorldItemNode : public QSGNode {
public:
	TerrainNode* m_terrain;
	BackgroundNode* m_background;
};

}

}
