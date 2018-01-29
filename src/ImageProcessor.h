#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

// Qt libraries.
#include <QObject>

// Project libraries.

// OpenCV libraries.
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ImageProcessor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool sourceImageLoaded READ sourceImageLoaded NOTIFY sourceImageLoadedChanged)
public:
    // --- CONSTRUCTORS / DESTRUCTOR ---

    //! Constructor.
    explicit ImageProcessor(QObject *parent = nullptr);

    // --- GETTERS ---

    //! Returns if source image is already loaded or not.
    bool sourceImageLoaded() const { return sourceImage_.data; }

    // --- PUBLIC METHODS ---

    //! Loads source image from the path given.
    bool loadSourceImage(QString imagePath);

signals:
    // --- SIGNALS ---

    //! Source image loaded changed signal.
    void sourceImageLoadedChanged();

    //! Preview image changed signal.
    void previewImageChanged(cv::Mat image);

public slots:
    // --- PUBLIC SLOTS ---

protected:
    // --- PROTECTED METHODS ---

    //! Sets previewImage.
    void setPreviewImage(cv::Mat& image);

protected:
    // --- PROTECTED CLASS VARIABLES ---

    cv::Mat sourceImage_;                       //!< Source image from which everything is computed.
    cv::Mat previewImage_;                      //!< Image that is shown and updated in GUI.


};

#endif // IMAGEPROCESSOR_H
