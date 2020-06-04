import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import "CustomComponents"

Item {
    id: root

    property int currentTx: rxTxComboBox.model.get(rxTxComboBox.currentIndex).tx
    property int currentRx: rxTxComboBox.model.get(rxTxComboBox.currentIndex).rx

    RxTxPopup {
        id: rxTxPopup
        anchors.centerIn: parent

        onRxTxPairAdded: {
            rxTxComboBox.model.append({ rx: rx, tx: tx, desc: "rx: " + rx + "  tx: " + tx });
            rxTxComboBox.currentIndex = rxTxComboBox.model.count - 1
        }
    }

    Column {
        id: headerColumn
        anchors {
            right: parent.right
            left: parent.left
        }
        spacing: 10

        RowLayout {
            width: parent.width
            spacing: 10
            clip: true

            ComboBox {
                id: providerComboBox
                Layout.fillWidth: true
                Layout.minimumWidth: 80
                Layout.maximumWidth: parent.width / 3

                enabled: !viewController.isConnected
                textRole: "text"
                indicator: Canvas { }
                model: ListModel {
                    ListElement { text: "Kvaser"; rawValue: 0 }
                    ListElement { text: "Wiicom"; rawValue: 1 }
                }
            }

            ComboBox {
                id: baudRateComboBox
                Layout.fillWidth: true
                Layout.minimumWidth: 80
                Layout.maximumWidth: parent.width / 3

                enabled: !viewController.isConnected
                textRole: "text"
                indicator: Canvas { }
                model: ListModel {
                    ListElement { text: "125k"; rawValue: 0 }
                    ListElement { text: "250k"; rawValue: 1 }
                    ListElement { text: "500k"; rawValue: 2 }
                    ListElement { text: "1000k"; rawValue: 3 }
                }
            }

            RxTxPicker {
                id: rxTxComboBox
                Layout.fillWidth: true
                Layout.minimumWidth: 130
                Layout.maximumWidth: parent.width / 3

                model: ListModel {
                    ListElement { rx: 0; tx: 1; desc: "rx: 0  tx: 1" }
                    ListElement { rx: 1; tx: 2; desc: "rx: 1  tx: 2" }
                }

                onAddButtonClicked: rxTxPopup.open()
            }
        }

        RowLayout {
            width: parent.width
            spacing: 10
            clip: true

            TextField {
                id: ipAddressTextField
                enabled: !viewController.isConnected
                Layout.fillWidth: true
                Layout.minimumWidth: 80
                Layout.maximumWidth: parent.width / 3

                text: "172.20.10.3"
                placeholderText: "IP Address"
            }

            TextField {
                id: portTextField
                enabled: !viewController.isConnected
                Layout.fillWidth: true
                Layout.minimumWidth: 80
                Layout.maximumWidth: parent.width / 3

                text: "8080"
                placeholderText: "Port"
            }

            Button {
                id: connectToggle
                Layout.fillWidth: true
                Layout.minimumWidth: 80
                Layout.maximumWidth: parent.width / 3
                Layout.alignment: Qt.AlignRight

                enabled: {
                    if (viewController.isConnected) {
                        return true
                    } else {
                        return rxTxComboBox.model.count && providerComboBox.model.count && ipAddressTextField.text && portTextField.text
                    }
                }

                text: viewController.isConnected ? "Disconnect" : "Connect"
                font.preferShaping: true
                onClicked: {
                    if (viewController.isConnected) {
                        viewController.disconnectTapped()
                    } else  {
                        let currentProvider = providerComboBox.model.get(providerComboBox.currentIndex).rawValue;
                        let ipAddress = ipAddressTextField.text;
                        let port = portTextField.text;
                        let baudRate = baudRateComboBox.model.get(baudRateComboBox.currentIndex).rawValue;
                        let rxTxPairs = [];
                        for (var i = 0; i < rxTxComboBox.model.count; i++) {
                            rxTxPairs.push({ rx: rxTxComboBox.model.get(i).rx, tx: rxTxComboBox.model.get(i).tx })
                        }

                        viewController.connectTapped(currentProvider, ipAddress, port, baudRate, rxTxPairs);
                    }
                }
            }
        }
    }
}
