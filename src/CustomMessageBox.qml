import QtQuick.Dialogs 1.2

MessageDialog{
    id: root
    modality: Qt.NonModal

    function showCriticalMessage(title, text)
    {
        root.icon = StandardIcon.Critical;
        root.title = title;
        root.informativeText = text;
        root.visible = true;
    }
    function showInformationMessage(title, text)
    {
        root.icon = StandardIcon.Information;
        root.title = title;
        root.informativeText = text;
        root.visible = true;
    }
}
