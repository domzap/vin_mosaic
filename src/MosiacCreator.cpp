
// Project libraries.
#include "MosiacCreator.h"

// Qt libraries.
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <QDebug>

// Defines.
#define VIEW_WIDTH 1280
#define VIEW_HEIGHT 768
#define WINDOW_WIDTH_MAX 1024
#define WINDOW_HEIGHT_MAX 768
#define WINDOW_WIDTH_MIN 1280
#define WINDOW_HEIGHT_MIN 768

//#define DEBUG_SOURCE_IMAGE_PATH "C:\\Data\\Programmingstuff\\VIN\\vin_mosaic\\build-mosaic-Desktop_Qt_5_10_0_MSVC2015_64bit2-Release\\mosiac_source\\1.JPG"
//#define DEBUG_SOURCE_IMAGE_DIRECTORY "C:\\Data\\Programmingstuff\\VIN\\vin_mosaic\\build-mosaic-Desktop_Qt_5_10_0_MSVC2015_64bit2-Release\\mosiac_dataset"
//#define DEBUG_SOURCE_IMAGE_PATH "/home/dominik/Programmingstuff/VIN/vin_mosaic/build-mosaic-Desktop_Qt_5_10_0_GCC_64bit-Release/mosiac_source/bob.jpg"
//#define DEBUG_SOURCE_IMAGE_DIRECTORY "/home/dominik/Programmingstuff/VIN/vin_mosaic/build-mosaic-Desktop_Qt_5_10_0_GCC_64bit-Release/mosiac_dataset_bob"

// Constructor.
MosiacCreator::MosiacCreator(QObject *parent) :
    QObject(parent)
{
    init();
    makeConnections();
    start();

    setNotificationString(tr("INFO: Start by loading a source image for mosiac application."));

    debugProcess();
}

// Sets notification string.
void MosiacCreator::setNotificationString(QString notificationString)
{
    if(notificationString_ != notificationString){
        notificationString_ = notificationString;
        emit notificationStringChanged();
    }
}

void MosiacCreator::debugProcess()
{
//    onSourceImageChosen(QVariant(DEBUG_SOURCE_IMAGE_PATH));
//    onSourceDirectoryChosen(QVariant(DEBUG_SOURCE_IMAGE_DIRECTORY));
}

// Slot is called whenever new source image is chosen.
void MosiacCreator::onSourceImageChosen(QVariant imagePath)
{
    qDebug() << "Source image chosen: " << imagePath.toString();

    if(!imageProcessor_.loadSourceImage(imagePath.toString())){
        setNotificationString(tr("ERROR: Source image was not loaded!"));
    } else {
        setNotificationString(tr("INFO: Select folder containing sufficient amount of images."));
    }
}

// Slot is called whenever new source image directory is chosen.
void MosiacCreator::onSourceDirectoryChosen(QVariant directoryPath)
{
    qDebug() << "Source image directory chosen: " << directoryPath.toString();

    // Find source directory images.
    imageProcessor_.loadSourceDirectory(directoryPath.toString());

    int imagesFound = imageProcessor_.imagesFound();

    if (imagesFound < MINIMUM_IMAGES_NECCESSARY){
        setNotificationString(tr("ERROR: %1 images found in the source directory - at least %2 images needed!").arg(imagesFound).arg(MINIMUM_IMAGES_NECCESSARY));
    } else {
        setNotificationString(tr("INFO: %1 images found in the source directory. Set grid size and compute mosaic image.").arg(imagesFound));
    }
}

// Slot is called whenever mosiac image is to be computed.
void MosiacCreator::onComputeMosaic()
{
    imageProcessor_.computeMosaic();
}

// Slot is called whenever preview image is to be exported.
void MosiacCreator::onExportImage(QVariant exportFilePath)
{
    if(imageProcessor_.exportImage(exportFilePath.toString())){
        setNotificationString(tr("INFO: Image exported successfully."));
    } else {
        setNotificationString(tr("ERROR: Image was not exported!"));
    }
}

// Initialize all necessary components.
void MosiacCreator::init()
{
    // Quick view allocation - it's better when the GUI view is allocated on heap.
    view_ = new QQuickView();

    // Set context properties.
    view_->rootContext()->setContextProperty("view", view_);
    view_->rootContext()->setContextProperty("creator", this);
    view_->rootContext()->setContextProperty("imagePreviewer", &imageUpdater_);
    view_->rootContext()->setContextProperty("imageProcessor", &imageProcessor_);

    // Set image privider for image preview.
    view_->engine()->addImageProvider(QString("imagePreviewProvider"), imageUpdater_.imageProvider());

    // Load main qml file.
    view_->setSource(QUrl(QStringLiteral("qrc:qml/main.qml")));

    // Set GUI window parameters.
    QQuickWindow * window = qobject_cast<QQuickWindow*>(view_);
    view_->setWidth(VIEW_WIDTH);
    view_->setHeight(VIEW_HEIGHT);
//    window->setMaximumWidth(WINDOW_WIDTH_MAX);
//    window->setMaximumHeight(WINDOW_HEIGHT_MAX);
    window->setMinimumWidth(WINDOW_WIDTH_MIN);
    window->setMinimumHeight(WINDOW_HEIGHT_MIN);

}

// Create all necessary Qt connections.
void MosiacCreator::makeConnections()
{
    // GUI <-> MosiacCreator connections.

    // Getting root QML item.
    QObject * item = view_->rootObject();//->findChild<QObject*>("root");

    QObject::connect(item, SIGNAL(sourceImageChosen(QVariant)), this, SLOT(onSourceImageChosen(QVariant)));
    QObject::connect(item, SIGNAL(sourceDirectoryChosen(QVariant)), this, SLOT(onSourceDirectoryChosen(QVariant)));
    QObject::connect(item, SIGNAL(computeMosaic()), this, SLOT(onComputeMosaic()));
    QObject::connect(item, SIGNAL(exportImage(QVariant)), this, SLOT(onExportImage(QVariant)));

    // ImageProcessor -> ImageUpdater connections
    QObject::connect(&imageProcessor_, SIGNAL(previewImageChanged(cv::Mat)), &imageUpdater_, SLOT(setImage(cv::Mat)));

}

// Start MosiacCreator.
void MosiacCreator::start()
{
    view_->show();
}
