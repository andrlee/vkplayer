#ifndef VKONTAKTEMESSAGES_H_
#define VKONTAKTEMESSAGES_H_

#include <QNetworkRequest>

#define REQUEST_TYPE "type"

namespace vkontaktemessages
{
	enum RequestType
    {
    	RetriveMessages = 0,
    	RetriveMusic,
    	DownloadAudioTrack,
    	SearchMusic,
    	AddMusic,
    	DeleteMusic,
    	RetrieveVideos,
    	SearchVideo,
    	AddVideo,
    	DeleteVideo
    };

	QNetworkRequest createRetrieveMessagesRequest(const QString &access_token, int count);
	QVariantList parseRetrievedMessages(const QByteArray &result);

	QNetworkRequest createRetrieveAudioRequest(const QString &access_token, int count);
	QVariantList parseRetrievedAudioList(const QByteArray &result, const QString& userId);

	QUrl saveAudioTrack(const QByteArray &result, const QUrl& source);

	QNetworkRequest createSearchMusicRequest(const QString &access_token, const QString& text, int count);

	QNetworkRequest createAddMusicRequest(const QString &access_token, const QString& aid, const QString& oid);
	bool musicAdded(const QByteArray &result);

	QNetworkRequest createDeleteMusicRequest(const QString &access_token, const QString& aid, const QString& oid);
	bool musicDeleted(const QByteArray &result);

	QNetworkRequest createRetrieveVideosRequest(const QString &access_token, const QString& userId, int count);
	QVariantList parseRetrievedVideosList(const QByteArray &result, const QString& userId);

	QNetworkRequest createSearchVideoRequest(const QString &access_token, const QString& text, int count);
	QVariantList parseSearchedVideoList(const QByteArray &result, const QString& userId);

	QNetworkRequest createAddVideoRequest(const QString &access_token, const QString& vid, const QString& oid);
	bool videoAdded(const QByteArray &result);

	QNetworkRequest createDeleteVideoRequest(const QString &access_token, const QString& vid, const QString& oid);
	bool videoDeleted(const QByteArray &result);
}

#endif /* VKONTAKTEMESSAGES_H_ */
