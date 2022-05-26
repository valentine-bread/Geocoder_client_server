import QtQuick 2.5
import QtQml 2.5
import QtQuick.Window 2.5
import QtQuick.Controls 2.5
import QtPositioning 5.15
import QtLocation 5.15
import QtQuick.Dialogs 1.3
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls.Universal 2.12
import QtGraphicalEffects 1.0


Rectangle{
    id:error_animation
    property string what: ""
    width: 200
    height: 50
    radius: width / 16
    color: "black"
    opacity: 0.5
    //color:"#998a8a8a"
    visible: false
    Text {
        anchors.fill:parent
        id: text
        text: qsTr(what)
        font.pointSize: 14
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        onTextChanged: {
            error_animation.visible = true;
        }
    }
    MouseArea{
        anchors.fill: parent
        onClicked: {
            error_animation.visible = false;
        }
    }
}
