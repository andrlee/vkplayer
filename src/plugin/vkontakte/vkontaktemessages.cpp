#include "vkontaktemessages.h"

#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

#include <iostream>

#define MY_PLAYLIST "my_playlist.m3u"
#define SEARCH_PLAYLIST "search_playlist.m3u"

#define URL_FORMAT "https://api.vk.com/method/%1?access_token=%2"
#define WALL_GET "wall.get"
#define AUDIO_GET "audio.get"
#define AUDIO_SEARCH "audio.search"
#define AUDIO_ADD "audio.add"
#define AUDIO_DELETE "audio.delete"

// Wall messages tags
#define MESSAGE_ID "id"
#define MESSAGE_DATE "date"
#define MESSAGE_TEXT "text"
#define MESSAGE_COMMENTS "comments"
#define MESSAGE_LIKES "likes"
#define MESSAGE_REPOSTS "reposts"
#define MESSAGE_ATTACHMENTS "attachments"
#define IS_ONLINE "online"
#define MESSAGE_REPLY_COUNT "reply_count"
#define MESSAGE_MEDIA "media"

#define ATTACHMENT_TYPE "type"
#define ATTACHMENT_LINK "link"

#define OFFLINE 0
#define ONLINE 1

// Audio tags
#define AID "aid"
#define ARTIST "artist"
#define TITLE "title"
#define DURATION "duration"
#define TRACK_URL "url"
#define OWNER_ID "owner_id"
#define OWNED "owned"
#define AUDIO_TRACK_PATH "/accounts/1000/shared/music/%1"

// Audio search
#define QUERY "q"

#define RESPONSE "response"
#define DATE_TIME_FORMAT "d MMM hh:mm"
#define COUNT "count"

namespace vkontaktemessages
{

QNetworkRequest createRetrieveMessagesRequest(const QString &access_token, int count)
{
	QString url = QString(URL_FORMAT).arg(WALL_GET).arg(access_token);
	url += "&count=" + QString::number(count);

	QNetworkRequest request(url);
    return request;
}

QVariantList parseRetrievedMessages(const QByteArray &result)
{
    std::cout << QString(result).toStdString() << std::endl;

	QVariantList list;
    QScriptEngine engine;
    QScriptValue response = engine.evaluate("(" + QString(result) + ")").property(RESPONSE);
    QScriptValueIterator it(response);

    while (it.hasNext()) {
        it.next();

        // Parse message id
        QString messageId = it.value().property(MESSAGE_ID).toString();

        // Parse message text
        QByteArray rawText = it.value().property(MESSAGE_TEXT).toVariant().toByteArray();
        QString messageText = QString::fromUtf8(rawText, rawText.size());

        // Parse message date
        QString messageDate = QDateTime::fromTime_t(it.value().property(MESSAGE_DATE).toUInt32()).toString(DATE_TIME_FORMAT);

        // Parse likes, comments and reposts
        int likes = it.value().property(MESSAGE_LIKES).property(COUNT).toInt32();
        int comments = it.value().property(MESSAGE_COMMENTS).property(COUNT).toInt32();
        int reposts = it.value().property(MESSAGE_REPOSTS).property(COUNT).toInt32();

        // Online status
        bool online = it.value().property(IS_ONLINE).toNumber() == ONLINE;

        if (!messageId.isEmpty() && !messageText.isEmpty() && !messageDate.isEmpty())
        {
        	QVariantMap message;

        	message.insert(MESSAGE_ID, messageId);
            message.insert(MESSAGE_TEXT, messageText);
            message.insert(MESSAGE_DATE,messageDate);

            if (likes > 0)
            	message.insert(MESSAGE_LIKES, likes);

            if (comments > 0)
            	message.insert(MESSAGE_COMMENTS, comments);

            if (reposts > 0)
            	message.insert(MESSAGE_REPOSTS, reposts);

            if (online)
            	message.insert(IS_ONLINE, ONLINE);

            list.append(message);
        }

    }
    return list;
}

QNetworkRequest createRetrieveAudioRequest(const QString &access_token, int count)
{
	QString url = QString(URL_FORMAT).arg(AUDIO_GET).arg(access_token);
	url += "&count=" + QString::number(count);

	QNetworkRequest request(url);
	return request;
}

QVariantList parseRetrievedAudioList(const QByteArray &result, const QString& userId)
{
	std::cout << QString(result).toStdString() << std::endl;

	QVariantList list;
	QScriptEngine engine;
	QScriptValue response = engine.evaluate("(" + QString(result) + ")").property(RESPONSE);
	QScriptValueIterator it(response);

	while (it.hasNext()) {
		it.next();

		// Parse aid & oid
		QString aid = it.value().property(AID).toString();
		QString oid = it.value().property(OWNER_ID).toString();

		// Parse artist
		QByteArray rawText = it.value().property(ARTIST).toVariant().toByteArray();
		QString artist = QString::fromUtf8(rawText, rawText.size());

		rawText.clear();

		// Parse title
		rawText = it.value().property(TITLE).toVariant().toByteArray();
		QString title = QString::fromUtf8(rawText, rawText.size());

		// Parse duration
		QString duration = it.value().property(DURATION).toString();

		// Parse url
		QString url = it.value().property(TRACK_URL).toString();

		if (!aid.isEmpty() && !oid.isEmpty() && !artist.isEmpty() && !title.isEmpty() && !url.isEmpty())
		{
			QVariantMap track;

			track.insert(AID, aid);
			track.insert(OWNER_ID, oid);
			track.insert(OWNED, userId.toInt() == oid.toInt());
			track.insert(ARTIST, artist);
			track.insert(TITLE,title);
			track.insert(DURATION,duration);
			track.insert(TRACK_URL,url);

			list.append(track);
		}
	}

	return list;
}

QUrl saveAudioTrack(const QByteArray &result, const QUrl& source)
{
	if (result.isEmpty())
	{
		return QUrl("");
	}

	QString uri = QString(AUDIO_TRACK_PATH).arg(source.toString().section('/', -1));
	qDebug() << uri;

	QFile audio(uri);
	if (audio.open(QIODevice::WriteOnly))
	{
		audio.write(result);
		audio.close();
	}
	return QUrl(uri);
}

QNetworkRequest createSearchMusicRequest(const QString &access_token, const QString& text, int count)
{
	QString url = QString(URL_FORMAT).arg(AUDIO_SEARCH).arg(access_token);
	url += "&" + QString(QUERY) + "=" + text;
	url += "&count=" + QString::number(count);
	url += "&auto_complete=1";

	std::cout << url.toStdString() << std::endl;

	QNetworkRequest request(url);
	return request;
}

QNetworkRequest createAddMusicRequest(const QString &access_token, const QString& aid, const QString& oid)
{
	QString url = QString(URL_FORMAT).arg(AUDIO_ADD).arg(access_token);
	url += "&aid=" + aid;
	url += "&oid=" + oid;

	QNetworkRequest request(url);
	return request;
}

bool musicAdded(const QByteArray &result)
{
	if (result.isEmpty())
	{
		return false;
	}

	QScriptEngine engine;
	int aid = engine.evaluate("(" + QString(result) + ")").property(RESPONSE).toInteger();

	return aid > 0;
}

QNetworkRequest createDeleteMusicRequest(const QString &access_token, const QString& aid, const QString& oid)
{
	QString url = QString(URL_FORMAT).arg(AUDIO_DELETE).arg(access_token);
	url += "&aid=" + aid;
	url += "&oid=" + oid;

	QNetworkRequest request(url);
	return request;
}

bool musicDeleted(const QByteArray &result)
{
	if (result.isEmpty())
	{
		return false;
	}

	QScriptEngine engine;
	int aid = engine.evaluate("(" + QString(result) + ")").property(RESPONSE).toInteger();

	return aid == 1;
}

}
