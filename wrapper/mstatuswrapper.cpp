#include "./wrapper/mstatuswrapper.h"

MStatusWrapper::MStatusWrapper(Buffer *buffer) :
    mContainer(readStatusContainerFromBuffer(buffer))
{}

MStatusWrapper::MStatusWrapper(const QString &act, const EState &state, const int &dura, const int &pos, const int &vol) :
    mContainer(createStatusContainer())
{
    if (mContainer) {
        mContainer->state = state;
        mContainer->dura  = dura;
        mContainer->pos   = pos;
        mContainer->vol   = vol;
        copyString(&mContainer->act,act.toStdString().c_str());
    }
}

MStatusWrapper::MStatusWrapper(const StatusContainer &cont) :
    mContainer(copyStatusContainer(&cont))
{}

MStatusWrapper::MStatusWrapper(const MStatusWrapper &other) :
    mContainer(other.isValid() ? copyStatusContainer(other.container()) : 0)
{}

MStatusWrapper& MStatusWrapper::operator=(const MStatusWrapper &other)
{
    if (&other != this && other.isValid()) {
        clearStatusContainer(mContainer);
        mContainer = copyStatusContainer(other.container());
    }
    return *this;
}

MStatusWrapper::~MStatusWrapper() {
    clearStatusContainer(mContainer);
}

bool MStatusWrapper::isValid() const {
    return mContainer;
}

const StatusContainer* MStatusWrapper::container() const {
    return mContainer;
}

const Buffer* MStatusWrapper::toBuffer() const {
    if (!mBuffer && isValid()) {
        mBuffer = writeStatusContainerToBuffer(mContainer);
    }
    return mBuffer;
}

