import QtQuick 2.0
import QtQuick.Controls 2.2
import "Properties.js" as Properties

Flickable{
    property alias area: area
    // height: area.implicitHeight
    id: flickable
    ScrollBar.vertical: ScrollBar { }
    TextArea.flickable: TextArea{

        id: area
        selectByMouse: true
        font.pointSize: Properties.fontPointSize
        font.family: Properties.fontFamily
        property bool highlight: false

        background: Rectangle{
            id: rect

        }
        implicitHeight: 100
    }
}
