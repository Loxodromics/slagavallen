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
#define TEXTURE_SIZE 64

namespace LFD {

namespace slagavallen {

TerrainNode::TerrainNode(QQuickWindow* window)
	: m_geometry(nullptr)
{
	this->m_geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4);
	this->setGeometry(m_geometry);
	m_geometry->setDrawingMode(GL_QUADS);
	setFlag(OwnsGeometry, true);

	setMaterial(&m_material);

	QImage image;
	image.load(":/resources/textures/grass.png");
	QSGTexture* texture = window->createTextureFromImage(image);
	texture->setFiltering(QSGTexture::Nearest);
	texture->setHorizontalWrapMode(QSGTexture::Repeat);
	texture->setVerticalWrapMode(QSGTexture::Repeat);

	this->m_material.setTexture(texture);
}

/*
 * The function hardcodes a fixed set of grid lines and scales
 * those to the bounding rect.
 */
void TerrainNode::setRect(const QRectF& rect)
{
	int vCount = int((rect.width() - 1) / GRID_SIZE);
	int hCount = int((rect.height() - 1) / GRID_SIZE);

	int tilesCount = (GRID_SIZE + 1) * (GRID_SIZE + 1) * 4;
	m_geometry->allocate(tilesCount);

	QSGGeometry::TexturedPoint2D* vertices = m_geometry->vertexDataAsTexturedPoint2D();

	for (int i_v = 0; i_v <= vCount; ++i_v) {
		for (int i_h = 0; i_h <= hCount; ++i_h) {
			float dx = i_v * GRID_SIZE;
			float dy = i_h * GRID_SIZE;
			int i = (i_v * vCount + i_h);
			vertices[i * 4].set(dx, dy, 0.0f, 0.0f);
			vertices[i * 4 + 1].set(dx + GRID_SIZE, dy, 1.0f, 0.0f);
			vertices[i * 4 + 2].set(dx + GRID_SIZE, dy + GRID_SIZE, 1.0f, 1.0f);
			vertices[i * 4 + 3].set(dx, dy + GRID_SIZE, 0.0f, 1.0f);
		}
	}

	// Tell the scenegraph we updated the geometry...
	markDirty(QSGNode::DirtyGeometry);
}

void TerrainNode::setOffset(const QPointF& offset)
{
	this->m_offset = offset;

	// Tell the scenegraph we updated the geometry..
	markDirty(QSGNode::DirtyGeometry);
}
}	// namespace slagavallen

}	// namespace LFD
