import QtQuick 2.0

Rectangle{
    width: 16
    height: width
    radius: width / 2
    border.width: 2
    color: mouseArea.containsMouse ? "lightgray" : "white";
    Drag.active: mouseArea.drag.active

    property alias mouseArea: mouseArea
    signal positionChanged()

    onXChanged: positionChanged();
    onYChanged: positionChanged();

    MouseArea{
        id: mouseArea
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        anchors.fill: parent
        drag.target: parent


    }
}
