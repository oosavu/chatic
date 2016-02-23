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
//        Tab{
//            title: "red"
//            Rectangle{color: "red"}
//        }

    }

    Component{
        id: chatComponent

        Item {
            property int host: 123


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
            Text{
                anchors.centerIn: parent
                text: host
            }
        }
    }

    Connections{
        target: chatManager
        onReceiveMessage:{
            chatTabView.getTab(0).messageHistory.append(msg)
        }
    }

    function getTabIndexForHost(host){
        console.log("gti. count"+chatTabView.count )
        for(var i = 0; i < chatTabView.count; i++){
            console.log(chatTabView.getTab(i),chatTabView.getTab(i).item.host)
            if(chatTabView.getTab(i).item.host === host ){
                console.log("jkgyjk" + i.toString())
                return i
            }
        }
        return -1
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
                    text:"name" + model.name
                    anchors.verticalCenter: parent.verticalCenter
                    x: 3
                }

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
//                        console.log("add" + getTabIndexForHost(model.host))
//                        if(getTabIndexForHost(model.host) === -1){
                            chatTabView.addTab(model.name,chatComponent)
                            chatTabView.getTab(chatTabView.count-1).active = true
//                            chatTabView.getTab(chatTabView.count-1).item.host = model.host;
//                        }
//                        console.log("now" + getTabIndexForHost(model.host))
//                        chatTabView.currentIndex = getTabIndexForHost(model.host)
                    }
                }
            }
        }
    }
}
