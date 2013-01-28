#include "vkontakteplaylist.h"
#include <pthread.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

VkontaktePlaylist::VkontaktePlaylist(QObject* parent):
	ArrayDataModel(parent),
	m_playingTrack(0)
{
}

VkontaktePlaylist::~VkontaktePlaylist()
{
}

int VkontaktePlaylist::playingTrack() const
{
	return m_playingTrack;
}

bool VkontaktePlaylist::myTrack()
{
	if (size() == 0)
		return false;

	return isCurrentTrackOwner();
}

void VkontaktePlaylist::setPlayingTrack(int index)
{
	if (index >= 0 && index < size() && m_playingTrack != index)
	{
		pthread_mutex_lock(&mutex);
		m_playingTrack = index;

		emit setPlayingTrack(m_playingTrack);
		emit myTrackChanged(isCurrentTrackOwner());
		pthread_mutex_unlock(&mutex);
	}
}

int VkontaktePlaylist::nextTrack() const
{
	return (m_playingTrack >= size() - 1) ? 0 : m_playingTrack + 1;
}

int VkontaktePlaylist::previousTrack() const
{
	return m_playingTrack == 0 ? size() - 1 : m_playingTrack - 1;
}

int VkontaktePlaylist::indexOfItem(const QVariantList &indexPath)
{
	return indexOf(data(indexPath));
}

bool VkontaktePlaylist::contains(const QVariant& item) const
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

void VkontaktePlaylist::clear()
{
	pthread_mutex_lock(&mutex);
	ArrayDataModel::clear();
	pthread_mutex_unlock(&mutex);

	setPlayingTrack(0);
}

void VkontaktePlaylist::appendItem(const QVariantList &values)
{
	clear();

	pthread_mutex_lock(&mutex);
	append(values);
	emit myTrackChanged(isCurrentTrackOwner());
	pthread_mutex_unlock(&mutex);
}

bool VkontaktePlaylist::isCurrentTrackOwner()
{
	QVariantMap val = value(playingTrack()).toMap();
	return val["owned"].toBool();
}
