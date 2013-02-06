#include "leadingvisual.h"
#include <iostream>

#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/StackLayout>
#include <bb/cascades/LayoutUpdateHandler>

using namespace bb::cascades;

LeadingVisual::LeadingVisual():
	Container(),
	_resetUpdater(new QTimer(this)),
	_updateDone(new QTimer(this)),
	_activity(NULL),
	_loadingLabel(NULL),
	_refreshLabel(NULL)
{
	_resetUpdater->setSingleShot(true);
	_updateDone->setSingleShot(true);

    connect(_resetUpdater, SIGNAL(timeout()), this, SLOT(resetUpdaterTimeout()));
    connect(_updateDone, SIGNAL(timeout()), this, SLOT(updateDoneTimeout()));

    this->setPreferredSize(768, 100);
    this->setLayout(new DockLayout());

    Container* container = new Container(this);

    StackLayout* layout = new StackLayout;
    layout->setOrientation(LayoutOrientation::LeftToRight);
    container->setLayout(layout);

    container->setVerticalAlignment(VerticalAlignment::Center);
    container->setHorizontalAlignment(HorizontalAlignment::Center);

	_activity = new ActivityIndicator(container);
	connect(_activity, SIGNAL(runningChanged(bool)), this, SLOT(activityStatusChanged(bool)));

	_loadingLabel = new Label(container);
    _loadingLabel->setText("Loading...");
    _loadingLabel->setVisible(false);

    _refreshLabel = new Label(container);
    _refreshLabel->setText("Pull down to refresh...");
    _refreshLabel->setVisible(true);

    LayoutUpdateHandler::create(this).onLayoutFrameChanged(this, SLOT(handleLayoutFrameUpdated(QRectF)));
}

LeadingVisual::~LeadingVisual()
{
}

void LeadingVisual::stop()
{
	_activity->stop();
}

void LeadingVisual::resetUpdaterTimeout()
{
	this->setPreferredHeight(100);
	_activity->stop();
}

void LeadingVisual::updateDoneTimeout()
{
	this->setPreferredHeight(0);
	_resetUpdater->start(50);
}

void LeadingVisual::activityStatusChanged(bool running)
{
	_loadingLabel->setVisible(running);
	_refreshLabel->setVisible(!running);
}

void LeadingVisual::handleLayoutFrameUpdated(const QRectF& layoutFrame)
{
	if (layoutFrame.y() > 0 && !_activity->isRunning())
	{
		emit update();
		_activity->start();
		_updateDone->start(2000);
	}
}
