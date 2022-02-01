import QtQuick 2.0
import "Functions.js" as Functions
import com.PaintedItem 1.0

Item {
    id: root

    property alias image: image

    property double zoom: 1.0


    property bool addMode: true
    property var imageModel: ({}) // используется для загрузки данных об изображении в режиме просмотра / редактиования

    property bool pointsDragging: false



    onVisibleChanged: {
//        var radioButton = (imageModel.diagnosis === "Меланома") ? descriptionToolbar.melanomaRadioButton :
//                                                                  descriptionToolbar.nonMelanomaRadioButton;
//        radioButton.checked = true;

        if(!addMode && visible)
        {
            Functions.setImage();
        }
    }



    ImageDescriptionToolbar{

       id: descriptionToolbar
       anchors.left: parent.left
       anchors.right: parent.right
       anchors.top: parent.top
       addMode: root.addMode

       onCancelClicked: {
           Functions.closeImagePage();
       }

       onDeleteImageClicked: {
           dbWorker.deleteImage(imageModel.id);
           Functions.closeImagePage();
       }

       onAddImageClicked: {
           if(Functions.addImage())
           {
               messageBox.showInformationMessage("Информация", "Изображение добавлено в базу");
               Functions.closeImagePage();
           }
           else
               messageBox.showCriticalMessage("Ошибка", "Не удалось добавить изображение в базу");
       }

       onSaveChangesClicked: {
           if(Functions.saveChages())
           {
               messageBox.showInformationMessage("Информация", "Изменения сохранены");
               Functions.closeImagePage();
           }
           else
               messageBox.showCriticalMessage("Ошибка", "Не удалось сохранить изменения");
       }
    }

    ImageFeaturesToolbar{

        id: imageFeaturesToolbar
        anchors.top: descriptionToolbar.bottom
        anchors.left: parent.left
        anchors.bottom: parent.bottom
    }



    Flickable{
        id: imageFlickable
        anchors.top: descriptionToolbar.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: imageFeaturesToolbar.right
        // x: imageFeaturesToolbar.x + width
        contentHeight: image.height * root.zoom
        contentWidth: image.width * root.zoom
        // boundsBehavior: Flickable.StopAtBounds
        clip: true

        interactive: !imageFeaturesToolbar.markerAddingMode && !root.pointsDragging;

        Image{
            id: image
            cache: false




            transform: Scale{
                id: imageScale
                origin.x: 0
                origin.y: 0
                xScale: root.zoom
                yScale: root.zoom
            }

            onSourceChanged: {
                console.log("source: " + source);
            }
            MouseArea{
                anchors.fill: parent
                cursorShape: imageFeaturesToolbar.markerAddingMode ? Qt.CrossCursor : Qt.ArrowCursor;
                onWheel: {
                    var k = 1.1;
                    var scaleFactor = wheel.angleDelta.y > 0 ? k : 1 / k;
                    root.zoom *= scaleFactor;
                }


                onPressed: {
                    if(imageFeaturesToolbar.markerAddingMode)
                    {
                        var marker = {};
                        marker._featureId = imageFeaturesToolbar.markerAddingFeature._id;
                        marker._visible = true;
                        marker._draggingEnabled = false;
                        marker._type = imageFeaturesToolbar.markerType;
                        marker._color = imageFeaturesToolbar.markerAddingFeature._markersColor;
                        marker._lineWidth = 2;
                        marker._x2 = marker._x1 = mouseX;
                        marker._y2 = marker._y1 = mouseY;
                        marker._id = (markersListModel.count == 0) ? 0 : markersListModel.get(markersListModel.count - 1)._id + 1;
                        console.log("Маркер создан, id = " + marker._id);
                        markersListModel.append(marker);
                    }
                }

                onPositionChanged: {
                    if(imageFeaturesToolbar.markerAddingMode)
                    {
                        var lastMarker = markersListModel.get(markersListModel.count - 1);
                        lastMarker._x2 = mouseX;
                        lastMarker._y2 = mouseY;
                    }
                }

                onReleased: {
                    if(imageFeaturesToolbar.markerAddingMode)
                    {
                        imageFeaturesToolbar.markerAddingMode = false;
                        var lastMarker = markersListModel.get(markersListModel.count - 1);

                        imageFeaturesToolbar.finishMarkerAdding(lastMarker._id, lastMarker._type);

                    }
                }

            }

            Repeater{ // для маркеров
                anchors.fill: parent

                model: markersListModel

                PaintedItem{
                    id: paintedItem
                    anchors.fill: parent
                    x1: _x1
                    y1: _y1
                    x2: _x2
                    y2: _y2

                    type: _type
                    lineWidth: _lineWidth
                    color: _color
                    visible: _visible
                    MarkerDragTool{

                        id: dragTool1
//                        x: parent.x1 - width / 2;
//                        y: parent.y1 - height / 2;

                        onVisibleChanged: {
                            if(visible)
                            {
                                x = parent.x1 - width / 2;
                                y = parent.y1 - height / 2;
                            }
                        }


                        visible: _draggingEnabled
                        onPositionChanged: {
                            if(dragTool1.mouseArea.drag.active)
                            {
                                var dx = parent.x2 - parent.x1; // для окружности
                                var dy = parent.y2 - parent.y1; // для окружности

                                markersListModel.get(index)._x1 = parent.x1 = x + width / 2;
                                markersListModel.get(index)._y1 = parent.y1 = y + height / 2;

                                if(parent.type === "Окружность")
                                {
                                    markersListModel.get(index)._x2 = parent.x2 = parent.x1 + dx;
                                    markersListModel.get(index)._y2 = parent.y2 = parent.y1 + dy;
                                    dragTool2.x = x + dx;
                                    dragTool2.y = y + dy;
                                }
                            }
                        }

                        // Без этого перетаскивание не работает при зуме - вместо контрольных точек
                        // перемещается изображение на Flickable
                        mouseArea.onPressed: {
                            root.pointsDragging = true;
                        }
                        mouseArea.onReleased: {
                            root.pointsDragging = false;
                        }


                    }

                    MarkerDragTool{
                        id: dragTool2

                        onVisibleChanged: {
                            if(visible)
                            {
                                x = parent.x2 - width / 2;
                                y = parent.y2 - height / 2;
                            }
                        }

//                        x: parent.x2 - width / 2;
//                        y: parent.y2 - height / 2;
                        visible: _draggingEnabled
                        onPositionChanged: {
                            if(dragTool2.mouseArea.drag.active)
                            {
                                markersListModel.get(index)._x2 = parent.x2 = x + width / 2;
                                markersListModel.get(index)._y2 = parent.y2 = y + height / 2;
                            }
                        }

                        mouseArea.onPressed: {
                            root.pointsDragging = true;
                        }
                        mouseArea.onReleased: {
                            root.pointsDragging = false;
                        }
                    }
                }



            }

        }
    }

    Rectangle{
        onXChanged: {
            if(ma.drag.active)
            {
                imageFeaturesToolbar.width = x + width / 2;
            }
        }
        //color: ma.containsMouse ? "red" : "lime"
        //opacity: 0.5
        color: "transparent"
        width: 10
        anchors.top: descriptionToolbar.bottom
        anchors.bottom: parent.bottom
        // anchors.horizontalCenter: imageFeaturesToolbar.right
        x: imageFlickable.x - width / 2 // то же самое, что imageFeaturesToolbar.width - width / 2

        MouseArea{
            id: ma
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


    ListModel{
        id: markersListModel
        function getById(id)
        {
            for(var i = 0; i < count; ++i)
                if(get(i)._id === id)
                    return get(i);

            return undefined;
        }
    }

}
