#ifndef IMAGE_H
#define IMAGE_H

// Qt libraries.
#include <QString>

class Image
{
public:
    // --- CONSTRUCTORS / DESTRUCTOR ---

    //! Constrcutor.
    Image(QString imagePath);

protected:
    // --- PROTECTED CLASS VARIABLES ---

    QString imagePath_;             //!< Path to the image.
};

#endif // IMAGE_H
