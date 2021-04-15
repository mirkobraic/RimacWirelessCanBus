import QtQuick 2.15
import QtQuick.Controls 2.14
import "CustomComponents"

Item {
    id: root
    property string title: "FESB Racing CAN bus"

    Rectangle {
        anchors.fill: parent
        z: -1
        color: "#EFEFF4"
    }

    Image {
        anchors.fill: parent
        opacity: 0.03
        fillMode: Image.PreserveAspectFit
        source: "../images/rimacLogoBig.png"
    }

    Flickable {
        id: flickable
        flickableDirection: Flickable.VerticalFlick
        anchors {
            fill: parent
            rightMargin: 15
            leftMargin: 15
        }

        Grid {
            id: grid
            anchors {
                top: parent.top
                topMargin: 30
                horizontalCenter: parent.horizontalCenter
            }
            width: parent.width
            columns: 2
            columnSpacing: 15
            rowSpacing: 15

            property real cellWidth: (width - 15) / 2
            property real cellHeight: 95

            MainMenuButton {
                width: grid.cellWidth
                height: grid.cellHeight
                iconSize: 55
                text: "UDS Client"
                iconSource: "../images/checkCarIcon.png"

                onClicked: {
                     stackView.push("UdsClientPage.qml")
                }
            }

            MainMenuButton {
                width: grid.cellWidth
                height: grid.cellHeight
                iconSize: 45
                text: "Server info"
                iconSource: "../images/infoIcon.png"

                onClicked: {
                    viewController.udsCheckVersion()
                }
            }

            MainMenuButton {
                width: grid.cellWidth
                height: grid.cellHeight
                iconSize: 45
                text: "Raw CAN"
                iconSource: "../images/canBusIcon.png"

                onClicked: {
                    stackView.push("RawCanPage.qml")
                }
            }

            MainMenuButton {
                width: grid.cellWidth
                height: grid.cellHeight
                iconSize: 45
                text: "Control outputs"
                iconSource: "../images/controlIcon.png"

                onClicked: {
                    stackView.push("ControlOutputsPage.qml")
                }
            }

            MainMenuButton {
                width: grid.cellWidth
                height: grid.cellHeight
                iconSize: 45
                text: "Disconnect"
                iconSource: "../images/disconnectIcon.png"

                onClicked: {
                    viewController.disconnectTapped()
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
