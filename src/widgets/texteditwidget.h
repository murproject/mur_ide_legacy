#ifndef TEXTEDITWIDGET_H
#define TEXTEDITWIDGET_H

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>

class TextEditWidget : public QsciScintilla
{
    Q_OBJECT
public:
    explicit TextEditWidget(QWidget *parent = 0);

signals:
private slots:
    void keyPressEvent(QKeyEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
public slots:
private:
    void addOrRemoveComments();
    void triggerHelp();
    void createConnections();
    void setupLexer();
    void setupEditor();
    bool m_isExist;
    QsciLexerCPP* m_lexCpp;

};

#endif // TEXTEDITWIDGET_H
