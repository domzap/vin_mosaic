
// Project libraries.
#include "ImageProcessor.h"

// Qt libraries.

// Constructor.
ImageProcessor::ImageProcessor(QObject *parent) : QObject(parent)
{

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

// Sets previewImage.
void ImageProcessor::setPreviewImage(cv::Mat &image)
{
    previewImage_ = image.clone();
    emit previewImageChanged(previewImage_.clone());
}
