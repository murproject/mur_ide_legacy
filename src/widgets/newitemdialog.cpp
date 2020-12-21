#include <QListWidgetItem>
#include <QDebug>

#include "newitemdialog.h"
#include "ui_newitemdialog.h"
#include "dispatcher.h"
#include "murgloabal.h"

NewItemDialog::NewItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewItemDialog)
{
    ui->setupUi(this);
    createItems();
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    connect(ui->createButton, &QPushButton::clicked, [this]() {
        auto list = ui->itemsListWidget->selectedItems();
        if (list.isEmpty()) {
            return;
        }

        if (ui->itemNameEdit->text().isEmpty()) {
            return;
        }

        if (list.first()->text() == "Project") {
            QAction *act = DISPATCHER->action("Action.New.Project");
            act->setData(ui->itemNameEdit->text());
            act->trigger();
        }

        if (list.first()->text() == ".cpp File") {
            QAction *act = DISPATCHER->action("Action.New.CPPFile");
            act->setData(ui->itemNameEdit->text());
            act->trigger();
        }

        if (list.first()->text() == ".h File") {
            QAction *act = DISPATCHER->action("Action.New.HFile");
            act->setData(ui->itemNameEdit->text());
            act->trigger();
        }

        if (list.first()->text() == ".h + .cpp Files") {
            QAction *act = DISPATCHER->action("Action.New.Class");
            act->setData(ui->itemNameEdit->text());
            act->trigger();
        }
        hide();
    });
}

NewItemDialog::~NewItemDialog()
{
    delete ui;
}

void NewItemDialog::setProjectOpen(bool flag)
{
    if (!flag) {
        ui->itemsListWidget->item(1)->setHidden(true);
        ui->itemsListWidget->item(2)->setHidden(true);
        ui->itemsListWidget->item(3)->setHidden(true);
    }
    else {
        ui->itemsListWidget->item(1)->setHidden(false);
        ui->itemsListWidget->item(2)->setHidden(false);
        ui->itemsListWidget->item(3)->setHidden(false);
    }
}

void NewItemDialog::createItems()
{
    QListWidgetItem* item = new QListWidgetItem(ui->itemsListWidget);
    item->setText(tr("Project"));
    item->setIcon(QIcon(":/misc/images/class.png"));

    item = new QListWidgetItem(ui->itemsListWidget);
    item->setText(tr(".cpp File"));
    item->setIcon(QIcon(":/misc/images/cpp-file.png"));

    item = new QListWidgetItem(ui->itemsListWidget);
    item->setText(tr(".h File"));
    item->setIcon(QIcon(":/misc/images/h-file.png"));

    item = new QListWidgetItem(ui->itemsListWidget);
    item->setText(tr(".h + .cpp Files"));
    item->setIcon(QIcon(":/misc/images/h-cpp-file.png"));

    ui->itemsListWidget->setIconSize(QSize(32, 32));
}
