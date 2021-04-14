import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: root
    function open(titleString, messageString) {
        title.text = titleString;
        message.text = messageString;
        popup.open();
    }

    Popup {
        id: popup
        // anchors.centerIn: parent
        contentWidth: parent.width

        modal: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        background: Rectangle {
            radius: 6
            color: "#ddffffff"
        }

        Column {
            spacing: 18
            width: parent.width
            Text {
                id: title
                width: parent.width
                text: ""
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: message
                width: parent.width
                text: ""
                font.pointSize: 13
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
