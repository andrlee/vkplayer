#ifndef VKONTAKTEPLAYLIST_H_
#define VKONTAKTEPLAYLIST_H_

#include <bb/cascades/ArrayDataModel>
#include <bb/cascades/DataModelChangeType>

class VkontaktePlaylist : public bb::cascades::ArrayDataModel
{
	Q_OBJECT
	Q_PROPERTY(int playingTrack READ playingTrack WRITE setPlayingTrack NOTIFY playingTrackChanged)
	Q_PROPERTY(bool myTrack READ myTrack NOTIFY myTrackChanged)
	Q_PROPERTY(bool empty READ empty NOTIFY emptyChanged)

public:
	VkontaktePlaylist(QObject* parent = 0);
	virtual ~VkontaktePlaylist();

	Q_INVOKABLE int nextTrack() const;
	Q_INVOKABLE int previousTrack() const;
	Q_INVOKABLE int randomTrack() const;
	Q_INVOKABLE int indexOfItem(const QVariantList &indexPath);
	Q_INVOKABLE bool contains(const QVariant& item) const;
	Q_INVOKABLE void appendItem(const QVariantList &values);

	int playingTrack() const;
	void setPlayingTrack(int index);

	bool myTrack();
	bool empty() const;

public slots:
	void clear();

signals:
	void playingTrackChanged(int index);
	void myTrackChanged(bool myTrack);
	void emptyChanged(bool empty);

private:
	bool isCurrentTrackOwner();

	int m_playingTrack;
};

#endif /* VKONTAKTEPLAYLIST_H_ */
