import QtQuick 2.15
import QtQuick.Controls 2.14

Item {
    id: root

    signal clicked()
    property int iconSize: 50
    property alias iconSource: icon.source
    property alias text: text.text

    Button {
        id: udsButton
        anchors.fill: parent

        onClicked: {
            root.clicked();
        }

        background: Rectangle {
            color: "white"
            radius: 12
        }

        contentItem: Rectangle {
            anchors.fill: parent
            color: "white"
            radius: 12

            Text {
                id: text
                anchors {
                    bottom: parent.bottom
                    bottomMargin: 10
                    left: parent.left
                    leftMargin: 10
                }
                font {
                    bold: true
                    pointSize: 16
                }
            }
            Image {
                id: icon
                height: root.iconSize
                width: root.iconSize
                anchors {
                    right: parent.right
                    rightMargin: 10
                    top: parent.top
                    topMargin: 10
                }

                fillMode: Image.PreserveAspectFit
                mipmap: true
            }
        }
    }

}
