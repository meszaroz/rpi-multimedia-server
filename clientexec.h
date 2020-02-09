#ifndef CLIENTEXEC_H
#define CLIENTEXEC_H

#include <QtCore>
#include <QtNetwork>
#include "./common/mcommon.h"

class QTcpSocket;
class QHostInfo;
class QString;
class QByteArray;
class QDataStream;

int remoteStop(QString serverName, const int &serverPort);

#endif // CLIENTEXEC_H
