import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.12

//import com.RimacWirelessCanBus 1.0

Window {
    id: mainWindow
    visible: true
    width: 480
    height: 720

    minimumWidth: 400
    minimumHeight: 600

    readonly property int screenMargin: 10

    function getExtId(id) {
        while (id.length < 8) {
            id = "0" + id;
        }
        // highest extended can id is 0x1fffffff
        if (id.charAt(0) > '1') {
            id = "1FFFFFFF";
        }
        return id;
    }

    function getStdId(id) {
        while (id.length < 3) {
            id = "0" + id;
        }
        return id;
    }

    function isExtended(id) {
        if (id.length > 3) {
            return true;
        }
        // highest standard can id is 0x7ff
        if (id.length === 3 && id.charAt(0) > '7') {
            return true;
        }
        return false;
    }

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
                anchors.bottom: chooseButton.bottom
                anchors.bottomMargin: 0
                text: canBusManager.isConnected ? "Connected" : "Not connected"
                opacity: 0.6
                font.pointSize: 9
            }

            Button {
                id: chooseButton
                // TODO: show list of available devices
                text: "Choose a device"
                enabled: false
            }

            Button {
                id: connectToggle
                text: canBusManager.isConnected ? "Disconnect" : "Connect"
                onClicked: canBusManager.isConnected ? canBusManager.disconnectTapped() : canBusManager.connectTapped()
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
                let formattedData = data.replace(/\w{2}(?=.)/g, '$& ').toUpperCase();
                let formattedId = getFormattedId(id);
                const message = {'canId': formattedId, "data": formattedData};
                listModel.append(message)
                canMessagesListView.currentIndex = listModel.count - 1
            }

            function getFormattedId(id) {
                let formatted = ""
                if (isExtended(id)) {
                    formatted = getExtId(id);
                } else {
                    formatted = getStdId(id);
                }
                return formatted.toUpperCase();
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
            width: parent.width * 0.18
            font.capitalization: Font.AllUppercase
            horizontalAlignment: Text.AlignHCenter
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            placeholderText: "ID"
            validator: RegExpValidator { regExp: /[0-9A-Fa-f]{0,8}/ }

            onEditingFinished: {
                if (text.length == 0) {
                    return;
                }

                if (isExtended(text)) {
                    text = getExtId(text);
                } else {
                    text = getStdId(text);
                }
            }
            onFocusChanged: {
                color = "black"
            }
        }

        Row {
            id: canDataContainer
            anchors.left: canIdTextField.right
            anchors.leftMargin: 10
            anchors.right: sendButton.left
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            spacing: -1

            readonly property real textFieldWidth: width / 8 + 1

            Repeater {
                id: canDataTextFields
                model: 8

                function checkTextFields() {
                    let maxByte = 0;
                    let i = 0;
                    for (i = 0; i < count; i++) {
                        if (itemAt(i).text) {
                            maxByte = i;
                        }
                    }

                    for (i = 0; i < maxByte; i++) {
                        if (itemAt(i).text === "") {
                            itemAt(i).text = "00";
                        }
                    }
                }

                TextField {
                    placeholderText: index + 1
                    width: canDataContainer.textFieldWidth
                    padding: 1
                    font.capitalization: Font.AllUppercase
                    horizontalAlignment: Text.AlignHCenter
                    validator: RegExpValidator { regExp: /[0-9A-Fa-f]{0,2}/ }

                    onEditingFinished: {
                        if (text.length == 1) {
                            text = "0" + text;
                        }
                    }
                    onFocusChanged: {
                        color = "black"
                    }
                }
            }
        }

        Button {
            id: sendButton
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            text: "Send"
            enabled: canBusManager.isConnected
            onClicked: {
                canDataTextFields.checkTextFields();
                let data = "";
                for (let i = 0; i < canDataTextFields.count; i++) {
                    data += canDataTextFields.itemAt(i).text;
                }
                if (canIdTextField.text === "") {
                    canIdTextField.color = "red";
                }
                if (data === "") {
                    canDataTextFields.itemAt(0).color = "red";
                }

                canBusManager.sendTapped(canIdTextField.text, data);
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
