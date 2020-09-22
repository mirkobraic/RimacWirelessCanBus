import QtQuick 2.0
import QtQuick.Controls 2.14
import QtGraphicalEffects 1.12
import "CustomComponents"

Item {
    id: root
    property string title: "UDS Client"

    UdsDtcListView {
        id: udsDtcsListView
        anchors {
            top: parent.top
            bottom: buttonBackground.top
            right: parent.right
            left: parent.left
        }
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

        property real columnWidth: (width - 5) / 2

        Button {
            id: dtcStatusButton
            height: 44
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
            height: 44
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
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
