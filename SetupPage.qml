import QtQuick 2.15
import QtQuick.Controls 2.14
import "CustomComponents"

Item {
    id: root

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
            source: "images/fesbRacingLogo.png"

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

                    textRole: "text"
                    indicator: Canvas { }
                    model: ListModel {
                        ListElement { text: "Kvaser"; rawValue: 0 }
                        ListElement { text: "Wiicom"; rawValue: 1 }
                    }

                    currentIndex: settingsManager.defaultProvider
                    onCurrentIndexChanged: {
                        settingsManager.defaultProvider = currentIndex
                        ipAddressTextField.text = settingsManager.defaultIpAddress
                        portTextField.text = settingsManager.defaultPort
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
                    text: "Baudrate"
                }
                ComboBox {
                    id: baudRateComboBox
                    width: column.secondColumnWidth

                    textRole: "text"
                    indicator: Canvas { }
                    model: ListModel {
                        ListElement { text: "125k"; rawValue: 0 }
                        ListElement { text: "250k"; rawValue: 1 }
                        ListElement { text: "500k"; rawValue: 2 }
                        ListElement { text: "1000k"; rawValue: 3 }
                    }

                    currentIndex: settingsManager.defaultBaudrate
                    onCurrentIndexChanged: {
                        settingsManager.defaultBaudrate = baudRateComboBox.currentIndex
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
                    text: "RX-TX ID's (hex)"
                }
                Row {
                    height: column.rowHeight
                    width: column.secondColumnWidth
                    spacing: 20

                    TextField {
                        id: rxTextField
                        width: parent.width / 2 - 10
                        font.capitalization: Font.AllUppercase

                        text: settingsManager.rx
                        horizontalAlignment: Text.AlignHCenter
                        placeholderText: "rx"

                        Keys.onReturnPressed: {
                            focus = false
                        }
                        onFocusChanged:  {
                            settingsManager.rx = rxTextField.text
                        }
                    }
                    TextField {
                        id: txTextField
                        width: parent.width / 2 - 10
                        font.capitalization: Font.AllUppercase

                        text: settingsManager.tx
                        horizontalAlignment: Text.AlignHCenter
                        placeholderText: "tx"

                        Keys.onReturnPressed: {
                            focus = false
                            settingsManager.tx = txTextField.text
                        }
                        onFocusChanged:  {
                            settingsManager.tx = txTextField.text
                        }
                    }
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

                    text: settingsManager.defaultIpAddress
                    placeholderText: providerComboBox.model.get(providerComboBox.currentIndex).rawValue === 0 ? "172.20.10.3" : "172.20.10.5"

                    Keys.onReturnPressed: {
                        focus = false
                        settingsManager.defaultIpAddress = ipAddressTextField.text
                    }
                    onFocusChanged:  {
                        settingsManager.defaultIpAddress = ipAddressTextField.text
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

                    text: settingsManager.defaultPort
                    placeholderText: providerComboBox.model.get(providerComboBox.currentIndex).rawValue === 0 ? "8080" : "65300"

                    Keys.onReturnPressed: {
                        focus = false
                    }
                    onFocusChanged:  {
                        settingsManager.defaultPort = portTextField.text
                    }
                }
            }
        }

        Button {
            id: connectButton
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: column.bottom
                topMargin: 60

            }
            width: parent.width * 0.8
            height: 44

            background: Rectangle {
                radius: 6
                color: enabled ? "#147EFB" : Qt.lighter("#147EFB")
            }

            contentItem: Text {
                text: "Connect"
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
                return rxTextField.text && txTextField.text && providerComboBox.model.count && ipAddressTextField.text && portTextField.text
            }

            onClicked: {
                let currentProvider = providerComboBox.model.get(providerComboBox.currentIndex).rawValue;
                let ipAddress = ipAddressTextField.text;
                let port = portTextField.text;
                let baudRate = baudRateComboBox.model.get(baudRateComboBox.currentIndex).rawValue;

                let rxTxPair = { rx: rxTextField.text, tx: txTextField.text }

                viewController.connectTapped(currentProvider, ipAddress, port, baudRate, rxTxPair);
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
