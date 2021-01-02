/**
 *  textureatlas.cpp
 *  Slagavallen
 *
 *  Created by philipp on 02.11.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
 */

#include "textureatlas.h"
#include <QCborMap>
#include <QCborValue>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

namespace LFD {

namespace slagavallen {

TextureAtlas::TextureAtlas()
{
	this->m_textureCoordinates.insert(Tile::TerrainType::None,  QVector<double> { 0.0f, 0.0f, 0.0001f, 0.0001f });
	this->m_textureCoordinates.insert(Tile::TerrainType::Grass0, QVector<double> { 0.0f, 0.0f, 0.5f, 0.5f });
	this->m_textureCoordinates.insert(Tile::TerrainType::Grass1, QVector<double> { 0.5f, 0.0f, 1.0f, 0.5f });
	this->m_textureCoordinates.insert(Tile::TerrainType::Grass2, QVector<double> { 0.0f, 0.5f, 0.5f, 1.0f });
	this->m_textureCoordinates.insert(Tile::TerrainType::Grass3, QVector<double> { 0.5f, 0.5f, 1.0f, 1.0f });
	this->m_filename = "grass.png";
}

TextureAtlas::TextureAtlas(const QString uri, SaveFormat saveFormat)
{
	QFile loadFile(uri);

	if (!loadFile.open(QIODevice::ReadOnly)) {
		qWarning("Couldn't open save file.");
		return;
	}

	QByteArray saveData = loadFile.readAll();

	QJsonDocument loadDoc(saveFormat == SaveFormat::Json
		? QJsonDocument::fromJson(saveData)
		: QJsonDocument(QCborValue::fromCbor(saveData).toMap().toJsonObject()));

	this->read(loadDoc.object());

//	QJsonObject json(uri);
//	this->read(json);
}

double TextureAtlas::textureCoordinates(std::shared_ptr<Tile> tile, Axis axis, unsigned int corner) const
{
	return TextureAtlas::rotatedTextureCoordinates(
				this->m_textureCoordinates[tile->terrainType], tile->rotation, axis, corner);
}

const QString TextureAtlas::textureUri() const
{
	QString uri = ":/resources/textures/";
	uri.append(this->m_filename);
	return uri;
}

void TextureAtlas::read(const QJsonObject& json)
{
	if (json.contains("version") && json["version"].isString()) {
		m_version = json["version"].toString();
		if (m_version != QStringLiteral("0.1"))
			return;
	}

	if (json.contains("texture_file") && json["texture_file"].isString())
		m_filename = json["texture_file"].toString();

	if (json.contains("coordinates") && json["coordinates"].isArray()) {
		QJsonArray coordinatesArray = json["coordinates"].toArray();
		m_textureCoordinates.clear();
		for (int coordinateIndex = 0; coordinateIndex < coordinatesArray.size(); ++coordinateIndex) {
			if (coordinatesArray[coordinateIndex].isArray()) {
				QJsonArray coordinateArray = coordinatesArray[coordinateIndex].toArray();
				QVector<double> coordinate = this->readVec4(coordinateArray);
				this->m_textureCoordinates.insert(static_cast<Tile::TerrainType>(coordinateIndex), coordinate);
			}
		}
	}
}

void TextureAtlas::write(QJsonObject& json) const
{

}

double TextureAtlas::rotatedTextureCoordinates(
  QVector<double> textureCoordinates, Tile::Rotation rotation, Axis axis, unsigned int corner) const
{
	/// % 4 corners
	int rotatedCorner = (corner + static_cast<int>(rotation)) % 4;
	return this->coordinate(textureCoordinates, axis, rotatedCorner);
}

double TextureAtlas::coordinate(QVector<double> textureCoordinates, Axis axis, unsigned int corner) const
{
	switch (corner) {
	case 0:
		/// Top Left
		if (axis == Axis::Horizontal)
			return textureCoordinates[0];
		else
			return textureCoordinates[1];
	case 1:
		/// Top Right
		if (axis == Axis::Horizontal)
			return textureCoordinates[2];
		else
			return textureCoordinates[1];
	case 2:
		/// Bottom Right
		if (axis == Axis::Horizontal)
			return textureCoordinates[2];
		else
			return textureCoordinates[3];
	case 3:
		/// Bottom Left
		if (axis == Axis::Horizontal)
			return textureCoordinates[0];
		else
			return textureCoordinates[3];

	default:
		return -1.0f;
	}
}

QVector<double> TextureAtlas::readVec4(QJsonArray& coordinatesArray)
{
	QVector<double> vec;
	for (int coordinateIndex = 0; coordinateIndex < coordinatesArray.size(); ++coordinateIndex) {
		if (coordinatesArray[coordinateIndex].isDouble()) {
			vec.append(coordinatesArray[coordinateIndex].toDouble());
		}
	}
	return vec;
}

}	/// namespace slagavallen

}	/// namespace LFD
