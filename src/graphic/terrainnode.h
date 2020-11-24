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

#include "src/graphic/textureatlas.h"
#include "src/logic/map.h"
#include "src/logic/mapgenerator.h"

namespace LFD {

namespace slagavallen {

struct TerrainMaterial {
	~TerrainMaterial();

	QSGTexture* texture;
};

class TerrainShader : public QSGSimpleMaterialShader<TerrainMaterial> {
	static QSGMaterialShader* createShader() { return new TerrainShader; }

public:
	explicit TerrainShader();
	static QSGSimpleMaterial<TerrainMaterial>* createMaterial();

	QList<QByteArray> attributes() const override;
	void updateState(const TerrainMaterial* m, const TerrainMaterial*) override;
	void resolveUniforms() override;

protected:
	int id_color = -1;
	int id_texture = -1;
	int id_textureSize = -1;
};

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
	TerrainMaterial m_material;
	QSGGeometry* m_geometry;
	QPointF m_offset;
	std::shared_ptr<Map> m_currentMap;
	TextureAtlas m_textureAtlas;
	TileMode m_tileMode;

	void drawTile(
	  unsigned int i_v, unsigned int i_h, QSGGeometry::TexturedPoint2D* vertices, double offsetX, double offsetY);
	void drawTileRectFlat(std::shared_ptr<Tile> tile, unsigned int i_v, unsigned int i_h,
	  QSGGeometry::TexturedPoint2D* vertices, double offsetX, double offsetY);
	void drawTileRectIso(std::shared_ptr<Tile> tile, unsigned int i_v, int unsigned i_h,
	  QSGGeometry::TexturedPoint2D* vertices, double offsetX, double offsetY);
};

}	// namespace slagavallen

}	// namespace LFD
