#ifndef MOSIACCREATOR_H
#define MOSIACCREATOR_H

// Qt libraries.
#include <QObject>
#include <QQuickView>

// Standard libraries.

// Project libraries.
#include "ImageUpdater.h"
#include "ImageProcessor.h"

class MosiacCreator : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString notificationString READ notificationString NOTIFY notificationStringChanged)
public:
    // --- CONSTRUCTOR / DESTRUCTOR ---

    //! Constructor.
    explicit MosiacCreator(QObject *parent = nullptr);

    // --- GETTERS ---

    //! Returns notification string to be shown in GUI.
    QString notificationString() const { return notificationString_; }

    // --- SETTERS ---

    //! Sets notification string.
    void setNotificationString(QString notificationString);

    // --- PUBLIC METHODS ---

    // TODO - remove in final build.
    void debugProcess();

signals:

    //! Notification string changed signal.
    void notificationStringChanged();

public slots:

    // --- GUI SLOTS ---

    //! Slot is called whenever new source image is chosen.
    void onSourceImageChosen(QVariant imagePath);

    //! Slot is called whenever new source image directory is chosen.
    void onSourceDirectoryChosen(QVariant directoryPath);

    //! Slot is called whenever mosiac image is to be computed.
    void onComputeMosaic();

    //! Slot is called whenever preview image is to be exported.
    void onExportImage(QVariant exportFilePath);

protected:
    // --- PROTECTED METHODS ---

    //! Initialize all necessary components.
    void init();

    //! Create all necessary Qt connections.
    void makeConnections();

    //! Start MosiacCreator.
    void start();

protected:
    // --- PROTECTED CLASS VARIABLES ---

    QQuickView * view_;                         //!< GUI qml view.
    ImageUpdater imageUpdater_;                 //!< Image preview updater.
    ImageProcessor imageProcessor_;             //!< Image processor where all mosiac processing is done.
    QString notificationString_;                //!< General notification string to be shown in GUI.
};

#endif // MOSIACCREATOR_H
