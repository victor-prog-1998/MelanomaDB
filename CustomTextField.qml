import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import "Properties.js" as Properties

TextField{
    function setIncorrectState(text)
    {
        highlight = true;
        placeholderText = text;
        clear();
    }

    function setNormalState()
    {
        highlight = false;
        placeholderText = ""
    }

    function reset()
    {
        clear();
        setNormalState();
    }

    function isEmpty()
    {
        return text == "";
    }


    property alias rectangle: rect
    property bool highlight: false
    selectByMouse: true
    font.pointSize: Properties.fontPointSize
    font.family: Properties.fontFamily
    implicitHeight: Properties.textFieldHeight
    background: Rectangle{
        id: rect
        border.color: highlight ? "red" : rect.color;
        border.width: highlight ? 2 : 0;
    }
}


