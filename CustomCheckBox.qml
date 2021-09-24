import QtQuick 2.9
import QtQuick.Controls 2.2
import "Properties.js" as Properties

CheckBox{
    id: root
    font.family: Properties.fontFamily
    font.pointSize: Properties.fontPointSize
    leftPadding: 0 // отступ слева, у CheckBox по умолчанию он != 0 и индикатор немного сдвинут вправо

    indicator: Rectangle {
            id: indicatorRect
            implicitWidth: Properties.checkBoxIndicatorSize
            implicitHeight: implicitWidth
            x: root.leftPadding
            //y: parent.height / 2 - height / 2
            anchors.verticalCenter: parent.verticalCenter
            //radius: 3
            border.color: root.down ? "#333333" : "black"

            Rectangle {
                visible: root.checkState === Qt.PartiallyChecked
                anchors.fill: parent
                anchors.margins: 2
                width: parent.implicitWidth / 2
                height: width
                radius: 2
                color: "#555555"
            }

            Image{
                anchors.centerIn: parent
                source: "qrc:/icons/check-icon.png"
                visible: root.checkState === Qt.Checked
            }



        }

    contentItem: Text {
            text: root.text
            font: root.font
            opacity: enabled ? 1.0 : 0.3
            color: indicatorRect.border.color
            verticalAlignment: Text.AlignVCenter
            leftPadding: root.indicator.width + root.spacing
        }
}
