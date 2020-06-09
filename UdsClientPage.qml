import QtQuick 2.0
import QtQuick.Controls 2.14
import QtGraphicalEffects 1.12
import "CustomComponents"

Item {
    id: root

    property int currentTx

    Connections {
        target: viewController
        onSetSupportedDtcs: {
            listView.model.clear();
            console.log("AAAAAAAAAAAAAA");
            for (var i = 0; i < keys.length; i++) {
                console.log(keys[i]);
                listView.model.append({ "key": keys[i], "value": values[i] });
            }
        }
    }

    Label {
        id: titleLabel
        text: "DTCs"
        font.bold: true
        font.pointSize: 14

        anchors {
            top: parent.top
            topMargin: 10
            left: parent.left
            leftMargin: 10
        }
    }

    ListView {
        id: listView
        anchors {
            top: titleLabel.bottom
            topMargin: 10
            bottom: buttonBackground.top
            bottomMargin: 10
            right: parent.right
            left: parent.left
        }

        clip: true
        spacing: 1

        delegate: listViewDelegate
        model: ListModel { }

        Component {
            id: listViewDelegate

            Rectangle {
                height: 44
                width: parent.width
                color: "#f2f2f2"

                Row {
                    id: row
                    anchors.fill: parent

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width * 0.5
                        font.pointSize: 12
                        text: model.key
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width * 0.5
                        font.pointSize: 12
                        text: model.value
                    }
                }
            }
        }
    }

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
                viewController.udsCheckVersion(currentTx)
            }
        }

        Button {
            id: dtcStatusButton
            width: grid.columnWidth
            text: "Get status of supported DTCs"
            onClicked: {
                viewController.udsGetSupportedDtcsStatus(currentTx)
            }
        }

        Button {
            id: clearDtcButton
            width: grid.columnWidth
            text: "Clear DTC infomration"
            onClicked: {
                viewController.udsClearDtcInformation(currentTx)
            }
        }

        Button {
            width: grid.columnWidth
            text: "Placeholder"
        }
    }
}
