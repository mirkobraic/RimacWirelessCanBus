import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.12

import com.RimacWirelessCanBus 1.0

Window {
    id: mainWindow
    visible: true
    width: 480
    height: 720

    property int screenMargin: 10

   Item {
        id: headerContainer
        anchors.top: parent.top
        anchors.topMargin: screenMargin
        anchors.right: parent.right
        anchors.rightMargin: screenMargin
        anchors.left: parent.left
        anchors.leftMargin: screenMargin
        height: 40

        Image {
            id: logoImgae
            anchors.left: parent.left
            anchors.verticalCenter: connectionButtonsRow.verticalCenter
            fillMode: Image.PreserveAspectFit
            height: 40
            source: "images/rimacLogo.png"
        }

        Row {
            id: connectionButtonsRow
            anchors.right: parent.right
            spacing: 10

            Text {
                id: connectionStatusText
                anchors.bottom: connectButton.bottom
                anchors.bottomMargin: 0
                text: {
                    switch (canBusManager.connectionStatus) {
                    case ConnectionStatus.NotConnected:
                        return "Not connected"
                    case ConnectionStatus.Connected:
                        return "Connected"
                    default:
                        return "Error"
                    }
                }
                opacity: 0.6
                font.pointSize: 9
            }

            Button {
                id: connectButton
                text: "Connect"
                onClicked: canBusManager.connectTapped()
                enabled: canBusManager.connectionStatus === ConnectionStatus.NotConnected
            }

            Button {
                id: disconnectButton
                text: "Disconnect"
                onClicked: canBusManager.disconnectTapped()
                enabled: canBusManager.connectionStatus === ConnectionStatus.Connected
            }
        }
   }

    ListView {
        id: canMessagesListView
        topMargin: 10
        bottomMargin: 10
        anchors.top: headerContainer.bottom
        anchors.topMargin: 10
        anchors.bottom: footerContainer.top
        anchors.bottomMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        clip: true
        spacing: 1

        delegate: listViewDelegate
        model: listModel

        Rectangle {
            anchors.fill: parent
            color: "#e6e6e6"
            z: -1
        }

        Component {
            id: listViewDelegate

            Rectangle {
                height: 44
                width: parent.width
                color: "#fbfbfb"

                Row {
                    anchors.fill: parent
                    anchors.leftMargin: screenMargin

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width * 0.3
                        font.pointSize: 12
                        text: model.canId
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width * 0.7
                        font.pointSize: 12
                        text: model.data
                    }
                }
            }
        }

        ListModel {
            id: listModel
            ListElement {
                canId: "example id"
                data: "example data"
            }
        }

        Connections {
            target: canBusManager
            onAddMessage: {
                const message = {'canId': id, "data": data};
                listModel.append(message)
                canMessagesListView.currentIndex = listModel.count - 1
            }
        }
    }

    Item {
        id: footerContainer
        anchors.bottom: parent.bottom
        anchors.bottomMargin: screenMargin
        anchors.right: parent.right
        anchors.rightMargin: screenMargin
        anchors.left: parent.left
        anchors.leftMargin: screenMargin
        height: 40

        TextField {
            id: canIdTextField
            width: parent.width * 0.2
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            placeholderText: "ID"

            onFocusChanged: {
                color = "black"
            }
        }

        TextField {
            id: canDataTextField
            anchors.left: canIdTextField.right
            anchors.leftMargin: 10
            anchors.right: sendButton.left
            anchors.rightMargin: 10
            anchors.verticalCenterOffset: 0
            anchors.verticalCenter: parent.verticalCenter
            placeholderText: "data"

            onFocusChanged: {
                color = "black"
            }
        }

        Button {
            id: sendButton
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            text: "Send"
            enabled: canBusManager.connectionStatus === ConnectionStatus.Connected
            onClicked: canBusManager.sendTapped(canIdTextField.text, canDataTextField.text);
        }

        Connections {
            target: canBusManager
            onInvalidCanId: {
                canIdTextField.color = "red";
            }

            onInvalidCanData: {
                canDataTextField.color = "red";
            }
        }
    }
}

/*##^##
Designer {
    D{i:10;anchors_x:147}D{i:9;anchors_x:"-420"}D{i:16;anchors_x:147}D{i:17;anchors_x:147}
D{i:15;anchors_x:"-420"}
}
##^##*/
