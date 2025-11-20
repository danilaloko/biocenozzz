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
                       model.species === "Fox" ? "red" : 
                       model.species === "Grass" ? "darkgreen" : "gray"
                radius: model.species === "Grass" ? 0 : 10
                border.color: "black"
                border.width: 1
            }
            
            Text {
                anchors.centerIn: parent
                text: model.species === "Rabbit" ? "R" : 
                      model.species === "Fox" ? "F" : 
                      model.species === "Grass" ? "G" : "U"
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
                    }
                }
            }
            xhr.send();
        }
    }

    Component.onCompleted: {
        entityController.updateEntities();
    }
}