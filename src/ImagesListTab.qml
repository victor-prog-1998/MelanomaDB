import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import "Functions.js" as Functions

Item {






    id: root

    Component.onCompleted: {

        Functions.showImagesTable();
        // sortComboBox.comboBox.currentIndex = sortComboBox.comboBox.find("По диагнозу")
    }

    onVisibleChanged: if(visible) Functions.showImagesTable();




    Rectangle{

        color: "lightblue"
        id: controlRect
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 100



        RowLayout{
            anchors.fill: parent
            Layout.fillWidth: true
            anchors.margins: 10
            ColumnLayout{
                Item{
                    Layout.fillHeight: true
                }
                ComboBoxWithLabel{
                    id: sortComboBox
                    Layout.minimumWidth: 200
                    Layout.maximumWidth: 500
                    Layout.fillWidth: true

                    onCurrentTextChanged: {
                        Functions.showImagesTable();

                        // console.log("+" + currentText);
                    }

                    label.text: "Сортировка"
                    comboBox.currentIndex: 0
                    comboBox.model: {
                        var m = ["По номеру", "По группе", "По диагнозу"];
                        return m;
                    }
                }
            }


            Item{
                Layout.fillWidth: true
            }
            ColumnLayout{
                Item{
                    Layout.fillHeight: true
                }
                CustomButton{
                    text: "Добавить изображение"
                    onClicked: fileDialog.visible = true;
                }
            }


        }


    } // controlRect (панель инструментов)



    Table{
        id: imagesTable
        anchors.top: controlRect.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        headerModel: ["№", "Дата добаления", "Дата изменения", "Группа", "Диагноз", "Признаки", "Действия"];
        //tableModel: dbWorker.imagesModel(sort);

        onOpenButtonClicked: {
            var id = imagesTable.tableModel[row].row[0];
            var image = dbWorker.getImage(id);
            if(image.error)
                messageBox.showCriticalMessage("Ошибка", "Не удалось загрузить изображение");
            else
            {
                Functions.openEditImagePage(image);
            }
        }
        onDeleteButtonClicked: {
            yesNoMessageDiaglog.deleteImageId = imagesTable.tableModel[row].row[0];
            yesNoMessageDiaglog.show("Подтверждение", "Вы действительно хотите удалить изображение?");
        }
    }

    FileDialog{
        id: fileDialog
        visible: false
        //selectedNameFilter: "Изображения"
        // nameFilters: ["Изображения"]
        onAccepted: {
            Functions.openAddImagePage(fileUrl);
            //Functions.openEditImagePage(1);
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
            Functions.showImagesTable();
        }
    }


}
