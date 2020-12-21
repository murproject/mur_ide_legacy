#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>

#include "contextwidget.h"
#include "projectstore.h"
#include "debugstore.h"
#include "connectivitystorage.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void onIndexChanged(int index);
private slots:
    void keyPressEvent(QKeyEvent *event);
private:
    void createToolBar();
    void createDockWidgets();
    void createMenus();
    void createConnections();
    QAction *m_helpViewAction;
    QDockWidget* m_helpWidget;
    bool m_isHelpHidden;
    ProjectStore *m_projectStore;
    DebugStore *m_debug;
    ConnectivityStorage *m_connectivity;
    QComboBox *m_buildTypeBox;

};

#endif // MAINWINDOW_H
