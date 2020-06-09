import QtQuick 2.0
import QtQuick.Controls 2.14
import QtGraphicalEffects 1.12
import "CustomComponents"

Item {
    id: root

    TitledListView {
        anchors {
            top: parent.top
            topMargin: 10
            bottom: sendField.top
            bottomMargin: 10
            right: parent.right
            left: parent.left
        }
        topMargin: 10
        bottomMargin: 10
        contentInset: 10

        title: "Recieved messages (hex)"
        textWidthRatio: 0.3
        model: recievedMessages
    }

    DropShadow {
        anchors.fill: sendFieldBackground
        radius: 5
        color: "gray"
        source: sendFieldBackground
    }

    Rectangle {
        id: sendFieldBackground
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: sendField.top
            topMargin: -10
        }
    }

    SendCanMessageField {
        id: sendField
        height: 40
        anchors {
            right: parent.right
            left: parent.left
            bottom: parent.bottom
            bottomMargin: 10
            leftMargin: 10
            rightMargin: 10
        }
    }
}


/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
