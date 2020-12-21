#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QJsonArray>
#include <QDebug>

#include "murgloabal.h"
#include "dispatcher.h"

#include "deviceswidget.h"
#include "ui_deviceswidget.h"

void setBlue(QLabel *&lable) {
    QPalette palette = lable->palette();
    palette.setColor(lable->backgroundRole(),QColor(61,142,201).lighter());
    palette.setColor(lable->foregroundRole(), QColor(61,142,201).lighter());
    lable->setPalette(palette);
}

void setGray(QLabel *&lable) {
    QPalette palette = lable->palette();
    palette.setColor(lable->backgroundRole(), Qt::gray);
    palette.setColor(lable->foregroundRole(), Qt::gray);
    lable->setPalette(palette);
}

void setDark(QLabel *&lable) {
    QPalette palette = lable->palette();
    palette.setColor(lable->backgroundRole(), QColor(20, 20, 20));
    palette.setColor(lable->foregroundRole(), QColor(20, 20, 20));
    lable->setPalette(palette);
}

void setThrusterEnable(QLabel* label) {
    static QPixmap pix(":/misc/images/thruster.png");
    label->setPixmap(pix);
}

void setThrusterDisable(QLabel* label) {
    static QPixmap pix(":/misc/images/thruster_dark.png");
    label->setPixmap(pix);

}

void setCameraEnable(QLabel* label) {
    static QPixmap pix(":/misc/images/camera.png");
    label->setPixmap(pix);
}

void setCameraDisable(QLabel* label) {
    static QPixmap pix(":/misc/images/camera_dark.png");
    label->setPixmap(pix);
}


DevicesWidget::DevicesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DevicesWidget)
{
    ui->setupUi(this);
    createConnections();

    setBlue(ui->cameraOne);
    setBlue(ui->cameraTwo);
    setBlue(ui->thrusterOne);
    setBlue(ui->thrusterTwo);
    setBlue(ui->thrusterThree);
    setBlue(ui->thrusterFour);
    setBlue(ui->i2cOne);
    setBlue(ui->i2cTwo);
    setBlue(ui->pitch);
    setBlue(ui->roll);
    setBlue(ui->cameraOne);
    setBlue(ui->yaw);
    setBlue(ui->temp);

    setThrusterDisable(ui->thrusterOne);
    setThrusterDisable(ui->thrusterTwo);
    setThrusterDisable(ui->thrusterThree);
    setThrusterDisable(ui->thrusterFour);

    setCameraDisable(ui->cameraOne);
    setCameraDisable(ui->cameraTwo);
}

DevicesWidget::~DevicesWidget()
{
    delete ui;
}

void DevicesWidget::createConnections()
{
    connect(DISPATCHER->action("Action.Transfer.Telimetry"), &QAction::triggered, [this]() {
        QAction *act = DISPATCHER->action("Action.Transfer.Telimetry");
        QJsonParseError err;
        QJsonObject obj = QJsonDocument::fromJson(act->data().toString().toUtf8(), &err).object();
        if (Q_UNLIKELY(err.error != QJsonParseError::NoError)) {
            QMessageBox::warning(nullptr, QObject::tr("Message parse error"), QObject::tr("Action.Transfer.Telimetry"), QMessageBox::Ok);
            return;
        }
        QJsonArray motors = obj["motors"].toArray();
        QJsonArray cameras = obj["cameras"].toArray();
        QJsonArray i2c = obj["i2c"].toArray();
        QJsonArray i2cVals = obj["i2cvals"].toArray();
        QJsonArray imu = obj["imu"].toArray();

        if (cameras.at(0).toInt()) {
            setCameraEnable(ui->cameraOne);
        }
        else {
            setCameraDisable(ui->cameraOne);
        }

        if (cameras.at(1).toInt()) {
            setCameraEnable(ui->cameraTwo);
        }
        else {
            setCameraDisable(ui->cameraTwo);
        }

        //!ThrusterOne
        if (motors.at(0).toInt()) {
            setThrusterEnable(ui->thrusterOne);
        }
        else {
            setThrusterDisable(ui->thrusterOne);
        }

        //!ThrusterTwo
        if (motors.at(1).toInt()) {
            setThrusterEnable(ui->thrusterTwo);
        }
        else {
            setThrusterDisable(ui->thrusterTwo);
        }

        //!ThrusterThree
        if (motors.at(2).toInt()) {
            setThrusterEnable(ui->thrusterThree);
        }
        else {
            setThrusterDisable(ui->thrusterThree);
        }

        //!ThrusterFour
        if (motors.at(3).toInt()) {
            setThrusterEnable(ui->thrusterFour);
        }
        else {
            setThrusterDisable(ui->thrusterFour);
        }

        if (i2c.at(0).toInt()) {
            ui->i2cOne->setText(QString::number(i2cVals.at(0).toDouble(), 'f', 2));
            setBlue(ui->i2cOne);
        } else {
            setDark(ui->i2cOne);
            ui->i2cOne->setText("-");
        }

        if (i2c.at(1).toInt()) {
            ui->i2cTwo->setText(QString::number(i2cVals.at(1).toDouble(), 'f', 2));
            setBlue(ui->i2cTwo);
        } else {
            setDark(ui->i2cTwo);
            ui->i2cTwo->setText("-");
        }

        ui->pitch->setText(QString::number(imu.at(0).toDouble(), 'f', 2));
        ui->roll->setText(QString::number(imu.at(1).toDouble(), 'f', 2));
        ui->yaw->setText(QString::number(imu.at(2).toDouble(), 'f', 2));

        if (i2cVals.at(2).toDouble() != -1.0) {
            ui->temp->setText(QString::number(i2cVals.at(2).toDouble(), 'f', 2));
            setBlue(ui->temp);
        } else  if (i2cVals.at(3).toDouble() != -1.0) {
            ui->temp->setText(QString::number(i2cVals.at(3).toDouble(), 'f', 2));
            setBlue(ui->temp);
        } else {
            ui->temp->setText("-");
            setDark(ui->temp);
        }
    });

    connect(DISPATCHER->action("Action.Transfer.Disconnected"), &QAction::triggered, [this](){
        setThrusterDisable(ui->thrusterOne);
        setThrusterDisable(ui->thrusterTwo);
        setThrusterDisable(ui->thrusterThree);
        setThrusterDisable(ui->thrusterFour);

        setCameraDisable(ui->cameraOne);
        setCameraDisable(ui->cameraTwo);

        setDark(ui->i2cOneLabel);
        setDark(ui->i2cTwoLabel);
        setDark(ui->rollLabel);
        setDark(ui->pitchLabel);
        setDark(ui->tempLabel);
        setDark(ui->yawLabel);

        setDark(ui->i2cOne);
        setDark(ui->i2cTwo);
        setDark(ui->pitch);
        setDark(ui->roll);
        setDark(ui->yaw);
        setDark(ui->temp);

        ui->i2cOne->setText("-");
        ui->i2cTwo->setText("-");
        ui->pitch->setText("-");
        ui->roll->setText("-");
        ui->yaw->setText("-");
        ui->temp->setText("-");
    });

    connect(DISPATCHER->action("Action.Transfer.Connected"), &QAction::triggered, [this](){
        setBlue(ui->i2cOneLabel);
        setBlue(ui->i2cTwoLabel);
        setBlue(ui->rollLabel);
        setBlue(ui->pitchLabel);
        setBlue(ui->tempLabel);
        setBlue(ui->yawLabel);

        setBlue(ui->roll);
        setBlue(ui->pitch);
        setBlue(ui->yaw);
    });
}
