#ifndef IMAGEUPDATER_H
#define IMAGEUPDATER_H

// Project libraries.
#include "ImageProvider.h"

// OpenCV libraries.
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Qt libraries.
#include <QTimer>
#include <QDebug>

//! Image updater class.
class ImageUpdater : public QObject
{
    Q_OBJECT

public:
    // --- GETTERS ---

    //! Get image provider reference.
    ImageProvider * imageProvider() { return &imageProvider_; }

public slots:

    // --- PUBLIC SLOTS ---

    //! Set image to display.
    void setImage(cv::Mat const& displayImage);

signals:
    // --- SIGNALS ---

    //! Signal is emitted whenever image is updated in image provider.
    void imageUpdated();

protected:
    // --- PROTECTED CLASS VARIABLES ---

    ImageProvider imageProvider_;           //!< Image provider object.

};

#endif // IMAGEUPDATER_H

