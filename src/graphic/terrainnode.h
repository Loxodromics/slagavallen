//
//  terrainnode.h
//  slagavallen
//
//  Created by philipp on 27.09.2020.
//  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
//
#pragma once

#include <QQuickWindow>
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGOpaqueTextureMaterial>

namespace LFD {

namespace slagavallen {

enum TerrainType {
	Grass0,
	Grass1,
	Grass2,
	Grass3,
};

struct Tile {
	unsigned int id;
	TerrainType terrainType;
};

class TerrainNode : public QSGGeometryNode {
public:
	TerrainNode(QQuickWindow* window);

	void setRect(const QRectF& rect);
	void setOffset(const QPointF& offset);
	void addOffset(const QPointF& offset);

protected:
	QSGOpaqueTextureMaterial m_material;
	QSGGeometry* m_geometry;
	QPointF m_offset;

	void drawTile(int i_v, int i_h, unsigned int vCount, unsigned hCount,
	  QSGGeometry::TexturedPoint2D* vertices, float offsetX, float offsetY);

	QRectF textureCoordinates(Tile& tile);
};

}

}
