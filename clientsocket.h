#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QtCore>
#include <QtNetwork>
#include <QtGlobal>

#include "./common/mcommon.h"
#include "./common/mbuffer.h"

class QTcpSocket;
class QHostAddress;
class QDateTime;
class QFile;
class QString;

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(const qintptr socketId, QObject *parent = 0);
public slots:
    void start();
    void close();
    void error(const QAbstractSocket::SocketError &error);
    void finish();    

    void receiveData();
    void sendData(const QByteArray &data, const qintptr socketId = -1); /* if socketId is -1 no filter is applied */

signals:
    void done();
    void stop();

    void dataReceived(Buffer *buffer, const qintptr socketId);

private:
    QTcpSocket *tcpSocket;
    qintptr     socketId;
    Buffer     *serverBuffer;
};

#endif // CLIENTSOCKET_H
