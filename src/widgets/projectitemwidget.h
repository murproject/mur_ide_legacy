#ifndef PROJECTITEMWIDGET_H
#define PROJECTITEMWIDGET_H

#include <QWidget>

namespace Ui {
class ProjectItemWidget;
}

class ProjectItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectItemWidget(QWidget *parent = 0);
    ~ProjectItemWidget();
    void setName(QString name);
    void setDescription(QString description);

private:
    Ui::ProjectItemWidget *ui;
};

#endif // PROJECTITEMWIDGET_H
