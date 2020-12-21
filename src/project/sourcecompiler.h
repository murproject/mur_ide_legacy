#ifndef SOURCECOMPILER_H
#define SOURCECOMPILER_H

#include <QObject>
#include <QProcess>

enum Target {
    Edison = 0,
    Computer = 1
};

class SourceCompiler : public QObject
{
    Q_OBJECT
public:
    explicit SourceCompiler(QObject *parent = 0);
    void compile(QString projectPath);
signals:
    void textRecived(QString);
    void finished(int);
public slots:
    void onProcessOutput();
    void setTarget(Target target);
private slots:
    void onCompilerExit(int ret);
private:
    QProcess *m_proc;
    QThread *m_processThread;
    Target  m_target = Target::Edison;
};

#endif // SOURCECOMPILER_H
