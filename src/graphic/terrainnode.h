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
#include <QtQuick/QSGSimpleMaterialShader>

#include "terrainmaterial.h"
#include "textureatlas.h"
#include "src/logic/map.h"
#include "src/game/game.h"

namespace LFD {

namespace slagavallen {

class TerrainNode : public QSGGeometryNode {
public:
	TerrainNode(QQuickWindow* window, Game* game);

	void setRect(const QRectF& rect);
	void setOffset(const QPointF& offset);
	void addOffset(const QPointF& offset);

protected:
	TerrainMaterial m_material;
	TerrainGeometry* m_geometry;
	QPointF m_offset;
	TextureAtlas m_textureAtlas;
	Game* m_game;

	void drawTile(
	  unsigned int i_v, unsigned int i_h, TerrainGeometry::TexturedNormalPoint2D* vertices, double offsetX, double offsetY);
	void drawTileRectFlat(std::shared_ptr<Tile> tile, unsigned int i_v, unsigned int i_h,
	  TerrainGeometry::TexturedNormalPoint2D* vertices, double offsetX, double offsetY);
	void drawTileRectIso(std::shared_ptr<Tile> tile, unsigned int i_v, int unsigned i_h,
	  TerrainGeometry::TexturedNormalPoint2D* vertices, double offsetX, double offsetY);
};

}	/// namespace slagavallen

}	/// namespace LFD
