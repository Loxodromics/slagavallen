//
//  backgroundnode.cpp
//  slagavallen
//
//  Created by philipp on 28.09.2020.
//  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
//
#include "backgroundnode.h"

#include <QtCore/QRandomGenerator>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QSGSimpleMaterialShader>
#include <QtQuick/QSGTexture>

#define NOISE_SIZE 64

namespace LFD {

namespace slagavallen {

struct BackgroundMaterial {
	~BackgroundMaterial() { delete texture; }

	QColor color;
	QSGTexture* texture;
};

class BackgroundShader : public QSGSimpleMaterialShader<BackgroundMaterial> {
	static QSGMaterialShader* createShader() { return new BackgroundShader; }

public:
	static QSGSimpleMaterial<BackgroundMaterial>* createMaterial()
	{
		return new QSGSimpleMaterial<BackgroundMaterial>(createShader);
	}

public:
	BackgroundShader()
	{
		setShaderSourceFile(QOpenGLShader::Vertex, ":/resources/shader/background.vsh");
		setShaderSourceFile(QOpenGLShader::Fragment, ":/resources/shader/background.fsh");
	}

	QList<QByteArray> attributes() const override
	{
		return QList<QByteArray>() << "aVertex"
								   << "aTexCoord";
	}

	void updateState(const BackgroundMaterial* m, const BackgroundMaterial*) override
	{

		// Set the color
		program()->setUniformValue(id_color, m->color);

		// Bind the texture and set program to use texture unit 0 (the default)
		m->texture->bind();

		// Then set the texture size so we can adjust the texture coordinates accordingly in the
		// vertex shader..
		QSize s = m->texture->textureSize();
		program()->setUniformValue(id_textureSize, QSizeF(1.0 / s.width(), 1.0 / s.height()));
	}

	void resolveUniforms() override
	{
		id_texture = program()->uniformLocation("texture");
		id_textureSize = program()->uniformLocation("textureSize");
		id_color = program()->uniformLocation("color");

		// We will only use texture unit 0, so set it only once.
		program()->setUniformValue(id_texture, 0);
	}

private:
	int id_color = -1;
	int id_texture = -1;
	int id_textureSize = -1;
};

BackgroundNode::BackgroundNode(QQuickWindow* window)
{
	// Make some noise...
	QImage image(NOISE_SIZE, NOISE_SIZE, QImage::Format_RGB32);
	uint* data = (uint*)image.bits();
	for (int i = 0; i < NOISE_SIZE * NOISE_SIZE; ++i) {
		uint g = QRandomGenerator::global()->bounded(0xff);
		data[i] = 0xff000000 | (g << 16) | (g << 8) | g;
	}

	QSGTexture* t = window->createTextureFromImage(image);
	t->setFiltering(QSGTexture::Nearest);
	t->setHorizontalWrapMode(QSGTexture::Repeat);
	t->setVerticalWrapMode(QSGTexture::Repeat);

	QSGSimpleMaterial<BackgroundMaterial>* m = BackgroundShader::createMaterial();
	m->state()->texture = t;
	m->state()->color = QColor::fromRgbF(0.0, 0.46, 0.75);
	m->setFlag(QSGMaterial::Blending);

	setMaterial(m);
	setFlag(OwnsMaterial, true);

	QSGGeometry* g = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4);
	QSGGeometry::updateTexturedRectGeometry(g, QRect(), QRect());
	setGeometry(g);
	setFlag(OwnsGeometry, true);
}

void BackgroundNode::setRect(const QRectF& bounds)
{
	QSGGeometry::updateTexturedRectGeometry(geometry(), bounds, QRectF(0, 0, 1, 1));
	markDirty(QSGNode::DirtyGeometry);
}

}	// namespace slagavallen

}	// namespace LFD
