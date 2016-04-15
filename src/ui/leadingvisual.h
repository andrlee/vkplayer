#ifndef LEADINGVISUAL_H_
#define LEADINGVISUAL_H_

#include <bb/cascades/Container>
#include <bb/cascades/ActivityIndicator>
#include <bb/cascades/Container>
#include <bb/cascades/Label>

#include <QTimer>

class LeadingVisual : public bb::cascades::Container
{
	Q_OBJECT

public:
	LeadingVisual();
	~LeadingVisual();

	Q_INVOKABLE void stop();

signals:
	void update();

private slots:
	void resetUpdaterTimeout();
	void updateDoneTimeout();
	void activityStatusChanged(bool running);
	void handleLayoutFrameUpdated(const QRectF &layoutFrame);

private:
	QTimer* _resetUpdater;
	QTimer* _updateDone;
	bb::cascades::ActivityIndicator* _activity;
	bb::cascades::Label*			 _loadingLabel;
	bb::cascades::Label*			 _refreshLabel;
};

#endif /* LEADINGVISUAL_H_ */
