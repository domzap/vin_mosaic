#ifndef IMAGE_H
#define IMAGE_H

// Qt libraries.
#include <QString>

// OpenCV libraries.
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Image
{
public:
    // --- CONSTRUCTORS / DESTRUCTOR ---

    //! Constrcutor 1.
    Image(QString imagePath);

    //! Constrcutor 2.
    Image(cv::Mat image);

    // --- GETTERS ---

    //! Returns if the image is loaded or not.
    bool isLoaded() const { return image_.data; }

    //! Returns OpenCV Mat image reference.
    cv::Mat& image() { return image_; }

    //! Returns computed color histogram.
    cv::MatND& histImage() { return histImage_; }

    //! Returns image mean value.
    double mean() const { return meanValue_; }

    // --- PUBLIC METHODS ---

    //! Compare image similarities based on computed color histograms.
    double compareToImage(Image& img);

    //! Copies this image to the other.
    void copyToImage(Image& img);

    //! Loads image from the image path given by constructor and resizes it.
    void loadImage(cv::Size scaleSize);

    //! Generates tile with random color and copies it into the OpenCV Mat image reference - used for debuging purposes.
    void generateRandomTile();

protected:
    // --- PROTECTED METHODS ---

    //! Compute comparison method.
    void computeComparisonMathod();

    //! Computes color histogram for the image.
    void computeHistogram();

    //! Computes average hash.
    void computeAverageHash();

    //! Computes mean values.
    void computeMean();

    //! Compare mean values.
    double compareMeanValues(Image& img);

    //! Compare color histograms.
    double compareHistograms(Image& img);


protected:
    // --- PROTECTED CLASS VARIABLES ---

    QString imagePath_;             //!< Path to the image.
    cv::Mat image_;                 //!< OpenCV Mat image instance.
    cv::MatND histImage_;           //!< Image histogram.
    double meanValue_;              //!< Image mean value.

};

#endif // IMAGE_H
