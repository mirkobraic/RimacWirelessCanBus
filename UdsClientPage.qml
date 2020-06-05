import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtGraphicalEffects 1.12
import "CustomComponents"

Item {
    id: root

    property int currentTx

    DropShadow {
        anchors.fill: buttonBackground
        radius: 5
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
            text: "Check version"
            onClicked: {
                viewController.checkVersion(currentTx)
            }
        }

        Button {
            id: dtcStatusButton
            width: grid.columnWidth
            text: "Get status of supported DTCs"
            onClicked: {
//                viewController.checkVersion(currentTx)
            }
        }

        Button {
            id: clearDtcButton
            width: grid.columnWidth
            text: "Clear DTC infomration"
            onClicked: {
//                viewController.checkVersion(currentTx)
            }
        }

        Button {
            width: grid.columnWidth
            text: "Some other funcionality"
        }
    }
}
