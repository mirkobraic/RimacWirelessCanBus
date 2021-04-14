import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import "CustomComponents"

Item {
    id: root

    UdsDtcListView {
        id: udsDtcsListView
        anchors {
            top: parent.top
            topMargin: 10
            bottom: buttonBackground.top
            right: parent.right
            left: parent.left
        }
        topMargin: 10
        contentInset: 10
    }

    DropShadow {
        anchors.fill: buttonBackground
        radius: 2
        color: "gray"
        source: buttonBackground
    }

    Rectangle {
        id: buttonBackground
        anchors {
            right: root.right
            bottom: root.bottom
            left: root.left
            top: grid.top
            topMargin: -10
        }
    }

    Grid {
        id: grid
        anchors {
            right: parent.right
            bottom: parent.bottom
            left: parent.left
            leftMargin: 10
            bottomMargin: 10
            rightMargin: 10
        }
        columns: 2
        columnSpacing: 5
        rowSpacing: 5
        enabled: viewController.isConnected

        property real columnWidth: (width - 5) / 2

        Button {
            id: checkVersionButton
            width: grid.columnWidth
            onClicked: {
                viewController.udsCheckVersion()
            }

            background: Rectangle {
                color: "#e6e6e6"
                radius: 4
            }

            contentItem: Text {
                text: "Check version"
                font {
                    bold: false
                    pointSize: 14
                    preferShaping: true
                }

                color: enabled ? "black" : "#bdbdbd"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Button {
            id: dtcStatusButton
            width: grid.columnWidth
            onClicked: {
                viewController.udsGetSupportedDtcsStatus()
            }

            background: Rectangle {
                color: "#e6e6e6"
                radius: 4
            }

            contentItem: Text {
                text: "Get DTC status"
                font {
                    bold: false
                    pointSize: 14
                    preferShaping: true
                }

                color: enabled ? "black" : "#bdbdbd"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Button {
            id: clearDtcButton
            width: grid.columnWidth
            onClicked: {
                viewController.udsClearDtcInformation()
            }

            background: Rectangle {
                color: "#e6e6e6"
                radius: 4
            }

            contentItem: Text {
                text: "Clear DTC info"
                font {
                    bold: false
                    pointSize: 14
                    preferShaping: true
                }

                color: enabled ? "black" : "#bdbdbd"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Button {
            width: grid.columnWidth

            background: Rectangle {
                color: "#e6e6e6"
                radius: 4
            }

            contentItem: Text {
                text: "-"
                font {
                    bold: false
                    pointSize: 14
                    preferShaping: true
                }

                color: enabled ? "black" : "#bdbdbd"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
