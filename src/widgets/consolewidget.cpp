#include "consolewidget.h"
#include "issuewidget.h"
#include <QListWidgetItem>
#include <QDebug>
#include "murgloabal.h"
#include "dispatcher.h"

QString rightTrimmed(const QString &in)
{
    int pos = in.length();
    for (; pos > 0; --pos) {
        if (!in.at(pos - 1).isSpace())
            break;
    }
    return in.mid(0, pos);
}


ConsoleWidget::ConsoleWidget(QWidget *parent) :
    QTabWidget(parent),
    m_textEdit(new QTextEdit(this)),
    m_errors(new QListWidget(this))
{
    m_textEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    m_textEdit->setReadOnly(true);
    QFont font = QFont("Courier");
    font.setPointSize(10);
    m_textEdit->setFont(font);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addTab(m_textEdit, tr("Console"));
    addTab(m_errors, tr("Issues"));
    createConnections();
    setTabPosition(QTabWidget::South);
}

void ConsoleWidget::putCompilationOut(QString text)
{
    m_textEdit->append(rightTrimmed(text));
}

void ConsoleWidget::putCompilationError(QString text)
{
    static QString alertHtml = "<font color=\"DeepPink\">";
    static QString endHtml = "</font>";
    text = alertHtml + text;
    text += endHtml;
    m_textEdit->append(text);
}

void ConsoleWidget::putMessage(QString text)
{
    static QString msgHtml = "<font color=\"Aqua\">";
    static QString endHtml = "</font>";
    text = msgHtml + text;
    text += endHtml;
    m_textEdit->append(text);
}

void ConsoleWidget::clear()
{
    m_textEdit->clear();
    m_errors->clear();
}

void ConsoleWidget::putError(int what, QString desc, QString file, int line)
{
    QListWidgetItem *listWidgetItem = new QListWidgetItem;
    m_errors->addItem(listWidgetItem);
    IssueWidget *item = new IssueWidget;
    listWidgetItem->setSizeHint (QSize(item->sizeHint().width(), 32));
    m_errors->setItemWidget (listWidgetItem, item);
    item->setData(line, file, desc, what);
}

void ConsoleWidget::createConnections()
{
    connect(m_errors, &QListWidget::itemDoubleClicked, [this](QListWidgetItem *item) {
        IssueWidget *widget = dynamic_cast<IssueWidget*>(m_errors->itemWidget(item));
        //"Action.Source.ErrorTriggered"
        QStringList data;
        data << widget->filename() << QString("%1").arg(widget->line());
        DISPATCHER->action("Action.Source.ErrorTriggered")->setData(data);
        DISPATCHER->action("Action.Source.ErrorTriggered")->trigger();
    });

}
