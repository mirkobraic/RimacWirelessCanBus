import QtQuick 2.15
import QtQuick.Controls 2.14
import "CustomComponents"

Item {
    id: root
    property string title: "Outputs control"

    Flickable {
        id: flickable
        flickableDirection: Flickable.VerticalFlick
        anchors {
            fill: parent
            rightMargin: 15
            leftMargin: 15
        }

        Column {
            anchors.fill: parent
            spacing: 4

            ControlOutputItem {
                width: parent.width
                height: 54

                name: "LEDs"
                canId: "5C1"
            }

            ControlOutputItem {
                width: parent.width
                height: 54

                name: "Fan 1"
                canId: "5C4"
            }

            ControlOutputItem {
                name: "Fan 2"
                width: parent.width
                height: 54
                canId: "5C2"
            }

            ControlOutputItem {
                width: parent.width
                height: 54

                name: "Light 1"
                canId: "5C3"
            }

            ControlOutputItem {
                width: parent.width
                height: 54

                name: "Light 2"
                canId: "5C5"
            }
        }
    }
}
