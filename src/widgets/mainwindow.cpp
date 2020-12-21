#include <QToolBar>
#include <QApplication>
#include <QDockWidget>
#include <QMenuBar>
#include <QFile>
#include <QSettings>
#include <QDebug>

#include "helpwidget.h"
#include "mainwindow.h"
#include "murgloabal.h"
#include "dispatcher.h"
#include "consolewidget.h"
#include "batterystatuswidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_projectStore(new ProjectStore(this)),
      m_debug(new DebugStore(this)),
      m_connectivity(new ConnectivityStorage(this)),
      m_buildTypeBox(new QComboBox(this))
{
    createMenus();
    createToolBar();
    createDockWidgets();
    setCentralWidget(m_projectStore->context());
    createConnections();


    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "RoboticsCenter", "murIDE");
    QByteArray state = settings.value("State", 0).toByteArray();
    if (state.size() != 0) {
        restoreState(state);
    }
    showMaximized();

}

MainWindow::~MainWindow()
{
    QByteArray state = saveState();
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "RoboticsCenter", "murIDE");
    settings.setValue("State", state);
}

void MainWindow::onIndexChanged(int index)
{
    if (index == 0) {
        DISPATCHER->action("Action.Build.Robot")->trigger();
    }

    if (index == 1) {
        DISPATCHER->action("Action.Build.Computer")->trigger();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case (Qt::Key_F1):
        if (m_helpWidget->isHidden()) {
            m_helpViewAction->trigger();
        }
        break;
    default:
        break;
    }
}

void MainWindow::createToolBar()
{
    QToolBar* toolBar = new QToolBar(tr("Tools"), this);
    toolBar->setObjectName("ToolBar");
    addToolBar(Qt::TopToolBarArea, toolBar);
    toolBar->setMovable(false);
    toolBar->setIconSize(QSize(22, 22));
    QAction* act;

    //Run
    act = DISPATCHER->action("Action.Source.Run");
    toolBar->addAction(act);
    //Stop
    act = DISPATCHER->action("Action.Source.Stop");
    toolBar->addAction(act);
    //Compile
    act = DISPATCHER->action("Action.Source.Compile");
    toolBar->addAction(act);

    toolBar->addSeparator();

    //Undo
    act = DISPATCHER->action("Action.Edit.Undo");
    toolBar->addAction(act);
    //Redo
    act = DISPATCHER->action("Action.Edit.Redo");
    toolBar->addAction(act);

    toolBar->addSeparator();

    //Save
    act = DISPATCHER->action("Action.Source.Save");
    toolBar->addAction(act);
    //Open
    act = DISPATCHER->action("Action.Source.Open");
    toolBar->addAction(act);
    //New
    act = DISPATCHER->action("Action.Source.New");
    toolBar->addAction(act);

    toolBar->addSeparator();

    //Settings
    act = DISPATCHER->action("Action.Ide.Settings");
    toolBar->addAction(act);

    //Log
    act = DISPATCHER->action("Action.Transfer.Log");
    toolBar->addAction(act);

    m_buildTypeBox->addItem(tr("Robot"));
    m_buildTypeBox->addItem(tr("Computer"));

    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    toolBar->addWidget(spacer);
    toolBar->addWidget(m_buildTypeBox);
}

void MainWindow::createDockWidgets()
{
    QMenu* view = menuBar()->addMenu(tr("&View"));

    QDockWidget *consDock = new QDockWidget(tr("Console"), this);
    consDock->setObjectName("ConsoleDockWidget");
    consDock->setAllowedAreas(Qt::BottomDockWidgetArea);
    consDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    consDock->setWidget(m_debug->console());
    addDockWidget(Qt::BottomDockWidgetArea, consDock);

    QDockWidget *projTreeDock = new QDockWidget(tr("Projects tree"), this);
    projTreeDock->setObjectName("ProjectsTreeDockWidget");
    projTreeDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    projTreeDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    projTreeDock->setWidget(m_projectStore->sourceTree());
    addDockWidget(Qt::LeftDockWidgetArea, projTreeDock);

    QDockWidget *devDock = new QDockWidget(tr("Devices"), this);

    devDock->setObjectName("DevicesDockWidget");
    devDock->setTitleBarWidget(new BatteryStatusWidget);
    devDock->setAllowedAreas(Qt::BottomDockWidgetArea);
    devDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    devDock->setWidget(m_connectivity->devices());
    addDockWidget(Qt::BottomDockWidgetArea, devDock);
    splitDockWidget(devDock, consDock, Qt::Horizontal);

    m_helpWidget = new QDockWidget(tr("Help"), this);
    m_helpWidget->setObjectName("HelpDockWidget");
    m_helpWidget->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    m_helpWidget->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    m_helpWidget->setWidget(new HelpWidget(this));
    addDockWidget(Qt::RightDockWidgetArea, m_helpWidget);

    QDockWidget *cameras = new QDockWidget(tr("Camera"), this);
    cameras->setObjectName("CameraDockWidget");
    cameras->setAllowedAreas(Qt::BottomDockWidgetArea);
   // cameras->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    cameras->setWidget(m_debug->cameras());
    addDockWidget(Qt::BottomDockWidgetArea, cameras);

    view->addAction(consDock->toggleViewAction());
    view->addAction(projTreeDock->toggleViewAction());
    view->addAction(devDock->toggleViewAction());
    view->addAction(m_helpWidget->toggleViewAction());
    view->addAction(cameras->toggleViewAction());
    m_helpViewAction = m_helpWidget->toggleViewAction();

    cameras->hide();
    devDock->hide();
    m_helpWidget->hide();
}

void MainWindow::createMenus()
{
    QMenu* file = menuBar()->addMenu(tr("&File"));
    file->addAction(DISPATCHER->action("Action.Source.Save"));
    file->addAction(DISPATCHER->action("Action.Source.Open"));
    file->addAction(DISPATCHER->action("Action.Source.New"));
    file->addAction(DISPATCHER->action("Action.Project.Close"));
    file->addSeparator();
    file->addAction(DISPATCHER->action("Action.IDE.Close"));

    QMenu* edit = menuBar()->addMenu(tr("&Edit"));
    edit->addAction(DISPATCHER->action("Action.Edit.Undo"));
    edit->addAction(DISPATCHER->action("Action.Edit.Redo"));
    edit->addAction(DISPATCHER->action("Action.Edit.Cut"));
    edit->addAction(DISPATCHER->action("Action.Edit.Copy"));
    edit->addAction(DISPATCHER->action("Action.Edit.Paste"));
    edit->addAction(DISPATCHER->action("Action.Edit.SelectAll"));

    QMenu* build = menuBar()->addMenu(tr("&Build"));
    build->addAction(DISPATCHER->action("Action.Source.Run"));
    build->addAction(DISPATCHER->action("Action.Source.Stop"));
    build->addAction(DISPATCHER->action("Action.Source.Compile"));
}

void MainWindow::createConnections()
{
    connect(m_buildTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onIndexChanged(int)));
    connect(DISPATCHER->action("Action.Source.Find"), &QAction::triggered, [this]() {

    });
}
