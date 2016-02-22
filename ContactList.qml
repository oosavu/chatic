import QtQuick 2.5

Rectangle {
    color: "red"
    ListView {
        anchors.fill: parent
        spacing: 3
        model: chatManager.users
        delegate: Rectangle {
            border.color: "black"
            height: 25
            width: 50
            Text {
                text: name
                anchors.verticalCenter: parent.verticalCenter
                x: 3
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {

                }
            }
        }
    }
}
