/**
 *  terrainnode.cpp
 *  Slagavallen
 *
 *  Created by philipp on 27.09.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
 */
#include "terrainnode.h"
#include "src/logic/hex.h"

#include "qmath.h"
#include <QtCore/QRandomGenerator>
#include <QtQuick/QSGTexture>

namespace LFD {

namespace slagavallen {

TerrainNode::TerrainNode(QQuickWindow* window, Game* game)
	: m_material()
	, m_geometry(nullptr)
	, m_offset(QPointF(0.0f, 0.0f))
	, m_textureAtlas()
	, m_game(game)
{
	Q_ASSERT(game);

	if ((game->tileMode() == Game::TileMode::RectFlat) || (game->tileMode() == Game::TileMode::RectIso)) {
		this->m_geometry = new TerrainGeometry(4);
		this->m_geometry->setDrawingMode(GL_QUADS);
	} else if ((game->tileMode() == Game::TileMode::HexFlat) || (game->tileMode() == Game::TileMode::HexIso)) {
		this->m_geometry = new TerrainGeometry(8);
		this->m_geometry->setDrawingMode(QSGGeometry::DrawTriangles);
	}
	this->setGeometry(m_geometry);
	this->setFlag(OwnsGeometry, true);

//	this->m_textureAtlas.createAtlas(16, 128, 224, "overworld", TextureAtlas::SaveFormat::Json);
//	this->m_textureAtlas.createAtlas(64, 128, 128, "grass", TextureAtlas::SaveFormat::Json);
	this->m_textureAtlas = TextureAtlas(QStringLiteral(":/resources/atlases/overworld.json"), TextureAtlas::SaveFormat::Json);

	QImage textureImage;
	textureImage.load(m_textureAtlas.textureUri());
	QSGTexture* texture = window->createTextureFromImage(textureImage);
	texture->setFiltering(QSGTexture::Nearest);
	texture->setHorizontalWrapMode(QSGTexture::Repeat);
	texture->setVerticalWrapMode(QSGTexture::Repeat);

	TerrainMaterial* terrainMaterial = new TerrainMaterial;
	terrainMaterial->setTexture(texture);

	terrainMaterial->setFlag(QSGMaterial::Blending);

	this->setMaterial(terrainMaterial);
	this->setFlag(OwnsMaterial, true);
}

void TerrainNode::drawTile(
  unsigned int i_h, unsigned int i_v, TerrainGeometry::TexturedNormalPoint2D* vertices, double offsetX, double offsetY)
{
	auto currentTile = this->m_game->currentMap()->getTile(i_v, i_h);
	switch (this->m_game->tileMode()) {
	case Game::TileMode::RectFlat:
		this->drawTileRectFlat(currentTile, i_h, i_v, vertices, offsetX, offsetY);
		break;
	case Game::TileMode::RectIso:
		this->drawTileRectIso(currentTile, i_h, i_v, vertices, offsetX, offsetY);
		break;
	case Game::TileMode::HexFlat:
		this->drawTileHexFlat(currentTile, i_h, i_v, vertices, offsetX, offsetY);
		break;
	case Game::TileMode::HexIso:
		break;
	default:
		break;
	}
}

void TerrainNode::drawTileRectFlat(std::shared_ptr<LFD::slagavallen::Tile> tile, unsigned int i_h, unsigned int i_v,
  TerrainGeometry::TexturedNormalPoint2D* vertices, double offsetX, double offsetY)
{
	Q_UNUSED(tile);

	double dh = (static_cast<double>(i_h) * this->m_game->currentMap()->tileSize() + offsetX);
	double dv = (static_cast<double>(i_v) * this->m_game->currentMap()->tileSize() + offsetY);

	QVector3D normal{0.0f, 0.0f, -1.0f};

	unsigned int index = i_h + this->m_game->currentMap()->width() * i_v;
	if (index < this->m_game->currentMap()->size()) {
		int i = (i_h * this->m_game->currentMap()->width() + i_v);
		vertices[i * 4].set(dh, dv,
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 0),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 0),
		  &normal);
		vertices[i * 4 + 1].set(dh + this->m_game->currentMap()->tileSize(), dv,
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 1),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 1),
		  &normal);
		vertices[i * 4 + 2].set(dh + this->m_game->currentMap()->tileSize(), dv + this->m_game->currentMap()->tileSize(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 2),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 2),
		  &normal);
		vertices[i * 4 + 3].set(dh, dv + this->m_game->currentMap()->tileSize(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 3),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 3),
		  &normal);
	}
}

void TerrainNode::drawTileRectIso(std::shared_ptr<Tile> tile, unsigned int i_h, unsigned int i_v,
  TerrainGeometry::TexturedNormalPoint2D* vertices, double offsetX, double offsetY)
{
	double dh = (-static_cast<double>(i_v) * this->m_game->currentMap()->tileSize() + (i_h * this->m_game->currentMap()->tileSize()) + offsetX);
	double dv = ((0.5f * i_v * this->m_game->currentMap()->tileSize()) + (0.5f * i_h * this->m_game->currentMap()->tileSize()) + offsetY);
	double heightFactor = 48.0;

	unsigned int index = i_h + this->m_game->currentMap()->width() * i_v;
	if (index < this->m_game->currentMap()->size()) {
		int i = (i_v * this->m_game->currentMap()->width() + i_h);

		vertices[i * 4].set(dh, dv + heightFactor * tile->height[0],
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 0),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 0),
		  this->m_game->currentMap()->getTile(index)->normal[0]);
		vertices[i * 4 + 1].set(dh + this->m_game->currentMap()->tileSize(), dv + (0.5f * this->m_game->currentMap()->tileSize()) + heightFactor * tile->height[1],
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 1),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 1),
		  this->m_game->currentMap()->getTile(index)->normal[1]);
		vertices[i * 4 + 2].set(dh, dv + this->m_game->currentMap()->tileSize() + heightFactor * tile->height[2],
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 2),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 2),
		  this->m_game->currentMap()->getTile(index)->normal[2]);
		vertices[i * 4 + 3].set(dh - this->m_game->currentMap()->tileSize(), dv + (0.5f * this->m_game->currentMap()->tileSize()) + heightFactor * tile->height[3],
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 3),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 3),
		  this->m_game->currentMap()->getTile(index)->normal[3]);
	}
}

void TerrainNode::drawTileHexFlat(std::shared_ptr<Tile> tile, unsigned int i_v, unsigned int i_h, TerrainGeometry::TexturedNormalPoint2D* vertices, double offsetX, double offsetY)
{
	Hex hex(i_v, i_h, -i_v - i_h);
	Layout layout(layout_flat,
				  QPointF(this->m_game->currentMap()->tileSize(), this->m_game->currentMap()->tileSize()),
				  QPointF(offsetX + 1 * i_v, offsetY + 1 * i_h));
	std::vector<QPointF> corners = polygonCorners(layout, hex);
	QPointF center = hexToPixel(layout, hex);

//	qDebug() << "center: " << center;
//	qDebug() << "corners: " << corners;

	QVector3D normal{0.0f, 0.0f, -1.0f};

	unsigned int index = i_h + this->m_game->currentMap()->width() * i_v;

	if (index < this->m_game->currentMap()->size()) {
		int i = (i_h * this->m_game->currentMap()->width() + i_v);
//		qDebug() << "i" << i;
		vertices[i * 18].set(center.x(), center.y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 0),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 0),
		  &normal);
		vertices[i * 18 + 1].set(corners[0].x(), corners[0].y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 0),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 0),
		  &normal);
		vertices[i * 18 + 2].set(corners[1].x(), corners[1].y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 1),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 1),
		  this->m_game->currentMap()->getTile(index)->normal[1]);

		vertices[i * 18 + 3].set(center.x(), center.y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 0),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 0),
		  &normal);
		vertices[i * 18 + 4].set(corners[1].x(), corners[1].y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 0),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 0),
		  &normal);
		vertices[i * 18 + 5].set(corners[2].x(), corners[2].y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 1),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 1),
		  this->m_game->currentMap()->getTile(index)->normal[1]);

		vertices[i * 18 + 6].set(center.x(), center.y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 0),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 0),
		  &normal);
		vertices[i * 18 + 7].set(corners[2].x(), corners[2].y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 0),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 0),
		  &normal);
		vertices[i * 18 + 8].set(corners[3].x(), corners[3].y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 1),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 1),
		  this->m_game->currentMap()->getTile(index)->normal[1]);

		vertices[i * 18 + 9].set(center.x(), center.y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 0),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 0),
		  &normal);
		vertices[i * 18 + 10].set(corners[3].x(), corners[3].y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 0),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 0),
		  &normal);
		vertices[i * 18 + 11].set(corners[4].x(), corners[4].y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 1),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 1),
		  this->m_game->currentMap()->getTile(index)->normal[1]);

		vertices[i * 18 + 12].set(center.x(), center.y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 0),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 0),
		  &normal);
		vertices[i * 18 + 13].set(corners[4].x(), corners[4].y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 0),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 0),
		  &normal);
		vertices[i * 18 + 14].set(corners[5].x(), corners[5].y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 1),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 1),
		  this->m_game->currentMap()->getTile(index)->normal[1]);

		vertices[i * 18 + 15].set(center.x(), center.y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 0),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 0),
		  &normal);
		vertices[i * 18 + 16].set(corners[5].x(), corners[5].y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 0),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 0),
		  &normal);
		vertices[i * 18 + 17].set(corners[0].x(), corners[0].y(),
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 1),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 1),
		  this->m_game->currentMap()->getTile(index)->normal[1]);
	}
}

void TerrainNode::setRect(const QRectF& rect)
{
	Q_UNUSED(rect);

//	int tilesCount = (this->m_game->currentMap()->width() + 1) * (this->m_game->currentMap()->height() + 1) * 4;
	int tilesCount = this->m_game->currentMap()->width() * this->m_game->currentMap()->height();
	this->m_geometry->allocate(tilesCount * 18);

	TerrainGeometry::TexturedNormalPoint2D* vertices = m_geometry->vertexDataAsTexturedNormalPoint2D();

	/// draw the whole world, not trying to figure out what is visible and what not
	for (unsigned int i_v = 0; i_v < this->m_game->currentMap()->width(); ++i_v) {
		for (unsigned int i_h = 0; i_h < this->m_game->currentMap()->height(); ++i_h) {
			this->drawTile(i_v, i_h, vertices, this->m_offset.x(), this->m_offset.y());
		}
	}
//	qDebug() << "ac: " << this->m_geometry->attributeCount();
//	qDebug() << "ic: " << this->m_geometry->indexCount();

//	for (int i = 0; i < tilesCount * 18; ++i) {
//		qDebug() << "vert:" << vertices[i].x << vertices[i].y;
//	}

	/// Tell the scenegraph we updated the geometry...
	this->markDirty(QSGNode::DirtyGeometry);
//	qDebug() << "TerrainNode::setRect";
}

void TerrainNode::setOffset(const QPointF& offset)
{
	this->m_offset = offset;
//	qDebug() << "offset: " << offset;
}

void TerrainNode::addOffset(const QPointF& offset)
{
	this->m_offset += offset;
	//	qDebug() << "offset: " << offset;
}

}	/// namespace slagavallen

}	/// namespace LFD
