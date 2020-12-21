#ifndef PROGRAMMOUTPUTVIEW_H
#define PROGRAMMOUTPUTVIEW_H

#include <QDialog>

namespace Ui {
class ProgramOutputView;
}

class ProgramOutputView : public QDialog
{
    Q_OBJECT

public:
    explicit ProgramOutputView(QWidget *parent = 0);
    ~ProgramOutputView();
    void setErrText(QString text);
    void setOutText(QString text);
private:
    Ui::ProgramOutputView *ui;
};

#endif // PROGRAMMOUTPUTVIEW_H
