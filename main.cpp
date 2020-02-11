#include <QApplication>
#include <QtCore>
#include <VLCQtCore/Common.h>
#include "version.h"
#include "server.h"
#include "businesslogic.h"
#include "executer.h"
#include "resource.h"
#include "player.h"
//#include "service.h"

//extern const std::string version = "Multimedia Controller - "+versionNumber+" - "+std::string(__DATE__)+" "+std::string(__TIME__)+"\nZ. Meszaros";

int main(int argc, char *argv[])
{
/*
#if !defined(Q_OS_WIN)
    // QtService stores service settings in SystemScope, which normally require root privileges.
    // To allow testing this example as non-root, we change the directory of the SystemScope settings file.
    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
    qWarning("(Example uses dummy settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
#endif
    Service service(argc, argv);
    return service.exec();
*/
    qRegisterMetaType<qintptr>("qintptr");
    
    QApplication app(argc, argv);
    VlcCommon::setPluginPath(app.applicationDirPath() + "/plugins");

    Player        player;
    BusinessLogic bloc(new Executer(&player), new ResourceContainer(QStringList("/home/pi/Media")));
    Server        daemon(&bloc);

    // Start listening
    daemon.listen(QHostAddress::Any,61486);
    if (!daemon.isListening()) {
        app.exit(2);
    }

    // Show player in full screen
    player.show();
    QTimer::singleShot(0, &player, SLOT(showFullScreen()));

    // Start event loop
    return app.exec();
}


