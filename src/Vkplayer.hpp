// Tabbed pane project template
#ifndef Vkplayer_HPP_
#define Vkplayer_HPP_

#include <QObject>

namespace bb { namespace cascades { class Application; }}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class Vkplayer : public QObject
{
    Q_OBJECT
public:
    Vkplayer(bb::cascades::Application *app);
    virtual ~Vkplayer() {}
};

#endif /* Vkplayer_HPP_ */