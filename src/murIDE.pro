#-------------------------------------------------
#
# Project created by QtCreator 2016-10-04T13:13:03
#
#-------------------------------------------------

QT       += core gui websockets printsupport network help
RC_FILE = $$PWD\..\misc\icon\icon.rc
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = murIDE
TEMPLATE = app

INCLUDEPATH += "$$PWD\..\misc\thirdpaty\qsci\include"
INCLUDEPATH ~= s,/,\\,g

DEPENDPATH += "$$PWD\..\misc\thirdpaty\qsci\include"
DEPENDPATH ~= s,/,\\,g

CONFIG(release, debug | release) : {
    message(Building for Windows release mode)
    LIBS += -L"$$PWD\..\misc\thirdpaty\qsci\bin\release"
    LIBS += -lqscintilla2
    LIBS ~= s,/,\\,g

    EXTRA_DLL += \
        $$PWD\..\misc\thirdpaty\qsci\bin\release\qscintilla2.dll
    EXTRA_DLL ~= s,/,\\,g

    DESTDIR = $$OUT_PWD\..\..\murIDE_Release
    DESTDIR ~= s,/,\\,g

    copydata.commands = $(COPY_FILE) $$EXTRA_DLL $$DESTDIR
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

CONFIG(debug, debug | release) : {
    message(Building for Windows debug mode)
    LIBS += -L"$$PWD\..\misc\thirdpaty\qsci\bin\debug"
    LIBS += -lqscintilla2
    LIBS ~= s,/,\\,g

    EXTRA_DLL += \
        $$PWD\..\misc\thirdpaty\qsci\bin\debug\qscintilla2.dll
    EXTRA_DLL ~= s,/,\\,g

    DESTDIR = $$OUT_PWD\..\..\murIDE_Debug
    DESTDIR ~= s,/,\\,g

    copydata.commands = $(COPY_FILE) $$EXTRA_DLL $$DESTDIR
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

CONFIG(release, debug|release) : {
    QMAKE_POST_LINK += $$[QT_INSTALL_BINS]\windeployqt $$OUT_PWD\..\..\murIDE_Release --libdir $$OUT_PWD\..\..\murIDE_Release\qtdeps --plugindir $$OUT_PWD\..\..\murIDE_Release\qtdeps \
                    && ($(CHK_DIR_EXISTS) $$OUT_PWD\..\..\murIDE_Release\help $(MKDIR) $$OUT_PWD\..\..\murIDE_Release\help) \
                    && $(COPY_DIR) $$PWD\..\misc\help $$OUT_PWD\release\help \
                    && $$[QT_INSTALL_BINS]\qcollectiongenerator $$OUT_PWD\release\help\robohelp.qhcp -o $$OUT_PWD\..\..\murIDE_Release\help\robohelp.qhc \
                    && $(COPY_FILE) $$OUT_PWD\release\help\help.qch $$OUT_PWD\..\..\murIDE_Release\help\help.qch \
                    && $(COPY_FILE) $$OUT_PWD\release\help\doc.html $$OUT_PWD\..\..\murIDE_Release\doc.html
    QMAKE_POST_LINK ~= s,/,\\,g
}
else : {
    QMAKE_POST_LINK += $$[QT_INSTALL_BINS]\windeployqt $$OUT_PWD\..\..\murIDE_Debug --libdir $$OUT_PWD\..\..\murIDE_Debug\qtdeps --plugindir $$OUT_PWD\..\..\murIDE_Debug\qtdeps \
                    && ($(CHK_DIR_EXISTS) $$OUT_PWD\..\..\murIDE_Debug\help $(MKDIR) $$OUT_PWD\..\..\murIDE_Debug\help) \
                    && $(COPY_DIR) $$PWD\..\misc\help $$OUT_PWD\debug\help \
                    && $$[QT_INSTALL_BINS]\qcollectiongenerator $$OUT_PWD\debug\help\robohelp.qhcp -o $$OUT_PWD\..\..\murIDE_Debug\help\robohelp.qhc \
                    && $(COPY_FILE) $$OUT_PWD\debug\help\help.qch $$OUT_PWD\..\..\murIDE_Debug\help\help.qch \
                    && $(COPY_FILE) $$OUT_PWD\debug\help\doc.html $$OUT_PWD\..\..\murIDE_Debug\doc.html
    QMAKE_POST_LINK ~= s,/,\\,g
}

include(src.pri)

RESOURCES += \
    ..\icons.qrc \
    ..\fonts.qrc \
    ..\api.qrc

DISTFILES +=

FORMS +=

HEADERS +=

SOURCES +=
