#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QWebSocket>
#include <QTimer>

class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    WebSocketClient(QObject *parent = nullptr);
    void uploadProgramm(QString path, QString name);
    void stopProgramm();
    void requestLog();
    void requestStatus();
    void startStream(int id);
    void stopStream();
    void startCalibration();
private:
    void startProgramm();
    void startProgramm(QString name);
    void requestOutput();
    void createConnections();
    QWebSocket m_socket;
    QTimer *m_connectTimer;
    QTimer *m_requestTelimetryTimer;
};

#endif // WEBSOCKETCLIENT_H
