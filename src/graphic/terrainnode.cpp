/**
 *  terrainnode.cpp
 *  Slagavallen
 *
 *  Created by philipp on 27.09.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
 */
#include "terrainnode.h"

#include "qmath.h"
#include <QtCore/QRandomGenerator>
#include <QtQuick/QSGTexture>

#define GRID_SIZE 32

namespace LFD {

namespace slagavallen {

TerrainNode::TerrainNode(QQuickWindow* window, Game* game)
	: m_material()
	, m_geometry(nullptr)
	, m_offset(QPointF(0.0f, 0.0f))
	, m_textureAtlas()
	, m_game(game)
{
	this->m_geometry = new TerrainGeometry(4);
	this->setGeometry(m_geometry);
	this->m_geometry->setDrawingMode(GL_QUADS);
	this->setFlag(OwnsGeometry, true);

	QImage textureImage;
	textureImage.load(":/resources/textures/grass.png");
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

	double dh = (static_cast<double>(i_h) * GRID_SIZE + offsetX);
	double dv = (static_cast<double>(i_v) * GRID_SIZE + offsetY);

	QVector3D normal{0.0f, 0.0f, -1.0f};

	unsigned int index = i_h + this->m_game->currentMap()->width() * i_v;
	if (index < this->m_game->currentMap()->size()) {
		int i = (i_h * this->m_game->currentMap()->width() + i_v);
		vertices[i * 4].set(dh, dv,
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 0),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 0),
		  &normal);
		vertices[i * 4 + 1].set(dh + GRID_SIZE, dv,
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 1),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 1),
		  &normal);
		vertices[i * 4 + 2].set(dh + GRID_SIZE, dv + GRID_SIZE,
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 2),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 2),
		  &normal);
		vertices[i * 4 + 3].set(dh, dv + GRID_SIZE,
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 3),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 3),
		  &normal);
	}
}

void TerrainNode::drawTileRectIso(std::shared_ptr<Tile> tile, unsigned int i_h, unsigned int i_v,
  TerrainGeometry::TexturedNormalPoint2D* vertices, double offsetX, double offsetY)
{
	double dh = (-static_cast<double>(i_v) * GRID_SIZE + (i_h * GRID_SIZE) + offsetX);
	double dv = ((0.5f * i_v * GRID_SIZE) + (0.5f * i_h * GRID_SIZE) + offsetY);
	double heightFactor = 48.0;

	unsigned int index = i_h + this->m_game->currentMap()->width() * i_v;
	if (index < this->m_game->currentMap()->size()) {
		int i = (i_v * this->m_game->currentMap()->width() + i_h);

		vertices[i * 4].set(dh, dv + heightFactor * tile->height[0],
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 0),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 0),
		  this->m_game->currentMap()->getTile(index)->normal[0]);
		vertices[i * 4 + 1].set(dh + GRID_SIZE, dv + (0.5f * GRID_SIZE) + heightFactor * tile->height[1],
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 1),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 1),
		  this->m_game->currentMap()->getTile(index)->normal[1]);
		vertices[i * 4 + 2].set(dh, dv + GRID_SIZE + heightFactor * tile->height[2],
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 2),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 2),
		  this->m_game->currentMap()->getTile(index)->normal[2]);
		vertices[i * 4 + 3].set(dh - GRID_SIZE, dv + (0.5f * GRID_SIZE) + heightFactor * tile->height[3],
		  this->m_textureAtlas.textureCoordinates(
			this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Horizontal, 3),
		  this->m_textureAtlas.textureCoordinates(this->m_game->currentMap()->getTile(index), TextureAtlas::Axis::Vertical, 3),
		  this->m_game->currentMap()->getTile(index)->normal[3]);
	}
}

void TerrainNode::setRect(const QRectF& rect)
{
	Q_UNUSED(rect);

	int tilesCount = (this->m_game->currentMap()->width() + 1) * (this->m_game->currentMap()->height() + 1) * 4;
	this->m_geometry->allocate(tilesCount);

	TerrainGeometry::TexturedNormalPoint2D* vertices = m_geometry->vertexDataAsTexturedNormalPoint2D();

	/// draw the whole world, not trying to figure out what is visible and what not
	for (unsigned int i_v = 0; i_v < this->m_game->currentMap()->width(); ++i_v) {
		for (unsigned int i_h = 0; i_h < this->m_game->currentMap()->height(); ++i_h) {
			this->drawTile(i_v, i_h, vertices, this->m_offset.x(), this->m_offset.y());
		}
	}

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

}	// namespace slagavallen

}	// namespace LFD
