# Project settings.
TEMPLATE = app
TARGET = mosaic
OBJECTS_DIR = object_files
MOC_DIR = moc_files

# Configs.
CONFIG += c++11

# Qt modules.
QT += core
QT += qml
QT += quick
QT += multimedia
QT += svg

# Qt plugins.
QTPLUGIN += qsvg

# Include paths.
INCLUDEPATH += .

# Project defines.
DEFINES += NOMINMAX
DEFINES += CV_MAJOR_VERSION=3

# Header files.
HEADERS += \
    MosiacCreator.h \
    ImageUpdater.h \
    ImageProvider.h \
    ImageProcessor.h

# Source files.
SOURCES += main.cpp \
    MosiacCreator.cpp \
    ImageUpdater.cpp \
    ImageProvider.cpp \
    ImageProcessor.cpp

# Resources.
RESOURCES += \
    resources.qrc

CONFIG(debug, debug|release){
    message("Debug build")
} else {
    message("Release build")
}

# Platform-dependent paths

linux {

    message("Building Mosiac for Linux.")

#    HOME_DIRECTORY = $$(HOME)
#    INCLUDEPATH += $HOME_DIRECTORY/libs/OpenCV/include/opencv2
#    INCLUDEPATH += $HOME_DIRECTORY/libs/OpenCV/include/opencv
#    INCLUDEPATH += $HOME_DIRECTORY/libs/OpenCV/include

    INCLUDEPATH += /home/dominik/libs/OpenCV/include/opencv2
    INCLUDEPATH += /home/dominik/libs/OpenCV/include/opencv
    INCLUDEPATH += /home/dominik/libs/OpenCV/include

    LIBS += -L"/home/dominik/libs/OpenCV/lib"

    LIBS += -lopencv_world
}

win32 {
    message("Building Mosiac for Windows.")

    INCLUDEPATH += C:\OpenCV\3.2.0\opencv\build\include\opencv2
    INCLUDEPATH += C:\OpenCV\3.2.0\opencv\build\include\opencv
    INCLUDEPATH += C:\OpenCV\3.2.0\opencv\build\include

    LIBS += -L"C:\OpenCV\3.2.0\opencv\build\x64\vc14\lib"

    CONFIG(debug, debug|release){
        LIBS += -lopencv_world320
    } else {
        LIBS += -lopencv_world320d
    }
}
