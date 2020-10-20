//
//  terrainnode.cpp
//  slagavallen
//
//  Created by philipp on 27.09.2020.
//  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
//
#include "terrainnode.h"

#include "qmath.h"

#include <QtQuick/QSGTexture>

#define GRID_SIZE 32

namespace LFD {

namespace slagavallen {

TerrainNode::TerrainNode(QQuickWindow* window)
	: m_geometry(nullptr)
	, m_offset(QPointF(0.0f, 0.0f))
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
}

void LFD::slagavallen::TerrainNode::drawTile(int i_v, int i_h, unsigned int vCount, unsigned hCount,
  QSGGeometry::TexturedPoint2D* vertices, float offsetX, float offsetY)
{
	float dx = (i_v * GRID_SIZE + ((int)offsetX % GRID_SIZE)) - GRID_SIZE;
	float dy = (i_h * GRID_SIZE + ((int)offsetY % GRID_SIZE)) - GRID_SIZE;

	Tile tile;
	tile.terrainType = TerrainType::Grass2;
	QRectF textureCoordinates = this->textureCoordinates(tile);
	int i = (i_v * vCount + i_h);
	vertices[i * 4].set(dx, dy, textureCoordinates.x(), textureCoordinates.y());
	vertices[i * 4 + 1].set(dx + GRID_SIZE, dy, textureCoordinates.x() + textureCoordinates.width(), textureCoordinates.y());
	vertices[i * 4 + 2].set(dx + GRID_SIZE, dy + GRID_SIZE, textureCoordinates.x() + textureCoordinates.width(), textureCoordinates.y() + textureCoordinates.height());
	vertices[i * 4 + 3].set(dx, dy + GRID_SIZE,textureCoordinates.x(), textureCoordinates.y() + textureCoordinates.height());
}

QRectF TerrainNode::textureCoordinates(Tile& tile)
{
	switch (tile.terrainType) {
	case TerrainType::Grass0:
		return QRectF(0.0f, 0.0f, 0.5f, 0.5f);
		break;
	case TerrainType::Grass1:
		return QRectF(0.5f, 0.0, 0.5f, 0.5f);
		break;
	case TerrainType::Grass2:
		return QRectF(0.0f, 0.5f, 0.5f, 0.5f);
		break;
	case TerrainType::Grass3:
		return QRectF(0.5f, 0.5f, 0.5f, 0.5f);
		break;
	}
}

void TerrainNode::setRect(const QRectF& rect)
{
	/// + 2 for adding a tile on both ends
	int vCount = int((rect.width() - 1) / GRID_SIZE) + 2;
	int hCount = int((rect.height() - 1) / GRID_SIZE) + 2;

	int tilesCount = (GRID_SIZE + 1) * (GRID_SIZE + 1) * 4;
	this->m_geometry->allocate(tilesCount);

	QSGGeometry::TexturedPoint2D* vertices = m_geometry->vertexDataAsTexturedPoint2D();

	for (int i_v = 0; i_v <= vCount; ++i_v) {
		for (int i_h = 0; i_h <= hCount; ++i_h) {
			this->drawTile(i_v, i_h, vCount, hCount, vertices, this->m_offset.x(), this->m_offset.y());
		}
	}

	// Tell the scenegraph we updated the geometry...
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
