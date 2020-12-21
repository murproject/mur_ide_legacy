#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <QObject>
#include <QMap>
#include <QAction>

class Dispatcher
{
public:
    void registerAction(QString name, QAction*act);
    void unregisterAction(QString name);
    void trigger(QString name);
    QAction* action(QString name);
private:
    QMap<QString, QAction*> m_actions;
};

#endif // DISPATCHER_H
