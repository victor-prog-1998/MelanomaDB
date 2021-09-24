import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "Properties.js" as Properties

RadioButton {
    id: control

    indicator: Rectangle {
        implicitWidth: Properties.radioButtonIndicatorSize
        implicitHeight: Properties.radioButtonIndicatorSize
        x: control.leftPadding
        //y: parent.height / 2 - height / 2
        anchors.verticalCenter: parent.verticalCenter
        radius: implicitWidth / 2
        border.color: point.color

        Rectangle {
            id: point
            width: parent.width / 2
            height: width
            anchors.centerIn: parent
            radius: width / 2
            color: control.down ? "#333333" : "black"
            visible: control.checked
        }
    }

    contentItem: Text {
        id: txt
        text: control.text
        font.family: Properties.fontFamily
        font.pointSize: Properties.fontPointSize
        opacity: enabled ? 1.0 : 0.3
        color: point.color
        verticalAlignment: Text.AlignVCenter
        leftPadding: control.indicator.width + control.spacing
    }
}
