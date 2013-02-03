
#ifndef NETIMAGETRACKER_H_
#define NETIMAGETRACKER_H_

#include "netimagemanager.h"
#include <bb/cascades/ImageTracker>

using namespace bb::cascades;

/**
 * The NetImageTracker is used so that Cascades can be informed when an image is downloaded
 * via a NetImageManager.
 */
class NetImageTracker: public bb::cascades::ImageTracker
{
Q_OBJECT

    /**
     * The NetImageManager property points to a manager that is used for downloading and caching
     * images.
     */
Q_PROPERTY(NetImageManager *manager READ manager WRITE setManager NOTIFY managerChanged)

    /**
     * Sets the NetImageTracker source, the remote networked url that points to the image.
     * You need to set one as an attached object in your QML files.
     */
Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    /**
     * Constructor; Sets up the net image view
     * @param parent The parent Container.
     */
    NetImageTracker(QObject *parent = 0);

public slots:

    /**
     * Setter function for setting the trackers net image manager
     *
     * @param manager in manager that we're going to use.
     */
    void setManager(NetImageManager *manager);

    /**
     * Getter for Trackers Manager object
     * @return
     */
    NetImageManager *manager();

    /**
     * Setter for the source object.
     *
     * @param source in string with path to the source.
     */
    void setSource(const QString source);

    /**
     * Getter for source
     * @return a string of the source
     */
    QString source();

signals:
    /**
     * Signal that emits when the source have changed, wont happen automatically
     *
     * @param source the new source
     */
    void sourceChanged(QString source);

    /**
     * signal that is emitted if the manager is changed, will not happen automatically.
     *
     * @param imageCache the new NetImageManager we want to have .
     */
    void managerChanged(NetImageManager *imageCache);

private slots:
    /**
     * Emitted when we are done with the setup of this class
     */
    void onCreationCompleted();

    /**
     * Emitted when we have a image that is ready for consumption
     *
     * @param filePath the path to the image that we can do what we want with
     * @param imageName the actual name of the file, useful!
     */
    void onImageReady(const QString filePath, const QString imageName);

private:
    QString mSource;

    NetImageManager *mManager;
    bool mIsCreated;
};


#endif /* NETIMAGETRACKER_H_ */
