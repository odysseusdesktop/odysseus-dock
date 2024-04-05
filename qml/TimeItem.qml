import QtQuick 2.4

Item {
    width: metrics.width
    TextMetrics {
        id: metrics
        font: timeText.font
        text: timeText.text
    }

    Text {
        id: timeText
        font.pixelSize: 22
        anchors.centerIn: parent
    }

    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: timeText.text = new Date().toTimeString()
    }
}
