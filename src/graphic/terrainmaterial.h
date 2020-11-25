/**
 *  terrainmaterial.h
 *  Slagavallen
 *
 *  Created by philipp on 24.11.2020.
 *  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
 */
#pragma once

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

}	// namespace slagavallen

}	// namespace LFD
