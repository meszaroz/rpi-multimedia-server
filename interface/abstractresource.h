#ifndef ABSTRACTRESOURCE_H
#define ABSTRACTRESOURCE_H

#include <QStringList>
#include <QObject>

class AbstractResourceContainer : public QObject
{
    Q_OBJECT
public:
    AbstractResourceContainer(QObject *parent = nullptr);
    virtual ~AbstractResourceContainer();

    virtual QStringList folderList(const QString &path = QString()) const = 0;
};

#endif // ABSTRACTRESOURCE_H
