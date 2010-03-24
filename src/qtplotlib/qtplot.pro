# -------------------------------------------------
# Project created by QtCreator 2010-03-22T13:38:03
# -------------------------------------------------
TARGET = qtplot
TEMPLATE = lib
DEFINES += QTPLOT_LIBRARY
INCLUDEPATH += . \
    /usr/include/qwt/
DESTDIR                 = /usr/local/lib/qtplot
LIBS += -lqwt
SOURCES += qtplot.cpp \
    scope_trace.cpp \
    scope_data.cpp
HEADERS += qtplot.h \
    qtplot_global.h \
    scope_data.h \
    scope_trace.h
