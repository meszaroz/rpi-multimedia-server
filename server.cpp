#include "server.h"
#include "./interface/abstractbusinesslogic.h"

// SERVER
Server::Server(AbstractBusinessLogic *bloc, QObject *parent) :
    QTcpServer(parent),
    mBloc(bloc),
    mStop(false),
    mCounter(0)
{
}

Server::~Server()
{
}

void Server::incomingConnection(qintptr socketId)
{
    if (!mStop)
    {
        // create socket object
        QThread      *thread = new QThread;
        ClientSocket *socket = new ClientSocket(socketId);

        // move object to thread
        socket->moveToThread(thread);

        // increase counter
        mCounter++;

        // connection
        connect(thread,SIGNAL(destroyed()),
                this,  SLOT(decreaseCounter()));
        connect(thread,SIGNAL(finished()),
                thread,SLOT(deleteLater()));
        connect(socket,SIGNAL(destroyed()),
                thread,SLOT(quit()));
        connect(socket,SIGNAL(done()),
                socket,SLOT(deleteLater()));

        connect(this,  SIGNAL(close()),
                socket,SLOT(close()));
        connect(socket,SIGNAL(stop()),
                this,  SLOT(stop()));        
        connect(thread,SIGNAL(started()),
                socket,SLOT(start()));

        // async communication with BLoC
        connect(socket, SIGNAL(dataReceived(Buffer*, const qintptr)),
                mBloc,  SLOT(processData(Buffer*, const qintptr)));
        connect(mBloc,  SIGNAL(sendData(const QByteArray &, const qintptr)),
                socket, SLOT(sendData(const QByteArray &, const qintptr)));

        connect(mBloc, SIGNAL(kill()),
                this, SLOT(stop()));

        // start thread
        thread->start();
    }
}


void Server::decreaseCounter()
{
    mCounter--;
    checkStop();
}

void Server::stop()
{
    mStop = true;
    emit close();
    checkStop();
}

void Server::checkStop()
{    
    if (mCounter == 0 && mStop) {
        QCoreApplication::quit();
    }
}
