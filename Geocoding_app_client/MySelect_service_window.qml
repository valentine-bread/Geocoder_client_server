import QtQuick 2.5
import QtQml 2.5
import QtQuick.Window 2.5
import QtQuick.Controls 2.5
import QtPositioning 5.15
import QtLocation 5.15
import QtQuick.Dialogs 1.3
import Qt.labs.qmlmodels 1.0

Window {
    id: select_service_window
    width:800
    height:320
    //property bool closing: false
    ListView{
        id: listview_key_check        
        anchors.fill: parent
        model: service_marker_сolour
        delegate:
        Rectangle{
            height: 40
            width: parent.width
            Switch{
                id: checkbox
                anchors.left: parent.left
                width: 40
                height: 40
                checked: check
                onClicked: {
                    service_marker_сolour.set(index, {check : checked})
                }
            }
            Text {
                id: service_name
                anchors.left: checkbox.right
                height: 40
                width: 150
                text: " " + service.toString()
                font.pointSize: 12

                verticalAlignment: TextInput.AlignTop
            }
            /*Rectangle{
                anchors.left: service_name.right
                anchors.right: parent.right
                height: 40
                border.color: "black"
                border.width: 1
                clip: true
                TextInput{
                    anchors.fill: parent
                    id: key_input
                    font.pointSize: 12
                    text: key
                    anchors.margins: 4
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: TextInput.AlignTop
                    selectByMouse: true
                    onEditingFinished: {
                        service_marker_сolour.set(index, {key : text})
                    }
                }
            }*/
            MyTexyInput{
                anchors.left: service_name.right
                anchors.right: parent.right
                height: 40

                id: key_input
                text_in: key
                onFinish_edit: {
                    //console.log("finish");
                    service_marker_сolour.set(index, {key : text_in})
                }
            }
        }
    }

    MyButton {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        height: 40
        text: "Сохранить"
        width: 80
        onClicked: {
            var key_map = {};
            for(var i = 0; i !== service_marker_сolour.count; i++){
                //console.log(service_marker_сolour.get(i).service + " " + service_marker_сolour.get(i).key);
                key_map[service_marker_сolour.get(i).service] = service_marker_сolour.get(i).key;
            }
            Geocode.download_API_key(key_map);
        }
    }
    /*onClosingChanged: {
        console.log("closing");
        on_Completed();
        select_service_window.close();
    }*/
}
