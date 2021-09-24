import QtQuick 2.0
import QtQuick.Controls 2.2
import "Properties.js" as Properties

Popup{
    id: root
//    x: addFeatureButton.x
//    y: addFeatureButton.y + addFeatureButton.height
//    width: addFeatureButton.width
    implicitHeight: contentItem.implicitHeight
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    focus: true
    padding: 1

    property alias model: listView.model

    signal selected(var data)


    contentItem: ListView{
        id: listView
//        model: dbWorker.featureTypes()

        clip: true
        implicitHeight: contentHeight
        ScrollIndicator.vertical: ScrollIndicator { }
        delegate: Rectangle{
            border.width: delegateMouseArea.containsMouse ? 1 : 0;
            color: index % 2 ? "white" : "#dadada";
            height: 30
            width: parent.width
            implicitHeight: txt.implicitHeight
            Text{
                id: txt
                anchors.centerIn: parent
                text: modelData
                font.family: Properties.fontFamily
                font.pointSize: Properties.fontPointSize

            }
            MouseArea{
                id: delegateMouseArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
//                    featuresPopup.close();
//                    var feature = {};
//                    feature._featureType = modelData;
//                    featuresListModel.append(feature);
                    root.selected(modelData);
                    root.close();
                }
            }
        }
    }
}
