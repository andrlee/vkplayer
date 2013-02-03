#ifndef VKONTAKTECONNECTION_H_
#define VKONTAKTECONNECTION_H_

#include <QNetworkAccessManager>
#include <QString>
#include <QVariantMap>
#include <QtCore/QStringList>
#include <QDateTime>

#include "socialconnection.h"

class VKontakteConnection : public SocialConnection
{
    Q_OBJECT

    Q_PROPERTY(QString accessToken READ accessToken WRITE setAccessToken NOTIFY accessTokenChanged)
    Q_PROPERTY(QString clientId READ clientId WRITE setClientId NOTIFY clientIdChanged)
    Q_PROPERTY(QString userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(QStringList permissions READ permissions WRITE setPermissions NOTIFY permissionsChanged)

public:
    explicit VKontakteConnection(QObject *parent = 0);
    virtual ~VKontakteConnection();

public:
    QString clientId() const;
    void setClientId(const QString &clientId);
    QString userId() const;
	void setUserId(const QString &userId);
    QString accessToken() const;
    void setAccessToken (const QString &accessToken);
    QStringList permissions() const;
	void setPermissions(const QStringList &permissions);

    // Virtual method implementations from the SocialConnect base class.
    bool authenticate();
    bool deauthenticate();
    bool postMessage(const QVariantMap &message);
    bool retrieveMessageCount();
    bool retrieveMessages(const QString &from, const QString &to, int max);
    void cancel();

    bool storeCredentials();
    bool restoreCredentials();
    bool removeCredentials();

public slots: // Operations unique to Vkontakte.
    bool loadMusic(int count);
    bool search(const QString& text, int count);
    bool addMusic(const QString& aid, const QString& oid);
    bool deleteMusic(const QString& aid);
	bool saveAudioTrack(const QUrl& url);

	bool loadVideos(int count);
	bool searchVideo(const QString& text, int count);
	bool addVideo(const QString& vid, const QString& oid);
    bool deleteVideo(const QString& vid);

signals:
	void clientIdChanged(const QString &clientId);
    void userIdChanged(const QString &userId);
	void accessTokenChanged(const QString &accessToken);
	void permissionsChanged(const QStringList &permissions);
    void authorizedChanged(const bool authorized);

    void musicLoaded(bool success, const QVariantList &audioList);
    void audioTrackSaved(bool success, const QUrl& uri);
    void searchMusicLoaded(bool success, const QVariantList &audioList);
    void musicAdded(bool success);
    void musicDeleted(bool success);

    void videosLoaded(bool success, const QVariantList &videoList);
    void searchVideoLoaded(bool success, const QVariantList &videoList);
    void videoAdded(bool success);
    void videoDeleted(bool success);

protected slots:
    void onUrlChanged(const QUrl &url);

private slots:
	void replyFinished(QNetworkReply* reply);

private:    // Members
    enum State
    {
        NotLogged = 0,
        RequestAccessToken,
        Authenticate,
        Logged
    };

    State state() const;
    void setState(State state);

    bool isAuthorized();
    void authorize(const QString &accessToken, const QString &userId);
    void checkAuthenticationUrl(const QUrl &url);
    void setWebInterfaceActive(const bool active);
    bool sessionValidated();
    int checkReplyErrors(QNetworkReply* reply);

private:    // Data
    QNetworkAccessManager m_networkManager;

    QString m_accessToken;
    QString m_clientId;
    QString m_userId;
    QStringList m_permissions;
    State m_state;
};


#endif /* VKONTAKTECONNECTION_H_ */
