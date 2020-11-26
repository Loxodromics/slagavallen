/**
 *  terrainmaterial.h
 *  Slagavallen
 *
 *  Created by philipp on 24.11.2020.
 *  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
 */
#pragma once

#include <QSGGeometry>
#include <QSGMaterial>
#include <QSGTexture>

namespace LFD {

namespace slagavallen {

class TerrainMaterialShader : public QSGMaterialShader {
public:
	explicit TerrainMaterialShader();
	void updateState(const RenderState& state, QSGMaterial* newMaterial, QSGMaterial* oldMaterial) override;
	const char* const* attributeNames() const override;

protected:
	void initialize() override;
	int m_matrixId = -1;
	int m_opacityId = -1;
};

class TerrainMaterial : public QSGMaterial {
public:
	void setTexture(QSGTexture* texture);
	QSGTexture* texture() const;

	QSGMaterialType* type() const override;
	QSGMaterialShader* createShader() const override;

private:
	QSGTexture* m_texture = nullptr;
};

class TerrainGeometry : public QSGGeometry {
public:
	explicit TerrainGeometry(int vertexCount, int indexCount = 0, int indexType = UnsignedShortType);

	struct TexturedNormalPoint2D {
		float x, y;
		float tx, ty;
		float nx, ny;
		void set(float _x, float _y, float _tx, float _ty, float _nx, float _ny)
		{
			x = _x;
			y = _y;
			tx = _tx;
			ty = _ty;
			nx = _nx;
			ny = _ny;
		}
	};

	inline const TerrainGeometry::TexturedNormalPoint2D* vertexDataAsTexturedNormalPoint2D() const {
		return static_cast<const TexturedNormalPoint2D*>(QSGGeometry::vertexData());
	}

	inline TerrainGeometry::TexturedNormalPoint2D* vertexDataAsTexturedNormalPoint2D() {
		return static_cast<TexturedNormalPoint2D*>(QSGGeometry::vertexData());
	}

	const QSGGeometry::AttributeSet& defaultAttributes_TexturedNormalPoint2D();
};

}	// namespace slagavallen

}	// namespace LFD
