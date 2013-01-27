#ifndef VKONTAKTEPLAYLISTMODEL_H_
#define VKONTAKTEPLAYLISTMODEL_H_

#include <bb/cascades/ArrayDataModel>

class VkontaktePlaylistModel : public bb::cascades::ArrayDataModel
{
	Q_OBJECT

	Q_PROPERTY(QString filename READ filename WRITE setFilename NOTIFY filenameChanged)

public:
	VkontaktePlaylistModel(QObject* parent = 0);
    virtual ~VkontaktePlaylistModel();

    Q_INVOKABLE QUrl path() const;
	Q_INVOKABLE int indexOfItem(const QVariantList &indexPath);
	Q_INVOKABLE bool contains(const QVariant& item) const;
	Q_INVOKABLE void appendItem(const QVariantList &values);

    QString filename() const;
    void setFilename(const QString& filename);

signals:
	void filenameChanged(const QString &filename);

private:
	void appendPlaylist(const QVariantList &values);

    QString m_path;
    QString m_filename;
};

#endif /* VKONTAKTEPLAYLISTMODEL_H_ */
