import QtQuick 2.6
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.2

Rectangle {
    id: root
    anchors.fill: parent
    color: "lightgray"

    property int defaultMargin: 5

    signal sourceImageChosen(var imagePath);
    signal sourceDirectoryChosen(var directoryPath);
    signal computeMosaic();
    signal exportImage(var exportPath);

    Column {
        anchors.fill: parent

        Item {
            id: previewHolder
            width: root.width
            height: root.height*0.87

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
            width: root.width
            height: 1
            color: "darkgrey"
        }

        Item {
            id: controlPanelHolder
            width: root.width
            height: root.height*0.09 - 1

            Row {
                anchors.fill: parent

                Item {
                    id: buttonChooseImageHolder
                    width: controlPanelHolder.width*0.2
                    height: controlPanelHolder.height

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
                            fileDialog.selectExisting = true
                            fileDialog.purpose = "file"
                            fileDialog.open()
                        }
                    }
                }

                /////////////////////////////////////////////////////////////

                Item {
                    id: buttonChooseFolderHolder
                    width: controlPanelHolder.width*0.2
                    height: controlPanelHolder.height

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
                            fileDialog.purpose = "folder"
                            fileDialog.selectFolder = true
                            fileDialog.selectExisting = true
                            fileDialog.open()
                        }
                    }
                }

                /////////////////////////////////////////////////////////////

                Item {
                    id: spinboxRowHolder
                    width: controlPanelHolder.width*0.2
                    height: controlPanelHolder.height
                    enabled: imageProcessor.sourceImageLoaded
                    opacity: enabled ? 1 : 0.5

                    Row {
                        anchors.fill: parent

                        Item {
                            width: spinboxRowHolder.width*0.5
                            height: spinboxRowHolder.height

                            Text {
                                anchors.centerIn: parent
                                text: "Grid size:"
                                anchors.fill: parent
                                anchors.margins: root.defaultMargin
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        Item {
                            width: spinboxRowHolder.width*0.5
                            height: spinboxRowHolder.height

                            SpinBox {
                                id: gridSizeSpinBox
                                anchors.centerIn: parent
                                width: parent.width
                                height: parent.height*0.5
                                anchors.margins: root.defaultMargin
                                value: imageProcessor.gridDivider
                                from: imageProcessor.minGrid
                                to: 999
                                editable: true

                                onValueChanged: {
                                    imageProcessor.gridDivider = value
                                }

                            }
                        }
                    }
                }

                /////////////////////////////////////////////////////////////

                Item {
                    id: buttonComputeMosaicHolder
                    width: controlPanelHolder.width*0.2
                    height: controlPanelHolder.height

                    Button {
                        id: buttonComputeMosaic
                        anchors.fill: parent
                        anchors.margins: root.defaultMargin
                        enabled: imageProcessor.imagesFound >= imageProcessor.minImages
                        opacity: enabled ? 1 : 0.5

                        Text {
                            anchors.centerIn: parent
                            text: "Compute a mosaic\nimage"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {
                            root.computeMosaic();
                        }
                    }
                }

                /////////////////////////////////////////////////////////////

                Item {
                    id: buttonExportHolder
                    width: controlPanelHolder.width*0.2
                    height: controlPanelHolder.height

                    Button {
                        id: buttonExport
                        anchors.fill: parent
                        anchors.margins: root.defaultMargin
                        enabled: imageProcessor.sourceImageLoaded
                        opacity: enabled ? 1 : 0.5

                        Text {
                            anchors.centerIn: parent
                            text: "Export"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            /*width: parent.width
                            height: parent.height
                            font.pointSize: 100
                            minimumPointSize: 10
                            fontSizeMode: Text.Fit*/
                        }

                        onClicked: {
                            fileDialog.title = "Export image"
                            fileDialog.purpose = "export"
                            fileDialog.selectFolder = false
                            fileDialog.selectExisting = false
                            fileDialog.open()
                        }
                    }
                }

                /////////////////////////////////////////////////////////////

                /////////////////////////////////////////////////////////////
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
        selectExisting: true
        visible: false

        property string purpose: ""

        onAccepted: {
            var path
            if(Qt.platform.os === "linux"){
                path = decodeURIComponent(fileDialog.fileUrl.toString().replace(/^(file:\/{2})/,""))
            } else if (Qt.platform.os === "windows"){
                path = decodeURIComponent(fileDialog.fileUrl.toString().replace(/^(file:\/{3})/,""))
            } else {
                path = decodeURIComponent(fileDialog.fileUrl.toString().replace(/^(file:\/{3})/,""))
            }

            if(purpose === "export"){
                //                console.log("Image for export: " + path)
                root.exportImage(path);
            } else if (purpose === "folder") {
                //                console.log("Mosaic source directory chosen: " + path)
                root.sourceDirectoryChosen(path);
            } else if (purpose === "file") {
                //                console.log("Source image chosen: " + path)
                root.sourceImageChosen(path);
            }

        }
    }
}


