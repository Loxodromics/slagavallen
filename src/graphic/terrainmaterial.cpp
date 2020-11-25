/**
 *  terrainmaterial.cpp
 *  Slagavallen
 *
 *  Created by philipp on 24.11.2020.
 *  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
 */
#include "terrainmaterial.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>

namespace LFD {

namespace slagavallen {

static QSGMaterialType materialType;

static inline bool isPowerOfTwo(int x)
{
	// Assumption: x >= 1
	return x == (x & -x);
}

TerrainMaterialShader::TerrainMaterialShader()
	: QSGMaterialShader()
{
	setShaderSourceFile(QOpenGLShader::Vertex, QStringLiteral(":/resources/shader/terrain.vsh"));
	setShaderSourceFile(QOpenGLShader::Fragment, QStringLiteral(":/resources/shader/terrain.fsh"));
}

char const* const* TerrainMaterialShader::attributeNames() const
{
	static char const* const attr[] = { "qt_VertexPosition", "qt_VertexTexCoord", 0 };
	return attr;
}

void TerrainMaterialShader::initialize()
{
	m_matrixId = program()->uniformLocation("qt_Matrix");
	m_opacityId = program()->uniformLocation("qt_Opacity");
}

void TerrainMaterialShader::updateState(const RenderState& state, QSGMaterial* newEffect, QSGMaterial* oldEffect)
{
	Q_ASSERT(oldEffect == 0 || newEffect->type() == oldEffect->type());

	TerrainMaterial* newMaterial = static_cast<TerrainMaterial*>(newEffect);
	TerrainMaterial* oldMaterial = static_cast<TerrainMaterial*>(oldEffect);

	QSGTexture* materialTexture = newMaterial->texture();
	if (materialTexture) {
		QOpenGLContext* ctx = const_cast<QOpenGLContext*>(state.context());
		if (!ctx->functions()->hasOpenGLFeature(QOpenGLFunctions::NPOTTextureRepeat)) {
			const QSize size = materialTexture->textureSize();
			const bool isNpot = !isPowerOfTwo(size.width()) || !isPowerOfTwo(size.height());
			if (isNpot) {
				materialTexture->setHorizontalWrapMode(QSGTexture::ClampToEdge);
				materialTexture->setVerticalWrapMode(QSGTexture::ClampToEdge);
			}
		}

		if (!oldMaterial || oldMaterial->texture()->textureId() != materialTexture->textureId())
			materialTexture->bind();
		else
			materialTexture->updateBindOptions();
	}

	if (state.isMatrixDirty())
		program()->setUniformValue(m_matrixId, state.combinedMatrix());

	if (state.isOpacityDirty())
		program()->setUniformValue(m_opacityId, state.opacity());
}

void TerrainMaterial::setTexture(QSGTexture* texture)
{
	this->m_texture = texture;
	setFlag(Blending, this->m_texture ? this->m_texture->hasAlphaChannel() : false);
}

QSGTexture* TerrainMaterial::texture() const {
	return this->m_texture;
}

QSGMaterialType* TerrainMaterial::type() const {
	return &materialType;
}

QSGMaterialShader* TerrainMaterial::createShader() const {
	return new TerrainMaterialShader;
}

}	// namespace slagavallen

}	// namespace LFD
