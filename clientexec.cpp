#include "clientexec.h"

extern const std::string version;

// restructure !!!
int remoteStop(QString serverName, const int &serverPort)
{
    // init
    int output = 1;
    QTcpSocket tcpSocket;
    QString domainName = QHostInfo::localDomainName();

    // check server name - if empty, use local host
    serverName = (serverName.isEmpty())?QHostAddress(QHostAddress::LocalHost).toString():serverName;

    //---------------------
    // connect to server
    //---------------------                not IP address                  AND name doesn't contain domain name
    tcpSocket.connectToHost(serverName+((QHostAddress(serverName).isNull() && !serverName.contains(domainName))?"."+domainName:""),serverPort);

    // wait for connection
    if (tcpSocket.waitForConnected())
    {
    //---------------------
    // create data stream -> to function
    //---------------------        
        Buffer *buffer = createBuffer(Kill,0);

        // to function OR wrapper class
        TSize size;
        TPointer *data = bufferToStreamData(buffer,&size);
        clearBuffer(buffer);

    //---------------------
    // send data
    //---------------------        
        tcpSocket.write((char*)data,size);

        // wait for written
        if (tcpSocket.waitForBytesWritten())
            output = 0;

    //---------------------
    // disconnect
    //---------------------
        tcpSocket.disconnectFromHost();
        if (tcpSocket.state() != QAbstractSocket::UnconnectedState)
            tcpSocket.waitForDisconnected();
    }

    // return
    return output;
}
