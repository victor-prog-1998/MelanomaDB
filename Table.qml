import QtQuick 2.9
import "Properties.js" as Properties

Item {
    id: root
    property alias headerModel: headerRepeater.model
    property alias tableModel: listView.model
    signal openButtonClicked(var row);
    signal deleteButtonClicked(var row);

    Item{  // шапка таблицы
        property int minimumColumnWidth: 20
        id: panel
        height: 30
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        Row{  // Колонки шапки
            id: headerRow
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left
            spacing: 2
            Repeater{
                id: headerRepeater
                delegate: Rectangle{
                    clip: true
                    width: (panel.width - headerRow.spacing * (headerRepeater.count - 1)) / headerRepeater.count
                    onXChanged: {
                        if(index == headerRepeater.count - 1)
                        {
                            width = panel.width - x;
                        }
                    }

                    height: panel.height
                    color: Properties.greenColor
                    //color: "darkmagenta"
                    Text{
                        anchors.centerIn: parent
                        color: "white"
                        text: modelData
                    }
                }
            }
        } // Row

        Repeater{ // Ресайзеры колонок
            model: headerRepeater.count - 1
            delegate: Rectangle{
                height: panel.height
                width: 20
//                color: "lime"
//                opacity: 0.5
                color: "transparent"
                x: headerRepeater.itemAt(index + 1).x - headerRow.spacing / 2 - width / 2;
                onXChanged: {
                    if(ma.drag.active)
                    {
                        var leftItem = headerRepeater.itemAt(index);

                        leftItem.width = x + width / 2 - headerRow.spacing / 2 - leftItem.x;
                    }
                }

                MouseArea{
                    id: ma
                    anchors.fill: parent
                    cursorShape: Qt.SplitHCursor
                    drag.target: parent
                    drag.axis: Drag.XAxis

                    drag.minimumX: {
                        var leftItem = headerRepeater.itemAt(index);
                        return leftItem.x + panel.minimumColumnWidth + headerRow.spacing / 2 - parent.width / 2;
                    }

                    drag.maximumX: panel.width - panel.minimumColumnWidth - headerRow.spacing / 2 - width / 2;


                }
            }
        }
    } // Panel

    ListView{ // строки таблицы
        id: listView
        clip: true
        anchors.top: panel.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.topMargin: 5

        antialiasing: true

        spacing: headerRow.spacing

        delegate: Component{
            Row{

                property int rowIndex: index
                id: tableRow
                spacing: headerRow.spacing
                Repeater{
                    id: tableRowRepeater
                    model: {
                        var m = [];
                        for(var i = 0; i < modelData.row.length; ++i)
                            m.push(modelData.row[i]);
                        return m;
                    }
                    delegate: Component{
                        Rectangle{
                            id: tableCellRect
                            height: 32
                            color: (tableRow.rowIndex % 2) ? Properties.ultraLightGreenColor : Properties.lightGreenColor;
                            // width: headerRepeater.count > 0 ? headerRepeater.itemAt(index).width : 100;
                            width: index < headerRepeater.count ? headerRepeater.itemAt(index).width : 100;
                            Text{
                                visible: modelData !== "<buttons>";
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                anchors.verticalCenter: parent.verticalCenter
                                text: modelData
                            }

                            Row{
                                visible: modelData == "<buttons>";
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                anchors.verticalCenter: parent.verticalCenter
                                spacing: 2
                                CustomButton{
                                    text: "Открыть"
                                    onClicked: root.openButtonClicked(tableRow.rowIndex);
                                }
                                CustomButton{
                                    text: "Удалить"
                                    onClicked: root.deleteButtonClicked(tableRow.rowIndex);
                                    color: Properties.buttonColor2
                                    hoverColor: Properties.buttonSpecialColor2
                                }
                            }


                        }
                    }
                }
            } // ListView delegate (Row)
        }
    }
}
