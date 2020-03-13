import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.12

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
                text: "Not connected"
                opacity: 0.6
                font.pointSize: 9
            }

            Button {
                id: connectButton
                text: "Connect"
            }

            Button {
                id: disconnectButton
                text: "Disconnect"
                enabled: false
            }
        }
   }

    ListView {
        id: canMessagesListView
        topMargin: 10
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
            color: "#e0e0e0"
            z: -1
        }

        Component {
            id: listViewDelegate

            Rectangle {
                height: 44
                width: parent.width
                color: "white"

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
                        text: model.message
                    }
                }
            }
        }

        ListModel {
            id: listModel
            ListElement {
                canId: "example id"
                message: "example messages"
            }
            ListElement {
                canId: "0xBC1"
                message: "0xff43daab34"
            }
            ListElement {
                canId: "0101010101"
                message: "0101011010100010"
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
            width: parent.width * 0.3
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            placeholderText: "ID"
        }

        TextField {
            id: canMessageTextField
            anchors.left: canIdTextField.right
            anchors.leftMargin: 10
            anchors.right: sendButton.left
            anchors.rightMargin: 10
            anchors.verticalCenterOffset: 0
            anchors.verticalCenter: parent.verticalCenter
            placeholderText: "data"
        }

        Button {
            id: sendButton
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            text: "Send"
        }
    }
}

/*##^##
Designer {
    D{i:10;anchors_x:147}D{i:9;anchors_x:"-420"}D{i:16;anchors_x:147}D{i:17;anchors_x:147}
D{i:15;anchors_x:"-420"}
}
##^##*/
