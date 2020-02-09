#include "service.h"

extern const std::string version;

Service::Service(int argc, char **argv) :
    QtService<Application>(argc, argv, "Multimedia Controller"),
    mSettings(NULL),
    mDaemon(NULL)
{
    registerTypes();
    setServiceDescription("Multimedia Controller");
    setServiceFlags(NeedsStopOnShutdown);
}

Service::State Service::state()
{
    Service::State out = Service::Undefined;
    /**/ if (mParamList.value("isHelp" ).toBool())
        out = Service::Help;
    else if (mParamList.value("isVers" ).toBool())
        out = Service::Vers;
    else if (mParamList.value("isStart").toBool()
          && mParamList.value("port"   ).toInt() != 0)
        out = Service::Start;
    else if (mParamList.value("isStop" ).toBool()
          && mParamList.value("port"   ).toInt() != 0)
        out = Service::Stop;
    return out;
}

int Service::help()
{
    std::cout << version << std::endl;
    std::cout << std::endl;
    std::cout << " Syntax: evalnetd [options]"                                                  << std::endl;
    std::cout << std::endl;
    std::cout << " opions:"                                                                     << std::endl;
    std::cout << "     --start              start server"                                       << std::endl;
    std::cout << "     --stop               stop server"                                        << std::endl;
    std::cout << std::endl;
    std::cout << " -s, --server             server neme (OPTIONAL for --stop)"                  << std::endl;
    std::cout << " -p, --port               server port (REQUIRED)"                             << std::endl;
    std::cout << std::endl;
    std::cout << " -a, --path               multimedia content path list"                       << std::endl;
    std::cout << std::endl;
    std::cout << " -h, --help               list of command line parameters"                    << std::endl;
    std::cout << " -v, --version            display version information"                        << std::endl;
    std::cout << std::endl;
    std::cout << "NOTE: - if no parameter is set, default configuration is loaded"              << std::endl;
    std::cout << "      - --path is a comma or semicolon separated list usable multiple times"  << std::endl;
    return 0;
}

int Service::vers()
{
    std::cout << version << std::endl;
    return 0;
}

int Service::terminate()
{
    return remoteStop(mParamList.value("server").toString(),
                      mParamList.value("port"  ).toInt());
}

int Service::execute()
{
    // get application
    QCoreApplication *app = application();

    // get variables
    int         port = mParamList.value("port").toInt();
    QStringList path = mParamList.value("path").toStringList();

    // store settings -> only if not custom config
    if (mSettings != NULL)
    {
        mSettings->setValue("port",port);
        mSettings->setValue("path",path);
    }

    // create object
    mDaemon = new Server(new Executer,new ResourceContainer(path));
    mDaemon->listen(QHostAddress::Any,port);

    // check if created and listening
    if (!mDaemon || !mDaemon->isListening())
        app->exit(2);

    // return
    return 0;
}

int Service::undefined()
{
    std::cout << "Command line parameter error" << std::endl;
    std::cout << "For help type: \"mc -h\"" << std::endl;
    return 1;
}

void Service::start()
{
    // get application and data
    Application *app = application();
    mSettings = new QSettings(app->applicationDirPath()+"/../mc.conf",QSettings::IniFormat);

    // load parameters
    loadParam();

    // execute
    switch (state())
    {
    case Service::Help:
        app->exit(help());
    case Service::Vers:
        app->exit(vers());
    case Service::Start:
        execute();
        break;
    case Service::Stop:
        app->exit(terminate());
    default:
        app->exit(undefined());
    }
}

void Service::stop()
{
    if (mDaemon != NULL)
        mDaemon->stop();
}

void Service::loadParam()
{
    // convert arguments
    Application *app = application();
    int    argc = app->argc();
    char **argv = app->argv();

    // init
    mParamList["isHelp" ] = QVariant(false        );
    mParamList["isVers" ] = QVariant(false        );
    mParamList["isStart"] = QVariant(false        );
    mParamList["isStop" ] = QVariant(false        );
    mParamList["server" ] = QVariant(QString()    );
    mParamList["port"   ] = QVariant(0            );
    mParamList["path"   ] = QVariant(QStringList());

    // load config file
    /**/ if (argc == 1 && mSettings != NULL)
    {
        mParamList["isStart"] = QVariant(true);
        mParamList["port"   ] = mSettings->value("port",mParamList.value("port"));
        mParamList["path"   ] = mSettings->value("path",mParamList.value("path"));
    }
    // parse input arguments
    else if (argc > 1)
    {
        // create parameter list
        static struct option longOpt[] = {{"help",   0,0,'h'},
                                          {"version",0,0,'v'},
                                          {"server", 1,0,'s'},
                                          {"port",   1,0,'p'},
                                          {"path",   1,0,'a'},
                                          {"start",  0,0, 0 },
                                          {"stop",   0,0, 0 },
                                          {0,        0,0, 0 }};
        // parameter processing
        int c;
        int optIdx = 0;
        while ((c = getopt_long(argc,argv,"hvs:p:a:0",longOpt,&optIdx)) != -1)
        {
            switch (c) {
            case 0:
                if (strcmp(longOpt[optIdx].name,"start"     ) == 0)
                    mParamList["isStart"   ] = true;
                if (strcmp(longOpt[optIdx].name,"stop"      ) == 0)
                    mParamList["isStop"    ] = true;
                break;
            case 'h':
                mParamList["isHelp"        ] = true;
                break;
            case 'v':
                mParamList["isVers"        ] = true;
                break;
            case 's':
                mParamList["server"        ] = optarg;
                break;
            case 'p':
                mParamList["port"          ] = atoi(optarg);
                break;
            case 'a':
            {
                QStringList path = mParamList.value("path").toStringList() << QString(optarg).split(QRegExp(",|;"));
                path.removeDuplicates();
                mParamList["path"          ] = path;
                break;
            }
            case '?':
                break;
            default:
                printf ("?? getopt returned character code 0%o ??\n", c);
            }
        }
        // not identified arguments
        if (optind < argc) {
            printf ("non-option ARGV-elements: ");
            while (optind < argc)
                printf ("%s ", argv[optind++]);
            printf ("\n");
        }
    }
}

void Service::registerTypes()
{
    qRegisterMetaType<StatusContainer>();
    qRegisterMetaType<ImageContainer>();
    qRegisterMetaType<CommandContainer>();
    qRegisterMetaType<ListContainer>();
}
