#ifndef DEVICESWIDGET_H
#define DEVICESWIDGET_H

#include <QWidget>

namespace Ui {
class DevicesWidget;
}

class DevicesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DevicesWidget(QWidget *parent = 0);
    ~DevicesWidget();
private:
    Ui::DevicesWidget *ui;
    void createConnections();
};

#endif // DEVICESWIDGET_H
