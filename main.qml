import QtQuick 2.5
import QtQuick.Window 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
//import CppImport 1.0
Window {
    id: window
    minimumHeight: 300
    minimumWidth: 600
    width: minimumWidth
    height: minimumHeight
    visible: true

    Item{
        id: chatItem
        anchors.margins: 5
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
            anchors.bottomMargin: 5
        }
        TextArea{
            id: inputTextArea
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            width:50
            Keys.onEnterPressed: {
                chatManager.sendMessage(text, userListModel.data(contactListView.currentIndex,"host"))
                inputTextArea.text = ""
                console.log(historyMap[0])
                console.log(historyMap[userListModel.data(contactListView.currentIndex,"host")])
            }
        }
    }

    Connections{
        target: chatManager
        onReceiveMessage:{
            //if(history[userIP])

            userListModel.data(contactListView.currentIndex,"host")
            messageHistory.append(userListModel.data(contactListView.currentIndex,"name") + " " +
                                  Qt.formatDateTime(new Date(), "hh:mm:ss \n ") + msg + "\n")
            //if(history[])
            historyMap[userIP] = msg

        }

    }

    property var historyMap:[]




    Item {
        //color: "red"
        id: contactList
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 150
        ListView {
            id: contactListView
            anchors.fill: parent
            anchors.margins: 5
            spacing: 5
            model: userListModel
            interactive: false
//            onCurrentIndexChanged: {
//                if(historyMap[userListModel.data(currentIndex,"name")])
//            }

            delegate: Rectangle {
                border.color: "black"
                radius: 5
                color: contactListView.currentIndex == index? "lightgreen": "white"
                height: 40
                width: contactListView.width
                Text {
                    text: model.ip
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.bottom: parent.verticalCenter
                    verticalAlignment:   Text.AlignHCenter
                    horizontalAlignment:  Text.AlignRight
                }
                Text {
                    text: model.name
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.verticalCenter
                    anchors.bottom: parent.bottom
                    verticalAlignment:   Text.AlignHCenter
                    horizontalAlignment:  Text.AlignVCenter
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
