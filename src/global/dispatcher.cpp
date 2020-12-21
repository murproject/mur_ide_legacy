#include "dispatcher.h"

void Dispatcher::registerAction(QString name, QAction* act)
{
    m_actions[name] = act;
}

void Dispatcher::unregisterAction(QString name)
{
    m_actions.remove(name);
}

void Dispatcher::trigger(QString name)
{
    QAction *act = m_actions[name];
    act->trigger();
}

QAction *Dispatcher::action(QString name)
{
    return m_actions[name];
}
