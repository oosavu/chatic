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

    Item{
        id: chatItem
        anchors.left: parent.left
        anchors.right: contactList.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

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
                chatManager.sendMessage(text, userListModel.data(contactListView.currentIndex,"host"))
                inputTextArea.text = ""
            }
        }
    }

    Connections{
        target: chatManager
        onReceiveMessage:{
            messageHistory.append(userListModel.data(contactListView.currentIndex,"name") + " " +
                                  Qt.formatDateTime(new Date(), "hh:mm:ss \n ") + msg + "\n")
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
            id: contactListView
            //property int activeHost
            highlight: Component {
                id: highlightBar
                Rectangle {
                    color: "green"
                     width: contactListView.width
                     height: 25
                }
            }
            //onCurrentIndexChanged: activeHost =
            anchors.fill: parent
            spacing: 3
            model: userListModel
            delegate: Rectangle {
                border.color: "black"
                color: contactListView.currentIndex == index? "green": "blue"
                height: 25
                width: contactListView.width
                Text {
                    text:"name" + model.name + " " + model.host
                    anchors.verticalCenter: parent.verticalCenter
                    x: 3
                }

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        contactListView.currentIndex = index
                    }
                }
            }
        }
    }
}
