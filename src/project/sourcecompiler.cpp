#include <thread>
#include <QFileInfo>
#include <QAction>
#include <QDir>
#include <QDebug>
#include "sourcecompiler.h"
#include "murgloabal.h"
#include "dispatcher.h"
#include <QCoreApplication>

SourceCompiler::SourceCompiler(QObject *parent) :
    QObject(parent),
    m_proc(new QProcess(this))
{
    connect(m_proc, &QProcess::readyReadStandardError, this, &SourceCompiler::onProcessOutput);
    connect(m_proc, &QProcess::readyReadStandardOutput, this, &SourceCompiler::onProcessOutput);
    connect(m_proc, &QProcess::readyRead, this, &SourceCompiler::onProcessOutput);

    connect(m_proc, SIGNAL(finished(int)), this, SLOT(onCompilerExit(int)));
//    connect(&m_proc, &QProcess::finished, [this](int ret) {
//        qDebug() << ret;
//    });
}

void SourceCompiler::compile(QString projectPath)
{
    if (projectPath.isEmpty()) {
        return;
    }

    if (m_proc->isOpen()) {
        m_proc->waitForFinished();
    }


    QStringList procArg;
    procArg << "-C";
    procArg << projectPath;
    QString mingwMake = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/devkit-x86/mingw530_32/bin/mingw32-make.exe");
    QFileInfo make(mingwMake);
    m_proc->start(make.absoluteFilePath(), procArg);
    QString err = m_proc->errorString();
    if (!err.isEmpty() && err != "Unknown error") {
        QAction *act = DISPATCHER->action("Action.Debug.CompileErr");
        act->setData(err);
        act->trigger();
    }
}

void SourceCompiler::onProcessOutput()
{
    QString str = m_proc->readAllStandardOutput();
    if (str != "") {
        QAction *act = DISPATCHER->action("Action.Debug.CompileOut");
        act->setData(str);
        act->trigger();
    }
    str = m_proc->readAllStandardError();
    if (str != "") {
        QAction *act = DISPATCHER->action("Action.Debug.CompileErr");
        act->setData(str);
        act->trigger();

    }
    str = m_proc->readAll();
    if (str != "") {
        QAction *act = DISPATCHER->action("Action.Debug.CompileOut");
        act->setData(str);
        act->trigger();
    }
}

void SourceCompiler::setTarget(Target target)
{
    m_target = target;
}

void SourceCompiler::onCompilerExit(int ret)
{
    if (ret == 0) {
        QAction *act = DISPATCHER->action("Action.Debug.Message");
        act->setData(tr("Compelation success!"));
        act->trigger();
        act = DISPATCHER->action("Action.Source.Compiled");
        act->trigger();
    }
    else {
        QAction *act = DISPATCHER->action("Action.Debug.Message");
        DISPATCHER->action("Action.Source.CompiledErr")->trigger();

        act->setData(tr("Compelation failed!"));
        act->trigger();
    }

}
