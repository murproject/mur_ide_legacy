#include "projectstore.h"
#include "murgloabal.h"
#include "dispatcher.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QApplication>
#include "colorize.h"

ProjectStore::ProjectStore(QObject *parent) :
    QObject(parent),
    m_compiler(new SourceCompiler(this))
{
    createActions();

    m_context = new ContextWidget;
    m_tree = new ProjectTreeWidget;
    m_itemDialog = new NewItemDialog;
    createReactions();

    connect(&m_executable, &QProcess::readyReadStandardError, this, &ProjectStore::onApplicationOutput);
    connect(&m_executable, &QProcess::readyReadStandardOutput, this, &ProjectStore::onApplicationOutput);
    connect(&m_executable, &QProcess::readyRead, this, &ProjectStore::onApplicationOutput);
    connect(&m_executable, SIGNAL(finished(int)), this, SLOT(onApplicationExit(int)));

}

QWidget *ProjectStore::sourceTree()
{
    return m_tree;
}

QWidget *ProjectStore::context()
{
    return m_context;
}

QWidget *ProjectStore::creatorDialog()
{
    return new QWidget();
}

ProjectStore::~ProjectStore()
{
    delete m_tree;
    delete m_context;
    delete m_itemDialog;
}

void ProjectStore::onApplicationOutput()
{
    QString str = m_executable.readAllStandardOutput();
    if (str != "") {
        QAction *act = DISPATCHER->action("Action.Debug.CompileOut");
        act->setData(str);
        act->trigger();
    }
    str = m_executable.readAllStandardError();
    if (str != "") {
        QAction *act = DISPATCHER->action("Action.Debug.CompileErr");
        act->setData(str);
        act->trigger();

    }
    str = m_executable.readAll();
    if (str != "") {
        QAction *act = DISPATCHER->action("Action.Debug.CompileOut");
        act->setData(str);
        act->trigger();
    }
}

void ProjectStore::onApplicationExit(int ret)
{
    qDebug() << "Application exit";
    QString str = m_executable.readAllStandardOutput();
    if (str != "") {
        QAction *act = DISPATCHER->action("Action.Debug.CompileOut");
        act->setData(str);
        act->trigger();
    }
    str = m_executable.readAllStandardError();
    if (str != "") {
        QAction *act = DISPATCHER->action("Action.Debug.CompileErr");
        act->setData(str);
        act->trigger();

    }
    str = m_executable.readAll();
    if (str != "") {
        QAction *act = DISPATCHER->action("Action.Debug.CompileOut");
        act->setData(str);
        act->trigger();
    }
}


void ProjectStore::createActions()
{
    QAction* act = new QAction(colorizeIcon(":/misc/images/undo.png", QColor(61, 142, 201)), tr("Undo"), this);
    act->setObjectName("Action.Edit.Undo");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(colorizeIcon(":/misc/images/compass.png", QColor(61, 142, 201)), tr("Calibrate"), this);
    act->setObjectName("Action.Transfer.Calibrate");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(colorizeIcon(":/misc/images/redo.png", QColor(61, 142, 201)), tr("Redo"), this);
    act->setObjectName("Action.Edit.Redo");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(colorizeIcon(":/misc/images/save.png", QColor(61, 142, 201)), tr("Save"), this);
    act->setObjectName("Action.Source.Save");
    act->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(colorizeIcon(":/misc/images/open.png", QColor(61, 142, 201)), tr("Open"), this);
    act->setObjectName("Action.Source.Open");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(colorizeIcon(":/misc/images/compile.png", QColor(61, 142, 201)), tr("Compile"), this);
    act->setObjectName("Action.Source.Compile");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(colorizeIcon(":/misc/images/start.png", QColor(61, 142, 201)), tr("Run"), this);
    act->setObjectName("Action.Source.Run");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(colorizeIcon(":/misc/images/new.png", QColor(61, 142, 201)), tr("New"), this);
    act->setObjectName("Action.Source.New");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(colorizeIcon(":/misc/images/settings.png", QColor(61, 142, 201)), tr("Settings"), this);
    act->setObjectName("Action.Ide.Settings");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(colorizeIcon(":/misc/images/stop.png", QColor(61, 142, 201)), tr("Stop"), this);
    act->setObjectName("Action.Source.Stop");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(colorizeIcon(":/misc/images/log.png",  QColor(61, 142, 201)), tr("Log"), this);
    act->setObjectName("Action.Transfer.Log");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(tr("Paste"), this);
    act->setObjectName("Action.Edit.Paste");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(tr("Cut"), this);
    act->setObjectName("Action.Edit.Cut");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(tr("Copy"), this);
    act->setObjectName("Action.Edit.Copy");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(tr("Select all..."), this);
    act->setObjectName("Action.Edit.SelectAll");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(tr("Close project"), this);
    act->setObjectName("Action.Project.Close");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(tr("Delete"), this);
    act->setObjectName("Action.Project.Delete");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Debug.CompileOut");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Debug.CompileErr");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Debug.Message");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Transfer.Binary");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Transfer.Telimetry");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Transfer.StdOutErr");
    DISPATCHER->registerAction(act->objectName(), act);


    act = new QAction(this);
    act->setObjectName("Action.Transfer.ProgramsList");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Transfer.Disconnected");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Transfer.Connected");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Transfer.Running");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Transfer.Notrunning");
    DISPATCHER->registerAction(act->objectName(), act);


    act = new QAction(this);
    act->setObjectName("Action.New.Project");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.New.Class");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.New.HFile");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.New.CPPFile");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Source.Compiled");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Source.CompiledErr");
    DISPATCHER->registerAction(act->objectName(), act);


    act = new QAction(this);
    act->setObjectName("Action.Transfer.LogReply");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Video.Start");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Video.Stop");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Video.Data");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction("Exit", this);
    act->setObjectName("Action.IDE.Close");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Source.ErrorTriggered");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Build.Robot");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Build.Computer");
    DISPATCHER->registerAction(act->objectName(), act);

    act = new QAction(this);
    act->setObjectName("Action.Source.Find");
    DISPATCHER->registerAction(act->objectName(), act);
}

void ProjectStore::createReactions()
{
    connect(DISPATCHER->action("Action.Source.Stop"), &QAction::triggered, [this]() {
        if (Target::Computer == m_target) {
            m_executable.kill();
        }
    });
    connect(DISPATCHER->action("Action.Source.Stops"), &QAction::triggered, [this]() {
        m_project.setTarget(Target::Edison);
        m_compiler->setTarget(Target::Edison);
        m_target = Target::Edison;
    });

    connect(DISPATCHER->action("Action.Build.Robot"), &QAction::triggered, [this]() {
        m_project.setTarget(Target::Edison);
        m_compiler->setTarget(Target::Edison);
        m_target = Target::Edison;
    });

    connect(DISPATCHER->action("Action.Build.Computer"), &QAction::triggered, [this]() {
        m_project.setTarget(Target::Computer);
        m_compiler->setTarget(Target::Computer);
        m_target = Target::Computer;
    });

    //! Open source
    connect(DISPATCHER->action("Action.Source.Open"), &QAction::triggered, [this]() {
        QString fileName = DISPATCHER->action("Action.Source.Open")->data().toString();
        if (fileName.isEmpty()) {
            fileName = QFileDialog::getOpenFileName(nullptr,
                                                    tr("Open project"),
                                                    QStandardPaths::displayName(QStandardPaths::DocumentsLocation),
                                                    tr("Project file (*.mur)"));
        }
        DISPATCHER->action("Action.Source.Open")->setData(QString());
        if (fileName.isEmpty()) {
            return;
        }
        DISPATCHER->action("Action.Project.Close")->trigger();

        if (m_project.open(fileName)) {
            m_context->switchContext(ContextWidget::Mode::EditMode);
        }
        else {
            return;
        }
        m_projectSources = m_project.files();
        m_tree->setSource(m_projectSources);
        m_context->welcomeWidget()->addCurrentFile(fileName, m_project.name());

        if (m_projectSources.isEmpty()) {
            return;
        }

        if (m_tree->topLevelItem(1)->childCount() == 0) {
            return;
        }

        for (int i = 0; i < m_tree->topLevelItem(1)->childCount(); i++) {
            if (m_tree->topLevelItem(1)->child(i)->text(0) == "main.cpp") {
                m_context->setText("main.cpp", m_projectSources["main.cpp"]);
                return;
            }
        }
        QString fname = m_tree->topLevelItem(1)->child(0)->text(0);
        m_context->setText(fname, m_projectSources[fname]);
    });

    //! Close project
    connect(DISPATCHER->action("Action.Project.Close"), &QAction::triggered, [this]() {
        DISPATCHER->action("Action.Source.Save")->trigger();
        m_projectSources.clear();
        m_tree->close();
        m_context->closeAllEditors();
        m_project.close();
        m_projectSources.clear();
        m_context->switchContext(ContextWidget::Mode::WelocmeMode);
    });

    //! On item select
    connect(m_tree, &ProjectTreeWidget::itemDoubleClicked, [this](QTreeWidgetItem* item, int col) {
        Q_UNUSED(col);

        if (item != nullptr) {
            if (item->childCount() == 0) {
                if (item->text(0) != "Sources"  && item->text(0) != "Headers") {
                    m_context->setText(item->text(0), m_projectSources[item->text(0)]);
                }
            }
        }
    });


    //! Save source
    connect(DISPATCHER->action("Action.Source.Save"), &QAction::triggered, [this]() {
        //! FIXME: Probably bug here. Have to iterate over all tabs!
        QVector<QPair<QString, QString> > files = m_context->openedFiles();

        for (int i = 0; i < files.size(); ++i) {
            m_projectSources[files.at(i).first] = files.at(i).second;
        }
        m_project.save(m_projectSources);
    });

    //! Compile project
    connect(DISPATCHER->action("Action.Source.Compile"), &QAction::triggered, [this]() {
        m_context->switchContext(ContextWidget::Mode::EditCompileMode);
        DISPATCHER->action("Action.Source.Save")->trigger();
        if (m_target == Target::Edison) {
            m_project.generateMakeFileEdison();
        } else {
            m_project.generateMakeFilePC();
        }
        m_compiler->compile(m_project.makePath());
        m_context->removePoints();
    });

    connect(DISPATCHER->action("Action.Source.Compiled"), &QAction::triggered, [this]() {
        if (m_isTransferRequest && m_target == Target::Edison) {
            QStringList params;
            params << m_project.name();
            params << QFileInfo(m_project.makePath() + QDir::separator() + m_project.name() + ".bin").canonicalFilePath();
            DISPATCHER->action("Action.Transfer.Binary")->setData(QVariant(params));
            DISPATCHER->action("Action.Transfer.Binary")->trigger();
            m_isTransferRequest = false;
        }

        if (m_isTransferRequest == Target::Computer) {
            m_executable.kill();
            QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
            env.insert("PATH", QDir::cleanPath(QCoreApplication::applicationDirPath() + "/devkit-x86/murlibs/"));

            m_executable.setProcessEnvironment(env);
            QString path = "\"";
            path += QFileInfo(m_project.makePath() + QDir::separator() + m_project.name() + ".exe").canonicalFilePath() + "\"";
            m_executable.start(path);
            QString error = m_executable.errorString();
            if (!error.isEmpty() && error != "Unknown error") {
                QAction *act = DISPATCHER->action("Action.Debug.CompileErr");
                act->setData(+ " " + path);
                act->trigger();
            }
            m_isTransferRequest = false;
        }

        m_context->switchContext(ContextWidget::Mode::EditMode);
    });

    connect(DISPATCHER->action("Action.Source.CompiledErr"), &QAction::triggered, [this]() {
        m_context->switchContext(ContextWidget::Mode::EditMode);
    });


    connect(DISPATCHER->action("Action.Source.Run"), &QAction::triggered, [this]() {
        m_isTransferRequest = true;
        DISPATCHER->action("Action.Source.Compile")->trigger();
    });

    connect(DISPATCHER->action("Action.Source.New"), &QAction::triggered, [this]() {
        if (m_project.isOpen()) {
            m_itemDialog->setProjectOpen(true);
        }
        else {
            m_itemDialog->setProjectOpen(false);
        }
        m_itemDialog->show();

    });

    //! New item
    connect(DISPATCHER->action("Action.New.Project"), &QAction::triggered, [this]() {

        DISPATCHER->action("Action.Source.Save")->trigger();
        DISPATCHER->action("Action.Project.Close")->trigger();
        QString path = QFileDialog::getSaveFileName(nullptr, tr("Project path"), "Project", tr("Project(*.mur)"));
        if (path.isEmpty()) {
            return;
        }
        m_project.create(path, DISPATCHER->action("Action.New.Project")->data().toString());
        if (m_project.open(path)) {
            m_context->switchContext(ContextWidget::Mode::EditMode);
        } else {
            return;
        }
        m_projectSources = m_project.files();
        m_tree->setSource(m_projectSources);
        m_context->welcomeWidget()->addCurrentFile(path, m_project.name());
        QAction *act = DISPATCHER->action("Action.New.CPPFile");
        act->setData("main");
        act->trigger();

        m_projectSources["main.cpp"] = "#include <murAPI.hpp>\n \nint main()\n{\n    return 0;\n}";

        DISPATCHER->action("Action.Source.Save")->trigger();
        m_context->setText("main.cpp", m_projectSources["main.cpp"]);
    });

    connect(DISPATCHER->action("Action.New.Class"), &QAction::triggered, [this]() {
        DISPATCHER->action("Action.Source.Save")->trigger();
        m_project.addClass(DISPATCHER->action("Action.New.Class")->data().toString());
        m_projectSources = m_project.files();
        m_tree->clear();
        m_tree->setSource(m_projectSources);
    });

    connect(DISPATCHER->action("Action.New.HFile"), &QAction::triggered, [this]() {
        DISPATCHER->action("Action.Source.Save")->trigger();
        m_project.addFile(DISPATCHER->action("Action.New.HFile")->data().toString() + ".h");
        m_projectSources = m_project.files();
        m_tree->clear();
        m_tree->setSource(m_projectSources);
    });

    connect(DISPATCHER->action("Action.New.CPPFile"), &QAction::triggered, [this]() {
        DISPATCHER->action("Action.Source.Save")->trigger();
        m_project.addFile(DISPATCHER->action("Action.New.CPPFile")->data().toString() + ".cpp");
        m_projectSources = m_project.files();
        m_tree->clear();
        m_tree->setSource(m_projectSources);
    });

    connect(DISPATCHER->action("Action.Project.Delete"), &QAction::triggered, [this]() {
        DISPATCHER->action("Action.Source.Save")->trigger();
        QString name = m_tree->selectedItems().at(0)->text(0);
        m_project.remove(name);
        m_tree->clear();

        m_projectSources = m_project.files();
        m_tree->clear();
        m_context->removeText(name);
        m_tree->setSource(m_projectSources);
    });

    connect(DISPATCHER->action("Action.IDE.Close"), &QAction::triggered, [this]() {
        QApplication::exit();
    });

    connect(DISPATCHER->action("Action.Transfer.Connected"), &QAction::triggered, [this]() {
        m_context->setConnected(true);
    });

    connect(DISPATCHER->action("Action.Transfer.Disconnected"), &QAction::triggered, [this]() {
        m_context->setConnected(false);
    });

    connect(DISPATCHER->action("Action.Transfer.Running"), &QAction::triggered, [this]() {
        m_context->setRunning(true);
    });

    connect(DISPATCHER->action("Action.Transfer.Notrunning"), &QAction::triggered, [this]() {
        m_context->setRunning(false);
    });

    connect(DISPATCHER->action("Action.Source.ErrorTriggered"), &QAction::triggered, [this]() {
        QAction *act =  DISPATCHER->action("Action.Source.ErrorTriggered");
        QStringList data = act->data().toStringList();

        int line = data.at(1).toInt();
        QString filename = data.at(0);
        if (m_projectSources.find(filename) != m_projectSources.end()) {
            m_context->setText(filename, m_projectSources[filename]);
            m_context->pointLine(filename, line);
        }
    });

}

