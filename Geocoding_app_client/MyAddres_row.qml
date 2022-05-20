import QtQuick 2.5
import QtQml 2.5
import QtQuick.Window 2.5
import QtQuick.Controls 2.5
import QtPositioning 5.15
import QtLocation 5.15
import QtQuick.Dialogs 1.3
import Qt.labs.qmlmodels 1.0

Row {
    id: addres_row
    height: 50
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right

   /*Rectangle{
        id:rect_addres
        width: (parent.width - 100)
        height: 30
        //anchors.left: parent.left
        border.color: "black"
        border.width: 1
        clip: true
        TextInput{
            id:addres_text
            anchors.fill: parent
            font.pointSize: 12
            text: "Moscow"
            anchors.margins: 4
            selectByMouse: true
            verticalAlignment: TextInput.AlignTop
        }
    }*/
    MyTexyInput {
        id: addres_text
    }
    MyButton{
        id:button
        text: "ADD"
        width: 100
        height: 30
        onClicked: {
            addres_listModel.append({addres: addres_text.text_in})

        }
    }
}
