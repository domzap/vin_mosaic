#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

// Qt libraries.
#include <QObject>

// Project libraries.
#include "Image.h"

// OpenCV libraries.
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Defines.
#define MINIMUM_IMAGES_NECCESSARY 10

class ImageProcessor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool sourceImageLoaded READ sourceImageLoaded NOTIFY sourceImageLoadedChanged)
    Q_PROPERTY(int gridDivider READ gridDivider WRITE setGridDivider NOTIFY gridDividerChanged)
public:
    // --- CONSTRUCTORS / DESTRUCTOR ---

    //! Constructor.
    explicit ImageProcessor(QObject *parent = nullptr);

    // --- GETTERS ---

    //! Returns if source image is already loaded or not.
    bool sourceImageLoaded() const { return sourceImage_.data; }

    //! Returns number of images found in source directory.
    int imagesFound() const { return imagesList_.size(); }

    //! Returns grid divider value.
    int gridDivider() const { return gridDivider_; }

    // --- SETTERS

    //! Sets new grid divider value.
    void setGridDivider(int gridDivider);

    // --- PUBLIC METHODS ---

    //! Loads source image from the path given.
    bool loadSourceImage(QString imagePath);

    //! Searches for images stored in the source directory.
    void loadSourceDirectory(QString directoryPath);

signals:
    // --- SIGNALS ---

    //! Source image loaded changed signal.
    void sourceImageLoadedChanged();

    //! Preview image changed signal.
    void previewImageChanged(cv::Mat image);

    //! Grid divider value changed signal.
    void gridDividerChanged();

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
    QList<Image> imagesList_;                   //!< List of source directory images.
    int gridDivider_;                           //!< Grid divider.


};

#endif // IMAGEPROCESSOR_H
