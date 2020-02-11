#include <assert.h>
#include "businesslogic.h"
#include "./interface/abstractexecuter.h"
#include "./interface/abstractresource.h"
#include "./wrapper/mlistwrapper.h"
#include "./wrapper/mcheckwrapper.h"
#include "./wrapper/mimagewrapper.h"
#include "./wrapper/mstatuswrapper.h"
#include "./wrapper/merrorwrapper.h"

BusinessLogic::BusinessLogic(AbstractExecuter *executer, AbstractResourceContainer *resources, QObject *parent) :
    AbstractBusinessLogic(parent),
    mExecuter (executer),
    mResources(resources)
{
    assert(executer  != nullptr);
    assert(resources != nullptr);
    
    connect(mResources, SIGNAL(folderListChanged(QStringList)),
            this,       SLOT  (folderListChanged()));
}

BusinessLogic::~BusinessLogic()
{
    delete mExecuter;
    delete mResources;
}

/*
  Command: QPair<uint8_t, QVariant> -> server -> send (status)
  Image: char* -> create ImageContainer -> load image resource -> send image
  Stop: emit stop()
 */
void BusinessLogic::processData(Buffer *buffer, const qintptr socketId)
{
    if (buffer) {
        switch (buffer->mode) {
            case Image: {
                sendImage(buffer, socketId);
                break;
            }
            case List: {
                sendFolderList(socketId);
                break;
            }
            case Check: {
                sendCheck(socketId);
                break;
            }
            case Status: {
                if (!buffer->data) { sendStatus(socketId);        } // GET
                else               { setStatus(buffer, socketId); } // SET
                break;
            }
            case Kill: {
                emit kill();
                break;
            }
            case Error  :
            case Invalid: {
                break;
            }
        }

        // clear buffer
        clearBuffer(buffer);
    }
}

void BusinessLogic::broadcastData(const MAbstractWrapper &data, const qintptr socketId)
{
    if (data.isValid()) {
        const QByteArray *tmp = data.toStreamByteArray();
        if (tmp) {
            emit sendData(*tmp, socketId);
        }
    }
}

void BusinessLogic::broadcastStatus(const MStatusWrapper &status)
{
    broadcastData(status, -1);
}

void BusinessLogic::sendStatus(const qintptr socketId)
{
    MStatusWrapper status = mExecuter->status();
    broadcastData(status, socketId);
}

void BusinessLogic::setStatus(Buffer *buffer, const qintptr socketId)
{
    MStatusWrapper tmp(buffer);
    if (tmp.isValid()) {
        QString error = mExecuter->setStatus(tmp);

        if (error.isEmpty()) { broadcastStatus(tmp);       } // status set successfully -> notify all clients
        else                 { sendError(error, socketId); } // return error to sender
    }
}

void BusinessLogic::sendImage(Buffer *buffer, const qintptr socketId)
{
    broadcastData(MImageWrapper(buffer), socketId);
}

void BusinessLogic::sendFolderList(const qintptr socketId)
{
    QStringList pathList = mResources->folderList();
    broadcastData(MListWrapper(pathList), socketId);
}

void BusinessLogic::sendCheck(const qintptr socketId)
{
    broadcastData(MCheckWrapper(), socketId);
}

void BusinessLogic::sendError(const QString &message, const qintptr socketId)
{
    broadcastData(MErrorWrapper(message), socketId);
}

// broadcast folder list to all clients
void BusinessLogic::folderListChanged()
{
    sendFolderList(-1);
}

