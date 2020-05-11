import QtQuick 2.0
import QtQuick.Controls 2.14

Item {
    TextField {
        id: canIdTextField
        width: parent.width * 0.19
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter

        font.capitalization: Font.AllUppercase
        horizontalAlignment: Text.AlignHCenter
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
        anchors {
            left: canIdTextField.right
            leftMargin: 10
            right: sendButton.left
            rightMargin: 10
            verticalCenter: parent.verticalCenter
        }
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
