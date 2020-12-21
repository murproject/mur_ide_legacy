#include "programslistwidget.h"
#include "ui_programslistwidget.h"

ProgrammsListWidget::ProgrammsListWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgrammsListWidget)
{
    ui->setupUi(this);
}

ProgrammsListWidget::~ProgrammsListWidget()
{
    delete ui;
}
