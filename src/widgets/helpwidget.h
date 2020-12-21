#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>
#include <QTextBrowser>
#include <QHelpEngine>
#include <QKeyEvent>
#include <QDropEvent>
#include <QMenu>
#include <QAction>

class HelpWidget : public QTextBrowser
{
    Q_OBJECT
public:
    explicit HelpWidget(QWidget *parent = nullptr);

public slots:
    QVariant loadResource(int type, const QUrl &name);
private:
    void createConnections();
    QHelpEngine *m_helpEngine;
};

#endif // HELPWIDGET_H
