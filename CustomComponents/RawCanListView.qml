import QtQuick 2.15
import QtQuick.Controls 2.14

Item {
    id: root

    property alias topMargin: listView.topMargin
    property alias bottomMargin: listView.bottomMargin
    property int contentInset: 10

    property var horizontalTextAlignment: Text.AlignLeft

    Component.onCompleted: {
        recievedMessages.toggleOverride(overrideButton.checked);
    }

    Label {
        id: titleLabel
        text: "Recieved messages"
        font.bold: true
        font.pointSize: 16

        anchors {
            top: parent.top
            topMargin: 10
            left: parent.left
            leftMargin: 10
        }
    }

    Button {
        id: button
        anchors {
            verticalCenter: overrideButton.verticalCenter
            right: overrideButton.left
            rightMargin: 10
        }

        height: 40
        width: 40

        background: Rectangle {
            color: "transparent"
        }

        contentItem: Image {
            fillMode: Image.PreserveAspectFit
            source: "../images/clearIcon.png"
            mipmap: true
        }

        onClicked: {
            recievedMessages.clearMessages();
        }
    }

    Button {
        id: overrideButton
        anchors {
            verticalCenter: titleLabel.verticalCenter
            right: parent.right
            rightMargin: 10
        }

        checkable: true
        checked: settingsManager.rawCanOverride

        height: 48
        width: 48

        background: Rectangle {
            color: "transparent"
        }

        contentItem: Image {
            fillMode: Image.PreserveAspectFit
            source: overrideButton.checked ? "../images/overrideOn.png" : "../images/overrideOff.png"
            mipmap: true
        }

        onClicked: {
            recievedMessages.toggleOverride(checked);
            settingsManager.rawCanOverride = checked
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

        model: recievedMessages

        onCountChanged: {
            if (!dragging) {
                currentIndex = count - 1
            }
        }

        delegate: listViewDelegate

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
                        horizontalAlignment: horizontalTextAlignment
                        width: parent.width * 0.3
                        font.pointSize: 12
                        text: model.CanId
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: horizontalTextAlignment
                        width: parent.width * 0.55
                        font.pointSize: 12
                        text: model.CanData
                    }
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: horizontalTextAlignment
                        width: parent.width * 0.15
                        font.pointSize: 10
                        text: model.Timestamp
                    }
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
