import QtQuick 2.0
import QtQuick.Controls 2.2
import "Properties.js" as Properties

TabButton{
    id: root
    property color color: "#65ebf7"
    property color checkedColor: "#1929bd"
    property color hoverColor: "#b0f9ff"
    property alias buttonText: txt
    implicitHeight: txt.height + 10

    background: Rectangle{
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: parent.height
        color: parent.checked ? Properties.buttonSpecialColor : Properties.buttonColor;

        Text{
            id: txt
            anchors.centerIn: parent
            font.pointSize: 12
            font.family: Properties.fontFamily
            //font.bold: true
            color: root.checked ? Properties.buttonTextSpecialColor : Properties.buttonTextColor;
        }

        MouseArea{
            id: ma
            anchors.fill: parent
            onClicked: {
                if(!root.checked) root.toggle();
            }
        }
    }
}
