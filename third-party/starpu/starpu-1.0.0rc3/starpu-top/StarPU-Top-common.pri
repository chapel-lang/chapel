# -------------------------------------------------
# Project created by QtCreator 2011-01-22T11:55:16
# -------------------------------------------------
QT += network
QT += opengl
QT += sql

TARGET = StarPU-Top
TEMPLATE = app
SOURCES += $$SRCDIR/main.cpp \
#STARPU-TOP
    $$SRCDIR/mainwindow.cpp \
    $$SRCDIR/configurationmanager.cpp \
    $$SRCDIR/widgetwindowsmanager.cpp \
    $$SRCDIR/communicationthread.cpp \
    $$SRCDIR/communicationmanager.cpp \
    $$SRCDIR/preferencesdialog.cpp \
    $$SRCDIR/datawidget.cpp \
    $$SRCDIR/interactivewidget.cpp \
    $$SRCDIR/ganttwidget.cpp \
    $$SRCDIR/debugconsole.cpp \
    $$SRCDIR/dataaggregatorwidget.cpp \
    $$SRCDIR/taskmanager.cpp \
    $$SRCDIR/abstractwidgetwindow.cpp \
    $$SRCDIR/sessionsetupmanager.cpp \
#QLEDINDICATOR
    $$SRCDIR/qledindicator/qledindicator.cpp \
    $$SRCDIR/aboutdialog.cpp
HEADERS += $$SRCDIR/mainwindow.h \
#STARPU-TOP
    $$SRCDIR/starpu_top_types.h \
    $$SRCDIR/widgetwindowsmanager.h \
    $$SRCDIR/configurationmanager.h \
    $$SRCDIR/communicationthread.h \
    $$SRCDIR/communicationmanager.h \
    $$SRCDIR/preferencesdialog.h \
    $$SRCDIR/datawidget.h \
    $$SRCDIR/interactivewidget.h \
    $$SRCDIR/ganttwidget.h \
    $$SRCDIR/debugconsole.h \
    $$SRCDIR/dataaggregatorwidget.h \
    $$SRCDIR/taskmanager.h \
    $$SRCDIR/abstractwidgetwindow.h \
    $$SRCDIR/sessionsetupmanager.h \
#QLEDINDICATOR
    $$SRCDIR/qledindicator/qledindicator.h \
    $$SRCDIR/aboutdialog.h

FORMS += $$SRCDIR/mainwindow.ui \
    $$SRCDIR/preferencesdialog.ui \
    $$SRCDIR/debugconsole.ui \
    $$SRCDIR/aboutdialog.ui
RESOURCES += $$SRCDIR/resources.qrc
OTHER_FILES += $$SRCDIR/TODO.txt
