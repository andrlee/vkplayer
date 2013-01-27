/**
 * Copyright (c) 2012 Nokia Corporation.
 */

#include <QtDeclarative/QtDeclarative>
#include <QtCore/QtPlugin>

#include "socialconnectplugin.h"
#include "webinterface.h"

#include "vkontakteconnection.h"

void SocialConnectPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<WebInterface>(uri, 1, 0, "WebInterface");
    qmlRegisterType<VKontakteConnection>(uri, 1, 0, "VKontakteConnection");
}

Q_EXPORT_PLUGIN2(socialconnect, SocialConnectPlugin)
