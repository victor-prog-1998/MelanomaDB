import QtQuick 2.9
import QtQuick.Controls 2.2
import "Properties.js" as Properties

Button{
    opacity: enabled ? 1.0 : 0.5;
    id: root
    font.pointSize: 10
    property alias rectangle: rect
    property color color: Properties.buttonColor
    property color hoverColor: Properties.buttonSpecialColor
    property color textColor: Properties.buttonTextColor
    property color textHoverColor: Properties.buttonTextSpecialColor
    //property alias buttonText: txt
    implicitHeight: Properties.buttonHeight
    background: Rectangle{
        id: rect
        color: mouseArea.containsMouse ? parent.hoverColor : parent.color

        MouseArea{
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: root.clicked()
        }
    }

    contentItem: Text{
        id: txt
        text: root.text
        anchors.centerIn: parent
        font.family: Properties.fontFamily
        font.pointSize: Properties.fontPointSize
        //opacity: enabled ? 1.0 : 0.3
        color: mouseArea.containsMouse ? root.textHoverColor : root.textColor;
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

}
