/**
 *  main.qml
 *  Slagavallen
 *
 *  Created by philipp on 27.09.2020.
 *  Copyright (c) 2020 Philipp Engelhard.
 *  Licensed under GPL-3.0, see LICENSE file.
 */
import QtQuick 2.11
import QtQuick.Controls 2.11
import QtQuick.Window 2.11

import Slagavallen 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Slagavallen")

    GameWorldItem {
        id: gameWorldItem

        anchors.fill: parent

        game: Game
    }


    ComboBox {
        id: tileMode
        model: ["RectFlat", "RectIso", "HexFlat", "HexIso"]
        anchors.top: parent.top
        anchors.left: parent.left

        onActivated: {
            Game.setTileMode(index);
        }
    }

    Button {
        id: newGame

        anchors.top: tileMode.bottom
        anchors.left: parent.left

        text: "New Game"

        onClicked: {
            Game.newGame();
        }
    }
}
