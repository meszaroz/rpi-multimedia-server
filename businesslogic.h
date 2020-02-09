#ifndef BUSINESSLOGIC_H
#define BUSINESSLOGIC_H

#include <QObject>
#include "./interface/abstractbusinesslogic.h"
#include "./wrapper/mstatuswrapper.h"

class AbstractExecuter;
class AbstractResourceContainer;

class MAbstractWrapper;

// WARNING: takes ownership of executer and resources
class BusinessLogic : public AbstractBusinessLogic
{
    Q_OBJECT
public:
    explicit BusinessLogic(AbstractExecuter *executer, AbstractResourceContainer *resources, QObject *parent = nullptr);
    ~BusinessLogic();

public slots:
    void processData(Buffer *buffer, const qintptr socketId);

signals:
    void sendData(const QByteArray &data, const qintptr socketId);
    void kill();

private:
    AbstractExecuter          *mExecuter;
    AbstractResourceContainer *mResources;

    void broadcastData(const MAbstractWrapper &data, const qintptr socketId);

    void broadcastStatus(const MStatusWrapper &status);
    void sendStatus(const qintptr socketId);
    void setStatus(Buffer *buffer, const qintptr socketId);

    void sendImage(Buffer *buffer, const qintptr socketId);
    void sendPathList(const qintptr socketId);
    void sendCheck(const qintptr socketId);
    void sendError(const QString &message, const qintptr socketId);    
};

#endif // BUSINESSLOGIC_H
