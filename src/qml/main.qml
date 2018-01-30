import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2

Rectangle {
    id: root
    anchors.fill: parent
    color: "lightgray"

    property int defaultMargin: 5

    signal sourceImageChosen(var imagePath);
    signal sourceDirectoryChosen(var directoryPath);

    Column {
        anchors.fill: parent

        Item {
            id: mainContentHolder
            width: root.width
            height: root.height*0.96

            Row {
                anchors.fill: parent

                Item {
                    id: previewHolder
                    width: mainContentHolder.width*0.8
                    height: mainContentHolder.height

                    Rectangle {
                        id: previewRectangle
                        anchors.fill: parent
                        anchors.margins: root.defaultMargin
                        border.color: "black"
                        border.width: 1
                        color: "white"

                        Image {
                            id: imagePreview
                            cache: false
                            width: parent.width
                            height: parent.height
                            fillMode: Image.PreserveAspectFit
                            mipmap: true

                            Connections {
                                target: imagePreviewer
                                onImageUpdated: {
                                    imagePreview.source = "";
                                    imagePreview.source = "image://imagePreviewProvider/foobar";
                                }
                            }
                        }
                    }

                }

                Rectangle {
                    width: 1
                    height: mainContentHolder.height
                    color: "darkgrey"
                }

                Item {
                    id: controlPanelHolder
                    width: mainContentHolder.width*0.2 - 1
                    height: mainContentHolder.height

                    Column {
                        anchors.fill: parent

                        Item {
                            id: buttonChooseImageHolder
                            width: controlPanelHolder.width
                            height: controlPanelHolder.height*0.1

                            Button {
                                id: buttonChooseImage
                                anchors.fill: parent
                                anchors.margins: root.defaultMargin

                                Text {
                                    anchors.centerIn: parent
                                    text: "Choose a source\nimage"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }

                                onClicked: {
                                    fileDialog.title = "Choose a source image"
                                    fileDialog.selectFolder = false
                                    fileDialog.open()
                                }
                            }
                        }

                        Item {
                            id: buttonChooseFolderHolder
                            width: controlPanelHolder.width
                            height: controlPanelHolder.height*0.1

                            Button {
                                id: buttonChooseFolderButton
                                anchors.fill: parent
                                anchors.margins: root.defaultMargin
                                enabled: imageProcessor.sourceImageLoaded
                                opacity: enabled ? 1 : 0.5

                                Text {
                                    anchors.centerIn: parent
                                    text: "Choose a source\nimage folder"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }

                                onClicked: {
                                    fileDialog.title = "Choose a source image folder"
                                    fileDialog.selectFolder = true
                                    fileDialog.open()
                                }
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            width: root.width
            height: 1
            color: "darkgrey"
        }

        Item {
            id: notificationContentHolder
            width: root.width
            height: root.height*0.04 - 1

            Item {
                id: notificationTextHolder
                anchors.fill: parent
                anchors.margins: root.defaultMargin

                Text {
                    text: creator.notificationString
                    width: notificationTextHolder.width
                    height: notificationTextHolder.height
                    font.pointSize: 100
                    minimumPointSize: 10
                    fontSizeMode: Text.Fit

                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: ""
        folder: Qt.resolvedUrl(".")
        nameFilters: [ "All files (*)"]
        selectMultiple: false
        selectFolder: false
        visible: false

        onAccepted: {
            var path = decodeURIComponent(fileDialog.fileUrl.toString().replace(/^(file:\/{3})/,""))
            if(selectFolder){
                console.log("Mosaic source directory chosen: " + path)
                root.sourceDirectoryChosen(path);
            } else {
                console.log("Source image chosen: " + path)
                root.sourceImageChosen(path);
            }
        }
        onRejected: {
            console.log("Canceled")
        }
    }
}
