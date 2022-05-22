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
    }
    Menu{
        title: "Старт"
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
    FileDialog {
        id: fileDialog_data
        signal load()
        signal save()
        property int type: 0

        title: "Please choose a file"
        folder: "."
        onAccepted: {
            if(type == 0){
                console.log("You chose: " + fileDialog_data.fileUrl);
                var l = Geocode.load_in_file((fileDialog_data.fileUrl.toString()));
                address_listModel.clear();
                for(var i = 0; i !== l.length; i++){
                    address_listModel.append({address: l[i] + ""});
                }
            }
            else if(type == 1){
                Geocode.get_rez((fileDialog_data.fileUrl.toString()));
            }
        }
        onRejected: {
            console.log("Canceled")
        }
        nameFilters: [ "Text (*.txt)", "All files (*)" ]
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
    }
    function start_geocoding(type){
        console.log('GO')
        marker_list.clear();
        process_indicator.value = 0;
        number_requests = 0;
        var map_address = [];
        for(var i = 0; i !== address_listModel.count; i++){
            map_address.push(address_listModel.get(i).address);
        }
        var map_chack = {};
        for(i = 0; i !== service_marker_сolour.count; i++){
            map_chack[service_marker_сolour.get(i).service] = service_marker_сolour.get(i).check;
            if(service_marker_сolour.get(i).check === true){
                number_requests += address_listModel.count;
            }
        }
        Geocode.set_address_list(map_address);
        if(type ==="direct"){
            Geocode.direct_geocoding_list(map_chack);
        }
        else if(type === "reverse"){
            Geocode.reverse_geocoding_list(map_chack);
        }

        //Geocode.geocoding_test();

    }
}


