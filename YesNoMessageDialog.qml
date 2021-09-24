import QtQuick.Dialogs 1.2

MessageDialog{
    id: root
    standardButtons: StandardButton.No | StandardButton.Yes
    icon: StandardIcon.Question




    function show(title, text)
    {
        root.title = title;
        root.informativeText = text;
        root.visible = true;
    }
}
