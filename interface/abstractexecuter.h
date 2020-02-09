#ifndef ABSTRACTEXECUTER_H
#define ABSTRACTEXECUTER_H

#include <QObject>
#include "./wrapper/mstatuswrapper.h"

class AbstractExecuter : public QObject
{
    Q_OBJECT
public:
    explicit AbstractExecuter(QObject *parent = 0);
    virtual ~AbstractExecuter();

    virtual MStatusWrapper status() const = 0;

public slots:
    virtual QString setStatus(const MStatusWrapper &status) = 0;

signals:
    void statusUpdate(const MStatusWrapper &status);
};

#endif // ABSTRACTEXECUTER_H
