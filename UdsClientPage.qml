import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import "CustomComponents"

Item {
    id: root

    property int currentTx

    Column {
        id: footerColumn
        anchors {
            bottom: parent.bottom
            bottomMargin: 10
            right: parent.right
            rightMargin: 10
            left: parent.left
            leftMargin: 10
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
                    viewController.checkVersion(currentTx)
                }
            }

            Button {

                enabled: viewController.isConnected
                text: "Some other functionality"
            }
        }
    }
}
