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


ApplicationWindow  {    
    width: 1024
    height: 512
    visible: true
    property var service_сolour: []    
    ListModel{
        id:service_marker_сolour
    }
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
            anchors.right: table_address.left
            copyrightsVisible: false
            MapItemGroup
            {
                MapItemView{
                    model: ListModel{
                        id: marker_list
                        onCountChanged: {
                            mapview.fitViewportToVisibleMapItems();
                            //mapview.zoomLevel;
                        }
                    }

                    delegate: MyGeoMarcker{
                        id: geomarker
                    }
                }
            }
        }


        MyLoading_animation {
            id: loading_animation
            anchors.horizontalCenter: mapview.horizontalCenter
            anchors.verticalCenter: mapview.verticalCenter
            visible: false
        }


        Rectangle {
            id:table_address
            width: parent.width * 0.20 + 100
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            MyAddress_row {
                id: address_row
            }

            ListView {
                id: address_list
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top: address_row.bottom
                anchors.bottom: parent.bottom
                delegate: MyAddress_list_item {
                    id: address_list_item
                }
                model: ListModel {
                    id: address_listModel
                }
            }
        }

        MyLegend {
            id: legend
        }
    }



    /*Component.onCompleted: {
        on_Completed();
    }*/

    MySelect_service_window {
        id: select_service_window
    }



    Connections{
        target: Geocode
        onGetcode:{
            onGetcode(name,address);
        }
        onFinish_download_key: {
            on_Completed();
        }
        onFinish_all_geocoding:{
            loading_animation.visible = false;
        }
    }

    function onGetcode(name,address){
        for(var i = 0; i !== address.length; i++){
            var tmp = address[i];
            var s =  name + "\r\n" + tmp.address + "\r\n" + tmp.lat + "\r\n" + tmp.lon;
            if(tmp.address !== "No result"){
                marker_list.append({lat : tmp.lat, lon: tmp.lon, line : s, сolour: service_сolour[name.toString()]});
            }
        }
        mapview.fitViewportToVisibleMapItems();
    }

    function on_Completed(){
        service_marker_сolour.clear();
        service_сolour = [];
        var s = Geocode.get_all_service_name();
        var key = Geocode.get_all_API_key();
        var c = ["red", "green", "blue" , "yellow"]
        for(var i = 0; i !== s.length; i++){
            service_marker_сolour.append({сolour : c[i % 4].toString(), service : s[i].toString(), key : key[s[i].toString()], check : true});
            service_сolour[s[i].toString()] = c[i % 4].toString();
        }
        //console.log("service" + service_marker_сolour.count);
    }
}
