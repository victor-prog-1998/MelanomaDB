import QtQuick 2.0
import "Properties.js" as Properties

ListView{
    // model: treeViewModel.data
    clip: true
    spacing: -1 // чтобы ширина разделителя была 1 пиксель
    delegate: Rectangle{
        visible: modelData.visible
        height: modelData.visible ? 24 : 1
        width: parent.width
        border.color: "lightgray"

//        Rectangle{ // визуальный разделитель шириной в 1 пиксель
//            anchors.left: parent.left
//            anchors.right: parent.right
//            anchors.bottom: parent.bottom
//            height: 1
//            color: "lightgray"
//        }

        Row{
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 10 + 20 * modelData.level
            spacing: 5
            ButtonWithIcon{
                visible: modelData.expandable
                source: modelData.open ? "qrc:/icons/minus-icon.png" : "qrc:/icons/plus-icon.png"
                anchors.verticalCenter: parent.verticalCenter
                onClicked: modelData.open = !modelData.open
            }

            CustomCheckBox{ // отображается у всех неразворачиваемых элементов, а также у разворачивамых,
                            // которые являются tristate
                visible: !modelData.expandable || modelData.tristate // если элемент tristate, то он однозначно expandable
                text: modelData.text
                anchors.verticalCenter: parent.verticalCenter
                checkState: modelData.checkState
                onClicked: modelData.checkState = checkState
            }

            Text{   // отображается у развовачиваемых элементов, не являющихся tristate
                visible: modelData.expandable && !modelData.tristate
                font.pointSize: Properties.fontPointSize
                font.family: Properties.fontFamily
                text: modelData.text

            }
        }


    }
}
