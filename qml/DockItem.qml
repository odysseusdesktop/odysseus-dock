import QtQuick 2.12
import QtQuick.Controls 2.5

Rectangle {
    id: dockItem
    width: root.height
    height: root.height

    property bool enableActivateDot: true
    property bool isActive: false
    property bool showIcon: true
    property var activateDotColor: accentColor
    property var inactiveDotColor: "#000000"

    property var popupText

    property double iconSizeRatio: 0.8
    property string iconName

    signal clicked()
    signal rightClicked()

    color: "transparent"

    Image {
        id: icon
        source: {
            if(iconName !== undefined) {
                //console.log("QML Image: " + iconName);
                /*return iconName ? iconName.indexOf("/") !== -1 || iconName.indexOf("file://") === 0 || iconName.indexOf("qrc") === 0
                                  ? iconName : "image://icontheme/" + iconName : iconName;*/
                return "image://icontheme/" + iconName;
            }
        }
        sourceSize.width: parent.height * iconSizeRatio
        sourceSize.height: parent.height * iconSizeRatio
        width: sourceSize.width
        height: sourceSize.height
        smooth: true

        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }

        states: ["mouseIn", "mouseOut"]
        state: "mouseOut"

        transitions: [
            Transition {
                from: "*"
                to: "mouseIn"

                NumberAnimation {
                    target: icon
                    properties: "scale"
                    from: 1
                    to: 1.1
                    duration: 150
                    easing.type: Easing.OutCubic
                }
            },
            Transition {
                from: "*"
                to: "mouseOut"

                NumberAnimation {
                    target: icon
                    properties: "scale"
                    from: 1.1
                    to: 1
                    duration: 100
                    easing.type: Easing.InCubic
                }
            }
        ]

        visible: showIcon
    }

    Text {
        visible: false
        id: popupTextComp
        text: popupText
    }

    TextMetrics {
            id:     popupMetrics
            font:   popupTextComp.font
            text:   popupText
    }

    MouseArea {
        id: iconArea
        anchors.fill: icon
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked: {
            if (mouse.button === Qt.LeftButton)
                dockItem.clicked();
            else if (mouse.button === Qt.RightButton)
                dockItem.rightClicked()
        }

        onEntered: {
            icon.state = "mouseIn"
            popupTips.popup(dockItem.mapToGlobal((-popupMetrics.boundingRect.width + popupTips.getPadding() * 2) / 2, 0), popupText)
        }

        onExited: {
            icon.state = "mouseOut"
            popupTips.hide()
        }
    }

    Rectangle {
        z: -9
        id: activeDot
        anchors.centerIn: parent
        width: parent.width * 0.95
        height: parent.height * 0.95
        opacity: isActive ? 0.6 : 0.3
        radius: 3

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: Qt.rgba(255, 255, 255, 0.3)
            }
            GradientStop {
                position: 1.0
                color: "white"
            }
        }

        border.color: "white"
        border.width: 2

        visible: enableActivateDot
    }
}
