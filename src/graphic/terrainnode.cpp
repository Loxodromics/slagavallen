//
//  terrainnode.cpp
//  slagavallen
//
//  Created by philipp on 27.09.2020.
//  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
//
#include "terrainnode.h"

#include "qmath.h"

#define GRID_SIZE 32

namespace LFD {

namespace slagavallen {

TerrainNode::TerrainNode()
	: m_geometry(QSGGeometry::defaultAttributes_Point2D(), 0)
{
	setGeometry(&m_geometry);
	m_geometry.setDrawingMode(GL_QUADS);

	setMaterial(&m_material);
	m_material.setColor(Qt::gray);
}

/*
 * The function hardcodes a fixed set of grid lines and scales
 * those to the bounding rect.
 */
void TerrainNode::setRect(const QRectF& rect)
{
	int vCount = int((rect.width() - 1) / GRID_SIZE);
	int hCount = int((rect.height() - 1) / GRID_SIZE);

	int segmentsCount = (GRID_SIZE + 1) * (GRID_SIZE + 1)* 4;
	m_geometry.allocate(segmentsCount);
	QSGGeometry::Point2D *vertices = m_geometry.vertexDataAsPoint2D();


	for (int i_v = 0; i_v <= vCount; ++i_v) {
		for (int i_h = 0; i_h <= hCount; ++i_h) {
			float dx = i_v * GRID_SIZE;
			float dy = i_h * GRID_SIZE;
			int i = (i_v * vCount + i_h);
			vertices[i * 4].set(dx, dy);
			vertices[i * 4 + 1].set(dx + GRID_SIZE, dy);
			vertices[i * 4 + 2].set(dx + GRID_SIZE, dy + GRID_SIZE);
			vertices[i * 4 + 3].set(dx, dy + GRID_SIZE);
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

}

}
