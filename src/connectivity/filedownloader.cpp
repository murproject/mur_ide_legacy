#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl url, QObject *parent) :
    QObject(parent)
{
    connect(&m_WebCtrl, &QNetworkAccessManager::finished, this, &FileDownloader::fileDownloaded);

    QNetworkRequest request(url);
    m_WebCtrl.get(request);
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {
    m_DownloadedData = pReply->readAll();
    pReply->deleteLater();
    emit downloaded(m_DownloadedData);
}

QByteArray FileDownloader::downloadedData() const {
    return m_DownloadedData;
}
