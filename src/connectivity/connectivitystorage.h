#ifndef CONNECTIVITYSTORAGE_H
#define CONNECTIVITYSTORAGE_H

#include <QObject>
#include "websocketclient.h"
#include "deviceswidget.h"

class ConnectivityStorage : public QObject
{
    Q_OBJECT
public:
    explicit ConnectivityStorage(QObject *parent = 0);
    ~ConnectivityStorage();
    QWidget* devices();
private:
    void createActions();
    void createReactions();
    WebSocketClient* m_webSocket;
    DevicesWidget* m_devices;
};

#endif // CONNECTIVITYSTORAGE_H
