import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 1.4

ApplicationWindow {
    id: window
    width: 800
    height: 600
    visible: true
    title: "BioSim Client"

    ListModel {
        id: entityModel
    }

    Timer {
        interval: 10
        running: true
        repeat: true
        onTriggered: entityController.updateEntities()
    }

    Rectangle {
        anchors.fill: parent
        color: "green"
    }

    Repeater {
        model: entityModel
        delegate: Item {
            x: model.x * 8
            y: model.y * 6
            width: 20
            height: 20
            
            Rectangle {
                anchors.fill: parent
                color: model.species === "Rabbit" ? "white" : 
                       model.species === "Fox" ? "red" : "gray"
                radius: 10
                border.color: "black"
                border.width: 1
            }
            
            Text {
                anchors.centerIn: parent
                text: model.species.charAt(0)
                font.pixelSize: 10
                color: "black"
            }
        }
    }

    QtObject {
        id: entityController
        
        function updateEntities() {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "http://localhost:8080/entities");
            xhr.onreadystatechange = function() {
                if (xhr.readyState === XMLHttpRequest.DONE) {
                    if (xhr.status === 200) {
                        var entities = JSON.parse(xhr.responseText);
                        entityModel.clear();
                        for (var i = 0; i < entities.length; i++) {
                            if (entities[i].is_alive) {
                                entityModel.append(entities[i]);
                            }
                        }
                        console.log("Updated entities:", entities.length);
                    } else {
                        console.log("Error fetching entities:", xhr.status, xhr.statusText);
                    }
                }
            }
            xhr.send();
        }
    }


    // Button {
    //     anchors.bottom: parent.bottom
    //     anchors.horizontalCenter: parent.horizontalCenter
    //     text: "Обновить"
    //     onClicked: entityController.updateEntities()
    // }

    Component.onCompleted: {
        entityController.updateEntities();
    }
}