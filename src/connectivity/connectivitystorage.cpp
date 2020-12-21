#include "connectivitystorage.h"
#include "murgloabal.h"
#include "dispatcher.h"
#include "filedownloader.h"

#include <QFileDialog>
#include <QFile>
#include <iostream>

#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDialog>
#include <QProcess>
#include <QMessageBox>
#include <QApplication>

const QString version = "0.0.5";

ConnectivityStorage::ConnectivityStorage(QObject *parent) :
    QObject(parent),
    m_webSocket(new WebSocketClient(this)),
    m_devices(new DevicesWidget)
{
    createReactions();
}

ConnectivityStorage::~ConnectivityStorage()
{
    delete m_devices;
}

QWidget *ConnectivityStorage::devices()
{
    return m_devices;
}

void ConnectivityStorage::createActions()
{

}

void ConnectivityStorage::createReactions()
{
    FileDownloader *m_downloader = new FileDownloader(QUrl("http://128.199.126.75/version.txt"), this);

    connect(m_downloader, &FileDownloader::downloaded, [this](QByteArray data) {
        if (version != QString(data).simplified()) {
            qDebug() << QString(data).simplified();
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(nullptr, "Updates are available", "Update now?",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                QProcess update;
                update.startDetached("MaintenanceTool.exe");
                QApplication::exit();
            }

        }
    });

    connect(DISPATCHER->action("Action.Transfer.Binary"), &QAction::triggered, [this] {
        QStringList params = DISPATCHER->action("Action.Transfer.Binary")->data().toStringList();
        m_webSocket->uploadProgramm(params.at(1), params.at(0));
    });

    connect(DISPATCHER->action("Action.Source.Stop"), &QAction::triggered, m_webSocket, &WebSocketClient::stopProgramm);
    connect(DISPATCHER->action("Action.Transfer.Log"), &QAction::triggered, m_webSocket, &WebSocketClient::requestLog);
    connect(DISPATCHER->action("Action.Transfer.LogReply"), &QAction::triggered, [this]() {
        QString path = QFileDialog::getSaveFileName(nullptr, tr("Log file path"), QStandardPaths::displayName(QStandardPaths::DocumentsLocation), tr("Log(*.csv)"));
        if (path.isEmpty()) {
            return;
        }

        QJsonObject text = QJsonDocument::fromJson(DISPATCHER->action("Action.Transfer.LogReply")->data().toByteArray()).object();
        QFile log(path);
        log.open(QIODevice::WriteOnly);

        log.write(QString(QByteArray::fromBase64(text["data"].toVariant().toByteArray())).toUtf8());
    });
    connect(DISPATCHER->action("Action.Video.Stop"), &QAction::triggered, m_webSocket, &WebSocketClient::stopStream);
    connect(DISPATCHER->action("Action.Video.Start"), &QAction::triggered, [this]() {
        int id = DISPATCHER->action("Action.Video.Start")->data().toInt();
        m_webSocket->startStream(id);
    });

    connect(DISPATCHER->action("Action.Transfer.Calibrate"), &QAction::triggered, [this] {
        m_webSocket->startCalibration();
    });

}
