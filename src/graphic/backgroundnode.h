//
//  backgroundnode.h
//  slagavallen
//
//  Created by philipp on 28.09.2020.
//  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
//
#pragma once

#include <QtQuick/QSGFlatColorMaterial>
#include <QtQuick/QSGGeometryNode>
#include <QQuickWindow>

namespace LFD {

namespace slagavallen {

class BackgroundNode : public QSGGeometryNode {
public:
	BackgroundNode(QQuickWindow* window);
	void setRect(const QRectF& rect);

protected:
//	QSGFlatColorMaterial m_material;
//	QSGGeometry m_geometry;
};

}

}
