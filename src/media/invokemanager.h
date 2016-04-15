
#ifndef INVOKEMANAGER_H_
#define INVOKEMANAGER_H_

#include <QObject>

class InvokeManager : public QObject
{
    Q_OBJECT

public:
    InvokeManager(QObject* parent = 0);
    ~InvokeManager();

    Q_INVOKABLE void invokeMediaPlayer(const QString& uri);
};


#endif /* INVOKEMANAGER_H_ */
