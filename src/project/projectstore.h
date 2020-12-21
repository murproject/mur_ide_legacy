 #ifndef PROJECTSTORE_H
#define PROJECTSTORE_H

#include <QObject>

#include "texteditwidget.h"
#include "contextwidget.h"
#include "projecttreewidget.h"
#include "project.h"
#include "sourcecompiler.h"
#include "newitemdialog.h"

class ProjectStore : public QObject
{
    Q_OBJECT
public:
    explicit ProjectStore(QObject *parent = 0);

    QWidget *editor();
    QWidget *sourceTree();
    QWidget *context();
    QWidget *creatorDialog();
    ~ProjectStore();


private slots:
    void onApplicationOutput();
    void onApplicationExit(int ret);
private:
    void createActions();
    void createReactions();
    SourceCompiler *m_compiler;
    NewItemDialog *m_itemDialog;
    ContextWidget *m_context;
    ProjectTreeWidget *m_tree;
    Project m_project;
    QMap<QString, QString> m_projectSources;
    bool m_isTransferRequest = false;
    Target m_target = Target::Edison;
    QProcess m_executable;
};

#endif // PROJECTSTORE_H
