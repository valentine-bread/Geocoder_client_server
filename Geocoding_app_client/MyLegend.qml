import QtQuick 2.5
import QtQml 2.5
import QtQuick.Window 2.5
import QtQuick.Controls 2.5
import QtPositioning 5.15
import QtLocation 5.15
import QtQuick.Dialogs 1.3
import Qt.labs.qmlmodels 1.0

Rectangle {
    id: legend
    anchors.left: mapview.left
    anchors.top: mapview.top
    width: 130
    height: 20 * service_marker_сolour.count + 5
    color: "black"
    opacity: 0.5
    ListView{
        anchors.fill: parent;
        model: service_marker_сolour
        delegate: Row {
            Image {
                source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_" + сolour +  ".png"
            }
            Text {
                text: " " + service.toString()
                font.pointSize: 10
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: TextInput.AlignTop
            }
        }
    }
}
