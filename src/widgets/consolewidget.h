#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QTabWidget>
#include <QListWidget>

class ConsoleWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ConsoleWidget(QWidget *parent = 0);
    void putCompilationOut(QString text);
    void putCompilationError(QString text);
    void putMessage(QString text);
    void clear();
    void putError(int what, QString desc, QString file, int line);
signals:

public slots:
private:
    void createConnections();

    QTextEdit *m_textEdit;
    QListWidget *m_errors;
};

#endif // CONSOLEWIDGET_H
