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

        function refresh(){
            messageHistory.text = ""
            inputTextArea.text = ""
            messageHistory.append(getHistory(userListModel.data(contactListView.currentIndex,"host")))
        }

        TextArea{
            id: messageHistory
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: inputTextArea.top
            anchors.top: parent.top
            anchors.bottomMargin: 5
            textFormat: TextEdit.RichText
            readOnly: true
        }
        TextArea{
            id: inputTextArea
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            width:50
            Keys.onReturnPressed: {
                if(inputTextArea.text.length == 0) return
                chatManager.sendMessage(text, userListModel.data(contactListView.currentIndex,"host"))
                var name = chatManager.name !== ""? chatManager.name: chatManager.selfIp
                var date = Qt.formatDateTime(new Date(), "hh:mm:ss")
                messageHistory.append("<b>[" + name + " (" + date + ")]:</b><br>" + text + "<br>")
                appendHistory(userListModel.data(contactListView.currentIndex,"host"),text,date,name)
                inputTextArea.text = ""
            }
        }
    }

    Connections{
        target: chatManager
        onReceiveMessage:{
            var name = userListModel.data(userIP,"name") !== ""?
                        userListModel.data(userIP,"name"):
                        userListModel.data(userIP,"ip")
            var date = Qt.formatDateTime(new Date(), "hh:mm:ss")
            if(userIP === userListModel.data(contactListView.currentIndex,"host"))
                messageHistory.append("<b>[" + name + " (" + date + ")]:</b><br>" + msg + "<br>")
            appendHistory(userIP,msg,date,name)
            getHistory(userListModel.data(contactListView.currentIndex,"host"))
        }
    }

    property var historyMap:[]
    function appendHistory(host,_msg,_date,_sender){
        if(historyMap[host] === undefined)
            historyMap[host] = []
        historyMap[host].push({date:_date,msg:_msg, sender: _sender})
    }
    function getHistory(host){
        if(historyMap[host] === undefined)
            historyMap[host] = []
        var res = ""
        for(var i = 0; i < historyMap[host].length; i++){
            var message = historyMap[host][i]
            res += "<br><b>[" + message.sender + " (" + message.date + ")]:</b><br>" +  message.msg + "<br>"
        }
        //console.log(res)
        return res
    }



    Item {
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
            onCurrentIndexChanged: chatItem.refresh()
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
                    text: "[" + model.name + "]"
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

    Rectangle{
        color:"lightGray"
        anchors.fill: parent
        id: helloScreen
        Column{
            anchors.centerIn: parent
            spacing: 20
            Label{
//                anchors.bottom: parent.verticalCenter
                text: "Welcome to sacred chat.\n Please enter you name:"
            }
            TextField{
                maximumLength: 15
                height: 20
                width: 100
                id: nameTextEdit
                focus: true
                Keys.onReturnPressed: {
                    chatManager.name = nameTextEdit.text
                    helloScreen.visible = false
                }
            }
            Button{
                text: "Join"
                onClicked:{
                    chatManager.name = nameTextEdit.text
                    helloScreen.visible = false
                }
            }
        }
    }
}
