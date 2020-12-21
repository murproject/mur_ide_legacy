#include "projectitemwidget.h"
#include "ui_projectitemwidget.h"

#include <QPainter>
#include "colorize.h"

ProjectItemWidget::ProjectItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectItemWidget)
{
    ui->setupUi(this);

    QIcon pix = colorizeIcon(":/misc/images/open.png",  QColor(61, 142, 201));
    ui->icon->setPixmap(pix.pixmap(22, 22));
    QPalette palette = ui->name->palette();
    palette.setColor(QPalette::WindowText, Qt::yellow);
    palette.setColor(QPalette::Window, Qt::yellow);
    ui->name->setStyleSheet("QLabel { background-color : none; color : rgb(61,142,201); }");
    ui->icon->setStyleSheet("QLabel { background-color : none;}");
    ui->path->setStyleSheet("QLabel { background-color : none;}");
}

ProjectItemWidget::~ProjectItemWidget()
{
    delete ui;
}

void ProjectItemWidget::setName(QString name)
{
    ui->name->setText(name);
}

void ProjectItemWidget::setDescription(QString description)
{
    ui->path->setText(description);
}
