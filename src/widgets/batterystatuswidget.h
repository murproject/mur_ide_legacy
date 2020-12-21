#ifndef BATTERYSTATUSWIDGET_H
#define BATTERYSTATUSWIDGET_H

#include <QWidget>
#include <QLabel>

class BatteryStatusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BatteryStatusWidget(QWidget *parent = 0);

signals:

public slots:
private:
    void createConnections();
    QLabel *m_text;
};

#endif // BATTERYSTATUSWIDGET_H
