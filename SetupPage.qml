import QtQuick 2.0
import QtQuick.Controls 2.14
import "CustomComponents"

Item {
    id: root

    property int currentTx: rxTxComboBox.model.get(rxTxComboBox.currentIndex).tx
    property int currentRx: rxTxComboBox.model.get(rxTxComboBox.currentIndex).rx

    RxTxPopup {
        id: rxTxPopup
        anchors.centerIn: root

        onRxTxPairAdded: {
            rxTxComboBox.model.append({ rx: rx, tx: tx, desc: "rx: " + rx + "  tx: " + tx });
            rxTxComboBox.currentIndex = rxTxComboBox.model.count - 1
        }
    }

    Flickable {
        id: flickable
        flickableDirection: Flickable.VerticalFlick
        anchors {
            fill: parent
            rightMargin: 15
            leftMargin: 15
        }

        Image {
            id: logoImage
            height: 55
            fillMode: Image.PreserveAspectFit
            source: "images/rimacLogo.png"

            anchors {
                left: parent.left
                top: parent.top
                topMargin: 30
            }
        }

        Column {
            id: column
            spacing: 15

            anchors {
                right: parent.right
                rightMargin: 0
                left: parent.left
                leftMargin: 0
                top: logoImage.bottom
                topMargin: 50
            }

            property int rowHeight: 44
            property int rowSpacing: 20
            property int firstColumnWidth: width * 0.3
            property int secondColumnWidth: width - firstColumnWidth - rowSpacing

            // first row
            Row {
                height: column.rowHeight
                spacing: column.rowSpacing
                Label {
                    width: column.firstColumnWidth
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    text: "CAN Provider"
                }
                ComboBox {
                    id: providerComboBox
                    width: column.secondColumnWidth

                    enabled: !viewController.isConnected
                    textRole: "text"
                    indicator: Canvas { }
                    model: ListModel {
                        ListElement { text: "Kvaser"; rawValue: 0 }
                        ListElement { text: "Wiicom"; rawValue: 1 }
                    }
                }
            }

            // second row
            Row {
                height: column.rowHeight
                spacing: column.rowSpacing
                enabled: providerComboBox.model.get(providerComboBox.currentIndex).rawValue === 0
                Label {
                    width: column.firstColumnWidth
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    text: "Baud rate"
                }
                ComboBox {
                    id: baudRateComboBox
                    width: column.secondColumnWidth

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
            }

            // third row
            Row {
                height: column.rowHeight
                spacing: column.rowSpacing
                Label {
                    width: column.firstColumnWidth
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    text: "Rx-Tx pairs"
                }
                RxTxPicker {
                    id: rxTxComboBox
                    width: column.secondColumnWidth
                    height: parent.height

                    model: ListModel {
                        ListElement { rx: 0; tx: 1; desc: "rx: 0  tx: 1" }
                        ListElement { rx: 1; tx: 2; desc: "rx: 1  tx: 2" }
                    }

                    onAddButtonClicked: rxTxPopup.open()
                }
            }

            // fourth row
            Row {
                height: column.rowHeight
                spacing: column.rowSpacing
                Label {
                    width: column.firstColumnWidth
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    text: "IP Address"
                }
                TextField {
                    id: ipAddressTextField
                    width: column.secondColumnWidth
                    enabled: !viewController.isConnected

                    text: providerComboBox.model.get(providerComboBox.currentIndex).rawValue === 0 ? "172.20.10.3" : "172.20.10.5"
                    placeholderText: "172.20.10.5"

                    Keys.onReturnPressed: {
                        portTextField.focus = true;
                    }
                }
            }

            // fifth row
            Row {
                height: column.rowHeight
                spacing: column.rowSpacing
                Label {
                    width: column.firstColumnWidth
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    text: "Port"
                }
                TextField {
                    id: portTextField
                    width: column.secondColumnWidth
                    enabled: !viewController.isConnected

                    text: providerComboBox.model.get(providerComboBox.currentIndex).rawValue === 0 ? "8080" : "65300"
                    placeholderText: "65300"

                    Keys.onReturnPressed: {
                        focus = false;
                    }
                }
            }
        }

        Button {
            id: connectToggle
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: column.bottom
                topMargin: 60

            }
            width: parent.width * 0.8
            height: 44

            background: Rectangle {
                radius: 6
                color: {
                    if (enabled)
                        return viewController.isConnected ? "#FC3D39" : "#147EFB"
                    else
                        return viewController.isConnected ? Qt.lighter("#FC3D39") : Qt.lighter("#147EFB")
                }
            }

            contentItem: Text {
                text: viewController.isConnected ? "Disconnect" : "Connect"
                font {
                    bold: true
                    pointSize: 14
                    preferShaping: true
                }

                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            enabled: {
                if (viewController.isConnected) {
                    return true
                } else {
                    return rxTxComboBox.model.count && providerComboBox.model.count && ipAddressTextField.text && portTextField.text
                }
            }

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

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
