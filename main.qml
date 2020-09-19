import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Window 2.12
import "CustomComponents"

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 480
    height: 720

    minimumWidth: 400
    minimumHeight: 600

    readonly property int screenMargin: 10

    Connections {
        target: viewController
        function onShowAlert(title, message) {
            alertPopup.open(title, message)
        }
    }

    AlertPopup {
        id: alertPopup
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.4
    }

    MBBusyIndicator {
        anchors.fill: parent
        z: 1
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        SetupPage {
            id: setupPage
        }

        UdsClientPage {
            id: udsClientPage
        }

        RawCanPage {
            id: rawCanPage
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        property color backgroundColor: "#fecb2e"
        property color selectedColor: "#303030"

        TabButton {
            text: qsTr("Setup")
            Rectangle {
                anchors.fill: parent
                color: parent.checked ? tabBar.backgroundColor : tabBar.selectedColor
            }
        }
        TabButton {
            text: qsTr("UDS Client")
            Rectangle {
                anchors.fill: parent
                color: parent.checked ? tabBar.backgroundColor : tabBar.selectedColor
            }
        }
        TabButton {
            text: qsTr("Raw CAN")
            Rectangle {
                anchors.fill: parent
                color: parent.checked ? tabBar.backgroundColor : tabBar.selectedColor
            }
        }
    }
}

/*##^##
Designer {
    D{i:9;anchors_x:"-420"}D{i:10;anchors_x:147}D{i:8;anchors_x:"-420"}D{i:12;anchors_x:"-420"}
D{i:13;anchors_x:147}D{i:14;anchors_x:"-420"}D{i:15;anchors_x:"-420"}D{i:19;anchors_x:147}
D{i:20;anchors_x:147}D{i:21;anchors_x:"-420"}D{i:18;anchors_x:"-420"}D{i:22;anchors_x:147}
D{i:17;anchors_x:147}D{i:16;anchors_x:147}
}
##^##*/
