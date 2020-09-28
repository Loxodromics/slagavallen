import QtQuick 2.11
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
    }
}
