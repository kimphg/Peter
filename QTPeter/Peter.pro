#-------------------------------------------------
#
# Project created by QtCreator 2015-06-20T15:53:12
# Author Phung Kim Phuong pawnc7@gmail.com
#-------------------------------------------------
#Features:
#Control and process data from HR2D radar
#view ARPA data
#-------------------------------------------------
# Version 4.0.1
#-------------------------------------------------
#data file type defined as .r2d, data starts from 22nd byte of each frame

@CONFIG  += debug_and_release@
QT       += core gui
QT       += network
QT       += serialport
QT	    += positioning
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter
TARGET = Peter
TEMPLATE = app
SOURCES += main.cpp\
    mainwindow.cpp \
    c_arpa_data.cpp \
    qcustombutton.cpp \
    qcustomframe.cpp \
    qcustomcombobox.cpp \
    qcustomgroupbox.cpp \
    qcustomtabwidget.cpp \
    ctarget.cpp \
    jtarget.cpp \
    jgraphictarget.cpp \
    Cmap/cmap.cpp\
    c_config.cpp \
    c_radar_data.cpp \
    c_radar_thread.cpp \
    dialogdocumentation.cpp \
    AIS/AIS.cpp \
    dialogaisinfo.cpp \
    dialogcommandlog.cpp \
    statuswindow.cpp


HEADERS  += mainwindow.h \
    c_arpa_data.h \
    qcustombutton.h \
    qcustomframe.h \
    qcustomcombobox.h \
    qcustomgroupbox.h \
    qcustomtabwidget.h \
    pkp.h \
    ctarget.h \
    jtarget.h \
    jgraphictarget.h \
    Cmap/cmap.h\
    c_config.h \
    c_radar_data.h \
    c_radar_thread.h \
    dialogdocumentation.h \
    AIS/AIS.h \
    dialogaisinfo.h \
    dialogcommandlog.h \
    statuswindow.h

FORMS    += mainwindow.ui \
    dialogdocumentation.ui \
    dialogaisinfo.ui \
    dialogcommandlog.ui \
    statuswindow.ui
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/shapelib/ -lshapelib
#else:unix: LIBS += -L$$PWD\shapelib\ -lshapelib
#INCLUDEPATH += $$PWD/shapelib
#DEPENDPATH += $$PWD/shapelib

INCLUDEPATH += $$PWD/WpdPack/Include
DEPENDPATH += $$PWD/WpdPack/Include
INCLUDEPATH += $$PWD/tinyxml/
DEPENDPATH += $$PWD/tinyxml/
INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
#INCLUDEPATH += $$PWD/mitab/mitab/
#INCLUDEPATH += $$PWD/mitab/cpl/
#INCLUDEPATH += $$PWD/mitab/ogr/
#RESOURCES +=
#INCLUDEPATH += $$PWD/../armadillo/include
#DEPENDPATH += $$PWD/../armadillo/include
#win32:LIBS += -L$$PWD/../armadilloWin32/lib_winx86/ -lblas_win32_MT
#win32:LIBS += -L$$PWD/../armadilloWin32/lib_winx86/ -llapack_win32_MT
#win32:LIBS += -L$$PWD/../armadillo/lib_winx86/ -lcbia.lib.blas.dyn.rel.x86.12
#win32:LIBS += -L$$PWD/../armadillo/lib_winx86/ -lcbia.lib.lapack.dyn.rel.x86.12

#unix: LIBS += -larmadillo
win32:LIBS += -L$$PWD//WpdPack/Lib/ -lPacket
win32:LIBS += -L$$PWD//WpdPack/Lib/ -lwpcap
#win32:LIBS += -L$$PWD/mitab/ -lmitab
#LIBS += -ltinyxml
#INCLUDEPATH += $$PWD/../opencv/build/include
#win32:LIBS += -L$$PWD/../opencv/build/x86/vc10/lib/ -lopencv_core249
#win32:LIBS += -L$$PWD/../opencv/build/x86/vc10/lib/ -lopencv_highgui249
#win32:LIBS += -L$$PWD/../opencv/build/x86/vc10/lib/ -lopencv_imgproc249
#win32:LIBS += -L$$PWD/../opencv/build/x86/vc10/lib/ -lopencv_videoio249

#LIBS +=

#win32:CONFIG(release, debug|release):INCLUDEPATH += $$PWD/armadillo/lib_winx86
#win32:CONFIG(release, debug|release):DEPENDPATH += $$PWD/armadillo/lib_winx86

#unix:QMAKE_CXXFLAGS += -O2
#unix:QMAKE_CXXFLAGS += -l -DARMA_DONT_USE_WRAPPER -lblas -llapack
DISTFILES += \
    appIcon.rc
win32:RC_FILE += appIcon.rc
