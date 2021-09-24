import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import "Properties.js" as Properties
import "Functions.js" as Functions

Rectangle{
    id: root
    color: "lightblue"
    height: rootLayout.implicitHeight + rootLayout.anchors.margins * 2;

    property bool addMode: false
    property alias melanomaRadioButton: melanomaRadioButton

    property alias nonMelanomaRadioButton: nonMelanomaRadioButton
    property alias nonMelanomaDiagnosisComboBox: nonMelanomaDiagnosisComboBox
    property alias otherNonmelanomaCheckBox: otherNonmelanomaCheckBox
    property alias otherNonmelanomaField: otherNonmelanomaField


    property alias groupComboBox: groupComboBox
    property alias otherGroupCheckBox: otherGroupCheckBox
    property alias otherGroupField: otherGroupField


    signal cancelClicked();
    signal deleteImageClicked()
    signal addImageClicked()
    signal saveChangesClicked() // используется в режиме просмотра / редактирования



    onVisibleChanged: {
        if(visible)
        {
            groupComboBox.comboBox.model = dbWorker.groups();
            nonMelanomaDiagnosisComboBox.comboBox.model = dbWorker.nonMelanomaDiagnoses();
        }
    }





    RowLayout{
        id: rootLayout
        anchors.margins: 10
        anchors.fill: parent
        Layout.fillWidth: true

        ColumnLayout{

            Item {
                Layout.fillHeight: true
            }

            CustomGroupBox{
                Layout.fillHeight: false
                Layout.fillWidth: true
                Layout.minimumWidth: 120
                Layout.maximumWidth: 200
                title: "Диагноз"

                ColumnLayout{


                    anchors.fill: parent
                    CustomRadioButton{
                        id: melanomaRadioButton
                        Layout.fillWidth: true
                        text: "Меланома"
                    }
                    CustomRadioButton{
                        id: nonMelanomaRadioButton
                        Layout.fillWidth: true
                        text: "Не меланома"
                        //implicitHeight: 16
                    }
                }



            } // Group Box
        }



        ColumnLayout{ // Non melanoma
            visible: nonMelanomaRadioButton.checked
            Layout.fillWidth: true
            //Layout.minimumWidth: 200
            Layout.maximumWidth: 500

            RowLayout{
                ColumnLayout{
                    Item{
                        Layout.fillHeight: true
                    }
                    ComboBoxWithLabel{
                        id: nonMelanomaDiagnosisComboBox;
                        label.text: "Уточнение"
                        comboBox.currentIndex: -1
                        comboBox.model: dbWorker.nonMelanomaDiagnoses()
                        enabled: !otherNonmelanomaCheckBox.checked
                        //Layout.fillWidth: true
                    }
                }

                ColumnLayout{
                    Item{
                        Layout.fillHeight: true
                    }
                    CustomCheckBox{
                        id: otherNonmelanomaCheckBox
                        text: "Другое"
                    }
                }
            }

            CustomTextField{
                id: otherNonmelanomaField
                Layout.fillWidth: true
                visible: otherNonmelanomaCheckBox.checked
            }
        } // RowLayout (Non melanoma)

        ColumnLayout{ // Group
            Layout.fillWidth: true
            //Layout.minimumWidth: 100
            Layout.maximumWidth: 200

            RowLayout{
                ColumnLayout{
                    Item{
                        Layout.fillHeight: true
                    }
                    ComboBoxWithLabel{
                        id: groupComboBox;
                        label.text: "Группа"
                        comboBox.currentIndex: -1
                        comboBox.model: dbWorker.groups()
                        enabled: !otherGroupCheckBox.checked
                        //Layout.fillWidth: true
                    }
                }

                ColumnLayout{
                    Item{
                        Layout.fillHeight: true
                    }
                    CustomCheckBox{
                        id: otherGroupCheckBox
                        text: "Другая"
                    }
                }
            }

            CustomTextField{
                id: otherGroupField
                Layout.fillWidth: true
                visible: otherGroupCheckBox.checked
            }
        } // RowLayout (Group)


        Item{
            Layout.fillWidth: true
        }

        ColumnLayout{
            Layout.fillWidth: true
            visible: !root.addMode
            //Layout.minimumWidth: 100
            Layout.maximumWidth: 200
            Item{
                Layout.fillHeight: true
            }
            CustomButton{ // Удаление
                Layout.fillWidth: true
                text: "Удалить"
                color: Properties.buttonColor2
                hoverColor: Properties.buttonSpecialColor2
                onClicked: {
                    deleteMessageDialog.show("Подтверждение", "Вы уверены, что хотите удалить изображение из базы?");
                }
            }
            CustomButton{
                Layout.fillWidth: true
                text: "Сменить изображение"
                onClicked: changeImageFileDialog.visible = true;
            }
        }

        ColumnLayout{  
            Layout.fillWidth: true
            //Layout.minimumWidth: 100
            Layout.maximumWidth: 200
            Item{
                Layout.fillHeight: true
            }


            CustomButton{ // Отмена / Назад
                Layout.fillWidth: true
                color: root.addMode ? Properties.buttonColor2 : Properties.buttonColor
                hoverColor: root.addMode ? Properties.buttonSpecialColor2 : Properties.buttonSpecialColor
                textColor: Properties.buttonTextColor
                textHoverColor: Properties.buttonTextSpecialColor
                text: root.addMode ? "Отмена" : "Назад"
                onClicked: {
                    var question = root.addMode ? "Вы уверены, что хотите отменить добавление изображения?" :
                                                  "Внесённые изменения не будут сохранены. Вернуться назад?";
                    cancelMessageDialog.show("Подтверждение", question);
                }
            }

//            CustomButton{ // Удаление
//                Layout.fillWidth: true
//                text: "Удалить"
//                visible: !root.addMode
//                color: Properties.buttonColor2
//                hoverColor: Properties.buttonSpecialColor2
//                onClicked: {
//                    deleteMessageDialog.show("Подтверждение", "Вы уверены, что хотите удалить изображение из базы?");
//                }
//            }

            CustomButton{ // Сохранение
                enabled: Functions.checkReadyToSaveImage();
                Layout.fillWidth: true
                text: "Сохранить"
                color: Properties.buttonColor3
                hoverColor: Properties.buttonSpecialColor3
                onClicked: { // В режиме добавления не требуем подтверждение
                    if(root.addMode)
                        root.addImageClicked();
                    else
                        saveMessageDialog.show("Подтверждение", "Вы уверены, что хотите сохранить изменения?");

//                    if(addMode)
//                    {
//                        if(Functions.addImage())
//                        {
//                            messageBox.showInformationMessage("Информация", "Изображение добавлено в базу");
//                            Functions.closeImagePage();
//                        }
//                        else
//                            messageBox.showCriticalMessage("Ошибка", "Не удалось добавить изображение в базу");
//                    }
//                    else
//                        saveMessageDialog.show("Подтверждение", "Вы уверены, что хотите сохранить изменения?");
                }
            }
        }
    }


    YesNoMessageDialog{
        id: cancelMessageDialog
        visible: false
        onYes: {
            // Functions.closeImagePage();
            root.cancelClicked();
        }
    }

    YesNoMessageDialog{
        id: deleteMessageDialog
        visible: false
        onYes: root.deleteImageClicked();
    }

    YesNoMessageDialog{     // показывается при нажатии на кнопку "сохранить" только в режиме просмотра / редактирования
        id: saveMessageDialog
        visible: false
        onYes: root.saveChangesClicked();
    }



    CustomMessageBox{
        id: messageBox
        visible: false
    }

    FileDialog{
        id: changeImageFileDialog
        visible: false
        onAccepted: Functions.changeImage(fileUrl);
    }

}
