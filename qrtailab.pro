######################################################################
# Automatically generated by qmake (2.01a) Sa Jul 7 16:14:07 2007
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . /usr/local/qwt-5.0.2/lib
INCLUDEPATH += . /usr/realtime/include /usr/local/qwt-5.0.2/include/
LIBS += -L/usr/local/qwt-5.0.2/lib -lqwt
CONFIG += QwtPlot QwtDesigner QwtExamples QwtWidgets qtestlib

# Input
HEADERS += qrtailab.h meter_window.h meters_manager.h led_window.h leds_manager.h scope_window.h scopes_manager.h main_window.h parameters_manager.h
FORMS += qrl_connectdialog.ui  qrl_main_window.ui  qrl_meters_manager.ui  qrl_leds_manager.ui  qrl_scopes_manager.ui qrl_parameters_manager.ui
SOURCES += qrtailab.cpp meter_window.cpp meters_manager.cpp led_window.cpp leds_manager.cpp scope_window.cpp scopes_manager.cpp main_window.cpp parameters_manager.cpp main.cpp
RESOURCES += qrtailab.qrc
