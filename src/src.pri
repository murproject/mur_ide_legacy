INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

message($$PWD)

include(widgets/widgets.pri)
include(global/global.pri)
include(connectivity/connectivity.pri)
include(project/project.pri)
include(debug/debug.pri)

SOURCES += \
    $$PWD/main.cpp
