// Project libraries.
#include "ImageUpdater.h"

// Qt libraries.
#include <QImage>

// Convert OpenCV Mat image to Qt QImage image. Deep copy is created and returned.
QImage Mat2QImage(cv::Mat const& src)
{
    QImage dest;
    cv::Mat temp;

    if(src.empty()){
        return dest;
    }

    // Convert source to RGB color space.
    cv::cvtColor(src, temp,CV_BGR2RGB);

    // Create QImage.
    dest = QImage((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);

    // Force deep copy and return (performs a deep copy of the shared pixel data).
    dest.bits();
    return dest;
}

// Set image to display.
void ImageUpdater::setImage(cv::Mat const& displayImage){
    imageProvider_.setImage(Mat2QImage(displayImage));
    emit imageUpdated();
}
