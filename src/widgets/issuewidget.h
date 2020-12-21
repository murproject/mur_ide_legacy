#ifndef ISSUEWIDGET_H
#define ISSUEWIDGET_H

#include <QWidget>
#include <QLabel>

class IssueWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IssueWidget(QWidget *parent = 0);
    void setData(int line, QString file, QString desc, int type);
    QString filename();
    int line();
signals:

public slots:
private:
    void createLayout();

    QString m_name;
    QString m_desc;
    int m_type;
    int m_line;

    QLabel *m_text;
    QLabel *m_image;
};

#endif // ISSUEWIDGET_H
