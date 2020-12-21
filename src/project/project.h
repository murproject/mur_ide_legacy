#ifndef PROJECT_H
#define PROJECT_H

#include <QMap>
#include <QJsonObject>

#include "sourcecompiler.h"

class Project
{
public:
    Project();
    Project(QString path);

    QMap<QString, QString> files();
    QString name();
    QString makePath();
    bool open(QString path);
    bool close();
    bool isOpen();
    bool create(QString path, QString name);
    bool addFile(QString name);
    bool addClass(QString name);
    bool save(QMap<QString, QString> sources);
    bool remove(QString name);
    void generateMakeFileEdison();
    void generateMakeFilePC();
    QString description();
    void setTarget(Target target);

private:
    void writeProject();
    QMap<QString, QString> fetchFiles();
    QJsonObject m_project;
    QString m_path;
    bool m_isOpen = false;
    Target m_target = Target::Edison;
};

#endif // PROJECT_H
