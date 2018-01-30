#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

// Qt libraries.
#include <QObject>

// Project libraries.
#include "Image.h"

// OpenCV libraries.
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Standard libraries.
#include <memory>

// Defines.
#define MINIMUM_IMAGES_NECCESSARY 10
#define MINIMUM_GRID_DIVIDER 10

class ImageProcessor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool sourceImageLoaded READ sourceImageLoaded NOTIFY sourceImageLoadedChanged)
    Q_PROPERTY(int gridDivider READ gridDivider WRITE setGridDivider NOTIFY gridDividerChanged)
    Q_PROPERTY(int imagesFound READ imagesFound NOTIFY imagesFoundChanged)
    Q_PROPERTY(int minImages READ minImages NOTIFY sourceImageLoadedChanged)
    Q_PROPERTY(int minGrid READ minGrid NOTIFY sourceImageLoadedChanged)
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

    //! Returns minimum images necessary value.
    int minImages() const { return MINIMUM_IMAGES_NECCESSARY; }

    //! Returns minimum grid size value.
    int minGrid() const { return MINIMUM_GRID_DIVIDER; }

    // --- SETTERS ---

    //! Sets new grid divider value.
    void setGridDivider(int gridDivider);

    // --- PUBLIC METHODS ---

    //! Loads source image from the path given.
    bool loadSourceImage(QString imagePath);

    //! Searches for images stored in the source directory.
    void loadSourceDirectory(QString directoryPath);

    //! Computes mosiac image.
    void computeMosaic();

    //! Exports currently displayed preview image.
    bool exportImage(QString imageFilePath);

signals:
    // --- SIGNALS ---

    //! Source image loaded changed signal.
    void sourceImageLoadedChanged();

    //! Preview image changed signal.
    void previewImageChanged(cv::Mat image);

    //! Grid divider value changed signal.
    void gridDividerChanged();

    //! Images found changed signal.
    void imagesFoundChanged();

public slots:
    // --- PUBLIC SLOTS ---

protected:
    // --- PROTECTED METHODS ---

    //! Sets previewImage.
    void setPreviewImage(cv::Mat& image);

    //! Computes mosiac grid.
    void computeGrid();

protected:
    // --- PROTECTED CLASS VARIABLES ---

    cv::Mat sourceImage_;                       //!< Original source image from which everything is computed.
    cv::Mat workingImage_;                      //!< Temporary image to work with.
    cv::Mat previewImage_;                      //!< Image that is shown and updated in GUI.
    QList<std::shared_ptr<Image>> imagesList_;  //!< List of source directory images.
    QList<std::shared_ptr<Image>> imageCells_;  //!< List of source image cells divided by computed grid.
    int gridDivider_;                           //!< Grid divider.
    double gridCellWidth_;                      //!< Grid cell width.
    double gridCellHeight_;                     //!< Grid cell height



};

#endif // IMAGEPROCESSOR_H
