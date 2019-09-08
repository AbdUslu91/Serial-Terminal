QT += widgets serialport
requires(qtConfig(combobox))

TARGET = terminal
TEMPLATE = app

DESTDIR += $$OUT_PWD/build
OBJECTS_DIR += $$OUT_PWD/build
MOC_DIR += $$OUT_PWD/build

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    console.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    terminal.qrc

#target.path = $$[QT_INSTALL_EXAMPLES]/serialport/terminal
INSTALLS += target
