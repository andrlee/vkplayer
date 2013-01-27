#include "vkontakteplaylistmodel.h"

#include <QDir>
#include <QFile>

#define M3U_HEADER "#EXTM3U\n"
#define M3U_TRACK_FORMAT "#EXTINF:%1, %2 - %3\n"

VkontaktePlaylistModel::VkontaktePlaylistModel(QObject* parent):
	ArrayDataModel(parent),
	m_path(QDir::homePath())
{
}

VkontaktePlaylistModel::~VkontaktePlaylistModel()
{
}

QString VkontaktePlaylistModel::filename() const
{
    return m_filename;
}

void VkontaktePlaylistModel::setFilename(const QString &filename)
{
	if (m_filename != filename)
	{
		m_filename = filename;
		emit filenameChanged(m_filename);
	}
}

QUrl VkontaktePlaylistModel::path() const
{
	return QUrl(m_path + "/" + m_filename);
}

int VkontaktePlaylistModel::indexOfItem(const QVariantList &indexPath)
{
	return indexOf(data(indexPath));
}

bool VkontaktePlaylistModel::contains(const QVariant& item) const
{
	QVariantMap item_map = item.toMap();
	for (int index = 0; index < size(); index++)
	{
		QVariantMap val = value(index).toMap();
		if (val["aid"].toString() == item_map["aid"].toString())
		{
			return true;
		}
	}
	return false;
}

void VkontaktePlaylistModel::appendItem(const QVariantList &values)
{
	clear();
	appendPlaylist(values);
	append(values);
}

void VkontaktePlaylistModel::appendPlaylist(const QVariantList &values)
{
	QFile file(path().toString());
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
	{
		qDebug() << "Failed to open playlist file: " << path().toString();
		return;
	}

	QTextStream out(&file);

	out << QString(M3U_HEADER);
	foreach(QVariant v, values)
	{
		QVariantMap map = v.toMap();
		QByteArray artist = map["artist"].toByteArray();
		QByteArray title = map["title"].toByteArray();

		out << QString(M3U_TRACK_FORMAT).arg(map["duration"].toString())
										.arg(QString::fromUtf8(artist, artist.size()))
										.arg(QString::fromUtf8(title, title.size()));

		out << map["url"].toString();
		out << QString('\n');
	}
	file.close();
}
