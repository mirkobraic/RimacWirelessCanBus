import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.12
import QtQuick.Layouts 1.14
import "CustomComponents"

Window {
    id: mainWindow
    visible: true
    width: 480
    height: 720

    minimumWidth: 400
    minimumHeight: 600

    readonly property int screenMargin: 10

    Connections {
        target: viewController
        onShowAlert: alertPopup.open(title, message)
    }

    RxTxPopup {
        id: rxTxPopup
        anchors.centerIn: parent

        onRxTxPairAdded: {
            rxTxComboBox.model.append({ rx: rx, tx: tx, desc: "rx: " + rx + "  tx: " + tx });
            rxTxComboBox.currentIndex = rxTxComboBox.model.count - 1
        }
    }

    AlertPopup {
        id: alertPopup
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.5
    }

    Rectangle {
        anchors.fill: parent
        enabled: busyIndicator.running
        visible: busyIndicator.running
        color: "#11000000"
        z: 1

        MouseArea {
            anchors.fill: parent
        }

        BusyIndicator {
            id: busyIndicator
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            running: viewController.fetchingInProgress
        }
    }

    Item {
        id: headerContainer
        height: headerColumn.height
        anchors {
            top: parent.top
            topMargin: screenMargin
            right: parent.right
            rightMargin: screenMargin
            left: parent.left
            leftMargin: screenMargin
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

    CanMessagesListView {
        topMargin: 10
        bottomMargin: 10
        contentInset: screenMargin
        anchors {
            top: headerContainer.bottom
            topMargin: 10
            bottom: footerContainer.top
            bottomMargin: 10
            right: parent.right
            rightMargin: 0
            left: parent.left
            leftMargin: 0
        }
    }

    Item {
        id: footerContainer
        height: footerColumn.height
        anchors {
            bottom: parent.bottom
            bottomMargin: screenMargin
            right: parent.right
            rightMargin: screenMargin
            left: parent.left
            leftMargin: screenMargin
        }

        Column {
            id: footerColumn
            anchors {
                right: parent.right
                left: parent.left
            }
            spacing: 10

            RowLayout {
                spacing: 10
                width: parent.width
                Button {
                    id: checkVersionButton
                    text: "Check version"
                    enabled: viewController.isConnected
                    onClicked: {
                        let tx = rxTxComboBox.model.get(rxTxComboBox.currentIndex).tx
                        viewController.checkVersion(tx)
                    }
                }
                Button {

                    enabled: viewController.isConnected
                    text: "Some other functionality"
                }

                Switch {
                    id: rawCanSwitch
                    Layout.alignment: Qt.AlignRight
                    text: "Raw CAN"
                    onToggled: viewController.isRawCanEnabled = (rawCanSwitch.position > 0.5)
                }
            }

            SendCanMessageField {
                height: 40
                anchors {
                    right: parent.right
                    left: parent.left
                }
                enabled: viewController.isRawCanEnabled
            }
        }
    }
}

/*##^##
Designer {
    D{i:9;anchors_x:"-420"}D{i:10;anchors_x:147}D{i:8;anchors_x:"-420"}D{i:12;anchors_x:"-420"}
D{i:13;anchors_x:147}D{i:14;anchors_x:"-420"}D{i:15;anchors_x:"-420"}D{i:19;anchors_x:147}
D{i:20;anchors_x:147}D{i:21;anchors_x:"-420"}D{i:18;anchors_x:"-420"}D{i:22;anchors_x:147}
D{i:17;anchors_x:147}D{i:16;anchors_x:147}
}
##^##*/
