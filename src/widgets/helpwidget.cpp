#include "helpwidget.h"
#include "murgloabal.h"
#include "dispatcher.h"

#include <QAction>
#include <QDebug>

HelpWidget::HelpWidget(QWidget *parent) :
    QTextBrowser(parent),
    m_helpEngine(new QHelpEngine("help/help.qhc", this))
{
   // setSource(QUrl("qthelp://doc.html"));
    setSource(QUrl("doc.html"));
    createConnections();
}

QVariant HelpWidget::loadResource(int type, const QUrl &name)
{
    if (name.scheme() == "qthelp") {
         return QVariant(m_helpEngine->fileData(name));
     }
     else {
         return QTextBrowser::loadResource(type, name);
    }
}

void HelpWidget::createConnections()
{
    connect(DISPATCHER->action("Action.Source.Find"), &QAction::triggered, [this]() {
        QAction *act = DISPATCHER->action("Action.Source.Find");
        QString toFind = act->data().toString();

        QTextCursor cursor = textCursor();
        cursor.setPosition(0);
        setTextCursor(cursor);

        qDebug() << toFind;
        find(toFind, QTextDocument::FindCaseSensitively);
    });
}
