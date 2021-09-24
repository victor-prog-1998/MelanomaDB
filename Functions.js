function openAddImagePage(imageUrl)
{
    var image = imagePage.image;
    image.source = imageUrl;
    if(image.status === Image.Error)
        messageBox.showCriticalMessage("Ошибка", "Не удалось загрузить изображение");
    else
    {
        imagePage.addMode = true;
        stackView.push(imagePage);
    }
}

function changeImage(imageUrl)
{
    var image = imagePage.image;
    var source = image.source;
    image.source = imageUrl;
    if(image.status === Image.Error)
    {
        messageBox.showCriticalMessage("Ошибка", "Не удалось загрузить изображение");
        image.source = source;
    }
}

function openEditImagePage(image)
{
    imagePage.addMode = false;
    imagePage.imageModel = image;
    stackView.push(imagePage);
}

function closeImagePage()
{
    imagePage.image.source = "";
    imagePage.zoom = 1.0;
    descriptionToolbar.melanomaRadioButton.checked = false;
    descriptionToolbar.nonMelanomaRadioButton.checked = false;

    descriptionToolbar.nonMelanomaDiagnosisComboBox.reset();
    descriptionToolbar.otherNonmelanomaCheckBox.checked = false;
    descriptionToolbar.otherNonmelanomaField.reset();

    descriptionToolbar.groupComboBox.reset();
    descriptionToolbar.otherGroupCheckBox.checked = false;
    descriptionToolbar.otherGroupField.reset();

    imageFeaturesToolbar.featuresListModel.clear();

    markersListModel.clear();

    stackView.pop();
}

function checkReadyToSaveImage()
{
    if(!melanomaRadioButton.checked && !nonMelanomaRadioButton.checked)
        return false;
    if(nonMelanomaRadioButton.checked)
    {
        if(otherNonmelanomaCheckBox.checked && otherNonmelanomaField.isEmpty())
            return false;
        if(!otherNonmelanomaCheckBox.checked && !nonMelanomaDiagnosisComboBox.isSelected())
            return false;
    }

    if(otherGroupCheckBox.checked && otherGroupField.isEmpty())
        return false;
    if(!otherGroupCheckBox.checked && !groupComboBox.isSelected())
        return false;

    return true;
}


function addImage(originalId) // Вызывается с ImagePage. Возвращает id изображения в базе (при ошибке вернет 0)
{
    // originalId как и в методе insertImage класса DBWorker указывается, когда создается "новая" запись при редактировании

    if(originalId === undefined)  // QML не поддерживает синтаксис JS для значений по умолчанию
        originalId = 0;


    var group = descriptionToolbar.otherGroupCheckBox.checked ? descriptionToolbar.otherGroupField.text :
                                                                descriptionToolbar.groupComboBox.currentText;
    var diagnosis = descriptionToolbar.melanomaRadioButton.checked ? descriptionToolbar.melanomaRadioButton.text :
                                                  descriptionToolbar.otherNonmelanomaCheckBox.checked ?
                                                                         descriptionToolbar.otherNonmelanomaField.text :
                                                      descriptionToolbar.nonMelanomaDiagnosisComboBox.currentText;

    var imageId = dbWorker.insertImage(group, diagnosis, imagePage.image.source, originalId);

    console.log("image id: " + imageId);

    if(imageId === 0)
        return 0;


    // Добавление признаков


    for(var i = 0; i < imageFeaturesToolbar.featuresListModel.count; ++i)
    {
        var feature = imageFeaturesToolbar.featuresListModel.get(i);
        var featureType = feature._featureType;
        var color = feature._markers.count > 0 ? feature._markersColor : "";
        var featureId = dbWorker.insertFeature(imageId, featureType, color);
        if(featureId === 0)
        {
            dbWorker.deleteImage(imageId);
            return 0;
        }
        for(var j = 0; j < feature._markers.count; ++j)
        {
            var marker = markersListModel.getById(feature._markers.get(j)._id);
            var markerType = marker._type;
            var lineWidth = marker._lineWidth;
            var x1 = marker._x1;
            var y1 = marker._y1;
            var x2 = marker._x2;
            var y2 = marker._y2;
            dbWorker.insertMarker(featureId, markerType, lineWidth, x1, y1, x2, y2);
        }
    }

    return imageId;
}

function saveChages() // Вызывается с ImagePage. Возвращает bool (в зависимости от того, удалось сохранить или нет).
{
    var id = imageModel.id;
    var temporaryId = addImage(id);
    if(temporaryId === 0)
        return false;
    dbWorker.deleteImage(id); // удаление всех записей таблиц, относящихся к исходному изображению
    dbWorker.changeImageId(temporaryId, id); // замена временного id на исходный во всех записях таблиц, относящихся к temporaryId
    return true;
}

function showImagesTable()
{
    var sort;
    switch(sortComboBox.currentText)
    {
    case "По диагнозу":
        sort = "diagnosis";
        break;
    case "По группе":
        sort = "group";
        break;
    default:
        sort = "id";
        break;
    }
    imagesTable.tableModel = dbWorker.allImagesModel(sort);
}

function setImage()
{
    image.source = imageModel.imageSource;
    descriptionToolbar.groupComboBox.comboBox.currentIndex =
            descriptionToolbar.groupComboBox.comboBox.find(imageModel.group);
    if(imageModel.diagnosis === "Меланома")
        descriptionToolbar.melanomaRadioButton.checked = true;
    else
    {
        descriptionToolbar.nonMelanomaRadioButton.checked = true;
        descriptionToolbar.nonMelanomaDiagnosisComboBox.comboBox.currentIndex =
                descriptionToolbar.nonMelanomaDiagnosisComboBox.comboBox.find(imageModel.diagnosis);
    }

    var markerId = 0;   // счетчик для задания id маркерам
    var features = imageModel.features;
    for(var i = 0; i < features.length; ++i)
    {
        var obj = {};
        obj._featureType = features[i].featureType;
        obj._markerAddingMode = false;
        obj._id = i;
        obj._markersColor = features[i].color === "" ? "red" : features[i].color;
        obj._markersAreVisible = true;
        obj._markersListIsExpanded = false;
        obj._markers = [];

        var markers = features[i].markers;
        for(var j = 0; j < markers.length; ++j)
        {
            obj._markers.push({_id: markerId, _type: markers[j].type, _visible: true, _draggingEnabled: false});
            var marker = {};
            marker._id = markerId;
            marker._featureId = i;
            marker._visible = true;
            marker._draggingEnabled = false;
            marker._type = markers[j].type;
            marker._color = features[i].color;
            console.log(features[i].color);
            marker._lineWidth = markers[j].lineWidth;
            marker._x1 = markers[j].x1;
            marker._y1 = markers[j].y1;
            marker._x2 = markers[j].x2;
            marker._y2 = markers[j].y2;
            markersListModel.append(marker);

            ++markerId;
        }

        imageFeaturesToolbar.featuresListModel.append(obj);
    }



//    var marker = {};
//    marker._featureId = imageFeaturesToolbar.markerAddingFeature._id;
//    marker._visible = true;
//    marker._draggingEnabled = false;
//    marker._type = imageFeaturesToolbar.markerType;
//    marker._color = imageFeaturesToolbar.markerAddingFeature._markersColor;
//    marker._lineWidth = 2;
//    marker._x2 = marker._x1 = mouseX;
//    marker._y2 = marker._y1 = mouseY;
//    marker._id = (markersListModel.count == 0) ? 0 : markersListModel.get(markersListModel.count - 1)._id + 1;
//    console.log("Маркер создан, id = " + marker._id);
//    markersListModel.append(marker);

}


