import QtQuick 2.5
import QtQml 2.5
import QtQuick.Window 2.5
import QtQuick.Controls 2.5
import QtPositioning 5.15
import QtLocation 5.15
import QtQuick.Dialogs 1.3
import Qt.labs.qmlmodels 1.0

Row {
    id: address_row
    height: 50
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right

    MyTexyInput {
        id: address_text
        width: (parent.width - 100)
    }
    Button{
        id:button
        text: "ADD"
        width: 100
        height: 30
        onClicked: {
            if(address_text.text_in !== "")
                address_listModel.append({address: address_text.text_in})
        }
    }
}
