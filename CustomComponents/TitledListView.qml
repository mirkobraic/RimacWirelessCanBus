import QtQuick 2.0
import QtQuick.Controls 2.14

Item {
    id: root

    property alias topMargin: listView.topMargin
    property alias bottomMargin: listView.bottomMargin
    property int contentInset: 10

    property string title: ""
    property real textWidthRatio: 0.5
    property var horizontalTextAlignment: Text.AlignLeft

    property alias model: listView.model

    Label {
        id: titleLabel
        text: root.title
        font.bold: true
        font.pointSize: 18

        anchors {
            top: parent.top
            topMargin: 10
            left: parent.left
            leftMargin: 10
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
            top: titleLabel.bottom
            topMargin: 10
            bottom: parent.bottom
            right: parent.right
            left: parent.left
        }

        clip: true
        spacing: 1

        onCountChanged: {
            if (!dragging) {
                currentIndex = count - 1
            }
        }

        delegate: listViewDelegate
        model: ListModel { }

        Component {
            id: listViewDelegate

            Rectangle {
                height: 44
                width: parent.width
                color: "white"

                Row {
                    id: row
                    anchors.fill: parent
                    anchors.leftMargin: root.contentInset
                    anchors.rightMargin: root.contentInset

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: horizontalTextAlignment
                        width: parent.width * root.textWidthRatio
                        font.pointSize: 14
                        text: model.firstField
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: horizontalTextAlignment
                        width: parent.width * (1 - root.textWidthRatio)
                        font.pointSize: 14
                        text: model.secondField
                    }
                }
            }
        }
    }
}
