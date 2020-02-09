#ifndef RESOURCE_H
#define RESOURCE_H

#include <QString>
#include <QDir>
#include <QDirIterator>
#include <QImage>
#include <QFileSystemWatcher>
#include <QMap>

// RESOURCE
class Resource
{
public:
    static bool isResourceDirValid(const QDir &dir);
    static bool dirContainsImage(const QDir &dir);
    static bool dirContainsVideo(const QDir &dir);
    static bool isDirValid(const QDir &dir);

    Resource(const QString &path);
    Resource(const Resource &other);
    ~Resource();

    bool operator ==(const Resource &other);

    bool isValid() const;

    const QDir*      dir()  const;
    const QString*   name() const;
    const QFileInfo* file();
    const QImage*    image();

private:
    static QStringList mSupportedVideoFormats;
    static QStringList mSupportedImageFormats;

    QDir      *mPath;
    QString   *mName;
    QFileInfo *mFile;
    QImage    *mImage;
};

// RESOURCECONTAINER
#include "./interface/abstractresource.h"

typedef QList<Resource> ResourceList;
typedef QMap<QString,ResourceList*> PathInfo;

class ResourceContainer : public AbstractResourceContainer
{
    Q_OBJECT
public:
    ResourceContainer(const QStringList &paths, QObject *parent = nullptr);
    ~ResourceContainer();

    QStringList folderList(const QString &path = QString());
    QStringList pathList() const;

public slots:
    void emitList();

private slots:
    void loadResource(const QString &path = QString());

signals:
    void folderListChanged(const QStringList &list);

private:
    QFileSystemWatcher *mWatcher;
    PathInfo mPathInfo;

    void storePaths(const QStringList &paths);
    void connectSignals();

    QStringList filterPathList(const QString &path);

    void checkPathAndUpdate(const QString &path);
    void removePath(const QString &path);
    void updatePath(const QString &path);

    void createOrClearDataInfo(const QString &key);
    void removeDataInfo(const QString &key);
    void appendPathToDataInfo(const QString &key, const QString &path);
};


#endif // RESOURCE_H
