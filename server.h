#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QFileInfoList>
#include <QMetaType>
#include "clientsocket.h"

class AbstractBusinessLogic;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(AbstractBusinessLogic *bloc, QObject *parent = 0);
    ~Server();

    bool active() const { return !mStop; }

public slots:
    void stop();

private slots:
    void decreaseCounter();
    void checkStop();

signals:
    void close();

protected:
    void incomingConnection(qintptr socketId) Q_DECL_OVERRIDE;

private:
    AbstractBusinessLogic *mBloc;

    bool mStop;
    int  mCounter;
};

#endif // SERVER_H
