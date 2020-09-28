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

class GameWorldItem : public QQuickItem {
	Q_OBJECT
	QML_ELEMENT
public:
	GameWorldItem();

protected:
	QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*);
	void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

public slots:

private:
	bool m_geometryChanged;
};

class GameWorldItemNode : public QSGNode {
public:
	TerrainNode* m_terrain;
};

}

}
