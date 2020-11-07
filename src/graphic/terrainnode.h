/**
 *  terrainnode.h
 *  Slagavallen
 *
 *  Created by philipp on 27.09.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
 */
#pragma once

#include <QQuickWindow>
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGOpaqueTextureMaterial>

#include "src/graphic/textureatlas.h"
#include "src/logic/tile.h"

namespace LFD {

namespace slagavallen {

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
	QVector<std::shared_ptr<Tile>> m_tiles;
	unsigned int m_worldSizeX;
	unsigned int m_worldSizeY;
	TextureAtlas m_textureAtlas;

	void drawTile(int i_v, int i_h, unsigned int vCount, unsigned hCount, QSGGeometry::TexturedPoint2D* vertices,
	  float offsetX, float offsetY);
};

}	// namespace slagavallen

}	// namespace LFD
