#include "cameraswidget.h"
#include "ui_cameraswidget.h"
#include "murgloabal.h"
#include "dispatcher.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QPushButton>
#include <QPixmap>
#include <QFileDialog>
#include <QFile>

CamerasWidget::CamerasWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CamerasWidget),
    m_pix(QSize(320, 240))
{
    ui->setupUi(this);
    createConnections();
    ui->image->setScaledContents(true);
    ui->startStreamButton->setIcon(QIcon(":/misc/images/start.png"));
    ui->stopStreamButton->setIcon(QIcon(":/misc/images/stop.png"));
}

CamerasWidget::~CamerasWidget()
{
    delete ui;
}

void CamerasWidget::createConnections()
{
    connect(ui->startStreamButton, &QPushButton::pressed, [this](){
        DISPATCHER->action("Action.Video.Start")->setData(ui->idBox->currentIndex());
        DISPATCHER->action("Action.Video.Start")->trigger();
    });
    connect(ui->stopStreamButton, &QPushButton::pressed, DISPATCHER->action("Action.Video.Stop"), &QAction::trigger);
    connect(DISPATCHER->action("Action.Video.Data"), &QAction::triggered, [this](){
        QAction *act = DISPATCHER->action("Action.Video.Data");
        QJsonParseError err;
        QJsonObject obj = QJsonDocument::fromJson(act->data().toString().toUtf8(), &err).object();
        if (Q_UNLIKELY(err.error != QJsonParseError::NoError)) {
            QMessageBox::warning(nullptr, QObject::tr("Message parse error"), QObject::tr("Action.Video.Data"), QMessageBox::Ok);
            return;
        }
        QByteArray data = QByteArray::fromBase64(obj["data"].toVariant().toByteArray());
        QPixmap pix;
        pix.loadFromData(data, "JPG");
        m_pix = pix;
        ui->image->setPixmap(m_pix);
    });
    connect(ui->makePhotoButton, &QPushButton::pressed, [this]() {
        QString path = QFileDialog::getSaveFileName(nullptr, tr("Image path"), "Image", tr("Image(*.png)"));
        if (path.isEmpty()) {
            return;
        }
        QFile file(path);
        file.open(QIODevice::WriteOnly);
        m_pix.save(&file, "PNG");
    });
}
