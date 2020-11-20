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
#include "src/logic/map.h"
#include "src/logic/mapgenerator.h"

namespace LFD {

namespace slagavallen {

class TerrainNode : public QSGGeometryNode {
public:
	enum class TileMode {
		RectFlat,
		RectIso,
		HexFlat,
		HexIso,
		NumTileMode,
	};

	TerrainNode(QQuickWindow* window);

	void setRect(const QRectF& rect);
	void setOffset(const QPointF& offset);
	void addOffset(const QPointF& offset);

	TileMode tileMode() const;
	void setTileMode(const TileMode& tileMode);

protected:
	QSGOpaqueTextureMaterial m_material;
	QSGGeometry* m_geometry;
	QPointF m_offset;
	std::shared_ptr<Map> m_currentMap;
	TextureAtlas m_textureAtlas;
	TileMode m_tileMode;

	void drawTile(
	  unsigned int i_v, unsigned int i_h, QSGGeometry::TexturedPoint2D* vertices, float offsetX, float offsetY);
	void drawTileRectFlat(std::shared_ptr<Tile> tile, unsigned int i_v, unsigned int i_h,
	  QSGGeometry::TexturedPoint2D* vertices, float offsetX, float offsetY);
	void drawTileRectIso(std::shared_ptr<Tile> tile, unsigned int i_v, int unsigned i_h,
	  QSGGeometry::TexturedPoint2D* vertices, float offsetX, float offsetY);
};

}	// namespace slagavallen

}	// namespace LFD
