import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.0

Rectangle {
    visible: true
    id: root

    color: "transparent"

    Rectangle {
        id: background
        anchors.fill: parent
        //radius: parent.height * 0.3
        opacity: 0.5
        color: "white"
        border.color: Qt.rgba(0, 0, 0, 0.1)
        border.width: 1
    }

    DockItem {
        id: launcherItem
        anchors.left: parent.left
        anchors.top: parent.top

        iconSizeRatio: 0.75
        enableActivateDot: false
        iconName: "qrc:/svg/launcher.svg"
        popupText: qsTr("Launcher")

        onClicked: {
            process.showLauncherDBus();
        }
    }

    Item {
        id: appList
        anchors.left: launcherItem.right
        anchors.top: parent.top
        width: parent.width - launcherItem.width - timeItem.width
        height: parent.height

        ListView {
            id: pageView
            anchors.fill: parent
            orientation: Qt.Horizontal
            snapMode: ListView.SnapOneItem
            model: appModel
            clip: true

            delegate: AppItemDelegate { }
        }
    }

//    DockItem {
//        id: trashItem
//        anchors.left: appList.right
//        anchors.top: parent.top
//        popupText: qsTr("Trash")

//        iconSizeRatio: 0.75
//        enableActivateDot: false
//        iconName: "user-trash-empty"
//    }

    TimeItem {
        id: timeItem
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
    }
}
