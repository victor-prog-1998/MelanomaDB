import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import "Properties.js" as Properties
import "Functions.js" as Functions

Item {

    id: root
    function showResults()
    {
        var model = dbWorker.findImagesByFilter(searchTypeComboBox.currentText === "Поиск \"И\"");
        table.tableModel = model;
        var count = model.length;
        var totalCount = dbWorker.imagesCount();
        var percent = count * 100 / totalCount;
        if ((percent ^ 0) !== percent) // если не целое
            percent = percent.toFixed(1);

        countLabel.text = "Показано изображений: " + count + " из " + totalCount + " (" + percent + " %)";
    }

    onVisibleChanged:
    {
        if(visible)
        {
            dbWorker.initTreeViewModel();
            filterTreeView.model = treeViewModel.data;
            root.showResults();
        }

    }

    Rectangle{
        id: filterRect
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        color: "lightblue"
        width: 300


        ColumnLayout{
            anchors.fill: parent
            anchors.margins: 10
            Layout.fillWidth: true
            FilterTreeView{
                id: filterTreeView
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            ComboBoxWithLabel{
                id: searchTypeComboBox
                label.text: "Тип поиска"
                comboBox.model: ["Поиск \"ИЛИ\"","Поиск \"И\""]
                comboBox.currentIndex: 0
            }

            RowLayout{
                id: buttonsLayout

                CustomButton{
                    Layout.fillWidth: true
                    text: "Cброс"
                    color: Properties.buttonColor2
                    hoverColor: Properties.buttonSpecialColor2
                    onClicked: treeViewModel.reset();
                }
                CustomButton{
                    Layout.fillWidth: true
                    text: "Результаты"
                    onClicked: root.showResults();
                }
            }

        }
    }

    Rectangle{
        anchors.top: parent.top
        anchors.left: filterRect.right
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        Label{
            id: countLabel
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 10
            font.family: Properties.fontFamily
            font.pointSize: Properties.fontPointSize
        }

        Table{
            id: table
            anchors.top: countLabel.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.topMargin: 10
            headerModel: ["№", "Дата добаления", "Дата изменения", "Группа", "Диагноз", "Признаки", "Действия"];

            onOpenButtonClicked: {
                var id = table.tableModel[row].row[0];
                var image = dbWorker.getImage(id);
                if(image.error)
                    messageBox.showCriticalMessage("Ошибка", "Не удалось загрузить изображение");
                else
                {
                    Functions.openEditImagePage(image);
                }
            }
            onDeleteButtonClicked: {
                yesNoMessageDiaglog.deleteImageId = table.tableModel[row].row[0];
                yesNoMessageDiaglog.show("Подтверждение", "Вы действительно хотите удалить изображение?");
            }
        }

    }

    Rectangle{ // Инструмент для перетягивания
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 12
        x: filterRect.width - width / 2
        color: "transparent"
        onXChanged: filterRect.width = x + width / 2;

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.SplitHCursor
            drag.target: parent
            drag.axis: Drag.XAxis
        }
    }

    CustomMessageBox{
        id: messageBox
        visible: false
    }

    YesNoMessageDialog{
        property int deleteImageId: 0
        id: yesNoMessageDiaglog
        visible: false
        onYes: {
            dbWorker.deleteImage(deleteImageId);
            root.showResults();
        }
    }
}
