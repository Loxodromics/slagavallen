//
//  backgroundnode.cpp
//  slagavallen
//
//  Created by philipp on 28.09.2020.
//  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
//
#include "backgroundnode.h"

namespace LFD {

namespace slagavallen {

BackgroundNode::BackgroundNode()
	: m_geometry(QSGGeometry::defaultAttributes_Point2D(), 0)
{
	setGeometry(&m_geometry);
	m_geometry.setDrawingMode(GL_QUADS);

	setMaterial(&m_material);
	m_material.setColor(Qt::darkBlue);
}

void BackgroundNode::setRect(const QRectF& rect)
{
	QSGGeometry* g = geometry();

	g->allocate(4);

	float x = rect.x();
	float y = rect.y();
	float w = rect.width();
	float h = rect.height();

	QSGGeometry::Point2D* v = g->vertexDataAsPoint2D();
	v[0].set(x, y);
	v[1].set(w, y);
	v[2].set(w, h);
	v[3].set(x, h);

	// Tell the scenegraph we updated the geometry..
	markDirty(QSGNode::DirtyGeometry);
}

}	// namespace slagavallen

}	// namespace LFD
