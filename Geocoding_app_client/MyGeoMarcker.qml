import QtQuick 2.5
import QtQml 2.5
import QtQuick.Window 2.5
import QtQuick.Controls 2.5
import QtPositioning 5.15
import QtLocation 5.15
import QtQuick.Dialogs 1.3
import Qt.labs.qmlmodels 1.0

MapQuickItem {
    id: geomarker
    visible:true
    ToolTip.visible: false
    sourceItem: Image{
        source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_" + сolour + ".png"
    }
    MouseArea {
        hoverEnabled: true
        anchors.fill: parent
        onEntered: {
            ToolTip.visible = true
            ToolTip.text = line
        }
        onExited: {
            ToolTip.visible = false
        }
    }
    anchorPoint: Qt.point(sourceItem.width/2, sourceItem.height/2)
    coordinate {
        latitude: lat
        longitude: lon
    }
}
