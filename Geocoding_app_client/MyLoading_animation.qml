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


Rectangle
{
    id:loading_animation
    width: 100
    height: 100
    radius: width / 8
    color:"#998a8a8a"
    Image
    {
        anchors.centerIn: parent
        width: parent.width/2
        height:parent.height/2
        source: "loading.png"
        RotationAnimation on rotation {
            from: 0
            to: 360
            duration: 1500
            loops: Animation.Infinite
        }
    }
}
