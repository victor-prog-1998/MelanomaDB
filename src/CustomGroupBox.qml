import QtQuick 2.0
import QtQuick.Controls 2.2
import "Properties.js" as Properties

GroupBox{
    id: root
    property alias titleLabel: lbl
    property alias rectangle: rect
//    Component.onCompleted: {
//        console.log("top: " + topPadding + ", bottom: " + bottomPadding);
//    }

    topPadding: 20
    bottomPadding: 0


    label: Label{
        id: lbl
        text: root.title
        font.pointSize: Properties.fontPointSize
        font.family: Properties.fontFamily
    }

    background: Rectangle {


        id: rect
         y: root.topPadding -  root.bottomPadding
         width: parent.width
         height: parent.height -  root.topPadding +  root.bottomPadding
         //height: parent.height -  root.topPadding
         color: "transparent"
         border.color: Properties.groupBoxFrameColor
         //radius: height / 2
         //antialiasing: false
    }
 }
