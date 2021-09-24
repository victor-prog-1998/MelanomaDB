import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page{
    TabBar{
        id: tabBar
        width: parent.width

        CustomTabButton{
            id: xxx
            buttonText.text: "Список изображений"
        }
        CustomTabButton{
            buttonText.text: "Фильтр"
        }
        CustomTabButton{
            buttonText.text: "Запрос к БД"
        }


    }

    StackLayout{
        anchors.top: tabBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        currentIndex: tabBar.currentIndex

        ImagesListTab{

        }

        FilterTab{

        }

        QueryTab{

        }


    }



}
