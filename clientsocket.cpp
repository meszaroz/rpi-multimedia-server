#include "clientsocket.h"

ClientSocket::ClientSocket(const qintptr socketId, QObject *parent) :
    QObject(parent)
{
    // init
    serverBuffer = 0;

    // create socket
    this->tcpSocket = new QTcpSocket(this);
    this->socketId  = socketId;   

    // create connections
    connect(tcpSocket,SIGNAL(readyRead()),
            this,     SLOT(receiveData()));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,     SLOT(error(QAbstractSocket::SocketError)));
    connect(tcpSocket,SIGNAL(disconnected()),
            this,     SLOT(finish()));
}

void ClientSocket::start()
{        
    // disable nagle's algorithm
    tcpSocket->setSocketOption(QAbstractSocket::LowDelayOption,1);

    // set socket descriptor or close thread if unable to set
    if (!tcpSocket->setSocketDescriptor(socketId))
        finish();
}

void ClientSocket::close()
{
    if (tcpSocket->state() == QTcpSocket::ConnectedState)
        tcpSocket->disconnectFromHost();
}

void ClientSocket::error(const QAbstractSocket::SocketError &/*error*/)
{    
}

void ClientSocket::finish()
{    
    // emit done
    emit done();
}

void ClientSocket::receiveData()
{    
    // loop until no data is available
    forever
    {
        if (serverBuffer == nullptr) { // buffer isempty
            if (tcpSocket->bytesAvailable() < headerSize())
                break;

            // read header
            QByteArray header = tcpSocket->read(headerSize());
            Buffer tmp = {
                            Invalid,
                            reinterpret_cast<TPointer*>(header.data()),
                            headerSize(),
                            0
                         };
            serverBuffer = createBufferFromHeader(&tmp);
        }

        // check if all data is received
        if (serverBuffer == nullptr || tcpSocket->bytesAvailable() < serverBuffer->size)
            break;

        // receive data, broadcast and prepare for next data
        if (serverBuffer != nullptr) {
            // read data
            QByteArray data = tcpSocket->read(serverBuffer->size);
            memmove(serverBuffer->data, data.data(), serverBuffer->size);

            // broadcast
            emit dataReceived(serverBuffer, socketId);

            // reset buffer -> prepare for next (data deletion is done by the BLoC)
            serverBuffer = nullptr;
        }
    }
}

void ClientSocket::sendData(const QByteArray &data, const qintptr socketId)
{
    if (socketId == -1 || socketId == this->socketId) {
        if (data.length() > 0) {
            tcpSocket->write(data);
        }
    }
}
