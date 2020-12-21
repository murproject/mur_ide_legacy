#include <QVariant>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

#include "debugstore.h"
#include "murgloabal.h"
#include "dispatcher.h"
#include "outputparser.h"

DebugStore::DebugStore(QObject *parent) :
    QObject(parent),
    m_console(new ConsoleWidget),
    m_stdView(new ProgramOutputView),
    m_cameras(new CamerasWidget)
{
    createReactions();
}

QWidget *DebugStore::console()
{
    return m_console;
}

QWidget *DebugStore::cameras()
{
    return m_cameras;
}

DebugStore::~DebugStore()
{
}

void DebugStore::createActions()
{

}

void DebugStore::createReactions()
{
    connect(DISPATCHER->action("Action.Debug.CompileOut"), &QAction::triggered, [this]() {
        QAction *act = DISPATCHER->action("Action.Debug.CompileOut");
        m_console->putCompilationOut(act->data().toString());
        act->setData(QString(""));
    });

    connect(DISPATCHER->action("Action.Debug.Message"), &QAction::triggered, [this]() {
        QAction *act = DISPATCHER->action("Action.Debug.Message");
        m_console->putMessage(act->data().toString());
        act->setData(QString(""));
    });

    connect(DISPATCHER->action("Action.Debug.CompileErr"), &QAction::triggered, [this]() {
        QAction *act = DISPATCHER->action("Action.Debug.CompileErr");
        QString cmpError = act->data().toString();
        QStringList errors = cmpError.split("\r\n");
        for (const auto &str : errors) {
            OutputParser::CompilationInfo info = OutputParser::parseOutput(str);
            if (info.infoType != OutputParser::NONE) {

                m_console->putError(info.infoType, info.description, info.fileName, info.line);
            }
        }
        m_console->putCompilationError(cmpError);
        act->setData(QString(""));
    });

    connect(DISPATCHER->action("Action.Transfer.StdOutErr"), &QAction::triggered, [this]() {
        QJsonObject text = QJsonDocument::fromJson(DISPATCHER->action("Action.Transfer.StdOutErr")->data().toByteArray()).object();

        m_stdView->setErrText(QString(QByteArray::fromBase64(text["stderr"].toVariant().toByteArray())));
        m_stdView->setOutText(QString(QByteArray::fromBase64(text["stdout"].toVariant().toByteArray())));
        m_stdView->show();
    });

    connect(DISPATCHER->action("Action.Source.Compile"), &QAction::triggered, [this]() {
        m_console->clear();
    });
}
