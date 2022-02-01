import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "Properties.js" as Properties



Item {




    id: root
    Rectangle{
        anchors.fill: parent

        color: "lightblue"

        ColumnLayout{
            id: controlLayout
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 10
            Layout.fillWidth: true
            spacing: 5
            CustomTextArea{
                id: textArea
                Layout.fillWidth: true
                height: 100
                area.font.family: "Courier new"
                area.font.pointSize: Properties.fontPointSize + 5
                area.font.bold: true
            }
            CustomButton{
                id: execButton
                text: "Выполнить запрос"
                onClicked: {
                    var query = textArea.area.text;
                    var result = dbWorker.execQuery(query);

                    if(!result.error)
                    {
                        queryTable.headerModel = result.tableHeader;
                        queryTable.tableModel = result.tableData;

                        messageBox.showInformationMessage("Информация", "Запрос выполнен");
                    }

                    else
                        messageBox.showCriticalMessage("Ошибка",
                                                       "Не удалось выполнить запрос.\nПроверьте корректность запроса.");

                }
            }

        }

        Table{
            id: queryTable
            anchors.top: controlLayout.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 10
        }


    }


    CustomMessageBox{
        id: messageBox
        visible: false
    }

}
