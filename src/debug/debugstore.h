#ifndef DEBUGSTORE_H
#define DEBUGSTORE_H

#include <QObject>
#include "consolewidget.h"
#include "programoutputview.h"
#include "cameraswidget.h"

class DebugStore : public QObject
{
    Q_OBJECT
public:
    DebugStore(QObject* parent = nullptr);
    QWidget* console();
    QWidget* cameras();
    ~DebugStore();
private:
    ConsoleWidget* m_console;
    ProgramOutputView* m_stdView;
    CamerasWidget* m_cameras;

    void createActions();
    void createReactions();
};

#endif // DEBUGSTORE_H
