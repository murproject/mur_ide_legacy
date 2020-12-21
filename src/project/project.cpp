#include "project.h"
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QWidget>
#include <QJsonDocument>
#include <QFileInfo>
#include <QDir>
#include <QJsonArray>
#include <QVariant>
#include <QDebug>
#include <QTextCodec>
#include <vector>
#include <QCoreApplication>

Project::Project()
{

}

QMap<QString, QString> Project::files()
{
    return fetchFiles();
}

QString Project::name()
{
    if (m_isOpen) {
        return m_project["name"].toString();
    } else {
        return "";
    }
}

QString Project::makePath()
{
    if (m_target == Target::Edison) {
        generateMakeFileEdison();
    } else {
        generateMakeFilePC();
    }
    QFileInfo info(m_path);
    QString path = info.dir().canonicalPath();
    return path;
}

bool Project::open(QString path)
{
    QFile project(path);
    if (!project.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, QObject::tr("Project open"), QObject::tr("Unable to open project!"), QMessageBox::Ok);
        return false;
    }
    m_path = path;

    QJsonParseError err;
    m_project = QJsonDocument::fromJson(project.readAll(), &err).object();
    if (Q_UNLIKELY(err.error != QJsonParseError::NoError)) {
        QMessageBox::warning(nullptr, QObject::tr("Project open"), QObject::tr("Unable to open project! Invalid project format."), QMessageBox::Ok);
        return false;
    }

    m_isOpen = true;
    return true;
}

bool Project::close()
{
    m_project = QJsonObject();
    m_path = "";
    m_isOpen = false;
    return m_isOpen;
}

bool Project::isOpen()
{
    return m_isOpen;
}

bool Project::create(QString path, QString name)
{
    QFile file(path);
    QJsonObject project;
    project["name"] = QJsonValue(name);
    project["sources"] = QJsonArray();
    project["headers"] = QJsonArray();
    project["description"] = QJsonValue();
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, QObject::tr("Project create"), QObject::tr("Unable to create file!") + path, QMessageBox::Ok);
        return false;
    }
    file.write(QJsonDocument(project).toJson());
    return true;
}

bool Project::addFile(QString name)
{
    QFileInfo info(m_path);
    QString path = info.dir().canonicalPath();
    QString fileName = path + QDir::separator() + name;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, QObject::tr("Project addFile"), QObject::tr("Unable to create file!") + fileName, QMessageBox::Ok);
        return false;
    }
    if (name.endsWith(".h")) {
        QJsonArray headers = m_project["headers"].toArray();
        headers.append(name);
        m_project["headers"] = headers;
    }

    if (name.endsWith(".cpp")) {
        QJsonArray sources = m_project["sources"].toArray();
        sources.append(name);
        m_project["sources"] = sources;
    }
    writeProject();
    return true;
}

bool Project::addClass(QString name)
{
    QFileInfo info(m_path);
    QString path = info.dir().canonicalPath();
    QString cpp = path + QDir::separator() + name + ".cpp";
    QString h = path + QDir::separator() + name + ".h";
    QFile cppfile(cpp);
    if (!cppfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, QObject::tr("Project addClass"), QObject::tr("Unable to create file!") + cpp, QMessageBox::Ok);
        return false;
    }
    QJsonArray sources = m_project["sources"].toArray();
    sources.append(name + ".cpp");
    m_project["sources"] = sources;
    QFile hfile(h);
    if (!hfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, QObject::tr("Project addClass"), QObject::tr("Unable to create file!") + h, QMessageBox::Ok);
        return false;
    }
    QJsonArray headers = m_project["headers"].toArray();
    headers.append(name + ".h");
    m_project["headers"] = headers;
    writeProject();
    return true;
}

bool Project::save(QMap<QString, QString> sources)
{
    QFileInfo info(m_path);
    QString path = info.dir().canonicalPath();

    for (QMap<QString, QString>::iterator it = sources.begin(); it != sources.end(); ++it) {
        QFile file(path + QDir::separator() + it.key());

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(nullptr, QObject::tr("Project save"), QObject::tr("Unable to write file!") + it.key(), QMessageBox::Ok);
            return false;
        }
        QTextStream streamFileOut(&file);
        streamFileOut.setCodec("UTF-8");
        streamFileOut.setGenerateByteOrderMark(true);
        streamFileOut << it.value();
        streamFileOut.flush();
        file.close();
    }
    return true;
}

bool Project::remove(QString name)
{
    QFileInfo info(m_path);
    QString path = info.dir().canonicalPath();
    if (name.endsWith(".h")) {
        QJsonArray headers = m_project["headers"].toArray();
        for (int i = 0; i < headers.size(); ++i) {
            if (headers.at(i).toString() == name) {
                headers.removeAt(i);
                QFile file(QDir::cleanPath(path + QDir::separator() + name));
                file.open(QIODevice::ReadWrite);
                file.remove();
                break;
            }
        }
        m_project["headers"] = headers;
    }
    if (name.endsWith(".cpp")) {
        QJsonArray sources = m_project["sources"].toArray();
        for (int i = 0; i < sources.size(); ++i) {
            if (sources.at(i).toString() == name) {
                sources.removeAt(i);
                QFile file(QDir::cleanPath(path + QDir::separator() + name));
                file.open(QIODevice::ReadWrite);
                file.remove();
                break;
            }
        }
        m_project["sources"] = sources;
    }
    writeProject();
    return true;
}


void Project::generateMakeFileEdison()
{
    if (!m_isOpen) {
        return;
    }
    QString sysroot = "\"" + QDir::cleanPath(QCoreApplication::applicationDirPath() + "/devkit-x86/sysroots/i586-poky-linux") + "\"";
    QString compiler = "\"" + QDir::cleanPath(QCoreApplication::applicationDirPath() + "/devkit-x86/sysroots/x86_64-pokysdk-mingw32/usr/bin/i586-poky-linux/i586-poky-linux-g++.exe") + "\"";
    QString make = QString(
    "CXX = %3\n"
    "CXXFLAGS = -std=c++11 -Wall -O2 --sysroot=%2\n\n"

    "INCLUDES = -I.\n"
    "LIBS = -lopencv_core -lopencv_highgui -lopencv_imgproc -lzmq\n\n"

    "cxxs = $(wildcard *.cpp)\n"
    "objs = $(patsubst %.cpp,%.o,$(cxxs))\n"
    "ds = $(wildcard *.d)\n\n"

    "all: %1\n\n"

    "include $(ds)\n\n"

    "%1: $(objs)\n\n"
        "\t$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o %1.bin\n\n"

    " %.o: %.cpp\n\n"
        "\t$(CXX) $(CXXFLAGS) -c -MD $< $(INCLUDES) -o $@\n\n"

    "clean:\n"
        "\trm -r *.d\n"
        "\trm -r *.o\n"
        "\trm %1\n").arg(name()).arg(sysroot).arg(compiler);

    QFileInfo info(m_path);
    QString path = info.dir().canonicalPath();

    QFile file(path + QDir::separator() + "makefile");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, QObject::tr("Project save"), QObject::tr("Unable generate makfile!"), QMessageBox::Ok);
        return;
    }
    QTextStream streamFileOut(&file);
    streamFileOut.setCodec("UTF-8");
    streamFileOut.setGenerateByteOrderMark(true);
    streamFileOut << make;
    streamFileOut.flush();
    file.close();
}

void Project::generateMakeFilePC()
{
    if (!m_isOpen) {
        return;
    }
    QString murlibs = "\"-L" + QDir::cleanPath(QCoreApplication::applicationDirPath() + "/devkit-x86/murlibs/") + "\"";
    QString compiler = "\"" + QDir::cleanPath(QCoreApplication::applicationDirPath() + "/devkit-x86/mingw530_32/bin/g++.exe") + "\"";
    QString inclibs = "\"-I" + QDir::cleanPath(QCoreApplication::applicationDirPath() + "/devkit-x86/murlibs/") + "\"";
    QString make = QString(
    "CXX = %3\n"
    "CXXFLAGS = -std=c++11 -Wall -O2\n\n"

    "INCLUDES = -I. %4\n"
    "LIBS = %2 -lopencv_core2410 -lopencv_highgui2410 -lopencv_imgproc2410 -lopencv_flann2410 -lopencv_features2d2410 -lopencv_objdetect2410 -lopencv_videostab2410 -lopencv_video2410 -lzmq\n\n"

    "cxxs = $(wildcard *.cpp)\n"
    "objs = $(patsubst %.cpp,%.o,$(cxxs))\n"
    "ds = $(wildcard *.d)\n\n"

    "all: %1\n\n"

    "include $(ds)\n\n"

    "%1: $(objs)\n\n"
        "\t$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o %1.exe\n\n"

    " %.o: %.cpp\n\n"
        "\t$(CXX) $(CXXFLAGS) -c $< $(INCLUDES) -o $@\n\n"

    "clean:\n"
        "\trm -r *.d\n"
        "\trm -r *.o\n"
        "\trm %1\n").arg(name()).arg(murlibs).arg(compiler).arg(inclibs);

    QFileInfo info(m_path);
    QString path = info.dir().canonicalPath();

    QFile file(path + QDir::separator() + "makefile");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, QObject::tr("Project save"), QObject::tr("Unable generate makfile!"), QMessageBox::Ok);
        return;
    }
    QTextStream streamFileOut(&file);
    streamFileOut.setCodec("UTF-8");
    streamFileOut.setGenerateByteOrderMark(true);
    streamFileOut << make;
    streamFileOut.flush();
    file.close();
}

QString Project::description()
{
    return m_project["description"].toString();
}

void Project::setTarget(Target target)
{
    m_target = target;
}

void Project::writeProject()
{
    QJsonDocument jdoc(m_project);
    QFile project(m_path);

    if (!project.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, QObject::tr("Project write"), QObject::tr("Unable to write file!") + m_path, QMessageBox::Ok);
        return;
    }
    project.write(jdoc.toJson());
}

QMap<QString, QString> Project::fetchFiles()
{
    QMap<QString, QString> files;

    QFileInfo info(m_path);
    QString path = info.dir().canonicalPath();

    //!Sources
    QVariantList sources = m_project["sources"].toArray().toVariantList();
    foreach (const QVariant &name, sources) {
        QFile file(path + QDir::separator() + name.toString());

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(nullptr, QObject::tr("Project open"), QObject::tr("Unable to open file!") + name.toString(), QMessageBox::Ok);
            return QMap<QString, QString>();
        }
        files[name.toString()] = file.readAll();
    }

    //!Headers
    QVariantList headers = m_project["headers"].toArray().toVariantList();
    foreach (const QVariant &name, headers) {
        QFile file(path + QDir::separator() + name.toString());

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(nullptr, QObject::tr("Project open"), QObject::tr("Unable to open file!") + name.toString(), QMessageBox::Ok);
            return QMap<QString, QString>();
        }
        files[name.toString()] = file.readAll();
    }
    return files;
}
