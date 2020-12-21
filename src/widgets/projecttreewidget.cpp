#include "projecttreewidget.h"
#include "murgloabal.h"
#include "dispatcher.h"

#include <QDebug>
#include <QFileIconProvider>

ProjectTreeWidget::ProjectTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setColumnCount(1);
    setHeaderHidden(true);
    createMenu();
}

void ProjectTreeWidget::setSource(QMap<QString, QString> source)
{
    QFileIconProvider provider;
    QIcon icon = provider.icon(QFileIconProvider::Folder);
    m_headers = new QTreeWidgetItem(this, QStringList(QString(tr("Headers"))));
    m_headers->setIcon(0,  QIcon(":/misc/images/h-folder.png"));
    m_sources = new QTreeWidgetItem(this, QStringList(QString(tr("Sources"))));
    m_sources->setIcon(0,  QIcon(":/misc/images/cpp-folder.png"));
    addTopLevelItem(m_headers);
    addTopLevelItem(m_sources);

    for (QMap<QString, QString>::iterator it = source.begin(); it != source.end(); ++it) {
        if (it.key().endsWith(".cpp")) {
            QTreeWidgetItem *item;
            item = new QTreeWidgetItem(m_sources, QStringList(it.key()));
            item->setIcon(0, QIcon(":/misc/images/cpp-file.png"));
        }
        if (it.key().endsWith(".h")) {
            QTreeWidgetItem *item;
            item = new QTreeWidgetItem(m_headers, QStringList(it.key()));
            item->setIcon(0, QIcon(":/misc/images/h-file.png"));
        }
    }
    expandAll();
}

void ProjectTreeWidget::close()
{
    clear();
}

void ProjectTreeWidget::contextMenuRequested(const QPoint &pos)
{
    m_context->clear();
    QModelIndex index = indexAt(pos);
    if (index.isValid()) {
        if (index.data().toString() == "Sources" ||
            index.data().toString() == "Headers") {
            m_context->addAction(DISPATCHER->action("Action.Project.Close"));
        } else {
            m_context->addAction(DISPATCHER->action("Action.Project.Delete"));
        }
        m_context->popup(viewport()->mapToGlobal(pos));
    }
}

void ProjectTreeWidget::createMenu()
{
    m_context = new QMenu(this);
    m_context->addAction(DISPATCHER->action("Action.Project.Close"));
    m_context->addAction(DISPATCHER->action("Action.Project.Delete"));
    connect(this, &ProjectTreeWidget::customContextMenuRequested, this, &ProjectTreeWidget::contextMenuRequested);
}
