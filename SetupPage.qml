import QtQuick 2.0
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
                    text: "Baudrate"
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
                    text: "rx-tx pair (hex)"
                }
                Row {
                    height: column.rowHeight
                    width: column.secondColumnWidth
                    spacing: 20

                    TextField {
                        id: rxTextField
                        width: parent.width / 2 - 10
                        enabled: !viewController.isConnected
                        font.capitalization: Font.AllUppercase

                        text: ""
                        placeholderText: "rx"

                        Keys.onReturnPressed: {
                            focus = false
                        }
                    }
                    TextField {
                        id: txTextField
                        width: parent.width / 2 - 10
                        enabled: !viewController.isConnected
                        font.capitalization: Font.AllUppercase

                        text: ""
                        placeholderText: "tx"

                        Keys.onReturnPressed: {
                            focus = false
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
                    enabled: !viewController.isConnected

                    text: providerComboBox.model.get(providerComboBox.currentIndex).rawValue === 0 ? "172.20.10.3" : "172.20.10.5"
                    placeholderText: "172.20.10.5"

                    Keys.onReturnPressed: {
                        focus = false
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
                        focus = false
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
                    return rxTextField.text && txTextField.text && providerComboBox.model.count && ipAddressTextField.text && portTextField.text
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

                    let rxTxPair = { rx: rxTextField.text, tx: txTextField.text }

                    viewController.connectTapped(currentProvider, ipAddress, port, baudRate, rxTxPair);
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
