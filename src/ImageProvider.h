#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

// Qt libraries.
#include <QQuickImageProvider>

//! Image provider class.
class ImageProvider : public QQuickImageProvider
{
public:
    // --- CONSTRUCTORS / DESTRUCTORS ---

    //! Contructor.
    ImageProvider() : QQuickImageProvider(QQuickImageProvider::Image){
        image_ = QImage(160, 120, QImage::Format_RGB888);
    }

    //! Destructor.
    virtual ~ImageProvider() { ; }

    // --- SETTERS ---

    //! Set image.
    void setImage(QImage image);

    // --- PUBLIC METHODS ---
    //! Request image virtual method implementation.
    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

protected:
    // --- PROTECTED CLASS VARIABLES ---

    QImage image_;          //!< Image to display by requestImage method.
};

#endif // IMAGEPROVIDER_H
