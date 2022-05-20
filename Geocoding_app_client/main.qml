import QtQuick 2.5
import QtQml 2.5
import QtQuick.Window 2.5
import QtQuick.Controls 2.5
import QtPositioning 5.15
import QtLocation 5.15
import QtQuick.Dialogs 1.3
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls.Universal 2.12

ApplicationWindow  {    
    width: 1024
    height: 512
    visible: true
    property int number_requests : 0


    property var service_сolour: []
    menuBar: MyMenu_bar {
        id: menu_bar
    }

    Rectangle{
        anchors.fill: parent
        Map{
            id:mapview
            center: QtPositioning.coordinate(55.7558, 37.6178);
            plugin: Plugin { name: "osm" }//"osm", "mapboxgl", "esri"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.right: table_addres.left
            copyrightsVisible: false
            MapItemGroup
            {
                MapItemView{
                    model: ListModel{
                        id: marker_list
                        onCountChanged: {
                            mapview.fitViewportToVisibleMapItems();
                            mapview.zoomLevel - 0.5;
                        }
                    }

                    delegate: MyGeoMarcker{
                        id: geomarker
                    }
                }
            }
        }

        ProgressBar{
            id: process_indicator
            anchors.bottom: mapview.bottom
            anchors.left: mapview.left
            width: mapview.width
            //height : 100
            value: 0

        }

        Rectangle {
            id:table_addres
            width: parent.width * 0.20 + 100
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            MyAddres_row {
                id: addres_row
            }

            ListView {
                id: addres_list
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top: addres_row.bottom
                anchors.bottom: parent.bottom
                delegate: MyAddres_list_item {
                    id: addres_list_item
                }
                model: ListModel {
                    id: addres_listModel
                }
            }
        }

        MyLegend {
            id: legend
        }
    }

    ListModel{
        id:service_marker_сolour
    }

    Component.onCompleted: {
        on_Completed();
    }

    MySelect_service_window {
        id: select_service_window
    }

    function on_Completed(){
        service_marker_сolour.clear();
        var s = Geocode.get_all_service_name();
        var key = Geocode.get_all_API_key();
        var c = ["red", "green", "blue" , "yellow"]
        for(var i = 0; i !== s.length; i++){
            service_marker_сolour.append({сolour : c[i % 4].toString(), service : s[i].toString(), key : key[s[i].toString()], check : true});
            service_сolour[s[i].toString()] = c[i % 4].toString();


        }
        //console.log("service" + service_marker_сolour.count);
    }

    Connections {
        target: Geocode
        onGetcode: {
            onGetcode(name,addres,lat,lon);
        }
    }
    Connections {
        target: Geocode
        onFinish_download_key: {
            console.log("finish");
            on_Completed();
        }
    }

    function onGetcode(name,addres,lat,lon){
        var s =  name + "\r\n" + addres + "\r\n" + lat + "\r\n" + lon
        if(name !== "No result"){
            marker_list.append({lat : lat, lon: lon, line : s, сolour: service_сolour[name.toString()]});
        }
        //mapview.zoomLevel--;
        //mapview.fitViewportToVisibleMapItems();
        process_indicator.value += (1 / (number_requests));
    }

}