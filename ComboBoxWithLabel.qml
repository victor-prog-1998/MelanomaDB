import QtQuick 2.0
import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "Properties.js" as Properties


// По умолчанию значение в комбобоксе устанавливается как невыбранное (индекс = -1)

ColumnLayout{
//    spacing: 10
//    Layout.margins: 10

    // После кастомизации ComboBox стандартное свойство currentText
    // принимает неверные значения (исправить нельзя, т.к. read only)

    property string currentText: (comboBox.count > 0 && comboBox.currentIndex >= 0) ?
                                     comboBox.model[comboBox.currentIndex] : "";

    //signal currentTextChanged(var currentText);

    //onCurrentTextChanged: console.log(currentText)

    function reset()
    {
        comboBox.currentIndex = -1;
    }

    function isSelected()
    {
        return comboBox.currentIndex != -1;
    }


    Layout.fillWidth: true
    Layout.fillHeight: true
    property alias label: label
    property alias comboBox: comboBox
    opacity: enabled ? 1.0 : 0.5;

    Label{
        id: label
        font.pointSize: Properties.fontPointSize
        font.family: Properties.fontFamily
        color: "black"
        Layout.fillWidth: true
    }
    ComboBox{

        id: comboBox
        currentIndex: -1
        //onCurrentIndexChanged: console.log(currentText + ", index: " + currentIndex);
        //onCurrentIndexChanged: console.log(model[currentIndex])
        Layout.minimumWidth: 80
        implicitHeight: Properties.comboBoxHeight
        Layout.fillWidth: true
        font.pointSize: Properties.fontPointSize
        font.family: Properties.fontFamily
        rightPadding: 6

        background: Rectangle{
            //radius: height / 2
        }

        indicator: Image{
            source: "qrc:/icons/combo-box-indicator.png"
            anchors.verticalCenter: parent.verticalCenter
            x: comboBox.width - width - comboBox.rightPadding
            rotation: comboBox.popup.visible ? 180 : 0;
        }

        contentItem: Text {
                leftPadding: 10
                rightPadding: comboBox.indicator.width + comboBox.spacing

                text: comboBox.displayText
                font: comboBox.font
                color: comboBox.pressed ? "#333333" : "black"
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

        delegate: ItemDelegate {
                width: comboBox.width
                implicitHeight: comboBox.implicitHeight
                contentItem: Text {
                    text: modelData
                    color: "black"
                    font: comboBox.font
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                }
                highlighted: comboBox.highlightedIndex === index
            }



        popup: Popup {
                y: comboBox.height - 1
                width: comboBox.width
                implicitHeight: contentItem.implicitHeight
                padding: 1

                contentItem: ListView {
                    clip: true
                    implicitHeight: contentHeight
                    model: comboBox.popup.visible ? comboBox.delegateModel : null
                    currentIndex: comboBox.highlightedIndex


                    ScrollIndicator.vertical: ScrollIndicator { }
                }

                background: Rectangle {
                    border.color: "black"
                    radius: 2
                }
            }
    }
}
