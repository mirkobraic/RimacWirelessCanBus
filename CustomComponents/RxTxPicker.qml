import QtQuick 2.0
import QtQuick.Controls 2.14

Item {
    id: root

    property alias model: comboBox.model
    property alias currentIndex: comboBox.currentIndex

    signal addButtonClicked()
    enabled: !viewController.isConnected

    ComboBox {
        id: comboBox
        width: parent.width - parent.height - 1
        height: 44

        textRole: "desc"
        indicator: Canvas { }

        delegate: SwipeDelegate {
            id: swipeDelegate
            width: parent.width
            text:  "rx: " + model.rx + "  tx: " + model.tx

            swipe.right: Rectangle {
                width: parent.width
                height: parent.height
                clip: true
                color: Qt.darker("tomato", 1.1)

                Label {
                    padding: 20
                    anchors.fill: parent

                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    color: "white"
                    text: "Delete"
                    opacity: 2 * -swipeDelegate.swipe.position
                }
                SwipeDelegate.onClicked: swipeDelegate.swipe.close()
            }

            swipe.onCompleted: {
                if (comboBox.count == index + 1) {
                    comboBox.currentIndex = index - 1;
                }

                comboBox.model.remove(index)
            }

            ListView.onRemove: SequentialAnimation {
                PropertyAction {
                    target: swipeDelegate
                    property: "ListView.delayRemove"
                    value: true
                }
                NumberAnimation {
                    target: swipeDelegate
                    property: "height"
                    to: 0
                    easing.type: Easing.InOutQuad
                }
                PropertyAction {
                    target: swipeDelegate
                    property: "ListView.delayRemove"
                    value: false
                }
            }
        }
    }

    Button {
        id: addButton
        height: comboBox.height
        width: height
        leftPadding: 6
        bottomPadding: 10
        text: "+"
        font.bold: false
        font.pointSize: 20

        anchors {
            left: comboBox.right
            verticalCenter: comboBox.verticalCenter
            leftMargin: 1
        }
        onClicked: root.addButtonClicked()
    }
}
