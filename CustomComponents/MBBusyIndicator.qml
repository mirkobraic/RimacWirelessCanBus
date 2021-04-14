import QtQuick 2.15
import QtQuick.Controls 2.14

Item {
    id: root

    Rectangle {
        anchors.fill: parent
        enabled: busyIndicator.running
        visible: busyIndicator.running
        color: "#11000000"

        MouseArea {
            anchors.fill: parent
        }

        BusyIndicator {
            id: busyIndicator
            anchors.centerIn: parent
            running: viewController.fetchingInProgress
        }
    }
}
