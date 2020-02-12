#-------------------------------------------------
#
# Project created by QtCreator 2015-01-29T22:00:06
#
#-------------------------------------------------

QT       += network core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rpi-multimedia-server
TEMPLATE = app

SOURCES += main.cpp \
    businesslogic.cpp \
    common/mbuffer.cpp \
    common/mcommon.cpp \
    common/mimage.cpp \
    common/mlist.cpp \
    common/mstatus.cpp \
    clientexec.cpp \
    clientsocket.cpp \
    executer.cpp \
    resource.cpp \
    server.cpp \
    wrapper/mimagewrapper.cpp \
    wrapper/mlistwrapper.cpp \
    wrapper/mabstractwrapper.cpp \
    wrapper/mstatuswrapper.cpp \
    wrapper/mcheckwrapper.cpp \
    interface/abstractbusinesslogic.cpp \
    interface/abstractexecuter.cpp \
    interface/abstractresource.cpp \
    player.cpp \
    common/mmessage.cpp \
    wrapper/mmessagewrapper.cpp
    #service.cpp \
    #qtservice/src/qtservice_unix.cpp \
    #qtservice/src/qtservice.cpp \
    #qtservice/src/qtunixserversocket.cpp \
    #qtservice/src/qtunixsocket.cpp

#include(deployment.pri)
#qtcAddDeployment()

HEADERS  += \
    businesslogic.h \
    common/mbuffer.h \
    common/mcommon.h \
    common/mimage.h \
    common/mlist.h \
    common/mstatus.h \
    common/mtypes.h \
    clientexec.h \
    clientsocket.h \
    executer.h \
    resource.h \
    server.h \
    version.h \
    wrapper/mimagewrapper.h \
    wrapper/mlistwrapper.h \
    wrapper/mabstractwrapper.h \
    wrapper/mstatuswrapper.h \
    wrapper/mcheckwrapper.h \    
    interface/abstractbusinesslogic.h \
    interface/abstractexecuter.h \
    interface/abstractresource.h \
    player.h \
    common/mmessage.h \
    wrapper/mmessagewrapper.h
    #service.h \
    #qtservice/src/qtservice_p.h \
    #qtservice/src/qtservice.h \
    #qtservice/src/QtServiceBase \
    #qtservice/src/QtServiceController \
    #qtservice/src/qtunixserversocket.h \
    #qtservice/src/qtunixsocket.h

LIBS       += -lVLCQtCore -lVLCQtWidgets

FORMS +=


