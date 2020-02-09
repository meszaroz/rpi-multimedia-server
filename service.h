#ifndef SERVICE_H
#define SERVICE_H

#include <QCoreApplication>
#include <iostream>
#include <getopt.h>
#include <string.h>
#include "server.h"
#include "clientexec.h"
#include "executer.h"
#include "resource.h"
#include "qtservice/src/qtservice.h"

#include "../../Xcode/RPIMultimediaController/RPIMultimediaController/Common/mstatus.h"
#include "../../Xcode/RPIMultimediaController/RPIMultimediaController/Common/mcommand.h"
#include "../../Xcode/RPIMultimediaController/RPIMultimediaController/Common/mlist.h"
#include "../../Xcode/RPIMultimediaController/RPIMultimediaController/Common/mimage.h"

class Application : public QCoreApplication
{
public:
    Application(int& argc, char **argv) :
        QCoreApplication(argc,argv), mArgc(argc), mArgv(argv)
    {}

    int argc() {return mArgc;}
    char** argv() {return mArgv;}

private:
    int& mArgc;
    char** mArgv;
};

class Service : public QtService<Application>
{
public:
    enum State
    {
        Help,
        Vers,
        Start,
        Stop,        
        Undefined
    };

    explicit Service(int argc, char **argv);

protected:
    void start();
    void stop();

private:
    void registerTypes();
    void loadParam();
    State state();
    int help();
    int vers();
    int update();
    int terminate();
    int execute();
    int undefined();

    QSettings *mSettings;
    Server *mDaemon;
    QMap<QString,QVariant> mParamList;
};

#endif // SERVICE_H
