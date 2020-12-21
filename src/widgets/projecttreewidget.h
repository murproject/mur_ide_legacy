#ifndef PROJECTTREEWIDGET_H
#define PROJECTTREEWIDGET_H

#include <QTreeWidget>
#include <QMenu>

class ProjectTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ProjectTreeWidget(QWidget *parent = 0);
    void setSource(QMap<QString, QString> source);
    void close();
signals:

public slots:
private slots:
    void contextMenuRequested(const QPoint &pos);
private:
    QTreeWidgetItem* m_headers;
    QTreeWidgetItem* m_sources;
    QMenu* m_context;
    void createMenu();
};

#endif // PROJECTTREEWIDGET_H
