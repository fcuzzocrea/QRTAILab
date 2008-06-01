######################################################################
# Automatically generated by qmake (2.01a) Sa Jul 7 16:14:07 2007
######################################################################

include( ../qrtailab.config )

TEMPLATE = app
TARGET = qrtailab
DESTDIR = ../
MOC_DIR           = moc
OBJECTS_DIR       = obj
UI_DIR		  = ui
CONFIG += QwtPlot   QwtWidgets qtestlib

# Input
HEADERS += qrtailab.h meter_window.h meters_manager.h led_window.h leds_manager.h scope_window.h 
HEADERS += scopes_manager.h main_window.h parameters_manager.h  qrtailab_core.h
HEADERS += qrtailab_listener.h targets_manager.h
FORMS += qrl_connectdialog.ui  qrl_main_window.ui  qrl_meters_manager.ui  qrl_leds_manager.ui  
FORMS += qrl_scopes_manager.ui qrl_parameters_manager.ui qrl_targets_manager.ui
SOURCES +=  meter_window.cpp meters_manager.cpp led_window.cpp leds_manager.cpp scope_window.cpp
SOURCES += scopes_manager.cpp main_window.cpp parameters_manager.cpp main.cpp
SOURCES +=   qrtailab_core.cpp qrtailab_listener.cpp targets_manager.cpp
RESOURCES += qrtailab.qrc
