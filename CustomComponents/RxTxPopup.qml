import QtQuick 2.0
import QtQuick.Controls 2.14

Item {
    id: root

    function open() {
        popup.open();
    }

    signal rxTxPairAdded(int rx, int tx)

    Popup {
        id: popup
        anchors.centerIn: parent
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
        onClosed: {
            rxTextField.text = ""
            txTextField.text = ""
        }

        Column {
            anchors.topMargin: 10
            spacing: 15

            Text {
                text: "Enter rx - tx pair:"
            }
            TextField {
                id: rxTextField
                placeholderText: "Rx"
                validator: IntValidator { bottom: 0; top: 2147483647 }
                onTextChanged: {
                    color = "black"
                }
            }
            TextField {
                id: txTextField
                placeholderText: "Tx"
                validator: IntValidator { bottom: 0; top: 2147483647 }
                onTextChanged: {
                    color = "black"
                }
            }
            Row {
                spacing: 5
                width: rxTextField.width
                height: 40

                Button {
                    height: parent.height
                    width: (parent.width - 5) / 2
                    text: "Cancel"
                    onClicked: {
                        popup.close();
                    }
                }
                Button {
                    height: parent.height
                    width: (parent.width - 5) / 2
                    text: "Submit"
                    onClicked: {
                        if (rxTextField.text.length > 0 && txTextField.text.length > 0) {
                            let rx = Number(rxTextField.text);
                            let tx = Number(txTextField.text);
                            root.rxTxPairAdded(rx, tx);
                            popup.close();
                        } else {
                            if (rxTextField.text.length === 0) {
                                rxTextField.color = "red";
                            }
                            if (txTextField.text.length === 0) {
                                txTextField.color = "red";
                            }
                        }
                    }
                }
            }
        }
    }
}
