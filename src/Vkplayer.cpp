#include "Vkplayer.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

#include "socialconnectplugin.h"
#include "vkontakteplaylist.h"
#include "volumeslider.h"
#include "netimagetracker.h"
#include "netimagemanager.h"
#include "invokemanager.h"
#include "leadingvisual.h"

#include <QTimer>

using namespace bb::cascades;

Vkplayer::Vkplayer(bb::cascades::Application *app)
: QObject(app)
{
	// Register social connect plugin
	SocialConnectPlugin plugin;
	plugin.registerTypes("SocialConnect");

	qmlRegisterType<QTimer>("bb.cascades", 1, 0, "QTimer");
	qmlRegisterType<VolumeSlider>("bb.cascades", 1, 0, "VolumeSlider");
	qmlRegisterType<NetImageTracker>("bb.cascades", 1, 0, "NetImageTracker");
	qmlRegisterType<NetImageManager>("bb.cascades", 1, 0, "NetImageManager");
	qmlRegisterType<LeadingVisual>("bb.cascades", 1, 0, "LeadingVisual");

	// create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    //Export custom data model
    VkontaktePlaylist* musicPlaylist = new VkontaktePlaylist(this);
    qml->setContextProperty("_musicPlaylist", musicPlaylist);

    VkontaktePlaylist* videoPlaylist = new VkontaktePlaylist(this);
	qml->setContextProperty("_videoPlaylist", videoPlaylist);

	InvokeManager* invoke = new InvokeManager(this);
	qml->setContextProperty("_invoke", invoke);

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app->setScene(root);
}
