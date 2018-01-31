
// Project libraries.
#include "MosaicCreator.h"

// Qt libraries.
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <QDebug>

// Defines.
#define VIEW_WIDTH 1280
#define VIEW_HEIGHT 768
#define WINDOW_WIDTH_MIN 1280
#define WINDOW_HEIGHT_MIN 768

// Constructor.
MosaicCreator::MosaicCreator(QObject *parent) :
    QObject(parent)
{
    init();
    makeConnections();
    start();

    setNotificationString(tr("INFO: Start by loading a source image for mosiac application."));
}

// Sets notification string.
void MosaicCreator::setNotificationString(QString notificationString)
{
    if(notificationString_ != notificationString){
        notificationString_ = notificationString;
        emit notificationStringChanged();
    }
}

// Slot is called whenever new source image is chosen.
void MosaicCreator::onSourceImageChosen(QVariant imagePath)
{
    if(!imageProcessor_.loadSourceImage(imagePath.toString())){
        setNotificationString(tr("ERROR: Source image was not loaded!"));
    } else {
        setNotificationString(tr("INFO: Select folder containing sufficient amount of images."));
    }
}

// Slot is called whenever new source image directory is chosen.
void MosaicCreator::onSourceDirectoryChosen(QVariant directoryPath)
{
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
void MosaicCreator::onComputeMosaic()
{
    imageProcessor_.computeMosaic();
}

// Slot is called whenever preview image is to be exported.
void MosaicCreator::onExportImage(QVariant exportFilePath)
{
    if(imageProcessor_.exportImage(exportFilePath.toString())){
        setNotificationString(tr("INFO: Image exported successfully."));
    } else {
        setNotificationString(tr("ERROR: Image was not exported!"));
    }
}

// Initialize all necessary components.
void MosaicCreator::init()
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
    window->setMinimumWidth(WINDOW_WIDTH_MIN);
    window->setMinimumHeight(WINDOW_HEIGHT_MIN);

}

// Create all necessary Qt connections.
void MosaicCreator::makeConnections()
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
void MosaicCreator::start()
{
    view_->show();
}
