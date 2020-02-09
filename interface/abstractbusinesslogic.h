#ifndef ABSTRACTBUSINESSLOGIC_H
#define ABSTRACTBUSINESSLOGIC_H

#include <QByteArray>
#include <QObject>

class Buffer;

class AbstractBusinessLogic : public QObject
{
    Q_OBJECT
public:
    explicit AbstractBusinessLogic(QObject *parent = nullptr);
    virtual ~AbstractBusinessLogic();

public slots:
    virtual void processData(Buffer *buffer, const qintptr socketId) = 0;

signals:
    void sendData(const QByteArray &data, const qintptr socketId);
    void kill();
};

#endif
