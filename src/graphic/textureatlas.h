/**
 *  textureatlas.h
 *  Slagavallen
 *
 *  Created by philipp on 02.11.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
 */
#pragma once

#include <QMap>
#include <QPointF>
#include <QVector>

#include "src/logic/tile.h"

class QJsonArray;

namespace LFD {

namespace slagavallen {

class TextureAtlas {
public:
	enum class Axis {
		Horizontal,
		Vertical
	};

	enum class SaveFormat {
		Json,
		Binary
	};

	explicit TextureAtlas();
	explicit TextureAtlas(const QString uri, SaveFormat saveFormat);

	double textureCoordinates(std::shared_ptr<Tile> tile, Axis axis, unsigned int corner) const;
	const QString textureUri() const;

	void read(const QJsonObject& json);
	void write(QJsonObject& json) const;

protected:
	double rotatedTextureCoordinates(
	  QVector<double> textureCoordinates, Tile::Rotation rotation, Axis axis, unsigned int corner) const;
	double coordinate(QVector<double> textureCoordinates, Axis axis, unsigned int corner) const;

	QVector<double> readVec4(QJsonArray& coordinatesArray);

	QMap<Tile::TerrainType, QVector<double>> m_textureCoordinates;
//	QMap<unsigned int, QVector<double>> m_textureCoordinates;

	QString m_filename;
	QString m_version;
};

}	/// namespace slagavallen

}	/// namespace LFD
