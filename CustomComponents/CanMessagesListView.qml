import QtQuick 2.0

Item {
    id: root

    property alias topMargin: listView.topMargin
    property alias bottomMargin: listView.bottomMargin
    property int contentInset: 10

    ListView {
        id: listView
        anchors.fill: parent

        clip: true
        spacing: 1

        onCountChanged: {
            if (!dragging) {
                currentIndex = count - 1
            }
        }

        delegate: listViewDelegate
        model: recievedMessages
        // TODO: maybe replace this with viewController.recievedMessages

        Rectangle {
            anchors.fill: parent
            color: "#e6e6e6"
            z: -1
        }

        Component {
            id: listViewDelegate

            Rectangle {
                height: 44
                width: parent.width
                color: "#fbfbfb"

                Row {
                    id: row
                    anchors.fill: parent
                    anchors.leftMargin: contentInset
                    anchors.rightMargin: contentInset

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width * 0.3
                        font.pointSize: 12
                        text: model.canId
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width * 0.7
                        font.pointSize: 12
                        text: model.canData
                    }
                }
            }
        }
    }
}
