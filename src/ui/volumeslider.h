#ifndef VOLUMESLIDER_H_
#define VOLUMESLIDER_H_

#include <bb/cascades/Slider>
#include <bb/cascades/Container>
#include <bb/AbstractBpsEventHandler>
#include <bps/audiomixer.h>

class VolumeSlider : public bb::cascades::Slider, public bb::AbstractBpsEventHandler
{
	Q_OBJECT
public:
	VolumeSlider(bb::cascades::Container* parent = 0);
	virtual ~VolumeSlider();

	virtual void event(bps_event_t* event);

private slots:
	void onValueChanged(float);

private:
	audiomixer_output_t _currentOutput;
};

#endif /* VOLUMESLIDER_H_ */
