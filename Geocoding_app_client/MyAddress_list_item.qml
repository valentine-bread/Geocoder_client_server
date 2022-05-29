import QtQuick 2.5
import QtQml 2.5
import QtQuick.Window 2.5
import QtQuick.Controls 2.5
import QtPositioning 5.15
import QtLocation 5.15
import QtQuick.Dialogs 1.3
import Qt.labs.qmlmodels 1.0

Item {
    id: address_list_item
    width: parent.width
    height: 30


    Button {
        id:buttom
        width: 30
        anchors.right: parent.right
        anchors.margins: 5
        text: "X"
        onClicked: {
            address_listModel.remove(index)
            console.log(address);
        }
    }
    Text {
        clip: true
        height: 30
        //width: parent.width - 30
        anchors.margins: 5
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: buttom.left
        font.pointSize: 12
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: TextInput.AlignTop
        text: address;
    }
}
