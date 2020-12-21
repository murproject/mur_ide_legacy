#ifndef CONTEXTWIDGET_H
#define CONTEXTWIDGET_H

#include <QStackedWidget>

#include "texteditwidget.h"
#include "welcomewidget.h"

class ContextWidget : public QStackedWidget
{
    Q_OBJECT
public:
    enum Mode {
        EditMode,
        WelocmeMode,
        EditCompileMode
    };
    explicit ContextWidget(QWidget *parent = 0);
    void switchContext(Mode mode);
    WelcomeWidget* welcomeWidget();
    void setConnected(bool connected);
    void setRunning(bool running);
    void removeText(QString name);
    void update();
    void setText(QString name, QString text);
    QString getCurrentFile();
    QString currentFileText();
    void closeAllEditors();
    QVector<QPair<QString, QString>> openedFiles();
    void pointLine(QString file, int line);
    void removePoints();
signals:

public slots:
private:
    void createConnections();

    bool m_isConnected = false;
    bool m_isAppRunning = false;

    void setWelcomeState();
    void setProjectState();
    void setProjectCompileState();
    void setProjecDisconnecedState();
    void setProjectCompileDisconnecedState();
    QString cutAmpersand(QString tabName);

    ContextWidget::Mode m_mode;

    WelcomeWidget* m_welcomeWidget;
    QTabWidget* m_tabWidget;
};

#endif // CONTEXTWIDGET_H
