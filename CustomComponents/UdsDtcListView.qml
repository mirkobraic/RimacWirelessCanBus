import QtQuick 2.15
import QtQuick.Controls 2.14

Item {
    id: root

    property alias topMargin: listView.topMargin
    property alias bottomMargin: listView.bottomMargin
    property int contentInset: 10

    function isStatusPassed(status) {
        let binaryMask = 0b00001111;
        return (binaryMask & status) === 0;
    }

    Connections {
        target: viewController
        function onSetSupportedDtcs(masks, statuses) {
            listView.model.clear();
            for (var i = 0; i < masks.length; i++) {
                let hexMask = masks[i].toString(16).toUpperCase();
                listView.model.append({ "mask": "0x" + hexMask, "status": statuses[i] });
            }
        }

        function onClearSupportedDtcs() {
            listView.model.clear()
        }
    }

    Row {
        id: titleRow
        height: 19
        anchors {
            top: parent.top
            topMargin: 10
            left: parent.left
            leftMargin: 10
            right: parent.right
            rightMargin: 10
        }

        Text {
            id: maskText
            text: "Mask"
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            font.bold: true
            font.pointSize: 16
            width: parent.width * 0.5
        }

        Text {
            id: statusText
            text: "Status"
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            font.bold: true
            font.pointSize: 16
            width: parent.width * 0.5
        }
    }

    ListView {
        Rectangle {
            anchors.fill: parent
            color: "#EFEFF4"
            z: -1
        }

        id: listView
        anchors {
            top: titleRow.bottom
            topMargin: 10
            bottom: parent.bottom
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
                width: listView.width
                color: "white"

                Row {
                    id: row
                    anchors.fill: parent
                    anchors.leftMargin: root.contentInset
                    anchors.rightMargin: root.contentInset

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width * 0.5
                        font.pointSize: 14
                        text: model.mask
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width * 0.5
                        font.pointSize: 14
                        font.bold: true
                        text: model.status
                        color: isStatusPassed(model.status) ? "green" : "red"
                    }
                }
            }
        }
    }
}
