#include "outputparser.h"
#include <QRegularExpressionMatch>

QRegularExpression OutputParser::m_regExp("^(<command[ -]line>|([A-Za-z]:)?[^:]+):(\\d+):(\\d+:)?\\s+((fatal |#)?(warning|error|note):?\\s)?([^\\s].+)$",
                                          QRegularExpression::MultilineOption);
QRegularExpression OutputParser::m_regExpGccNames("^(.*?[\\\\/])?([a-z0-9]+-[a-z0-9]+-[a-z0-9]+-)?(gcc|g\\+\\+)(-[0-9\\.]+)?(\\.exe)?: ",
                                                  QRegularExpression::MultilineOption);
QRegularExpression OutputParser::m_regExpIncluded("\\bfrom\\s(<command[ -]line>|([A-Za-z]:)?[^:]+):(\\d+)(:\\d+)?[,:]?$",
                                                  QRegularExpression::MultilineOption);

OutputParser::CompilationInfo OutputParser::parseOutput(QString out)
{
    QString lne = rightTrimmed(out);

    CompilationInfo result;

    // Handle misc issues:
    if (lne.startsWith(QLatin1String("ERROR:")) ||
        lne == QLatin1String("* cpp failed")) {
        result.infoType = Type::ERROR;
        result.line = -1;
        result.fileName = "";
        result.description = lne;

        return result;
    }

    QRegularExpressionMatch match = m_regExpGccNames.match(lne);
    if (match.hasMatch()) {
        QString description = lne.mid(match.capturedLength());
        Type type = Type::ERROR;
        if (description.startsWith(QLatin1String("warning: "))) {
            type = Type::WARNING;
            description = description.mid(9);
        } else if (description.startsWith(QLatin1String("fatal: ")))  {
            description = description.mid(7);
        }

        result.infoType = type;
        result.line = -1;
        result.fileName = "";
        result.description = description;

        return result;
    }

    match = m_regExp.match(lne);
    if (match.hasMatch()) {
        QString filename = match.captured(1);
        int lineno = match.captured(3).toInt();
        Type type = Type::UNKNOWN;
        QString description = match.captured(8);
        if (match.captured(7) == QLatin1String("warning"))
            type = Type::WARNING;
        else if (match.captured(7) == QLatin1String("error") ||
                 description.startsWith(QLatin1String("undefined reference to")) ||
                 description.startsWith(QLatin1String("multiple definition of")))
            type = Type::ERROR;
        // Prepend "#warning" or "#error" if that triggered the match on (warning|error)
        // We want those to show how the warning was triggered
        if (match.captured(5).startsWith(QLatin1Char('#')))
            description = match.captured(5) + description;

        result.infoType = type;
        result.line = lineno;
        result.fileName = filename;
        result.description = description;

        return result;
    }

    match = m_regExpIncluded.match(lne);
    if (match.hasMatch()) {
        result.infoType = Type::UNKNOWN;
        result.line = match.captured(3).toInt();
        result.fileName = match.captured(1);
        result.description = lne.trimmed();

        return result;
    }

    result.description = lne;

    return result;
}

QString OutputParser::rightTrimmed(const QString &in)
{
    int pos = in.length();
    for (; pos > 0; --pos) {
        if (!in.at(pos - 1).isSpace())
            break;
    }
    return in.mid(0, pos);
}
