import QtQuick 2.5
import QtQml 2.5
import QtQuick.Window 2.5
import QtQuick.Controls 2.5
import QtPositioning 5.15
import QtLocation 5.15
import QtQuick.Dialogs 1.3
import Qt.labs.qmlmodels 1.0

MenuBar{
    id: menu_bar
    visible: true
    Menu{
        title: "Файл"
        Action{
            text: "Загрузить..."
            onTriggered:{
                console.log('Загрузить...')
                fileDialog_data.load();
            }
        }
        Action{
            text: "Сохранить..."
            onTriggered:{
                console.log('Сохранить...');
                fileDialog_data.save();
                //fileDialog_data.open();
                //Geocode.get_rez()
            }
        }
        Action{
            text: "Загрузить ключи"
            onTriggered:{
                fileDialog_data.load_key();
            }
        }
    }
    Menu{
        title: "Геокодирование"
        Action{
            text: "Прямое геокодирование"
            onTriggered:{
                start_geocoding("direct");
            }
        }
        Action{
            text: "Обратное геокодирование"
            onTriggered:{
                start_geocoding("reverse");
               }
        }

        Action{
            text: "Выбор сервисов"
            onTriggered:{
                //on_Completed();
                select_service_window.show();
            }
        }        
    }
    Menu{
        title: "Сервер"
        Action{
            text: "IP сервера"
            onTriggered:{
                input_server_ip.show();
            }
        }
        Action{
            text: "API ключи"
            onTriggered:{
                select_service_window.show();
            }
        }
    }

    FileDialog {
        id: fileDialog_data
        signal load()
        signal save()
        signal load_key()
        property int type: 0

        title: "Please choose a file"
        folder: "."
        onAccepted: {
            if(type == 0){
                console.log("You chose: " + fileDialog_data.fileUrl);
                var l = Geocode.load_out_file(fileDialog_data.fileUrl);
                address_listModel.clear();
                for(var i = 0; i !== l.length; i++){
                    address_listModel.append({address: l[i] + ""});
                }
            }
            else if(type == 1){
                Geocode.save_in_file((fileDialog_data.fileUrl.toString()));
            }
            else if(type == 2){
                Geocode.load_key_out_file((fileDialog_data.fileUrl.toString()));
            }
        }
        onRejected: {
            console.log("Canceled")
        }
        nameFilters: [ "Text (*.txt)", "JSON (*.json)", "All files (*)" ]
        onLoad:{
            type = 0;
            selectExisting = true
            open();
        }
        onSave:{
            type = 1;
            selectExisting = false
            open();
        }
        onLoad_key: {
            type = 2;
            selectExisting = true
            open();
        }
    }
    function start_geocoding(type){
        console.log('GO')
        marker_list.clear();
        var map_address = [];
        for(var i = 0; i !== address_listModel.count; i++){
            map_address.push(address_listModel.get(i).address);
        }
        var map_chack = {};
        for(i = 0; i !== service_marker_сolour.count; i++){
            map_chack[service_marker_сolour.get(i).service] = service_marker_сolour.get(i).check;
        }
        Geocode.set_address_list(map_address);
        if(type ==="direct"){
            Geocode.direct_geocoding_list(map_chack);
        }
        else if(type === "reverse"){
            Geocode.reverse_geocoding_list(map_chack);
        }
        loading_animation.visible = true;

        //Geocode.geocoding_test();

    }
}
