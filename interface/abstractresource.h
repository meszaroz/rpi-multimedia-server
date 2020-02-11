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
    
signals:
    void folderListChanged(const QStringList &list);
};

#endif // ABSTRACTRESOURCE_H
