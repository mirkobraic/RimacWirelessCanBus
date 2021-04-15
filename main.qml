import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Window 2.0
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
        anchors.centerIn: parent
        width: parent.width * 0.4
    }

    MBBusyIndicator {
        anchors.fill: parent
        z: 1
    }

    SetupPage {
        anchors.fill: parent
        visible: !viewController.isConnected
        enabled: !viewController.isConnected
    }

    header: ToolBar {
        Rectangle {
            anchors.fill: parent
            z: -1
            color: "#191947"
        }

        visible: viewController.isConnected
        enabled: viewController.isConnected
        contentHeight: 30

        ToolButton {
            id: toolButton
            height: 34
            width: 44
            anchors.verticalCenter: parent.verticalCenter
            background: Rectangle {
                color: "#191947"
            }
            contentItem: Image {
                fillMode: Image.PreserveAspectFit
                source: "../images/backIcon.png"
                mipmap: true
            }
            enabled: stackView.depth > 1
            visible: stackView.depth > 1
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                }
            }
        }

        Label {
            text: stackView.currentItem.title
            color: "white"
            anchors.centerIn: parent
            font.pixelSize: 17
            font.bold: true
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: "MainMenu.qml"
        visible: viewController.isConnected
        enabled: viewController.isConnected
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
