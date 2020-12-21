#ifndef PROGRAMMSLISTWIDGET_H
#define PROGRAMMSLISTWIDGET_H

#include <QDialog>

namespace Ui {
class ProgrammsListWidget;
}

class ProgrammsListWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ProgrammsListWidget(QWidget *parent = 0);
    ~ProgrammsListWidget();

private:
    Ui::ProgrammsListWidget *ui;
};

#endif // PROGRAMMSLISTWIDGET_H
