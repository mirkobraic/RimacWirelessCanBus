import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.12

Window {
    id: mainWindow
    visible: true
    width: 480
    height: 720

    minimumWidth: 400
    minimumHeight: 600

    readonly property int screenMargin: 10

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
                text: viewController.isConnected ? "Connected" : "Not connected"
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
                text: viewController.isConnected ? "Disconnect" : "Connect"
                onClicked: viewController.isConnected ? viewController.disconnectTapped() : viewController.connectTapped()
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

        onCountChanged: {
            if (!dragging) {
                currentIndex = count - 1
            }
        }

        delegate: listViewDelegate
        model: recievedMessages

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
                        text: model.canData
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

            function adjustIdToLength(len) {
                while (text.length < len) {
                    text = '0' + text;
                }
                while (text.length > len) {
                    text = text.substring(1);
                }
            }

            function setExtId() {
                adjustIdToLength(8)
                // highest extended can id is 0x1fffffff
                if (text.charAt(0) > '1') {
                    text = "1FFFFFFF";
                }
            }

            function setStdId() {
                adjustIdToLength(3);
            }

            function formatId() {
                // highest standard can id is 0x7ff
                if (parseInt(text, 16) > 0x7FF) {
                    setExtId();
                } else {
                    setStdId();
                }
            }

            onEditingFinished: {
                if (text.length > 0) {
                    formatId();
                }
            }
            onTextChanged: {
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
                    onTextChanged: {
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
            enabled: viewController.isConnected
            onClicked: {
                canDataTextFields.checkTextFields();
                let data = [];
                for (let i = 0; i < canDataTextFields.count; i++) {
                    if (canDataTextFields.itemAt(i).text) {
                        data.push(canDataTextFields.itemAt(i).text);
                    }
                }

                if (canIdTextField.text === "") {
                    canIdTextField.color = "red";
                }
                if (data.length === 0) {
                    canDataTextFields.itemAt(0).color = "red";
                }

                viewController.sendTapped(canIdTextField.text, data);
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
