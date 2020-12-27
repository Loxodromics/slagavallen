/**
 *  main.cpp
 *  Slagavallen
 *
 *  Created by philipp on 27.09.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
 */
#include "src/game/game.h"
#include "graphic/gameworlditem.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char* argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;
	qmlRegisterType<LFD::slagavallen::GameWorldItem>("Slagavallen", 1, 0, "GameWorldItem");

	LFD::slagavallen::Game game;

	engine.rootContext()->setContextProperty( QStringLiteral("Game"),
											  &game );

	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(
	  &engine, &QQmlApplicationEngine::objectCreated, &app,
	  [url](QObject* obj, const QUrl& objUrl) {
		  if (!obj && url == objUrl)
			  QCoreApplication::exit(-1);
	  },
	  Qt::QueuedConnection);
	engine.load(url);

	return app.exec();
}
