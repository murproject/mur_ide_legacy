#ifndef NEWITEMDIALOG_H
#define NEWITEMDIALOG_H

#include <QDialog>

namespace Ui {
class NewItemDialog;
}

class NewItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewItemDialog(QWidget *parent = 0);
    ~NewItemDialog();
    void setProjectOpen(bool flag);

private:
    void createItems();
    Ui::NewItemDialog *ui;
};

#endif // NEWITEMDIALOG_H
