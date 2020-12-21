#include <QAction>

#include "murgloabal.h"
#include "texteditwidget.h"
#include "dispatcher.h"
#include <QKeyEvent>
#include <QDebug>
#include <QFontDatabase>
#include <Qsci/qsciapis.h>

#define base03 QColor(0, 43, 54)
#define base02 QColor(7, 54, 66)
#define base01 QColor(88, 110, 117)
#define base00 QColor(101, 123, 131)
#define base0 QColor(131, 148, 150)
#define base1 QColor(147, 161, 161)
#define base2 QColor(238, 232, 213)
#define base3 QColor(253, 246, 227)
#define yellow QColor(181, 137, 0)
#define orange QColor(203,  75,  22)
#define red QColor(220,  50,  47)
#define magenta QColor(211,  54, 130)
#define violet QColor(108, 113, 196)
#define blue QColor(38, 139, 210)
#define cyan QColor(42, 161, 152)
#define green QColor(133, 153, 0)



TextEditWidget::TextEditWidget(QWidget *parent) : QsciScintilla(parent)
{
    setupLexer();
    setupEditor();
    createConnections();
    QPixmap pix(":/misc/images/error_point.png");
    markerDefine(pix.scaled(QSize(12, 12)), 0);
    setMarginMarkerMask(25, 0);
    QFontDatabase::addApplicationFont(":/misc/fonts/Hack-Bold.ttf");
    QFontDatabase::addApplicationFont(":/misc/fonts/Hack-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/misc/fonts/Hack-Italic.ttf");
    QFontDatabase::addApplicationFont(":/misc/fonts/Hack-Regular.ttf");
}

void TextEditWidget::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
        case (Qt::Key_Slash):
            if (e->modifiers() == Qt::ControlModifier) {
               addOrRemoveComments();
            }
        break;
    case (Qt::Key_F1):
        triggerHelp();
        break;
    default:
        break;
    }

    if((e->modifiers() == Qt::CTRL) && (e->key() == Qt::Key_Space)) {
        autoCompleteFromAll();
        return;
    }

    QsciScintilla::keyPressEvent(e);
}

void TextEditWidget::mouseMoveEvent(QMouseEvent *e)
{
    QsciScintilla::mouseMoveEvent(e);
}

void TextEditWidget::wheelEvent(QWheelEvent *e)
{
    int numDegrees = e->delta() / 8;
    if (e->modifiers() == Qt::ControlModifier) {
        if (numDegrees > 0) {
            zoomIn();
        } else {
            zoomOut();
        }
    }


    QsciScintilla::wheelEvent(e);
}

void TextEditWidget::addOrRemoveComments()
{
    QString str = selectedText();

    bool isCommentedBegin = false;
    if (str.size() < 2) {
        return;
    }

    int posBegin = 0;
    for (auto i = 0; i < str.size() - 2; ++i) {
        if (str.at(i) == ' ') {
            continue;
        }

        if (str.at(i++) == '/' && str.at(i) == '*') {
            isCommentedBegin = true;
            posBegin = --i;
            break;
        }
        break;
    }

    bool isCommentedEnd = false;
    int posEnd = 0;
    for (auto i = str.size() - 1; i != 1; i--) {
        if (str.at(i) == '\x0') {
            continue;
        }
        qDebug() << str.at(i);
        if (str.at(i--) == '/' && str.at(i) == '*') {
            isCommentedEnd = true;
            posEnd = i;
            break;
        }
        break;
    }

    if (isCommentedEnd && isCommentedBegin) {
        str.remove(posBegin, 2);
        str.remove(posEnd - 2, 2);
        replaceSelectedText(str);
    }
    else {
        str.append("*/");
        str.prepend("/*");
        replaceSelectedText(str);
    }
}

void TextEditWidget::triggerHelp()
{
    int line;
    int index;
    getCursorPosition(&line, &index);
    QString word = wordAtLineIndex(line, index);

    if (!word.isEmpty()) {
        QAction *act = DISPATCHER->action("Action.Source.Find");
        act->setData(word);
        act->trigger();
    }
}

void TextEditWidget::createConnections()
{
    connect(DISPATCHER->action("Action.Edit.Redo"), &QAction::triggered, this, &QsciScintilla::redo);
    connect(DISPATCHER->action("Action.Edit.Undo"), &QAction::triggered, this, &QsciScintilla::undo);
    connect(DISPATCHER->action("Action.Edit.Copy"), &QAction::triggered, this, &QsciScintilla::copy);
    connect(DISPATCHER->action("Action.Edit.Paste"), &QAction::triggered, this, &QsciScintilla::paste);
    connect(DISPATCHER->action("Action.Edit.Cut"), &QAction::triggered, this, &QsciScintilla::cut);
    connect(DISPATCHER->action("Action.Edit.SelectAll"), &QAction::triggered, this, &QsciScintilla::selectAll);
}

void TextEditWidget::setupLexer()
{
    m_lexCpp = new QsciLexerCPP(this);
    setLexer(m_lexCpp);

    //! Setup encoding and font
    QFont font = QFont("Hack");
    font.setPointSize(10);
    m_lexCpp->setFont(font);

    m_lexCpp->setPaper(QColor(37, 37, 37));

    m_lexCpp->setColor("#FFFFFF", m_lexCpp->Default);
    m_lexCpp->setColor("#E7DB75", m_lexCpp->PreProcessor);
    m_lexCpp->setColor("#E7DB75", m_lexCpp->PreProcessorComment);

    m_lexCpp->setColor("#76715E", m_lexCpp->Comment);
    m_lexCpp->setColor("#76715E", m_lexCpp->CommentLine);
    m_lexCpp->setColor("#76715E", m_lexCpp->CommentDoc);
    m_lexCpp->setColor("#76715E", m_lexCpp->CommentDocKeyword);

    m_lexCpp->setColor("#66D9EE", m_lexCpp->Keyword);
    m_lexCpp->setColor("#AE82FF", m_lexCpp->KeywordSet2);

    m_lexCpp->setColor("#FE9720", m_lexCpp->SingleQuotedString);
    m_lexCpp->setColor("#FE9720", m_lexCpp->DoubleQuotedString);
    m_lexCpp->setColor("#FE9720", m_lexCpp->UnclosedString);

    m_lexCpp->setColor("#AE82FF", m_lexCpp->Number);
    m_lexCpp->setColor("#FFFFFF", m_lexCpp->Operator);

    m_lexCpp->setColor("#FFFFFF", m_lexCpp->Identifier);
    m_lexCpp->setColor("#36AF90", m_lexCpp->UserLiteral);
    m_lexCpp->setColor("#36AF90", m_lexCpp->UUID);

    /*Light
    m_lexCpp->setPaper(base3);

    m_lexCpp->setColor(base02, m_lexCpp->Default);
    m_lexCpp->setColor(red, m_lexCpp->PreProcessor);
    m_lexCpp->setColor(red, m_lexCpp->PreProcessorComment);

    m_lexCpp->setColor(base1, m_lexCpp->Comment);
    m_lexCpp->setColor(base1, m_lexCpp->CommentLine);
    m_lexCpp->setColor(base1, m_lexCpp->CommentDoc);
    m_lexCpp->setColor(base1, m_lexCpp->CommentDocKeyword);

    m_lexCpp->setColor(magenta, m_lexCpp->Keyword);
    m_lexCpp->setColor(orange, m_lexCpp->KeywordSet2);

    m_lexCpp->setColor(green, m_lexCpp->SingleQuotedString);
    m_lexCpp->setColor(green, m_lexCpp->DoubleQuotedString);
    m_lexCpp->setColor(green, m_lexCpp->UnclosedString);

    m_lexCpp->setColor(violet, m_lexCpp->Number);
    m_lexCpp->setColor(base02, m_lexCpp->Operator);

    m_lexCpp->setColor(yellow, m_lexCpp->Identifier);
    m_lexCpp->setColor(green, m_lexCpp->UserLiteral);
    m_lexCpp->setColor(green, m_lexCpp->UUID);
    */

}

void TextEditWidget::setupEditor()
{
    SendScintilla(SCI_SETHSCROLLBAR, 0);
    setUtf8(true);
    setEolMode(EolUnix);
    setAutoCompletionReplaceWord(true);
    setAutoCompletionCaseSensitivity(true);
    setAutoCompletionThreshold(1);
    setAutoCompletionSource(QsciScintilla::AcsAll);

    setCallTipsPosition(CallTipsAboveText);
    setCallTipsStyle(CallTipsContext);

    setCallTipsBackgroundColor("#D0D0D0");
    setCallTipsForegroundColor("#252525");

    //! Setup highlighting on current line
    setCaretLineVisible(true);
    /*Light
     setCaretLineBackgroundColor(base2);
    setCaretForegroundColor(base1);
    setMarginsForegroundColor(base00);
    */

    setCaretLineBackgroundColor(QColor("#323232"));
    setCaretForegroundColor(QColor("#D0D0D0"));
    setMarginsForegroundColor(QColor("#777777"));

    //! Setup right margin
    /* Light
    setMarginsBackgroundColor(QColor(base2));
    */
    setMarginsBackgroundColor(QColor("gainsboro"));

    setMarginLineNumbers(1, true);
    setMarginWidth(1, 45);

    //! Setup indent for 4 spaces
    setAutoIndent(true);
    setIndentationGuides(false);
    setIndentationsUseTabs(false);
    setIndentationWidth(4);

    //! Brace matching highlighting
    setBraceMatching(QsciScintilla::SloppyBraceMatch);


    setMatchedBraceBackgroundColor("#76715E");
    setMatchedBraceForegroundColor("#FFFFFF");

    setUnmatchedBraceForegroundColor("#FFFFFF");
    setUnmatchedBraceBackgroundColor("#DA2772");

    /*Light
    setMatchedBraceBackgroundColor(blue);
    setMatchedBraceForegroundColor(base03);

    setUnmatchedBraceForegroundColor(base03);
    setUnmatchedBraceBackgroundColor(red);
    */

    //!Styles

    setMarginsBackgroundColor(QColor(37, 37, 37));

    /*
    setStyleSheet("QWidget {"
                     "border-top: none;"
                     "border-left: none;"
                     "border-top: none;"
                     "border-right: none;"
                 "}");
    */

    QsciAPIs *api = new QsciAPIs(m_lexCpp);
    api->load(":/misc/api/cpp.api");
    api->load(":/misc/api/mur.api");
    api->prepare();
}
