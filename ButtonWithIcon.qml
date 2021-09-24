import QtQuick 2.0
import QtQuick.Controls 2.2

Image{
    id: root
    property string toolTipText: ""
    signal clicked();
    scale: iconMa.containsMouse ? 1.1 : 1.0;
    // smooth: false
    opacity: enabled ? 1.0 : 0.5;
    MouseArea{
        id: iconMa
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked();
        ToolTip.text: root.toolTipText
        ToolTip.visible: ToolTip.text == "" ? false : containsMouse;
    }
}
