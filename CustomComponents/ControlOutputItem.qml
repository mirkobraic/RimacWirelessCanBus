import QtQuick 2.15
import QtQuick.Controls 2.14

Item {
    property string name: "-"
    property string canId: "0"

    Row {
        anchors.fill: parent
        anchors.leftMargin: 16
        spacing: 10

        Text {
            height: parent.height
            width: 160
            text: name
            verticalAlignment: Text.AlignVCenter
            font.bold: true
            font.pointSize: 16
        }

        Switch {
            id: toggleSwitch
            height: parent.height

            onClicked: {
                let data = [];
                if (toggleSwitch.checked) {
                    data = ["FF", "FF", "FF", "FF", "FF", "FF", "FF", "FF"];
                } else {
                    data = ["00", "00", "00", "00", "00", "00", "00", "00"];
                }
                viewController.sendDirectCanMessage(canId, data);
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
