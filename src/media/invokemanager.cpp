#include "invokemanager.h"

#include <bb/system/InvokeRequest>
#include <bb/system/InvokeManager>

InvokeManager::InvokeManager(QObject* parent):
	QObject(parent)
{
}

InvokeManager::~InvokeManager()
{
}

void InvokeManager::invokeMediaPlayer(const QString& uri)
{
	bb::system::InvokeRequest cardRequest;
	cardRequest.setTarget("sys.browser");
	cardRequest.setUri(uri);
	bb::system::InvokeManager invokeManager;
	invokeManager.invoke(cardRequest);
}


