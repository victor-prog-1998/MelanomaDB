import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import "Properties.js" as Properties

Rectangle{
    id: root
    property alias featuresListModel: featuresListModel
    property bool markerAddingMode: false
    property string markerType: ""
    property var markerAddingFeature: ({})  // признак, к которому добавляется маркер

    color: "white"
    width: 200
    border.width: 1

    function finishMarkerAdding(markerId, markerType)
    {
        root.markerAddingFeature._markerAddingMode = false;
        root.markerAddingFeature._markers.append({_id: markerId, _type: markerType, _visible: true, _draggingEnabled: false})
    }


    ColumnLayout{
        anchors.fill: parent
        anchors.margins: 5
        Layout.fillWidth: true

        Label{
            Layout.fillWidth: true
            text: "Признаки"
            font.family: Properties.fontFamily
            font.pointSize: Properties.fontPointSize + 2
        }

        CustomButton{
            id: addFeatureButton
            text: "Добавить признак"
            Layout.fillWidth: true
            onClicked: {
                featuresPopup.open();
            }
        }

        ListPopup{
            id: featuresPopup
            x: addFeatureButton.x
            y: addFeatureButton.y + addFeatureButton.height
            width: addFeatureButton.width
            model: dbWorker.featureTypes();
            onSelected: {
                var feature = {};
                feature._featureType = data;
                feature._markerAddingMode = false;
                feature._id = (featuresListModel.count == 0) ? 0 : featuresListModel.get(featuresListModel.count - 1)._id + 1;
                feature._markers = [];
                feature._markersColor = "red";
                feature._markersAreVisible = true;
                feature._markersListIsExpanded = false;
                featuresListModel.append(feature);


            }
        }

        Rectangle{
            Layout.fillHeight: true
            Layout.fillWidth: true
            ListView{
                anchors.fill: parent
                ScrollIndicator.vertical: ScrollIndicator { }
                model: featuresListModel
                spacing: 3
                clip: true

                delegate: Column{
                    width: parent.width
                    spacing: 2
                    Rectangle{
                        border.width: 1
                        width: parent.width
                        height: 60
                        color: "lightblue"

                        Row{
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.margins: 8
                            spacing: 4
                            Rectangle{
                                width: 12
                                height: width
                                border.width: 1
                                color: _markersColor
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            Text{
                                id: featureTypeText
                                font.family: Properties.fontFamily
                                font.pointSize: Properties.fontPointSize
                                text: _featureType
                            }

                        }


                        Row{
                            anchors.top: parent.top
                            anchors.right: parent.right
                            anchors.margins: 8
                            spacing: 4

//                            Text{
//                                id: featureNumberText
//                                font.family: Properties.fontFamily
//                                font.pointSize: Properties.fontPointSize
//                                text: index + 1
//                                color: "gray"
//                            }
                            ButtonWithIcon{
                                id: deleteFeatureButton
                                source: "qrc:/icons/delete-icon.png"
                                toolTipText: "Удалить"
                                enabled: !root.markerAddingMode
                                onClicked: {


                                    var newMarkersCount = markersListModel.count - _markers.count;
                                    _markers.clear();

                                    var i = 0;

                                    while(markersListModel.count > newMarkersCount)
                                    {
                                        var marker = markersListModel.get(i);
                                        if(marker._featureId === _id)
                                            markersListModel.remove(i);
                                        else
                                            ++i;
                                    }


                                    featuresListModel.remove(index);
                                }
                            }
                        }




                        Row{ // инструменты маркеров
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            anchors.margins: 8
                            spacing: 4

                            ButtonWithIcon{
                                visible: _markers.count > 0
                                source: _markersListIsExpanded ? "qrc:/icons/collapse-icon.png" : "qrc:/icons/expand-icon.png"
                                toolTipText: _markersListIsExpanded ? "Cвернуть список маркеров" : "Развернуть список маркеров";
                                onClicked: {
                                    _markersListIsExpanded = !_markersListIsExpanded;
                                }
                            }

                            ButtonWithIcon{
                                id: addMarkerButton
                                enabled: !(root.markerAddingMode ^ _markerAddingMode)

                                source: _markerAddingMode ?
                                            "qrc:/icons/cancel-adding-icon.png" :
                                                            "qrc:/icons/add-feature-icon.png"
                                toolTipText: _markerAddingMode ?
                                                 "Отменить добавление маркера" : "Добавить маркер"
                                onClicked: {
                                    if(_markerAddingMode)
                                    {
                                        root.markerAddingMode =
                                                _markerAddingMode = false;
                                    }
                                    else
                                        addMarkerPopup.open();
                                }
                            }

                            ButtonWithIcon{
                                source: "qrc:/icons/palette-icon.png"
                                toolTipText: "Цвет маркеров"
                                onClicked: {
                                    colorDialog.featureIndex = index;
                                    colorDialog.color = _markersColor;
                                    colorDialog.open();
                                }
                            }
                            ButtonWithIcon{
                                source: _markersAreVisible ? "qrc:/icons/visible-icon.png" : "qrc:/icons/invisible-icon.png"
                                opacity: _markersAreVisible ? 1.0 : 0.5;
                                toolTipText: "Показать / скрыть маркеры"
                                onClicked: {
                                    if(!_markerAddingMode) // в режиме добавления маркеры автоматически делаюются видимыми
                                    {
                                        _markersAreVisible = !_markersAreVisible;
                                        for(var i = 0; i < _markers.count; ++i)
                                        {
                                            var marker = _markers.get(i);
                                            markersListModel.getById(marker._id)._visible = _markersAreVisible;
                                            // иконка в списке маркеров:
                                            marker._visible = _markersAreVisible;

                                        }
                                    }
                                }
                            }




                        } // инструменты маркеров (Row)

                        ListPopup{
                            id: addMarkerPopup
                            x: addMarkerButton.parent.x + addMarkerButton.x + addMarkerButton.width
                            y: addMarkerButton.parent.y + addMarkerButton.y + addMarkerButton.height
                            width: 100
                            model: dbWorker.markerTypes();
                            onSelected: {
                                root.markerAddingMode = _markerAddingMode = true;
                                _markersAreVisible = true;
                                root.markerAddingFeature = featuresListModel.get(index);
                                root.markerType = data;

                                console.log(index)
                            }
                        }
                    } // Feature Rectangle
                    Column{ //
                        visible: _markersListIsExpanded
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        anchors.right: parent.right
                        spacing: 2
                        Repeater{
                            model: _markers
                            delegate: Rectangle{
                                id: markersListDelegate
                                width: parent.width
                                height: 60
                                border.width: 1
                                color: "white"

                                Row{
                                    anchors.top: parent.top
                                    anchors.left: parent.left
                                    anchors.margins: 8
                                    spacing: 4
                                    ButtonWithIcon{
                                        opacity: _visible ? 1.0 : 0.5
                                        source: _visible ? "qrc:/icons/visible-icon.png" : "qrc:/icons/invisible-icon.png"
                                        toolTipText: "Показать / скрыть"
                                        onClicked: {
                                            _visible = !_visible;
                                            markersListModel.getById(_id)._visible = _visible;
                                        }
                                    }
                                    Text{
                                        font.family: Properties.fontFamily
                                        font.pointSize: Properties.fontPointSize
                                        text: _type
                                    }
                                }



                                Row{
                                    anchors.top: parent.top
                                    anchors.right: parent.right
                                    anchors.margins: 8
                                    spacing: 4


                                    ButtonWithIcon{
                                        source: "qrc:/icons/delete-icon.png"
                                        toolTipText: "Удалить"
                                        onClicked: {

                                            for(var i = 0; i < markersListModel.count; ++i)
                                                if(markersListModel.get(i)._id === _id)
                                                {
                                                    markersListModel.remove(i);
                                                    break;
                                                }
                                            for(i = 0; i < _markers.count; ++i)
                                                if(_markers.get(i)._id === _id)
                                                {
                                                    _markers.remove(i);
                                                    break;
                                                }
                                        }
                                    }
                                }

                                Row{
                                    anchors.left: parent.left
                                    anchors.bottom: parent.bottom
                                    anchors.margins: 8
                                    spacing: 4
                                    ButtonWithIcon{
                                        id: lineWidthButton

                                        source: "qrc:/icons/line_width-icon.png"
                                        toolTipText: "Толщина маркера"
                                        onClicked: {
                                            lineWidthPopup.open();
                                        }
                                    }
                                    ButtonWithIcon{
                                        source: "qrc:/icons/drag-icon.png"
                                        opacity: _draggingEnabled ? 1.0 : 0.5;
                                        toolTipText: "Перетаскивание контрольных точек"
                                        onClicked: {
                                            _draggingEnabled = !_draggingEnabled;
                                            markersListModel.getById(_id)._draggingEnabled = _draggingEnabled;
                                        }
                                    }
                                }



                                Popup{
                                    id: lineWidthPopup
                                    x: lineWidthButton.parent.x + lineWidthButton.x
                                    y: lineWidthButton.parent.y + lineWidthButton.y + lineWidthButton.height
                                    implicitWidth: contentItem.implicitWidth
                                    implicitHeight: contentItem.implicitHeight
                                    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
                                    focus: true
                                    padding: 1
                                    contentItem: SpinBox{
                                        from: 1
                                        to: 10
                                        value: markersListModel.getById(_id) === undefined ? 0 :
                                                                        markersListModel.getById(_id)._lineWidth
                                        font.pointSize: Properties.fontPointSize
                                        font.family: Properties.fontFamily
                                        onValueChanged: {
                                            var marker = markersListModel.getById(_id);
                                            if(marker !== undefined)
                                                markersListModel.getById(_id)._lineWidth = value;
                                        }
                                    }
                                }




                            }
                        }
                    }
                }
            } // ListView
        }


    }

    ListModel{
        id: featuresListModel

    }

    ColorDialog{
        property int featureIndex: 0
        id:colorDialog
        visible: false
        modality: Qt.WindowModal
        onAccepted: {
            var feature = featuresListModel.get(featureIndex);
            feature._markersColor = color.toString();
            for(var i = 0; i < feature._markers.count; ++i)
            {
                var id = feature._markers.get(i)._id;
                markersListModel.getById(id)._color = color.toString();
            }
        }

    }
}
