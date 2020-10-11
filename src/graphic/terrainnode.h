//
//  terrainnode.h
//  slagavallen
//
//  Created by philipp on 27.09.2020.
//  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
//
#pragma once

#include <QtQuick/QSGOpaqueTextureMaterial>
#include <QtQuick/QSGGeometryNode>
#include <QQuickWindow>

namespace LFD {

namespace slagavallen {

class TerrainNode : public QSGGeometryNode {
public:
	TerrainNode(QQuickWindow* window);

	void setRect(const QRectF& rect);

protected:
	QSGOpaqueTextureMaterial m_material;
	QSGGeometry* m_geometry;
};

}

}
