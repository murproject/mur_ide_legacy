#include "batterystatuswidget.h"
#include <QPalette>
#include <QLabel>
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>

#include "murgloabal.h"
#include "dispatcher.h"

void setBlack(QLabel *&lable) {
    QPalette palette = lable->palette();
    palette.setColor(lable->backgroundRole(),QColor(20,20,20));
    palette.setColor(lable->foregroundRole(), QColor(20,20,20));
    lable->setPalette(palette);
}

void setGreen(QLabel *&lable) {
    QPalette palette = lable->palette();
    palette.setColor(lable->backgroundRole(),QColor("#27AE60"));
    palette.setColor(lable->foregroundRole(), QColor("#27AE60"));
    lable->setPalette(palette);
}


BatteryStatusWidget::BatteryStatusWidget(QWidget *parent) :
    QWidget(parent),
    m_text(new QLabel("Battery",this))
{
    QHBoxLayout *layout= new QHBoxLayout();

    createConnections();
    QPalette pal = palette();
    layout->addWidget(m_text);
    setBlack(m_text);
    m_text->setAlignment(Qt::AlignCenter);

//    pal.setColor(QPalette::Background, QColor("#C0392B"));
//    pal.setColor(QPalette::Background, QColor("#F39C12"));
//    pal.setColor(QPalette::Background, QColor("#27AE60"));
//    //pal.setColor(QPalette::Background, QColor(20, 20, 20));



//    setAutoFillBackground(true);
//    setPalette(pal);

    setLayout(layout);
}

void BatteryStatusWidget::createConnections()
{
    connect(DISPATCHER->action("Action.Transfer.Telimetry"), &QAction::triggered, [this]() {
        QAction *act = DISPATCHER->action("Action.Transfer.Telimetry");
        QJsonParseError err;
        QJsonObject obj = QJsonDocument::fromJson(act->data().toString().toUtf8(), &err).object();
        if (Q_UNLIKELY(err.error != QJsonParseError::NoError)) {
          QMessageBox::warning(nullptr, QObject::tr("Message parse error"), QObject::tr("Action.Transfer.Telimetry"), QMessageBox::Ok);
          return;
        }
        int value = obj["battery"].toVariant().toInt();
        m_text->setText(QString("Battery: %1").arg(value));
    });

    connect(DISPATCHER->action("Action.Transfer.Disconnected"), &QAction::triggered, [this](){
        setBlack(m_text);
    });

    connect(DISPATCHER->action("Action.Transfer.Connected"), &QAction::triggered, [this](){
        setGreen(m_text);
    });

}
