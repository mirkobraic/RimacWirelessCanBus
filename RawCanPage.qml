import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import "CustomComponents"

Item {
    id: root

    CanMessagesListView {
        topMargin: 10
        bottomMargin: 10
        contentInset: 10
        anchors {
            top: parent.top
            topMargin: 10
            bottom: sendField.top
            bottomMargin: 10
            right: parent.right
            rightMargin: 0
            left: parent.left
            leftMargin: 0
        }
    }

    SendCanMessageField {
        id: sendField
        height: 40
        anchors {
            right: parent.right
            left: parent.left
            bottom: parent.bottom
        }
    }
}

