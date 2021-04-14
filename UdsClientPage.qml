import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
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
        topMargin: 10
        bottomMargin: 10
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
            leftMargin: 15
            bottomMargin: 10
            rightMargin: 15
        }
        columns: 2
        columnSpacing: 15
        rowSpacing: 15

        property real columnWidth: (width - 15) / 2

        Button {
            id: dtcStatusButton
            width: grid.columnWidth
            text: "Get DTC status"
            onClicked: {
                viewController.udsGetSupportedDtcsStatus()
            }
        }

        Button {
            id: clearDtcButton
            width: grid.columnWidth
            text: "Clear DTC info"
            onClicked: {
                viewController.udsClearDtcInformation()
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
