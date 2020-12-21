TEMPLATE = aux

#This path must be added for outer of source code heavy crosscompiler
#It seems like C:\path\to\crosscompiler\devkit-x86.7z
#The content of this devkit-x86.7z a is a directory devkit-x86 with crosscompiller itself.
CROSSCOMPILER = $$CROSSCOMPILER

PWD ~= s,/,\\,g
OUT_PWD ~= s,/,\\,g

CONFIG(release, debug|release) {
    #first, copy qt installer base source tree to build destination
    BUILD_DESTDIR_WIN = $$OUT_PWD\release\build_data
    BUILD_DESTDIR_WIN ~= s,/,\\,g
    sourcecopydata.commands = ($(CHK_DIR_EXISTS) $$BUILD_DESTDIR_WIN $(MKDIR) $$BUILD_DESTDIR_WIN) \
                           && $(COPY_DIR) $$PWD\config $$BUILD_DESTDIR_WIN\config \
                           && $(COPY_DIR) $$PWD\packages $$BUILD_DESTDIR_WIN\packages
    sourcecopydata.commands ~= s,/,\\,g

    #complete installer path with installer name
    INSTALLER_OFFLINE = $$OUT_PWD/../../InstallerRelease/murInstallerIDE.offline
    INSTALLER_OFFLINE ~= s,/,\\,g
    INSTALLER_ONLINE = $$OUT_PWD/../../InstallerRelease/murInstallerIDE.online
    INSTALLER_ONLINE ~= s,/,\\,g

    #project build path
    SOURCEDIR_WIN = $$OUT_PWD\..\..\murIDE_Release
    SOURCEDIR_WIN ~= s,/,\\,g

    #IDE component
    IDE_DESTDIR_WIN = $$BUILD_DESTDIR_WIN\packages\IDE\data
    IDE_DESTDIR_WIN ~= s,/,\\,g
    binarycopydata.commands += ($(CHK_DIR_EXISTS) $$IDE_DESTDIR_WIN $(MKDIR) $$IDE_DESTDIR_WIN) \
                            && $(COPY_FILE) $$SOURCEDIR_WIN\murIDE.exe $$IDE_DESTDIR_WIN\murIDE.exe
    binarycopydata.depends = sourcecopydata
    #Installer custom part translation
    translateinstallerinterface.commands = lrelease $$IDE_DESTDIR_WIN\..\meta\ru.ts -qm $$IDE_DESTDIR_WIN\..\meta\ru.qm #create of translation file must be runned before first target
    translateinstallerinterface.depends = sourcecopydata

    #IDE translations subcomponent
    IDETS_DESTDIR_WIN = $$BUILD_DESTDIR_WIN\packages\IDE.translations\data
    IDETS_DESTDIR_WIN ~= s,/,\\,g
    translationscopydata.commands += ($(CHK_DIR_EXISTS) $$IDETS_DESTDIR_WIN\translations $(MKDIR) $$IDETS_DESTDIR_WIN\translations) \
                                  && $(COPY_DIR) $$SOURCEDIR_WIN\translations $$IDETS_DESTDIR_WIN\translations
    translationscopydata.depends = sourcecopydata

    #IDE help subcomponent
    IDEHELP_DESTDIR_WIN = $$BUILD_DESTDIR_WIN\packages\IDE.help\data
    IDEHELP_DESTDIR_WIN ~= s,/,\\,g
    helpcopydata.commands += ($(CHK_DIR_EXISTS) $$IDEHELP_DESTDIR_WIN\help $(MKDIR) $$IDEHELP_DESTDIR_WIN\help) \
                          && $(COPY_DIR) $$SOURCEDIR_WIN\help $$IDEHELP_DESTDIR_WIN\help \
                          && $(COPY_FILE) $$SOURCEDIR_WIN\doc.html $$IDEHELP_DESTDIR_WIN\doc.html

    #IDE examples subcomponent
    IDEXMPL_DESTDIR_WIN = $$BUILD_DESTDIR_WIN\packages\IDE.examples\data
    IDEXMPL_DESTDIR_WIN ~= s,/,\\,g
    examplescopydata.commands += ($(CHK_DIR_EXISTS) $$IDEXMPL_DESTDIR_WIN\examples $(MKDIR) $$IDEXMPL_DESTDIR_WIN\examples) \
                              && $(COPY_DIR) $$PWD\..\misc\examples $$IDEXMPL_DESTDIR_WIN\examples
    examplescopydata.depends = sourcecopydata

    #QScintilla component
    QSCI_DESTDIR_WIN = $$BUILD_DESTDIR_WIN\packages\QScintilla\data
    QSCI_DESTDIR_WIN ~= s,/,\\,g
    qscicopydata.commands += ($(CHK_DIR_EXISTS) $$QSCI_DESTDIR_WIN $(MKDIR) $$QSCI_DESTDIR_WIN) \
                          && $(COPY_FILE) $$SOURCEDIR_WIN\qscintilla2.dll $$QSCI_DESTDIR_WIN\qscintilla2.dll
    qscicopydata.depends = sourcecopydata

    #QtDeps component
    QTDEPS_DESTDIR_WIN = $$BUILD_DESTDIR_WIN\packages\QtDeps\data
    QTDEPS_DESTDIR_WIN ~= s,/,\\,g
    qtdepscopydata.commands += ($(CHK_DIR_EXISTS) $$QTDEPS_DESTDIR_WIN $(MKDIR) $$QTDEPS_DESTDIR_WIN) \
                            && $(COPY_DIR) $$SOURCEDIR_WIN\qtdeps $$QTDEPS_DESTDIR_WIN
    qtdepscopydata.depends = sourcecopydata

    #Crosscompiler component
    CROSSCOMPILER_DESTDIR_WIN = $$BUILD_DESTDIR_WIN\packages\Crosscompiler\data
    CROSSCOMPILER_DESTDIR_WIN ~= s,/,\\,g
    crosscompilerdata.commands += ($(CHK_DIR_EXISTS) $$CROSSCOMPILER_DESTDIR_WIN $(MKDIR) $$CROSSCOMPILER_DESTDIR_WIN) \
                               && $(COPY_FILE) $$CROSSCOMPILER $$CROSSCOMPILER_DESTDIR_WIN\devkit-x86.7z
    crosscompilerdata.depends = sourcecopydata

    #API component
    #devkit-x86\sysroots\i586-poky-linux\usr\include\murAPI.hpp
    #No need to copy. The file is already there.

    #VCppRedist component
    VCPPREDIST_DESTDIR_WIN = $$BUILD_DESTDIR_WIN\packages\VCppRedist\data
    VCPPREDIST_DESTDIR_WIN ~= s,/,\\,g
    vcppredist.commands += ($(CHK_DIR_EXISTS) $$VCPPREDIST_DESTDIR_WIN $(MKDIR) $$VCPPREDIST_DESTDIR_WIN) \
                        && $(COPY_FILE) $$PWD\..\misc\thirdpaty\vcpp\vcredist_x86.exe $$VCPPREDIST_DESTDIR_WIN\vcredist_x86.exe
    vcppredist.depends = sourcecopydata

    #create offline installer
    offlineInstaller.output = $$INSTALLER_OFFLINE
    offlineInstaller.commands += ($(CHK_DIR_EXISTS) $$OUT_PWD\..\..\InstallerRelease $(MKDIR) $$OUT_PWD\..\..\InstallerRelease) \
                              && binarycreator --offline-only -c $$BUILD_DESTDIR_WIN\config\config.xml -p $$BUILD_DESTDIR_WIN\packages $$INSTALLER_OFFLINE
    offlineInstaller.commands ~= s,/,\\,g
    offlineInstaller.depends = binarycopydata translateinstallerinterface translationscopydata helpcopydata examplescopydata qscicopydata qtdepscopydata crosscompilerdata vcppredist
    offlineInstaller.CONFIG += target_predeps no_link combine

    #create online installer
    onlineInstaller.output = $$INSTALLER_OFFLINE
    onlineInstaller.commands += ($(CHK_DIR_EXISTS) $$OUT_PWD\..\..\InstallerRelease $(MKDIR) $$OUT_PWD\..\..\InstallerRelease) \
                             && binarycreator --online-only -c $$BUILD_DESTDIR_WIN\config\config.xml -p $$BUILD_DESTDIR_WIN\packages $$INSTALLER_ONLINE
    onlineInstaller.commands ~= s,/,\\,g
    onlineInstaller.depends = binarycopydata translateinstallerinterface translationscopydata helpcopydata examplescopydata qscicopydata qtdepscopydata crosscompilerdata vcppredist
    onlineInstaller.CONFIG += target_predeps no_link combine

    #create repository
    repository.commands += repogen -p $$BUILD_DESTDIR_WIN\packages -i IDE,IDE.examples,IDE.help,IDE.translations,QScintilla,QtDeps,Crosscompiler,API --update $$OUT_PWD\..\..\repository
    repository.depends = binarycopydata translateinstallerinterface translationscopydata helpcopydata examplescopydata qscicopydata qtdepscopydata crosscompilerdata vcppredist

    first.depends = $(first) offlineInstaller onlineInstaller repository
    export(first.depends)
    export(offlineInstaller.depends)
    export(onlineInstaller.depends)
    export(repository.depends)
    export(sourcecopydata.commands)
    export(binarycopydata.commands)
    export(translateinstallerinterface.commands)
    export(translationscopydata.commands)
    export(helpcopydata.commands)
    export(examplescopydata.commands)
    export(qscicopydata.commands)
    export(qtdepscopydata.commands)
    export(crosscompilerdata.commands)
    export(vcppredist.commands)

    QMAKE_EXTRA_TARGETS += first offlineInstaller onlineInstaller repository sourcecopydata binarycopydata translateinstallerinterface translationscopydata helpcopydata examplescopydata qscicopydata qtdepscopydata crosscompilerdata vcppredist

}

DISTFILES += \
    config\config.xml \
    packages\IDE\meta\package.xml \
    packages\IDE\meta\installscript.qs \
    packages\IDE\meta\license.txt \
    packages\IDE\meta\ru.ts \
    packages\QScintilla\meta\package.xml \
    packages\QScintilla\meta\license.txt \
    packages\QtDeps\meta\package.xml \
    packages\QtDeps\meta\license.txt \
    packages\Crosscompiler\meta\package.xml \
    packages\Crosscompiler\meta\license.txt \
    packages\API\meta\package.xml \
    packages\API\meta\license.txt \
    packages\VCppRedist\meta\package.xml \
    packages\VCppRedist\meta\installscript.qs \
    packages\IDE.examples\meta\package.xml \
    packages\IDE.help\meta\package.xml \
    packages\IDE.translations\meta\package.xml

FORMS += \
    packages\IDE\meta\installshortcutsform.ui

TRANSLATIONS += packages\IDE\meta\ru.ts

HEADERS += \
    packages\API\data\devkit-x86\sysroots\i586-poky-linux\usr\include\murAPI.hpp \
    packages\API\data\devkit-x86\murlibs\murAPI.hpp
