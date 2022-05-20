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
            text: "GO"
            onTriggered:{
                console.log('GO')
                marker_list.clear();
                process_indicator.value = 0;
                number_requests = 0;
                var map_addres = [];
                for(var i = 0; i !== addres_listModel.count; i++){
                    map_addres.push(addres_listModel.get(i).addres);
                }
                var map_chack = {};
                for(i = 0; i !== service_marker_сolour.count; i++){
                    map_chack[service_marker_сolour.get(i).service] = service_marker_сolour.get(i).check;
                    if(service_marker_сolour.get(i).check === true){
                        number_requests += addres_listModel.count;
                    }
                }
                Geocode.set_addres_list(map_addres);
                Geocode.geocoding_list(map_chack);
                //Geocode.geocoding_test();


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
                addres_listModel.clear();
                for(var i = 0; i !== l.length; i++){
                    addres_listModel.append({addres: l[i] + ""});
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
}
