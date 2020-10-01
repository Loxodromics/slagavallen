//
//  backgroundnode.h
//  slagavallen
//
//  Created by philipp on 28.09.2020.
//  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
//
#pragma once

#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>

namespace LFD {

namespace slagavallen {

class BackgroundNode : public QSGGeometryNode {
public:
	BackgroundNode();
	void setRect(const QRectF& rect);

private:
	QSGFlatColorMaterial m_material;
	QSGGeometry m_geometry;
};

}

}
