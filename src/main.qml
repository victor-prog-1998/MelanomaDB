import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    id: programRoot
    visible: true
    width: 700
    height: 480

    StackView{
        id: stackView
        anchors.fill: parent
        anchors.margins: 10
        initialItem: mainPage
    }


    MainPage{
        id: mainPage
    }

    ImagePage{
        id: imagePage
        visible: false
    }









}
