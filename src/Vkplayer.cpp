#include "Vkplayer.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

#include "socialconnectplugin.h"
#include "vkontakteplaylistmodel.h"

#include <QTimer>

using namespace bb::cascades;

Vkplayer::Vkplayer(bb::cascades::Application *app)
: QObject(app)
{
	// Register social connect plugin
	SocialConnectPlugin plugin;
	plugin.registerTypes("SocialConnect");

	qmlRegisterType<QTimer>("bb.cascades", 1, 0, "QTimer");
	qmlRegisterType<VkontaktePlaylistModel>("bb.cascades", 1, 0, "VkontaktePlaylistModel");

	// create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app->setScene(root);
}
