
// Project libraries.
#include "ImageProcessor.h"

// Qt libraries.
#include <QDir>

// Defines.
#define MINIMUM_GRID_DIVIDER 10

// Constructor.
ImageProcessor::ImageProcessor(QObject *parent) :
    QObject(parent),
    gridDivider_(MINIMUM_IMAGES_NECCESSARY)
{

}

// Sets new grid divider value.
void ImageProcessor::setGridDivider(int gridDivider)
{
    if(gridDivider < MINIMUM_IMAGES_NECCESSARY || gridDivider == gridDivider_) {
        return;
    } else {
        gridDivider_ = gridDivider;
        emit gridDividerChanged();
    }
}

// Loads source image from the path given.
bool ImageProcessor::loadSourceImage(QString imagePath)
{
    bool retVal = false;
    sourceImage_ = cv::imread(imagePath.toStdString());
    if(sourceImage_.data){
        setPreviewImage(sourceImage_);
        retVal = true;
    }
    emit sourceImageLoadedChanged();
    return retVal;
}

// Searches for images stored in the source directory.
void ImageProcessor::loadSourceDirectory(QString directoryPath)
{
    // Clear current source directory image list.
    imagesList_.clear();

    // Filter images from the source directory.
    QStringList images = QDir(directoryPath).entryList(QStringList() << "*.bmp" << "*.dib" << "*.jpeg" << "*.jpg" << "*.jpe" << "*.jp2" << "*.png" << "*.pbm" << "*.pgm" << "*.ppm" << "*.sr" << "*.ras" << "*.tiff" << "*.tif", QDir::Files);

    // Set image paths.
    foreach (QString imagePath, images) {
        imagesList_.push_back(Image(imagePath));
    }

}

// Sets previewImage.
void ImageProcessor::setPreviewImage(cv::Mat &image)
{
    previewImage_ = image.clone();
    emit previewImageChanged(previewImage_.clone());
}
