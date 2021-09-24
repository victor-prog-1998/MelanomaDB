import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ColumnLayout{
    Layout.fillWidth: true
    Layout.fillHeight: true
    property alias label: label
    property alias field: field

    Label{
        id: label
        font.pixelSize: 16
        Layout.fillWidth: true
    }
    CustomTextField{
        id: field
        Layout.fillWidth: true
    }
}
