
// Project libraries.
#include "Image.h"

// Qt libraries.
#include <QDebug>

// Constructor 1.
Image::Image(QString imagePath) :
    imagePath_(imagePath)
{

}

// Constrcutor 2.
Image::Image(cv::Mat image) :
    image_(image)
{
    computeComparisonMathod();
}

// Compare image similarities based on computed color histograms.
double Image::compareToImage(Image &img)
{
//    return compareHistograms(img);
    return compareMeanValues(img);
}

// Copies this image to the other.
void Image::copyToImage(Image &img)
{
//    qDebug() << imagePath_;
    if(img.image().data && image_.data){
        cv::Mat tmpImage = image_.clone();
        cv::resize(tmpImage, tmpImage, cv::Size(img.image().cols, img.image().rows), 0, 0, cv::INTER_AREA);
        tmpImage.copyTo(img.image());
    } else {
        qWarning() << "Image: Cannot copy to/from an empty image.";
    }
}

// Loads image from the image path given by constructor and resizes it.
void Image::loadImage(cv::Size scaleSize)
{
    image_ = cv::imread(imagePath_.toStdString());

    if(image_.data){

        // Resize image to the given size.
        cv::resize(image_, image_, scaleSize, 0, 0, cv::INTER_AREA);

        computeComparisonMathod();
    } else {
        qWarning() << "Image: image was not properly loaded.";
    }
}

// Generates tile with random color and copies it into the OpenCV Mat image reference - used for debuging purposes.
void Image::generateRandomTile()
{
    // Generate ranom tile only when image is loaded.
    if(image_.data){
        cv::Mat(image_.rows, image_.cols, image_.type(), cv::Scalar(qrand() % 256, qrand() % 256, qrand() % 256)).copyTo(image_);
    }
}

// Compute comparison method.
void Image::computeComparisonMathod()
{
//    computeHistogram();
    computeMean();
}

// Computes color histogram for the image.
void Image::computeHistogram()
{
    if(image_.data){
        // Compute color histogram for the image.
        cv::Mat hsvImge;

        // Convert to HSV.
        cv::cvtColor(image_, hsvImge, cv::COLOR_BGR2HSV);

        // Using 50 bins for hue and 60 for saturation.
        int hBins = 25;
        int sBins = 30;
        int histSize[] = {hBins, sBins};

        // Hue varies from 0 to 179, saturation from 0 to 255.
        float hRanges[] = {0, 180};
        float sRanges[] = {0, 256};

        const float* ranges[] = {hRanges, sRanges};

        // Use the o-th and 1-st channels.
        int channels[] = {0, 1};

        // Calculate the histograms for the HSV image.
        cv::calcHist(&hsvImge, 1, channels, cv::Mat(), histImage_, 2, histSize, ranges, true, false);
        cv::normalize(histImage_, histImage_, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
    } else {
        qWarning() << "Image: Cannot compute histogram from an empty image!";
    }
}

// Computes average hash.
void Image::computeAverageHash()
{

}

// Computes mean values.
void Image::computeMean()
{

    if(image_.data){
        cv::Scalar meanBGR = cv::mean(image_);
        meanValue_ = meanBGR[0]/3.0 + meanBGR[1]/3.0 + meanBGR[2]/3.0;
    } else {
        qWarning() << "Image: Cannot compute mean from an empty image!";
    }

}

// Compare mean values
double Image::compareMeanValues(Image &img)
{
    return (255.0 - fabs(meanValue_ - img.mean()))/255.0;
}

// Compare color histograms.
double Image::compareHistograms(Image &img)
{
    if(histImage_.data && img.histImage().data){
        return cv::compareHist(histImage_, img.histImage(), 2);
    } else {
        return 0.0;
    }
}
