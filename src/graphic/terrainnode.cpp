/**
 *  terrainnode.cpp
 *  Slagavallen
 *
 *  Created by philipp on 27.09.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
 */
#include "terrainnode.h"
#include "textureatlas.h"

#include "qmath.h"
#include <QtCore/QRandomGenerator>
#include <QtQuick/QSGTexture>

#define GRID_SIZE 32

namespace LFD {

namespace slagavallen {

TerrainNode::TerrainNode(QQuickWindow* window)
	: m_geometry(nullptr)
	, m_offset(QPointF(0.0f, 0.0f))
	, m_worldSizeX(64)
	, m_worldSizeY(64)
{
	this->m_geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4);
	this->setGeometry(m_geometry);
	this->m_geometry->setDrawingMode(GL_QUADS);
	this->setFlag(OwnsGeometry, true);

	this->setMaterial(&m_material);

	QImage image;
	image.load(":/resources/textures/grass.png");
	QSGTexture* texture = window->createTextureFromImage(image);
	texture->setFiltering(QSGTexture::Nearest);
	texture->setHorizontalWrapMode(QSGTexture::Repeat);
	texture->setVerticalWrapMode(QSGTexture::Repeat);

	this->m_material.setTexture(texture);

	/// randomly fill the world with tiles
	for (unsigned int i = 0; i < this->m_worldSizeX * this->m_worldSizeY + 1; ++i) {
		auto tile = std::make_shared<Tile>();
		tile->terrainType = static_cast<Tile::TerrainType>(QRandomGenerator::global()->bounded(static_cast<int>(Tile::TerrainType::NumTerrainType) - 1) + 1);
		tile->id = i;
		tile->rotation = static_cast<Tile::Rotation>(QRandomGenerator::global()->bounded(static_cast<int>(Tile::Rotation::NumRotation)));
		this->m_tiles.push_back(tile);
	}
}

void LFD::slagavallen::TerrainNode::drawTile(int i_v, int i_h, unsigned int vCount, unsigned hCount,
  QSGGeometry::TexturedPoint2D* vertices, float offsetX, float offsetY)
{
//	float dx = (i_v * GRID_SIZE + ((int)offsetX % GRID_SIZE)) - GRID_SIZE;
//	float dy = (i_h * GRID_SIZE + ((int)offsetY % GRID_SIZE)) - GRID_SIZE;

	float dx = (i_v * GRID_SIZE + (int)offsetX) - GRID_SIZE;
	float dy = (i_h * GRID_SIZE + (int)offsetY) - GRID_SIZE;

	int index = i_v + this->m_worldSizeX * i_h;
	if (index < this->m_tiles.size()) {
		int i = (i_v * this->m_worldSizeX /*vCount*/ + i_h);
		vertices[i * 4].set(dx, dy,
		  this->m_textureAtlas.textureCoordinates(this->m_tiles[index], TextureAtlas::Axis::Horizontal, 0),
		  this->m_textureAtlas.textureCoordinates(this->m_tiles[index], TextureAtlas::Axis::Vertical, 0));
		vertices[i * 4 + 1].set(dx + GRID_SIZE, dy,
		  this->m_textureAtlas.textureCoordinates(this->m_tiles[index], TextureAtlas::Axis::Horizontal, 1),
		  this->m_textureAtlas.textureCoordinates(this->m_tiles[index], TextureAtlas::Axis::Vertical, 1));
		vertices[i * 4 + 2].set(dx + GRID_SIZE, dy + GRID_SIZE,
		  this->m_textureAtlas.textureCoordinates(this->m_tiles[index], TextureAtlas::Axis::Horizontal, 2),
		  this->m_textureAtlas.textureCoordinates(this->m_tiles[index], TextureAtlas::Axis::Vertical, 2));
		vertices[i * 4 + 3].set(dx, dy + GRID_SIZE,
		  this->m_textureAtlas.textureCoordinates(this->m_tiles[index], TextureAtlas::Axis::Horizontal, 3),
		  this->m_textureAtlas.textureCoordinates(this->m_tiles[index], TextureAtlas::Axis::Vertical, 3));
	}
}

void TerrainNode::setRect(const QRectF& rect)
{
	/// + 2 for adding a tile on both ends
	int vCount = int((rect.width() - 1) / GRID_SIZE) + 2;
	int hCount = int((rect.height() - 1) / GRID_SIZE) + 2;

	int tilesCount = (this->m_worldSizeX + 1) * (this->m_worldSizeY + 1) * 4;
	this->m_geometry->allocate(tilesCount);

	QSGGeometry::TexturedPoint2D* vertices = m_geometry->vertexDataAsTexturedPoint2D();

	/// first try: draw the whole world, lots of over draw...
	for (unsigned int i_v = 0; i_v <= this->m_worldSizeX /*vCount*/; ++i_v) {
		for (unsigned int i_h = 0; i_h <= this->m_worldSizeY /*hCount*/; ++i_h) {
			this->drawTile(i_v, i_h, vCount, hCount, vertices, this->m_offset.x(), this->m_offset.y());
		}
	}

	/// Tell the scenegraph we updated the geometry...
	this->markDirty(QSGNode::DirtyGeometry);
	qDebug() << "TerrainNode::setRect";
}

void TerrainNode::setOffset(const QPointF& offset)
{
	this->m_offset = offset;
	qDebug() << "offset: " << offset;
}

void TerrainNode::addOffset(const QPointF& offset)
{
	this->m_offset += offset;
	qDebug() << "offset: " << offset;
}

}	// namespace slagavallen

}	// namespace LFD
