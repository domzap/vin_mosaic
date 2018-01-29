// Project libraries.
#include "ImageProvider.h"

// Qt libraries.
#include <QDebug>

// Set image.
void ImageProvider::setImage(QImage image){
    image_ = image;
}

// Request image virtual method implementation.
QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage result;
    if (requestedSize.isValid()) {
        if(image_.size() != requestedSize){
            result = image_.scaled(requestedSize, Qt::KeepAspectRatio);
        }
    } else {
        result = image_;
    }
    *size = result.size();
    return result;
}
