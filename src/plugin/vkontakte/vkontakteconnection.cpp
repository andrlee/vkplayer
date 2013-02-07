#include "vkontakteconnection.h"
#include "vkontaktemessages.h"

#include <QDebug>
#include <QMap>
#include <QNetworkReply>
#include <QSettings>
#include <QStringList>
#include <QUrl>

#include <bb/system/SystemToast>

#include <pthread.h>

#include "webinterface.h"

#define CLIENT_ID "3269076"
#define ACCESS_TOKEN_KEY "vk_access_token"
#define USER_ID_KEY "vk_user_id"

#define OAUTH_URL_FORMAT "%1?client_id=%2&redirect_uri=%3&display=popup&response_type=token"
#define OAUTH_URL "https://oauth.vk.com/authorize"
#define REDIRECT_URL "https://oauth.vk.com/blank.html"
#define ACCESS_TOKEN "access_token"
#define USER_ID "user_id"
#define ERROR_STR "error"

//Error Codes
#define NO_ERROR 0
#define AUTH_ERROR 5

static pthread_mutex_t s_mutex = PTHREAD_MUTEX_INITIALIZER;

using namespace vkontaktemessages;
using namespace bb::system;

VKontakteConnection::VKontakteConnection(QObject *parent) :
    SocialConnection(parent),
    m_clientId(CLIENT_ID),
    m_state(NotLogged)
{
	connect(&m_networkManager, SIGNAL(finished(QNetworkReply*)),
	        this, SLOT(replyFinished(QNetworkReply*)));
}

VKontakteConnection::~VKontakteConnection()
{
}

QString VKontakteConnection::clientId() const
{
    return m_clientId;
}

void VKontakteConnection::setClientId(const QString &clientId)
{
	if (m_clientId != clientId) {
		m_clientId = clientId;
		emit clientIdChanged(m_clientId);
	}
}

QString VKontakteConnection::userId() const
{
    return m_userId;
}

void VKontakteConnection::setUserId(const QString &userId)
{
	if (m_userId != userId) {
		m_userId = userId;
		emit userIdChanged(m_userId);
	}
}

QString VKontakteConnection::accessToken() const
{
    return m_accessToken;
}

void VKontakteConnection::setAccessToken (const QString &accessToken)
{
    if (m_accessToken != accessToken) {
        m_accessToken = accessToken;
        emit accessTokenChanged(m_accessToken);
    }
}

QStringList VKontakteConnection::permissions() const
{
    return m_permissions;
}

void VKontakteConnection::setPermissions(const QStringList &permissions)
{
    if (m_permissions != permissions) {
        m_permissions = permissions;
        emit permissionsChanged(permissions);
    }
}

bool VKontakteConnection::isAuthorized()
{
    bool ret = true;

    if (m_accessToken.isEmpty() || m_clientId.isEmpty()) {
        ret = false;
    }

    // Emit signal if access token has been expired during the session.
    if (!ret) {
        emit authorizedChanged(false);
    }

    return ret;
}

bool VKontakteConnection::sessionValidated()
{
    if (!isAuthorized() || !authenticated()) {
        qWarning() << "Error. VKConnection not authenticated.";
        return false;
    }

    if (busy()) {
        qWarning() << "VKConnection busy.";
        return false;
    }

    return true;
}

VKontakteConnection::State VKontakteConnection::state() const
{
    return m_state;
}

void VKontakteConnection::setState(State state)
{
    if (m_state != state) {
        m_state = state;
    }
}

void VKontakteConnection::setWebInterfaceActive(const bool active)
{
    WebInterface *webInterface = qobject_cast<WebInterface*>(SocialConnection::webInterface());
    if (webInterface) {
        webInterface->setActive(active);
    }
}

bool VKontakteConnection::authenticate()
{
	bool ret = isAuthorized();

    if(!ret && !busy())  {
        WebInterface *webInterface = qobject_cast<WebInterface*>(SocialConnection::webInterface());
        if (webInterface) {
        	setState(RequestAccessToken);
			setBusy(true);
			setTransmitting(true);

        	QString url = QString(OAUTH_URL_FORMAT).arg(OAUTH_URL).arg(clientId()).arg(REDIRECT_URL);

			// Add permissions to the authorization URL
			if (m_permissions.count() > 0) {
				url += "&scope=";
				url += m_permissions.join(",");
			}

			qDebug() << url;

			webInterface->setUrl(url);
			ret = true;
        }
    }
    else if (ret){
		setAuthenticated(true);
		QMetaObject::invokeMethod(this, "authenticateCompleted", Qt::QueuedConnection, Q_ARG(bool, true));
	}

    return ret;
}

bool VKontakteConnection::deauthenticate()
{
    setState(NotLogged);
    setName("");
    setAuthenticated(false);

    QMetaObject::invokeMethod(this, "deauthenticateCompleted",
                              Qt::QueuedConnection, Q_ARG(bool, true));

    return true;
}

void VKontakteConnection::onUrlChanged(const QUrl &url)
{
    SocialConnection::onUrlChanged(url);

    switch(m_state)
    {
		case Authenticate:
			checkAuthenticationUrl(url);
			break;
		case RequestAccessToken:
			qDebug() << "Request token";
        	setState(Authenticate);
			break;
		default:
			break;
    }
}

void VKontakteConnection::authorize(const QString &accessToken, const QString &userId)
{
    setAccessToken(accessToken);
    setUserId(userId);

    qDebug() << "VK::authorize - authorized with access token:"
             << accessToken;
}

void VKontakteConnection::checkAuthenticationUrl(const QUrl &url)
{
    // Ignore all but URLs starting with VK successful URL.
    if (!url.toString().startsWith(REDIRECT_URL)) {
        qDebug() << "Invalid URL: " << url.toString();
    	return;
    }

    qDebug() << "Valid URL: " << url.toString();

    if (url.toString().contains(ACCESS_TOKEN)) {

        // Little trick for converting fragment to query items.
        QString authUrl = url.toString();
        authUrl.replace("#", "?");
        const QUrl newUrl(authUrl);

        // Authorize VK with access token and expiration time.
        authorize(newUrl.queryItemValue(ACCESS_TOKEN),
        		  newUrl.queryItemValue(USER_ID));

        // At this point web interface is not needed anymore.
        setWebInterfaceActive(false);

        setState(Logged);
        setAuthenticated(true);
        storeCredentials();
		emit authenticateCompleted(true);
    }

    // TODO: Error handling to be improved later if we will to pass error
    // strings to the user in the future.
    else if (url.toString().contains(ERROR_STR)) {

        // At this point web interface is not needed anymore.
        setWebInterfaceActive(false);

        setState(NotLogged);
        setBusy(false);
        setTransmitting(false);
        emit authenticateCompleted(false);
    }
}

int VKontakteConnection::checkReplyErrors(QNetworkReply* reply)
{
	setTransmitting(false);
    setBusy(false);
    const int requestError = reply->error();

    if (requestError == QNetworkReply::AuthenticationRequiredError) {
        setState(NotLogged);
        setAuthenticated(false);
    }
    return requestError;
}

void VKontakteConnection::checkVKConnectionErrors(const QByteArray& reply)
{
	//Check for VK connection errors
	QString error_msg;
	int error = getVKConnectionErrorCode(reply, error_msg);
	if (NO_ERROR != error)
	{
		SystemToast* toast = new SystemToast;
		toast->setBody(error_msg);
		toast->exec();
		delete toast;
	}

	if (AUTH_ERROR == error)
	{
		setState(NotLogged);
		setAuthenticated(false);
	}
}

bool VKontakteConnection::postMessage(const QVariantMap &message)
{
	Q_UNUSED(message);
	return false;
}

bool VKontakteConnection::retrieveMessageCount()
{
	return false;
}

bool VKontakteConnection::retrieveMessages(const QString &from, const QString &to, int max)
{
	Q_UNUSED(to);
	Q_UNUSED(from);

	if (!sessionValidated()) {
		return false;
	}

	setBusy(true);
	setTransmitting(true);
	QNetworkRequest request = createRetrieveMessagesRequest(accessToken(), max);

	QNetworkReply* reply = m_networkManager.get(request);
	if (reply != NULL)
		reply->setProperty(REQUEST_TYPE, RetriveMessages);

    return true;
}

void VKontakteConnection::cancel()
{
}

void VKontakteConnection::replyFinished(QNetworkReply* reply)
{
	if (NULL == reply)
	{
		return;
	}

	pthread_mutex_lock(&s_mutex);
	const int requestError = checkReplyErrors(reply);

	QByteArray payload = reply->readAll();
	checkVKConnectionErrors(payload);

	switch(reply->property(REQUEST_TYPE).toInt())
	{
		case RetriveMessages:
			 emit retrieveMessagesCompleted(requestError == QNetworkReply::NoError,
				  parseRetrievedMessages(payload));
			break;
		case RetriveMusic:
			emit musicLoaded(requestError == QNetworkReply::NoError,
				 parseRetrievedAudioList(payload, m_userId));
			break;
		case DownloadAudioTrack:
			emit audioTrackSaved(requestError == QNetworkReply::NoError,
				 vkontaktemessages::saveAudioTrack(payload, reply->url()));
			break;
		case SearchMusic:
			emit searchMusicLoaded(requestError == QNetworkReply::NoError,
				 parseRetrievedAudioList(payload, m_userId));
			break;
		case AddMusic:
			if (requestError == QNetworkReply::NoError)
			{
				emit musicAdded(vkontaktemessages::musicAdded(payload));
			}
			else
			{
				emit musicAdded(false);
			}
			break;
		case DeleteMusic:
			if (requestError == QNetworkReply::NoError)
			{
				emit musicDeleted(vkontaktemessages::musicDeleted(payload));
			}
			else
			{
				emit musicDeleted(false);
			}
			break;
		case RetrieveVideos:
			emit videosLoaded(requestError == QNetworkReply::NoError,
							  parseRetrievedVideosList(payload, m_userId));
			break;
		case SearchVideo:
			emit searchVideoLoaded(requestError == QNetworkReply::NoError,
								   parseSearchedVideoList(payload, m_userId));
			break;
		case AddVideo:
			if (requestError == QNetworkReply::NoError)
			{
				emit videoAdded(vkontaktemessages::videoAdded(payload));
			}
			else
			{
				emit videoAdded(false);
			}
			break;
		case DeleteVideo:
			if (requestError == QNetworkReply::NoError)
			{
				emit videoDeleted(vkontaktemessages::videoDeleted(payload));
			}
			else
			{
				emit videoDeleted(false);
			}
			break;
		default:
			break;
	}

	reply->deleteLater();
	pthread_mutex_unlock(&s_mutex);
}

bool VKontakteConnection::storeCredentials()
{
	if (!authenticated()) {
		qWarning() << "Error. Cannot store credentials while not authenticated.";
		return false;
	}

	QSettings settings;
	settings.setValue(ACCESS_TOKEN_KEY, m_accessToken);
	settings.setValue(USER_ID_KEY, m_userId);

	qDebug() << "Store: Access token:" << m_accessToken;

	return settings.status() == QSettings::NoError;
}

bool VKontakteConnection::restoreCredentials()
{
	QSettings settings;
	setAccessToken(settings.value(ACCESS_TOKEN_KEY).toString());
	setUserId(settings.value(USER_ID_KEY).toString());

	qDebug() << "Restore: Access token for user" << m_accessToken;

	// Check is VK authenticated.
	if (isAuthorized() && !authenticated()) {
		setAuthenticated(true);
		emit authenticateCompleted(true);
	}

	return settings.status() == QSettings::NoError;
}

bool VKontakteConnection::removeCredentials()
{
	m_accessToken.clear();

	QSettings settings;
	settings.remove(ACCESS_TOKEN_KEY);
	settings.remove(USER_ID_KEY);

	return settings.status() == QSettings::NoError;
}

bool VKontakteConnection::saveAudioTrack(const QUrl& url)
{
	if (!sessionValidated()) {
		return false;
	}

	setBusy(true);
	setTransmitting(true);

	QNetworkReply* reply = m_networkManager.get(QNetworkRequest(url));
	if (reply != NULL)
		reply->setProperty(REQUEST_TYPE, DownloadAudioTrack);

	return true;
}

bool VKontakteConnection::loadMusic(int count)
{
	if (!sessionValidated()) {
		return false;
	}

	setBusy(true);
	setTransmitting(true);
	QNetworkRequest request = createRetrieveAudioRequest(accessToken(), count);

	QNetworkReply* reply = m_networkManager.get(request);
	if (reply != NULL)
		reply->setProperty(REQUEST_TYPE, RetriveMusic);

	return true;
}

bool VKontakteConnection::search(const QString& text, int count)
{
	if (!sessionValidated()) {
		return false;
	}

	setBusy(true);
	setTransmitting(true);
	QNetworkRequest request = createSearchMusicRequest(accessToken(), text, count);

	QNetworkReply* reply = m_networkManager.get(request);
	if (reply != NULL)
		reply->setProperty(REQUEST_TYPE, SearchMusic);

	return true;
}

bool VKontakteConnection::addMusic(const QString& aid, const QString& oid)
{
	if (!sessionValidated()) {
		return false;
	}

	setBusy(true);
	setTransmitting(true);
	QNetworkRequest request = createAddMusicRequest(accessToken(), aid, oid);

	QNetworkReply* reply = m_networkManager.get(request);
	if (reply != NULL)
		reply->setProperty(REQUEST_TYPE, AddMusic);

	return true;
}

bool VKontakteConnection::deleteMusic(const QString& aid)
{
	if (!sessionValidated()) {
		return false;
	}

	setBusy(true);
	setTransmitting(true);
	QNetworkRequest request = createDeleteMusicRequest(accessToken(), aid, m_userId);

	QNetworkReply* reply = m_networkManager.get(request);
	if (reply != NULL)
		reply->setProperty(REQUEST_TYPE, DeleteMusic);

	return true;
}

bool VKontakteConnection::loadVideos(int count)
{
	if (!sessionValidated()) {
		return false;
	}

	setBusy(true);
	setTransmitting(true);
	QNetworkRequest request = createRetrieveVideosRequest(accessToken(), m_userId, count);

	QNetworkReply* reply = m_networkManager.get(request);
	if (reply != NULL)
		reply->setProperty(REQUEST_TYPE, RetrieveVideos);

	return true;
}

bool VKontakteConnection::searchVideo(const QString& text, int count)
{
	if (!sessionValidated()) {
		return false;
	}

	setBusy(true);
	setTransmitting(true);
	QNetworkRequest request = createSearchVideoRequest(accessToken(), text, count);

	QNetworkReply* reply = m_networkManager.get(request);
	if (reply != NULL)
		reply->setProperty(REQUEST_TYPE, SearchVideo);

	return true;
}

bool VKontakteConnection::addVideo(const QString& vid, const QString& oid)
{
	if (!sessionValidated()) {
		return false;
	}

	setBusy(true);
	setTransmitting(true);
	QNetworkRequest request = createAddVideoRequest(accessToken(), vid, oid);

	QNetworkReply* reply = m_networkManager.get(request);
	if (reply != NULL)
		reply->setProperty(REQUEST_TYPE, AddVideo);

	return true;
}

bool VKontakteConnection::deleteVideo(const QString& vid)
{
	if (!sessionValidated()) {
		return false;
	}

	setBusy(true);
	setTransmitting(true);
	QNetworkRequest request = createDeleteVideoRequest(accessToken(), vid, m_userId);

	QNetworkReply* reply = m_networkManager.get(request);
	if (reply != NULL)
		reply->setProperty(REQUEST_TYPE, DeleteVideo);

	return true;
}

