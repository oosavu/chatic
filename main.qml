import QtQuick 2.5
import QtQuick.Window 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
//import CppImport 1.0
Window {
    id: window
    minimumHeight: 200
    minimumWidth: 300
    visible: true

    TabView{
        id: chatTabView
        anchors.left: parent.left
        anchors.right: contactList.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        Tab{
            title: "red"
            Rectangle{color: "red"}
        }

    }

    Component{
        id: chatComponent

        Item {
            property int ip
            TextArea{
                id: messageHistory
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: inputTextArea.top
                anchors.top: parent.top
            }
            TextArea{
                id: inputTextArea
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                width:100
                Keys.onEnterPressed: {
                    chatManager.sendMessage(text, ip)
                    inputTextArea.text = ""
                }
            }
        }
    }

    Connections{
        target: chatManager
        onReceiveMessage:{
            chatTabView.getTab(0).messageHistory.append(msg)
        }
    }

    Rectangle {
        color: "red"
        id: contactList
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 100
        ListView {
            anchors.fill: parent
            spacing: 3
            model: userListModel//chatManager.users
            delegate: Rectangle {
                border.color: "black"
                height: 25
                width: 50
                Text {
                    text:"name" + name
                    anchors.verticalCenter: parent.verticalCenter
                    x: 3
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        var tab = chatTabView.addTab(name,chatComponent)
                        tab.ip = 123

                    }
                }
            }
        }
    }
}
