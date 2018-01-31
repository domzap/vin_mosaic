
// Project libraries.
#include "ImageProcessor.h"

// Qt libraries.
#include <QDir>
#include <QDebug>
#include <QTime>

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

        if(sourceImageLoaded()){
            computeGrid();
        }
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
    computeGrid();
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
        imagesList_.push_back(std::shared_ptr<Image>(new Image(directoryPath + QDir::separator() + imagePath)));
    }

    emit imagesFoundChanged();

}

// Computes mosiac image.
void ImageProcessor::computeMosaic()
{

    // Load images to the memory and compute color histograms.
    foreach (std::shared_ptr<Image> img, imagesList_) {
        img->loadImage(cv::Size(gridCellWidth_, gridCellHeight_));
    }

    // For each grid cell find the best candidate in the image list.
    foreach (std::shared_ptr<Image> imageCell, imageCells_) {
        double bestScore = 0.0;
        std::shared_ptr<Image> bestCandidate = std::shared_ptr<Image>(nullptr);

        foreach (std::shared_ptr<Image> candidate, imagesList_) {
            double currentScore = candidate->compareToImage(*imageCell);
            if(currentScore > bestScore){
                bestScore = currentScore;
                bestCandidate = candidate;
            }
        }
        if(bestCandidate != std::shared_ptr<Image>(nullptr)){
            bestCandidate->copyToImage(*imageCell);
        }
    }

    setPreviewImage(workingImage_);
}

// Exports currently displayed preview image.
bool ImageProcessor::exportImage(QString imageFilePath)
{
    if(previewImage_.data){
        QString finalPath;
        QStringList supportedFormats = QStringList() << "bmp" << "dib" << "jpeg" << "jpg" << "jpe" << "jp2" << "png" << "pbm" << "pgm" << "ppm" << "sr" << "ras" << "tiff" << "tif";
        if(!supportedFormats.contains(imageFilePath.split('.').last())){
            finalPath = imageFilePath + ".jpg";
        } else {
            finalPath = imageFilePath;
        }

        qDebug() << finalPath;

        return cv::imwrite(finalPath.toStdString(), previewImage_);
    } else {
        qWarning() << "ImageProcessor: Preview image is empty!";
        return false;
    }
    return true;
}

// Sets previewImage.
void ImageProcessor::setPreviewImage(cv::Mat &image)
{
    previewImage_ = image.clone();
    emit previewImageChanged(previewImage_.clone());
}

// Computes mosiac grid.
void ImageProcessor::computeGrid()
{
    // Clone source image to draw grid lines in it.
    cv::Mat tmpImage = sourceImage_.clone();

    // Create new clean instance of the source image.
    workingImage_ = sourceImage_.clone();

    // Clear image cells list - new will be created.
    imageCells_.clear();

    // Get grid cell dimensions.
    gridCellWidth_ = ((double)tmpImage.cols)/gridDivider();
    gridCellHeight_ = ((double)tmpImage.rows)/gridDivider();

    double xErr = gridCellWidth_ - (int)gridCellWidth_;
    double currxErr = 0.0;
    double yErr = gridCellHeight_ - (int)gridCellHeight_;

    int currentGridCellWidth = gridCellWidth_;
    int currentGridCellHeight = gridCellHeight_;

    // For every column in the source image.
    for (int x = 0; x < tmpImage.cols; x+=currentGridCellWidth) {

        currentGridCellWidth = gridCellWidth_;

        if(currxErr >= 1){
            currentGridCellWidth++;
            currxErr -= 1;
        }

        // Draw vertical line.
        cv::line(tmpImage, cv::Point(x, 0), cv::Point(x, tmpImage.rows), cv::Scalar(0, 0, 0), 3);

        double curryErr = 0.0;

        // For every row in the column in the source image.
        for (int y = 0; y < tmpImage.rows; y+=currentGridCellHeight) {

            currentGridCellHeight = gridCellHeight_;

            if(curryErr >= 1){
                currentGridCellHeight++;
                curryErr -= 1;
            }

            if(x == 0){
                // Draw horizontal line only once for the first column.
                cv::line(tmpImage, cv::Point(0, y), cv::Point(tmpImage.cols, y), cv::Scalar(0, 0, 0), 3);
            }

            // Get current grid cell image reference.
            imageCells_.push_back(
                        std::shared_ptr<Image>(new Image(
                            workingImage_(
                                cv::Rect(
                                    x,
                                    y,
                                    x + gridCellWidth_ >= tmpImage.cols ? tmpImage.cols - x : currentGridCellWidth,
                                    y + gridCellHeight_ >= tmpImage.rows ? tmpImage.rows - y : currentGridCellHeight)))));

            curryErr += yErr;

        }

        currxErr += xErr;
    }

    setPreviewImage(tmpImage);
}
