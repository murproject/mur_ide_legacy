#include "programoutputview.h"
#include "ui_programoutputview.h"

ProgramOutputView::ProgramOutputView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgramOutputView)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->stdOutEdit->setReadOnly(true);
    ui->stdErrEdit->setReadOnly(true);
}

ProgramOutputView::~ProgramOutputView()
{
    delete ui;
}

void ProgramOutputView::setErrText(QString text)
{
    ui->stdErrEdit->clear();
    ui->stdErrEdit->appendPlainText(text);
}

void ProgramOutputView::setOutText(QString text)
{
    ui->stdOutEdit->clear();
    ui->stdOutEdit->appendPlainText(text);
}
