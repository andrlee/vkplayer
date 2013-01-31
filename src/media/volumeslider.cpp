#include "volumeslider.h"

#include <bps/bps.h>

#define MIN_VOLUME_LEVEL 0.0
#define MAX_VOLUME_LEVEL 100.0

using namespace bb::cascades;

VolumeSlider::VolumeSlider(Container* parent):
	Slider(parent),
	_currentOutput(AUDIOMIXER_OUTPUT_DEFAULT)
{
	subscribe(audiomixer_get_domain());
	bps_initialize();
	audiomixer_request_events(0);

	setRange(MIN_VOLUME_LEVEL, MAX_VOLUME_LEVEL);

	float volume = 0.0;
	if (BPS_SUCCESS == audiomixer_get_output_level(_currentOutput, &volume))
	{
		setValue(volume);
	}

	connect(this, SIGNAL(immediateValueChanged(float)),this, SLOT(onValueChanged(float)));
}

VolumeSlider::~VolumeSlider()
{
    bps_shutdown();
}

void VolumeSlider::event(bps_event_t* event)
{
    int domain = bps_event_get_domain(event);
    if (domain == audiomixer_get_domain() && AUDIOMIXER_INFO == bps_event_get_code(event))
    {
    	qDebug() << "Audiomixer event";
    	_currentOutput = audiomixer_event_get_available(event);
    	setValue(audiomixer_event_get_output_level(event, _currentOutput));
    }
}

void VolumeSlider::onValueChanged(float value)
{
	if (BPS_SUCCESS == audiomixer_set_output_level(_currentOutput, value))
	{
		qDebug() << "Volume set to: " << value;
	}
}

