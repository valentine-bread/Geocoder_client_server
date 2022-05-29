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
import QtQuick.Layouts 1.0


Window{
    id: input_server_ip
    visible: false
    width: 352
    height: 85
    maximumHeight: height
    maximumWidth: width
    minimumHeight: height
    minimumWidth: width
    title: qsTr("Адрес сервера")

        Text {
            id:l1
            anchors.left: parent.left
            anchors.margins: 10
            font.pointSize: 12
            anchors.top: parent.top
            //anchors.verticalCenter: parent.verticalCenter
            text: "Введите IP-адрес"
        }

        TextField {
            id:text
            anchors.left: l1.right
            anchors.right: l2.left
            anchors.margins: 10
            font.pointSize: 12
            anchors.top: parent.top
            text: Geocode.get_ip_address_server();
            validator: RegExpValidator {
                regExp:  /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
            }
        }

        Text {
            id:l2
            anchors.right: parent.right
            font.pointSize: 12
            anchors.margins: 10
            anchors.top: parent.top
            //anchors.verticalCenter: parent.verticalCenter
            text: ":27001"
        }

        Button {
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            height: 40
            text: "Сохранить"
            width: 100
            onClicked: {
                if(text.text !== "")
                    Geocode.set_ip_address_server(text.text);
            }
        }
}
