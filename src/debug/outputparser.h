#ifndef OUTPUTPARSER_H
#define OUTPUTPARSER_H

#include <QString>
#include <QObject>
#include <QRegularExpression>
#include <QRegExp>

class OutputParser
{
public:
    enum Type {
        NONE,
        ERROR,
        WARNING,
        UNKNOWN
    };
    struct CompilationInfo {
        int infoType = Type::NONE;
        int line = 0;
        QString fileName = 0;
        QString description = 0;
    };
    static CompilationInfo parseOutput(QString out);

private:
    static QRegularExpression m_regExp;
    static QRegularExpression m_regExpIncluded;
    static QRegularExpression m_regExpGccNames;

    static QString rightTrimmed(const QString &in);
};

#endif // OUTPUTPARSER_H
