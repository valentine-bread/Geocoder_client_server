import QtQuick 2.5
import QtQml 2.5
import QtQuick.Window 2.5
import QtQuick.Controls 2.5
import QtPositioning 5.15
import QtLocation 5.15
import QtQuick.Dialogs 1.3
import Qt.labs.qmlmodels 1.0

Rectangle{
    id:rect_address
    property string text_in: ""
    signal finish_edit()    
    height: 30
    border.color: "black"
    border.width: 1
    anchors.margins: 4
    clip: true
    //onFinish_edit:{}

    TextInput{
        id:address_text_text
        anchors.fill: parent
        font.pointSize: 12
        text: text_in
        anchors.margins: 4
        selectByMouse: true
        verticalAlignment: TextInput.AlignTop
        onEditingFinished: {
            rect_address.text_in = text
            rect_address.finish_edit();
        }
    }
}
