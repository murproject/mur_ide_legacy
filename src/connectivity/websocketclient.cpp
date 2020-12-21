#include "websocketclient.h"
#include <QJsonObject>
#include <QJsonParseError>
#include <QMessageBox>
#include <QFile>
#include <QJsonArray>
#include <QFileInfo>
#include "murgloabal.h"
#include "dispatcher.h"
#include "fastlz.h"

const uint16_t poly_val = 0x1021;
const uint16_t seed_val = 0xFFFF;

static uint16_t updateCRC(uint16_t acc, const uint8_t input)
{
    acc ^= (input << 8);
    for (uint8_t i = 0; i < 8; i++) {
        if ((acc & 0x8000) == 0x8000) {
            acc <<= 1;
            acc ^= ::poly_val;
        }
        else {
            acc <<= 1;
        }
    }

    return acc;
}

static uint16_t calculateCRC(const char* data, const size_t len)
{
    uint16_t crcout = ::seed_val;

    for (size_t i = 0; i < len; ++i) {
        crcout = updateCRC(crcout, data[i]);
    }

    return crcout;
}


WebSocketClient::WebSocketClient(QObject *parent) :
    QObject(parent),
    m_connectTimer(new QTimer(this)),
    m_requestTelimetryTimer(new QTimer(this))
{
    m_connectTimer->setInterval(5000);
    m_connectTimer->start();
    m_requestTelimetryTimer->setInterval(1000);
    createConnections();
}

void WebSocketClient::startProgramm()
{
    static QString str("{\"request"" : \"run\"}");
    if (m_socket.isValid()) {
        m_socket.sendTextMessage(str);
    }
}

void WebSocketClient::startProgramm(QString name)
{
    QString str = QString("{\"request\" : \"run\", \"programName\" : \"%1.bin\"}").arg(name);
    if (m_socket.isValid()) {
        m_socket.sendTextMessage(str);
        QAction *act = DISPATCHER->action("Action.Debug.Message");
        act->setData("Program started!");
        act->trigger();
    }
}

void WebSocketClient::stopProgramm()
{
    static QString str = QString("{\"request\" : \"stop\"}");
    if (m_socket.isValid()) {
        m_socket.sendTextMessage(str);
        QAction *act = DISPATCHER->action("Action.Debug.Message");
        act->setData("Program stoped!");
        act->trigger();
    }
}

void WebSocketClient::requestLog()
{
    static QString str("{\"request\" : \"getLog\"}");
    if (m_socket.isValid()) {
        m_socket.sendTextMessage(str);
    }
}

void WebSocketClient::requestStatus()
{
    //deviceStatus
    static QString str("{\"request\" : \"deviceStatus\"}");
    if (m_socket.isValid()) {
        m_socket.sendTextMessage(str);
    }
}

void WebSocketClient::startStream(int id)
{
    QString str = QString("{\"request\" : \"startStream\", \"id\" : %1}").arg(id);
    if (m_socket.isValid()) {
        m_socket.sendTextMessage(str);
    }
    qDebug() << str;
}

void WebSocketClient::stopStream()
{
    static QString str("{\"request\" : \"stopStream\"}");
    if (m_socket.isValid()) {
        m_socket.sendTextMessage(str);
    }
}

void WebSocketClient::startCalibration()
{
    static QString str("{\"request\" : \"calibrate\"}");
    if (m_socket.isValid()) {
        m_socket.sendTextMessage(str);
    }
}

void WebSocketClient::requestOutput()
{
    static QString str("{\"request\" : \"lastError\"}");
    if (m_socket.isValid()) {
        m_socket.sendTextMessage(str);
    }

}

void WebSocketClient::uploadProgramm(QString path, QString name)
{
    QFile binary(path);
    if (!binary.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(nullptr, QObject::tr("uploadProgramm"), QObject::tr("Unable open binary file!"), QMessageBox::Ok);
        return;
    }

    QByteArray bdata = binary.readAll();
    QByteArray cdata;
    cdata.resize(bdata.size() + (bdata.size() * 0.05F));

    int compSize = fastlz_compress(reinterpret_cast<void*>(bdata.data()), bdata.size(),
                                             reinterpret_cast<void*>((cdata.data())));

    cdata.resize(compSize);
    QJsonObject file;
    file["crc"] =   calculateCRC(cdata.data(), cdata.size());
    file["csize"] = compSize;
    file["rsize"] = bdata.size();
    file["data"] = QString(cdata.toBase64());
    file["name"] = QJsonValue(name + ".bin").toString();
    QJsonArray files;
    files.append(file);

    QJsonObject obj;
    obj["files"] = files;
    obj["request"] = QJsonValue("uploadProgram").toString();
    obj["name"] = QJsonValue(name).toString();
    obj["type"] = QJsonValue("cpp").toString();

    QJsonDocument document(obj);
    m_socket.sendTextMessage(QString(document.toJson()));
    startProgramm(name);
}

void WebSocketClient::createConnections()
{
    connect(&m_socket, &QWebSocket::connected, [this]() {
        m_connectTimer->stop();
        m_requestTelimetryTimer->start();
        DISPATCHER->action("Action.Transfer.Connected")->trigger();
    });

    connect(&m_socket, &QWebSocket::disconnected, [this]() {
        m_connectTimer->start();
        m_requestTelimetryTimer->stop();
        DISPATCHER->action("Action.Transfer.Disconnected")->trigger();
    });

    connect(m_requestTelimetryTimer, &QTimer::timeout, [this](){
       requestStatus();
    });

    connect(&m_socket, &QWebSocket::textMessageReceived, [this](QString msg) {
        qDebug() << "Message:" << msg;

        QJsonParseError err;
        QJsonObject obj = QJsonDocument::fromJson(msg.toUtf8(), &err).object();
        if (Q_UNLIKELY(err.error != QJsonParseError::NoError)) {
            qDebug() << "Parse error!";
            return;
        }

        if (obj["reply"].toString() == "output") {
            QAction *act = DISPATCHER->action("Action.Transfer.StdOutErr");
            act->setData(msg);
            act->trigger();
        }

        if (obj["reply"].toString() == "log") {
            QAction *act = DISPATCHER->action("Action.Transfer.LogReply");
            act->setData(msg);
            act->trigger();
        }

        if (obj["reply"].toString() == "programmsList") {
            QAction *act = DISPATCHER->action("Action.Transfer.ProgramsList");
            act->setData(msg);
            act->trigger();
        }

        if (obj["reply"].toString() == "status") {
            QAction *act = DISPATCHER->action("Action.Transfer.Telimetry");
            act->setData(msg);
            act->trigger();

            if (obj["output"].toString() == "yes") {
                requestOutput();
            }

            if (obj["running"].toString() == "yes") {
                DISPATCHER->action("Action.Transfer.Running")->trigger();
            }

            if (obj["running"].toString() == "no") {
                DISPATCHER->action("Action.Transfer.Notrunning")->trigger();
            }
        }

        if (obj["reply"].toString() == "image") {
            QAction *act = DISPATCHER->action("Action.Video.Data");
            act->setData(msg);
            act->trigger();
        }

        if (obj["reply"].toString() == "calibrated") {
            QMessageBox::warning(nullptr, QObject::tr("Calibration"), QObject::tr("MPU Calibrated!"), QMessageBox::Ok);
        }

        if (obj["reply"].toString() == "calibrated") {
            QMessageBox::warning(nullptr, QObject::tr("Calibration"), QObject::tr("MPU Calibrated!"), QMessageBox::Ok);
        }

        if (obj["reply"].toString() == "repeat") {
            QMessageBox::warning(nullptr, QObject::tr("Calibration"), QObject::tr("Calibration failed. Please try again."), QMessageBox::Ok);
        }
    });

    connect(m_connectTimer, &QTimer::timeout, [this]() {
        m_socket.abort();
        m_socket.open((QUrl("ws://192.168.42.1:2090")));
    });
}
