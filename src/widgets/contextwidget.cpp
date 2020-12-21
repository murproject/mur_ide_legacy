#include "contextwidget.h"
#include "murgloabal.h"
#include "dispatcher.h"
#include <QTabBar>
#include <QTabWidget>
#include <QDebug>

ContextWidget::ContextWidget(QWidget *parent) :
    QStackedWidget(parent),
    m_welcomeWidget(new WelcomeWidget(this)),
    m_tabWidget(new QTabWidget(this))
{
    addWidget(m_welcomeWidget);
    addWidget(m_tabWidget);
    m_tabWidget->setTabsClosable(true);
    m_tabWidget->setMovable(true);

    setWelcomeState();
    m_welcomeWidget->loadRecentFiles();
    m_welcomeWidget->loadExamples();
    createConnections();
}

void ContextWidget::switchContext(ContextWidget::Mode mode)
{
    m_mode = mode;

    if (mode == Mode::EditMode) {
        setCurrentIndex(1);
        if (m_isConnected) {
            setProjectState();
        }
        else {
            setProjecDisconnecedState();
        }
    }
    if (mode == Mode::WelocmeMode) {
        setCurrentIndex(0);
        setWelcomeState();
        m_welcomeWidget->loadRecentFiles();
        m_welcomeWidget->loadExamples();
    }

    if (mode == Mode::EditCompileMode) {
        if (m_isConnected) {
            setProjectCompileState();
        }
        else {
            setProjectCompileDisconnecedState();
        }
    }
}


WelcomeWidget *ContextWidget::welcomeWidget()
{
    return m_welcomeWidget;
}

void ContextWidget::setConnected(bool connected)
{
    m_isConnected = connected;
    switchContext(m_mode);
}

void ContextWidget::setRunning(bool running)
{
    m_isAppRunning = running;
    if (m_isConnected) {
        DISPATCHER->action("Action.Source.Run")->setEnabled(!m_isAppRunning);
        DISPATCHER->action("Action.Source.Stop")->setEnabled(m_isAppRunning);
    }
}

void ContextWidget::removeText(QString name)
{
    for (int i = 0; i < m_tabWidget->count(); ++i) {
       if (m_tabWidget->tabText(i) == name)
       {
           m_tabWidget->removeTab(i);
           return;
       }
    }
}

void ContextWidget::update()
{
    switchContext(m_mode);
}

void ContextWidget::setText(QString name, QString text)
{
    for (int i = 0; i < m_tabWidget->count(); ++i) {
        if (cutAmpersand(m_tabWidget->tabText(i)) == name) {
            m_tabWidget->setCurrentIndex(i);
            return;
        }
    }

    TextEditWidget* te = new TextEditWidget;
    te->setText(text);
    m_tabWidget->addTab(te, name);
    m_tabWidget->setCurrentIndex(m_tabWidget->count() - 1);
}

QString ContextWidget::getCurrentFile()
{
    return cutAmpersand(m_tabWidget->tabText(m_tabWidget->currentIndex()));
}

QString ContextWidget::currentFileText()
{
    return ((TextEditWidget*)m_tabWidget->widget(m_tabWidget->currentIndex()))->text();
}

void ContextWidget::closeAllEditors()
{
    for (int i = m_tabWidget->count() - 1; i > m_tabWidget->currentIndex(); --i) {
       m_tabWidget->removeTab(i);
    }

    for (int i = m_tabWidget->count(); i >= 1; --i) {
       m_tabWidget->removeTab(0);
    }
}

QVector<QPair<QString, QString> > ContextWidget::openedFiles()
{
    QVector<QPair<QString, QString> > result;
    for (int i = 0; i < m_tabWidget->count(); ++i) {
        result.push_back(qMakePair(cutAmpersand(m_tabWidget->tabText(i)), ((TextEditWidget*)m_tabWidget->widget(i))->text()));

    }
    return result;
}

void ContextWidget::pointLine(QString file, int line)
{
    int index = -1;
    for (int i = 0; i < m_tabWidget->count(); ++i) {
        if (cutAmpersand(m_tabWidget->tabText(i)) == file) {
            m_tabWidget->setCurrentIndex(i);
            index = i;
        }
    }

    if (index == -1) {
        return;
    }
    TextEditWidget* widget = dynamic_cast<TextEditWidget*>(m_tabWidget->widget(index));
    widget->setFocus();
    widget->markerAdd(line - 1, 0);
    widget->setCursorPosition(line - 1, 0);
}

void ContextWidget::removePoints()
{
    for (int i = 0; i < m_tabWidget->count(); ++i) {
        TextEditWidget* widget = dynamic_cast<TextEditWidget*>(m_tabWidget->widget(i));
        if (widget != nullptr) {
            widget->markerDeleteAll();
        }
    }
}

void ContextWidget::createConnections()
{
    connect(m_tabWidget, &QTabWidget::tabCloseRequested, [this](int index){
        m_tabWidget->removeTab(index);
    });
}

void ContextWidget::setWelcomeState()
{
    DISPATCHER->action("Action.Edit.Undo")->setEnabled(false);
    DISPATCHER->action("Action.Edit.Redo")->setEnabled(false);
    DISPATCHER->action("Action.Source.Save")->setEnabled(false);
    DISPATCHER->action("Action.Source.Compile")->setEnabled(false);
    DISPATCHER->action("Action.Source.Run")->setEnabled(false);
    DISPATCHER->action("Action.Edit.Paste")->setEnabled(false);
    DISPATCHER->action("Action.Edit.Cut")->setEnabled(false);
    DISPATCHER->action("Action.Edit.Copy")->setEnabled(false);
    DISPATCHER->action("Action.Edit.SelectAll")->setEnabled(false);
    DISPATCHER->action("Action.Source.Stop")->setEnabled(false);
    DISPATCHER->action("Action.Transfer.Calibrate")->setEnabled(false);
}

void ContextWidget::setProjectState()
{
    DISPATCHER->action("Action.Edit.Undo")->setEnabled(true);
    DISPATCHER->action("Action.Edit.Redo")->setEnabled(true);
    DISPATCHER->action("Action.Source.Save")->setEnabled(true);
    DISPATCHER->action("Action.Source.Compile")->setEnabled(true);
    DISPATCHER->action("Action.Source.Run")->setEnabled(true);
    DISPATCHER->action("Action.Edit.Paste")->setEnabled(true);
    DISPATCHER->action("Action.Edit.Cut")->setEnabled(true);
    DISPATCHER->action("Action.Edit.Copy")->setEnabled(true);
    DISPATCHER->action("Action.Edit.SelectAll")->setEnabled(true);
    DISPATCHER->action("Action.Source.Stop")->setEnabled(true);
    DISPATCHER->action("Action.Transfer.Calibrate")->setEnabled(true);
}

void ContextWidget::setProjectCompileState()
{
    DISPATCHER->action("Action.Edit.Undo")->setEnabled(true);
    DISPATCHER->action("Action.Edit.Redo")->setEnabled(true);
    DISPATCHER->action("Action.Source.Save")->setEnabled(true);
    DISPATCHER->action("Action.Source.Compile")->setEnabled(false);
    DISPATCHER->action("Action.Source.Run")->setEnabled(false);
    DISPATCHER->action("Action.Edit.Paste")->setEnabled(true);
    DISPATCHER->action("Action.Edit.Cut")->setEnabled(true);
    DISPATCHER->action("Action.Edit.Copy")->setEnabled(true);
    DISPATCHER->action("Action.Edit.SelectAll")->setEnabled(true);
    DISPATCHER->action("Action.Source.Stop")->setEnabled(false);
    DISPATCHER->action("Action.Transfer.Calibrate")->setEnabled(false);
}

void ContextWidget::setProjecDisconnecedState()
{

    DISPATCHER->action("Action.Edit.Undo")->setEnabled(true);
    DISPATCHER->action("Action.Edit.Redo")->setEnabled(true);
    DISPATCHER->action("Action.Source.Save")->setEnabled(true);
    DISPATCHER->action("Action.Source.Compile")->setEnabled(true);
    DISPATCHER->action("Action.Source.Run")->setEnabled(true);
    DISPATCHER->action("Action.Edit.Paste")->setEnabled(true);
    DISPATCHER->action("Action.Edit.Cut")->setEnabled(true);
    DISPATCHER->action("Action.Edit.Copy")->setEnabled(true);
    DISPATCHER->action("Action.Edit.SelectAll")->setEnabled(true);
    DISPATCHER->action("Action.Source.Stop")->setEnabled(true);
    DISPATCHER->action("Action.Transfer.Calibrate")->setEnabled(false);
}

void ContextWidget::setProjectCompileDisconnecedState()
{
    DISPATCHER->action("Action.Edit.Undo")->setEnabled(true);
    DISPATCHER->action("Action.Edit.Redo")->setEnabled(true);
    DISPATCHER->action("Action.Source.Save")->setEnabled(true);
    DISPATCHER->action("Action.Source.Compile")->setEnabled(false);
    DISPATCHER->action("Action.Source.Run")->setEnabled(false);
    DISPATCHER->action("Action.Edit.Paste")->setEnabled(true);
    DISPATCHER->action("Action.Edit.Cut")->setEnabled(true);
    DISPATCHER->action("Action.Edit.Copy")->setEnabled(true);
    DISPATCHER->action("Action.Edit.SelectAll")->setEnabled(true);
    DISPATCHER->action("Action.Source.Stop")->setEnabled(false);
    DISPATCHER->action("Action.Transfer.Calibrate")->setEnabled(false);
}

QString ContextWidget::cutAmpersand(QString tabName)
{
    int firstAmpersandPos = tabName.indexOf('&');

    if (firstAmpersandPos > -1) {
        tabName.remove(firstAmpersandPos, 1);
    }

    return tabName;
}


