#include "resource.h"
#include <QDebug>

// RESOURCE - STATIC
QStringList Resource::mSupportedVideoFormats = QStringList() << "*.avi" << "*.m4v" << "*.mp4";
QStringList Resource::mSupportedImageFormats = QStringList() << "*.jpg" << "*.png";

bool Resource::isResourceDirValid(const QDir &dir)
{
    return dirContainsVideo(dir);
}

bool Resource::dirContainsImage(const QDir &dir)
{
    return dir.exists()
            && QDirIterator(dir.absolutePath(),mSupportedImageFormats,QDir::Files).hasNext();
}

bool Resource::dirContainsVideo(const QDir &dir)
{
    return dir.exists()
            && QDirIterator(dir.absolutePath(),mSupportedVideoFormats,QDir::Files).hasNext();
}

bool Resource::isDirValid(const QDir &dir)
{
    return dir.exists()
            && dir.absolutePath() != dir.currentPath()
            && isResourceDirValid(dir);
}

// RESOURCE
Resource::Resource(const QString &path) :
    mPath(isDirValid(QDir(path)) ? new QDir(path) : 0),
    mName(mPath ? new QString(mPath->absolutePath()) : 0),
    mFile(0),
    mImage(0)
{}

Resource::Resource(const Resource &other) :
    mPath(isDirValid(*other.mPath) ? new QDir(*other.mPath) : 0),
    mName(mPath ? new QString(mPath->absolutePath()) : 0),
    mFile(0),
    mImage(0)
 {}

Resource::~Resource()
{
    delete mPath;
    delete mName;
    delete mFile;
    delete mImage;
}

bool Resource::operator ==(const Resource &other)
{
    return mPath && other.mPath && mPath == other.mPath;
}

const QDir* Resource::dir() const
{
    return mPath;
}

const QString* Resource::name() const
{
    return mName;
}

const QFileInfo* Resource::file()
{
    if (!mFile && isValid()) {
        QFileInfoList tmp = mPath->entryInfoList(mSupportedVideoFormats,QDir::Files,QDir::Name);
        if (!tmp.isEmpty()) {
            mFile = new QFileInfo(tmp.first().absoluteFilePath());
        }
    }
    return mFile;
}

const QImage* Resource::image()
{
    if (!mImage && isValid()) {
        QFileInfoList tmp = mPath->entryInfoList(mSupportedImageFormats,QDir::Files,QDir::Name);
        if (!tmp.isEmpty()) {
            mImage = new QImage(tmp.first().absoluteFilePath());
        }
    }
    return mImage;
}

bool Resource::isValid() const
{
    return mPath;
}

// RESOURCECONTAINER
ResourceContainer::ResourceContainer(const QStringList &paths, QObject *parent) :
    AbstractResourceContainer(parent),
    mWatcher(new QFileSystemWatcher)
{
    storePaths(paths);
    connectSignals();
    loadResource();
}

ResourceContainer::~ResourceContainer()
{
    delete mWatcher;
    foreach (const QString &item, pathList())
        removeDataInfo(item);
}

QStringList ResourceContainer::folderList(const QString &path)
{
    QStringList out;
    foreach (const QString &item, filterPathList(path)) {
        const ResourceList *list = mPathInfo.value(item,0);
        if (list)
            foreach (const Resource &resource, *list)
                out.append(*resource.name());
    }
    return out;
}

QStringList ResourceContainer::pathList() const
{
    return mPathInfo.keys();
}

void ResourceContainer::loadResource(const QString &path)
{
    QStringList filter = filterPathList(path);
    if (filter.isEmpty())
        removeDataInfo(path);
    else
        foreach (const QString &item, filter)
            checkPathAndUpdate(item);
    emitList();
}

QStringList ResourceContainer::filterPathList(const QString &path)
{
    QStringList dirList = mWatcher->directories();
    return !path.isEmpty() ?
                dirList.filter(QRegExp(path,Qt::CaseSensitive,QRegExp::Wildcard)) :
                dirList;
}

void ResourceContainer::storePaths(const QStringList &paths)
{
    if (mWatcher)
        foreach (const QString &item, paths)
            if (!item.isEmpty() && QFileInfo(item).isDir())
                mWatcher->addPath(item);
}

void ResourceContainer::checkPathAndUpdate(const QString &path)
{
    if (!QDir(path).exists())
        removePath(path);
    else
        updatePath(path);
}

void ResourceContainer::removePath(const QString &path)
{
    removeDataInfo(path);
    if (mWatcher)
        mWatcher->removePath(path);
}

void ResourceContainer::updatePath(const QString &path)
{
    createOrClearDataInfo(path);
    QDirIterator it(QDir(path).absolutePath());
    while (it.hasNext())
        appendPathToDataInfo(path,it.next());
}

void ResourceContainer::connectSignals()
{
    if (mWatcher && !mWatcher->directories().isEmpty())
        connect(mWatcher,SIGNAL(directoryChanged(QString)),
                this,SLOT(loadResource(QString)));
}

void ResourceContainer::createOrClearDataInfo(const QString &key)
{
    ResourceList *tmp = const_cast<ResourceList*>(mPathInfo.value(key,0));
    if (tmp)
        tmp->clear();
    else
        mPathInfo.insert(key,new ResourceList);
}

void ResourceContainer::removeDataInfo(const QString &key)
{
    if (!key.isEmpty()) {
        ResourceList *tmp = const_cast<ResourceList*>(mPathInfo.value(key,0));
        if (tmp) {
            tmp->clear();
            delete tmp;
        }
        mPathInfo.remove(key);
    }
}

void ResourceContainer::appendPathToDataInfo(const QString &key, const QString &path)
{
    if (!key.isEmpty()) {
        ResourceList *tmp = const_cast<ResourceList*>(mPathInfo.value(key,0));
        if (tmp) {
            Resource resource(path);
            if (resource.isValid() && !tmp->contains(resource))
                tmp->append(resource);
        }
    }
}

void ResourceContainer::emitList()
{
    emit folderListChanged(folderList());
}
