#ifndef CAMERASWIDGET_H
#define CAMERASWIDGET_H

#include <QWidget>

namespace Ui {
class CamerasWidget;
}

class CamerasWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CamerasWidget(QWidget *parent = 0);
    ~CamerasWidget();

private:
    Ui::CamerasWidget *ui;
    QPixmap m_pix;
    void createConnections();
};

#endif // CAMERASWIDGET_H
